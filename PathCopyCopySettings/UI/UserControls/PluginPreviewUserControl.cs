// PluginPreviewUserControl.cs
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

using System.Windows.Forms;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Core.Plugins;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// User control that can be used to display a preview for
    /// a specific <see cref="Plugin"/>.
    /// </summary>
    public partial class PluginPreviewUserControl : UserControl
    {
        /// Plugin we're currently previewing.
        private Plugin plugin;

        /// Object used to access user settings.
        private UserSettings settings;
        
        /// <summary>
        /// Plugin to preview. Change this to update the preview displayed.
        /// </summary>
        public Plugin Plugin
        {
            get {
                return plugin;
            }
            set {
                if (plugin != value) {
                    plugin = value;
                    UpdatePreview();
                }
            }
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PluginPreviewUserControl()
        {
            InitializeComponent();

            settings = new UserSettings();
        }

        /// <summary>
        /// Updates the preview displayed in the control.
        /// </summary>
        private void UpdatePreview()
        {
            if (plugin != null && !(plugin is SeparatorPlugin)) {
                PreviewTxt.Text = plugin.GetPreview(settings);
            } else {
                // Clear content of preview textbox.
                PreviewTxt.Clear();
            }
        }
    }
}
