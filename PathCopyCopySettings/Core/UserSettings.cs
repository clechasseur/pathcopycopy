// UserSettings.cs
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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.Win32;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.Core
{
    /// <summary>
    /// Class used to interact with the user settings of Path Copy Copy.
    /// </summary>
    public sealed class UserSettings : IDisposable
    {
        /// <summary>
        /// Determines how to encode characters in paths. Corresponds to the
        /// "EncodeParam" enum in the StringUtils C++ class.
        /// </summary>
        public enum StringEncodeParam
        {
            None,
            Whitespace,
            All,
        }

        /// Path of registry key containing global settings.
        private const string PCC_GLOBAL_SETTINGS_KEY = @"Software\clechasseur\PathCopyCopy";

        /// Path of registry key containing global settings when running under WOW64.
        private const string PCC_GLOBAL_SETTINGS_KEY_WOW64 = @"Software\Wow6432Node\clechasseur\PathCopyCopy";

        /// Path of registry key containing user settings.
        private const string PCC_USER_SETTINGS_KEY = @"Software\clechasseur\PathCopyCopy";

        /// Path of registry key containing icon files per plugin (relative to the main registry key).
        private const string PCC_ICONS_KEY = "Icons";

        /// Path of registry key containing the pipeline plugins (relative to the main registry key).
        private const string PCC_PIPELINE_PLUGINS_KEY = "PipelinePlugins";

        /// Path of registry key containing the temporary pipeline plugins (relative to the main registry key).
        private const string PCC_TEMP_PIPELINE_PLUGINS_KEY = "TempPipelinePlugins";

        /// Name of registry value determining whether we use hidden shares in UNC plugins.
        private const string USE_HIDDEN_SHARES_VALUE_NAME = "UseHiddenShares";

        /// Name of registry value determining whether we add quotes to copied paths.
        private const string ADD_QUOTES_VALUE_NAME = "AddQuotes";

       /// Name of registry value determining whether we turn paths into e-mail links.
        private const string MAKE_EMAIL_LINKS_VALUE_NAME = "MakeEmailLinks";

        /// Name of registry value determining how to encode characters in paths.
        private const string ENCODE_PARAM_VALUE_NAME = "EncodeParam";

        /// Name of registry value determining whether we show an icon next to the default plugin.
        private const string USE_ICON_FOR_DEFAULT_PLUGIN_VALUE_NAME = "UseIconForDefaultPlugin";

        /// Name of registry value determining whether we show an icon next to the submenu.
        private const string USE_ICON_FOR_SUBMENU_VALUE_NAME = "UseIconForSubmenu";

        /// Name of registry value determining whether to show previews in submenu instead of descriptions.
        private const string USE_PREVIEW_MODE_VALUE_NAME = "UsePreviewMode";

        /// Name of registry value determining whether to drop redundant words in plugin descriptions.
        private const string DROP_REDUNDANT_WORDS_VALUE_NAME = "DropRedundantWords";

        /// Name of registry value determining whether software updates are disabled.
        private const string DISABLE_SOFTWARE_UPDATE_VALUE_NAME = "DisableSoftwareUpdate";

        /// Name of registry value written by the installer to specify installation source (Inno, MSI, etc.)
        private const string INSTALL_SOURCE_VALUE_NAME = "InstallSource";

        /// Name of registry value determining whether the submenu is always shown.
        private const string ALWAYS_SHOW_SUBMENU_VALUE_NAME = "AlwaysShowSubmenu";

        /// Name of registry value specifying the separator to use between multiple copied paths.
        private const string PATHS_SEPARATOR_VALUE_NAME = "PathsSeparator";

        /// Name of registry value containing the ID of the plugin to activate when Ctrl key is held down.
        private const string CTRL_KEY_PLUGIN_VALUE_NAME = "CtrlKeyPlugin";

        /// Name of registry value containing the plugins to display in the main menu, in order.
        private const string MAIN_MENU_DISPLAY_ORDER_VALUE_NAME = "MainMenuDisplayOrder";

        /// Name of registry value containing the plugins to display in the submenu, in order.
        private const string SUBMENU_DISPLAY_ORDER_VALUE_NAME = "SubmenuDisplayOrder";

        /// Name of registry value containing the plugins to display in the Settings UI, in order.
        private const string UI_DISPLAY_ORDER_VALUE_NAME = "UIDisplayOrder";

        /// Name of registry value containing the plugins that are known to the settings app.
        private const string KNOWN_PLUGINS_VALUE_NAME = "KnownPlugins";

        /// Name of registry value containing the last ignored software update version.
        private const string IGNORED_UPDATE_VALUE_NAME = "IgnoredUpdate";

        /// Name of registry value containing the X component of the Settings form position.
        private const string SETTINGS_FORM_POS_X_VALUE_NAME = "SettingsFormPosX";

        /// Name of registry value containing the Y component of the Settings form position.
        private const string SETTINGS_FORM_POS_Y_VALUE_NAME = "SettingsFormPosY";

        /// Name of registry value containing the width component of the Settings form size.
        private const string SETTINGS_FORM_SIZE_WIDTH_VALUE_NAME = "SettingsFormSizeWidth";

        /// Name of registry value containing the height component of the Settings form size.
        private const string SETTINGS_FORM_SIZE_HEIGHT_VALUE_NAME = "SettingsFormSizeHeight";

        /// Name of registry value containing a pipeline plugin's description.
        private const string PIPELINE_PLUGIN_DESCRIPTION_VALUE_NAME = "Description";

        /// Name of registry value containing the path to a pipeline plugin's icon file.
        private const string PIPELINE_PLUGIN_ICON_VALUE_NAME = "IconFile";

        /// Name of registry value containing the minimum required version for a pipeline plugin.
        private const string PIPELINE_PLUGIN_REQUIRED_VERSION_VALUE_NAME = "RequiredVersion";

        /// Name of registry value containing the order in which to display pipeline plugins.
        private const string PIPELINE_PLUGINS_DISPLAY_ORDER_VALUE_NAME = "DisplayOrder";

        /// Name of registry value that can exist in the global key to indicate
        /// that settings editing is disabled.
        private const string EDITING_DISABLED_VALUE_NAME = "KeyLock";

        /// Name of registry value that can exist in the global key to store
        /// the URL of the software update file to check.
        private const string UPDATE_CHANNEL_VALUE_NAME = "UpdateChannel";

        /// Separator used between plugins in the registry values.
        private const char PLUGINS_VALUE_SEPARATOR = ',';

        /// Default value of the "use hidden shares" setting.
        private const int USE_HIDDEN_SHARES_DEFAULT_VALUE = 0;

        /// Default value of the "add quotes around paths" setting.
        private const int ADD_QUOTES_DEFAULT_VALUE = 0;

        /// Default value of the "make e-mail links" setting.
        private const int MAKE_EMAIL_LINKS_DEFAULT_VALUE = 0;

        /// Default value of the "encode param" setting.
        private const StringEncodeParam ENCODE_PARAM_DEFAULT_VALUE = StringEncodeParam.None;

        /// Default value of the "use icon for default plugin" setting.
        private const int USE_ICON_FOR_DEFAULT_PLUGIN_DEFAULT_VALUE = 0;

        /// Default value of the "use icon for submenu" setting.
        private const int USE_ICON_FOR_SUBMENU_DEFAULT_VALUE = 1;

        /// Default value of the "use preview mode" setting.
        private const int USE_PREVIEW_MODE_DEFAULT_VALUE = 0;

        /// Default value of the "drop redundant words" setting.
        private const int DROP_REDUNDANT_WORDS_DEFAULT_VALUE = 0;

        /// Default value of the "always show submenu" setting.
        private const int ALWAYS_SHOW_SUBMENU_DEFAULT_VALUE = 1;

        /// Default value of the "paths separator" setting.
        private const string PATHS_SEPARATOR_DEFAULT_VALUE = "";

        /// Default value of the "disable software update" setting.
        private const int DISABLE_SOFTWARE_UPDATE_DEFAULT_VALUE = 0;

        /// Default value of the "install source" setting.
        private const string INSTALL_SOURCE_DEFAULT_VALUE = "Inno";

        /// Default value for all size and position components of Settings form.
        private const int SETTINGS_FORM_POS_SIZE_DEFAULT_VALUE = -1;

        /// Separator used between pipeline plugins in the display order string.
        private const char PIPELINE_PLUGINS_DISPLAY_ORDER_SEPARATOR = ',';

        /// Registry key containing global settings for all users. Can be null for portable installations.
        private RegistryKey globalKey;

        /// Registry key containing user-specific settings.
        private RegistryKey userKey;

        /// Registry key containing the global plugin icon files. Can be null for portable installations.
        private RegistryKey globalIconsKey;

        /// Registry key containing user-specific plugin icon files.
        private RegistryKey userIconsKey;

        /// Registry key containing the global pipeline plugins. Can be null for portable installations.
        private RegistryKey globalPipelinePluginsKey;

        /// Registry key containing user-specific pipeline plugins.
        private RegistryKey userPipelinePluginsKey;

        /// Registry key containing temporary pipeline plugins. They are always user-specific.
        private RegistryKey userTempPipelinePluginsKey;

        /// <summary>
        /// Whether the UNC plugins should use hidden shares or not.
        /// </summary>
        public bool UseHiddenShares
        {
            get {
                return ((int) GetUserOrGlobalValue(USE_HIDDEN_SHARES_VALUE_NAME, USE_HIDDEN_SHARES_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(USE_HIDDEN_SHARES_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to add quotes around all copied paths.
        /// </summary>
        public bool AddQuotes
        {
            get {
                return ((int) GetUserOrGlobalValue(ADD_QUOTES_VALUE_NAME, ADD_QUOTES_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(ADD_QUOTES_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to turn paths into e-mail links by surrounding them with &lt; and &gt;
        /// </summary>
        public bool MakeEmailLinks
        {
            get {
                return ((int) GetUserOrGlobalValue(MAKE_EMAIL_LINKS_VALUE_NAME, MAKE_EMAIL_LINKS_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(MAKE_EMAIL_LINKS_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// How to encode characters in paths (if at all).
        /// </summary>
        public StringEncodeParam EncodeParam
        {
            get {
                try {
                    return (StringEncodeParam) Enum.Parse(typeof(StringEncodeParam), (string) GetUserOrGlobalValue(
                        ENCODE_PARAM_VALUE_NAME, ENCODE_PARAM_DEFAULT_VALUE.ToString()), true);
                } catch (ArgumentException ae) {
                    // Invalid value in the registry... Don't choke, use default value.
                    Debug.Fail(ae.Message);
                    return ENCODE_PARAM_DEFAULT_VALUE;
                }
            }
            set {
                userKey.SetValue(ENCODE_PARAM_VALUE_NAME, value.ToString());
            }
        }

        /// <summary>
        /// Whether to display an icon next to the default plugin in the contextual menu.
        /// </summary>
        public bool UseIconForDefaultPlugin
        {
            get {
                return ((int) GetUserOrGlobalValue(USE_ICON_FOR_DEFAULT_PLUGIN_VALUE_NAME, USE_ICON_FOR_DEFAULT_PLUGIN_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(USE_ICON_FOR_DEFAULT_PLUGIN_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to display an icon next to the submenu in the contextual menu.
        /// </summary>
        public bool UseIconForSubmenu
        {
            get {
                return ((int) GetUserOrGlobalValue(USE_ICON_FOR_SUBMENU_VALUE_NAME, USE_ICON_FOR_SUBMENU_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(USE_ICON_FOR_SUBMENU_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to use preview mode, e.g. display preview of result instead of descriptions in submenu.
        /// </summary>
        public bool UsePreviewMode
        {
            get {
                return ((int) GetUserOrGlobalValue(USE_PREVIEW_MODE_VALUE_NAME, USE_PREVIEW_MODE_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(USE_PREVIEW_MODE_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to drop redundant words from plugin descriptions in the submenu, like "copy".
        /// </summary>
        public bool DropRedundantWords
        {
            get {
                return ((int) GetUserOrGlobalValue(DROP_REDUNDANT_WORDS_VALUE_NAME, DROP_REDUNDANT_WORDS_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(DROP_REDUNDANT_WORDS_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Whether to always show the submenu. If not, it will only be shown
        /// when the user holds down the Shift key.
        /// </summary>
        public bool AlwaysShowSubmenu
        {
            get {
                return ((int) GetUserOrGlobalValue(ALWAYS_SHOW_SUBMENU_VALUE_NAME, ALWAYS_SHOW_SUBMENU_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(ALWAYS_SHOW_SUBMENU_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Separator to use between multiple copied paths. An empty string means to use
        /// the default value (usually a newline).
        /// </summary>
        public string PathsSeparator
        {
            get {
                return (string) GetUserOrGlobalValue(PATHS_SEPARATOR_VALUE_NAME, PATHS_SEPARATOR_DEFAULT_VALUE);
            }
            set {
                if (!String.IsNullOrEmpty(value)) {
                    userKey.SetValue(PATHS_SEPARATOR_VALUE_NAME, value);
                } else {
                    // Delete the value to use default.
                    userKey.DeleteValue(PATHS_SEPARATOR_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// Whether software updates are disabled.
        /// </summary>
        public bool DisableSoftwareUpdate
        {
            get {
                return ((int) GetUserOrGlobalValue(DISABLE_SOFTWARE_UPDATE_VALUE_NAME, DISABLE_SOFTWARE_UPDATE_DEFAULT_VALUE)) != 0;
            }
            set {
                userKey.SetValue(DISABLE_SOFTWARE_UPDATE_VALUE_NAME, value ? 1 : 0);
            }
        }

        /// <summary>
        /// Installation source. This is written by the installer and cannot be modified by this app.
        /// </summary>
        public string InstallSource
        {
            get {
                return (string) GetUserOrGlobalValue(INSTALL_SOURCE_VALUE_NAME, INSTALL_SOURCE_DEFAULT_VALUE);
            }
        }

        /// <summary>
        /// ID of plugin to activate when user holds down the Ctrl key when the
        /// contextual menu is shown. Will return <c>null</c> if not set.
        /// </summary>
        public Guid? CtrlKeyPlugin
        {
            get {
                var pluginIds = LoadPluginsFromValue(CTRL_KEY_PLUGIN_VALUE_NAME, false);
                Guid? pluginId = null;
                if (pluginIds.Count == 1) {
                    pluginId = pluginIds[0];
                }
                return pluginId;
            }
            set {
                if (value.HasValue) {
                    List<Guid> pluginIds = new List<Guid>();
                    pluginIds.Add(value.Value);
                    SavePluginsInValue(CTRL_KEY_PLUGIN_VALUE_NAME, pluginIds);
                } else {
                    // Delete the value in the registry instead.
                    userKey.DeleteValue(CTRL_KEY_PLUGIN_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// List of plugins to display in the main menu, in order.
        /// Will return <c>null</c> if not set.
        /// </summary>
        public List<Guid> MainMenuDisplayOrder
        {
            get {
                return LoadPluginsFromValue(MAIN_MENU_DISPLAY_ORDER_VALUE_NAME, true);
            }
            set {
                if (value != null) {
                    SavePluginsInValue(MAIN_MENU_DISPLAY_ORDER_VALUE_NAME, value);
                } else {
                    // Delete the value in the registry instead.
                    userKey.DeleteValue(MAIN_MENU_DISPLAY_ORDER_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// List of plugins to display in the submenu, in order.
        /// Will return <c>null</c> if not set.
        /// </summary>
        public List<Guid> SubmenuDisplayOrder
        {
            get {
                return LoadPluginsFromValue(SUBMENU_DISPLAY_ORDER_VALUE_NAME, true);
            }
            set {
                if (value != null) {
                    SavePluginsInValue(SUBMENU_DISPLAY_ORDER_VALUE_NAME, value);
                } else {
                    // Delete the value in the registry instead.
                    userKey.DeleteValue(SUBMENU_DISPLAY_ORDER_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// List of plugins to display in the Settings UI, in order.
        /// Will return <c>null</c> if not set.
        /// </summary>
        public List<Guid> UIDisplayOrder
        {
            get {
                return LoadPluginsFromValue(UI_DISPLAY_ORDER_VALUE_NAME, true);
            }
            set {
                if (value != null) {
                    SavePluginsInValue(UI_DISPLAY_ORDER_VALUE_NAME, value);
                } else {
                    // Delete the value in the registry instead.
                    userKey.DeleteValue(UI_DISPLAY_ORDER_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// List of plugins known to the settings app. The contextual menu extension
        /// uses this information to properly add new plugins in the menu.
        /// Will return <c>null</c> if not set.
        /// </summary>
        public List<Guid> KnownPlugins
        {
            get {
                return LoadPluginsFromValue(KNOWN_PLUGINS_VALUE_NAME, true);
            }
            set {
                if (value != null) {
                    SavePluginsInValue(KNOWN_PLUGINS_VALUE_NAME, value);
                } else {
                    // Delete the value in the registry instead.
                    userKey.DeleteValue(KNOWN_PLUGINS_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// List of pipeline plugins. Contains both global and user-specific ones.
        /// </summary>
        public List<PipelinePluginInfo> PipelinePlugins
        {
            get {
                return LoadPipelinePlugins(globalPipelinePluginsKey, userPipelinePluginsKey);
            }
            set {
                SavePipelinePlugins(value, userPipelinePluginsKey, true, true);
            }
        }

        /// <summary>
        /// Last ignored software update version. Returns <c>null</c> if no update has been ignored.
        /// </summary>
        public Version IgnoredUpdate
        {
            get {
                string ignoredValue = (string) GetUserOrGlobalValue(IGNORED_UPDATE_VALUE_NAME, String.Empty);
                return !String.IsNullOrEmpty(ignoredValue) ? new Version(ignoredValue) : null;
            }
            set {
                if (value != null) {
                    userKey.SetValue(IGNORED_UPDATE_VALUE_NAME, value.ToString());
                } else {
                    userKey.DeleteValue(IGNORED_UPDATE_VALUE_NAME, false);
                }
            }
        }

        /// <summary>
        /// X component of the Settings form position.
        /// </summary>
        public int SettingsFormPosX
        {
            get {
                return (int) GetUserOrGlobalValue(SETTINGS_FORM_POS_X_VALUE_NAME, SETTINGS_FORM_POS_SIZE_DEFAULT_VALUE);
            }
            set {
                userKey.SetValue(SETTINGS_FORM_POS_X_VALUE_NAME, value);
            }
        }

        /// <summary>
        /// Y component of the Settings form position.
        /// </summary>
        public int SettingsFormPosY
        {
            get {
                return (int) GetUserOrGlobalValue(SETTINGS_FORM_POS_Y_VALUE_NAME, SETTINGS_FORM_POS_SIZE_DEFAULT_VALUE);
            }
            set {
                userKey.SetValue(SETTINGS_FORM_POS_Y_VALUE_NAME, value);
            }
        }

        /// <summary>
        /// Width component of the Settings form size.
        /// </summary>
        public int SettingsFormSizeWidth
        {
            get {
                return (int) GetUserOrGlobalValue(SETTINGS_FORM_SIZE_WIDTH_VALUE_NAME, SETTINGS_FORM_POS_SIZE_DEFAULT_VALUE);
            }
            set {
                userKey.SetValue(SETTINGS_FORM_SIZE_WIDTH_VALUE_NAME, value);
            }
        }

        /// <summary>
        /// Height component of the Settings form size.
        /// </summary>
        public int SettingsFormSizeHeight
        {
            get {
                return (int) GetUserOrGlobalValue(SETTINGS_FORM_SIZE_HEIGHT_VALUE_NAME, SETTINGS_FORM_POS_SIZE_DEFAULT_VALUE);
            }
            set {
                userKey.SetValue(SETTINGS_FORM_SIZE_HEIGHT_VALUE_NAME, value);
            }
        }

        /// <summary>
        /// Whether editing settings has been disabled by the administrator.
        /// This is a read-only setting.
        /// </summary>
        public bool EditingDisabled
        {
            get {
                object keyLockValue = globalKey != null ? globalKey.GetValue(EDITING_DISABLED_VALUE_NAME) : null;
                return keyLockValue != null ? ((int) keyLockValue) != 0 : false;
            }
        }

        /// <summary>
        /// URL of the software update channel to use.
        /// </summary>
        public string UpdateChannel
        {
            get {
                object channelValue = globalKey != null ? globalKey.GetValue(UPDATE_CHANNEL_VALUE_NAME) : null;
                return channelValue != null ? (string) channelValue : Resources.UserSettings_DefaultUpdateChannel;
            }
        }
        
        /// <summary>
        /// Constructor. Creates the registry key right away to read the settings,
        /// both global and user-specific.
        /// </summary>
        public UserSettings()
        {
            // Get path of global key. If we're being called by a 32-bit extension
            // running under WOW64 we'll have to adjust.
            string globalKeyPath;
            if (!PCCEnvironment.Is64Bit && PCCEnvironment.IsWow64) {
                globalKeyPath = PCC_GLOBAL_SETTINGS_KEY_WOW64;
            } else {
                globalKeyPath = PCC_GLOBAL_SETTINGS_KEY;
            }

            // Open the global key for reading only.
            globalKey = Registry.LocalMachine.OpenSubKey(globalKeyPath, false);

            // Open the user key for reading and writing.
            userKey = Registry.CurrentUser.CreateSubKey(PCC_USER_SETTINGS_KEY);

            // Open the global icons key for reading only.
            globalIconsKey = globalKey?.OpenSubKey(PCC_ICONS_KEY, false);

            // Open the user icons key for reading and writing.
            userIconsKey = userKey.CreateSubKey(PCC_ICONS_KEY);

            // Open the global pipeline plugins key for reading only.
            globalPipelinePluginsKey = globalKey?.OpenSubKey(PCC_PIPELINE_PLUGINS_KEY, false);

            // Open the user pipeline plugins key for reading and writing.
            userPipelinePluginsKey = userKey.CreateSubKey(PCC_PIPELINE_PLUGINS_KEY);

            // Open the temporary pipeline plugins key for reading and writing. They are always user-specific.
            userTempPipelinePluginsKey = userKey.CreateSubKey(PCC_TEMP_PIPELINE_PLUGINS_KEY);
        }
        
        /// <summary>
        /// Called when the object is disposed. We close the registry keys.
        /// </summary>
        public void Dispose()
        {
            userTempPipelinePluginsKey?.Close();
            userPipelinePluginsKey?.Close();
            globalPipelinePluginsKey?.Close();
            userIconsKey?.Close();
            globalIconsKey?.Close();
            userKey?.Close();
            globalKey?.Close();
        }

        /// <summary>
        /// Uses the <c>reg</c> command to export the contents of the user key to a file.
        /// </summary>
        /// <param name="fileName">File where to save the registry data.
        /// Should use the <c>.reg</c> extension to be importable later.</param>
        /// <returns><c>true</c> if the export worked.</returns>
        /// <remarks>
        /// The <c>/y</c> switch is used when calling <c>reg</c>, meaning that
        /// <paramref name="fileName"/> will be overwritten without prompt.
        /// Validate that user wants to overwrite before calling this.
        /// </remarks>
        public static bool ExportUserSettings(string fileName)
        {
            Debug.Assert(!String.IsNullOrEmpty(fileName));

            ProcessStartInfo psi = new ProcessStartInfo("cmd.exe") {
                Arguments = String.Format("/c \"reg export HKCU\\{0} \"\"{1}\"\" /y\"",
                    PCC_USER_SETTINGS_KEY, fileName),
                CreateNoWindow = true,
                UseShellExecute = false,
            };
            using (Process reg = Process.Start(psi)) {
                reg.WaitForExit();
                return reg.ExitCode == 0;
            }
        }
        
        /// <summary>
        /// Fetches an icon file for the given plugin from settings, if one was set.
        /// </summary>
        /// <param name="pluginId">Id of plugin for which we want an icon file.</param>
        /// <returns><c>null</c> if no icon file was found for this plugin,
        /// an empty string if the default icon should be used for this plugin,
        /// otherwise the path to a local file to use for this plugin.</returns>
        public string GetIconFileForPlugin(Guid pluginId)
        {
            return GetUserOrGlobalIconFile(pluginId.ToString("B"));
        }
        
        /// <summary>
        /// Sets or resets the icon file associated with the given plugin in settings.
        /// </summary>
        /// <param name="pluginId">Id of plugin for which to set or reset the icon
        /// file.</param>
        /// <param name="iconFile">To set an icon file for the plugin, pass the
        /// full path to the icon file on disk. To instruct plugin to use the
        /// default icon, pass an empty string. To reset the icon file associated
        /// with plugin, thus instructing PCC to display no icon for this plugin,
        /// pass <c>null</c>.</param>
        public void SetIconFileOfPlugin(Guid pluginId, string iconFile)
        {
            string pluginIdAsString = pluginId.ToString("B");
            if (iconFile != null) {
                userIconsKey.SetValue(pluginIdAsString, iconFile);
            } else if (userIconsKey.GetValue(pluginIdAsString) != null) {
                userIconsKey.DeleteValue(pluginIdAsString);
            }
        }
        
        /// <summary>
        /// Saves a temporary pipeline plugin to the temp pipeline plugins key.
        /// </summary>
        /// <param name="pluginInfo"><see cref="PipelinePluginInfo"/> to save.</param>
        public void SaveTempPipelinePlugin(PipelinePluginInfo pluginInfo)
        {
            Debug.Assert(pluginInfo != null);
            
            List<PipelinePluginInfo> pluginInfos = new List<PipelinePluginInfo>();
            pluginInfos.Add(pluginInfo);
            SavePipelinePlugins(pluginInfos, userTempPipelinePluginsKey, false, false);
        }
        
        /// <summary>
        /// Removes a temporary pipeline plugin from the temp pipeline plugins key.
        /// </summary>
        /// <param name="pluginInfo"><see cref="PipelinePluginInfo"/> to remove.</param>
        public void DeleteTempPipelinePlugin(PipelinePluginInfo pluginInfo)
        {
            Debug.Assert(pluginInfo != null);

            try {
                userTempPipelinePluginsKey.DeleteSubKeyTree(pluginInfo.Id.ToString("B"));
            } catch (ArgumentException) {
                // The subkey did not exist, so no need to "remove" it.
            }
        }
        
        /// <summary>
        /// Function called to load a list of plugin IDs from a registry value.
        /// </summary>
        /// <param name="valueName">Name of registry value containing the data.</param>
        /// <param name="wantsNull">Whether to return <c>null</c> (<c>true</c>)
        /// or an empty list (<c>false</c>) when the registry value is not found.</param>
        /// <returns>List of plugin IDs.</returns>
        private List<Guid> LoadPluginsFromValue(string valueName, bool wantsNull)
        {
            List<Guid> plugins = null;

            // Load registry value containing the plugins.
            object pluginsValue = GetUserOrGlobalValue(valueName);
            if (pluginsValue != null && pluginsValue is string) {
                // Split the values so we get individual IDs.
                string[] pluginIds = ((string) pluginsValue).Split(PLUGINS_VALUE_SEPARATOR);

                // Convert each ID and add it to the list of plugins.
                plugins = new List<Guid>();
                foreach (string id in pluginIds) {
                    try {
                        if (id.Length != 0) {
                            plugins.Add(new Guid(id));
                        }
                    } catch (FormatException) {
                    } catch (OverflowException) {
                    }
                }
            } else if (!wantsNull) {
                // Create an empty list instead.
                plugins = new List<Guid>();
            }

            return plugins;
        }
        
        /// <summary>
        /// Saves a list of plugin IDs in a user settings' registry value.
        /// </summary>
        /// <param name="valueName">Name of registry value where to save plugins.</param>
        /// <param name="plugins">List of plugins to save.</param>
        private void SavePluginsInValue(string valueName, List<Guid> plugins)
        {
            // Scan guids and build a registry value.
            string regValue = String.Empty;
            foreach (Guid id in plugins) {
                string idAsString = id.ToString("B");
                if (regValue.Length != 0) {
                    regValue += PLUGINS_VALUE_SEPARATOR;
                }
                regValue += idAsString;
            }

            // Save this value content in the registry.
            userKey.SetValue(valueName, regValue);
        }
        
        /// <summary>
        /// Loads the list of pipeline plugins, both global and user-specific.
        /// </summary>
        /// <param name="globalRegKey">Global registry key containing pipeline
        /// plugins. Can be <c>null</c>, in which case only user plugins will
        /// be loaded.</param>
        /// <param name="userRegKey">User registry key containing pipeline plugins.</param>
        /// <returns>List of pipeline plugins, in the order they should be
        /// displayed.</returns>
        private List<PipelinePluginInfo> LoadPipelinePlugins(RegistryKey globalRegKey,
            RegistryKey userRegKey)
        {
            Debug.Assert(userRegKey != null);

            // First load the plugins found in the global and user-specific keys.
            List<PipelinePluginInfo> pluginInfos = new List<PipelinePluginInfo>();
            if (globalRegKey != null) {
                LoadPipelinePluginsFromKey(globalRegKey, pluginInfos, true);
            }
            LoadPipelinePluginsFromKey(userRegKey, pluginInfos, false);

            // Check if we have a value to display the pipeline plugins in a specific
            // order. There could be one in either the user or global key.
            object displayOrderValue = userRegKey.GetValue(PIPELINE_PLUGINS_DISPLAY_ORDER_VALUE_NAME);
            if (displayOrderValue == null && globalRegKey != null) {
                displayOrderValue = globalRegKey.GetValue(PIPELINE_PLUGINS_DISPLAY_ORDER_VALUE_NAME);
            }
            if (displayOrderValue != null && displayOrderValue is string) {
                // The display order is represented as a comma-separated list of pipeline plugin IDs.
                string[] idsAsString = ((string) displayOrderValue).Split(PIPELINE_PLUGINS_DISPLAY_ORDER_SEPARATOR);
                List<Guid> ids = new List<Guid>();
                foreach (string idAsString in idsAsString) {
                    try {
                        ids.Add(new Guid(idAsString));
                    } catch (FormatException) {
                    } catch (OverflowException) {
                    }
                }

                // Sort the pipeline plugin infos using the display order IDs.
                // Note: to match the C++ implementation, we position unknown
                // pipeline plugins at the end of the display order.
                pluginInfos.Sort((leftInfo, rightInfo) => {
                    int leftIndex = ids.FindIndex(id => id.Equals(leftInfo.Id));
                    int rightIndex = ids.FindIndex(id => id.Equals(rightInfo.Id));
                    if (leftIndex < 0) {
                        leftIndex = int.MaxValue;
                    }
                    if (rightIndex < 0) {
                        rightIndex = int.MaxValue;
                    }
                    return leftIndex - rightIndex;
                });
            }

            return pluginInfos;
        }
        
        /// <summary>
        /// Saves the list of pipeline plugins. User-specific plugins will be saved
        /// to the registry; global plugins are not modified, but the display order
        /// will be honored.
        /// </summary>
        /// <param name="pipelinePlugins">List of pipeline plugins in the order
        /// they should be displayed.</param>
        /// <param name="regKey">Registry key where to save user pipeline plugins.</param>
        /// <param name="saveDisplayOrder">Whether to save the plugins' display
        /// order to the registry key. This should generally be set to <c>true</c>
        /// except for temporary pipeline plugins.</param>
        /// <param name="removeObsolete">Whether to remove obsolete plugins from
        /// the registry key (e.g. plugins that no longer exist). This should
        /// generally be set to <c>true</c> except for temporary pipeline plugins.</param>
        private void SavePipelinePlugins(List<PipelinePluginInfo> pipelinePlugins,
            RegistryKey regKey, bool saveDisplayOrder, bool removeObsolete)
        {
            Debug.Assert(pipelinePlugins != null);
            Debug.Assert(regKey != null);

            if (saveDisplayOrder) {
                // Build display order string and save it.
                List<string> idsAsString = pipelinePlugins.ConvertAll(plugin => plugin.Id.ToString("B"));
                string displayOrder = String.Join(PIPELINE_PLUGINS_DISPLAY_ORDER_SEPARATOR.ToString(), idsAsString.ToArray());
                regKey.SetValue(PIPELINE_PLUGINS_DISPLAY_ORDER_VALUE_NAME, displayOrder);
            }

            // Here's the algo to save the plugins:
            // 1. Save existing list of pipeline plugins in user key (if we need to remove obsolete plugins).
            // 2. Scan provided list and for each user-specific plugin...
            //    2a. Create registry key for plugin and save info.
            // 3. If we need to remove obsolete plugins, for each plugin that no longer exists...
            //    3a. Delete its registry key.

            // 1.
            string[] existingPlugins = removeObsolete ? regKey.GetSubKeyNames() : null;

            // 2.
            foreach (PipelinePluginInfo pluginInfo in pipelinePlugins) {
                if (!pluginInfo.Global) {
                    // 2a.
                    using (RegistryKey pluginKey = regKey.CreateSubKey(pluginInfo.Id.ToString("B"))) {
                        pluginKey.SetValue(PIPELINE_PLUGIN_DESCRIPTION_VALUE_NAME, pluginInfo.Description);
                        if (pluginInfo.IconFile != null) {
                            pluginKey.SetValue(PIPELINE_PLUGIN_ICON_VALUE_NAME, pluginInfo.IconFile);
                        } else if (pluginKey.GetValue(PIPELINE_PLUGIN_ICON_VALUE_NAME) != null) {
                            pluginKey.DeleteValue(PIPELINE_PLUGIN_ICON_VALUE_NAME);
                        }
                        pluginKey.SetValue(PIPELINE_PLUGIN_REQUIRED_VERSION_VALUE_NAME, pluginInfo.RequiredVersionAsString);
                        pluginKey.SetValue(null, pluginInfo.EncodedElements);
                    }
                }
            }

            // 3.
            if (removeObsolete) {
                Debug.Assert(existingPlugins != null);
                foreach (string existingPlugin in existingPlugins) {
                    if (!pipelinePlugins.Exists(plugin => plugin.Id.ToString("B").Equals(existingPlugin))) {
                        // 3a.
                        regKey.DeleteSubKeyTree(existingPlugin);
                    }
                }
            }
        }
        
        /// <summary>
        /// Loads the list of pipeline plugins stored in the given registry key.
        /// Does not check the display order.
        /// </summary>
        /// <param name="regKey">Registry key from which to load plugins.</param>
        /// <param name="pipelinePlugins">Where to store the pipeline plugins
        /// loaded.</param>
        /// <param name="isGlobal">Whether this is the global key we're parsing.</param>
        private void LoadPipelinePluginsFromKey(RegistryKey regKey,
            List<PipelinePluginInfo> pipelinePlugins, bool isGlobal)
        {
            Debug.Assert(regKey != null);
            Debug.Assert(pipelinePlugins != null);

            // Plugins are stored as subkeys of the registry key.
            string[] pluginKeyNames = regKey.GetSubKeyNames();
            foreach (string idAsString in pluginKeyNames) {
                // Convert this ID to a guid and make sure it works.
                try {
                    Guid id = new Guid(idAsString);

                    // Open the subkey and read values for the description,
                    // encoded elements, icon file and required version.
                    string description, encodedElements, iconFile, minVersionAsString;
                    using (RegistryKey subKey = regKey.OpenSubKey(idAsString, false)) {
                        description = (string) subKey.GetValue(PIPELINE_PLUGIN_DESCRIPTION_VALUE_NAME);
                        iconFile = (string) subKey.GetValue(PIPELINE_PLUGIN_ICON_VALUE_NAME);
                        minVersionAsString = (string) subKey.GetValue(PIPELINE_PLUGIN_REQUIRED_VERSION_VALUE_NAME);
                        encodedElements = (string) subKey.GetValue(null);
                    }

                    // If we don't have min version in the registry, try to compute it now.
                    Version minVersion;
                    if (minVersionAsString != null) {
                        minVersion = new Version(minVersionAsString);
                    } else {
                        try {
                            Pipeline pipeline = PipelineDecoder.DecodePipeline(encodedElements);
                            minVersion = pipeline.RequiredVersion;
                        } catch (InvalidPipelineException) {
                            // The encoded elements for this pipeline plugin is either invalid
                            // or from a more recent version which we don't know of for some reason.
                            // We can't do much, we'll have to pass null to the constructor below.
                            minVersion = null;
                        }
                    }

                    // If we made it here we have the plugin info, add it to the list.
                    pipelinePlugins.Add(new PipelinePluginInfo(id, description, encodedElements,
                        iconFile, isGlobal, minVersion));
                } catch (FormatException) {
                } catch (OverflowException) {
                }
            }
        }
        
        /// <summary>
        /// Reads a registry value from the user key first, and if not found,
        /// from the global key.
        /// </summary>
        /// <param name="name">Name of the registry value to load.</param>
        /// <returns>Value, or <c>null</c> if the value is not found.</returns>
        private object GetUserOrGlobalValue(string name)
        {
            return userKey.GetValue(name) ?? globalKey?.GetValue(name);
        }
        
        /// <summary>
        /// Reads a registry value from the user key first, and if not found,
        /// from the global key. If not found at all, a default value is returned.
        /// </summary>
        /// <param name="name">Name of the registry value to load.</param>
        /// <param name="defaultValue">Default value to return if not found.</param>
        /// <returns>Value, or <paramref name="defaultValue"/> if not found.</returns>
        private object GetUserOrGlobalValue(string name, object defaultValue)
        {
            return GetUserOrGlobalValue(name) ?? defaultValue;
        }
        
        /// <summary>
        /// Reads an icon file from the user icons key first, and if not found,
        /// from the global icons key.
        /// </summary>
        /// <param name="pluginId">Id of plugin for which we want an icon file.</param>
        /// <returns>Path to icon file for plugin, or <c>null</c> if no icon file
        /// was found.</returns>
        private string GetUserOrGlobalIconFile(string pluginId)
        {
            object value = userIconsKey.GetValue(pluginId);
            if ((value == null || !(value is string)) && globalIconsKey != null) {
                value = globalIconsKey.GetValue(pluginId);
            }
            if (value != null && !(value is string)) {
                value = null;
            }
            return (string) value;
        }
    }
}
