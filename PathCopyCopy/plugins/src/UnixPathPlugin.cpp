// UnixPathPlugin.cpp
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
#include <UnixPathPlugin.h>

#include <algorithm>


namespace
{
    // Plugin unique ID: {5B5DA5CB-3284-45a9-A1E5-4D6B03107924}
    const GUID UNIX_PATH_PLUGIN_ID = { 0x5b5da5cb, 0x3284, 0x45a9, { 0xa1, 0xe5, 0x4d, 0x6b, 0x3, 0x10, 0x79, 0x24 } };

} // anonymous namespace


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        UnixPathPlugin::UnixPathPlugin() noexcept(false)
            : LongPathPlugin(IDS_UNIX_PATH_PLUGIN_DESCRIPTION, IDS_UNIX_PATH_PLUGIN_DESCRIPTION, IDS_UNIX_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& UnixPathPlugin::Id() const noexcept(false)
        {
            return UNIX_PATH_PLUGIN_ID;
        }

        //
        // Returns the Unix path of the specified file.
        //
        // @param p_File File path.
        // @return File path with backslashes replaced by forward slashes.
        //
        std::wstring UnixPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get long path.
            std::wstring path = LongPathPlugin::GetPath(p_File);

            // Replace all backslashes with forward slashes and return the path.
            std::replace(path.begin(), path.end(), L'\\', L'/');
            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        UnixPathPlugin::UnixPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                       const unsigned short p_HelpTextStringResourceID)
            : LongPathPlugin(p_DescriptionStringResourceID, p_DescriptionStringResourceID, p_HelpTextStringResourceID)
        {
        }

        //
        // Determines if this plugin is androgynous. In our case, it never is.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool UnixPathPlugin::IsAndrogynous() const noexcept(false)
        {
            return false;
        }

    } // namespace Plugins

} // namespace PCC
