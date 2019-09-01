// AdvancedPipelinePluginForm.cs
// (c) 2019, Charles Lechasseur
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
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Utils;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form used to create or edit a pipeline plugin in Expert Mode. To use,
    /// create the form and call the <see cref="EditPlugin"/> method.
    /// </summary>
    public partial class AdvancedPipelinePluginForm : Form
    {
        /// Plugin info for the plugin we're editing.
        private PipelinePluginInfo pluginInfo;

        /// Pipeline of the plugin info, if we have one.
        private Pipeline pipeline;

        /// User control to edit currently-selected pipeline element.
        private UserControl currentUserControl;

        /// <summary>
        /// Constructor.
        /// </summary>
        public AdvancedPipelinePluginForm()
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
        /// <param name="switchToSimple">Upon exit, will indicate whether user
        /// chose to switch to Simple Mode.</param>
        /// <returns>Info about the new plugin that user edited. Will be
        /// <c>null</c> if user cancels editing.</returns>
        public PipelinePluginInfo EditPlugin(IWin32Window owner, PipelinePluginInfo oldInfo,
            out bool switchToSimple)
        {
            // Save old info so that the OnLoad event handler can use it.
            pluginInfo = oldInfo;

            // If a plugin info was specified, decode its pipeline immediately.
            // We want pipeline exceptions to propagate out *before* we show the form.
            if (pluginInfo != null) {
                pipeline = PipelineDecoder.DecodePipeline(pluginInfo.EncodedElements);
            } else {
                // No plugin info to decode, create new empty pipeline.
                pipeline = new Pipeline();
            }

            // Show the form and check result.
            DialogResult dialogRes = ShowDialog(owner);

            // If user saved, return the new info.
            Debug.Assert(dialogRes == DialogResult.Cancel || pluginInfo != null);
            switchToSimple = dialogRes == DialogResult.Retry;
            return dialogRes != DialogResult.Cancel ? pluginInfo : null;
        }

        /// <summary>
        /// Called when the form is loaded. We use this opportunity to load the
        /// controls necessary to edit the pipeline plugin.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void AdvancedPipelinePluginForm_Load(object sender, EventArgs e)
        {
            Debug.Assert(pipeline != null);

            // Populate our controls.
            NameTxt.Text = pluginInfo?.Description ?? String.Empty;
            ElementsLst.DataSource = pipeline.Elements;

            // Update initial controls.
            UpdateControls();
        }

        /// <summary>
        /// Called when the form is about to close. If user pressed OK, we save
        /// the content of plugin here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void AdvancedPipelinePluginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Debug.Assert(pipeline != null);

            // If user chose to press OK or switch to Simple Mode, save plugin info.
            if (this.DialogResult == DialogResult.OK || this.DialogResult == DialogResult.Retry) {
                // Make sure user has entered a name (unless we're switching to Simple Mode).
                if (!String.IsNullOrEmpty(NameTxt.Text) || this.DialogResult == DialogResult.Retry) {
                    // If pipeline is too complex, user might lose customization by switching
                    // to simple mode. Warn in this case.
                    if (this.DialogResult == DialogResult.Retry && !PipelinePluginEditor.IsPipelineSimple(pipeline)) {
                        DialogResult subDialogRes = MessageBox.Show(Resources.PipelinePluginForm_PipelineTooComplexForSimpleMode,
                            Resources.PipelinePluginForm_MsgTitle, MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
                        if (subDialogRes == DialogResult.No) {
                            e.Cancel = true;
                        }
                    }
                    if (!e.Cancel) {
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
                    }
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
        /// Updates controls that are dependent on the list selection.
        /// </summary>
        private void UpdateControls()
        {
            DeleteElementBtn.Enabled = ElementsLst.SelectedIndex >= 0;
            MoveElementUpBtn.Enabled = ElementsLst.SelectedIndex > 0;
            MoveElementDownBtn.Enabled = ElementsLst.SelectedIndex >= 0 &&
                ElementsLst.SelectedIndex < (pipeline.Elements.Count - 1);
        }

        /// <summary>
        /// Called when a new pipeline element is selected in the list.
        /// We must show the appropriate user control to enable editing.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ElementsLst_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (currentUserControl != null) {
                // We currently display a user control for another element, remove it.
                Controls.Remove(currentUserControl);
                currentUserControl.Dispose();
                currentUserControl = null;
            }

            // Show or hide label instructing user to select an element.
            SelectElementLbl.Visible = ElementsLst.SelectedIndex < 0;

            // If user selected an element, display its control.
            if (ElementsLst.SelectedIndex >= 0) {
                currentUserControl = pipeline.Elements[ElementsLst.SelectedIndex].GetEditingControl();
                Controls.Add(currentUserControl);
                currentUserControl.Location = SelectElementLbl.Location;
                currentUserControl.Size = new Size(this.Size.Width - currentUserControl.Location.X - 23, ElementsLst.Size.Height);
                currentUserControl.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Bottom | AnchorStyles.Right;
            }

            // Update selection-dependent controls.
            UpdateControls();
        }
    }
}
