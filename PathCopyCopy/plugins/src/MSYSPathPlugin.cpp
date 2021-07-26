// MSYSPathPlugin.cpp
// (c) 2019-2021, Charles Lechasseur
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
#include <MSYSPathPlugin.h>
#include <StringUtils.h>

#include <sstream>


namespace
{
    // Plugin unique ID: {31022A3D-6FEE-4B36-843E-BBB4556AB35B}
    const GUID MSYS_PATH_PLUGIN_ID = { 0x31022a3d, 0x6fee, 0x4b36, { 0x84, 0x3e, 0xbb, 0xb4, 0x55, 0x6a, 0xb3, 0x5b } };

} // anonymous namespace


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        MSYSPathPlugin::MSYSPathPlugin() noexcept(false)
            : UnixPathPlugin(IDS_MSYS_PATH_PLUGIN_DESCRIPTION, IDS_MSYS_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& MSYSPathPlugin::Id() const noexcept(false)
        {
            return MSYS_PATH_PLUGIN_ID;
        }

        //
        // Returns the MSYS/MSYS2 path of the specified file.
        //
        // @param p_File File path.
        // @return File path in MSYS/MSYS2 format (/c/...)
        //
        std::wstring MSYSPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get Unix path.
            std::wstring path = UnixPathPlugin::GetPath(p_File);

            // Check if the file begins with a drive letter. If so,
            // remove the drive letter and replace it with /letter.
            [[gsl::suppress(type.4)]] // Compiler considers foo{bar} to be a C-style cast
            if (path.size() >= 3 && path.at(1) == L':') {
                std::wstringstream newPathSS;
                newPathSS << L"/"                               // The prefix
                          << wchar_t{::towlower(path.front())}  // Drive letter
                          << path.substr(2);                    // Rest of the path, including the slash after that : we had.
                path = newPathSS.str();
            }

            // Escape spaces bash-style. This works without quotes.
            StringUtils::ReplaceAll(path, L" ", L"\\ ");

            // Return modified path.
            return path;
        }

    } // namespace Plugins

} // namespace PCC
