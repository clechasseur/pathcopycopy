// COMPlugins.cs
// (c) 2011-2019, Charles Lechasseur
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

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// <see cref="Plugin"/> subclass that is used for COM plugins.
    /// </summary>
    public sealed class COMPlugin : Plugin
    {
        /// Path to icon file for plugin.
        private readonly string iconFile;

        /// Whether this is a global plugin.
        private bool global;

        /// <inheritDoc/>
        public override string IconFile
        {
            get {
                return iconFile;
            }
            set {
                Debug.Fail("COM plugins' icon files cannot be modified");
            }
        }

        /// <summary>
        /// Whether this COM plugin is global (stored in LOCAL_MACHINE) or per-user
        /// (stored in CURRENT_USER). This depends on how the plugin was registered.
        /// </summary>
        public bool Global
        {
            get {
                return global;
            }
            set {
                global = value;
                CallPropertyChanged(nameof(Global));
            }
        }
        
        /// <summary>
        /// Creates a new COM plugin bean.
        /// </summary>
        /// <param name="id">Plugin unique ID.</param>
        /// <param name="description">Plugin description.</param>
        /// <param name="iconFile">Path to icon file for plugin, or an empty
        /// string to use the default icon, or <c>null</c> to avoid showing
        /// an icon for this plugin.</param>
        /// <param name="global">Whether this is a global plugin or one for
        /// the current user only.</param>
        public COMPlugin(Guid id, string description, string iconFile, bool global)
            : base(id, description)
        {
            this.iconFile = iconFile;
            this.global = global;
        }
    }
}
