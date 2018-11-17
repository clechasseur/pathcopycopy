// ShortNamePlugin.cpp
// (c) 2008-2018, Charles Lechasseur
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
#include <ShortNamePlugin.h>
#include <LongNamePlugin.h>
#include <PathCopyCopySettings.h>
#include <PluginUtils.h>
#include <resource.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {C8757DCA-1BA8-48d0-8145-B93BBB9237AF}
        const GUID ShortNamePlugin::ID = { 0xc8757dca, 0x1ba8, 0x48d0, { 0x81, 0x45, 0xb9, 0x3b, 0xbb, 0x92, 0x37, 0xaf } };

        //
        // Constructor.
        //
        ShortNamePlugin::ShortNamePlugin()
            : ShortPathPlugin(IDS_SHORT_NAME_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_NAME_PLUGIN_DESCRIPTION, IDS_SHORT_NAME_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& ShortNamePlugin::Id() const
        {
            return ShortNamePlugin::ID;
        }

        //
        // Returns the short name of the specified file.
        //
        // @param p_File File path.
        // @return Short file name.
        //
        std::wstring ShortNamePlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get the short path.
            std::wstring shortPath = ShortPathPlugin::GetPath(p_File);

            // Get the last part, the file name.
            size_t lastDelimiterPos = shortPath.find_last_of(L"/\\");
            if (lastDelimiterPos == shortPath.size() - 1) {
                // Delimiter is at the end, remove it and get name just before that.
                shortPath = shortPath.substr(0, lastDelimiterPos);
                lastDelimiterPos = shortPath.find_last_of(L"/\\");
            }
            return lastDelimiterPos != std::wstring::npos
                ? shortPath.substr(lastDelimiterPos + 1) : shortPath;
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the long name plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool ShortNamePlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, LongNamePlugin::ID);
        }

    } // namespace Plugins

} // namespace PCC
