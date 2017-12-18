// PathCopyCopySettings.cpp
// (c) 2009-2017, Charles Lechasseur
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
#include <PathCopyCopySettings.h>

#include <PathCopyCopy_i.h>
#include <PathCopyCopyPluginsRegistry.h>
#include <PipelinePlugin.h>
#include <PluginSeparator.h>
#include <PluginUtils.h>
#include <StCoInitialize.h>
#include <StOleStr.h>

#include <algorithm>
#include <sstream>

#include <assert.h>
#include <time.h>

#ifdef _DEBUG
#   include <mutex>
#endif // _DEBUG


namespace
{
    // Name of registry keys storing the PCC settings.
    const wchar_t* const    PCC_SETTINGS_KEY                                = L"Software\\clechasseur\\PathCopyCopy";
    const wchar_t* const    PCC_ICONS_KEY                                   = L"Software\\clechasseur\\PathCopyCopy\\Icons";
    const wchar_t* const    PCC_PLUGINS_KEY                                 = L"Software\\clechasseur\\PathCopyCopy\\Plugins";
    const wchar_t* const    PCC_PIPELINE_PLUGINS_KEY                        = L"Software\\clechasseur\\PathCopyCopy\\PipelinePlugins";
    const wchar_t* const    PCC_TEMP_PIPELINE_PLUGINS_KEY                   = L"Software\\clechasseur\\PathCopyCopy\\TempPipelinePlugins";

    // Values used for PCC settings.
    const wchar_t* const    SETTING_REVISIONS                               = L"Revisions";
    const wchar_t* const    SETTING_USE_HIDDEN_SHARES                       = L"UseHiddenShares";
    const wchar_t* const    SETTING_ADD_QUOTES                              = L"AddQuotes";
    const wchar_t* const    SETTING_MAKE_EMAIL_LINKS                        = L"MakeEmailLinks";
    const wchar_t* const    SETTING_ENCODE_PARAM                            = L"EncodeParam";
    const wchar_t* const    SETTING_USE_ICON_FOR_DEFAULT_PLUGIN             = L"UseIconForDefaultPlugin";
    const wchar_t* const    SETTING_USE_ICON_FOR_SUBMENU                    = L"UseIconForSubmenu";
    const wchar_t* const    SETTING_USE_PREVIEW_MODE                        = L"UsePreviewMode";
    const wchar_t* const    SETTING_DROP_REDUNDANT_WORDS                    = L"DropRedundantWords";
    const wchar_t* const    SETTING_ALWAYS_SHOW_SUBMENU                     = L"AlwaysShowSubmenu";
    const wchar_t* const    SETTING_PATHS_SEPARATOR                         = L"PathsSeparator";
    const wchar_t* const    SETTING_CTRL_KEY_PLUGIN                         = L"CtrlKeyPlugin";
    const wchar_t* const    SETTING_MAIN_MENU_PLUGIN_DISPLAY_ORDER          = L"MainMenuDisplayOrder";
    const wchar_t* const    SETTING_SUBMENU_PLUGIN_DISPLAY_ORDER            = L"SubmenuDisplayOrder";
    const wchar_t* const    SETTING_UI_PLUGIN_DISPLAY_ORDER                 = L"UIDisplayOrder";
    const wchar_t* const    SETTING_KNOWN_PLUGINS                           = L"KnownPlugins";
    const wchar_t* const    SETTING_PIPELINE_DESCRIPTION                    = L"Description";
    const wchar_t* const    SETTING_PIPELINE_ICON_FILE                      = L"IconFile";
    const wchar_t* const    SETTING_PIPELINE_DISPLAY_ORDER                  = L"DisplayOrder";
    const wchar_t* const    SETTING_LAST_UPDATE_CHECK                       = L"LastUpdateCheck";
    const wchar_t* const    SETTING_UPDATE_INTERVAL                         = L"UpdateInterval";
    const wchar_t* const    SETTING_DISABLE_SOFTWARE_UPDATE                 = L"DisableSoftwareUpdate";

    // Deprecated PCC setting values. Used for revising.
    const wchar_t* const    OLD_SETTING_DISABLED_PLUGINS                    = L"DisabledPlugins";
    const wchar_t* const    OLD_SETTING_DEFAULT_PLUGIN                      = L"DefaultPlugin";
    const wchar_t* const    OLD_SETTING_PLUGINS_NOT_IN_SUBMENU              = L"PluginsNotInSubmenu";
    const wchar_t* const    OLD_SETTING_PLUGINS_IN_MAIN_MENU                = L"PluginsInMainMenu";

    // Possible values for the EncodeParam setting.
    const wchar_t* const    SETTING_ENCODE_PARAM_VALUE_NONE                 = L"None";
    const wchar_t* const    SETTING_ENCODE_PARAM_VALUE_WHITESPACE           = L"Whitespace";
    const wchar_t* const    SETTING_ENCODE_PARAM_VALUE_ALL                  = L"All";

    // Default values for PCC settings.
    const bool              SETTING_USE_HIDDEN_SHARES_DEFAULT               = false;
    const bool              SETTING_ADD_QUOTES_DEFAULT                      = false;
    const bool              SETTING_MAKE_EMAIL_LINKS_DEFAULT                = false;
    const wchar_t* const    SETTING_ENCODE_PARAM_DEFAULT                    = SETTING_ENCODE_PARAM_VALUE_NONE;
    const bool              SETTING_USE_ICON_FOR_DEFAULT_PLUGIN_DEFAULT     = false;
    const bool              SETTING_USE_ICON_FOR_SUBMENU_DEFAULT            = true;
    const bool              SETTING_USE_PREVIEW_MODE_DEFAULT                = false;
    const bool              SETTING_DROP_REDUNDANT_WORDS_DEFAULT            = false;
    const bool              SETTING_ALWAYS_SHOW_SUBMENU_DEFAULT             = true;
    const wchar_t* const    SETTING_PATHS_SEPARATOR_DEFAULT                 = L"";
    const double            SETTING_UPDATE_INTERVAL_DEFAULT                 = 604800.0;     // One week, in seconds.
    const bool              SETTING_DISABLE_SOFTWARE_UPDATE_DEFAULT         = false;

    // Constants used to parse data.
    const wchar_t           PLUGINS_SEPARATOR                               = L',';
    const wchar_t           REVISIONS_SEPARATOR                             = L',';

    // Constants used to generate plugin info for the UI.
    const wchar_t           INFO_GROUP_INFO_SEPARATOR                       = L',';
    const wchar_t           INFO_DESCRIPTION_SEPARATOR                      = L'|';

    // Constants used for icons.
    const wchar_t* const    DEFAULT_ICON_MARKER_STRING                      = L"default";

    //
    // Predicate used to sort pipeline plugins according to their sort order.
    // Uses an ID vector of ordered plugin IDs to know whether two plugins
    // should be before one another.
    //
    class PipelinePluginLess
    {
    public:
                        //
                        // Constructor.
                        //
                        // @param p_pvOrderedPluginIds Vector of ordered pipeline plugin IDs.
                        //
        explicit        PipelinePluginLess(const PCC::GUIDV& p_vOrderedPluginIds)
                            : m_vOrderedPluginIds(p_vOrderedPluginIds)
                        {
                        }

