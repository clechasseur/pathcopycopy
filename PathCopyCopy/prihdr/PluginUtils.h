// PluginUtils.h
// (c) 2011-2018, Charles Lechasseur
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

#pragma once

#include "PathCopyCopyPrivateTypes.h"
#include "RegKey.h"

#include <mutex>
#include <regex>
#include <string>

#include <windows.h>


namespace PCC
{
    //
    // PluginUtils
    //
    // Static class with methods designed to help plugin writers.
    //
    class PluginUtils final
    {
    public:
                        PluginUtils() = delete;
                        ~PluginUtils() = delete;

        static bool     ExtractFolderFromPath(std::wstring& p_rPath);

        static bool     IsUNCPath(const std::wstring& p_FilePath);
        static bool     GetMappedDriveFilePath(std::wstring& p_rFilePath);
        static bool     GetNetworkShareFilePath(std::wstring& p_rFilePath,
                                                const bool p_UseHiddenShares);
        static bool     GetHiddenDriveShareFilePath(std::wstring& p_rFilePath);
        static const std::wstring&
                        GetLocalComputerName();

        static long     ReadRegistryStringValue(const RegKey& p_Key,
                                                const wchar_t* const p_pValueName,
                                                std::wstring& p_rValue);

        static std::wstring
                        GetMultiStringLineBeginningWith(const std::wstring& p_MultiStringValue,
                                                        const std::wstring& p_Prefix);
        static GUIDV    StringToPluginIds(std::wstring& p_rPluginIdsAsString,
                                          const wchar_t p_Separator);
        static UInt32V  StringToUInt32s(std::wstring& p_rUInt32sAsString,
                                        const wchar_t p_Separator);
        static std::wstring
                        PluginIdsToString(const GUIDV& p_vPluginIds,
                                          const wchar_t p_Separator);
        static std::wstring
                        UInt32sToString(const UInt32V& p_vUInt32s,
                                        const wchar_t p_Separator);

        static bool     IsPluginShown(const Settings& p_Settings,
                                      const GUID& p_PluginId);

    private:
        static std::mutex
                        s_Lock;                     // Mutex to protect member access.
        static std::wstring
                        s_ComputerName;             // Name of local computer.
        static bool     s_HasComputerName;          // Whether we have precomputed the computer name.
        static std::wregex
                        s_HiddenDriveShareRegex;    // Regex used to perform conversion of hidden drive shares.
    };

} // namespace PCC
