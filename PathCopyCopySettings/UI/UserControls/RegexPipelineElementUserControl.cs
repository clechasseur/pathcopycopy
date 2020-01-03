// RegexPipelineElementUserControl.cs
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
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.UI.Forms;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl used to configure a regex pipeline element.
    /// </summary>
    public partial class RegexPipelineElementUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private RegexPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public RegexPipelineElementUserControl(RegexPipelineElement element)
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
            FindTxt.Text = element.Regex;
            ReplaceTxt.Text = element.Format;
            IgnoreCaseChk.Checked = element.IgnoreCase;
        }

        /// <summary>
        /// Called when the text of the Regex textbox changes. We update
        /// our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void FindTxt_TextChanged(object sender, EventArgs e)
        {
            element.Regex = FindTxt.Text;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the text of the Replace textbox changes. We update
        /// our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void ReplaceTxt_TextChanged(object sender, EventArgs e)
        {
            element.Format = ReplaceTxt.Text;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user checks or unchecks the Ignore Case checkbox.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void IgnoreCaseChk_CheckedChanged(object sender, EventArgs e)
        {
            element.IgnoreCase = IgnoreCaseChk.Checked;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user presses the button to test a regular expression.
        /// We will show a dialog allowing the user to test the currently-provided
        /// expression and optionally change it.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void TestBtn_Click(object sender, EventArgs e)
        {
            // Grab content of controls so that the tester form
            // can optionally modify them.
            string regex = FindTxt.Text;
            string format = ReplaceTxt.Text;
            bool ignoreCase = IgnoreCaseChk.Checked;

            // Show form and ask user to test. See what happens.
            using (RegexTesterForm testerForm = new RegexTesterForm()) {
                DialogResult res = testerForm.TestRegex(ref regex, ref format, ref ignoreCase, this);
                if (res == DialogResult.OK) {
                    // User pressed OK which means we have updated values.
                    FindTxt.Text = regex;
                    ReplaceTxt.Text = format;
                    IgnoreCaseChk.Checked = ignoreCase;
                }
            }
        }
    }
}