                        //
                        // Copy constructor is allowed, but not assignment operator.
                        //
                        PipelinePluginLess(const PipelinePluginLess&) = default;
        PipelinePluginLess&
                        operator=(const PipelinePluginLess&) = delete;

                        //
                        // Comparison operator that sorts pipeline plugins according to the
                        // position of their IDs in an ordered vector of plugin IDs.
                        //
                        // @param p_spPlugin1 First pipeline plugin to compare.
                        // @param p_spPlugin2 Second pipeline plugin to compare.
                        // @return true if p_spPlugin1 should be displayed before p_spPlugin2.
                        //
        bool            operator()(const PCC::PluginSP& p_spPlugin1,
                                   const PCC::PluginSP& p_spPlugin2) const
                        {
                            assert(p_spPlugin1 != nullptr);
                            assert(p_spPlugin2 != nullptr);

                            // Find each plugin ID in our ordered vector and compare the positions.
                            auto it1 = std::find(m_vOrderedPluginIds.cbegin(), m_vOrderedPluginIds.cend(), p_spPlugin1->Id());
                            auto it2 = std::find(m_vOrderedPluginIds.cbegin(), m_vOrderedPluginIds.cend(), p_spPlugin2->Id());
                            return it1 < it2;
                        }

    private:
        const PCC::GUIDV&
                        m_vOrderedPluginIds;
    };

#ifdef _DEBUG
    // Used to detect recursive revisions
    std::mutex  g_DebugRecursiveCheckMutex;
    bool        g_DebugIsRevising = false;
#endif // _DEBUG

} // anonymous namespace

namespace PCC
{
    //
    // Constructor. Opens the user key as an overrideable key (see class for details).
    // Also opens the plugins key in read/write mode if possible, otherwise read-only.
    //
    Settings::Settings()
        : COMPluginProvider(),
          PipelinePluginProvider(),
          m_UserKey(PCC_SETTINGS_KEY),
          m_IconsKey(PCC_ICONS_KEY),
          m_PipelinePluginsKey(PCC_PIPELINE_PLUGINS_KEY),
          m_TempPipelinePluginsKey(PCC_TEMP_PIPELINE_PLUGINS_KEY),
          m_UserPluginsKey(),
          m_GlobalPluginsKey(),
          m_GlobalPluginsKeyReadOnly(false),
          m_Revised(false)
    {
        // Open user plugins key.
        m_UserPluginsKey.Open(HKEY_CURRENT_USER, PCC_PLUGINS_KEY, true);

        // Open global plugins key.
        m_GlobalPluginsKey.Open(HKEY_LOCAL_MACHINE, PCC_PLUGINS_KEY, true);
        if (!m_GlobalPluginsKey.Valid()) {
            // Perhaps this is because we're running as a restricted user who doesn't
            // have full access to HKLM. Open as read-only.
            m_GlobalPluginsKey.Open(HKEY_LOCAL_MACHINE, PCC_PLUGINS_KEY, false, KEY_READ);
            m_GlobalPluginsKeyReadOnly = true;
        }
    }

