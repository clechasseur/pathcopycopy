// CygwinPathPlugin.cpp
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

#include <stdafx.h>
#include <CygwinPathPlugin.h>
#include <resource.h>

#include <sstream>


namespace
{
    // Strings used to build a Cygwin path.
    const wchar_t CYGDRIVE_PREFIX[]   = L"/cygdrive/";

    // Plugin unique ID: {CD50DCE3-9A5C-4adf-B552-1741361567D6}
    const GUID CYGWIN_PATH_PLUGIN_ID = { 0xcd50dce3, 0x9a5c, 0x4adf, { 0xb5, 0x52, 0x17, 0x41, 0x36, 0x15, 0x67, 0xd6 } };

} // anonymous namespace


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        CygwinPathPlugin::CygwinPathPlugin()
            : UnixPathPlugin(IDS_CYGWIN_PATH_PLUGIN_DESCRIPTION, IDS_CYGWIN_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& CygwinPathPlugin::Id() const
        {
            return CYGWIN_PATH_PLUGIN_ID;
        }

        //
        // Returns the Cygwin path of the specified file.
        //
        // @param p_File File path.
        // @return File path in Cygwin format (/cygdrive/c/...)
        //
        std::wstring CygwinPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get Unix path.
            std::wstring path = UnixPathPlugin::GetPath(p_File);

            // Check if the file begins with a drive letter. If so,
            // remove the drive letter and replace it with /cygdrive/letter.
            if (path.size() >= 3 && path[1] == L':') {
                std::wstringstream newPathSS;
                newPathSS << CYGDRIVE_PREFIX                            // The Cygwin prefix
                          << static_cast<wchar_t>(::towlower(path[0]))  // Drive letter
                          << path.substr(2);                            // Rest of the path, including the slash after that : we had.
                path = newPathSS.str();
            }

            // Return modified path.
            return path;
        }

    } // namespace Plugins

} // namespace PCC
