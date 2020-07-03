// PathsSeparatorPipelineElementUserControl.cs
// (c) 2019-2020, Charles Lechasseur
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
    /// UserControl used to configure a paths separator pipeline element.
    /// </summary>
    public partial class PathsSeparatorPipelineElementUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private  readonly PathsSeparatorPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public PathsSeparatorPipelineElementUserControl(PathsSeparatorPipelineElement element)
        {
            Debug.Assert(element != null);

            this.element = element;

            InitializeComponent();
        }

        /// <summary>
        /// Called when the control is initially loaded. We populate our controls here.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            SeparatorTxt.Text = element.PathsSeparator;
        }

        /// <summary>
        /// Called when the text of the Separator textbox changes. We update
        /// our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SeparatorTxt_TextChanged(object sender, EventArgs e)
        {
            element.PathsSeparator = SeparatorTxt.Text;
            OnPipelineElementChanged(EventArgs.Empty);
        }
    }
}
