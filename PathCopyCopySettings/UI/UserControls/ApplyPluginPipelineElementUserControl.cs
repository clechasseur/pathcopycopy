// ApplyPluginPipelineElementUserControl.cs
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
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl used to configure an Apply Plugin pipeline element.
    /// </summary>
    public partial class ApplyPluginPipelineElementUserControl : UserControl
    {
        /// Element we're configuring.
        private ApplyPluginPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public ApplyPluginPipelineElementUserControl(ApplyPluginPipelineElement element)
        {
            Debug.Assert(element != null);

            this.element = element;

            InitializeComponent();
        }

        /// <summary>
        /// Called when the control is initially loaded. We populate our controls here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ApplyPluginPipelineElementUserControl_Load(object sender, EventArgs e)
        {
            // First load list of plugins to display in the listbox for the base
            // plugin. We only load default and COM plugins for this since we
            // don't want a pipeline plugin to be based off another (for now at least).
            List<Plugin> plugins;
            using (UserSettings settings = new UserSettings()) {
                plugins = PluginsRegistry.GetPluginsInDefaultOrder(settings, false);
            }

            // Add all plugins to the list box.
            BasePluginLst.Items.AddRange(plugins.ToArray());

            // Validate base plugin ID.
            Guid? basePluginId = element.PluginID;
            if (plugins.Find(pl => pl.Id == basePluginId) == null) {
                // Invalid, clear it so that we'll fall back
                // on the default below.
                basePluginId = null;
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
        /// Called when the user selects a new plugin in the list.
        /// We must update our associated element.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void BasePluginLst_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (BasePluginLst.SelectedIndex >= 0) {
                // Only update if this is a non-separator plugin selected.
                Plugin plugin = (Plugin) BasePluginLst.Items[BasePluginLst.SelectedIndex];
                if (!(plugin is SeparatorPlugin)) {
                    element.PluginID = plugin.Id;
                }
            } else {
                // User deselected plugins, use default one.
                element.PluginID = new Guid(Resources.LONG_PATH_PLUGIN_ID);
            }
        }
    }
}
