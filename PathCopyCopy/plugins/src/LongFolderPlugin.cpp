// LongFolderPlugin.cpp
// (c) 2011-2019, Charles Lechasseur
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
#include <LongFolderPlugin.h>
#include <PathCopyCopySettings.h>
#include <PluginUtils.h>
#include <resource.h>
#include <ShortFolderPlugin.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {349939D7-780F-43fd-A98E-83D9ADD44E22}
        const GUID LongFolderPlugin::ID = { 0x349939d7, 0x780f, 0x43fd, { 0xa9, 0x8e, 0x83, 0xd9, 0xad, 0xd4, 0x4e, 0x22 } };

        //
        // Constructor.
        //
        LongFolderPlugin::LongFolderPlugin()
            : LongPathPlugin(IDS_LONG_FOLDER_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_FOLDER_PLUGIN_DESCRIPTION, IDS_LONG_FOLDER_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& LongFolderPlugin::Id() const
        {
            return LongFolderPlugin::ID;
        }

        //
        // Returns the long path of the specified file's parent directory.
        //
        // @param p_File File path.
        // @return Long path of parent directory.
        //
        std::wstring LongFolderPlugin::GetPath(const std::wstring& p_File) const
        {
            assert(m_pSettings != nullptr);

            // Call parent to get the long path.
            std::wstring longPath = LongPathPlugin::GetPath(p_File);

            // If parent appended a separator, remove it here so that we
            // can properly extract parent folder.
            if (!longPath.empty() && (longPath.back() == L'\\' || longPath.back() == L'/')) {
                longPath = longPath.substr(0, longPath.size() - 1);
            }

            // Extract parent directory.
            PluginUtils::ExtractFolderFromPath(longPath);

            // If settings instructs us to append separator for directories, append one,
            // since this plugin always returns directory paths.
            if (m_pSettings != nullptr && m_pSettings->GetAppendSeparatorForDirectories()) {
                longPath += L"\\";
            }

            return longPath;
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the short folder plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool LongFolderPlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, ShortFolderPlugin::ID);
        }

    } // namespace Plugins

} // namespace PCC
