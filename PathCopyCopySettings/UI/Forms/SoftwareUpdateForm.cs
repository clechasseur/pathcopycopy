// RegexTesterForm.cs
// (c) 2012-2021, Charles Lechasseur
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
using System.Text;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core;

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Form used to present a software update to the user.
    /// </summary>
    public partial class SoftwareUpdateForm : Form
    {
        /// Format string used to paste HTML release notes in the web browser.
        private const string ReleaseNotesHTMLFormat = @"<html><head><title></title></head><body>{0}</body></html>";

        /// URL to follow when the update link is clicked.
        private string updateUrl;

        /// Will be set to true if user chooses to ignore this update.
        private bool ignoreUpdate;
        
        /// <summary>
        /// Constructor.
        /// </summary>
        public SoftwareUpdateForm()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Shows the form as a modal dialog and displays a software update to the user.
        /// </summary>
        /// <param name="updateInfo">Software update info to display.</param>
        /// <param name="owner">Window owner. Can be <c>null</c>.</param>
        /// <returns><c>true</c> if the the user asked to ignore this update.</returns>
        public bool ShowUpdate(SoftwareUpdateInfo updateInfo, IWin32Window owner)
        {
            if (updateInfo == null) {
                throw new ArgumentNullException(nameof(updateInfo));
            }

            // Initialize our controls.
            if (owner == null) {
                StartPosition = FormStartPosition.CenterScreen;
            }
            SoftwareUpdateTitleLbl.Text = updateInfo.Name;
            updateUrl = updateInfo.Url;
            SoftwareUpdatesTooltip.SetToolTip(SoftwareUpdateUrlLinkLbl, updateUrl);
            if (updateInfo.ReleaseNotes != null && updateInfo.ReleaseNotes.Count != 0) {
                string innerHtml;
                if (updateInfo.ReleaseNotes.Count == 1) {
                    innerHtml = updateInfo.ReleaseNotes[0];
                } else {
                    StringBuilder htmlBuilder = new StringBuilder();
                    htmlBuilder.Append("<ul>");
                    foreach (string note in updateInfo.ReleaseNotes) {
                        htmlBuilder.Append($"<li>{note}</li>");
                    }
                    htmlBuilder.Append("</ul>");
                    innerHtml = htmlBuilder.ToString();
                }
                ReleaseNotesWebBrowser.DocumentText = string.Format(CultureInfo.InvariantCulture,
                    ReleaseNotesHTMLFormat, innerHtml);
            }

            // Display the form.
            ignoreUpdate = false;
            ShowDialog(owner);

            // Return whether user ignored the update.
            return ignoreUpdate;
        }
        
        /// <summary>
        /// Called when the link of the website is clicked.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void SoftwareUpdateUrlLinkLbl_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(updateUrl);
        }
        
        /// <summary>
        /// Called when the user presses the button to ignore the update. We need
        /// to set a flag that will be processed in <see cref="ShowUpdate"/>.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void IgnoreUpdateBtn_Click(object sender, EventArgs e)
        {
            ignoreUpdate = true;
            DialogResult = DialogResult.Cancel;
            Close();
        }
    }
}
