// RegexTesterForm.cs
// (c) 2012-2019, Charles Lechasseur
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

using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Regexes;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Utils;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form used to test regular expressions using the same syntax as that
    /// used by the <see cref="T:RegexPipelineElement"/>. To use, create an
    /// instance and call the <see cref="TestRegex"/> method.
    /// </summary>
    /// <seealso cref="T:RegexTester"/>
    public partial class RegexTesterForm : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public RegexTesterForm()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Shows the form as a modal dialog, allowing the user to test out a
        /// regular expression and see the results.
        /// </summary>
        /// <param name="owner">Owner of this dialog. Can be <c>null</c>.</param>
        /// <returns>Dialog result.</returns>
        public DialogResult TestRegex(IWin32Window owner)
        {
            string regex = null;
            string format = null;
            bool ignoreCase = false;
            return TestRegex(ref regex, ref format, ref ignoreCase, owner);
        }
        
        /// <summary>
        /// Shows the form as a modal dialog, allowing the user to test out the
        /// given regular expression, see the results and optionally modify the
        /// regular expression.
        /// </summary>
        /// <param name="regex">Regex to test. Can be <c>null</c>. Upon exit,
        /// if the user clicked OK, will contain an updated version.</param>
        /// <param name="format">Format of replacement string to test. Can be
        /// <c>null</c>. Upon exit, if the user clicked OK, will contain an
        /// updated version.</param>
        /// <param name="ignoreCase">Flag indicating whether to ignore case
        /// when looking for matches. Upon exit, if the user clicked OK,
        /// will contain an updated value.</param>
        /// <param name="owner">Owner of this dialog. Can be <c>null</c>.</param>
        /// <returns>Dialog result. If the method returns <see cref="DialogResult.Cancel"/>,
        /// the contents of the various <c>ref</c> parameters is unchanged.</returns>
        public DialogResult TestRegex(ref string regex, ref string format, ref bool ignoreCase,
            IWin32Window owner)
        {
            // Init controls with the values provided.
            if (regex != null) {
                RegexTxt.Text = regex;
            }
            if (format != null) {
                ReplacementTxt.Text = format;
            }
            IgnoreCaseChk.Checked = ignoreCase;

            // Show as modal and wait for result.
            DialogResult res = ShowDialog(owner);

            // If user clicked OK, save new values.
            if (res == DialogResult.OK) {
                regex = RegexTxt.Text;
                format = ReplacementTxt.Text;
                ignoreCase = IgnoreCaseChk.Checked;
            }

            return res;
        }
        
        /// <summary>
        /// Called when the user presses the "Test" button. We perform the regex
        /// testing here and show the results to the user.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void TestBtn_Click(object sender, System.EventArgs e)
        {
            try {
                RegexTester tester = new RegexTester();
                using (new CursorChanger(this, Cursors.WaitCursor)) {
                    ResultTxt.Text = tester.ModifyWithRegex(SampleTxt.Text, RegexTxt.Text,
                        ReplacementTxt.Text, IgnoreCaseChk.Checked);
                }
                // If we made it here, the regular expression is valid.
                InvalidNoticeLbl.Visible = false;
            } catch (RegexTesterException) {
                // Invalid regular expression.
                ResultTxt.Clear();
                InvalidNoticeLbl.Visible = true;
            }
        }
        
        /// <summary>
        /// Called when the user presses a link to get a regex tutorial.
        /// We need to launch a browser to navigate to the website.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RegexSyntaxHelpLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start((sender as LinkLabel).Text);
        }

        /// <summary>
        /// Called when the user presses the Help button in the form's caption bar.
        /// We navigate to the wiki to show help in such a case.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void RegexTesterForm_HelpButtonClicked(object sender, CancelEventArgs e)
        {
            // Open wiki link to Custom Commands page, then cancel the event to avoid
            // displaying a help mouse pointer like the default behavior.
            Process.Start(Resources.WikiLink_CustomCommands);
            e.Cancel = true;
        }
    }
}
