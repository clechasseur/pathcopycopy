// WSLPathPlugin.cpp
// (c) 2018-2019, Charles Lechasseur
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
#include <WSLPathPlugin.h>
#include <StringUtils.h>
#include <resource.h>

#include <sstream>


namespace
{
    // Strings used to build a WSL path.
    const wchar_t MNT_PREFIX[]   = L"/mnt/";

    // Plugin unique ID: {BD574871-5DF9-4B64-83D1-2AF9C0C17F66}
    const GUID WSL_PATH_PLUGIN_ID = { 0xbd574871, 0x5df9, 0x4b64, { 0x83, 0xd1, 0x2a, 0xf9, 0xc0, 0xc1, 0x7f, 0x66 } };

} // anonymous namespace


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        WSLPathPlugin::WSLPathPlugin()
            : UnixPathPlugin(IDS_WSL_PATH_PLUGIN_DESCRIPTION, IDS_WSL_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& WSLPathPlugin::Id() const
        {
            return WSL_PATH_PLUGIN_ID;
        }

        //
        // Returns the WSL path of the specified file.
        //
        // @param p_File File path.
        // @return File path in WSL format (/mnt/c/...)
        //
        std::wstring WSLPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get Unix path.
            std::wstring path = UnixPathPlugin::GetPath(p_File);

            // Check if the file begins with a drive letter. If so,
            // remove the drive letter and replace it with /mnt/letter.
            if (path.size() >= 3 && path[1] == L':') {
                std::wstringstream newPathSS;
                newPathSS << MNT_PREFIX                                 // The mnt prefix
                          << static_cast<wchar_t>(::towlower(path[0]))  // Drive letter
                          << path.substr(2);                            // Rest of the path, including the slash after that : we had.
                path = newPathSS.str();
            }

            // Escape spaces bash-style. This works without quotes.
            StringUtils::ReplaceAll(path, L" ", L"\\ ");

            // Return modified path.
            return path;
        }

    } // namespace Plugins

} // namespace PCC
