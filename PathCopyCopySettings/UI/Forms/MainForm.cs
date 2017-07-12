// MainForm.cs
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
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Base;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Utils;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Main form of the Path Copy Copy settings application.
    /// </summary>
    public partial class MainForm : Form
    {
        /// Paths separator that copies multiple paths on the same line.
        private const string PATHS_SEPARATOR_ON_SAME_LINE = " ";

        /// Name of our LICENSE file.
        private const string LICENSE_TXT_FILE = "LICENSE.TXT";

        /// URI of the Donations page.
        private const string DONATIONS_PAGE_URI = @"https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=LM5B9WNTH4KN4&lc=CA&item_name=Charles%20Lechasseur&item_number=PathCopyCopy&currency_code=CAD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted";

        /// URI of the page specifying the Path Copy Copy license on CodePlex.
        private const string LICENSE_PAGE_URI = @"https://pathcopycopy.codeplex.com/license";

        /// Extension of legacy pipeline plugins export files.
        private const string LEGACY_PIPELINE_PLUGINS_EXT = ".pccpp";

        /// The standard DPI values in Windows.
        private const int STANDARD_DPI = 96;

        /// Path to license file, either locally or online.
        private string licensePath;

        /// Object used to access user settings.
        private UserSettings settings;

        /// BindingList used to store all existing plugins and their display infos.
        private BindingList<PluginDisplayInfo> pluginDisplayInfos;

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
                if (((int) gridViewGraphics.DpiX) != STANDARD_DPI || ((int) gridViewGraphics.DpiY) != STANDARD_DPI) {
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

            // Locate license file.
            Assembly thisAssembly = this.GetType().Assembly;
            licensePath = LocateLicenseFile(thisAssembly);

            // Init about "box" controls.
            Version version = thisAssembly.GetName().Version;
            int numComponents;
            if (version.Revision > 0) {
                numComponents = 4;
            } else if (version.Build > 0) {
                numComponents = 3;
            } else {
                numComponents = 2;
            }
            ProductAndVersionLbl.Text = String.Format(ProductAndVersionLbl.Text, version.ToString(numComponents));
            CopyrightLbl.Text = GetAssemblyCopyrightString(thisAssembly);
            MainToolTip.SetToolTip(CodePlexSiteLinkLbl, CodePlexSiteLinkLbl.Text);
            MainToolTip.SetToolTip(LicenseTxtLinkLbl, licensePath);
            DonationLinkLbl.Links[0].LinkData = DONATIONS_PAGE_URI;
            MainToolTip.SetToolTip(DonationLinkLbl, DONATIONS_PAGE_URI);
        }
        
        /// <summary>
        /// Given the reference to an <see cref="Assembly"/>, returns the copyright
        /// string as found in its <see cref="AssemblyCopyrightAttribute"/>.
        /// </summary>
        private string GetAssemblyCopyrightString(Assembly assembly)
        {
            string copyrightString = null;

            // Look for custom attribute containing copyright string.
            object[] copyrightAttributes = assembly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), true);
            if (copyrightAttributes.Length != 0) {
                // Extract the entire string from the attribute.
                string entireCopyrightString = ((AssemblyCopyrightAttribute) copyrightAttributes[0]).Copyright;

                // If there is a period, stop there. We don't want the part about LICENSE.TXT.
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
                copyrightString = "(c) 2008-2017, Charles Lechasseur";
            }

            // Replace "(c)" with the copyright symbol
            Debug.Assert(!String.IsNullOrEmpty(copyrightString));
            copyrightString = copyrightString.Replace("(c)", "\u00A9");

            return copyrightString;
        }

        /// <summary>
        /// Looks for the LICENSE.TXT file on disk and returns its full path.
        /// If not found, returns the URL of the license file online.
        /// </summary>
        /// <param name="assembly">Assembly used to locate license file on disk.</param>
        /// <returns>Path to license file, on disk or online.</returns>
        private string LocateLicenseFile(Assembly assembly)
        {
            Debug.Assert(assembly != null);

            // In a normal installation, the license file will be located right next to the executable.
            string licensePath = Path.Combine(Path.GetDirectoryName(Path.GetFullPath(assembly.Location)), LICENSE_TXT_FILE);
            FileInfo licenseFileInfo = new FileInfo(licensePath);
            if (!licenseFileInfo.Exists) {
                // In a dev environment, license file is three directories up.
                FileInfo[] fileInfos = null;
                DirectoryInfo dirInfo = licenseFileInfo.Directory;
                for (int i = 0; dirInfo != null && i < 3; ++i) {
                    dirInfo = dirInfo.Parent;
                }
                if (dirInfo != null) {
                    fileInfos = dirInfo.GetFiles(LICENSE_TXT_FILE);
                }
                if (fileInfos != null && fileInfos.Length != 0) {
                    Debug.Assert(fileInfos.Length == 1);
                    licenseFileInfo = fileInfos[0];
                } else {
                    licenseFileInfo = null;
                }
            }
            
            // If license file was found, return its full path, otherwise return online URL.
            return licenseFileInfo != null ? licenseFileInfo.FullName : LICENSE_PAGE_URI;
        }
        
        /// <summary>
        /// Loads the options and populates the form.
        /// </summary>
        private void LoadUserSettings()
        {
            Debug.Assert(settings != null);

            // Check if we remembered form position.
            int posX = settings.SettingsFormPosX;
            int posY = settings.SettingsFormPosY;
            if (posX >= 0 && posY >= 0) {
                this.StartPosition = FormStartPosition.Manual;
                this.Location = new Point(posX, posY);
            }

            // Check if we remembered form size.
            int sizeWidth = settings.SettingsFormSizeWidth;
            int sizeHeight = settings.SettingsFormSizeHeight;
            if (sizeWidth >= this.MinimumSize.Width) {
                this.Size = new Size(sizeWidth, this.Size.Height);
            }
            if (sizeHeight >= this.MinimumSize.Height) {
                this.Size = new Size(this.Size.Width, sizeHeight);
            }

            // Load list of plugins in default order.
            List<Plugin> pluginsInDefaultOrder = PluginsRegistry.GetPluginsInDefaultOrder(settings);

            // Create sorted set of all plugins from the list above, to be able to perform lookups.
            SortedPluginsSet setOfAllPlugins = new SortedPluginsSet(pluginsInDefaultOrder);

            // Create binding list to store plugins to display. Populate it by ordering
            // plugins using UI display order and known plugins from settings.
            List<Guid> uiDisplayOrder = settings.UIDisplayOrder;
            if (uiDisplayOrder == null) {
                // No display order, just use all plugins in default order
                uiDisplayOrder = new List<Guid>();
                foreach (Plugin plugin in pluginsInDefaultOrder) {
                    uiDisplayOrder.Add(plugin.Id);
                }
            }
            List<Guid> knownPlugins = settings.KnownPlugins;
            SortedSet<Guid> knownPluginsAsSet = null;
            if (knownPlugins != null) {
                knownPluginsAsSet = new SortedSet<Guid>(knownPlugins);
            }
            List<Plugin> plugins = PluginsRegistry.OrderPluginsToDisplay(setOfAllPlugins,
                uiDisplayOrder, knownPluginsAsSet, pluginsInDefaultOrder);
            pluginDisplayInfos = new BindingList<PluginDisplayInfo>();
            foreach (Plugin plugin in plugins) {
                pluginDisplayInfos.Add(new PluginDisplayInfo(plugin));
            }

            // Scan display infos and set the ShowInMainMenu/Submenu fields
            // according to whether plugins are found in corresponding settings.
            List<Guid> mainMenuDisplayOrder = settings.MainMenuDisplayOrder;
            SortedSet<Guid> mainMenuPlugins;
            if (mainMenuDisplayOrder != null) {
                mainMenuPlugins = new SortedSet<Guid>(mainMenuDisplayOrder);
            } else {
                // No main menu plugin specified in settings, use default.
                mainMenuPlugins = new SortedSet<Guid>();
                mainMenuPlugins.Add(new Guid(Resources.LONG_PATH_PLUGIN_ID));
            }
            List<Guid> submenuDisplayOrder = settings.SubmenuDisplayOrder;
            SortedSet<Guid> submenuPlugins;
            if (submenuDisplayOrder != null) {
                submenuPlugins = new SortedSet<Guid>(submenuDisplayOrder);
            } else {
                // No submenu plugins specified in settings, use all plugins in default order.
                submenuPlugins = new SortedSet<Guid>();
                foreach (Plugin plugin in pluginsInDefaultOrder) {
                    submenuPlugins.Add(plugin.Id);
                }
            }
            foreach (PluginDisplayInfo info in pluginDisplayInfos) {
                if (!(info.Plugin is SeparatorPlugin)) {
                    info.ShowInMainMenu = mainMenuPlugins.Contains(info.Plugin.Id);
                    info.ShowInSubmenu = submenuPlugins.Contains(info.Plugin.Id);
                }
            }

            // Set binding list as data source for the data grid.
            PluginsDataGrid.DataSource = pluginDisplayInfos;

            // Set options checkboxes.
            AddQuotesChk.Checked = settings.AddQuotes;
            EmailLinksChk.Checked = settings.MakeEmailLinks;
            HiddenSharesChk.Checked = settings.UseHiddenShares;
            AlwaysShowSubmenuChk.Checked = settings.AlwaysShowSubmenu;
            UseIconForSubmenuChk.Checked = settings.UseIconForSubmenu;
            UsePreviewModeChk.Checked = settings.UsePreviewMode;
            DropRedundantWordsChk.Checked = settings.DropRedundantWords;
            EnableSoftwareUpdateChk.Checked = !settings.DisableSoftwareUpdate;

            // Encode param is a little special since it has two checkboxes.
            switch (settings.EncodeParam) {
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
                    Debug.Fail(String.Format("Unknown encode param: {0}", settings.EncodeParam));
                    EncodeURIWhitespaceChk.Enabled = false;
                    break;
                }
            }

            // "Copy on same line" is a little special since it could be any string.
            Debug.Assert(!CopyOnSameLineChk.Checked);
            string pathsSeparator = settings.PathsSeparator;
            if (pathsSeparator == PATHS_SEPARATOR_ON_SAME_LINE) {
                CopyOnSameLineChk.Checked = true;
            } else if (!String.IsNullOrEmpty(pathsSeparator)) {
                CopyOnSameLineChk.Enabled = false;
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
            Debug.Assert(settings != null);
            Debug.Assert(pluginDisplayInfos != null);

            // Build list of plugin IDs to save in config for main menu.
            // This is easy: we jolt down the plugins marked as display
            // in main menu without worrying about separators.
            List<Guid> mainMenuDisplayOrder = new List<Guid>();
            foreach (PluginDisplayInfo displayInfo in pluginDisplayInfos) {
                if (displayInfo.ShowInMainMenu) {
                    mainMenuDisplayOrder.Add(displayInfo.Plugin.Id);
                }
            }

            // If the only plugin left to show in the main menu is our default one,
            // clear the value instead.
            if (mainMenuDisplayOrder.Count == 1 && mainMenuDisplayOrder[0] == new Guid(Resources.LONG_PATH_PLUGIN_ID)) {
                mainMenuDisplayOrder = null;
            }
            settings.MainMenuDisplayOrder = mainMenuDisplayOrder;

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
            settings.SubmenuDisplayOrder = submenuDisplayOrder;

            // Build list of plugin IDs to save in config for the UI.
            // This is also easy as we basically save everything.
            List<Guid> uiDisplayOrder = new List<Guid>();
            foreach (PluginDisplayInfo displayInfo in pluginDisplayInfos) {
                uiDisplayOrder.Add(displayInfo.Plugin.Id);
            }
            settings.UIDisplayOrder = uiDisplayOrder;

            // Build set of known plugins from the UI display order and save it in config.
            SortedSet<Guid> knownPluginsAsSet = new SortedSet<Guid>(uiDisplayOrder);
            List<Guid> knownPlugins = new List<Guid>();
            foreach (Guid pluginId in knownPluginsAsSet) {
                knownPlugins.Add(pluginId);
            }
            settings.KnownPlugins = knownPlugins;

            // Save icon files for default plugins. Icon files for pipeline plugins
            // are saved with the pipeline plugins, below.
            foreach (PluginDisplayInfo displayInfo in pluginDisplayInfos) {
                if (displayInfo.Plugin is DefaultPlugin) {
                    settings.SetIconFileOfPlugin(displayInfo.Plugin.Id, displayInfo.Plugin.IconFile);
                }
            }

            // Build list of pipeline plugins from the list of all plugins.
            List<PipelinePluginInfo> pipelinePluginInfos = new List<PipelinePluginInfo>();
            foreach (PluginDisplayInfo displayInfo in pluginDisplayInfos) {
                if (displayInfo.Plugin is PipelinePlugin) {
                    pipelinePluginInfos.Add(((PipelinePlugin) displayInfo.Plugin).Info);
                }
            }

            // Save pipeline plugins in config.
            settings.PipelinePlugins = pipelinePluginInfos;

            // Save options.
            if (AddQuotesChk.Checked != settings.AddQuotes) {
                settings.AddQuotes = AddQuotesChk.Checked;
            }
            if (EmailLinksChk.Checked != settings.MakeEmailLinks) {
                settings.MakeEmailLinks = EmailLinksChk.Checked;
            }
            if (HiddenSharesChk.Checked != settings.UseHiddenShares) {
                settings.UseHiddenShares = HiddenSharesChk.Checked;
            }
            if (AlwaysShowSubmenuChk.Checked != settings.AlwaysShowSubmenu) {
                settings.AlwaysShowSubmenu = AlwaysShowSubmenuChk.Checked;
            }
            if (UseIconForSubmenuChk.Checked != settings.UseIconForSubmenu) {
                settings.UseIconForSubmenu = UseIconForSubmenuChk.Checked;
            }
            if (UsePreviewModeChk.Checked != settings.UsePreviewMode) {
                settings.UsePreviewMode = UsePreviewModeChk.Checked;
            }
            if (DropRedundantWordsChk.Checked != settings.DropRedundantWords) {
                settings.DropRedundantWords = DropRedundantWordsChk.Checked;
            }
            if (EnableSoftwareUpdateChk.Checked != (!settings.DisableSoftwareUpdate)) {
                settings.DisableSoftwareUpdate = !EnableSoftwareUpdateChk.Checked;
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
            if (encodeParam != settings.EncodeParam) {
                settings.EncodeParam = encodeParam;
            }

            // "Copy on same line" is a little special (see above)
            if (CopyOnSameLineChk.Enabled) {
                string pathsSeparator = CopyOnSameLineChk.Checked
                    ? PATHS_SEPARATOR_ON_SAME_LINE : String.Empty;
                if (pathsSeparator != settings.PathsSeparator) {
                    settings.PathsSeparator = pathsSeparator;
                }
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
            RemovePipelinePluginBtn.Enabled = PluginsDataGrid.SelectedRows.Count == 1 && AreOnlyPipelinePluginsSelected();

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
                        if (Path.GetExtension(iconFile).ToLower() == ".ico") {
                            rowIconCell.ValueIsIcon = true;
                            rowIconCell.Value = new Icon(iconFile);
                        } else {
                            rowIconCell.Value = new Bitmap(iconFile);
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
            // Create user settings object and load settings in the form.
            settings = new UserSettings();
            LoadUserSettings();

            // Perform an initial button update.
            UpdatePluginButtons();
        }
        
        /// <summary>
        /// Called when the form finally closes. We need to do cleanup work here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            // Dispose of user settings object.
            if (settings != null) {
                settings.Dispose();
                settings = null;
            }
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
                using (new CursorChanger(this, Cursors.WaitCursor)) {
                    UserSettings.ExportUserSettings(ExportUserSettingsSaveDlg.FileName);
                }
                MessageBox.Show(this, String.Format(Resources.MainForm_Msg_UserSettingsExported,
                    ExportUserSettingsSaveDlg.FileName), Resources.MainForm_Msg_UserSettingsExportedMsgTitle,
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
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
        /// Called when the user clicks on the link to visit the PCC website.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void CodePlexSiteLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            // Link is the control's text.
            Process.Start(CodePlexSiteLinkLbl.Text);
        }
        
        /// <summary>
        /// Called when the user clicks on the link to see the license file.
        /// We need to locate the license file on disk and open it.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LicenseTxtLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Debug.Assert(licensePath != null);

            Process.Start(licensePath);
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
                bool setToolTip = false;
                if (!(rowPlugin is SeparatorPlugin)) {
                    string iconFile = rowPlugin.IconFile;
                    if (rowPlugin is PipelinePlugin || rowPlugin is DefaultPlugin) {
                        setToolTip = true;
                    }
                    LoadIconFileForCell(iconFile, rowIconCell);
                }

                // If the plugin's icon file can be edited, set tooltip text.
                // Otherwise, clear it and set the cell as read-only.
                if (setToolTip) {
                    rowIconCell.ToolTipText = Resources.MainForm_PluginsDataGrid_IconToolTipText;
                } else {
                    rowIconCell.ToolTipText = String.Empty;
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
        /// Called when the content of a cell is clicked in the plugins data grid.
        /// We need to handle clicks in icon cells to ask for an icon file.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PluginsDataGrid_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            // Immediately get current key modifiers to be able to determine the action to perform.
            Keys modifierKeys = Control.ModifierKeys & Keys.Modifiers;

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
                        rowPlugin.IconFile = String.Empty;
                        LoadIconFileForCell(String.Empty, rowIconCell);
                    } else if (modifierKeys == Keys.Control) {
                        // We must clear plugin's icon file.
                        rowPlugin.IconFile = null;
                        LoadIconFileForCell(null, rowIconCell);
                    } else if (modifierKeys == Keys.None) {
                        // Ask user to provide a new icon file for this plugin. If the plugin
                        // already has an icon file, start in the same folder as the previous icon.
                        if (!String.IsNullOrEmpty(rowPlugin.IconFile)) {
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
            if (PluginsDataGrid.SelectedRows.Count == 1) {
                // Display preview of plugin.
                PluginDisplayInfo info = (PluginDisplayInfo) PluginsDataGrid.SelectedRows[0].DataBoundItem;
                if (info.Plugin is SeparatorPlugin) {
                    // ...except for separators, which is pointless.
                    PreviewTxt.Clear();
                } else {
                    PreviewTxt.Text = info.Plugin.GetPreview(settings);
                }
            } else {
                // Clear content of preview textbox.
                PreviewTxt.Clear();
            }

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
                PluginsDataGrid.FirstDisplayedScrollingRowIndex = PluginsDataGrid.FirstDisplayedScrollingRowIndex + 1;
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
            using (PipelinePluginForm editForm = new PipelinePluginForm()) {
                PipelinePluginInfo pluginInfo = editForm.EditPlugin(this, settings, null);
                if (pluginInfo != null) {
                    // User pressed OK. Add the new plugin to the list view
                    // and select it.
                    Plugin plugin = pluginInfo.ToPlugin();
                    PluginDisplayInfo displayInfo = new PluginDisplayInfo(plugin);
                    displayInfo.ShowInSubmenu = true;
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
            using (PipelinePluginForm editForm = new PipelinePluginForm()) {
                try {
                    PipelinePluginInfo newPluginInfo = editForm.EditPlugin(this, settings, pluginInfo);
                    if (newPluginInfo != null) {
                        // Replace the existing plugin object in the data grid.
                        Debug.Assert(newPluginInfo.Id == pluginInfo.Id);
                        PluginDisplayInfo newDisplayInfo = new PluginDisplayInfo(newPluginInfo.ToPlugin());
                        newDisplayInfo.ShowInMainMenu = oldDisplayInfo.ShowInMainMenu;
                        newDisplayInfo.ShowInSubmenu = oldDisplayInfo.ShowInSubmenu;
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
        }

        /// <summary>
        /// Called when the Remove button is pressed. We remove
        /// selected pipeline plugins.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RemovePipelinePluginBtn_Click(object sender, EventArgs e)
        {
            Debug.Assert(PluginsDataGrid.SelectedRows.Count == 1);

            // First confirm since this is an "irreversible" action. We have a string
            // in resources for this message and it includes a format placeholder
            // for the plugin description.
            DataGridViewRow row = PluginsDataGrid.SelectedRows[0];
            int rowIndex = row.Index;
            PluginDisplayInfo displayInfo = (PluginDisplayInfo) row.DataBoundItem;
            PipelinePluginInfo pluginInfo = ((PipelinePlugin) displayInfo.Plugin).Info;
            DialogResult res = MessageBox.Show(this, String.Format(Resources.REMOVE_PIPELINE_PLUGIN_MESSAGE, pluginInfo.Description),
                Resources.REMOVE_PIPELINE_PLUGIN_TITLE, MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
            if (res == DialogResult.OK) {
                // Remove plugin from data grid.
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
                    collection.ToXML(fstream, String.Equals(Path.GetExtension(
                        ExportPipelinePluginsSaveDlg.FileName), LEGACY_PIPELINE_PLUGINS_EXT,
                        StringComparison.CurrentCultureIgnoreCase));
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
                        if (String.Equals(Path.GetExtension(ImportPipelinePluginsOpenDlg.FileName),
                            LEGACY_PIPELINE_PLUGINS_EXT, StringComparison.CurrentCultureIgnoreCase)) {

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
                                    new ImportedPipelinePluginOverwrites.OverwriteInfo(existingInfo, i);
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
                                ImportedPipelinePluginOverwrites.OverwriteInfo overwriteInfo;
                                if (pluginOverwrites.OverwriteInfos.TryGetValue(newPluginInfo, out overwriteInfo)) {
                                    PluginDisplayInfo displayInfo = pluginDisplayInfos[overwriteInfo.OldIndex];
                                    displayInfo.Plugin = newPlugin;
                                } else {
                                    PluginDisplayInfo newDisplayInfo = new PluginDisplayInfo(newPlugin);
                                    newDisplayInfo.ShowInSubmenu = true;
                                    pluginDisplayInfos.Add(newDisplayInfo);
                                    addedNewPlugins = true;
                                }
                            }

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
        /// Called when the form's location changes.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MainForm_LocationChanged(object sender, EventArgs e)
        {
            if (settings != null) {
                // Save location in settings right away; this is not saved at the end like other settings.
                settings.SettingsFormPosX = this.Location.X;
                settings.SettingsFormPosY = this.Location.Y;
            }
        }

        /// <summary>
        /// Called when the form's size changes.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            if (settings != null) {
                // Save size in settings right away; this is not saved at the end like other settings.
                settings.SettingsFormSizeWidth = this.Size.Width;
                settings.SettingsFormSizeHeight = this.Size.Height;
            }
        }
    }
}
