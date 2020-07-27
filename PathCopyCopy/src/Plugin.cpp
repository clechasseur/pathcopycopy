// Plugin.cpp
// (c) 2008-2020, Charles Lechasseur
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
#include <Plugin.h>

#include <CopyToClipboardPathAction.h>

#include <assert.h>
#include <string.h>


namespace PCC
{
    //
    // Returns the ID for this plugin to use to fetch its icon.
    // For most plugins this is the same as the plugin ID, so
    // the default implementation is fine.
    //
    // @return ID to use to fetch plugin icon.
    //
    const GUID& Plugin::IdForIcon() const
    {
        return Id();
    }

    //
    // Returns the help text to show in the Windows Explorer status bar
    // when the user hovers the cursor over the plugin's menu item.
    // If not overridden, no help text will be shown.
    //
    // @return Help text for plugin.
    //
    std::wstring Plugin::HelpText() const
    {
        return L"";
    }

    //
    // Returns the path to the icon to use for this plugin. If not overridden,
    // no icon file will be specified.
    //
    // @return Path to icon file, or an empty string if not specified.
    //
    std::wstring Plugin::IconFile() const
    {
        return L"";
    }

    //
    // Determines if the default icon should be used to display this
    // plugin in the contextual menu. If not overridden, the default
    // icon will not be used.
    //
    // @return true to use default icon, false otherwise.
    //
    bool Plugin::UseDefaultIcon() const noexcept(false)
    {
        return false;
    }

    //
    // Returns whether the plugin should be enabled or not in the
    // contextual menu. If not overridden, it will always be enabled.
    //
    // @param p_ParentPath Path of the parent directory of files that
    //                     triggered the contextual menu.
    // @param p_File Path of one file that was selected.
    // @return true if plugin should be enabled in contextual menu.
    //
    bool Plugin::Enabled(const std::wstring& /*p_ParentPath*/,
                         const std::wstring& /*p_File*/) const noexcept(false)
    {
        return true;
    }

    //
    // Returns the separator to use between each path when using this plugin.
    // The default value is the empty string, which instructs PCC to use the
    // default value (usually a newline).
    //
    // @return Separator between each path, or empty string to use default value.
    //
    std::wstring Plugin::PathsSeparator() const
    {
        return L"";
    }

    //
    // Checks if this plugin wants to copy paths recursively.
    // The default value is false.
    //
    // @return Whether to copy paths recursively.
    //
    bool Plugin::CopyPathsRecursively() const noexcept(false)
    {
        return false;
    }

    //
    // Returns the action to perform on the path or paths when using this plugin.
    // By default, this returns an action copying the path or paths to the clipboard.
    //
    // @return Path action instance to use.
    //
    PathActionSP Plugin::Action() const
    {
        return std::make_shared<PCC::Actions::CopyToClipboardPathAction>();
    }

    //
    // Checks if this plugin is a separator.
    // Do not override: will be overridden by the PluginSeparator class *only*.
    //
    // @return true if this plugin is a submenu separator proxy.
    //
    bool Plugin::IsSeparator() const noexcept
    {
        return false;
    }

    //
    // Checks if Path Copy Copy can drop redundant words like
    // "copy" from this plugin's description. The default implementation
    // returns true; Plugin classes that do not want to honor the
    // "Drop redundant words" setting should override this.
    //
    // @return true if PCC can drop redundant words from this plugin's description.
    //
    bool Plugin::CanDropRedundantWords() const noexcept(false)
    {
        return true;
    }

    //
    // Provides a pointer to the object that can be used to access PCC settings.
    // Some plugins depend on this to work.
    //
    // @param p_pSettings Pointer to settings object.
    //                    Can be nullptr to remove an existing settings object.
    //
    void Plugin::SetSettings(const Settings* const p_pSettings) noexcept
    {
        m_pSettings = p_pSettings;
    }

    //
    // Provides a pointer to an object that can be used to access other plugins.
    // Some plugins depend on this to work.
    //
    // @param p_pPluginProvider Pointer to plugin provider.
    //                          Can be nullptr to remove an existing provider.
    //
    void Plugin::SetPluginProvider(const PluginProvider* const p_pPluginProvider) noexcept
    {
        m_pPluginProvider = p_pPluginProvider;
    }

    //
    // Compares two plugins stored in shared pointers using their unique identifiers.
    //
    // @param p_spPlugin1 First plugin to compare.
    // @param p_spPlugin2 Second plugin to compare.
    // @return true if p_spPlugin1's identifier comes before p_spPlugin2's.
    //
    bool operator<(const PluginSP& p_spPlugin1,
                   const PluginSP& p_spPlugin2)
    {
        assert(p_spPlugin1 != nullptr);
        assert(p_spPlugin2 != nullptr);

        return ::memcmp(&p_spPlugin1->Id(), &p_spPlugin2->Id(), sizeof(GUID)) < 0;
    }
    
    //
    // Compares a plugin's unique identifier with a GUID.
    //
    // @param p_spPlugin1 Plugin whose unique identifier to compare.
    // @param p_PluginId2 Unique identifier to compare it with.
    // @return true if the plugin's unique identifier comes before p_PluginId2.
    //
    bool operator<(const PluginSP& p_spPlugin1,
                   const GUID& p_PluginId2)
    {
        assert(p_spPlugin1 != nullptr);

        return ::memcmp(&p_spPlugin1->Id(), &p_PluginId2, sizeof(GUID)) < 0;
    }
    
    //
    // Compares a plugin's unique identifier with a GUID.
    //
    // @param p_PluginId1 Unique identifier to compare.
    // @param p_spPlugin2 Plugin whose unique identifier to compare it with.
    // @return true if p_PluginId1 comes before the plugin's unique identifier.
    //
    bool operator<(const GUID& p_PluginId1,
                   const PluginSP& p_spPlugin2)
    {
        assert(p_spPlugin2 != nullptr);

        return ::memcmp(&p_PluginId1, &p_spPlugin2->Id(), sizeof(GUID)) < 0;
    }

}   // namespace PCC
