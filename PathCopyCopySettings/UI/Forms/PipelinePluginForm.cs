// PipelinePluginForm.cs
// (c) 2011-2021, Charles Lechasseur
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
using System.IO;
using System.Linq;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Utils;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form used to create or edit a pipeline plugin. To use, create the form
    /// and call the <see cref="EditPlugin"/> method.
    /// </summary>
    public partial class PipelinePluginForm : PositionPersistedForm
    {
        /// Initial plugin info used to populate our controls.
        /// Will be null if we're creating a new pipeline plugin.
        private PipelinePluginInfo oldPluginInfo;

        /// Pipeline of the initial plugin, if we have one.
        private Pipeline oldPipeline;

        /// Separator to use between multiple paths. Used to remember non-standard values (see Load).
        private string oldPathsSeparator;

        /// ID of the plugin we're editing. Will be generated
        /// if we're creating a new pipeline plugin.
        private Guid pluginId;

        /// Plugin info of the plugin being edited. Updated as
        /// the control changes. Will be returned if user chooses OK.
        private PipelinePluginInfo newPluginInfo;
        
        /// <summary>
        /// Constructor.
        /// </summary>
        public PipelinePluginForm()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Main method to use the form. Will show the form as a modal dialog,
        /// allowing the user to edit the given pipeline plugin. When the user
        /// is done, the method will return the new <see cref="PipelinePluginInfo"/>
        /// if the user accepted the changes.
        /// </summary>
        /// <param name="owner">Owner of this dialog. Can be <c>null</c>.</param>
        /// <param name="oldInfo">Info about a pipeline plugin. If set, we'll
        /// populate the form with the plugin's values to allow the user to
        /// edit the plugin.</param>
        /// <param name="switchToExpert">Upon exit, will indicate whether user
        /// chose to switch to Expert Mode.</param>
        /// <returns>Info about the new plugin that user edited. Will be
        /// <c>null</c> if user cancels editing.</returns>
        public PipelinePluginInfo EditPlugin(IWin32Window owner, PipelinePluginInfo oldInfo,
            out bool switchToExpert)
        {
            // Save old info so that the OnLoad event handler can use it.
            oldPluginInfo = oldInfo;

            // If a plugin info was specified, decode its pipeline immediately.
            // We want pipeline exceptions to propagate out *before* we show the form.
            if (oldPluginInfo != null) {
                oldPipeline = PipelineDecoder.DecodePipeline(oldPluginInfo.EncodedElements);
            }

            // Save plugin ID, or generate a new one if this is a new plugin.
            if (oldPluginInfo != null) {
                pluginId = oldPluginInfo.Id;
            } else {
                pluginId = Guid.NewGuid();
            }

            // Show the form and check result.
            DialogResult dialogRes = ShowDialog(owner);

            // If user saved, return the new info.
            Debug.Assert(dialogRes == DialogResult.Cancel || newPluginInfo != null);
            switchToExpert = dialogRes == DialogResult.Retry;
            return dialogRes != DialogResult.Cancel ? newPluginInfo : null;
        }
        
        /// <summary>
        /// Called when the form is loaded. We use this opportunity to populate
        /// the controls with info about a plugin we're editing, if we have one.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_Load(object sender, EventArgs e)
        {
            // First load list of plugins in default order for the base plugin.
            // Note: we want *temp* pipeline plugins in order to use those saved by the MainForm,
            // in order to get the most recent snapshot of pipeline plugins.
            List<Plugin> pluginsInDefaultOrder = PluginsRegistry.GetPluginsInDefaultOrder(
                Settings, PipelinePluginsOptions.FetchTempPipelinePlugins);

            // Create sorted dictionary of all plugins from the list above, to be able to perform lookups.
            SortedDictionary<Guid, Plugin> dictionaryOfAllPlugins = new SortedDictionary<Guid, Plugin>();
            foreach (Plugin plugin in pluginsInDefaultOrder) {
                if (!dictionaryOfAllPlugins.ContainsKey(plugin.Id)) {
                    dictionaryOfAllPlugins.Add(plugin.Id, plugin);
                }
            }

            // Use UI display order from settings to order the plugins.
            // (See MainForm.LoadSettings for some more details on this process)
            List<Guid> uiDisplayOrder = Settings.UIDisplayOrder;
            if (uiDisplayOrder == null) {
                // No display order, just use all plugins in default order
                uiDisplayOrder = pluginsInDefaultOrder.Select(plugin => plugin.Id).ToList();
            }
            SortedSet<Guid> uiDisplayOrderAsSet = new SortedSet<Guid>(uiDisplayOrder);
            List<Plugin> plugins = PluginsRegistry.OrderPluginsToDisplay(dictionaryOfAllPlugins,
                uiDisplayOrder, uiDisplayOrderAsSet, pluginsInDefaultOrder);

            // Add all plugins to the list box.
            BasePluginLst.Items.AddRange(plugins.ToArray());

            // Check if we have a plugin info to load.
            Guid? basePluginId = null;
            if (oldPluginInfo != null) {
                Debug.Assert(oldPipeline != null);

                // Populate our controls.
                NameTxt.Text = oldPluginInfo.Description;
                PipelineElement element = oldPipeline.Elements.Find(el => el is PipelineElementWithPluginID);
                if (element != null) {
                    basePluginId = ((PipelineElementWithPluginID) element).PluginID;
                }
                if (oldPipeline.Elements.Find(el => el is OptionalQuotesPipelineElement) != null) {
                    QuotesChk.Checked = true;
                    OptionalQuotesChk.Checked = true;
                } else if (oldPipeline.Elements.Find(el => el is QuotesPipelineElement) != null) {
                    QuotesChk.Checked = true;
                }
                EmailLinksChk.Checked = oldPipeline.Elements.Find(el => el is EmailLinksPipelineElement) != null;
                if (oldPipeline.Elements.Find(el => el is EncodeURICharsPipelineElement) != null) {
                    EncodeURIWhitespaceChk.Checked = true;
                    EncodeURICharsChk.Checked = true;
                } else if (oldPipeline.Elements.Find(el => el is EncodeURIWhitespacePipelineElement) != null) {
                    EncodeURIWhitespaceChk.Checked = true;
                }
                RemoveExtChk.Checked = oldPipeline.Elements.Find(el => el is RemoveExtPipelineElement) != null;
                if (oldPipeline.Elements.Find(el => el is BackToForwardSlashesPipelineElement) != null) {
                    BackToForwardSlashesRadio.Checked = true;
                } else if (oldPipeline.Elements.Find(el => el is ForwardToBackslashesPipelineElement) != null) {
                    ForwardToBackslashesRadio.Checked = true;
                } else {
                    Debug.Assert(NoSlashesChangeRadio.Checked);
                }
                element = oldPipeline.Elements.Find(el => el is RegexPipelineElement);
                if (element != null) {
                    UseRegexChk.Checked = true;
                    IgnoreCaseChk.Enabled = true;
                    TestRegexBtn.Enabled = true;
                    RegexPipelineElement regexElement = (RegexPipelineElement) element;
                    FindTxt.Text = regexElement.Regex;
                    ReplaceTxt.Text = regexElement.Format;
                    IgnoreCaseChk.Checked = regexElement.IgnoreCase;
                } else {
                    Debug.Assert(!IgnoreCaseChk.Enabled);
                    Debug.Assert(!TestRegexBtn.Enabled);
                    element = oldPipeline.Elements.Find(el => el is FindReplacePipelineElement);
                    if (element != null) {
                        FindTxt.Text = ((FindReplacePipelineElement) element).OldValue;
                        ReplaceTxt.Text = ((FindReplacePipelineElement) element).NewValue;
                    }
                }
                if (oldPipeline.Elements.Find(el => el is UnexpandEnvironmentStringsPipelineElement) != null) {
                    UnexpandEnvStringsChk.Checked = true;
                }

                // "Copy on same line" is a little special since it could be any string.
                element = oldPipeline.Elements.Find(el => el is PathsSeparatorPipelineElement);
                oldPathsSeparator = (element as PathsSeparatorPipelineElement)?.PathsSeparator;
                if (oldPathsSeparator == PipelinePluginEditor.PATHS_SEPARATOR_ON_SAME_LINE) {
                    CopyOnSameLineChk.Checked = true;
                } else if (!string.IsNullOrEmpty(oldPathsSeparator)) {
                    CopyOnSameLineChk.Enabled = false;
                }

                RecursiveCopyChk.Checked = oldPipeline.Elements.Find(el => el is RecursiveCopyPipelineElement) != null;
                element = oldPipeline.Elements.Find(el => el is ExecutablePipelineElement);
                if (element != null) {
                    LaunchExecutableChk.Checked = true;
                    ExecutableTxt.Text = ((ExecutablePipelineElement) element).Executable;
                } else {
                    element = oldPipeline.Elements.Find(el => el is ExecutableWithFilelistPipelineElement);
                    if (element != null) {
                        LaunchExecutableChk.Checked = true;
                        WithFilelistChk.Checked = true;
                        ExecutableTxt.Text = ((ExecutableWithFilelistPipelineElement) element).Executable;
                    } else {
                        Debug.Assert(!ExecutableLbl.Enabled);
                        Debug.Assert(!ExecutableTxt.Enabled);
                        Debug.Assert(!BrowserForExecutableBtn.Enabled);
                    }
                }
            }

            // Validate base plugin ID if found.
            if (basePluginId != null) {
                if (plugins.Find(pl => pl.Id == basePluginId) == null) {
                    // Invalid, clear it so that we'll fall back
                    // on the default below.
                    basePluginId = null;
                }
            }

            // If we didn't find a base plugin ID yet, use the "Long Path" plugin ID.
            if (basePluginId == null) {
                basePluginId = new Guid(Resources.LONG_PATH_PLUGIN_ID);
            }

            // Scan list of plugins to select the base plugin.
            for (int i = 0; i < BasePluginLst.Items.Count; ++i) {
                if (!(BasePluginLst.Items[i] is SeparatorPlugin) &&
                    ((Plugin) BasePluginLst.Items[i]).Id == basePluginId) {

                    // This is the default plugin, select it and break.
                    BasePluginLst.SelectedIndex = i;
                    break;
                }
            }
            Debug.Assert(BasePluginLst.SelectedIndex != -1);

            // Immediately update plugin info so that preview box is initially filled.
            UpdatePluginInfo();
        }

        /// <summary>
        /// Updates <see cref="newPluginInfo"/> with the contents of the
        /// form's controls. Also updates the preview box.
        /// </summary>
        private void UpdatePluginInfo()
        {
            // Create a pipeline based on form controls.
            Pipeline pipeline = new Pipeline();
            if (LaunchExecutableChk.Checked) {
                if (WithFilelistChk.Checked) {
                    pipeline.Elements.Add(new ExecutableWithFilelistPipelineElement(ExecutableTxt.Text));
                } else {
                    pipeline.Elements.Add(new ExecutablePipelineElement(ExecutableTxt.Text));
                }
            }
            if (RecursiveCopyChk.Checked) {
                pipeline.Elements.Add(new RecursiveCopyPipelineElement());
            }
            if (CopyOnSameLineChk.Enabled) {
                if (CopyOnSameLineChk.Checked) {
                    pipeline.Elements.Add(new PathsSeparatorPipelineElement(PipelinePluginEditor.PATHS_SEPARATOR_ON_SAME_LINE));
                }
            } else {
                // Copy non-standard value we had earlier
                Debug.Assert(!string.IsNullOrEmpty(oldPathsSeparator));
                pipeline.Elements.Add(new PathsSeparatorPipelineElement(oldPathsSeparator));
            }
            if (BasePluginLst.SelectedIndex != -1) {
                // If user selected a pipeline plugin, we need to use a different
                // kind of pipeline element.
                Plugin selectedPlugin = (Plugin) BasePluginLst.SelectedItem;
                if (selectedPlugin is PipelinePlugin) {
                    pipeline.Elements.Add(new ApplyPipelinePluginPipelineElement(selectedPlugin.Id));
                } else {
                    pipeline.Elements.Add(new ApplyPluginPipelineElement(selectedPlugin.Id));
                }
            }
            if (UnexpandEnvStringsChk.Checked) {
                pipeline.Elements.Add(new UnexpandEnvironmentStringsPipelineElement());
            }
            if (FindTxt.Text.Length > 0) {
                if (UseRegexChk.Checked) {
                    pipeline.Elements.Add(new RegexPipelineElement(FindTxt.Text, ReplaceTxt.Text, IgnoreCaseChk.Checked));
                } else {
                    pipeline.Elements.Add(new FindReplacePipelineElement(FindTxt.Text, ReplaceTxt.Text));
                }
            }
            if (BackToForwardSlashesRadio.Checked) {
                pipeline.Elements.Add(new BackToForwardSlashesPipelineElement());
            } else if (ForwardToBackslashesRadio.Checked) {
                pipeline.Elements.Add(new ForwardToBackslashesPipelineElement());
            }
            if (RemoveExtChk.Checked) {
                pipeline.Elements.Add(new RemoveExtPipelineElement());
            }
            if (OptionalQuotesChk.Checked) {
                pipeline.Elements.Add(new OptionalQuotesPipelineElement());
            } else if (QuotesChk.Checked) {
                pipeline.Elements.Add(new QuotesPipelineElement());
            }
            if (EmailLinksChk.Checked) {
                pipeline.Elements.Add(new EmailLinksPipelineElement());
            }
            if (EncodeURIWhitespaceChk.Checked) {
                if (EncodeURICharsChk.Enabled && EncodeURICharsChk.Checked) {
                    pipeline.Elements.Add(new EncodeURICharsPipelineElement());
                } else {
                    pipeline.Elements.Add(new EncodeURIWhitespacePipelineElement());
                }
            }

            // Create new plugin info and save encoded elements.
            PipelinePluginInfo pluginInfo = new PipelinePluginInfo {
                Id = pluginId,
                Description = NameTxt.Text,
                EncodedElements = pipeline.Encode(),
                RequiredVersion = pipeline.RequiredVersion,
                EditMode = PipelinePluginEditMode.Simple,
            };
            Debug.Assert(!pluginInfo.Global);

            // Save plugin info in newPluginInfo and update preview.
            newPluginInfo = pluginInfo;
            PreviewCtrl.Plugin = newPluginInfo.ToPlugin();
        }
        
        /// <summary>
        /// Called when the form is about to close. If user pressed OK, we save
        /// the content of the plugin here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // If user chose to press OK or switch to Expert Mode, save plugin info.
            if (DialogResult == DialogResult.OK || DialogResult == DialogResult.Retry) {
                // Make sure user has entered a name (unless we're switching to Expert Mode).
                if (!string.IsNullOrEmpty(NameTxt.Text) || DialogResult == DialogResult.Retry) {
                    // Update plugin info in case it's out-of-date, so that EditPlugin can return it.
                    UpdatePluginInfo();
                } else {
                    // Warn user that we need a non-empty name.
                    MessageBox.Show(Resources.PipelinePluginForm_EmptyName,
                        Resources.PipelinePluginForm_MsgTitle, MessageBoxButtons.OK,
                        MessageBoxIcon.Warning);
                    NameTxt.Focus();
                    e.Cancel = true;
                }
            }
        }

        /// <summary>
        /// Called by controls that modify the plugin info, so that the preview
        /// is updated when things change.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_UpdatePreview(object sender, EventArgs e)
        {
            UpdatePluginInfo();
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Add quotes"
        /// checkbox. We need to enable to disable the "...only if
        /// there's space" checkbox when this occurs since it's
        /// conditional to the former.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void QuotesChk_CheckedChanged(object sender, EventArgs e)
        {
            OptionalQuotesChk.Enabled = QuotesChk.Checked;
            UpdatePluginInfo();
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
            UpdatePluginInfo();
        }
        
        /// <summary>
        /// Called when the user checks or unchecks the "Use regular expressions"
        /// checkbox. We need to enable or disable the "ignore case" checkbox
        /// when this occurs, as this flag is only used with regular expressions.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void UseRegexChk_CheckedChanged(object sender, EventArgs e)
        {
            IgnoreCaseChk.Enabled = UseRegexChk.Checked;
            TestRegexBtn.Enabled = UseRegexChk.Checked;
            UpdatePluginInfo();
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Launch executable..."
        /// checkbox. We need to enable or disable other controls when this occurs.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LaunchExecutableChk_CheckedChanged(object sender, EventArgs e)
        {
            WithFilelistChk.Enabled = LaunchExecutableChk.Checked;
            ExecutableLbl.Enabled = LaunchExecutableChk.Checked;
            ExecutableTxt.Enabled = LaunchExecutableChk.Checked;
            BrowserForExecutableBtn.Enabled = LaunchExecutableChk.Checked;
        }
        
        /// <summary>
        /// Called when the user presses the button to test a regular expression.
        /// We will show a dialog allowing the user to test the currently-provided
        /// expression and optionally change it.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void TestRegexBtn_Click(object sender, EventArgs e)
        {
            // Grab content of controls so that the tester form
            // can optionally modify them.
            string regex = FindTxt.Text;
            string format = ReplaceTxt.Text;
            bool ignoreCase = IgnoreCaseChk.Checked;

            // Show form and ask user to test. See what happens.
            using (RegexTesterForm testerForm = new RegexTesterForm()) {
                DialogResult res = testerForm.TestRegex(ref regex, ref format, ref ignoreCase, this);
                if (res == DialogResult.OK) {
                    // User pressed OK which means we have updated values.
                    FindTxt.Text = regex;
                    ReplaceTxt.Text = format;
                    IgnoreCaseChk.Checked = ignoreCase;
                }
            }
        }

        /// <summary>
        /// Called when the user presses the button to browse for an executable.
        /// We will show an open dialog allowing user to pick one.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void BrowserForExecutableBtn_Click(object sender, EventArgs e)
        {
            // Show browse box, using the current filename as hint.
            try {
                ChooseExecutableOpenDlg.InitialDirectory = Path.GetDirectoryName(ExecutableTxt.Text);
                ChooseExecutableOpenDlg.FileName = Path.GetFileName(ExecutableTxt.Text);
            } catch (ArgumentException) {
                // Bad format or something, simply don't use.
            }
            if (ChooseExecutableOpenDlg.ShowDialog(this) == DialogResult.OK) {
                // User chose a new executable, copy its path back in our control.
                ExecutableTxt.Text = ChooseExecutableOpenDlg.FileName;
            }
        }

        /// <summary>
        /// Called when the user selects a new base plugin.
        /// We need to make sure it's not a separator and disable the OK button in that case.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void BasePluginLst_SelectedIndexChanged(object sender, EventArgs e)
        {
            OKBtn.Enabled = BasePluginLst.SelectedIndex >= 0 &&
                !(((Plugin) BasePluginLst.SelectedItem) is SeparatorPlugin);
            UpdatePluginInfo();
        }

        /// <summary>
        /// Called when the user presses the Help button in the form's caption bar.
        /// We navigate to the wiki to show help in such a case.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_HelpButtonClicked(object sender, CancelEventArgs e)
        {
            // Open wiki link to Custom Commands page, then cancel the event to avoid
            // displaying a help mouse pointer like the default behavior.
            Process.Start(Resources.WikiLink_CustomCommands);
            e.Cancel = true;
        }
    }
}