    //
    // Checks whether user wants to consider hidden shares when
    // returning paths for the UNC plugins.
    //
    // @return true if hidden shares should be considered, false otherwise.
    //
    bool Settings::GetUseHiddenShares() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool useHiddenShares = SETTING_USE_HIDDEN_SHARES_DEFAULT;
        DWORD regUseHiddenShares = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_USE_HIDDEN_SHARES, regUseHiddenShares) == ERROR_SUCCESS) {
            useHiddenShares = regUseHiddenShares != 0;
        }
        return useHiddenShares;
    }

    //
    // Checks whether user wants us to add quotes around the paths
    // saved to the clipboard. This applies to all plugins.
    //
    // @return true if we should add quotes around the paths.
    //
    bool Settings::GetAddQuotesAroundPaths() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool addQuotes = SETTING_ADD_QUOTES_DEFAULT;
        DWORD regAddQuotes = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_ADD_QUOTES, regAddQuotes) == ERROR_SUCCESS) {
            addQuotes = regAddQuotes != 0;
        }
        return addQuotes;
    }

    //
    // Checks whether user wants us to turn the paths into e-mail links
    // by surrounding them with < and >. This applies to all plugins.
    //
    // @return true if we should make paths into e-mail links.
    //
    bool Settings::GetMakePathsIntoEmailLinks() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool makeEmailLinks = SETTING_MAKE_EMAIL_LINKS_DEFAULT;
        DWORD regMakeEmailLinks = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_MAKE_EMAIL_LINKS, regMakeEmailLinks) == ERROR_SUCCESS) {
            makeEmailLinks = regMakeEmailLinks != 0;
        }
        return makeEmailLinks;
    }

    //
    // Checks how user wants us to encode characters in path.
    // This applies to all plugins.
    //
    // @return Encode parameter.
    //
    StringUtils::EncodeParam Settings::GetEncodeParam() const
    {
        // Perform late-revising.
        Revise();

        std::wstring encodeParamStr;
        if (PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_ENCODE_PARAM, encodeParamStr) != ERROR_SUCCESS) {
            encodeParamStr = SETTING_ENCODE_PARAM_DEFAULT;
        }
        StringUtils::EncodeParam encodeParam = StringUtils::EncodeParam::None;
        if (encodeParamStr == SETTING_ENCODE_PARAM_VALUE_WHITESPACE) {
            encodeParam = StringUtils::EncodeParam::Whitespace;
        } else if (encodeParamStr == SETTING_ENCODE_PARAM_VALUE_ALL) {
            encodeParam = StringUtils::EncodeParam::All;
        } else {
            assert(encodeParamStr == SETTING_ENCODE_PARAM_VALUE_NONE);
        }
        return encodeParam;
    }

    //
    // Checks whether we want to show an icon next to the default plugin
    // in the contextual menu.
    //
    // @return true if we should show an icon next to default plugin.
    //
    bool Settings::GetUseIconForDefaultPlugin() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool useIcon = SETTING_USE_ICON_FOR_DEFAULT_PLUGIN_DEFAULT;
        DWORD regUseIcon = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_USE_ICON_FOR_DEFAULT_PLUGIN, regUseIcon) == ERROR_SUCCESS) {
            useIcon = regUseIcon != 0;
        }
        return useIcon;
    }

    //
    // Checks whether we want to show an icon next to the submenu in the contextual menu.
    //
    // @return true if we should show an icon next to submenu.
    //
    bool Settings::GetUseIconForSubmenu() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool useIcon = SETTING_USE_ICON_FOR_SUBMENU_DEFAULT;
        DWORD regUseIcon = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_USE_ICON_FOR_SUBMENU, regUseIcon) == ERROR_SUCCESS) {
            useIcon = regUseIcon != 0;
        }
        return useIcon;
    }

    //
    // Checks whether we want to show a preview of plugin results instead of plugin names.
    //
    // @return true if we should use preview mode.
    //
    bool Settings::GetUsePreviewMode() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool previewMode = SETTING_USE_PREVIEW_MODE_DEFAULT;
        DWORD regPreviewMode = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_USE_PREVIEW_MODE, regPreviewMode) == ERROR_SUCCESS) {
            previewMode = regPreviewMode != 0;
        }
        return previewMode;
    }

    //
    // Checks whether we should drop redundant words in plugin descriptions, like "copy".
    //
    // @return true if we should drop redundant words in plugin descriptions.
    //
    bool Settings::GetDropRedundantWords() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool dropRedundantWords = SETTING_DROP_REDUNDANT_WORDS_DEFAULT;
        DWORD regDropRedundantWords = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_DROP_REDUNDANT_WORDS, regDropRedundantWords) == ERROR_SUCCESS) {
            dropRedundantWords = regDropRedundantWords != 0;
        }
        return dropRedundantWords;
    }

    //
    // Checks whether we should always show the submenu with all plugins.
    // If not, we will only show the submenu when the Shift key is pressed.
    //
    // @return true to always show the submenu.
    //
    bool Settings::GetAlwaysShowSubmenu() const
    {
        // Perform late-revising.
        Revise();

        // Check if value exists. If so, read it, otherwise use default value.
        bool alwaysShowSubmenu = SETTING_ALWAYS_SHOW_SUBMENU_DEFAULT;
        DWORD regAlwaysShowSubmenu = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_ALWAYS_SHOW_SUBMENU, regAlwaysShowSubmenu) == ERROR_SUCCESS) {
            alwaysShowSubmenu = regAlwaysShowSubmenu != 0;
        }
        return alwaysShowSubmenu;
    }

    //
    // Returns the string to use between each path copied. An empty string
    // instructs PCC to use the default value (usually a newline).
    //
    // @return Separator between paths, or empty string to use default value.
    //
    std::wstring Settings::GetPathsSeparator() const
    {
        // Perform late-revising.
        Revise();

        std::wstring pathsSeparator;
        if (PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_PATHS_SEPARATOR, pathsSeparator) != ERROR_SUCCESS) {
            pathsSeparator.clear();
        }
        return pathsSeparator;
    }

    //
    // Returns the plugin to use when user opens the contextual menu
    // while holding down the Ctrl key.
    //
    // @param p_rPluginId Upon return, will contain the ID of the plugin
    //                    to use. If the method returns false, this is untouched.
    // @return true if we have a Ctrl key plugin and its ID was copied
    //         in p_rPluginId.
    //
    bool Settings::GetCtrlKeyPlugin(GUID& p_rPluginId) const
    {
        // Perform late-revising.
        Revise();

        bool hasPluginId = false;
        std::wstring pluginAsString;
        if (PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_CTRL_KEY_PLUGIN, pluginAsString) == ERROR_SUCCESS) {
            GUIDV vPluginIds = PluginUtils::StringToPluginIds(pluginAsString, PLUGINS_SEPARATOR);
            if (vPluginIds.size() == 1) {
                p_rPluginId = vPluginIds.front();
                hasPluginId = true;
            }
        }
        return hasPluginId;
    }

    //
    // Returns the display order of plugins in the main contextual menu.
    //
    // @param p_rvPluginIds Upon return, will contain a list of plugin IDs.
    //                      If the method returns false, this list is untouched.
    // @return true if there was a plugin display order in the settings and they
    //         were copied to p_rvPluginIds.
    //
    bool Settings::GetMainMenuPluginDisplayOrder(GUIDV& p_rvPluginIds) const
    {
        // Perform late-revising.
        Revise();

        std::wstring pluginsAsString;
        bool hasValues = PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_MAIN_MENU_PLUGIN_DISPLAY_ORDER, pluginsAsString) == ERROR_SUCCESS;
        if (hasValues) {
            p_rvPluginIds.clear();
            if (!pluginsAsString.empty()) {
                p_rvPluginIds = PluginUtils::StringToPluginIds(pluginsAsString, PLUGINS_SEPARATOR);
            }
        }
        return hasValues;
    }

    //
    // Returns the display order of plugins in the PCC contextual submenu.
    //
    // @param p_rvPluginIds Upon return, will contain a list of plugin IDs.
    //                      If the method returns false, this list is untouched.
    // @return true if there was a plugin display order in the settings and they
    //         were copied to p_rvPluginIds.
    //
    bool Settings::GetSubmenuPluginDisplayOrder(GUIDV& p_rvPluginIds) const
    {
        // Perform late-revising.
        Revise();

        std::wstring pluginsAsString;
        bool hasValues = PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_SUBMENU_PLUGIN_DISPLAY_ORDER, pluginsAsString) == ERROR_SUCCESS;
        if (hasValues) {
            p_rvPluginIds.clear();
            if (!pluginsAsString.empty()) {
                p_rvPluginIds = PluginUtils::StringToPluginIds(pluginsAsString, PLUGINS_SEPARATOR);
            }
        }
        return hasValues;
    }

    //
    // Returns the list of known plugins. This is updated by the settings app and
    // is used to track new plugins when displaying the menu.
    //
    // @param p_rvPluginIds Upon return, will contain a list of plugin IDs.
    //                      If the method returns false, this list is untouched.
    // @return true if there was a list of known plugins in the settings and it
    //         was copied to p_rvPluginIds.
    //
    bool Settings::GetKnownPlugins(GUIDV& p_rvPluginIds) const
    {
        // Perform late-revising.
        Revise();

        std::wstring pluginsAsString;
        bool hasValues = PluginUtils::ReadRegistryStringValue(m_UserKey, SETTING_KNOWN_PLUGINS, pluginsAsString) == ERROR_SUCCESS;
        if (hasValues) {
            p_rvPluginIds.clear();
            if (!pluginsAsString.empty()) {
                p_rvPluginIds = PluginUtils::StringToPluginIds(pluginsAsString, PLUGINS_SEPARATOR);
            }
        }
        return hasValues;
    }

    //
    // Checks if we need to perform a software update check according to the last time we did.
    //
    // @return true if an update check is needed.
    //
    bool Settings::NeedsUpdateCheck() const
    {
        // Perform late-revising.
        Revise();

        bool needsUpdate = false;

        // Check if software update is disabled.
        bool updateDisabled = SETTING_DISABLE_SOFTWARE_UPDATE_DEFAULT;
        DWORD storedUpdateDisabled = 0;
        if (m_UserKey.QueryDWORDValue(SETTING_DISABLE_SOFTWARE_UPDATE, storedUpdateDisabled) == ERROR_SUCCESS) {
            updateDisabled = storedUpdateDisabled != 0;
        }
        if (!updateDisabled) {
            // Get last time it was performed. If we don't have info on that, assume that it's been a hell of a while.
            __time64_t lastUpdateCheck;
            ULONGLONG storedLastUpdate = 0;
            if (m_UserKey.QueryQWORDValue(SETTING_LAST_UPDATE_CHECK, storedLastUpdate) == ERROR_SUCCESS) {
                lastUpdateCheck = static_cast<__time64_t>(storedLastUpdate);
            } else {
                lastUpdateCheck = 0;
            }

            // Get current time.
            __time64_t now;
            ::_time64(&now);

            // Get update interval.
            double updateInterval = SETTING_UPDATE_INTERVAL_DEFAULT;
            DWORD storedUpdateInterval = 0;
            if (m_UserKey.QueryDWORDValue(SETTING_UPDATE_INTERVAL, storedUpdateInterval) == ERROR_SUCCESS) {
                updateInterval = static_cast<double>(storedUpdateInterval);
            }

            // Compare and check if enough time has passed.
            needsUpdate = ::_difftime64(now, lastUpdateCheck) >= updateInterval;
        }

        return needsUpdate;
    }

    //
    // Sets a value in the settings indicating that we performed a software update check now.
    // This will enable the NeedsUpdateCheck method to compare up to this point.
    //
    void Settings::SetLastUpdateCheckNow()
    {
        // Perform late-revising.
        Revise();

        __time64_t now;
        ::_time64(&now);
        m_UserKey.SetQWORDValue(SETTING_LAST_UPDATE_CHECK, static_cast<ULONGLONG>(now));
    }

    //
    // Looks in the Icons key and attempts to find an icon file for the given plugin.
    //
    // @param p_PluginId ID of plugin to look for.
    // @return If an icon file exists for this plugin, will return its path. If the
    // default icon is to be used, will return an empty string. If no icon is specified
    // for this plugin, will return a value whose has_value() method returns false.
    //
    cl::optional<std::wstring> Settings::GetIconFileForPlugin(const CLSID& p_PluginId) const
    {
        // Perform late-revising.
        Revise();

        // Assume there's no info for the icon.
        cl::optional<std::wstring> resultingIconFile;

        // Convert plugin ID to string.
        StOleStr pluginIdAsString;
        HRESULT hRes = ::StringFromCLSID(p_PluginId, &pluginIdAsString);
        if (SUCCEEDED(hRes)) {
            // Look for a value for this plugin in the icons key.
            std::wstring iconFile;
            if (PluginUtils::ReadRegistryStringValue(m_IconsKey, pluginIdAsString, iconFile) == ERROR_SUCCESS) {
                // Check if it's the marker value pointing to the default icon.
                if (iconFile == DEFAULT_ICON_MARKER_STRING) {
                    // Return an empty string to note this.
                    resultingIconFile.emplace();
                } else {
                    // Return the value, which should point to icon file.
                    resultingIconFile = iconFile;
                }
            }
        } else {
            throw SettingsException(static_cast<LONG>(hRes));
        }

        return resultingIconFile;
    }

    //
    // Checks whether the administrator has disabled editing of the
    // settings by the settings app.
    //
    // @return true if editing is disabled.
    //
    bool Settings::GetEditingDisabled() const
    {
        // No need to revise for this because this cannot change.
        return m_UserKey.Locked();
    }

    //
    // Returns the list of COM plugins registered to be used with Path Copy Copy.
    //
    // @return IDs of COM plugin classes.
    //
    CLSIDV Settings::GetCOMPlugins() const
    {
        // Perform late-revising.
        Revise();

        // Fetch COM plugins from both user and global key.
        CLSIDV vPluginIds;
        GetCOMPlugins(m_UserPluginsKey, vPluginIds);
        if (m_GlobalPluginsKey.Valid()) {
            GetCOMPlugins(m_GlobalPluginsKey, vPluginIds);
        }

        // Now sort them and remove duplicates since a plugin might be registered
        // globally AND for the current user.
        std::sort(vPluginIds.begin(), vPluginIds.end(), CLSIDLess());
        vPluginIds.erase(std::unique(vPluginIds.begin(), vPluginIds.end(), CLSIDEqualTo()), vPluginIds.end());

        return vPluginIds;
    }

    //
    // Registers a plugin in the Path Copy Copy registry so that it is used in the contextual menu.
    //
    // @param p_CLSID ID of plugin COM class.
    // @param p_User Whether to register the plugin per-user (true) or per-machine (false).
    // @return true if the plugin has been registered, false it if was already registered.
    //
    bool Settings::RegisterCOMPlugin(const CLSID& p_CLSID,
                                     const bool p_User)
    {
        // Do not revise here. COM plugins that try to register in their DllRegisterServer
        // will be called back when we revise.

        bool registered = false;

        if (p_User || !m_GlobalPluginsKeyReadOnly) {
            // Get proper key depending on target.
            AtlRegKey& rKey = p_User ? m_UserPluginsKey : m_GlobalPluginsKey;

            // Convert CLSID to string.
            StOleStr clsidAsString;
            HRESULT hRes = ::StringFromCLSID(p_CLSID, &clsidAsString);
            if (SUCCEEDED(hRes)) {
                // Check if plugin was already registered.
                DWORD valueType = 0;
                if (::RegQueryValueExW(rKey.GetHKEY(), clsidAsString.Get(), 0, &valueType, 0, 0) != ERROR_SUCCESS) {
                    // Register plugin.
                    rKey.SetStringValue(clsidAsString.Get(), GetCOMPluginInfo(p_CLSID).c_str());

                    // We successfully registered.
                    registered = true;
                }
            } else {
                throw SettingsException(static_cast<LONG>(hRes));
            }
        }

        return registered;
    }

    //
    // Unregisters a plugin, removing it from our registry.
    //
    // @param p_CLSID ID of plugin COM class.
    // @param p_User Whether the plugin was registered per-user (true) or per-machine (false).
    // @return true if plugin has been unregistered, false if it wasn't registered.
    //
    bool Settings::UnregisterCOMPlugin(const CLSID& p_CLSID,
                                       const bool p_User)
    {
        // Do not revise here. COM plugins that try to register in their DllRegisterServer
        // will be called back when we revise.

        bool unregistered = false;

        if (p_User || !m_GlobalPluginsKeyReadOnly) {
            // Get proper key depending on target.
            AtlRegKey& rKey = p_User ? m_UserPluginsKey : m_GlobalPluginsKey;

            // Convert CLSID to string.
            StOleStr clsidAsString;
            HRESULT hRes = ::StringFromCLSID(p_CLSID, &clsidAsString);
            if (SUCCEEDED(hRes)) {
                // Unregister the plugin and check if it worked in one swoop.
                unregistered = rKey.DeleteValue(clsidAsString.Get()) == ERROR_SUCCESS;
            } else {
                throw SettingsException(static_cast<LONG>(hRes));
            }
        }

        return unregistered;
    }

    //
    // Loads pipeline plugins found in the registry as saved there by the settings app.
    //
    // @param p_rvspPlugins Where to save all pipeline plugins.
    //
    void Settings::GetPipelinePlugins(PluginSPV& p_rvspPlugins) const
    {
        // Perform late-revising.
        Revise();

        GetPipelinePlugins(m_PipelinePluginsKey, p_rvspPlugins, true);
    }

    //
    // Loads temporary pipeline plugins found in the registry as saved there by the settings app.
    // Such plugins are used to execute pipeline plugins not saved yet.
    //
    // @param p_rvspPlugins Where to save all pipeline plugins.
    //
    void Settings::GetTempPipelinePlugins(PluginSPV& p_rvspPlugins) const
    {
        // Perform late-revising.
        Revise();

        GetPipelinePlugins(m_TempPipelinePluginsKey, p_rvspPlugins, false);
    }

    //
    // Applies revisions to the user config registry key immediately instead of
    // doing it lazily the next time settings are accessed.
    //
    void Settings::ApplyRevisions()
    {
        // Force revise immediately
        Revise();
    }

    //
    // Applies revisions to the global config registry key. This must be called by
    // a user that has the proper access rights on the global key.
    //
    void Settings::ApplyGlobalRevisions()
    {
        // Open global registry keys. If user key doesn't exist or we don't have access, bail out.
        AtlRegKey globalUserKey(HKEY_LOCAL_MACHINE, PCC_SETTINGS_KEY, false);
        AtlRegKey globalPluginsKey(HKEY_LOCAL_MACHINE, PCC_PLUGINS_KEY, false);
        AtlRegKey globalPipelinePluginsKey(HKEY_LOCAL_MACHINE, PCC_PIPELINE_PLUGINS_KEY, false);
        if (globalUserKey.Valid()) {
            // We got the global key, apply all revisions.
            RegCOMPluginProvider comPluginProvider(globalPluginsKey);
            Reviser::ApplyRevisions(globalUserKey, globalPipelinePluginsKey, comPluginProvider);
        }
    }

    //
    // Performs config revisions on the first call. This is done to avoid
    // revising when we don't need to.
    //
    void Settings::Revise() const
    {
        if (!m_Revised) {
            m_Revised = true;
            Reviser::ApplyRevisions(m_UserKey, m_PipelinePluginsKey, *this);
        }
    }

    //
    // Returns the info to write in the value for a registered
    // COM plugin. This info is no longer used by the UI, but left
    // for backward-compatibility purposes in case plugins depend on it.
    //
    // @param p_CLSID ID of the plugin COM class.
    // @return Info string.
    //
    std::wstring Settings::GetCOMPluginInfo(const CLSID& p_CLSID)
    {
        // The format is:
        //
        // <group_id>,<group_pos>|<description>
        //
        // If the plugin doesn't support group info, that will amount to
        //
        // |<description>
        //
        // Note that the UI now uses the COMPluginExecutor to fetch this
        // information; however we'll keep writing it for backward-compatibility.

        std::wostringstream wos;

        ATL::CComPtr<IPathCopyCopyPlugin> cpPlugin;
        if (SUCCEEDED(cpPlugin.CoCreateInstance(p_CLSID)) && cpPlugin.p != nullptr) {
            // First check for group info.
            ATL::CComQIPtr<IPathCopyCopyPluginGroupInfo> cpPluginGI(cpPlugin);
            if (cpPluginGI.p != nullptr) {
                ULONG groupId = 0, groupPos = 0;
                if (SUCCEEDED(cpPluginGI->get_GroupId(&groupId)) &&
                    SUCCEEDED(cpPluginGI->get_GroupPosition(&groupPos))) {

                    wos << groupId << INFO_GROUP_INFO_SEPARATOR << groupPos;
                }
            }

            // Now include description.
            ATL::CComBSTR bstrDesc;
            wos << INFO_DESCRIPTION_SEPARATOR;
            if (SUCCEEDED(cpPlugin->get_Description(&bstrDesc)) && bstrDesc.m_str != nullptr) {
                wos << bstrDesc.m_str;
            }
        }

        std::wstring s = wos.str();
        return s;
    }

    //
    // Static method that returns the list of COM plugins registered in
    // the given registry key.
    //
    // @param p_PluginsKey Registry key containing registered COM plugins.
    // @param p_rvCLSIDs Where to store the CLSIDs of COM plugins.
    //
    void Settings::GetCOMPlugins(const RegKey& p_PluginsKey,
                                 CLSIDV& p_rvCLSIDs)
    {
        // Plugins are stored as value names in the plugins registry keys.
        RegKey::ValueInfoV vValues;
        p_PluginsKey.GetValues(vValues);
        for (const auto& valueInfo : vValues) {
            // Check if this value name is a CLSID.
            CLSID clsid;
            if (SUCCEEDED(::CLSIDFromString(valueInfo.m_ValueName.c_str(), &clsid))) {
                // It is, copy it to vector.
                p_rvCLSIDs.push_back(clsid);
            }
        }
    }

    //
    // Static method that loads pipeline plugins found in the given pipeline plugins registry key.
    //
    // @param p_PipelinePluginsKey Registry key containing pipeline plugins to load.
    // @param p_rvspPlugins Where to save all pipeline plugins.
    // @param p_AddSeparator Whether to add a separator before pipeline plugins if
    //                       p_rvspPlugins already contains plugins.
    //
    void Settings::GetPipelinePlugins(const RegKey& p_PipelinePluginsKey,
                                      PluginSPV& p_rvspPlugins,
                                      const bool p_AddSeparator)
    {
        // Pipeline plugins are stored in the registry in this way:
        //
        // clechasseur\PathCopyCopy
        // \- <pipeline plugins key>
        //    |   val DisplayOrder = <guid>,<guid>...
        //    \- <guid>
        //    |     val '' = <encoded pipeline>
        //    |     val Description = <description>
        //    |     val IconFile = <optional path to icon file>
        //    \- <guid>
        //          ...
        //
        // We'll need to enumerate the subkeys of the pipeline plugins key to find plugins.
        PluginSPV vspPipelinePlugins;
        RegKey::SubkeyInfoV vSubkeyInfos;
        p_PipelinePluginsKey.GetSubKeys(vSubkeyInfos);
        for (const auto& subkeyInfo : vSubkeyInfos) {
            // Convert key name into a GUID and make sure it's valid.
            GUID pluginId = { 0 };
            if (::CLSIDFromString(const_cast<wchar_t*>(subkeyInfo.m_KeyName.c_str()), &pluginId) == S_OK) {
                // Get values for the pipeline encoded elements as well as the plugin description
                // and its optional icon file.
                std::wstring encodedElements, description, iconFile;
                bool useDefaultIcon = false;
                AtlRegKey pluginKey;
                LONG res = pluginKey.Open(subkeyInfo.m_hParent, subkeyInfo.m_KeyName.c_str(), false, KEY_QUERY_VALUE);
                if (res == ERROR_SUCCESS) {
                    res = PluginUtils::ReadRegistryStringValue(pluginKey, SETTING_PIPELINE_DESCRIPTION, description);
                }
                if (res == ERROR_SUCCESS) {
                    res = PluginUtils::ReadRegistryStringValue(pluginKey, nullptr, encodedElements);
                }
                if (res == ERROR_SUCCESS) {
                    // Icon file is optional. If not found, we're not displaying any icon.
                    LONG iconRes = PluginUtils::ReadRegistryStringValue(pluginKey, SETTING_PIPELINE_ICON_FILE, iconFile);
                    if (iconRes == ERROR_SUCCESS && iconFile.empty()) {
                        // This indicates that we want to use the default icon.
                        useDefaultIcon = true;
                    }
                }
                if (res == ERROR_SUCCESS) {
                    // We have all the info we need, create the plugin and add it to the temp list.
                    vspPipelinePlugins.push_back(std::make_shared<PCC::Plugins::PipelinePlugin>(
                        pluginId, description, iconFile, useDefaultIcon, encodedElements));
                }
            }
        }

        // Get value containing the display order. If found, we'll have to reorder the
        // pipeline plugins according to this value.
        std::wstring displayOrder;
        LONG res = PluginUtils::ReadRegistryStringValue(p_PipelinePluginsKey, SETTING_PIPELINE_DISPLAY_ORDER, displayOrder);
        if (res == ERROR_SUCCESS && !displayOrder.empty()) {
            // The value contains a comma-separated list of pipeline plugin IDs.
            GUIDV vOrderedPluginIds = PluginUtils::StringToPluginIds(displayOrder, PLUGINS_SEPARATOR);

            // Sort plugins using our special predicate that will order them properly.
            PipelinePluginLess lessPredicate(vOrderedPluginIds);
            std::sort(vspPipelinePlugins.begin(), vspPipelinePlugins.end(), lessPredicate);
        }

        // If we have pipeline plugins, insert them in the provided return vector.
        // Add a separator as appropriate.
        if (!vspPipelinePlugins.empty() && p_AddSeparator &&
            !p_rvspPlugins.empty() && !p_rvspPlugins.back()->IsSeparator()) {

            p_rvspPlugins.push_back(std::make_shared<PluginSeparator>());
        }
        std::move(vspPipelinePlugins.begin(), vspPipelinePlugins.end(), std::back_inserter(p_rvspPlugins));
    }

    //
    // Applies a series of revisions to the config. Revisions are used to update the config
    // when the structure changes. Every time this happens, add a revision to the map in
    // CreateRevisionFuncMap to ensure the data is updated before settings are accessed.
    //
    // @param p_rUserKey Config registry key containing user settings.
    // @param p_rPluginsKey p_rPipelinePluginsKey Config registry key containing pipeline plugins.
    // @param p_COMPluginProvider Object to access COM plugins.
    //
    void Settings::Reviser::ApplyRevisions(RegKey& p_rUserKey,
                                           RegKey& p_rPipelinePluginsKey,
                                           const COMPluginProvider& p_COMPluginProvider)
    {
#ifdef _DEBUG
        // Make sure we're not calling this recursively otherwise
        // we'll end up in an infinite loop of revising.
        {
            std::lock_guard<std::mutex> debugLock(g_DebugRecursiveCheckMutex);
            assert(!g_DebugIsRevising);
            g_DebugIsRevising = true;
        }
        struct ResetIsRevising {
            ~ResetIsRevising() {
                std::lock_guard<std::mutex> debugLock(g_DebugRecursiveCheckMutex);
                assert(g_DebugIsRevising);
                g_DebugIsRevising = false;
            }
        } _resetIsRevisingAtEndOfScope;
#endif // _DEBUG

        // Create bean to store revise info.
        ReviseInfo reviseInfo(p_rUserKey, p_rPipelinePluginsKey, p_COMPluginProvider);

        // Get map of revise functions.
        RevisionFuncM mRevisions = CreateRevisionFuncMap();

        // Read value of already-applied revisions.
        UInt32V vCurrentRevisions;
        std::wstring currentRevisionsAsString;
        if (PluginUtils::ReadRegistryStringValue(p_rUserKey, SETTING_REVISIONS, currentRevisionsAsString) == ERROR_SUCCESS) {
            vCurrentRevisions = PluginUtils::StringToUInt32s(currentRevisionsAsString, REVISIONS_SEPARATOR);
        }

        // Sort the list of already-applied revisions so that we can perform quick finds.
        std::sort(vCurrentRevisions.begin(), vCurrentRevisions.end());

        // Look for and apply missing revisions.
        UInt32V vNewRevisions(vCurrentRevisions);
        for (const auto& revisionAndFunc : mRevisions) {
            const uint32_t revision = revisionAndFunc.first;
            const RevisionFunc& revisionFunc = revisionAndFunc.second;

            if (!std::binary_search(vCurrentRevisions.cbegin(), vCurrentRevisions.cend(), revision)) {
                revisionFunc(reviseInfo);
                vNewRevisions.push_back(revision);
            }
        }

        // Check if we applied some new revisions. If so, save them.
        if (vNewRevisions.size() != vCurrentRevisions.size()) {
            std::wstring newRevisionsAsString = PluginUtils::UInt32sToString(vNewRevisions, REVISIONS_SEPARATOR);
            p_rUserKey.SetStringValue(SETTING_REVISIONS, newRevisionsAsString.c_str());
        }
    }

    //
    // Static method to create the map of revise functions.
    //
    // @return Map of revisions and their associated functions to apply them.
    //
    Settings::Reviser::RevisionFuncM Settings::Reviser::CreateRevisionFuncMap()
    {
        RevisionFuncM mRevisions;

        mRevisions.emplace(201201311ul, &ApplyNewPluginOrganizationRevision201201311);
        mRevisions.emplace(201204051ul, &ApplyPluginsInMainMenuAdjustment201204051);
        // Note: 201601051ul was used at some point; do not use again.
        mRevisions.emplace(201601052ul, &ApplyInitialMainMenuPluginDisplayOrder201601052);
        mRevisions.emplace(201601053ul, &ApplyInitialSubmenuPluginDisplayOrder201601053);
        mRevisions.emplace(201601054ul, &ApplyInitialKnownPlugins201601054);
        mRevisions.emplace(201707061ul, &ApplyInitialUIPluginDisplayOrder201707061);

        // Add any new revisions here.
        
        return mRevisions;
    }

    //
    // Revises the config by changing legacy settings for disabled and default plugins
    // to the new format of "in the main menu"/"not in the contextual menu".
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyNewPluginOrganizationRevision201201311(const ReviseInfo& p_ReviseInfo)
    {
        // The old format used the following registry values:
        // - DisabledPlugins: comma-separated list of plugins to remove from submenu.
        // - DefaultPlugin: ID of plugin to show in the main menu.
        //
        // The new structure uses the following values instead:
        // - PluginsNotInSubmenu: comma-separated list of plugins to remove from submenu.
        // - PluginsInMainMenu: comma-separated list of plugins to show in the main menu.
        //
        // To revise:
        // - Copy value of "DisabledPlugins" (if there is one) to "PluginsNotInSubmenu"
        // - Copy value of "DefaultPlugin" (if there is one) to "PluginsInMainMenu"

        DWORD dummyValueType = 0;
        if (p_ReviseInfo.m_rUserKey.QueryValue(OLD_SETTING_PLUGINS_NOT_IN_SUBMENU, &dummyValueType, 0, 0) == ERROR_FILE_NOT_FOUND) {
            std::wstring disabledPluginsAsString;
            if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, OLD_SETTING_DISABLED_PLUGINS, disabledPluginsAsString) == ERROR_SUCCESS) {
                p_ReviseInfo.m_rUserKey.SetStringValue(OLD_SETTING_PLUGINS_NOT_IN_SUBMENU, disabledPluginsAsString.c_str());
                p_ReviseInfo.m_rUserKey.DeleteValue(OLD_SETTING_DISABLED_PLUGINS);
            }
        }
        if (p_ReviseInfo.m_rUserKey.QueryValue(OLD_SETTING_PLUGINS_IN_MAIN_MENU, &dummyValueType, 0, 0) == ERROR_FILE_NOT_FOUND) {
            std::wstring defaultPluginAsString;
            if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, OLD_SETTING_DEFAULT_PLUGIN, defaultPluginAsString) == ERROR_SUCCESS) {
                p_ReviseInfo.m_rUserKey.SetStringValue(OLD_SETTING_PLUGINS_IN_MAIN_MENU, defaultPluginAsString.c_str());
                p_ReviseInfo.m_rUserKey.DeleteValue(OLD_SETTING_DEFAULT_PLUGIN);
            }
        }
    }

    //
    // Revises the config by making it possible to specify an empty list of plugins in main menu
    // and make this different from not having a registry value at all.
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyPluginsInMainMenuAdjustment201204051(const ReviseInfo& p_ReviseInfo)
    {
        // Formely, an empty list of plugins in main menu was the same as if the value
        // did not exist: we used the default plugin. Now, however, we will only use
        // the default value if no registry value exists; an empty list will be honored.
        // So if we currently have an empty list, delete the value.
        std::wstring pluginsList;
        if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, OLD_SETTING_PLUGINS_IN_MAIN_MENU, pluginsList) == ERROR_SUCCESS && pluginsList.empty()) {
            p_ReviseInfo.m_rUserKey.DeleteValue(OLD_SETTING_PLUGINS_IN_MAIN_MENU);
        }
    }

    //
    // Revises the config by generating a config value for MainMenuDisplayOrder from the
    // former value of PluginsInMainMenu.
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyInitialMainMenuPluginDisplayOrder201601052(const ReviseInfo& p_ReviseInfo)
    {
        // Check if user had a previous value specifying plugins to display in the main menu.
        std::wstring oldPluginsInMainMenuAsString;
        if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, OLD_SETTING_PLUGINS_IN_MAIN_MENU, oldPluginsInMainMenuAsString) == ERROR_SUCCESS) {
            // Sort plugin IDs in main menu to make it easier to perform find operations.
            GUIDV vOldPluginsInMainMenu = PluginUtils::StringToPluginIds(oldPluginsInMainMenuAsString, PLUGINS_SEPARATOR);
            GUIDLess guidLess;
            std::sort(vOldPluginsInMainMenu.begin(), vOldPluginsInMainMenu.end(), guidLess);

            // Fetch all plugins in default order.
            RegPipelinePluginProvider pipelinePluginProvider(p_ReviseInfo.m_rPipelinePluginsKey);
            PluginSPV vspPlugins = PluginsRegistry::GetPluginsInDefaultOrder(
                &p_ReviseInfo.m_COMPluginProvider, &pipelinePluginProvider, false);

            // Scan plugins and keep all those that are to be included in main menu.
            // Do not keep separators (there shouldn't be any but just to be sure).
            GUIDV vMainMenuPluginDisplayOrder;
            for (const PluginSP& spPlugin : vspPlugins) {
                if (!spPlugin->IsSeparator() &&
                    std::binary_search(vOldPluginsInMainMenu.cbegin(), vOldPluginsInMainMenu.cend(), spPlugin->Id(), guidLess)) {

                    vMainMenuPluginDisplayOrder.push_back(spPlugin->Id());
                }
            }

            // Store the resulting list of plugins in the new setting value.
            std::wstring mainMenuPluginDisplayOrderAsString =
                PluginUtils::PluginIdsToString(vMainMenuPluginDisplayOrder, PLUGINS_SEPARATOR);
            p_ReviseInfo.m_rUserKey.SetStringValue(SETTING_MAIN_MENU_PLUGIN_DISPLAY_ORDER, mainMenuPluginDisplayOrderAsString.c_str());

            // Delete the legacy value.
            p_ReviseInfo.m_rUserKey.DeleteValue(OLD_SETTING_PLUGINS_IN_MAIN_MENU);
        }
    }

    //
    // Revises the config by generating a config value for SubmenuDisplayOrder from the
    // former value of PluginsNotInSubmenu.
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyInitialSubmenuPluginDisplayOrder201601053(const ReviseInfo& p_ReviseInfo)
    {
        // Check if user had a previous value specifying plugins NOT to display in the submenu.
        std::wstring oldPluginsNotInSubmenuAsString;
        if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, OLD_SETTING_PLUGINS_NOT_IN_SUBMENU, oldPluginsNotInSubmenuAsString) == ERROR_SUCCESS) {
            // Sort plugin IDs not in submenu to make it easier to perform find operations.
            GUIDV vOldPluginsNotInSubmenu = PluginUtils::StringToPluginIds(oldPluginsNotInSubmenuAsString, PLUGINS_SEPARATOR);
            GUIDLess guidLess;
            std::sort(vOldPluginsNotInSubmenu.begin(), vOldPluginsNotInSubmenu.end(), guidLess);

            // Fetch all plugins in default order.
            RegPipelinePluginProvider pipelinePluginProvider(p_ReviseInfo.m_rPipelinePluginsKey);
            PluginSPV vspPlugins = PluginsRegistry::GetPluginsInDefaultOrder(
                &p_ReviseInfo.m_COMPluginProvider, &pipelinePluginProvider, false);

            // Scan plugins and keep them all EXCEPT those that should not be displayed in the submenu.
            // Also keep separators, but be careful not to double them.
            GUIDV vSubmenuPluginDisplayOrder;
            for (const PluginSP& spPlugin : vspPlugins) {
                if (!spPlugin->IsSeparator()) { 
                    if (!std::binary_search(vOldPluginsNotInSubmenu.cbegin(), vOldPluginsNotInSubmenu.cend(), spPlugin->Id(), guidLess)) {
                        // This plugin should be included in the submenu.
                        vSubmenuPluginDisplayOrder.push_back(spPlugin->Id());
                    }
                } else {
                    // Careful not to double separators.
                    if (!vSubmenuPluginDisplayOrder.empty() && !::IsEqualGUID(vSubmenuPluginDisplayOrder.back(), spPlugin->Id())) {
                        vSubmenuPluginDisplayOrder.push_back(spPlugin->Id());
                    }
                }
            }

            // Careful not to leave a separator at the end.
            if (!vSubmenuPluginDisplayOrder.empty() && ::IsEqualGUID(vSubmenuPluginDisplayOrder.back(), PluginSeparator().Id())) {
                vSubmenuPluginDisplayOrder.pop_back();
            }

            // Store the resulting list of plugins in the new setting value.
            std::wstring submenuPluginDisplayOrderAsString =
                PluginUtils::PluginIdsToString(vSubmenuPluginDisplayOrder, PLUGINS_SEPARATOR);
            p_ReviseInfo.m_rUserKey.SetStringValue(SETTING_SUBMENU_PLUGIN_DISPLAY_ORDER, submenuPluginDisplayOrderAsString.c_str());

            // Whatever happens, delete the old value.
            p_ReviseInfo.m_rUserKey.DeleteValue(OLD_SETTING_PLUGINS_NOT_IN_SUBMENU);
        }
    }

    //
    // Revises the config by generating the initial list of known plugins if needed.
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyInitialKnownPlugins201601054(const ReviseInfo& p_ReviseInfo)
    {
        // Don't overwrite an existing list, since it's updated by the settings app.
        DWORD dummyValueType = 0;
        if (p_ReviseInfo.m_rUserKey.QueryValue(SETTING_KNOWN_PLUGINS, &dummyValueType, nullptr, nullptr) == ERROR_FILE_NOT_FOUND) {
            // Get set of all plugins and convert it to vector of plugin IDs.
            RegPipelinePluginProvider pipelinePluginProvider(p_ReviseInfo.m_rPipelinePluginsKey);
            PluginSPV vspPlugins = PluginsRegistry::GetPluginsInDefaultOrder(
                &p_ReviseInfo.m_COMPluginProvider, &pipelinePluginProvider, false);
            PluginSPS sspAllPlugins(vspPlugins.cbegin(), vspPlugins.cend());
            GUIDV vPluginIds;
            vPluginIds.reserve(sspAllPlugins.size());
            for (const PluginSP& spPlugin : sspAllPlugins) {
                vPluginIds.push_back(spPlugin->Id());
            }

            // Store list of plugin IDs in registry.
            std::wstring knownPluginsAsString = PluginUtils::PluginIdsToString(vPluginIds, PLUGINS_SEPARATOR);
            p_ReviseInfo.m_rUserKey.SetStringValue(SETTING_KNOWN_PLUGINS, knownPluginsAsString.c_str());
        }
    }

    //
    // Revises the config by generating the initial UI plugin display order from the submenu display order.
    //
    // @param p_ReviseInfo Bean containing objects to perform revision.
    //
    void Settings::Reviser::ApplyInitialUIPluginDisplayOrder201707061(const ReviseInfo& p_ReviseInfo)
    {
        // Don't overwrite an existing list, since it's updated by the settings app.
        DWORD dummyValueType = 0;
        if (p_ReviseInfo.m_rUserKey.QueryValue(SETTING_UI_PLUGIN_DISPLAY_ORDER, &dummyValueType, nullptr, nullptr) == ERROR_FILE_NOT_FOUND) {
            // Load submenu plugin display order. If not found, then leave UI display order empty also.
            std::wstring submenuDisplayOrderAsString;
            if (PluginUtils::ReadRegistryStringValue(p_ReviseInfo.m_rUserKey, SETTING_SUBMENU_PLUGIN_DISPLAY_ORDER, submenuDisplayOrderAsString) == ERROR_SUCCESS) {
                // Here's the algo:
                // 1. Take all plugins displayed in the submenu, in order
                // 2. Load all plugins in default order
                // 3. Take all plugins not displayed in the submenu, in order

                // Convert submenu display order to list of plugin IDs. This is usable as-is.
                GUIDV vPluginIds = PluginUtils::StringToPluginIds(submenuDisplayOrderAsString, PLUGINS_SEPARATOR);

                // Create set of plugin IDs in submenu for easier lookups.
                GUIDS sPluginIdsInSubmenu(vPluginIds.cbegin(), vPluginIds.cend());

                // Load plugins in default order.
                RegPipelinePluginProvider pipelinePluginProvider(p_ReviseInfo.m_rPipelinePluginsKey);
                PluginSPV vspPlugins = PluginsRegistry::GetPluginsInDefaultOrder(
                    &p_ReviseInfo.m_COMPluginProvider, &pipelinePluginProvider, false);

                // Keep all plugins in default order that are not already in the list.
                // Be careful not to double up separators.
                for (const PluginSP& spPlugin : vspPlugins) {
                    if (!spPlugin->IsSeparator()) { 
                        if (sPluginIdsInSubmenu.find(spPlugin->Id()) == sPluginIdsInSubmenu.end()) {
                            vPluginIds.emplace_back(spPlugin->Id());
                        }
                    } else {
                        // Careful not to double separators.
                        if (!vPluginIds.empty() && !::IsEqualGUID(vPluginIds.back(), spPlugin->Id())) {
                            vPluginIds.emplace_back(spPlugin->Id());
                        }
                    }
                }

                // Store list of plugin IDs in registry.
                std::wstring uiDisplayOrderAsString = PluginUtils::PluginIdsToString(vPluginIds, PLUGINS_SEPARATOR);
                p_ReviseInfo.m_rUserKey.SetStringValue(SETTING_UI_PLUGIN_DISPLAY_ORDER, uiDisplayOrderAsString.c_str());
            }
        }
    }

    //
    // Constructor.
    //
    // @param p_rUserKey Config registry key containing user settings.
    // @param p_rPluginsKey p_rPipelinePluginsKey Config registry key containing pipeline plugins.
    // @param p_COMPluginProvider Object to access COM plugins.
    //
    Settings::Reviser::ReviseInfo::ReviseInfo(RegKey& p_rUserKey,
                                              RegKey& p_rPipelinePluginsKey,
                                              const COMPluginProvider& p_COMPluginProvider)
        : m_rUserKey(p_rUserKey),
          m_rPipelinePluginsKey(p_rPipelinePluginsKey),
          m_COMPluginProvider(p_COMPluginProvider)
    {
    }

    //
    // Constructor.
    //
    // @param p_PluginsKey Registry key storing (COM) plugins.
    //
    Settings::RegCOMPluginProvider::RegCOMPluginProvider(const RegKey& p_PluginsKey)
        : COMPluginProvider(),
          m_PluginsKey(p_PluginsKey)
    {
    }

    //
    // Returns a list of registered COM plugins.
    //
    // @return CLSIDs of registered COM plugins.
    //
    CLSIDV Settings::RegCOMPluginProvider::GetCOMPlugins() const
    {
        CLSIDV vPluginIds;
        if (m_PluginsKey.Valid()) {
            Settings::GetCOMPlugins(m_PluginsKey, vPluginIds);
        }
        return vPluginIds;
    }

    //
    // Constructor.
    //
    // @param p_PipelinePluginsKey Registry key containing pipeline plugins.
    //
    Settings::RegPipelinePluginProvider::RegPipelinePluginProvider(const RegKey& p_PipelinePluginsKey)
        : PipelinePluginProvider(),
          m_PipelinePluginsKey(p_PipelinePluginsKey)
    {
    }

    //
    // Returns pipeline plugins stored in our registry key.
    //
    // @param p_rvspPlugins Where to store pipeline plugins. If the vector already contains
    //                      plugins, a separator will be added.
    //
    void Settings::RegPipelinePluginProvider::GetPipelinePlugins(PluginSPV& p_rvspPlugins) const
    {
        if (m_PipelinePluginsKey.Valid()) {
            Settings::GetPipelinePlugins(m_PipelinePluginsKey, p_rvspPlugins, true);
        }
    }

    //
    // Returns temporary pipeline plugins. In our case, we don't support any.
    //
    // @param p_rvspPlugins Where to store pipeline plugins; unused.
    //
    void Settings::RegPipelinePluginProvider::GetTempPipelinePlugins(PluginSPV& /*p_rvspPlugins*/) const
    {
    }

    //
    // Constructor with error code.
    //
    // @param p_ErrorCode The Windows error code associated with the exception.
    //
    SettingsException::SettingsException(const LONG p_ErrorCode)
        : std::exception(),
          m_ErrorCode(p_ErrorCode)
    {
    }

    //
    // Returns the associated error code, returned by a Win32 API call.
    //
    // @return Associated error code.
    //
    LONG SettingsException::ErrorCode() const
    {
        return m_ErrorCode;
    }

    //
    // Returns a textual description of the exception.
    //
    // @return Exception description.
    //
    const char* SettingsException::what() const
    {
        return "PCC::SettingsException";
    }

} // namespace PCC
