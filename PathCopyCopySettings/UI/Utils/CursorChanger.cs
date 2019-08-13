// CursorChanger.cs
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

using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Helper class to change a control's cursor in a block of code. To use,
    /// create an instance in a <c>using</c> block:
    ///
    /// <code>
    /// using (new CursorChanger(MyControl, Cursors.WaitCursor)) {
    ///     ... // Stuff here
    /// }
    /// </code>
    /// </summary>
    public sealed class CursorChanger : IDisposable
    {
        /// Control whose cursor we change.
        private Control control;

        /// The control's former cursor.
        private Cursor oldCursor;
        
        /// <summary>
        /// Constructor. Saves a control's current cursor and optionally changes it.
        /// The cursor will be properly restored when the object is disposed of.
        /// </summary>
        /// <param name="control">Control whose cursor to modify.</param>
        /// <param name="newCursor">New cursor to assign. Can be <c>null</c>.</param>
        public CursorChanger(Control control, Cursor newCursor)
        {
            Debug.Assert(control != null);

            // Save data.
            this.control = control;
            this.oldCursor = this.control.Cursor;

            // Optionally change cursor.
            if (newCursor != null) {
                this.control.Cursor = newCursor;
            }
        }

        #region IDisposable Members
        
        /// <summary>
        /// Called when the object is disposed of. We set back the control's old cursor.
        /// </summary>
        public void Dispose()
        {
            control.Cursor = oldCursor;
        }

        #endregion
    }
}
