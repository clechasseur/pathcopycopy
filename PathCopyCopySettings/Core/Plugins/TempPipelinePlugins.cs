// TempPipelinePlugins.cs
// (c) 2020, Charles Lechasseur
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
using System.Linq;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// Class intended to be used to save a temporary <see cref="PipelinePluginInfo"/>
    /// to <see cref="UserSettings"/>, deleting it when the object is disposed.
    /// </summary>
    public sealed class TempPipelinePluginSaver : IDisposable
    {
        /// Object to access user settings.
        private readonly UserSettings settings;

        /// Whether the temp pipeline plugin already existed when we saved it.
        /// If so, we won't delete it as it is "owned" by someone else.
        private readonly bool existed;

        /// <summary>
        /// Pipeline plugin info to save and delete.
        /// </summary>
        public PipelinePluginInfo PluginInfo
        {
            get;
            private set;
        }
            
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="pluginInfo"><see cref="PipelinePluginInfo"/> to save.</param>
        /// <param name="settings">Object to access user settings.</param>
        public TempPipelinePluginSaver(PipelinePluginInfo pluginInfo, UserSettings settings)
        {
            PluginInfo = pluginInfo ?? throw new ArgumentNullException(nameof(pluginInfo));
            this.settings = settings ?? throw new ArgumentNullException(nameof(settings));

            existed = this.settings.SaveTempPipelinePlugin(PluginInfo);
        }
            
        /// <summary>
        /// Finalizer. Deletes the temp pipeline plugin from settings if object
        /// hasn't been properly disposed of.
        /// </summary>
        ~TempPipelinePluginSaver()
        {
            if (!existed) {
                settings.DeleteTempPipelinePlugin(PluginInfo);
            }
        }
            
        /// <inheritDoc/>
        public void Dispose()
        {
            if (!existed) {
                settings.DeleteTempPipelinePlugin(PluginInfo);
            }
            GC.SuppressFinalize(this);
        }
    }

    /// <summary>
    /// Class that can keep track of temp pipeline plugins saved to
    /// <see cref="UserSettings"/>, saving and deleting them as appropriate.
    /// </summary>
    public sealed class TempPipelinePluginsHelper : IDisposable
    {
        /// Keeps track of our temp pipeline plugins saved to settings.
        private readonly List<TempPipelinePluginSaver> tempPipelinePluginSavers = new List<TempPipelinePluginSaver>();

        /// Object to access user settings.
        private readonly UserSettings settings;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="userSettings">Object to access user settings.</param>
        public TempPipelinePluginsHelper(UserSettings settings)
        {
            this.settings = settings ?? throw new ArgumentNullException(nameof(settings));
        }

        /// <summary>
        /// Finalizer. Clears tracked temp pipeline plugins.
        /// </summary>
        ~TempPipelinePluginsHelper()
        {
            Clear();
        }
            
        /// <inheritDoc/>
        public void Dispose()
        {
            Clear();
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Adds a temp pipeline plugin, saving it to the registry.
        /// </summary>
        /// <param name="pluginInfo">Plugin info for the pipeline
        /// plugin to save.</param>
        public void Add(PipelinePluginInfo pluginInfo)
        {
            // If we already have this plugin, destroy it, then add it again.
            Remove(pluginInfo);
            tempPipelinePluginSavers.Add(new TempPipelinePluginSaver(pluginInfo, settings));
        }

        /// <summary>
        /// Removes a temp pipeline plugin from the registry.
        /// </summary>
        /// <param name="pluginInfo">Plugin info for the pipeline
        /// plugin to destroy.</param>
        public void Remove(PipelinePluginInfo pluginInfo)
        {
            int index = tempPipelinePluginSavers.FindIndex(saver => saver.PluginInfo == pluginInfo);
            if (index >= 0) {
                tempPipelinePluginSavers[index].Dispose();
                tempPipelinePluginSavers.RemoveAt(index);
            }
        }

        /// <summary>
        /// Assigns a collection of pipeline plugins to the helper.
        /// All existing tracked temp pipeline plugins will be updated.
        /// </summary>
        /// <param name="pluginInfos">Collection of plugin infos
        /// for the pipeline plugins to store.</param>
        public void Assign(IEnumerable<PipelinePluginInfo> pluginInfos)
        {
            if (pluginInfos == null) {
                throw new ArgumentNullException(nameof(pluginInfos));
            }

            // Remove any existing temp pipeline plugins that are no longer
            // in the list.
            var toRemove = tempPipelinePluginSavers.Where(
                saver => !pluginInfos.Any(info => info == saver.PluginInfo));
            foreach (var saver in toRemove) {
                saver.Dispose();
                tempPipelinePluginSavers.Remove(saver);
            }

            // Now add all plugins. Existing ones will be replaced.
            foreach (var pluginInfo in pluginInfos) {
                Add(pluginInfo);
            }
        }

        /// <summary>
        /// Assigns a collection of plugins to the helper.
        /// All existing tracked temp pipeline plugins will be updated.
        /// Plugins that are not pipeline plugins will be ignored.
        /// </summary>
        /// <param name="plugins">Collection of plugins to track.</param>
        public void Assign(IEnumerable<Plugin> plugins)
        {
            if (plugins == null) {
                throw new ArgumentNullException(nameof(plugins));
            }

            Assign(plugins.Where(plugin => plugin is PipelinePlugin)
                          .Cast<PipelinePlugin>()
                          .Select(plugin => plugin.Info));
        }

        /// <summary>
        /// Removes all tracked temp pipeline plugins.
        /// </summary>
        public void Clear()
        {
            tempPipelinePluginSavers.ForEach(saver => saver.Dispose());
            tempPipelinePluginSavers.Clear();
        }
    }
}
