// CommandLinePipelineElementUserControl.cs
// (c) 2020-2021, Charles Lechasseur
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
    /// UserControl used to configure command-line pipeline elements.
    /// </summary>
    public partial class CommandLinePipelineElementUserControl : PipelineElementWithExecutableUserControl
    {
        /// Element we're configuring.
        private readonly CommandLinePipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element we're configuring.</param>
        public CommandLinePipelineElementUserControl(CommandLinePipelineElement element)
            : base(element)
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
            ArgumentsTxt.Text = element.Arguments;
            UseFilelistChk.Checked = element.UseFilelist;
        }

        /// <summary>
        /// Called when the text of the Arguments textbox changes. We update
        /// our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ArgumentsTxt_TextChanged(object sender, EventArgs e)
        {
            element.Arguments = ArgumentsTxt.Text;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the checked state of the Use Filelist checkbox changes.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void UseFilelistChk_CheckedChanged(object sender, EventArgs e)
        {
            element.UseFilelist = UseFilelistChk.Checked;
            OnPipelineElementChanged(EventArgs.Empty);
        }
    }
}
