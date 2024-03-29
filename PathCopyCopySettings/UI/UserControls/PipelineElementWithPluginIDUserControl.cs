﻿// PipelineElementWithPluginIDUserControl.cs
// (c) 2019-2021, Charles Lechasseur
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
using System.Linq;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl used to configure a pipeline element with plugin ID.
    /// </summary>
    public partial class PipelineElementWithPluginIDUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private readonly PipelineElementWithPluginID element;

        /// Whether to include pipeline plugins as possible plugins for our element.
        private readonly bool includePipelinePlugins;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        /// <param name="includePipelinePlugins">Whether to include pipeline
        /// plugins as possible plugins for our element.</param>
        public PipelineElementWithPluginIDUserControl(PipelineElementWithPluginID element,
            bool includePipelinePlugins)
        {
            this.element = element ?? throw new ArgumentNullException(nameof(element));
            this.includePipelinePlugins = includePipelinePlugins;

            InitializeComponent();
        }

        /// <summary>
        /// Called to load our pipeline element. We populate our controls here.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected override void OnPipelineElementLoad(EventArgs e)
        {
            base.OnPipelineElementLoad(e);

            // First load list of plugins to display in the listbox for the base plugin.
            List<Plugin> plugins;
            using (UserSettings settings = new UserSettings()) {
                // If we're instructed to include pipeline plugins, we actually want *temp*
                // pipeline plugins saved by the MainForm, in order to get the most recent
                // snapshot of pipeline plugins.
                var pipelinePluginsOptions = includePipelinePlugins
                    ? PipelinePluginsOptions.FetchTempPipelinePlugins : PipelinePluginsOptions.FetchNone;
                List<Plugin> pluginsInDefaultOrder = PluginsRegistry.GetPluginsInDefaultOrder(settings, pipelinePluginsOptions);
                if (!includePipelinePlugins) {
                    // Sufficient when not using pipeline plugins.
                    plugins = pluginsInDefaultOrder;
                } else {
                    // Create sorted dictionary of all plugins from the list above, to be able to perform lookups.
                    SortedDictionary<Guid, Plugin> dictionaryOfAllPlugins = new SortedDictionary<Guid, Plugin>();
                    foreach (Plugin plugin in pluginsInDefaultOrder) {
                        if (!dictionaryOfAllPlugins.ContainsKey(plugin.Id)) {
                            dictionaryOfAllPlugins.Add(plugin.Id, plugin);
                        }
                    }

                    // Use UI display order from settings to order the plugins.
                    // (See MainForm.LoadSettings for some more details on this process)
                    List<Guid> uiDisplayOrder = settings.UIDisplayOrder;
                    if (uiDisplayOrder == null) {
                        // No display order, just use all plugins in default order
                        uiDisplayOrder = pluginsInDefaultOrder.Select(plugin => plugin.Id).ToList();
                    }
                    SortedSet<Guid> uiDisplayOrderAsSet = new SortedSet<Guid>(uiDisplayOrder);
                    plugins = PluginsRegistry.OrderPluginsToDisplay(dictionaryOfAllPlugins,
                        uiDisplayOrder, uiDisplayOrderAsSet, pluginsInDefaultOrder);
                }
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
                    OnPipelineElementChanged(EventArgs.Empty);
                }
            } else {
                // User deselected plugins, use default one.
                element.PluginID = new Guid(Resources.LONG_PATH_PLUGIN_ID);
                OnPipelineElementChanged(EventArgs.Empty);
            }
        }
    }
}
