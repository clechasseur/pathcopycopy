// PluginUtils.cpp
// (c) 2011-2020, Charles Lechasseur
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdafx.h>
#include <PluginUtils.h>
#include <PathCopyCopyPluginsRegistry.h>
#include <PathCopyCopySettings.h>
#include <StringUtils.h>
#include <StWSAStartup.h>
#include <StAddrInfo.h>
#include <StHandle.h>

#include <DefaultPlugin.h>

#include <memory>
#include <sstream>

#include <comutil.h>
#include <lm.h>

#include <coveo/enumerable.h>
#include <coveo/linq.h>


namespace
{
    constexpr DWORD         INITIAL_BUFFER_SIZE         = 1024;     // Initial size of buffer used to fetch UNC name.
    constexpr DWORD         MAX_REG_KEY_NAME_SIZE       = 255;      // Max size of a registry key's name.

    const wchar_t* const    SHARES_KEY_NAME             = L"SYSTEM\\CurrentControlSet\\Services\\Lanmanserver\\Shares"; // Name of key storing network shares
    const wchar_t* const    SHARE_PATH_VALUE            = L"Path=";     // Part of a share key's value containing the share path.
    constexpr wchar_t       HIDDEN_SHARE_SUFFIX         = L'$';         // Suffix used for hidden shares; we will not consider them unless specified.

    const wchar_t* const    HIDDEN_DRIVE_SHARES_REGEX   = L"^([A-Za-z])\\:((\\\\|/).*)$";   // Regex used to convert hidden drive shares.
    const wchar_t* const    HIDDEN_DRIVE_SHARES_FORMAT  = L"$1$$$2";                        // Format string used to convert hidden drive shares.

    constexpr ULONG         REG_BUFFER_CHUNK_SIZE = 512;        // Size of chunks allocated to read the registry.

    const wchar_t* const    LOCAL_DRIVE_SYMLINK_PREFIX  = L"\\\\?\\";   // Prefix for local paths when fetching symlink targets.
    const wchar_t* const    UNC_DRIVE_SYMLINK_PREFIX    = L"?\\UNC\\";  // Prefix for UNC paths when fetching symlink targets.
} // anonymous namespace

namespace PCC
{
    // Static members of PluginUtils

#pragma warning(push)
#pragma warning(disable: 26426)

    std::mutex      PluginUtils::s_Lock;
    std::wstring    PluginUtils::s_ComputerName;
    bool            PluginUtils::s_HasComputerName = false;

#pragma warning(pop)

