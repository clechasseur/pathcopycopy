﻿// PipelineElementWithExecutableUserControl.cs
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
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl used to configure pipeline elements with an executable path.
    /// </summary>
    public partial class PipelineElementWithExecutableUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private readonly PipelineElementWithExecutable element;

        /// <summary>
        /// Default constructor.
        /// </summary>
        public PipelineElementWithExecutableUserControl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Constructor with pipeline element.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public PipelineElementWithExecutableUserControl(PipelineElementWithExecutable element)
            : this()
        {
            Debug.Assert(element != null);

            this.element = element;
        }

        /// <summary>
        /// Called to load our pipeline element. We populate our controls here.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected override void OnPipelineElementLoad(EventArgs e)
        {
            base.OnPipelineElementLoad(e);
            ExecutableTxt.Text = element?.Executable ?? string.Empty;
        }
        
        /// <summary>
        /// Called when the text of the Executable textbox changes. We update
        /// our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ExecutableTxt_TextChanged(object sender, EventArgs e)
        {
            if (element != null) {
                element.Executable = ExecutableTxt.Text;
                OnPipelineElementChanged(EventArgs.Empty);
            }
        }

        /// <summary>
        /// Called when the user presses the button to browse for an executable.
        /// We will show an open dialog allowing user to pick one.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void BrowseForExecutableBtn_Click(object sender, EventArgs e)
        {
            // Show browse box, using the current filename as hint.
            try {
                ChooseExecutableOpenDlg.InitialDirectory = Path.GetDirectoryName(ExecutableTxt.Text);
                ChooseExecutableOpenDlg.FileName = Path.GetFileName(ExecutableTxt.Text);
            } catch (ArgumentException) {
                // Bad format or something, simply don't use.
            }
            if (ChooseExecutableOpenDlg.ShowDialog(this) == DialogResult.OK) {
                // User chose a new executable, copy its path back in our control.
                ExecutableTxt.Text = ChooseExecutableOpenDlg.FileName;
            }
        }
    }
}
