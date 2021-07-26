// PopFromStackPipelineElementUserControl.cs
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
using System.Globalization;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.UI.Forms;

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl to configure a "pop from stack" pipeline element.
    /// </summary>
    public partial class PopFromStackPipelineElementUserControl : PipelineElementUserControl
    {
        /// Element we're configuring.
        private readonly PopFromStackPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Element we're configuring.</param>
        public PopFromStackPipelineElementUserControl(PopFromStackPipelineElement element)
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

            foreach (var control in Controls) {
                if (control is RadioButton button && (string) button.Tag == $"{(int) element.Location}") {
                    button.Checked = true;
                    break;
                }
            }
            RangeBeginTxt.Text = element.Begin.ToString(CultureInfo.InvariantCulture);
            RangeEndTxt.Text = element.End.ToString(CultureInfo.InvariantCulture);
            RegexTxt.Text = element.Regex ?? string.Empty;
            IgnoreCaseChk.Checked = element.IgnoreCase;
            UpdateControlsState();
        }

        /// <summary>
        /// Updates the state of controls depending on the pop location picked.
        /// </summary>
        private void UpdateControlsState()
        {
            RangeBeginTxt.Enabled = RangeEndTxt.Enabled = element.Location == PopFromStackLocation.Range;
            RegexTxt.Enabled = IgnoreCaseChk.Enabled = TestRegexBtn.Enabled =
                element.Location == PopFromStackLocation.Regex;
        }

        /// <summary>
        /// Called when the user changes the pop location. We update our
        /// associated element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void LocationRadio_CheckedChanged(object sender, EventArgs e)
        {
            // The actual location is stored in the radio button's Tag property.
            element.Location = (PopFromStackLocation) int.Parse((string) ((Control) sender).Tag,
                CultureInfo.InvariantCulture);
            UpdateControlsState();
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user changes the beginning of the range.
        /// We update our associated element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RangeBeginTxt_TextChanged(object sender, EventArgs e)
        {
            // Try parsing the string. Because we block non-numeric keys, if the
            // parsing fails, it means the edit box is empty - use 0 in that case.
            bool valid = int.TryParse(RangeBeginTxt.Text, out int begin);
            element.Begin = valid ? begin : 0;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user changes the end of the range.
        /// We update our associated element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RangeEndTxt_TextChanged(object sender, EventArgs e)
        {
            // Try parsing the string. Because we block non-numeric keys, if the
            // parsing fails, it means the edit box is empty - use 0 in that case.
            bool valid = int.TryParse(RangeEndTxt.Text, out int end);
            element.End = valid ? end : 0;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user changes the regex. We update our
        /// associated element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RegexTxt_TextChanged(object sender, EventArgs e)
        {
            element.Regex = RegexTxt.Text;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user changes the ignore case flag.
        /// We update our associated element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void IgnoreCaseChk_CheckedChanged(object sender, EventArgs e)
        {
            element.IgnoreCase = IgnoreCaseChk.Checked;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user presses the Test regex button.
        /// We must show a modal dialog allowing the user to test
        /// the regex and optionally modify it.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void TestRegexBtn_Click(object sender, EventArgs e)
        {
            // Grab content of controls so that the tester form
            // can optionally modify them.
            string regex = RegexTxt.Text;
            string format = "$&";
            bool ignoreCase = IgnoreCaseChk.Checked;

            // Show form and ask user to test. See what happens.
            using (RegexTesterForm testerForm = new RegexTesterForm()) {
                DialogResult res = testerForm.TestRegex(ref regex, ref format, ref ignoreCase, this);
                if (res == DialogResult.OK) {
                    // User pressed OK which means we have updated values.
                    RegexTxt.Text = regex;
                    IgnoreCaseChk.Checked = ignoreCase;
                }
            }
        }

        /// <summary>
        /// Called when the user presses a key in one of the numeric
        /// textboxes. We filter out non-numeric keys.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void NumericTxt_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!(e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == (char) Keys.Back)) {
                e.Handled = true;
            }
        }
    }
}
