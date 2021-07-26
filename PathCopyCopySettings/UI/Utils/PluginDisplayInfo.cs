// PluginDisplayInfo.cs
// (c) 2017-2021, Charles Lechasseur
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

using System.Diagnostics;
using PathCopyCopy.Settings.Core.Plugins;

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Wrapper for a <see cref="Plugin"/> to display it in
    /// a <c>DataGridView</c>.
    /// </summary>
    public sealed class PluginDisplayInfo
    {
        /// <summary>
        /// <see cref="Plugin"/> object we're displaying.
        /// </summary>
        public Plugin Plugin
        {
            get;
            set;
        }

        /// <summary>
        /// Name of the plugin to show in the grid view, AKA plugin description.
        /// </summary>
        public string Name
        {
            get {
                return Plugin.Description;
            }
        }

        /// <summary>
        /// Whether to show the plugin in the main contextual menu.
        /// </summary>
        public bool ShowInMainMenu
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to show the plugin in the PCC submenu.
        /// </summary>
        public bool ShowInSubmenu
        {
            get;
            set;
        }

        /// <summary>
        /// Equivalent of <see cref="ShowInMainMenu"/> that
        /// uses an empty string for <c>false</c> and <c>1</c>
        /// for <c>true</c>. Used to be able to avoid displaying
        /// checkboxes for separators in the data grid.
        /// </summary>
        public string ShowInMainMenuStr
        {
            get {
                return ShowInMainMenu ? "1" : string.Empty;
            }
            set {
                ShowInMainMenu = !string.IsNullOrEmpty(value);
            }
        }

        /// <summary>
        /// Equivalent of <see cref="ShowInSubmenu"/> that
        /// uses an empty string for <c>false</c> and <c>1</c>
        /// for <c>true</c>. Used to be able to avoid displaying
        /// checkboxes for separators in the data grid.
        /// </summary>
        public string ShowInSubmenuStr
        {
            get {
                return ShowInSubmenu ? "1" : string.Empty;
            }
            set {
                ShowInSubmenu = !string.IsNullOrEmpty(value);
            }
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="plugin"><see cref="Plugin"/> object to wrap.</param>
        public PluginDisplayInfo(Plugin plugin)
        {
            Debug.Assert(plugin != null);

            Plugin = plugin;
        }
    }
}
