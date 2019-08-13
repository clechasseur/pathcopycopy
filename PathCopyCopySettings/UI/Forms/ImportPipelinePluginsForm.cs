// ImportPipelinePluginsForm.cs
// (c) 2012-2019, Charles Lechasseur
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

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form that can be used to select pipeline plugins to import. To use,
    /// create an instance and call <see cref="SelectPlugins"/>.
    /// </summary>
    public partial class ImportPipelinePluginsForm : Form
    {
        /// Caches a brush to paint regular list items in our plugins list box.
        private Brush listItemRegularBrush;

        /// Caches a brush to paint disabled list items in our plugins list box.
        private Brush listItemDisabledBrush;

        /// Caches a brush to paint selected list items in our plugins list box.
        private Brush listItemSelectedBrush;

        /// Flag to prevent stack overflows when changing selection.
        private bool reselecting = false;

        /// Flag to tell whether we already warned user about overwriting global plugins.
        private static bool warnedAboutGlobalOverwrites = false;

        /// Flag to tell whether we already warned user about importing incompatible plugins.
        private static bool warnedAboutIncompatiblePlugins = false;
        
        /// <summary>
        /// Constructor.
        /// </summary>
        public ImportPipelinePluginsForm()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Shows the form as a modal dialog, asking the user which plugins among
        /// the given list he/she wants to import.
        /// </summary>
        /// <param name="owner">Owner window of the dialog. Can be <c>null</c>.</param>
        /// <param name="pluginCollection">List of plugins available for importing.
        /// Upon exit, if the method returns <c>true</c>, the list will be modified
        /// to only contain those plugins selected by the user.</param>
        /// <param name="pluginOverwrites">Container of information about plugins
        /// overwriting existing ones. Upon exit, if the method returns <c>true</c>,
        /// the list will be modified to only contain plugins that are also in
        /// <paramref name="pluginCollection"/>.</param>
        /// <returns><c>true</c> if the user pressed OK to import selected
        /// plugins.</returns>
        public bool SelectPlugins(IWin32Window owner, ref PipelinePluginCollection pluginCollection,
            ref ImportedPipelinePluginOverwrites pluginOverwrites)
        {
            Debug.Assert(pluginCollection != null);
            Debug.Assert(pluginOverwrites != null);

            PipelinePluginsLst.BeginUpdate();
            try {
                // Populate the list with given plugins.
                foreach (PipelinePluginInfo pluginInfo in pluginCollection.Plugins) {
                    PipelinePluginsLst.Items.Add(new PluginToImportDisplayInfo {
                        PluginInfo = pluginInfo,
                        Importable = true,
                    });
                }

                // Disable any plugin that would overwrite a global one set by the administrator.
                for (int i = 0; i < PipelinePluginsLst.Items.Count; ++i) {
                    PluginToImportDisplayInfo newPlugin = (PluginToImportDisplayInfo) PipelinePluginsLst.Items[i];
                    ImportedPipelinePluginOverwrites.OverwriteInfo overwriteInfo;
                    if (pluginOverwrites.OverwriteInfos.TryGetValue(newPlugin.PluginInfo, out overwriteInfo) && overwriteInfo.OldInfo.Global) {
                        newPlugin.Importable = false;
                    }
                }

                // Default is to want to select them all importable, compatible plugins.
                reselecting = true;
                try {
                    for (int i = 0; i < PipelinePluginsLst.Items.Count; ++i) {
                        PluginToImportDisplayInfo newPlugin = (PluginToImportDisplayInfo) PipelinePluginsLst.Items[i];
                        if (newPlugin.Importable && newPlugin.Compatible) {
                            PipelinePluginsLst.SetSelected(i, true);
                        }
                    }
                } finally {
                    reselecting = false;
                }

                // Update buttons to match the initial selection state.
                UpdateButtons();
            } finally {
                PipelinePluginsLst.EndUpdate();
            }

            // Show dialog to allow user to select plugins.
            bool res = ShowDialog(owner) == DialogResult.OK;
            if (res) {
                // Rebuild list of plugins in the collection.
                pluginCollection.Plugins.Clear();
                foreach (var item in PipelinePluginsLst.SelectedItems) {
                    pluginCollection.Plugins.Add(((PluginToImportDisplayInfo) item).PluginInfo);
                }

                // Also rebuild the list of overwrites to only include those we included in the collection.
                ImportedPipelinePluginOverwrites newOverwrites = new ImportedPipelinePluginOverwrites();
                foreach (var pluginInfo in pluginCollection.Plugins) {
                    ImportedPipelinePluginOverwrites.OverwriteInfo overwriteInfo;
                    if (pluginOverwrites.OverwriteInfos.TryGetValue(pluginInfo, out overwriteInfo)) {
                        newOverwrites.OverwriteInfos.Add(pluginInfo, overwriteInfo);
                    }
                }
                pluginOverwrites = newOverwrites;
            }
            return res;
        }
        
        /// <summary>
        /// Updates buttons in the form to react to user selection.
        /// Call this whenever selection changes.
        /// </summary>
        private void UpdateButtons()
        {
            // OK button is only enabled if there are selected plugins.
            OKBtn.Enabled = PipelinePluginsLst.SelectedIndices.Count != 0;
        }
        
        /// <summary>
        /// Called to draw an item in our plugins list box. We have to do this by
        /// hand to be able to support disabled items.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginsLst_DrawItem(object sender, DrawItemEventArgs e)
        {
            // The code to do this has been ripped off MSDN and adapted.
            // Open help and look at the code example for the DrawItemEventArgs class.
            if (listItemRegularBrush != null) {
                PluginToImportDisplayInfo pluginDisplayInfo = (PluginToImportDisplayInfo) PipelinePluginsLst.Items[e.Index];
                Brush foreBrush = listItemRegularBrush;
                if (PipelinePluginsLst.SelectedIndices.Contains(e.Index)) {
                    foreBrush = listItemSelectedBrush;
                } else if (!pluginDisplayInfo.Importable) {
                    foreBrush = listItemDisabledBrush;
                }
                e.DrawBackground();
                e.Graphics.DrawString(PipelinePluginsLst.Items[e.Index].ToString(), e.Font, foreBrush,
                    e.Bounds, StringFormat.GenericDefault);
                e.DrawFocusRectangle();
            }
        }
        
        /// <summary>
        /// Called when the selection changes in the list box. We need to prevent
        /// the user from selecting disabled plugins and warn him/her about
        /// incompatible plugins.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PipelinePluginsLst_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Watch for stack overflows.
            if (!reselecting) {
                reselecting = true;
                try {
                    PipelinePluginsLst.BeginUpdate();
                    try {
                        // Deselect any non-importable plugins and warn about any
                        // non-compatible plugins.
                        for (int i = 0; i < PipelinePluginsLst.Items.Count; ++i) {
                            if (!((PluginToImportDisplayInfo) PipelinePluginsLst.Items[i]).Importable) {
                                // If this is the first time user does this, warn him/her.
                                if (!warnedAboutGlobalOverwrites && PipelinePluginsLst.SelectedIndices.Contains(i)) {
                                    MessageBox.Show(this, Resources.ImportPipelinePluginsForm_CantOverwriteGlobalMsg,
                                        Resources.ImportPipelinePluginsForm_MsgTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    warnedAboutGlobalOverwrites = true;
                                }
                                PipelinePluginsLst.SetSelected(i, false);
                            }
                            if (!((PluginToImportDisplayInfo) PipelinePluginsLst.Items[i]).Compatible) {
                                // If this is the first time user does this, warn him/her.
                                if (!warnedAboutIncompatiblePlugins && PipelinePluginsLst.SelectedIndices.Contains(i)) {
                                    MessageBox.Show(this, Resources.ImportPipelinePluginsForm_IncompatiblePluginsMsg,
                                        Resources.ImportPipelinePluginsForm_MsgTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    warnedAboutIncompatiblePlugins = true;
                                }
                            }
                        }
                    } finally {
                        PipelinePluginsLst.EndUpdate();
                    }
                } finally {
                    reselecting = false;
                }
            }

            // Update buttons to match the new selection.
            UpdateButtons();
        }
        
        /// <summary>
        /// Called when the form is first loaded. We perform further initialization here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ImportPipelinePluginsForm_Load(object sender, EventArgs e)
        {
            // Create brushes from our list box properties.
            listItemRegularBrush = new SolidBrush(PipelinePluginsLst.ForeColor);
            listItemDisabledBrush = new SolidBrush(SystemColors.GrayText);
            listItemSelectedBrush = new SolidBrush(PipelinePluginsLst.BackColor);
        }
        
        /// <summary>
        /// Called when the form is closed and is about to be released. We perform
        /// final cleanup here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ImportPipelinePluginsForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            // Dispose of brushes before leaving.
            if (listItemRegularBrush != null) {
                listItemRegularBrush.Dispose();
                listItemRegularBrush = null;
            }
            if (listItemDisabledBrush != null) {
                listItemDisabledBrush.Dispose();
                listItemDisabledBrush = null;
            }
            if (listItemSelectedBrush != null) {
                listItemSelectedBrush.Dispose();
                listItemSelectedBrush = null;
            }
        }
        
        /// <summary>
        /// Simple wrapper used to display pipeline plugin infos in the list. Holds
        /// extra info to know if plugins are importable or not.
        /// </summary>
        private sealed class PluginToImportDisplayInfo
        {
            /// <summary>
            /// Info about pipeline plugin to display.
            /// </summary>
            public PipelinePluginInfo PluginInfo
            {
                get;
                set;
            }

            /// <summary>
            /// Whether pipeline plugin can be safely imported or not.
            /// </summary>
            public bool Importable
            {
                get;
                set;
            }

            /// <summary>
            /// Whether pipeline plugin is compatible with this version of Path Copy Copy.
            /// An incompatible plugin is still <see cref="Importable"/>, but won't work
            /// in this version.
            /// </summary>
            public bool Compatible
            {
                get {
                    return PluginInfo.Compatible;
                }
            }
            
            /// <summary>
            /// Returns a textual representation of this pipeline plugin info.
            /// </summary>
            /// <returns>Textual representation of <see cref="PluginInfo"/>.</returns>
            public override string ToString()
            {
                return (PluginInfo != null) ? PluginInfo.ToString() : String.Empty;
            }
        }
    }
}
