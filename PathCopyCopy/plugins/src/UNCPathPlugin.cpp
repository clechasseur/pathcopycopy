// UNCPathPlugin.cpp
// (c) 2008-2020, Charles Lechasseur
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

#include "stdafx.h"
#include "UNCPathPlugin.h"

#include <ArrayAutoPtr.h>
#include "resource.h"

#include <lm.h>

#include <sstream>


namespace
{
    const DWORD         INITIAL_BUFFER_SIZE     = 1024;     // Initial size of buffer used to fetch UNC name.
    const DWORD         MAX_REG_KEY_NAME_SIZE   = 255;      // Max size of a registry key's name.

    const std::wstring  SHARES_KEY_NAME     = L"SYSTEM\\CurrentControlSet\\Services\\Lanmanserver\\Shares"; // Name of key storing network shares
    const std::wstring  SHARE_PATH_VALUE    = L"Path=";     // Part of a share key's value containing the share path.
    const wchar_t       HIDDEN_SHARE_SUFFIX = L'$';         // Suffix used for hidden shares; we will not consider them.

    // Plugin unique ID: {EA24BCC3-35F5-46b9-A2A5-A12A0AED2D28}
    const GUID          UNC_PATH_PLUGIN_ID  = { 0xea24bcc3, 0x35f5, 0x46b9, { 0xa2, 0xa5, 0xa1, 0x2a, 0xa, 0xed, 0x2d, 0x28 } };

} // anonymous namespace

namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        UNCPathPlugin::UNCPathPlugin()
            : LongPathPlugin(IDS_UNC_PATH_PLUGIN_DESCRIPTION, IDS_UNC_PATH_PLUGIN_HINT),
              m_ComputerName()
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& UNCPathPlugin::Id() const
        {
            return UNC_PATH_PLUGIN_ID;
        }

        //
        // Returns the UNC path of the specified file.
        //
        // @param p_File File path.
        // @returns UNC path if file has one, otherwise its long path.
        //
        std::wstring UNCPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get long path.
            std::wstring path = LongPathPlugin::GetPath(p_File);

            // Try to get path on mapped network drive.
            std::wstring newPath = path;
            bool converted = GetMappedDriveFilePath(newPath);

            // If it wasn't on a mapped drive, check if it's in a network share.
            if (!converted) {
                converted = GetNetworkShareFilePath(newPath);
            }

            if (converted) {
                path = newPath;
            }
            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        UNCPathPlugin::UNCPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                     const unsigned short p_HelpTextStringResourceID)
            : LongPathPlugin(p_DescriptionStringResourceID, p_HelpTextStringResourceID),
              m_ComputerName()
        {
        }

        //
        // Checks if the given file resides on a mapped network drive.
        // If it does, returns its corresponding network path.
        // Ex: N:\Data\File.txt -> \\server\share\Data\File.txt
        //
        // @param p_rFilePath Local file path. Upon exit, will contain network path.
        // @returns true if the file was on a mapped network drive and we fetched its network path.
        //
        bool UNCPathPlugin::GetMappedDriveFilePath(std::wstring& p_rFilePath) const
        {
            // WNetGetUniversalName allows us to get the network path
            // if file is on a mapped drive.
            bool converted = false;
            DWORD bufferSize = INITIAL_BUFFER_SIZE;
            ArrayAutoPtr<char> apBuffer;
            DWORD ret = ERROR_MORE_DATA;
            while (ret == ERROR_MORE_DATA) {
                apBuffer = new char[bufferSize];
                ret = ::WNetGetUniversalNameW(p_rFilePath.c_str(),
                                              UNIVERSAL_NAME_INFO_LEVEL,
                                              apBuffer.Get(),
                                              &bufferSize);
            }
            if (ret == NO_ERROR) {
                // Got UNC path, return it.
                p_rFilePath.assign(reinterpret_cast<UNIVERSAL_NAME_INFO*>(apBuffer.Get())->lpUniversalName);
                converted = true;
            }
            return converted;
        }

        //
        // Checks if the given file resides in a directory in a public network share.
        // If it does, returns its corresponding network path.
        // Ex: C:\SharedDir\File.txt -> \\thiscomputer\SharedDir\File.txt
        //
        // @param p_rFilePath Local file path. Upon exit, will contain network path.
        // @returns true if the file was in a network share and we fetched its network path.
        //
        bool UNCPathPlugin::GetNetworkShareFilePath(std::wstring& p_rFilePath) const
        {
            bool converted = false;

            // Scan registry to see if we can find a share that contains this path.
            // Shares are stored in multi-string registry values in the Lanmanserver service keys.
            CRegKey shareKey;
            if (shareKey.Open(HKEY_LOCAL_MACHINE, SHARES_KEY_NAME.c_str(), KEY_READ) == ERROR_SUCCESS) {
                // Iterate registry values to check each share.
                wchar_t valueName[MAX_REG_KEY_NAME_SIZE + 1];
                std::wstring multiStringValue;
                std::wstring path;
                LONG ret = 0;
                DWORD i = 0;
                do {
                    DWORD valueNameSize = MAX_REG_KEY_NAME_SIZE;
                    DWORD valueType = 0;
                    ret = ::RegEnumValue(shareKey, i, valueName, &valueNameSize, 0, &valueType, 0, 0);
                    if (ret == ERROR_SUCCESS && valueType == REG_MULTI_SZ) {
                        // Get the multi-string values.
                        ULONG bufferSize = INITIAL_BUFFER_SIZE;
                        ArrayAutoPtr<wchar_t> buffer;
                        do {
                            buffer = new wchar_t[bufferSize];
                            ret = shareKey.QueryMultiStringValue(valueName, buffer.Get(), &bufferSize);
                        } while (ret == ERROR_MORE_DATA);
                        if (ret == ERROR_SUCCESS) {
                            // Make sure this is not a hidden share.
                            if (valueNameSize != 0 && valueName[valueNameSize - 1] != HIDDEN_SHARE_SUFFIX) {
                                // Find the "Path=" part of the mult-string. This contains the share path.
                                multiStringValue.assign(buffer.Get(), bufferSize);
                                GetMultiStringLineBeginningWith(multiStringValue, SHARE_PATH_VALUE, path);
                                if (!path.empty()) {
                                    // Check if our path is in that share.
                                    PCCDEBUGCODE(std::wstring::size_type pathPos = p_rFilePath.find(path));
                                    if (p_rFilePath.find(path) == 0) {
                                        // Success: this is a share that contains our path.
                                        // Replace the start of the path with the computer and share name.
                                        std::wstringstream ss;
                                        ss << L"\\\\" << GetLocalComputerName() << L"\\" << valueName
                                           << p_rFilePath.substr(path.size());
                                        std::wstring newPath = ss.str();
                                        p_rFilePath = newPath;
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
        // Returns the name of the local computer.
        //
        // @returns Name of local computer.
        //
        const std::wstring& UNCPathPlugin::GetLocalComputerName() const
        {
            if (m_ComputerName.empty()) {
                DWORD length = MAX_COMPUTERNAME_LENGTH + 1;
                wchar_t name[MAX_COMPUTERNAME_LENGTH + 1];
                if (::GetComputerNameW(name, &length) != FALSE) {
                    if (::_wcslwr_s(name, length + 1) == 0) {
                        m_ComputerName = name;
                    }
                }
            }
            return m_ComputerName;
        }

        //
        // Given a multi-line string read from a REG_MULTI_SZ registry value,
        // finds a line that begins with a given prefix and returns it.
        //
        // @param p_MultiStringValue The multi-string value.
        // @param p_Prefix Prefix of line to look for.
        // @param p_rLine Returns the entire matching line (excluding prefix), or an empty string if line is not found.
        //
        void UNCPathPlugin::GetMultiStringLineBeginningWith(const std::wstring& p_MultiStringValue,
                                                            const std::wstring& p_Prefix,
                                                            std::wstring&       p_rLine) const
        {
            bool found = false;
            
            // Multi-line values contain embedded NULLs to separate the lines.
            std::wstring::size_type offset = 0;
            do {
                std::wstring::size_type endOffset = p_MultiStringValue.find_first_of(L'\0', offset);
                PCCDEBUGCODE(const wchar_t* pCur = &p_MultiStringValue[offset]);
                if (endOffset == offset) {
                    // We're at the end of the value.
                    break;
                }

                if (p_MultiStringValue.find(p_Prefix, offset) == offset) {
                    // This is the line we're looking for.
                    std::wstring::size_type prefixSize = p_Prefix.size();
                    p_rLine = p_MultiStringValue.substr(offset + prefixSize, endOffset - offset - prefixSize);
                    found = true;
                    break;
                } else {
                    // Not the line, go to next one.
                    offset = endOffset + 1;
                }
            } while (offset < p_MultiStringValue.size());

            if (!found) {
                p_rLine.empty();
            }
        }

    } // namespace Plugins

} // namespace PCC
