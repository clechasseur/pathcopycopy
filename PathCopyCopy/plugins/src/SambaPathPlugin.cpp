// SambaPathPlugin.cpp
// (c) 2018-2020, Charles Lechasseur
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
#include <SambaPathPlugin.h>
#include <resource.h>


namespace
{
    const wchar_t* const FILE_URI_PREFIX    = L"file://";   // Prefix of file URI paths
    const wchar_t* const SAMBA_URI_PREFIX   = L"smb://";    // Prefix of Samba paths

    // Plugin unique ID: {7DA6A4A2-AE54-40E0-9910-EBD9EF3F017E}
    const GUID          SAMBA_PATH_PLUGIN_ID = { 0x7da6a4a2, 0xae54, 0x40e0, { 0x99, 0x10, 0xeb, 0xd9, 0xef, 0x3f, 0x1, 0x7e } };


} // anonymous namespace

namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        SambaPathPlugin::SambaPathPlugin() noexcept(false)
            : InternetPathPlugin(IDS_SAMBA_PATH_PLUGIN_DESCRIPTION, IDS_SAMBA_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& SambaPathPlugin::Id() const noexcept(false)
        {
            return SAMBA_PATH_PLUGIN_ID;
        }

        //
        // Returns the path of the specified file in Samba format
        //
        // @param p_File File path.
        // @return Samba path.
        //
        std::wstring SambaPathPlugin::GetPath(const std::wstring& p_File) const
        {
            // First call inherited version to get the Internet path.
            std::wstring path = InternetPathPlugin::GetPath(p_File);

            // The Internet path plugin did almost all the job for us.
            // All we have to do is replace the prefix.
            if (path.find(FILE_URI_PREFIX) == 0) {
                path = SAMBA_URI_PREFIX + path.replace(0, ::wcslen(FILE_URI_PREFIX), L"");
            }

            return path;
        }

    } // namespace Plugins

} // namespace PCC
