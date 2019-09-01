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
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
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
        /// Object to access user settings.
        private UserSettings settings;

        /// Plugin info for the plugin we're editing.
        private PipelinePluginInfo pluginInfo;

        /// Pipeline of the plugin info, if we have one.
        private Pipeline pipeline;

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
        /// <param name="settings">Object to access user settings. If <c>null</c>,
        /// a new <see cref="UserSettings"/> object will be created.</param>
        /// <param name="oldInfo">Info about a pipeline plugin. If set, we'll
        /// populate the form with the plugin's values to allow the user to
        /// edit the plugin.</param>
        /// <param name="switchToSimple">Upon exit, will indicate whether user
        /// chose to switch to Simple Mode.</param>
        /// <returns>Info about the new plugin that user edited. Will be
        /// <c>null</c> if user cancels editing.</returns>
        public PipelinePluginInfo EditPlugin(IWin32Window owner, UserSettings settings,
            PipelinePluginInfo oldInfo, out bool switchToSimple)
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
            // TODO
        }

        /// <summary>
        /// Called when the form is about to close. If user pressed OK, we save
        /// the content of plugin here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void AdvancedPipelinePluginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // If user chose to press OK or switch to Simple Mode, save plugin info.
            if (this.DialogResult == DialogResult.OK || this.DialogResult == DialogResult.Retry) {
                // Create a pipeline based on form controls.
                if (pipeline == null) {
                    pipeline = new Pipeline();
                } else {
                    //pipeline.Clear(); // TODO clear existing elements
                }
                // TODO

                // Create plugin info if we don't already have one.
                if (pluginInfo == null) {
                    pluginInfo = new PipelinePluginInfo();
                    pluginInfo.Id = Guid.NewGuid();
                }

                // Save info in plugin info wrapper.
                //pluginInfo.Description = NameTxt.Text; // TODO save description
                pluginInfo.EncodedElements = pipeline.Encode();
                pluginInfo.RequiredVersion = pipeline.RequiredVersion;
                Debug.Assert(!pluginInfo.Global);

                // If pipeline is too complex, user might lose customization by switching
                // to simple mode. Warn in this case.
                if (this.DialogResult == DialogResult.Retry && !PipelinePluginEditor.IsPipelineSimple(pipeline)) {
                    DialogResult subDialogRes = MessageBox.Show(Resources.PipelinePluginForm_PipelineTooComplexForSimpleMode,
                        Resources.PipelinePluginForm_MsgTitle, MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
                    if (subDialogRes == DialogResult.No) {
                        e.Cancel = true;
                    }
                }
            }
        }
    }
}
