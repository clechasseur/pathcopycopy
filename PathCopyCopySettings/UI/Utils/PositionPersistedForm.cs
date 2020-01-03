// PositionPersistedForm.cs
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
using System.Drawing;
using System.Windows.Forms;

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Base form class that automatically persists its size and position
    /// in the user settings when moved or resized.
    /// </summary>
    public class PositionPersistedForm : UserSettingsWieldingForm
    {
        /// Set this when it's OK to listen to size/position changes.
        private bool canSaveFormInfo = false;

        /// <summary>
        /// Name used to identify the form's information
        /// in the user settings. By default, uses the
        /// form type's fully-qualified name; this is fine
        /// if there is only ever one instance of the form
        /// at the same time on screen (or if it's OK if
        /// they share their size/position info).
        /// </summary>
        public virtual string FormInfoName
        {
            get {
                return GetType().FullName;
            }
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PositionPersistedForm()
            : base()
        {
            Load += PositionPersistedForm_Load;
            LocationChanged += PositionPersistedForm_LocationChanged;
            SizeChanged += PositionPersistedForm_SizeChanged;
        }

        /// <summary>
        /// Called when the form is first loaded, before it is displayed.
        /// We use this opportunity to reposition the form if there is
        /// saved information in the user settings.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PositionPersistedForm_Load(object sender, EventArgs e)
        {
            // Load form information if we have some.
            Settings.GetFormInformation(FormInfoName, out Point position, out Size size);
            if (position.X != -1 && position.Y != -1) {
                StartPosition = FormStartPosition.Manual;
                Location = position;
            }
            if (size.Width != -1 && size.Height != -1) {
                Size = size;
            }

            // Set flag telling listeners that from now on, any change is size/position
            // must be persisted to user settings.
            canSaveFormInfo = true;
        }

        /// <summary>
        /// Called when the form's position changes. We persist the
        /// new position in user settings.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PositionPersistedForm_LocationChanged(object sender, EventArgs e)
        {
            if (canSaveFormInfo) {
                Settings.SetFormInformation(FormInfoName, Location, null);
            }
        }

        /// <summary>
        /// Called when the form's size changes. We persist the
        /// new size in user settings.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void PositionPersistedForm_SizeChanged(object sender, EventArgs e)
        {
            if (canSaveFormInfo) {
                Settings.SetFormInformation(FormInfoName, null, Size);
            }
        }
    }
}