    //
    // Determines if the given path points to a directory or file.
    //
    // @param p_Path Path to check.
    // @return true if path points to a directory.
    //
    bool PluginUtils::IsDirectory(const std::wstring& p_Path) noexcept
    {
        const DWORD attribs = ::GetFileAttributesW(p_Path.c_str());
        return attribs != INVALID_FILE_ATTRIBUTES &&
               (attribs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
    }

    //
    // Given a path to a file or folder, will return the path to its
    // parent folder. Ex:
    // C:\Foo\Bar.txt => C:\Foo
    //
    // @param p_rPath Complete path on input, parent's path on output.
    // @return true if we did find a parent path and returned it.
    //
    bool PluginUtils::ExtractFolderFromPath(std::wstring& p_rPath)
    {
        // Find the last delimiter in the path and truncate path
        // as appropriate to return only the parent's path.
        std::wstring::size_type lastDelimPos = p_rPath.find_last_of(L"/\\");
        const bool found = (lastDelimPos != std::wstring::npos);
        if (found) {
            // We found a delimiter, clear everything after that
            // (and the delimiter as well). Exception: if we're left
            // with only a drive letter, keep the delimiter.
            if (lastDelimPos <= 2) {
                ++lastDelimPos;
            }
            p_rPath.erase(lastDelimPos);
        }
        return found;
    }

    //
    // Returns a list of a path's parents, in reverse order.
    // So if, for instance, is called with "C:\Program Files\Path Copy Copy\PCC32.dll", would return
    //
    // C:\Program Files\Path Copy Copy
    // C:\Program Files
    // C:\
    //
    // @param p_Path Path to enumerate the parents of.
    // @return List of path's parents.
    //
    std::vector<std::wstring> PluginUtils::EnumerateParents(std::wstring p_Path)
    {
        std::vector<std::wstring> vParents;
        for (;;) {
            const auto prevPath = p_Path;
            const auto hasParent = ExtractFolderFromPath(p_Path);
            if (!hasParent || p_Path == prevPath) {
                break;
            }
            vParents.emplace_back(p_Path);
        }
        return vParents;
    }

    //
    // If the provided path or one of its parents points to a symbolic link,
    // follow the symlink and return the path to its target.
    //
    // @param p_rPath Path to follow symlink for, if required.
    // @return true if p_rPath or one of its parents pointed to a symlink.
    //
    bool PluginUtils::FollowSymlinkIfRequired(std::wstring& p_rPath)
    {
        using namespace coveo::linq;

        // Check if path or one of its parent points to a symlink.
        const auto isSymlink = [](const std::wstring& path) noexcept {
            const auto attributes = ::GetFileAttributesW(path.c_str());
            return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
        };
        const bool symlink = from(coveo::enumerate_one(p_rPath))
                           | concat(EnumerateParents(p_rPath))
                           | any(isSymlink);
        if (symlink) {
            // In order to follow the symlink, we need a handle.
            DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
            if ((::GetFileAttributesW(p_rPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                // Need this flag to open directory handles according to MSDN
                flagsAndAttributes |= FILE_FLAG_BACKUP_SEMANTICS;
            }
            StHandle hFile = ::CreateFileW(p_rPath.c_str(),
                                           GENERIC_READ,
                                           FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                           nullptr,
                                           OPEN_EXISTING,
                                           flagsAndAttributes,
                                           nullptr);
            if (hFile != nullptr) {
                std::wstring finalPath(MAX_PATH + 1, L'\0');
                const auto finalPathRes = ::GetFinalPathNameByHandleW(hFile,
                                                                      &*finalPath.begin(),
                                                                      MAX_PATH,
                                                                      FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
                if (finalPathRes != 0) {
                    p_rPath = finalPath.c_str();

                    // Fetching symlink target probably left us with a weird path, fix it
                    // because Explorer can't handle paths with \\?\ in them.
                    StringUtils::ReplaceAll(p_rPath, UNC_DRIVE_SYMLINK_PREFIX, L"");
                    StringUtils::ReplaceAll(p_rPath, LOCAL_DRIVE_SYMLINK_PREFIX, L"");
                }
            }
        }
        return symlink;
    }

    //
    // Checks if the given path is a UNC path in the form
    // \\server\share[\...]
    //
    // @param p_FilePath Path to verify.
    // @return true if the path is a UNC path.
    //
    bool PluginUtils::IsUNCPath(const std::wstring& p_FilePath) noexcept
    {
        return p_FilePath.find(L"\\\\") == 0 && p_FilePath.find(L'\\', 2) > 2;
    }

    //
    // Checks if the given file resides on a mapped network drive.
    // If it does, returns its corresponding network path.
    // Ex: N:\Data\File.txt -> \\server\share\Data\File.txt
    //
    // @param p_rFilePath Local file path. Upon exit, will contain network path.
    // @return true if the file was on a mapped network drive and we fetched its network path.
    //
    bool PluginUtils::GetMappedDriveFilePath(std::wstring& p_rFilePath)
    {
        // WNetGetUniversalName allows us to get the network path
        // if file is on a mapped drive.
        bool converted = false;
        DWORD bufferSize = INITIAL_BUFFER_SIZE;
        std::vector<char> vBuffer;
        DWORD ret = ERROR_MORE_DATA;
        while (ret == ERROR_MORE_DATA) {
            vBuffer.resize(bufferSize, '\0');
            ret = ::WNetGetUniversalNameW(p_rFilePath.c_str(),
                                          UNIVERSAL_NAME_INFO_LEVEL,
                                          vBuffer.data(),
                                          &bufferSize);
        }
        if (ret == NO_ERROR) {
            // Got UNC path, return it.
#pragma warning(suppress: 26490) // No choice but to use reinterpret_cast here, can't change the Win32 API
            p_rFilePath.assign(reinterpret_cast<UNIVERSAL_NAME_INFOW*>(vBuffer.data())->lpUniversalName);
            converted = true;
        }
        return converted;
    }

    //
    // Checks if the given file resides in a directory in a network share.
    // If it does, returns its corresponding network path.
    // Ex: C:\SharedDir\File.txt -> \\thiscomputer\SharedDir\File.txt
    //
    // @param p_rFilePath Local file path. Upon exit, will contain network path.
    // @param p_UseHiddenShares Whether to consider hidden shares when looking for valid shares.
    // @return true if the file was in a network share and we fetched its network path.
    //
    bool PluginUtils::GetNetworkShareFilePath(std::wstring& p_rFilePath,
                                              const bool p_UseHiddenShares)
    {
        bool converted = false;

        // Scan registry to see if we can find a share that contains this path.
        // Shares are stored in multi-string registry values in the Lanmanserver service keys.
        ATL::CRegKey shareKey;
        if (shareKey.Open(HKEY_LOCAL_MACHINE, SHARES_KEY_NAME, KEY_READ) == ERROR_SUCCESS) {
            // Iterate registry values to check each share.
            std::wstring valueName(MAX_REG_KEY_NAME_SIZE + 1, L'\0');
            std::wstring multiStringValue;
            std::wstring path;
            LONG ret = 0;
            DWORD i = 0;
            do {
                DWORD valueNameSize = MAX_REG_KEY_NAME_SIZE;
                DWORD valueType = 0;
                ret = ::RegEnumValue(shareKey, i, &*valueName.begin(), &valueNameSize, nullptr, &valueType, nullptr, nullptr);
                if (ret == ERROR_SUCCESS && valueType == REG_MULTI_SZ) {
                    // Get the multi-string values.
                    ULONG bufferSize = INITIAL_BUFFER_SIZE;
                    std::vector<wchar_t> vBuffer;
                    do {
                        vBuffer.resize(bufferSize, L'\0');
                        ret = shareKey.QueryMultiStringValue(valueName.c_str(), vBuffer.data(), &bufferSize);
                    } while (ret == ERROR_MORE_DATA);
                    if (ret == ERROR_SUCCESS) {
                        // Make sure this is not a hidden share (unless we use them).
                        if (valueNameSize != 0 && (p_UseHiddenShares || valueName.at(valueNameSize - 1) != HIDDEN_SHARE_SUFFIX)) {
                            // Find the "Path=" part of the mult-string. This contains the share path.
                            multiStringValue.assign(vBuffer.data(), bufferSize);
                            path = GetMultiStringLineBeginningWith(multiStringValue, SHARE_PATH_VALUE);
                            if (!path.empty()) {
                                // Check if our path is in that share.
                                if (p_rFilePath.find(path) == 0) {
                                    // Success: this is a share that contains our path.
                                    // Replace the start of the path with the computer and share name.
                                    std::wstringstream ss;
                                    ss << L"\\\\" << GetLocalComputerName() << L"\\" << valueName.c_str();
                                    if (*path.rbegin() == L'\\' || *path.rbegin() == L'/') {
                                        // The substr below will remove the terminator if the share path
                                        // ends with one (for example, for drives' administrative shares).
                                        // We'll have to add an extra one manually.
                                        ss << L'\\';
                                    }
                                    ss << p_rFilePath.substr(path.size());
                                    p_rFilePath = ss.str();
                                    converted = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                // Go to next share.
                ++i;
            } while (ret == ERROR_SUCCESS);
        }

        return converted;
    }

    //
    // Checks if the given file resides in a directory on a local drive.
    // If it does, returns its corresponding network path using a hidden drive share.
    // Ex: C:\Dir\File.txt -> \\thiscomputer\C$\Dir\File.txt
    //
    // @param p_rFilePath Local file path. Upon exit, will contain network path.
    // @return true if the file was on a local drive and we fetched its network path.
    //
    bool PluginUtils::GetHiddenDriveShareFilePath(std::wstring& p_rFilePath)
    {
        // Try to perform the replacement in one shot. If it works, the path is converted.
        bool converted = false;

        try {
            const std::wregex hiddenDriveShareRegex(HIDDEN_DRIVE_SHARES_REGEX, std::regex_constants::ECMAScript);
            const std::wstring replaced = std::regex_replace(p_rFilePath, hiddenDriveShareRegex, HIDDEN_DRIVE_SHARES_FORMAT);
            if (!replaced.empty()) {
                std::wstringstream ss;
                ss << L"\\\\" << GetLocalComputerName() << L"\\" << replaced;
                p_rFilePath = ss.str();
                converted = true;
            }
        } catch (const std::regex_error&) {
            // Didn't work.
        }

        return converted;
    }

    //
    // Replaces the hostname in the given UNC path with a
    // fully-qualified domain name (FQDN).
    //
    // @param p_rFilePath UNC path. Upon exit, host name will have been replaced.
    //
    void PluginUtils::ConvertUNCHostToFQDN(std::wstring& p_rFilePath)
    {
        // Find hostname in file path.
        std::wstring hostname, restOfPath;
        if (p_rFilePath.substr(0, 2) == L"\\\\") {
            auto withoutPrefix = p_rFilePath.substr(2);
            const auto delimPos = withoutPrefix.find_first_of(L"\\/");
            if (delimPos != std::wstring::npos) {
                hostname = withoutPrefix.substr(0, delimPos);
                restOfPath = withoutPrefix.substr(delimPos);
            }
        }
        if (!hostname.empty()) {
            // First initialize Winsock if it's not already initialized in the process.
            StWSAStartup wsaStartup;
            if (wsaStartup.Started()) {
                // Try fetching info for the hostname
                addrinfoW hints;
                ::ZeroMemory(&hints, sizeof(hints));
                hints.ai_flags = AI_CANONNAME;
                hints.ai_family = AF_UNSPEC;
                StAddrInfo addrinfo;
                if (::GetAddrInfoW(hostname.c_str(), nullptr, &hints, &addrinfo)) {
                    // Rebuild the path by replacing the hostname with its FQDN
                    std::wstringstream wss;
                    wss << L"\\\\"
                        << addrinfo->ai_canonname
                        << restOfPath;
                    p_rFilePath = wss.str();
                }
            }
        }
    }

    //
    // Returns the name of the local computer.
    //
    // @return Name of local computer.
    //
    const std::wstring& PluginUtils::GetLocalComputerName()
    {
        if (!s_HasComputerName) {
            std::lock_guard<std::mutex> lock(s_Lock);
            if (!s_HasComputerName) {
                std::wstring name(MAX_COMPUTERNAME_LENGTH + 1, L'\0');
                DWORD length = gsl::narrow<DWORD>(name.size());
                if (::GetComputerNameW(&*name.begin(), &length) != FALSE) {
#pragma warning(suppress: 26493) // Compiler thinks size_t{length} is a C-style cast
                    if (::_wcslwr_s(&*name.begin(), size_t{length} + 1) == 0) {
                        s_ComputerName = name.c_str();
                    }
                }
                s_HasComputerName = true;
            }
        }
        return s_ComputerName;
    }

    //
    // Reads the content of a string registry value and returns it in
    // a std::wstring so that it's easier to manage. Will take care of
    // reallocating buffers as needed.
    //
    // @param p_Key Key containing the value to read.
    // @param p_pValueName Name of value.
    // @param p_rValue Upon exit, will contain the value.
    // @return Error code, or ERROR_SUCCESS if all goes well.
    //
    long PluginUtils::ReadRegistryStringValue(const RegKey& p_Key,
                                              const wchar_t* const p_pValueName,
                                              std::wstring& p_rValue)
    {
        // Clear the content to assume value doesn't exist.
        p_rValue.clear();

        // Loop until we are able to read the value.
        long lRes = ERROR_MORE_DATA;
        ULONG curSize = 0;
        while (lRes == ERROR_MORE_DATA) {
            curSize += REG_BUFFER_CHUNK_SIZE;
            std::vector<wchar_t> vBuffer(curSize, L'\0');
            DWORD valueType = REG_SZ;
            DWORD curSizeInBytes = curSize * sizeof(wchar_t);
            lRes = p_Key.QueryValue(p_pValueName, &valueType, vBuffer.data(), &curSizeInBytes);
            if (lRes == ERROR_SUCCESS) {
                // Make sure it is a string.
                if (valueType == REG_SZ && (curSizeInBytes % sizeof(wchar_t)) == 0) {
                    // Success, copy resulting string.
                    p_rValue.assign(vBuffer.data());
                } else {
                    lRes = ERROR_INVALID_DATATYPE;
                }
            }
        }

        return lRes;
    }

    //
    // Given a multi-line string read from a REG_MULTI_SZ registry value,
    // finds a line that begins with a given prefix and returns it.
    //
    // @param p_MultiStringValue The multi-string value.
    // @param p_Prefix Prefix of line to look for.
    // @return The entire matching line (excluding prefix), or an empty string if line is not found.
    //
    std::wstring PluginUtils::GetMultiStringLineBeginningWith(const std::wstring& p_MultiStringValue,
                                                              const std::wstring& p_Prefix)
    {
        std::wstring line;

        // Multi-line values contain embedded NULLs to separate the lines.
        std::wstring::size_type offset = 0;
        do {
            const std::wstring::size_type endOffset = p_MultiStringValue.find_first_of(L'\0', offset);
            if (endOffset == offset) {
                // We're at the end of the value.
                break;
            }

            if (p_MultiStringValue.find(p_Prefix, offset) == offset) {
                // This is the line we're looking for.
                const std::wstring::size_type prefixSize = p_Prefix.size();
                line = p_MultiStringValue.substr(offset + prefixSize, endOffset - offset - prefixSize);
                break;
            } else {
                // Not the line, go to next one.
                offset = endOffset + 1;
            }
        } while (offset < p_MultiStringValue.size());

        return line;
    }

    //
    // Converts a string containing a list of plugin unique identifiers
    // to a vector of GUID structs.
    //
    // @param p_PluginIdsAsString String containing the plugin IDs.
    // @param p_Separator Character used to separate the plugin IDs in the string.
    // @return Vector of plugin IDs as GUID structs.
    //
    GUIDV PluginUtils::StringToPluginIds(std::wstring p_PluginIdsAsString,
                                         const wchar_t p_Separator)
    {
        // Assume there are no plugin IDs.
        GUIDV vPluginIds;

        // First split the string.
        WStringV vStringParts = StringUtils::Split(std::move(p_PluginIdsAsString), p_Separator);

        // Scan parts and convert to GUIDs.
        GUID onePluginId = { 0 };
        for (const std::wstring& stringPart : vStringParts) {
            if (SUCCEEDED(::CLSIDFromString(stringPart.c_str(), &onePluginId))) {
                vPluginIds.push_back(onePluginId);
            }
        }

        return vPluginIds;
    }

    //
    // Converts a string containing a list of unsigned integers to a vector.
    //
    // @param p_UInt32sAsString String containing the integers.
    // @param p_Separator Character used to separate the integers in the string.
    // @return Vector of unsigned integers.
    //
    UInt32V PluginUtils::StringToUInt32s(std::wstring p_UInt32sAsString,
                                         const wchar_t p_Separator)
    {
        // Assume there are no integers.
        UInt32V vUInt32s;

        // First split the string.
        WStringV vStringParts = StringUtils::Split(std::move(p_UInt32sAsString), p_Separator);

        // Scan parts and convert to integers.
        for (const std::wstring& stringPart : vStringParts) {
            std::wistringstream wis(stringPart.c_str());
            uint32_t integer = 0;
            wis >> integer;
            vUInt32s.push_back(integer);
        }

        return vUInt32s;
    }

    //
    // Converts a list of plugin IDs to a string containing them.
    // This is the opposite of StringToPluginIds.
    //
    // @param p_vPluginIds List of plugin IDs to convert.
    // @param p_Separator Character used to separate the plugin IDs in the string.
    // @return String with merged plugin IDs.
    //
    std::wstring PluginUtils::PluginIdsToString(const GUIDV& p_vPluginIds,
                                                const wchar_t p_Separator)
    {
        std::wstring guidBuffer(40, L'\0'); // See StringFromGUID2 in MSDN
        const auto guidToString = [&](const GUID& p_GUID) -> std::wstring {
            return (::StringFromGUID2(p_GUID, &*guidBuffer.begin(), 40) != 0) ? guidBuffer.c_str() : L"";
        };

        // Insert the first plugin ID without separator.
        std::wostringstream wos;
        if (!p_vPluginIds.empty()) {
            wos << guidToString(p_vPluginIds.front());

            // Insert the other elements with separators.
            for (auto it = p_vPluginIds.cbegin() + 1; it != p_vPluginIds.cend(); ++it) {
                wos << p_Separator << guidToString(*it);
            }
        }

        // Return resulting string.
        return wos.str();
    }

    //
    // Converts a list of unsigned integers to a string containing them.
    // This is the opposite of StringToUInts.
    //
    // @param p_vUInt32s List of unsigned integers to convert.
    // @param p_Separator Character used to separate the integers in the string.
    // @return String with merged unsigned integers.
    //
    std::wstring PluginUtils::UInt32sToString(const UInt32V& p_vUInt32s,
                                              const wchar_t p_Separator)
    {
        // Insert the first integer without separator.
        std::wostringstream wos;
        if (!p_vUInt32s.empty()) {
            wos << p_vUInt32s.front();

            // Insert the other elements with separators.
            for (auto it = p_vUInt32s.cbegin() + 1; it != p_vUInt32s.cend(); ++it) {
                wos << p_Separator << *it;
            }
        }

        // Return resulting string.
        return wos.str();
    }

    //
    // Checks in the Path Copy Copy settings if a specific plugin
    // is shown at all, whether in the main menu or in the submenu.
    //
    // @param p_Settings Object to access settings.
    // @param p_PluginId ID of plugin to look for in the submenu.
    // @return true if the plugin is displayed.
    //
    bool PluginUtils::IsPluginShown(const Settings& p_Settings,
                                    const GUID& p_PluginId)
    {
        // Assume it's shown.
        bool shown = true;

        // Get list of plugins in main menu and submenu from settings.
        GUIDV vPluginsInMainMenu, vPluginsInSubmenu;
        if (!p_Settings.GetMainMenuPluginDisplayOrder(vPluginsInMainMenu)) {
            // Not specified, use the default plugin.
            vPluginsInMainMenu.push_back(Plugins::DefaultPlugin().Id());
        }
        if (!p_Settings.GetSubmenuPluginDisplayOrder(vPluginsInSubmenu)) {
            // Not specified, use default plugins in default order.
            PluginSPV vspPlugins = PluginsRegistry::GetPluginsInDefaultOrder(
                &p_Settings, &p_Settings, PipelinePluginsOptions::FetchPipelinePlugins);
            for (const PluginSP& spPlugin : vspPlugins) {
                vPluginsInSubmenu.push_back(spPlugin->Id());
            }
        }

        // Scan lists to find our plugin.
        const auto isOurPlugin = [&](const GUID& p_Id) noexcept -> bool {
            return ::IsEqualGUID(p_Id, p_PluginId) != FALSE;
        };
        if (std::find_if(vPluginsInMainMenu.cbegin(), vPluginsInMainMenu.cend(), isOurPlugin) == vPluginsInMainMenu.cend() &&
            std::find_if(vPluginsInSubmenu.cbegin(), vPluginsInSubmenu.cend(), isOurPlugin) == vPluginsInSubmenu.cend()) {

            // The plugins is not shown.
            shown = false;
        }

        return shown;
    }

} // namespace PCC
