// PluginRegistry.cs
// (c) 2016-2021, Charles Lechasseur
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
using System.Security;
using Microsoft.Win32;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// Enum used to specify what kind of pipeline plugin to load
    /// from the registry. Akin to the <c>PipelinePluginsOptions</c>
    /// enum in C++ code.
    /// </summary>
    [Flags]
    public enum PipelinePluginsOptions
    {
        FetchPipelinePlugins = 0x1,
        FetchTempPipelinePlugins = 0x2,
        FetchBoth = FetchPipelinePlugins | FetchTempPipelinePlugins,
        FetchNone = 0x0,
    }

    /// <summary>
    /// Registry used to fetch list of plugins and order them for display.
    /// Akin to the <c>PluginsRegistry</c> class in C++ code.
    /// </summary>
    public static class PluginsRegistry
    {
        /// Path of the COM plugins key in the registry.
        private const string COMPluginsKey = @"Software\clechasseur\PathCopyCopy\Plugins";

        /// Path of the COM plugins key in the registry when running under WOW64 (global only).
        private const string COMPluginsKeyWow64 = @"Software\Wow6432Node\clechasseur\PathCopyCopy\Plugins";

        /// Name of the legacy value that used to store a marker to know when the plugins were last updated.
        /// We skip that if it exists.
        private const string LegacyCOMPluginsLastUpdateValueName = "LastUpdate";

        /// Bean containing info about a COM plugin.
        private sealed class COMPluginInfo
        {
            /// <summary>
            /// <see cref="COMPlugin"/> instance.
            /// </summary>
            public COMPlugin Plugin
            {
                get;
                set;
            }

            /// <summary>
            /// COM plugin's group ID.
            /// </summary>
            /// <seealso cref="GroupPosition"/>
            public int GroupId
            {
                get;
                set;
            }

            /// <summary>
            /// COM plugin's position in its group.
            /// </summary>
            /// <seealso cref="GroupId"/>
            public int GroupPosition
            {
                get;
                set;
            }
        }

        /// Bean containing info about a registry key that contains COM plugins.
        private sealed class COMPluginsRegKeyInfo
        {
            /// <summary>
            /// Whether this reg key contains global plugins.
            /// </summary>
            public bool Global
            {
                get;
                set;
            }

            /// <summary>
            /// Path of registry key containing COM plugins.
            /// </summary>
            public string KeyPath
            {
                get;
                set;
            }

            /// <summary>
            /// Registry root containing this reg key.
            /// </summary>
            public RegistryKey Root
            {
                get {
                    return Global ? Registry.LocalMachine : Registry.CurrentUser;
                }
            }
        }

        /// <summary>
        /// Returns a list containing all plugins to display in the default
        /// order. This should be used if user did not specify a custom order.
        /// </summary>
        /// <param name="settings"><see cref="UserSettings"/> object used to
        /// fetch plugin information.</param>
        /// <param name="pipelinePluginsOptions">What kind of pipeline plugins
        /// to include in the returned list (if any).</param>
        /// <returns>List of <see cref="Plugin"/>s in default order.</returns>
        public static List<Plugin> GetPluginsInDefaultOrder(UserSettings settings,
            PipelinePluginsOptions pipelinePluginsOptions)
        {
            if (settings == null) {
                throw new ArgumentNullException(nameof(settings));
            }

            List<Plugin> plugins = new List<Plugin>();

            // This mimics the code in PathCopyCopyPluginsRegistry.cpp in the C++ project.

            // Default plugins
            GetDefaultPlugins(plugins, settings);

            // COM plugins
            GetCOMPlugins(plugins);

            // Pipeline plugins
            GetPipelinePlugins(plugins, settings, pipelinePluginsOptions);

            return plugins;
        }

        /// <summary>
        /// Given a set of plugins, orders them for display according to the users' settings.
        /// </summary>
        /// <param name="plugins">Plugins to order.</param>
        /// <param name="displayOrder">List of plugin IDs representing display order
        /// as found in the users' settings.</param>
        /// <param name="knownPlugins">Optional set of known plugins. If set, all plugins in
        /// <paramref name="plugins"/> that are not found in <paramref name="displayOrder"/>
        /// will be added at the end of the plugins to display.</param>
        /// <param name="pluginsInDefaultOrder">Optional list of plugins in default order.
        /// Should represent the default way of displaying <paramref name="plugins"/>.
        /// Ignored unless <paramref name="knownPlugins"/> is also set.</param>
        /// <returns>List of plugins in the order they should be displayed.</returns>
        public static List<Plugin> OrderPluginsToDisplay(IDictionary<Guid, Plugin> plugins,
            IEnumerable<Guid> displayOrder, ISet<Guid> knownPlugins,
            IEnumerable<Plugin> pluginsInDefaultOrder)
        {
            if (plugins == null) {
                throw new ArgumentNullException(nameof(plugins));
            }
            if (displayOrder == null) {
                throw new ArgumentNullException(nameof(displayOrder));
            }

            // This mimics the code in PathCopyCopyPluginsRegistry.cpp in the C++ project.
            
            // First generate list of plugins from display order.
            List<Plugin> orderedPlugins = new List<Plugin>();
            foreach (Guid id in displayOrder) {
                if (plugins.TryGetValue(id, out Plugin plugin)) {
                    orderedPlugins.Add(plugin);
                }
            }

            // If we have a list of known plugins, add all unknown plugins
            // after those specified in the display order.
            if (knownPlugins != null) {
                // Create set of plugin IDs for all plugins.
                SortedSet<Guid> pluginIds = new SortedSet<Guid>(plugins.Values.Select(plugin => plugin.Id));

                // Substract known plugins from list of all plugins to find unknown plugins' IDs.
                pluginIds.ExceptWith(knownPlugins);
                if (pluginIds.Count > 0) {
                    // We have unknown plugins. Add a separator if needed, then add
                    // them to the returned list.
                    if (orderedPlugins.Count > 0 && !(orderedPlugins[orderedPlugins.Count - 1] is SeparatorPlugin)) {
                        orderedPlugins.Add(new SeparatorPlugin());
                    }
                    if (pluginsInDefaultOrder != null) {
                        // We know how to display plugins in default order: scan that
                        // list and add all unknown plugins. This will probably help
                        // display them in correct order.
                        bool prevWasUnknown = false;
                        foreach (Plugin plugin in pluginsInDefaultOrder) {
                            if (!(plugin is SeparatorPlugin)) {
                                if (pluginIds.Contains(plugin.Id)) {
                                    // This is an unknown plugin, add it.
                                    orderedPlugins.Add(plugin);
                                    prevWasUnknown = true;
                                } else {
                                    // Not an unknown plugin.
                                    prevWasUnknown = false;
                                }
                            } else {
                                // If prev plugin was an unknown plugin, add the separator anyway.
                                // This takes care of preserving COM plugin grouping.
                                if (prevWasUnknown) {
                                    orderedPlugins.Add(plugin);
                                    prevWasUnknown = false;
                                }
                            }
                        }
                    } else {
                        // No info on how to display plugins, simply add them in
                        // a possibly-random order.
                        foreach (Guid id in pluginIds) {
                            if (plugins.TryGetValue(id, out Plugin plugin)) {
                                orderedPlugins.Add(plugin);
                            }
                        }
                    }
                }
            }

            // Return final list of ordered plugins.
            return orderedPlugins;
        }

        /// <summary>
        /// Adds all default (e.g. builtin) plugins to the given list,
        /// in the default display order.
        /// </summary>
        /// <param name="plugins">List where to add plugins.</param>
        /// <param name="settings">Optional <see cref="UserSettings"/>
        /// used to fetch icon files if present.</param>
        private static void GetDefaultPlugins(List<Plugin> plugins, UserSettings settings)
        {
            Debug.Assert(plugins != null);
            Debug.Assert(settings != null);

            SeparatorPlugin separator = new SeparatorPlugin();

            // This mimics the code in PathCopyCopyPluginsRegistry.cpp in the C++ project.

            plugins.Add(CreateDefaultPlugin(Resources.SHORT_NAME_PLUGIN_ID, Resources.SHORT_NAME_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.LONG_NAME_PLUGIN_ID, Resources.LONG_NAME_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.SHORT_PATH_PLUGIN_ID, Resources.SHORT_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.LONG_PATH_PLUGIN_ID, Resources.LONG_PATH_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.SHORT_FOLDER_PLUGIN_ID, Resources.SHORT_FOLDER_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.LONG_FOLDER_PLUGIN_ID, Resources.LONG_FOLDER_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.SHORT_UNC_PATH_PLUGIN_ID, Resources.SHORT_UNC_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.LONG_UNC_PATH_PLUGIN_ID, Resources.LONG_UNC_PATH_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.SHORT_UNC_FOLDER_PATH_PLUGIN_ID, Resources.SHORT_UNC_FOLDER_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.LONG_UNC_FOLDER_PATH_PLUGIN_ID, Resources.LONG_UNC_FOLDER_PATH_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.INTERNET_PATH_PLUGIN_ID, Resources.INTERNET_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.SAMBA_PATH_PLUGIN_ID, Resources.SAMBA_PATH_PLUGIN_DESCRIPTION, settings));

            plugins.Add(separator);
            plugins.Add(CreateDefaultPlugin(Resources.UNIX_PATH_PLUGIN_ID, Resources.UNIX_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.CYGWIN_PATH_PLUGIN_ID, Resources.CYGWIN_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.WSL_PATH_PLUGIN_ID, Resources.WSL_PATH_PLUGIN_DESCRIPTION, settings));
            plugins.Add(CreateDefaultPlugin(Resources.MSYS_PATH_PLUGIN_ID, Resources.MSYS_PATH_PLUGIN_DESCRIPTION, settings));
        }

        /// <summary>
        /// Adds all COM plugins to the given list in the proper display order.
        /// </summary>
        /// <param name="plugins">List where to add plugins.</param>
        private static void GetCOMPlugins(List<Plugin> plugins)
        {
            Debug.Assert(plugins != null);

            List<COMPluginInfo> comPluginInfos = new List<COMPluginInfo>();
            SeparatorPlugin separator = new SeparatorPlugin();

            // Get path to the registry keys that contain the COM plugins.
            // If we're called by a 32-bit extension running under WOW64 we'll need to adjust.
            List<COMPluginsRegKeyInfo> regKeyInfos = new List<COMPluginsRegKeyInfo> {
                new COMPluginsRegKeyInfo {
                    Global = true,
                    KeyPath = (!PCCEnvironment.Is64Bit && PCCEnvironment.IsWow64) ? COMPluginsKeyWow64 : COMPluginsKey,
                },
                new COMPluginsRegKeyInfo {
                    Global = false,
                    KeyPath = COMPluginsKey,
                },
            };

            // Create executor to be able to fetch info for COM plugins.
            COMPluginExecutor executor = new COMPluginExecutor();

            // Scan each registry key in turn.
            foreach (COMPluginsRegKeyInfo regKeyInfo in regKeyInfos) {
                try {
                    using (RegistryKey pluginsKey = regKeyInfo.Root.OpenSubKey(regKeyInfo.KeyPath)) {
                        if (pluginsKey != null) {
                            // Key exists and user has access, scan for plugins.
                            // Get names of all values. Each name is a plugin ID except for the marker.
                            string[] ids = pluginsKey.GetValueNames();
                            foreach (string id in ids) {
                                // Try converting this ID to a Guid. Note that there are other values
                                // in that key so if it fails, simply skip it.
                                Guid? idAsGuid = null;
                                try {
                                    if (id != LegacyCOMPluginsLastUpdateValueName) {
                                        idAsGuid = new Guid(id);
                                    }
                                } catch (FormatException) {
                                    idAsGuid = null;
                                } catch (OverflowException) {
                                    idAsGuid = null;
                                }
                                if (idAsGuid != null) {
                                    // Fetch plugin infos using executor.
                                    string description = null;
                                    int groupId = 0;
                                    int groupPosition = 0;
                                    string iconFile = null;
                                    try {
                                        description = executor.GetDescription(idAsGuid.Value);
                                        groupId = executor.GetGroupId(idAsGuid.Value);
                                        groupPosition = executor.GetGroupPosition(idAsGuid.Value);

                                        if (executor.GetUseDefaultIcon(idAsGuid.Value)) {
                                            iconFile = string.Empty;
                                        } else {
                                            iconFile = executor.GetIconFile(idAsGuid.Value);
                                            if (string.IsNullOrEmpty(iconFile)) {
                                                // No icon file specified, assume no icon.
                                                iconFile = null;
                                            }
                                        }
                                    } catch (COMPluginExecutorException) {
                                        // Failed to fetch information, skip this plugin.
                                        idAsGuid = null;
                                    }
                                    if (idAsGuid != null) {
                                        // Construct bean for this plugin and add it to the list.
                                        comPluginInfos.Add(new COMPluginInfo {
                                            Plugin = new COMPlugin(idAsGuid.Value, description, iconFile, regKeyInfo.Global),
                                            GroupId = groupId,
                                            GroupPosition = groupPosition,
                                        });
                                    }
                                }
                            }
                        }
                    }
                } catch (SecurityException) {
                    // User does not have access to that key, skip.
                } catch (ObjectDisposedException) {
                    // There's something seriously wrong with the .NET framework, but hey.
                }
            }

            // Sort list of COM plugins by IDs and remove duplicates, since a plugin might be
            // registered both globally and for the current user.
            if (comPluginInfos.Count > 1) {
                comPluginInfos.Sort((info1, info2) => info1.Plugin.Id.CompareTo(info2.Plugin.Id));
                for (int i = comPluginInfos.Count - 1; i > 0; --i) {
                    if (comPluginInfos[i].Plugin.Id.Equals(comPluginInfos[i - 1].Plugin.Id)) {
                        comPluginInfos.RemoveAt(i);
                    }
                }
            }

            // Now sort list of COM plugins first by group ID then by group position.
            comPluginInfos.Sort((info1, info2) => {
                int cmp = info1.GroupId - info2.GroupId;
                if (cmp == 0) {
                    cmp = info1.GroupPosition - info2.GroupPosition;
                }
                return cmp;
            });

            // Copy all plugins to provided list. Insert a separator between groups.
            if (comPluginInfos.Count > 0) {
                int currentGroup = int.MaxValue;
                foreach (var info in comPluginInfos) {
                    if (plugins.Count > 0 && currentGroup != info.GroupId) {
                        plugins.Add(separator);
                    }
                    plugins.Add(info.Plugin);
                    currentGroup = info.GroupId;
                }
            }
        }

        /// <summary>
        /// Adds all pipeline plugins to the given list, in display order if possible.
        /// </summary>
        /// <param name="plugins">List where to add plugins</param>
        /// <param name="settings"><see cref="UserSettings"/> used to fetch
        /// pipeline plugins.</param>
        /// <param name="pipelinePluginsOptions">What kind of pipeline plugins
        /// to add to the list.</param>
        private static void GetPipelinePlugins(List<Plugin> plugins, UserSettings settings,
            PipelinePluginsOptions pipelinePluginsOptions)
        {
            Debug.Assert(plugins != null);
            Debug.Assert(settings != null);

            List<PipelinePluginInfo> pluginInfos = new List<PipelinePluginInfo>();
            if ((pipelinePluginsOptions & PipelinePluginsOptions.FetchPipelinePlugins) != 0) {
                pluginInfos.AddRange(settings.PipelinePlugins);
            }
            if ((pipelinePluginsOptions & PipelinePluginsOptions.FetchTempPipelinePlugins) != 0) {
                // Temp pipeline plugins can actually override existing pipeline plugins.
                // This mimics the code in PathCopyCopyPluginsRegistry.cpp in the C++ project.
                pluginInfos = settings.TempPipelinePlugins.Union(pluginInfos).ToList();
            }
            if (pluginInfos.Count > 0) {
                if (plugins.Count > 0) {
                    plugins.Add(new SeparatorPlugin());
                }
                plugins.AddRange(PipelinePluginInfo.ToPlugins(pluginInfos));
            }
        }

        /// <summary>
        /// Creates a <see cref="Plugin"/> object for a default plugin and returns it.
        /// </summary>
        /// <param name="id">ID of plugin.</param>
        /// <param name="description">Plugin's description.</param>
        /// <param name="settings"><see cref="UserSettings"/> to use to fetch
        /// icon files if set.</param>
        /// <returns>New <see cref="Plugin"/> instance.</returns>
        private static Plugin CreateDefaultPlugin(string id, string description, UserSettings settings)
        {
            Debug.Assert(!string.IsNullOrEmpty(id));
            Debug.Assert(!string.IsNullOrEmpty(description));

            // Fetch icon file from settings.
            string iconFile = settings?.GetIconFileForPlugin(new Guid(id));
            return new DefaultPlugin(id, description, iconFile);
        }
    }
}
