// MainForm.cs
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

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Utils;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Main form of the Path Copy Copy settings application.
    /// </summary>
    /// <remarks>
    /// The form remembers its position and size, but not through
    /// the generic <see cref="PositionPersistedForm"/>, because
    /// it came before that. So it still uses the legacy way of
    /// doing it, with its own entries in user settings.
    /// </remarks>
    public partial class MainForm : PositionPersistedForm
    {
        /// Paths separator that copies multiple paths on the same line.
        private const string PathsSeparatorOnSameLine = " ";
        
        /// The standard DPI values in Windows.
        private const int StandardWindowsDPI = 96;

        /// The desired size when loading icons from icon files.
        private static readonly Size DesiredIconSize = new Size(16, 16);

        /// Map of pipeline plugins exported file extensions to the
        /// corresponding XML serializer version needed to read them.
        private static readonly IDictionary<string, PipelinePluginXmlSerializerVersion> PipelinePluginsExtToSerializerVersion =
            new Dictionary<string, PipelinePluginXmlSerializerVersion>()
            {
                { ".eccv3", PipelinePluginXmlSerializerVersion.V3 },
                { ".ecc", PipelinePluginXmlSerializerVersion.V2 },
                { ".pccpp", PipelinePluginXmlSerializerVersion.V1 },
            };

        /// Helper that saves temp pipeline plugins to registry.
        private TempPipelinePluginsHelper tempPipelinePluginsHelper;

        /// BindingList used to store all existing plugins and their display infos.
        private BindingList<PluginDisplayInfo> pluginDisplayInfos;

        /// Will contain the last value of SelectedIndex in Ctrl key plugin combo box.
        /// Used to be able to switch back to the previous value quickly if selection is invalid.
        private int lastCtrlKeyPluginSelectedIndex = -1;

        /// Will track SelectedIndexChanged calls to avoid recursion issues.
        private bool inCtrlKeyPluginComboBoxSelectedIndexChanged = false;

        /// BindingList used to store all languages for the application.
        private BindingList<LanguageDisplayInfo> languageDisplayInfos;

        /// Whether we've shown the warning to the user that switching languages
        /// requires logoff/logon to work.
        private bool languageSwitchWarningShown = false;

        /// <summary>
        /// Constructor
        /// </summary>
        public MainForm()
        {
            InitializeComponent();

            // Disable the grid view's auto-creation of columns since we've
            // entered them by hand.
            PluginsDataGrid.AutoGenerateColumns = false;

            // The checkbox columns are bound to properties that return strings.
            // We'll set the true and false values accordingly.
            foreach (string colName in new string[] { InMainMenuCol.Name, InSubmenuCol.Name }) {
                DataGridViewCheckBoxColumn column = (DataGridViewCheckBoxColumn) PluginsDataGrid.Columns[colName];
                column.TrueValue = "1";
                column.FalseValue = String.Empty;
            }

            // Our DataGridView's checkboxes don't show up properly in larger font sizes
            // unless the AutoSizeRowsMode is set to AllCells (or AllCellsExceptHeaders).
            // However I find that it looks a bit too large to set it to that height...
            // What we'll do is try to detect non-standard DPI and if found, we'll set
            // the property (it'll produce tall rows but at least the checkboxes will show up.)
            using (Graphics gridViewGraphics = PluginsDataGrid.CreateGraphics()) {
                if (((int) gridViewGraphics.DpiX) != StandardWindowsDPI || ((int) gridViewGraphics.DpiY) != StandardWindowsDPI) {
                    PluginsDataGrid.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCellsExceptHeaders;
                }
            }
            
            // Connect all checkboxes in the Options page to the CheckChanged event handler that
            // will enable the Apply button.
            foreach (Control control in MiscOptionsPage.Controls) {
                if (control is CheckBox) {
                    (control as CheckBox).CheckedChanged += Chk_CheckedChanged;
                }
            }

            // Init about "box" controls.
            Assembly thisAssembly = GetType().Assembly;
            Version version = thisAssembly.GetName().Version;
            int numComponents;
            if (version.Revision > 0) {
                numComponents = 4;
            } else if (version.Build > 0) {
                numComponents = 3;
            } else {
                numComponents = 2;
            }
            ProductAndVersionLbl.Text = string.Format(CultureInfo.CurrentCulture,
                ProductAndVersionLbl.Text, version.ToString(numComponents),
                Settings.DevBuild ? Resources.DEV_BUILD : string.Empty);
            CopyrightLbl.Text = GetAssemblyCopyrightString(thisAssembly);
            MainToolTip.SetToolTip(SiteLinkLbl, SiteLinkLbl.Text);
            MainToolTip.SetToolTip(LicenseTxtLinkLbl, Resources.MainForm_About_LicensePageURI);
            DonationLinkLbl.Links[0].LinkData = Resources.MainForm_About_DonationsPageURI;
            MainToolTip.SetToolTip(DonationLinkLbl, Resources.MainForm_About_DonationsPageURI);
        }
        
        /// <summary>
        /// Given the reference to an <see cref="Assembly"/>, returns the copyright
        /// string as found in its <see cref="AssemblyCopyrightAttribute"/>.
        /// </summary>
        private string GetAssemblyCopyrightString(Assembly assembly)
        {
            string copyrightString;

            // Look for custom attribute containing copyright string.
            object[] copyrightAttributes = assembly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), true);
            if (copyrightAttributes.Length != 0) {
                // Extract the entire string from the attribute.
                string entireCopyrightString = ((AssemblyCopyrightAttribute) copyrightAttributes[0]).Copyright;

                // If there is a period, stop there. We don't want the part about LICENSE.
                int periodPos = entireCopyrightString.IndexOf('.');
                if (periodPos >= 0) {
                    copyrightString = entireCopyrightString.Substring(0, periodPos);
                } else {
                    Debug.Fail("Copyright string is not formatted like we thought?");
                    copyrightString = entireCopyrightString;
                }
            } else {
                // Use default copyright.
                Debug.Fail("No copyright attribute on assembly?");
                copyrightString = "(c) 2008-2020, Charles Lechasseur";
            }

            // Replace "(c)" with the copyright symbol
            Debug.Assert(!string.IsNullOrEmpty(copyrightString));
            copyrightString = copyrightString.Replace("(c)", "\u00A9");

            return copyrightString;
        }
        
        /// <summary>
        /// Loads the options and populates the form.
        /// </summary>
        private void LoadUserSettings()
        {
            // Load list of plugins in default order.
            List<Plugin> pluginsInDefaultOrder = PluginsRegistry.GetPluginsInDefaultOrder(
                Settings, PipelinePluginsOptions.FetchPipelinePlugins);

            // Create sorted dictionary of all plugins from the list above, to be able to perform lookups.
            SortedDictionary<Guid, Plugin> dictionaryOfAllPlugins = new SortedDictionary<Guid, Plugin>();
            foreach (Plugin plugin in pluginsInDefaultOrder) {
                if (!dictionaryOfAllPlugins.ContainsKey(plugin.Id)) {
                    dictionaryOfAllPlugins.Add(plugin.Id, plugin);
                }
            }

            // Create binding list to store plugins to display. Populate it by ordering
            // plugins using UI display order from settings. (We use UI display order as
            // "known plugins" also because we want to display any plugin not in UI display
            // order at the end, otherwise we might skip displaying known plugins that are
            // not in UI display order setting.)
            List<Guid> uiDisplayOrder = Settings.UIDisplayOrder;
            if (uiDisplayOrder == null) {
                // No display order, just use all plugins in default order
                uiDisplayOrder = pluginsInDefaultOrder.Select(plugin => plugin.Id).ToList();
            }
            SortedSet<Guid> uiDisplayOrderAsSet = new SortedSet<Guid>(uiDisplayOrder);
            List<Plugin> plugins = PluginsRegistry.OrderPluginsToDisplay(dictionaryOfAllPlugins,
                uiDisplayOrder, uiDisplayOrderAsSet, pluginsInDefaultOrder);
            pluginDisplayInfos = new BindingList<PluginDisplayInfo>();
            foreach (Plugin plugin in plugins) {
                pluginDisplayInfos.Add(new PluginDisplayInfo(plugin));
            }

            // Create helper to track temp pipeline plugins and assign our existing plugins
            // so they are initially saved to registry.
            tempPipelinePluginsHelper = new TempPipelinePluginsHelper();
            tempPipelinePluginsHelper.Assign(pluginDisplayInfos.Select(info => info.Plugin));

            // Scan display infos and set the ShowInMainMenu/Submenu fields
            // according to whether plugins are found in corresponding settings.
            List<Guid> mainMenuDisplayOrder = Settings.MainMenuDisplayOrder;
            SortedSet<Guid> mainMenuPlugins;
            if (mainMenuDisplayOrder != null) {
                mainMenuPlugins = new SortedSet<Guid>(mainMenuDisplayOrder);
            } else {
                // No main menu plugin specified in settings, use default.
                mainMenuPlugins = new SortedSet<Guid> {
                    new Guid(Resources.LONG_PATH_PLUGIN_ID),
                };
            }
            List<Guid> submenuDisplayOrder = Settings.SubmenuDisplayOrder;
            SortedSet<Guid> submenuPlugins;
            if (submenuDisplayOrder != null) {
                submenuPlugins = new SortedSet<Guid>(submenuDisplayOrder);
            } else {
                // No submenu plugins specified in settings, use all plugins in default order.
                submenuPlugins = new SortedSet<Guid>(pluginsInDefaultOrder.Select(plugin => plugin.Id));
            }
            foreach (PluginDisplayInfo info in pluginDisplayInfos) {
                if (!(info.Plugin is SeparatorPlugin)) {
                    info.ShowInMainMenu = mainMenuPlugins.Contains(info.Plugin.Id);
                    info.ShowInSubmenu = submenuPlugins.Contains(info.Plugin.Id);
                }
            }

            // Set binding list as data source for the data grid.
            PluginsDataGridBindingSource.DataSource = pluginDisplayInfos;

            // Set options checkboxes.
            AddQuotesChk.Checked = Settings.AddQuotes;
            if (AddQuotesChk.Checked) {
                AreQuotesOptionalChk.Checked = Settings.AreQuotesOptional;
            }
            EmailLinksChk.Checked = Settings.MakeEmailLinks;
            AppendSepForDirChk.Checked = Settings.AppendSeparatorForDirectories;
            HiddenSharesChk.Checked = Settings.UseHiddenShares;
            UseFQDNChk.Checked = Settings.UseFQDN;
            AlwaysShowSubmenuChk.Checked = Settings.AlwaysShowSubmenu;
            UseIconForSubmenuChk.Checked = Settings.UseIconForSubmenu;
            UsePreviewModeChk.Checked = Settings.UsePreviewMode;
            if (UsePreviewModeChk.Checked) {
                UsePreviewModeInMainMenuChk.Checked = Settings.UsePreviewModeInMainMenu;
            }
            DropRedundantWordsChk.Checked = Settings.DropRedundantWords;
            RecursiveCopyChk.Checked = Settings.CopyPathsRecursively;
            TrueLnkPathsChk.Checked = Settings.TrueLnkPaths;
            EnableSoftwareUpdateChk.Checked = !Settings.DisableSoftwareUpdate;

            // Set binding list as data source for the combo box used to pick ctrl key plugin.
            CtrlKeyPluginComboBindingSource.DataSource = pluginDisplayInfos;

            // Encode param is a little special since it has two checkboxes.
            switch (Settings.EncodeParam) {
                case UserSettings.StringEncodeParam.None: {
                    break;
                }
                case UserSettings.StringEncodeParam.Whitespace: {
                    EncodeURIWhitespaceChk.Checked = true;
                    break;
                }
                case UserSettings.StringEncodeParam.All: {
                    EncodeURIWhitespaceChk.Checked = true;
                    EncodeURICharsChk.Checked = true;
                    break;
                }
                default: {
                    Debug.Fail($"Unknown encode param: {Settings.EncodeParam}");
                    EncodeURIWhitespaceChk.Enabled = false;
                    break;
                }
            }

            // "Copy on same line" is a little special since it could be any string.
            Debug.Assert(!CopyOnSameLineChk.Checked);
            string pathsSeparator = Settings.PathsSeparator;
            if (pathsSeparator == PathsSeparatorOnSameLine) {
                CopyOnSameLineChk.Checked = true;
            } else if (!string.IsNullOrEmpty(pathsSeparator)) {
                CopyOnSameLineChk.Enabled = false;
            }

            // Ctrl key plugin is a little special since there's a combo box to update.
            Guid? ctrlKeyPluginId = Settings.CtrlKeyPlugin;
            if (ctrlKeyPluginId.HasValue && !ctrlKeyPluginId.Value.Equals(new SeparatorPlugin().Id)) {
                for (int i = 0; i < pluginDisplayInfos.Count; ++i) {
                    if (pluginDisplayInfos[i].Plugin.Id == ctrlKeyPluginId.Value) {
                        CtrlKeyPluginChk.Checked = true;
                        CtrlKeyPluginCombo.SelectedIndex = i;
                        lastCtrlKeyPluginSelectedIndex = i;
                        break;
                    }
                }
            }

            // Load list of all supported languages.
            languageDisplayInfos = new BindingList<LanguageDisplayInfo> {
                new LanguageDisplayInfo {
                    Name = Resources.Language_English_Name,
                    DisplayName = Resources.Language_English_DisplayName,
                },
                new LanguageDisplayInfo {
                    Name = Resources.Language_French_Name,
                    DisplayName = Resources.Language_French_DisplayName,
                },
            };

            // If the language chosen in the settings is not known, add an entry so it can be selected.
            string language = Settings.Language;
            if (language != null && !languageDisplayInfos.Any(ldi => ldi.Name == language)) {
                languageDisplayInfos.Add(new LanguageDisplayInfo {
                    Name = language,
                    DisplayName = language,
                });
            }

            // Set binding list as data source for the language combo box binding source.
            LanguageComboBindingSource.DataSource = languageDisplayInfos;

            // Select language in the dropdown.
            if (language != null) {
                for (int i = 0; i < languageDisplayInfos.Count; i++) {
                    if (language == languageDisplayInfos[i].Name) {
                        LanguageCombo.SelectedIndex = i;
                        break;
                    }
                }
            }
            // If no language is selected, choose default.
            if (LanguageCombo.SelectedIndex == -1) {
                LanguageCombo.SelectedIndex = 0;
            }

            // All those changes probably enabled the "Apply" button so disable it again here.
            ApplyBtn.Enabled = false;

            Debug.Assert(pluginDisplayInfos != null);
        }
        
        /// <summary>
        /// Saves the current options in the user settings.
        /// </summary>
        private void SaveUserSettings()
        {
            Debug.Assert(pluginDisplayInfos != null);

            // Build list of plugin IDs to save in config for main menu.
            // This is easy: we jolt down the plugins marked as display
            // in main menu without worrying about separators.
            List<Guid> mainMenuDisplayOrder = pluginDisplayInfos.Where(info => info.ShowInMainMenu)
                                                                .Select(info => info.Plugin.Id)
                                                                .ToList();

            // If the only plugin left to show in the main menu is our default one,
            // clear the value instead.
            if (mainMenuDisplayOrder.Count == 1 && mainMenuDisplayOrder[0] == new Guid(Resources.LONG_PATH_PLUGIN_ID)) {
                mainMenuDisplayOrder = null;
            }
            Settings.MainMenuDisplayOrder = mainMenuDisplayOrder;

            // Build list of plugin IDs to save in config for submenu.
            // This is more complicated as we need to include separators.
            // Be careful not to double them.
            List<Guid> submenuDisplayOrder = new List<Guid>();
            for (int i = 0; i < pluginDisplayInfos.Count; ++i) {
                if (pluginDisplayInfos[i].ShowInSubmenu) {
                    submenuDisplayOrder.Add(pluginDisplayInfos[i].Plugin.Id);
                } else if (pluginDisplayInfos[i].Plugin is SeparatorPlugin &&
                    submenuDisplayOrder.Count > 0 &&
                    submenuDisplayOrder[submenuDisplayOrder.Count - 1] != pluginDisplayInfos[i].Plugin.Id) {

                    submenuDisplayOrder.Add(pluginDisplayInfos[i].Plugin.Id);
                }
            }

            // If we have a separator at the end, remove it.
            SeparatorPlugin separator = new SeparatorPlugin();
            if (submenuDisplayOrder.Count > 0 && submenuDisplayOrder[submenuDisplayOrder.Count - 1] == separator.Id) {
                submenuDisplayOrder.RemoveAt(submenuDisplayOrder.Count - 1);
            }
            Settings.SubmenuDisplayOrder = submenuDisplayOrder;

            // Build list of plugin IDs to save in config for the UI.
            // This is also easy as we basically save everything.
            List<Guid> uiDisplayOrder = pluginDisplayInfos.Select(info => info.Plugin.Id).ToList();
            Settings.UIDisplayOrder = uiDisplayOrder;

            // Build set of known plugins from the UI display order and save it in config.
            List<Guid> knownPlugins = new SortedSet<Guid>(uiDisplayOrder).ToList();
            Settings.KnownPlugins = knownPlugins;

            // Save icon files for default plugins. Icon files for pipeline plugins
            // are saved with the pipeline plugins, below.
            foreach (PluginDisplayInfo displayInfo in pluginDisplayInfos) {
                if (displayInfo.Plugin is DefaultPlugin) {
                    Settings.SetIconFileOfPlugin(displayInfo.Plugin.Id, displayInfo.Plugin.IconFile);
                }
            }

            // Build list of pipeline plugins from the list of all plugins.
            List<PipelinePluginInfo> pipelinePluginInfos =
                pluginDisplayInfos.Where(info => info.Plugin is PipelinePlugin)
                                  .Select(info => ((PipelinePlugin) info.Plugin).Info)
                                  .ToList();

            // Save pipeline plugins in config.
            Settings.PipelinePlugins = pipelinePluginInfos;

            // Save options.
            if (AddQuotesChk.Checked != Settings.AddQuotes) {
                Settings.AddQuotes = AddQuotesChk.Checked;
            }
            if ((AreQuotesOptionalChk.Enabled && AreQuotesOptionalChk.Checked) != Settings.AreQuotesOptional) {
                Settings.AreQuotesOptional = AreQuotesOptionalChk.Enabled && AreQuotesOptionalChk.Checked;
            }
            if (EmailLinksChk.Checked != Settings.MakeEmailLinks) {
                Settings.MakeEmailLinks = EmailLinksChk.Checked;
            }
            if (AppendSepForDirChk.Checked != Settings.AppendSeparatorForDirectories) {
                Settings.AppendSeparatorForDirectories = AppendSepForDirChk.Checked;
            }
            if (HiddenSharesChk.Checked != Settings.UseHiddenShares) {
                Settings.UseHiddenShares = HiddenSharesChk.Checked;
            }
            if (UseFQDNChk.Checked != Settings.UseFQDN) {
                Settings.UseFQDN = UseFQDNChk.Checked;
            }
            if (AlwaysShowSubmenuChk.Checked != Settings.AlwaysShowSubmenu) {
                Settings.AlwaysShowSubmenu = AlwaysShowSubmenuChk.Checked;
            }
            if (UseIconForSubmenuChk.Checked != Settings.UseIconForSubmenu) {
                Settings.UseIconForSubmenu = UseIconForSubmenuChk.Checked;
            }
            if (UsePreviewModeChk.Checked != Settings.UsePreviewMode) {
                Settings.UsePreviewMode = UsePreviewModeChk.Checked;
            }
            if ((UsePreviewModeInMainMenuChk.Enabled && UsePreviewModeInMainMenuChk.Checked) != Settings.UsePreviewModeInMainMenu) {
                Settings.UsePreviewModeInMainMenu = UsePreviewModeInMainMenuChk.Enabled && UsePreviewModeInMainMenuChk.Checked;
            }
            if (DropRedundantWordsChk.Checked != Settings.DropRedundantWords) {
                Settings.DropRedundantWords = DropRedundantWordsChk.Checked;
            }
            if (RecursiveCopyChk.Checked != Settings.CopyPathsRecursively) {
                Settings.CopyPathsRecursively = RecursiveCopyChk.Checked;
            }
            if (TrueLnkPathsChk.Checked != Settings.TrueLnkPaths) {
                Settings.TrueLnkPaths = TrueLnkPathsChk.Checked;
            }
            if (EnableSoftwareUpdateChk.Checked != (!Settings.DisableSoftwareUpdate)) {
                Settings.DisableSoftwareUpdate = !EnableSoftwareUpdateChk.Checked;
            }

            // Encode param is a little special (see above)
            UserSettings.StringEncodeParam encodeParam;
            if (EncodeURIWhitespaceChk.Enabled && EncodeURIWhitespaceChk.Checked) {
                if (EncodeURICharsChk.Enabled && EncodeURICharsChk.Checked) {
                    encodeParam = UserSettings.StringEncodeParam.All;
                } else {
                    encodeParam = UserSettings.StringEncodeParam.Whitespace;
                }
            } else {
                encodeParam = UserSettings.StringEncodeParam.None;
            }
            if (encodeParam != Settings.EncodeParam) {
                Settings.EncodeParam = encodeParam;
            }

            // "Copy on same line" is a little special (see above)
            if (CopyOnSameLineChk.Enabled) {
                string pathsSeparator = CopyOnSameLineChk.Checked ? PathsSeparatorOnSameLine : string.Empty;
                if (pathsSeparator != Settings.PathsSeparator) {
                    Settings.PathsSeparator = pathsSeparator;
                }
            }

            // Ctrl key plugin is a little special (see above)
            Guid? ctrlKeyPluginId = null;
            if (CtrlKeyPluginChk.Checked && CtrlKeyPluginCombo.SelectedIndex != -1) {
                ctrlKeyPluginId = (CtrlKeyPluginCombo.SelectedValue as Plugin).Id;
            }
            if (!ctrlKeyPluginId.Equals(Settings.CtrlKeyPlugin)) {
                Settings.CtrlKeyPlugin = ctrlKeyPluginId;
            }

            // Language is a little special since the combo box contains objects.
            string language = (LanguageCombo.SelectedItem as LanguageDisplayInfo)?.Name;
            if (language != Settings.Language) {
                // Warn user that switching language requires a logoff/logon (only once).
                if (!languageSwitchWarningShown) {
                    MessageBox.Show(this, Resources.MainForm_Msg_LanguageSwitched,
                        Resources.MainForm_Msg_LanguageSwitchedMsgTitle, MessageBoxButtons.OK,
                        MessageBoxIcon.Warning);
                    languageSwitchWarningShown = true;
                }
                Settings.Language = language;
            }

            // Now that everything is saved, disable the "Apply button".
            ApplyBtn.Enabled = false;
        }

        /// <summary>
        /// Determines if the selected plugins in the data grid are pipeline plugins.
        /// </summary>
        /// <returns><c>true</c> if all selected plugins are pipeline plugins.</returns>
        /// <remarks>
        /// If no plugin is selected, this method returns <c>true</c>.
        /// </remarks>
        private bool AreOnlyPipelinePluginsSelected()
        {
            bool onlyPipeline = true;

            for (int i = 0; onlyPipeline && i < PluginsDataGrid.SelectedRows.Count; ++i) {
                DataGridViewRow row = PluginsDataGrid.SelectedRows[i];
                Plugin rowPlugin = ((PluginDisplayInfo) row.DataBoundItem).Plugin;
                onlyPipeline = rowPlugin is PipelinePlugin;
            }

            return onlyPipeline;
        }

        /// <summary>
        /// Determines if the selected plugins in the data grid are pipeline
        /// or separator plugins.
        /// </summary>
        /// <returns><c>true</c> if all selected plugins are pipeline or
        /// separator plugins.</returns>
        /// <remarks>
        /// If no plugin is selected, this method returns <c>true</c>.
        /// </remarks>
        private bool AreOnlyPipelineOrSeparatorPluginsSelected()
        {
            bool onlyPipelineOrSeparator = true;

            for (int i = 0; onlyPipelineOrSeparator && i < PluginsDataGrid.SelectedRows.Count; ++i) {
                DataGridViewRow row = PluginsDataGrid.SelectedRows[i];
                Plugin rowPlugin = ((PluginDisplayInfo) row.DataBoundItem).Plugin;
                onlyPipelineOrSeparator = (rowPlugin is PipelinePlugin) || (rowPlugin is SeparatorPlugin);
            }

            return onlyPipelineOrSeparator;
        }

        /// <summary>
        /// Updates the state of buttons on the Commands page.
        /// Call this when selection changes somewhere.
        /// </summary>
        private void UpdatePluginButtons()
        {
            MovePluginUpBtn.Enabled = PluginsDataGrid.SelectedRows.Count == 1 &&
                PluginsDataGrid.SelectedRows[0].Index > 0;
            MovePluginDownBtn.Enabled = PluginsDataGrid.SelectedRows.Count == 1 &&
                PluginsDataGrid.SelectedRows[0].Index < (PluginsDataGrid.RowCount - 1);

            AddPipelinePluginBtn.Enabled = true;
            AddSeparatorBtn.Enabled = true;
            EditPipelinePluginBtn.Enabled = PluginsDataGrid.SelectedRows.Count == 1 && AreOnlyPipelinePluginsSelected();
            DuplicatePipelinePluginBtn.Enabled = EditPipelinePluginBtn.Enabled;
            RemovePluginBtn.Enabled = PluginsDataGrid.SelectedRows.Count == 1 && AreOnlyPipelineOrSeparatorPluginsSelected();

            ExportPipelinePluginsBtn.Enabled = PluginsDataGrid.SelectedRows.Count > 0 && AreOnlyPipelinePluginsSelected();
            ImportPipelinePluginsBtn.Enabled = true;
        }
        
        /// <summary>
        /// Loads the given icon file and displays it in the given image cell in
        /// the <see cref="IconCol"/> of the plugin data grid.
        /// </summary>
        /// <param name="iconFile">Path to icon file, or an empty string to display
        /// the default icon, or <c>null</c> to display no icon.</param>
        /// <param name="rowIconCell">Cell where to display the icon. Cannot
        /// be <c>null</c>.</param>
        private void LoadIconFileForCell(string iconFile, DataGridViewImageCell rowIconCell)
        {
            Debug.Assert(rowIconCell != null);

            // Assume row will not display an Icon.
            rowIconCell.ValueIsIcon = false;

            // Check if we have an icon file to display.
            if (iconFile != null) {
                if (iconFile.Length == 0) {
                    // This indicates that we need to use the default icon.
                    rowIconCell.Value = Resources.DefaultIcon;
                } else {
                    try {
                        // Icon file to load. Check file extension for .ico,
                        // which we need to display differently.
                        if (Path.GetExtension(iconFile).ToLower(CultureInfo.CurrentCulture) == ".ico") {
                            rowIconCell.ValueIsIcon = true;
                            rowIconCell.Value = new Icon(iconFile, DesiredIconSize);
                        } else {
                            using (var bitmap = new Bitmap(iconFile)) {
                                rowIconCell.Value = new Bitmap(bitmap, DesiredIconSize);
                            }
                        }
                    } catch (Exception e) {
                        if (e is InvalidOperationException || e is ArgumentException || e is IOException) {
                            // Failed to load the icon for some reason. Display a blank icon.
                            rowIconCell.ValueIsIcon = false;
                            rowIconCell.Value = Resources.BlankIcon;
                        } else {
                            throw;
                        }
                    }
                }
            } else {
                // No icon, set a blank image.
                rowIconCell.Value = Resources.BlankIcon;
            }
        }
        
        /// <summary>
        /// Called when the form is first loaded. We initialize controls here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            // Load settings in the form.
            LoadUserSettings();

            // Perform an initial button update.
            UpdatePluginButtons();
        }
        
        /// <summary>
        /// Called when the user clicks the "Apply" button. We'll need to save
        /// the user settings.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ApplyBtn_Click(object sender, EventArgs e)
        {
            // Save user settings but do not close the form.
            SaveUserSettings();
        }
        
        /// <summary>
        /// Called when the user clicks the "Cancel" button. We close the form.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void CancelBtn_Click(object sender, EventArgs e)
        {
            // Simply close the form.
            Close();
        }
        
        /// <summary>
        /// Called when the user clicks the "OK" button. We save the user settings
        /// then close the form.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void OKBtn_Click(object sender, EventArgs e)
        {
            // Save user settings then close the form.
            SaveUserSettings();
            Close();
        }

        /// <summary>
        /// Called when user presses the button to export user settings.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ExportUserSettingsBtn_Click(object sender, EventArgs e)
        {
            // In order to export complete settings, we need to apply changes if needed.
            bool canExport = true;
            if (ApplyBtn.Enabled) {
                canExport = MessageBox.Show(this, Resources.MainForm_Confirm_ExportUserSettingsSaving,
                    Resources.MainForm_Confirm_ExportUserSettingsMsgTitle, MessageBoxButtons.YesNo,
                    MessageBoxIcon.Question) == DialogResult.Yes;
                if (canExport) {
                    SaveUserSettings();
                }
            }
            if (canExport && ExportUserSettingsSaveDlg.ShowDialog(this) == DialogResult.OK) {
                int exitCode;
                using (new CursorChanger(this, Cursors.WaitCursor)) {
                    exitCode = UserSettings.ExportUserSettings(ExportUserSettingsSaveDlg.FileName);
                }
                if (exitCode == 0) {
                    MessageBox.Show(this, string.Format(CultureInfo.CurrentCulture,
                        Resources.MainForm_Msg_UserSettingsExported, ExportUserSettingsSaveDlg.FileName),
                        Resources.MainForm_Msg_UserSettingsExportedMsgTitle, MessageBoxButtons.OK,
                        MessageBoxIcon.Information);
                } else {
                    MessageBox.Show(this, string.Format(CultureInfo.CurrentCulture,
                        Resources.MainForm_Msg_UserSettingsNotExported, ExportUserSettingsSaveDlg.FileName,
                        exitCode), Resources.MainForm_Msg_UserSettingsNotExportedMsgTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        
        /// <summary>
        /// Called when the user checks or unchecks a checkbox in the Options tab.
        /// We'll enable the "Apply" button to allow the user to apply those changes.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void Chk_CheckedChanged(object sender, EventArgs e)
        {
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Add quotes" checkbox.
        /// We need to enable or disable the "...only if path contains space"
        /// checkbox when this occurs since it's conditional to the former.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddQuotesChk_CheckedChanged(object sender, EventArgs e)
        {
            AreQuotesOptionalChk.Enabled = AddQuotesChk.Checked;
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Encode whitespace"
        /// checkbox. We need to enable or disable the "Encode URI chars"
        /// checkbox when this occurs since it's conditional to the former.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void EncodeURIWhitespaceChk_CheckedChanged(object sender, EventArgs e)
        {
            EncodeURICharsChk.Enabled = EncodeURIWhitespaceChk.Checked;
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Use preview mode"
        /// checkbox. We need to enable or disable the "Use preview mode
        /// in main menu" checkbox when this occurs since it's conditional
        /// to the former.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void UsePreviewModeChk_CheckedChanged(object sender, EventArgs e)
        {
            UsePreviewModeInMainMenuChk.Enabled = UsePreviewModeChk.Checked;
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Use this command on Ctrl key" checkbox.
        /// We need to enable or disable the combo box when this occurs.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void CtrlKeyPluginChk_CheckedChanged(object sender, EventArgs e)
        {
            CtrlKeyPluginCombo.Enabled = CtrlKeyPluginChk.Checked;
        }

        /// <summary>
        /// Called when the user picks a new Ctrl key plugin the combo box.
        /// We need to validate the entry and possibly enable the "Apply" button.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CtrlKeyPluginCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Prevent recursion issues.
            if (!inCtrlKeyPluginComboBoxSelectedIndexChanged) {
                inCtrlKeyPluginComboBoxSelectedIndexChanged = true;
                try {
                    // Validate that user hasn't selected a separator. If he did, re-select previous value.
                    if (CtrlKeyPluginCombo.SelectedIndex != -1 && CtrlKeyPluginCombo.SelectedValue is SeparatorPlugin) {
                        CtrlKeyPluginCombo.SelectedIndex = lastCtrlKeyPluginSelectedIndex;
                    } else {
                        // This selection is OK, update our saved value and enable Apply button.
                        lastCtrlKeyPluginSelectedIndex = CtrlKeyPluginCombo.SelectedIndex;
                        ApplyBtn.Enabled = true;
                    }
                } finally {
                    inCtrlKeyPluginComboBoxSelectedIndexChanged = false;
                }
            }
        }

        /// <summary>
        /// Called when the user switches the selected language. We enable the Apply
        /// button here (since switching languages requires an app restart to work).
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LanguageCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            ApplyBtn.Enabled = true;
        }
        
        /// <summary>
        /// Called when the user clicks on the link to visit the PCC website.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SiteLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            // Link is the control's text.
            Process.Start(SiteLinkLbl.Text);
        }
        
        /// <summary>
        /// Called when the user clicks on the link to see the license.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LicenseTxtLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(Resources.MainForm_About_LicensePageURI);
        }
        
        /// <summary>
        /// Called when a link has been clicked related to the donations page.
        /// The URL to follow is in the link data.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void DonationLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start((string) e.Link.LinkData);
        }

        /// <summary>
        /// Called when a cell in the plugins data grid changes its "dirty" state
        /// (e.g., someone edited the value). Since we have checkboxes, we want to
        /// commit any changes to the data source immediately when their state changes.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CurrentCellDirtyStateChanged(object sender, EventArgs e)
        {
            if (PluginsDataGrid.IsCurrentCellDirty && PluginsDataGrid.CurrentCell is DataGridViewCheckBoxCell) {
                PluginsDataGrid.CommitEdit(DataGridViewDataErrorContexts.Commit);
            }
        }

        /// <summary>
        /// Called when the value of a cell in the plugins data grid changes. We
        /// use this opportunity to enable our "Apply" button.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            // Separators are disabled, so we can be sure that any changes are legitimately on plugins.
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when new rows are inserted in the plugins data grid. We use this
        /// opportunity to populate cells that cannot be populated through data binding.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
            for (int i = e.RowIndex; i < (e.RowIndex + e.RowCount); ++i) {
                DataGridViewRow row = PluginsDataGrid.Rows[i];
                Plugin rowPlugin = ((PluginDisplayInfo) row.DataBoundItem).Plugin;

                // The "Icon" column is not data-bound, because we can display two types
                // of images and this required setting a property of the cell that cannot
                // be controlled through data binding. We'll display icons here.
                DataGridViewImageCell rowIconCell = (DataGridViewImageCell) row.Cells[IconCol.Name];
                string iconFile = rowPlugin is SeparatorPlugin ? null : rowPlugin.IconFile;
                LoadIconFileForCell(iconFile, rowIconCell);

                // If the plugin's icon file can be edited, set tooltip text.
                // Otherwise, clear it and set the cell as read-only.
                if (rowPlugin is PipelinePlugin || rowPlugin is DefaultPlugin) {
                    rowIconCell.ToolTipText = Resources.MainForm_PluginsDataGrid_IconToolTipText;
                } else {
                    rowIconCell.ToolTipText = string.Empty;
                    rowIconCell.ReadOnly = true;
                }

                // Disable all rows that correspond to separators and remove their checkboxes.
                if (rowPlugin is SeparatorPlugin) {
                    row.Cells[InMainMenuCol.Index] = new DataGridViewTextBoxCell();
                    row.Cells[InSubmenuCol.Index] = new DataGridViewTextBoxCell();
                    row.ReadOnly = true;
                }
            }
        }

        /// <summary>
        /// Called when a cell is clicked in the plugins data grid.
        /// We need to handle clicks in icon cells to ask for an icon file.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            // Immediately get current key modifiers to be able to determine the action to perform.
            Keys modifierKeys = ModifierKeys & Keys.Modifiers;

            // We're only interested in clicks in icon cells; other cells are
            // data-bound so will take care of themselves.
            if (e.ColumnIndex == IconCol.Index) {
                // Get plugin info.
                DataGridViewRow row = PluginsDataGrid.Rows[e.RowIndex];
                Plugin rowPlugin = ((PluginDisplayInfo) row.DataBoundItem).Plugin;
                DataGridViewImageCell rowIconCell = (DataGridViewImageCell) row.Cells[e.ColumnIndex];

                // Make sure this plugin's icon file is editable. We can't edit
                // icons of COM plugins or separator plugins.
                if (!rowIconCell.ReadOnly) {
                    // Check action to perform.
                    if (modifierKeys == Keys.Shift) {
                        // Plugin must now use the default icon.
                        rowPlugin.IconFile = string.Empty;
                        LoadIconFileForCell(string.Empty, rowIconCell);
                    } else if (modifierKeys == Keys.Control) {
                        // We must clear plugin's icon file.
                        rowPlugin.IconFile = null;
                        LoadIconFileForCell(null, rowIconCell);
                    } else if (modifierKeys == Keys.None) {
                        // Ask user to provide a new icon file for this plugin. If the plugin
                        // already has an icon file, start in the same folder as the previous icon.
                        if (!string.IsNullOrEmpty(rowPlugin.IconFile)) {
                            ChoosePluginIconOpenDlg.InitialDirectory = Path.GetDirectoryName(rowPlugin.IconFile);
                        }
                        if (ChoosePluginIconOpenDlg.ShowDialog(this) == DialogResult.OK) {
                            // Save the new icon.
                            rowPlugin.IconFile = ChoosePluginIconOpenDlg.FileName;

                            // Immediately load icon file for this plugin to update the grid.
                            LoadIconFileForCell(rowPlugin.IconFile, rowIconCell);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Called when a cell is double-clicked in the plugins data grid.
        /// If user double-clicked on a pipeline plugin, we want to act as
        /// if the Edit button was pressed.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // If user double-clicked in the Command column and the Edit button is enabled,
            // act as if that button was pressed.
            if (e.ColumnIndex == PluginCol.Index && EditPipelinePluginBtn.Enabled) {
                EditPipelinePluginBtn_Click(EditPipelinePluginBtn, EventArgs.Empty);
            }
        }

        /// <summary>
        /// Called when the user moves the cursor inside a cell of the plugin
        /// data grid. We use this opportunity to modify the cursor.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CellMouseEnter(object sender, DataGridViewCellEventArgs e)
        {
            // If the cursor enters an editable icon file cell, change the cursor to a hand.
            if (e.RowIndex >= 0 && e.ColumnIndex == IconCol.Index &&
                !PluginsDataGrid.Rows[e.RowIndex].Cells[e.ColumnIndex].ReadOnly) {

                PluginsDataGrid.Cursor = Cursors.Hand;
            } else {
                PluginsDataGrid.ResetCursor();
            }
        }

        /// <summary>
        /// Called when a new plugin is selected in the data grid. We need to
        /// update our preview textbox.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_SelectionChanged(object sender, EventArgs e)
        {
            // Update preview control.
            PreviewCtrl.Plugin = PluginsDataGrid.SelectedRows.Count == 1
                ? ((PluginDisplayInfo) PluginsDataGrid.SelectedRows[0].DataBoundItem).Plugin : null;

            // Also update plugin-related buttons.
            UpdatePluginButtons();
        }

        /// <summary>
        /// Called when the Up button is pressed. We need to move the
        /// selected plugin up one position in the grid.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MovePluginUpBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1);

            DataGridViewRow row = PluginsDataGrid.SelectedRows[0];
            int rowIndex = row.Index;
            PluginDisplayInfo displayInfo = (PluginDisplayInfo) row.DataBoundItem;
            pluginDisplayInfos.RemoveAt(rowIndex);
            int newRowIndex = rowIndex - 1;
            pluginDisplayInfos.Insert(newRowIndex, displayInfo);

            // Reselect same plugin.
            PluginsDataGrid.ClearSelection();
            PluginsDataGrid.Rows[newRowIndex].Selected = true;
            if (!PluginsDataGrid.Rows[newRowIndex].Displayed) {
                PluginsDataGrid.FirstDisplayedScrollingRowIndex = newRowIndex;
            }

            // Enable "Apply" button.
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when the Down button is pressed. We need to move the
        /// selected plugin down one position in the grid.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MovePluginDownBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1);

            DataGridViewRow row = PluginsDataGrid.SelectedRows[0];
            int rowIndex = row.Index;
            PluginDisplayInfo displayInfo = (PluginDisplayInfo) row.DataBoundItem;
            pluginDisplayInfos.RemoveAt(rowIndex);
            int newRowIndex = rowIndex + 1;
            pluginDisplayInfos.Insert(newRowIndex, displayInfo);

            // Reselect same plugin.
            PluginsDataGrid.ClearSelection();
            PluginsDataGrid.Rows[newRowIndex].Selected = true;
            while (!PluginsDataGrid.Rows[newRowIndex].Displayed) {
                PluginsDataGrid.FirstDisplayedScrollingRowIndex += 1;
            }

            // Enable "Apply" button.
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when the New button is pressed. We display a form to create
        /// a new pipeline plugin and maybe add it to the list.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void AddPipelinePluginBtn_Click(object sender, EventArgs e)
        {
            // Ask user to create a new plugin.
            PipelinePluginInfo pluginInfo = PipelinePluginEditor.EditPlugin(this, null);
            if (pluginInfo != null) {
                // User pressed OK. Add the new plugin to temp pipeline plugins.
                tempPipelinePluginsHelper.Add(pluginInfo);

                // Also add the new plugin to the list view and select it.
                Plugin plugin = pluginInfo.ToPlugin();
                PluginDisplayInfo displayInfo = new PluginDisplayInfo(plugin) {
                    ShowInSubmenu = true,
                };
                pluginDisplayInfos.Add(displayInfo);
                PluginsDataGrid.ClearSelection();
                PluginsDataGrid.Rows[pluginDisplayInfos.Count - 1].Selected = true;
                if (!PluginsDataGrid.Rows[pluginDisplayInfos.Count - 1].Displayed) {
                    PluginsDataGrid.FirstDisplayedScrollingRowIndex = pluginDisplayInfos.Count - 1;
                }
                    
                // All this will enable the "Apply" button.
                ApplyBtn.Enabled = true;
            }
        }

        /// <summary>
        /// Called when the Separator button is pressed. We need to
        /// add a new separator below the current selection point.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void AddSeparatorBtn_Click(object sender, EventArgs e)
        {
            SeparatorPlugin plugin = new SeparatorPlugin();
            PluginDisplayInfo displayInfo = new PluginDisplayInfo(plugin);
            int insertionIndex;
            if (PluginsDataGrid.SelectedRows.Count == 1) {
                insertionIndex = PluginsDataGrid.SelectedRows[0].Index + 1;
                pluginDisplayInfos.Insert(insertionIndex, displayInfo);
            } else {
                pluginDisplayInfos.Add(displayInfo);
                insertionIndex = pluginDisplayInfos.Count - 1;
            }

            // Make sure new separator is selected.
            PluginsDataGrid.ClearSelection();
            PluginsDataGrid.Rows[insertionIndex].Selected = true;
            if (!PluginsDataGrid.Rows[insertionIndex].Displayed) {
                PluginsDataGrid.FirstDisplayedScrollingRowIndex = insertionIndex;
            }

            // All this will enable the "Apply" button.
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when the Edit button is pressed. We display a form
        /// to let user edit a pipeline plugin.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void EditPipelinePluginBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1);

            // Get reference to the selected pipeline plugin.
            DataGridViewRow row = PluginsDataGrid.SelectedRows[0];
            int rowIndex = row.Index;
            PluginDisplayInfo oldDisplayInfo = (PluginDisplayInfo) row.DataBoundItem;
            PipelinePluginInfo pluginInfo = ((PipelinePlugin) oldDisplayInfo.Plugin).Info;

            // Ask user to edit the plugin info.
            try {
                PipelinePluginInfo newPluginInfo = PipelinePluginEditor.EditPlugin(this, pluginInfo);
                if (newPluginInfo != null) {
                    // Re-add the plugin to temp pipeline plugins so that it can be updated.
                    tempPipelinePluginsHelper.Add(newPluginInfo);

                    // Replace the existing plugin object in the data grid.
                    Debug.Assert(newPluginInfo.Id == pluginInfo.Id);
                    PluginDisplayInfo newDisplayInfo = new PluginDisplayInfo(newPluginInfo.ToPlugin()) {
                        ShowInMainMenu = oldDisplayInfo.ShowInMainMenu,
                        ShowInSubmenu = oldDisplayInfo.ShowInSubmenu,
                    };
                    pluginDisplayInfos.RemoveAt(rowIndex);
                    pluginDisplayInfos.Insert(rowIndex, newDisplayInfo);

                    // Make sure to reselect the plugin.
                    PluginsDataGrid.ClearSelection();
                    PluginsDataGrid.Rows[rowIndex].Selected = true;
                    if (!PluginsDataGrid.Rows[rowIndex].Displayed) {
                        PluginsDataGrid.FirstDisplayedScrollingRowIndex = rowIndex;
                    }

                    // All this will enable the "Apply" button.
                    ApplyBtn.Enabled = true;
                }
            } catch (InvalidPipelineException) {
                // Maybe a plugin created by a newer version of Path Copy Copy?
                MessageBox.Show(this, Resources.MainForm_Error_InvalidPipelinePluginEncodedElements,
                    Resources.MainForm_Confirm_EditMsgTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        /// <summary>
        /// Called when the Duplicate button is pressed. We duplicate
        /// the selected pipeline plugin.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void DuplicatePipelinePluginBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1);

            // Get reference to the selected pipeline plugin.
            DataGridViewRow row = PluginsDataGrid.SelectedRows[0];
            int rowIndex = row.Index;
            PluginDisplayInfo displayInfo = (PluginDisplayInfo) row.DataBoundItem;
            PipelinePluginInfo pluginInfo = ((PipelinePlugin) displayInfo.Plugin).Info;

            // Duplicate pipeline plugin and change its name.
            PipelinePluginInfo newPluginInfo = pluginInfo.Duplicate();
            newPluginInfo.Description += Resources.MainForm_PluginsDataGrid_DuplicateSuffix;

            // Add the new plugin to temp pipeline plugins.
            tempPipelinePluginsHelper.Add(newPluginInfo);

            // Also add the new plugin to the list view and select it.
            Plugin newPlugin = newPluginInfo.ToPlugin();
            PluginDisplayInfo newDisplayInfo = new PluginDisplayInfo(newPlugin) {
                ShowInMainMenu = displayInfo.ShowInMainMenu,
                ShowInSubmenu = displayInfo.ShowInSubmenu,
            };
            pluginDisplayInfos.Insert(rowIndex + 1, newDisplayInfo);
            PluginsDataGrid.ClearSelection();
            PluginsDataGrid.Rows[rowIndex + 1].Selected = true;
            if (!PluginsDataGrid.Rows[rowIndex + 1].Displayed) {
                PluginsDataGrid.FirstDisplayedScrollingRowIndex = rowIndex + 1;
            }
                    
            // All this will enable the "Apply" button.
            ApplyBtn.Enabled = true;
        }

        /// <summary>
        /// Called when the Remove button is pressed. We remove
        /// selected plugins.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RemovePluginBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1); 
 
            // Check type of selected plugin.
            DataGridViewRow row = PluginsDataGrid.SelectedRows[0]; 
            int rowIndex = row.Index; 
            PluginDisplayInfo displayInfo = (PluginDisplayInfo) row.DataBoundItem; 

            if (displayInfo.Plugin is PipelinePlugin pipelinePlugin) {
                // First confirm since this is an "irreversible" action. We have a string 
                // in resources for this message and it includes a format placeholder 
                // for the plugin description. 
                PipelinePluginInfo pluginInfo = pipelinePlugin.Info; 
                DialogResult res = MessageBox.Show(this, string.Format(CultureInfo.CurrentCulture,
                    Resources.REMOVE_PIPELINE_PLUGIN_MESSAGE, pluginInfo.Description), 
                    Resources.REMOVE_PIPELINE_PLUGIN_TITLE, MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Warning); 
                if (res == DialogResult.OK) { 
                    // Remove plugin from data grid and from temp pipeline plugins.
                    tempPipelinePluginsHelper.Remove(pluginInfo);
                    pluginDisplayInfos.RemoveAt(rowIndex);
 
                    // All this will enable the "Apply" button. 
                    ApplyBtn.Enabled = true; 
                }
            } else {
                Debug.Assert(displayInfo.Plugin is SeparatorPlugin);

                // No need to confirm removing separator plugins, since it's reversible.
                pluginDisplayInfos.RemoveAt(rowIndex);

                // All this will enable the "Apply" button.
                ApplyBtn.Enabled = true;
            }
        }

        /// <summary>
        /// Called when the Export button is pressed. We ask user to
        /// export the selected pipeline plugins to disk.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ExportPipelinePluginsBtn_Click(object sender, EventArgs e)
        {
            // Ask user where to save the data.
            if (ExportPipelinePluginsSaveDlg.ShowDialog(this) == DialogResult.OK) {
                // Create a pipeline plugins collection and add all selected plugins to it.
                PipelinePluginCollection collection = new PipelinePluginCollection();
                foreach (DataGridViewRow row in PluginsDataGrid.SelectedRows) {
                    collection.Plugins.Add(((PipelinePlugin) ((PluginDisplayInfo) row.DataBoundItem).Plugin).Info);
                }

                // Open file where to save data and serialize collection in XML.
                // Use file extension to determine if we should use legacy serializing.
                using (FileStream fstream = new FileStream(ExportPipelinePluginsSaveDlg.FileName, FileMode.Create)) {
                    collection.ToXML(fstream, PipelinePluginsExtToSerializerVersion[
                        Path.GetExtension(ExportPipelinePluginsSaveDlg.FileName).ToLower(CultureInfo.CurrentCulture)]);
                }
            }
        }

        /// <summary>
        /// Called when the Import button is pressed. We allow the user
        /// to import pipeline plugins from disk.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ImportPipelinePluginsBtn_Click(object sender, EventArgs e)
        {
            // Ask user to select a file.
            if (ImportPipelinePluginsOpenDlg.ShowDialog(this) == DialogResult.OK) {
                // Open the file for reading and deserialize a collection of pipeline plugins.
                PipelinePluginCollection collection = null;
                using (FileStream fstream = new FileStream(ImportPipelinePluginsOpenDlg.FileName, FileMode.Open)) {
                    try {
                        collection = PipelinePluginCollection.FromXML(fstream);

                        // If this is a legacy file without required versions, compute them now.
                        if (PipelinePluginsExtToSerializerVersion[Path.GetExtension(ImportPipelinePluginsOpenDlg.FileName).ToLower(CultureInfo.CurrentCulture)] == PipelinePluginXmlSerializerVersion.V1) {
                            foreach (PipelinePluginInfo info in collection.Plugins) {
                                try {
                                    Pipeline pipeline = PipelineDecoder.DecodePipeline(info.EncodedElements);
                                    info.RequiredVersion = pipeline.RequiredVersion;
                                } catch (InvalidPipelineException) {
                                    // Most illogical, Captain. Why can't we compute the required version
                                    // of something legacy? Regardless, there's not much we can do.
                                }
                            }
                        }
                    } catch (InvalidOperationException) {
                        Debug.Assert(collection == null);
                    }
                }

                // Make sure it worked.
                if (collection != null && collection.Plugins.Count > 0) {
                    // Go through list of plugins and locate any plugin overwrites.
                    ImportedPipelinePluginOverwrites pluginOverwrites = new ImportedPipelinePluginOverwrites();
                    for (int i = 0; i < pluginDisplayInfos.Count; ++i) {
                        PipelinePluginInfo existingInfo = (pluginDisplayInfos[i].Plugin as PipelinePlugin)?.Info;
                        if (existingInfo != null) {
                            PipelinePluginInfo newInfo = collection.Plugins.Find(pl => pl.Equals(existingInfo));
                            if (newInfo != null) {
                                pluginOverwrites.OverwriteInfos[newInfo] =
                                    new ImportedPipelinePluginOverwriteInfo(existingInfo, i);
                            }
                        }
                    }

                    // Popup a form asking the user which plugins to import.
                    bool shouldContinue;
                    using (ImportPipelinePluginsForm importForm = new ImportPipelinePluginsForm()) {
                        shouldContinue = importForm.SelectPlugins(this, ref collection, ref pluginOverwrites);
                    }
                    if (shouldContinue) {
                        // Check if any plugin to import will overwrite existing ones.
                        // If so, ask user to confirm overwrite.
                        if (pluginOverwrites.OverwriteInfos.Count > 0) {
                            shouldContinue = MessageBox.Show(this, Resources.MainForm_Confirm_ImportPipelinePluginOverwrite,
                                Resources.MainForm_Confirm_ImportMsgTitle, MessageBoxButtons.OKCancel,
                                MessageBoxIcon.Warning) == DialogResult.OK;
                        }
                        if (shouldContinue) {
                            // Reset all Global flags so that imported plugins are user-specific.
                            collection.Plugins.ForEach(pl => pl.Global = false);

                            // Scan plugins to import. If the plugin already exists, overwrite the existing
                            // one, otherwise add it at the end.
                            bool addedNewPlugins = false;
                            foreach (var newPluginInfo in collection.Plugins) {
                                Plugin newPlugin = newPluginInfo.ToPlugin();
                                if (pluginOverwrites.OverwriteInfos.TryGetValue(newPluginInfo, out var overwriteInfo)) {
                                    PluginDisplayInfo displayInfo = pluginDisplayInfos[overwriteInfo.OldIndex];
                                    displayInfo.Plugin = newPlugin;
                                } else {
                                    PluginDisplayInfo newDisplayInfo = new PluginDisplayInfo(newPlugin) {
                                        ShowInSubmenu = true,
                                    };
                                    pluginDisplayInfos.Add(newDisplayInfo);
                                    addedNewPlugins = true;
                                }
                            }

                            // Since temp pipeline plugins need to be refreshed,
                            // simply assign everything and let it sort it out.
                            tempPipelinePluginsHelper.Assign(pluginDisplayInfos.Select(info => info.Plugin));

                            // If we added new plugins, select the last one.
                            if (addedNewPlugins) {
                                PluginsDataGrid.ClearSelection();
                                PluginsDataGrid.Rows[PluginsDataGrid.RowCount - 1].Selected = true;
                                PluginsDataGrid.FirstDisplayedScrollingRowIndex = PluginsDataGrid.RowCount - 1;
                            }

                            // All this will enable the "Apply" button.
                            ApplyBtn.Enabled = true;
                        }
                    }
                } else if (collection == null) {
                    // Error deserializing the XML document for some reason.
                    MessageBox.Show(this, Resources.MainForm_Error_ImportPipelinePluginBadXMLFormat,
                        Resources.MainForm_Confirm_ImportMsgTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                } else {
                    Debug.Assert(collection.Plugins.Count == 0);
                    // Empty XML document for some reason.
                    MessageBox.Show(this, Resources.MainForm_Error_ImportPipelinePluginEmptyXML,
                        Resources.MainForm_Confirm_ImportMsgTitle, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// Called when the user presses the Help button in the form's caption bar.
        /// We navigate to the wiki to show help in such a case.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MainForm_HelpButtonClicked(object sender, CancelEventArgs e)
        {
            // Open wiki link to Settings page, then cancel the event to avoid
            // displaying a help mouse pointer like the default behavior.
            Process.Start(Resources.WikiLink_Settings);
            e.Cancel = true;
        }
    }
}
