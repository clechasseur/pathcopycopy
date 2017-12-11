// PipelinePluginForm.cs
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
using System.IO;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form used to create or edit a pipeline plugin. To use, create the form
    /// and call the <see cref="EditPlugin"/> method.
    /// </summary>
    public partial class PipelinePluginForm : Form
    {
        /// Paths separator that copies multiple paths on the same line.
        private const string PATHS_SEPARATOR_ON_SAME_LINE = " ";

        /// Object to access user settings.
        private UserSettings settings;

        /// Plugin info for the plugin we're editing.
        private PipelinePluginInfo pluginInfo;

        /// Pipeline of the plugin info, if we have one.
        private Pipeline pipeline;

        /// Separator to use between multiple paths. Used to remember non-standard values (see Load).
        private string pathsSeparator;
        
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
        /// <param name="settings">Object to access user settings. If <c>null</c>,
        /// a new <see cref="UserSettings"/> object will be created.</param>
        /// <param name="oldInfo">Info about a pipeline plugin. If set, we'll
        /// populate the form with the plugin's values to allow the user to
        /// edit the plugin.</param>
        /// <returns>Info about the new plugin that user edited. Will be
        /// <c>null</c> if user cancels editing.</returns>
        public PipelinePluginInfo EditPlugin(IWin32Window owner, 
            UserSettings settings, PipelinePluginInfo oldInfo)
        {
            // Save settings object or create one if we didn't get one.
            this.settings = settings ?? new UserSettings();

            // Save old info so that the OnLoad event handler can use it.
            pluginInfo = oldInfo;

            // If a plugin info was specified, decode its pipeline immediately.
            // We want pipeline exceptions to propagate out *before* we show the form.
            if (pluginInfo != null) {
                pipeline = PipelineDecoder.DecodePipeline(pluginInfo.EncodedElements);
            }

            // Show the form and check result.
            DialogResult dialogRes = ShowDialog(owner);

            // If user saved, return the new info.
            Debug.Assert(dialogRes != DialogResult.OK || pluginInfo != null);
            return dialogRes == DialogResult.OK ? pluginInfo : null;
        }
        
        /// <summary>
        /// Called when the form is loaded. We use this opportunity to populate
        /// the controls with info about a plugin we're editing, if we have one.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_Load(object sender, EventArgs e)
        {
            // First load list of plugins to display in the listbox for the base
            // plugin. We only load default and COM plugins for this since we
            // don't want a pipeline plugin to be based off another (for now at least).
            List<Plugin> plugins = PluginsRegistry.GetPluginsInDefaultOrder(settings, false);

            // Add all plugins to the list box.
            BasePluginLst.Items.AddRange(plugins.ToArray());

            // Check if we have a plugin info to load.
            Guid? basePluginId = null;
            if (pluginInfo != null) {
                Debug.Assert(pipeline != null);

                // Populate our controls.
                NameTxt.Text = pluginInfo.Description;
                PipelineElement element = pipeline.Elements.Find(el => el is ApplyPluginPipelineElement);
                if (element != null) {
                    basePluginId = ((ApplyPluginPipelineElement) element).PluginID;
                }
                QuotesChk.Checked = pipeline.Elements.Find(el => el is QuotesPipelineElement) != null;
                EmailLinksChk.Checked = pipeline.Elements.Find(el => el is EmailLinksPipelineElement) != null;
                if (pipeline.Elements.Find(el => el is EncodeURICharsPipelineElement) != null) {
                    EncodeURIWhitespaceChk.Checked = true;
                    EncodeURICharsChk.Checked = true;
                } else if (pipeline.Elements.Find(el => el is EncodeURIWhitespacePipelineElement) != null) {
                    EncodeURIWhitespaceChk.Checked = true;
                }
                if (pipeline.Elements.Find(el => el is BackToForwardSlashesPipelineElement) != null) {
                    BackToForwardSlashesRadio.Checked = true;
                } else if (pipeline.Elements.Find(el => el is ForwardToBackslashesPipelineElement) != null) {
                    ForwardToBackslashesRadio.Checked = true;
                } else {
                    Debug.Assert(NoSlashesChangeRadio.Checked);
                }
                element = pipeline.Elements.Find(el => el is RegexPipelineElement);
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
                    element = pipeline.Elements.Find(el => el is FindReplacePipelineElement);
                    if (element != null) {
                        FindTxt.Text = ((FindReplacePipelineElement) element).OldValue;
                        ReplaceTxt.Text = ((FindReplacePipelineElement) element).NewValue;
                    }
                }

                // "Copy on same line" is a little special since it could be any string.
                element = pipeline.Elements.Find(el => el is PathsSeparatorPipelineElement);
                pathsSeparator = element == null ? null : ((PathsSeparatorPipelineElement) element).PathsSeparator;
                if (pathsSeparator == PATHS_SEPARATOR_ON_SAME_LINE) {
                    CopyOnSameLineChk.Checked = true;
                } else if (!String.IsNullOrEmpty(pathsSeparator)) {
                    CopyOnSameLineChk.Enabled = false;
                }

                element = pipeline.Elements.Find(el => el is ExecutablePipelineElement);
                if (element != null) {
                    LaunchExecutableChk.Checked = true;
                    ExecutableTxt.Text = ((ExecutablePipelineElement) element).Executable;
                } else {
                    Debug.Assert(!ExecutableLbl.Enabled);
                    Debug.Assert(!ExecutableTxt.Enabled);
                    Debug.Assert(!BrowserForExecutableBtn.Enabled);
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
        }
        
        /// <summary>
        /// Called when the form is about to close. If user pressed OK, we save
        /// the content of the plugin here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // If user chose to press OK, save plugin info.
            if (this.DialogResult == DialogResult.OK) {
                // Make sure user has entered a name.
                if (!String.IsNullOrEmpty(NameTxt.Text)) {
                    // Create a pipeline based on form controls.
                    if (pipeline == null) {
                        pipeline = new Pipeline();
                    } else {
                        pipeline.Clear();
                    }
                    if (LaunchExecutableChk.Checked) {
                        pipeline.Elements.Add(new ExecutablePipelineElement(ExecutableTxt.Text));
                    }
                    if (CopyOnSameLineChk.Enabled) {
                        if (CopyOnSameLineChk.Checked) {
                            pipeline.Elements.Add(new PathsSeparatorPipelineElement(PATHS_SEPARATOR_ON_SAME_LINE));
                        }
                    } else {
                        // Copy non-standard value we had earlier
                        Debug.Assert(!String.IsNullOrEmpty(pathsSeparator));
                        pipeline.Elements.Add(new PathsSeparatorPipelineElement(pathsSeparator));
                    }
                    if (BasePluginLst.SelectedIndex != -1) {
                        pipeline.Elements.Add(new ApplyPluginPipelineElement(
                            ((Plugin) BasePluginLst.SelectedItem).Id));
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
                    if (QuotesChk.Checked) {
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

                    // Create plugin info if we don't already have one.
                    if (pluginInfo == null) {
                        pluginInfo = new PipelinePluginInfo();
                        pluginInfo.Id = Guid.NewGuid();
                    }

                    // Save info in plugin info wrapper.
                    pluginInfo.Description = NameTxt.Text;
                    pluginInfo.EncodedElements = pipeline.Encode();
                    pluginInfo.RequiredVersion = pipeline.RequiredVersion;
                    Debug.Assert(!pluginInfo.Global);
                } else {
                    // Warn user that we need a non-empty name.
                    MessageBox.Show(Resources.PipelinePluginForm_EmptyName, Resources.PipelinePluginForm_MsgTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    NameTxt.Focus();
                    e.Cancel = true;
                }
            }
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
        }

        /// <summary>
        /// Called when the user checks or unchecks the "Launch executable..."
        /// checkbox. We need to enable or disable the executable path textbox
        /// and the browse button when this occurs.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LaunchExecutableChk_CheckedChanged(object sender, EventArgs e)
        {
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
            } catch {
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
        }
    }
}
