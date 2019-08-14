// InternetPathPlugin.cpp
// (c) 2010-2019, Charles Lechasseur
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
#include <InternetPathPlugin.h>
#include <resource.h>

#include <algorithm>
#include <sstream>


namespace
{
    const std::wstring  NETWORK_SHARE_PREFIX    = L"\\\\";      // Prefix of string for network share paths
    const std::wstring  FILE_URI_PREFIX         = L"file:///";  // Prefix of file URI paths
    const std::wstring  NETWORK_FILE_URI_PREFIX = L"file://";   // Prefix of network file URI paths
    const std::wstring  WHITESPACE_TO_ESCAPE    = L"\t\r ";     // Whitespace to escape in internet paths
    const std::wstring  WHITESPACE_ESCAPE_SEQ   = L"%20";       // Escape sequence to use instead of whitespace

    // Plugin unique ID: {8F2ADCCC-9693-407d-9300-FCCB9A12B982}
    const GUID          INTERNET_PATH_PLUGIN_ID = { 0x8f2adccc, 0x9693, 0x407d, { 0x93, 0x0, 0xfc, 0xcb, 0x9a, 0x12, 0xb9, 0x82 } };

} // anonymous namespace

namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        InternetPathPlugin::InternetPathPlugin()
            : LongUNCPathPlugin(IDS_INTERNET_PATH_PLUGIN_DESCRIPTION, IDS_INTERNET_PATH_PLUGIN_DESCRIPTION, IDS_INTERNET_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& InternetPathPlugin::Id() const
        {
            return INTERNET_PATH_PLUGIN_ID;
        }

        //
        // Determines if the plugin should be enabled or not in the contextual menu.
        // In our case, we inherit from the UNC path plugin, which grays itself out
        // if there are no valid shares. In our case however, we always want to be
        // enabled and we'll use the long path if no UNC path is available.
        //
        // @param p_ParentPath Path of parent directory; unused.
        // @param p_File Path of one file selected; unused.
        // @return always true to tell PCC to enable our plugin.
        //
        bool InternetPathPlugin::Enabled(const std::wstring& /*p_ParentPath*/,
                                         const std::wstring& /*p_File*/) const
        {
            return true;
        }

        //
        // Returns the path of the specified file in file URI format
        //
        // @param p_File File path.
        // @return Internet (e.g., URI) path.
        //
        std::wstring InternetPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // First call inherited version to get the path.
            std::wstring path = LongUNCPathPlugin::GetPath(p_File);

            // There are two possible formats we use. For local files, we use
            // C:\path\to\file -> file://C:/path/to/file
            // For network shares, we use
            // \\computer\share\path\to\file -> file://computer/share/path/to/file
            if (path.find(NETWORK_SHARE_PREFIX) == 0) {
                path = NETWORK_FILE_URI_PREFIX + path.replace(0, NETWORK_SHARE_PREFIX.size(), L"");
            } else {
                path = FILE_URI_PREFIX + path;
            }

            // Now switch backslashes to slashes.
            std::replace(path.begin(), path.end(), L'\\', L'/');

            // Switch whitespace for %20.
            std::wstringstream newPathSS;
            std::wstring::size_type oldPos = 0, whitespacePos = path.find_first_of(WHITESPACE_TO_ESCAPE);
            while (whitespacePos != std::wstring::npos) {
                newPathSS << path.substr(oldPos, whitespacePos - oldPos) << WHITESPACE_ESCAPE_SEQ;
                oldPos = whitespacePos + 1;
                whitespacePos = path.find_first_of(WHITESPACE_TO_ESCAPE, oldPos);
            }
            if (oldPos < path.size()) {
                newPathSS << path.substr(oldPos, path.size() - oldPos);
            }
            path = newPathSS.str();

            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        InternetPathPlugin::InternetPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                               const unsigned short p_HelpTextStringResourceID)
            : LongUNCPathPlugin(p_DescriptionStringResourceID, p_DescriptionStringResourceID, p_HelpTextStringResourceID)
        {
        }

        //
        // Determines if this plugin is androgynous. In our case, it never is.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool InternetPathPlugin::IsAndrogynous() const
        {
            return false;
        }

    } // namespace Plugins

} // namespace PCC
