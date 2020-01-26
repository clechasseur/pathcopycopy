// UserSettingsWieldingForm.cs
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

using System.Windows.Forms;
using PathCopyCopy.Settings.Core;

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Base class for forms that need a <see cref="UserSettings"/> instance.
    /// </summary>
    public class UserSettingsWieldingForm : Form
    {
        /// <summary>
        /// <see cref="UserSettings"/> instance usable by the form.
        /// Created in constructor and disposed of when form is closed.
        /// </summary>
        protected UserSettings Settings
        {
            get;
            private set;
        }

        /// <summary>
        /// Constructor. Instanciates the <see cref="UserSettings"/>.
        /// </summary>
        public UserSettingsWieldingForm()
            : base()
        {
            Settings = new UserSettings();
            FormClosed += UserSettingsWieldingForm_FormClosed;
        }

        /// <summary>
        /// Called when the form is closed. We dispose of our
        /// <see cref="UserSettings"/> instance here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void UserSettingsWieldingForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Settings?.Dispose();
            Settings = null;
        }
    }
}
