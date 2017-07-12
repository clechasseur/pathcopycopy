// LongUNCFolderPlugin.cpp
// (c) 2011-2017, Charles Lechasseur
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
#include <LongUNCFolderPlugin.h>
#include <PluginUtils.h>
#include <resource.h>
#include <ShortUNCFolderPlugin.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {E2C942AC-917C-4aee-A867-8F6AB960BA76}
        const GUID LongUNCFolderPlugin::ID = { 0xe2c942ac, 0x917c, 0x4aee, { 0xa8, 0x67, 0x8f, 0x6a, 0xb9, 0x60, 0xba, 0x76 } };

        //
        // Constructor.
        //
        LongUNCFolderPlugin::LongUNCFolderPlugin()
            : LongPathPlugin(IDS_LONG_UNC_FOLDER_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_UNC_FOLDER_PLUGIN_DESCRIPTION, IDS_LONG_UNC_FOLDER_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& LongUNCFolderPlugin::Id() const
        {
            return LongUNCFolderPlugin::ID;
        }

        //
        // Determines if the plugin should be enabled or not in the contextual
        // menu. For UNC plugins, we only enable the item if there is a valid share.
        //
        // @param p_ParentPath Path of the parent folder of items being acted upon.
        // @return true if the plugin should be enabled, false otherwise.
        //
        bool LongUNCFolderPlugin::Enabled(const std::wstring& p_ParentPath,
                                          const std::wstring& /*p_File*/) const
        {
            // Call method to get the path and check if there was a valid share.
            std::wstring path(p_ParentPath);
            return InternalGetPath(path, false);
        }

        //
        // Returns the long UNC path of the specified file's parent directory.
        //
        // @param p_File File path.
        // @return UNC path of parent if file has one, otherwise its long path.
        //
        std::wstring LongUNCFolderPlugin::GetPath(const std::wstring& p_File) const
        {
            std::wstring path(p_File);
            InternalGetPath(path, true);
            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_AndrogynousDescriptionStringResourceID ID of androgynous description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        LongUNCFolderPlugin::LongUNCFolderPlugin(const unsigned short p_DescriptionStringResourceID,
                                                 const unsigned short p_AndrogynousDescriptionStringResourceID,
                                                 const unsigned short p_HelpTextStringResourceID)
            : LongPathPlugin(p_DescriptionStringResourceID, p_AndrogynousDescriptionStringResourceID, p_HelpTextStringResourceID)
        {
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the short UNC folder plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool LongUNCFolderPlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, ShortUNCFolderPlugin::ID);
        }

        //
        // Returns the long UNC path of the specified file's parent directory.
        //
        // @param p_rPath File path on input, UNC path of parent directory on output.
        // @param p_ExtractFolder Whether to extract folder before looking for UNC
        //                        paths. If this is set to false, the caller is
        //                        expected to have performed the task already.
        // @return true if the file's parent directory has a valid UNC path, false otherwise.
        //
        bool LongUNCFolderPlugin::InternalGetPath(std::wstring& p_rPath,
                                                  const bool p_ExtractFolder) const
        {
            assert(m_pSettings != nullptr);

            // We need to first get the long path, extract the parent
            // then look for shares with that parent, since the fact that a folder
            // has a valid UNC path does not necessarily mean that the parent has one.
            bool converted = false;

            // Get parent's path.
            p_rPath = LongPathPlugin::GetPath(p_rPath);
            if ((!p_ExtractFolder) || PluginUtils::ExtractFolderFromPath(p_rPath)) {
                // Got the parent path, check if this is already an UNC path.
                converted = PluginUtils::IsUNCPath(p_rPath);
                if (!converted) {
                    // Try to get path on mapped network drive.
                    std::wstring newPath = p_rPath;
                    converted = PluginUtils::GetMappedDriveFilePath(newPath);

                    // If it wasn't on a mapped drive, check if it's in a network share.
                    const bool useHiddenShares = m_pSettings != nullptr ? m_pSettings->GetUseHiddenShares() : false;
                    if (!converted) {
                        converted = PluginUtils::GetNetworkShareFilePath(newPath, useHiddenShares);
                    }

                    // If it wasn't in a defined network share, use a hidden drive share if we're allowed.
                    if (!converted && useHiddenShares) {
                        converted = PluginUtils::GetHiddenDriveShareFilePath(newPath);
                    }

                    // If we got a UNC path, use it, otherwise keep the long path.
                    if (converted) {
                        p_rPath = newPath;
                    }
                }
            } else {
                // No parent for some reason, simply return the original path.
            }
            return converted;
        }

    } // namespace Plugins

} // namespace PCC
