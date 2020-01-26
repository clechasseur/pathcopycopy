// ShortFolderPlugin.cpp
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
#include <ShortFolderPlugin.h>
#include <LongFolderPlugin.h>
#include <PathCopyCopySettings.h>
#include <PluginUtils.h>
#include <resource.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: // {0929538B-5F52-4480-9902-2BBEC9C84905}
        const GUID ShortFolderPlugin::ID = { 0x929538b, 0x5f52, 0x4480, { 0x99, 0x2, 0x2b, 0xbe, 0xc9, 0xc8, 0x49, 0x5 } };

        //
        // Constructor.
        //
        ShortFolderPlugin::ShortFolderPlugin() noexcept(false)
            : ShortPathPlugin(IDS_SHORT_FOLDER_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_FOLDER_PLUGIN_DESCRIPTION, IDS_SHORT_FOLDER_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& ShortFolderPlugin::Id() const noexcept(false)
        {
            return ShortFolderPlugin::ID;
        }

        //
        // Returns the short path of the specified file's parent directory.
        //
        // @param p_File File path.
        // @return Short path of parent directory.
        //
        std::wstring ShortFolderPlugin::GetPath(const std::wstring& p_File) const
        {
            assert(m_pSettings != nullptr);

            // Call parent to get the short path.
            std::wstring shortPath = ShortPathPlugin::GetPath(p_File);

            // If parent appended a separator, remove it here so that we
            // can properly extract parent folder.
            if (!shortPath.empty() && (shortPath.back() == L'\\' || shortPath.back() == L'/')) {
                shortPath = shortPath.substr(0, shortPath.size() - 1);
            }

            // Extract parent directory.
            PluginUtils::ExtractFolderFromPath(shortPath);

            // If settings instructs us to append separator for directories, append one,
            // since this plugin always returns directory paths.
            if (m_pSettings != nullptr && m_pSettings->GetAppendSeparatorForDirectories()) {
                shortPath += L"\\";
            }

            return shortPath;
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the long folder plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool ShortFolderPlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, LongFolderPlugin::ID);
        }

    } // namespace Plugins

} // namespace PCC
