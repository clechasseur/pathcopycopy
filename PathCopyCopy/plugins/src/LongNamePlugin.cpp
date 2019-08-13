// LongNamePlugin.cpp
// (c) 2008-2019, Charles Lechasseur
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
#include <LongNamePlugin.h>
#include <PathCopyCopySettings.h>
#include <PluginUtils.h>
#include <resource.h>
#include <ShortNamePlugin.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {3A42B5C6-72D6-4a8a-BA44-014FA64AA11A}
        const GUID LongNamePlugin::ID = { 0x3a42b5c6, 0x72d6, 0x4a8a, { 0xba, 0x44, 0x1, 0x4f, 0xa6, 0x4a, 0xa1, 0x1a } };

        //
        // Constructor.
        //
        LongNamePlugin::LongNamePlugin()
            : LongPathPlugin(IDS_LONG_NAME_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_NAME_PLUGIN_DESCRIPTION, IDS_LONG_NAME_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& LongNamePlugin::Id() const
        {
            return LongNamePlugin::ID;
        }

        //
        // Returns the long name of the specified file.
        //
        // @param p_File File path.
        // @return Long file name.
        //
        std::wstring LongNamePlugin::GetPath(const std::wstring& p_File) const
        {
            // Call parent to get the long path.
            std::wstring longPath = LongPathPlugin::GetPath(p_File);

            // Get the last part, the file name.
            size_t lastDelimiterPos = longPath.find_last_of(L"/\\");
            if (lastDelimiterPos == longPath.size() - 1) {
                // Delimiter is at the end, remove it and get name just before that.
                longPath = longPath.substr(0, lastDelimiterPos);
                lastDelimiterPos = longPath.find_last_of(L"/\\");
            }
            return lastDelimiterPos != std::wstring::npos
                ? longPath.substr(lastDelimiterPos + 1) : longPath;
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the short name plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool LongNamePlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, ShortNamePlugin::ID);
        }

    } // namespace Plugins

} // namespace PCC
