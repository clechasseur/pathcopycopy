// ShortPathPlugin.cpp
// (c) 2008-2021, Charles Lechasseur
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
#include <ShortPathPlugin.h>
#include <LongPathPlugin.h>
#include <PathCopyCopySettings.h>
#include <PluginUtils.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {93B362D1-2B13-467a-B618-6784F24F3BA1}
        const GUID ShortPathPlugin::ID = { 0x93b362d1, 0x2b13, 0x467a, { 0xb6, 0x18, 0x67, 0x84, 0xf2, 0x4f, 0x3b, 0xa1 } };

        //
        // Constructor.
        //
        ShortPathPlugin::ShortPathPlugin() noexcept(false)
            : AndrogynousInternalPlugin(IDS_SHORT_PATH_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_PATH_PLUGIN_DESCRIPTION, IDS_SHORT_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& ShortPathPlugin::Id() const noexcept(false)
        {
            return ID;
        }

        //
        // Returns the short path of the specified file.
        //
        // @param p_File File path.
        // @return Short path.
        //
        std::wstring ShortPathPlugin::GetPath(const std::wstring& p_File) const
        {
            assert(m_pSettings != nullptr);

            std::wstring path(p_File);
            if (!path.empty()) {
                std::wstring shortPath(MAX_PATH + 1, L'\0');
                if (::GetShortPathNameW(p_File.c_str(), &*shortPath.begin(), gsl::narrow<DWORD>(shortPath.size())) != 0) {
                    path = shortPath.c_str();
                }

                // Append separator if needed.
                if (m_pSettings != nullptr && m_pSettings->GetAppendSeparatorForDirectories() && PluginUtils::IsDirectory(path)) {
                    path += L"\\";
                }
            }
            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_AndrogynousDescriptionStringResourceID ID of androgynous description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        ShortPathPlugin::ShortPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                         const unsigned short p_AndrogynousDescriptionStringResourceID,
                                         const unsigned short p_HelpTextStringResourceID)
            : AndrogynousInternalPlugin(p_DescriptionStringResourceID, p_AndrogynousDescriptionStringResourceID, p_HelpTextStringResourceID)
        {
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the long path plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool ShortPathPlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, LongPathPlugin::ID);
        }

    } // namespace Plugins

} // namespace PCC
