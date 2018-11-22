// LongUNCPathPlugin.cpp
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
#include <LongUNCPathPlugin.h>
#include <PluginUtils.h>
#include <resource.h>
#include <ShortUNCPathPlugin.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        // Plugin unique ID: {EA24BCC3-35F5-46b9-A2A5-A12A0AED2D28}
        const GUID LongUNCPathPlugin::ID = { 0xea24bcc3, 0x35f5, 0x46b9, { 0xa2, 0xa5, 0xa1, 0x2a, 0xa, 0xed, 0x2d, 0x28 } };

        //
        // Constructor.
        //
        LongUNCPathPlugin::LongUNCPathPlugin()
            : LongPathPlugin(IDS_LONG_UNC_PATH_PLUGIN_DESCRIPTION, IDS_ANDROGYNOUS_UNC_PATH_PLUGIN_DESCRIPTION, IDS_LONG_UNC_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& LongUNCPathPlugin::Id() const
        {
            return LongUNCPathPlugin::ID;
        }

        //
        // Determines if the plugin should be enabled or not in the contextual
        // menu. For UNC plugins, we only enable the item if there is a valid share.
        //
        // @param p_ParentPath Path of the parent folder of items being acted upon.
        // @return true if the plugin should be enabled, false otherwise.
        //
        bool LongUNCPathPlugin::Enabled(const std::wstring& /*p_ParentPath*/,
                                        const std::wstring& p_File) const
        {
            // Call method to get the path and check if there was a valid share.
            std::wstring path(p_File);
            return InternalGetPath(path);
        }

        //
        // Returns the long UNC path of the specified file.
        //
        // @param p_File File path.
        // @return UNC path if file has one, otherwise its long path.
        //
        std::wstring LongUNCPathPlugin::GetPath(const std::wstring& p_File) const
        {
            std::wstring path(p_File);
            InternalGetPath(path);
            return path;
        }

        //
        // Protected constructor with custom description and help text resources.
        //
        // @param p_DescriptionStringResourceID ID of description resource.
        // @param p_AndrogynousDescriptionStringResourceID ID of androgynous description resource.
        // @param p_HelpTextStringResourceID ID of help text resource.
        //
        LongUNCPathPlugin::LongUNCPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                             const unsigned short p_AndrogynousDescriptionStringResourceID,
                                             const unsigned short p_HelpTextStringResourceID)
            : LongPathPlugin(p_DescriptionStringResourceID, p_AndrogynousDescriptionStringResourceID, p_HelpTextStringResourceID)
        {
        }

        //
        // Determines if this plugin is androgynous. It is considered androgynous
        // if the short UNC path plugin is not shown according to settings.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool LongUNCPathPlugin::IsAndrogynous() const
        {
            assert(m_pSettings != nullptr);

            return m_pSettings != nullptr &&
                   m_pSettings->GetDropRedundantWords() &&
                   !PluginUtils::IsPluginShown(*m_pSettings, ShortUNCPathPlugin::ID);
        }

        //
        // Returns the long UNC path of the specified file.
        //
        // @param p_File File path on input, UNC path if it has one on output.
        // @return true if the path returned is a UNC path, false otherwise.
        //
        bool LongUNCPathPlugin::InternalGetPath(std::wstring& p_rPath) const
        {
            assert(m_pSettings != nullptr);

            // Call parent to get long path.
            p_rPath = LongPathPlugin::GetPath(p_rPath);

            // Check if it already was an UNC path.
            bool converted = PluginUtils::IsUNCPath(p_rPath);
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

                // If we got a path and we must use FQDN, convert it.
                const bool useFQDN = m_pSettings != nullptr ? m_pSettings->GetUseFQDN() : false;
                if (converted && useFQDN) {
                    PluginUtils::ConvertUNCHostToFQDN(newPath);
                }

                if (converted) {
                    p_rPath = newPath;
                }
            }
            return converted;
        }


    } // namespace Plugins

} // namespace PCC
