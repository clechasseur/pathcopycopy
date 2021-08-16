// DisplayForSelectionPipelineElementUserControl.cs
// (c) 2021, Charles Lechasseur
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
using PathCopyCopy.Settings.Core.Plugins;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl used to configure a display for selection pipeline element.
    /// </summary>
    public partial class DisplayForSelectionPipelineElementUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private readonly DisplayForSelectionPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public DisplayForSelectionPipelineElementUserControl(DisplayForSelectionPipelineElement element)
        {
            Debug.Assert(element != null);

            this.element = element;

            InitializeComponent();
        }

        /// <summary>
        /// Called to load our pipeline element. We populate our controls here.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected override void OnPipelineElementLoad(EventArgs e)
        {
            base.OnPipelineElementLoad(e);
            ShowForFilesCheck.Checked = element.ShowForFiles;
            ShowForFoldersCheck.Checked = element.ShowForFolders;
        }

        /// <summary>
        /// Called when the checked state of the "Show for files" checkbox changes.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ShowForFilesCheck_CheckedChanged(object sender, EventArgs e)
        {
            element.ShowForFiles = ShowForFilesCheck.Checked;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the checked state of the "Show for folders" checkbox changes.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ShowForFoldersCheck_CheckedChanged(object sender, EventArgs e)
        {
            element.ShowForFolders = ShowForFoldersCheck.Checked;
            OnPipelineElementChanged(EventArgs.Empty);
        }
    }
}
