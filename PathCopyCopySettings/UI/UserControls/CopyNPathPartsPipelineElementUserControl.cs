// InjectDriveLabelPipelineElementUserControl.cs
// (c) 2020, Charles Lechasseur
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

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// UserControl for CopyNPathParts pipeline elements. Allows editing
    /// the number of path parts and from which direction to copy.
    /// </summary>
    public partial class CopyNPathPartsPipelineElementUserControl : PipelineElementUserControl
    {
        /// Index of the "first" item in the combo box.
        private const int IndexFirst = 0;

        /// Index of the "last" item in the combo box.
        private const int IndexLast = 1;

        /// Element we're configuring.
        private readonly CopyNPathPartsPipelineElement element;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="element">Pipeline element to configure.</param>
        public CopyNPathPartsPipelineElementUserControl(CopyNPathPartsPipelineElement element)
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
            FirstLastCombo.SelectedIndex = element.First ? IndexFirst : IndexLast;
            NumPartsTxt.Text = element.NumParts.ToString(CultureInfo.InvariantCulture);
        }

        /// <summary>
        /// Called when the user selects a new copy direction.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void FirstLastCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            element.First = FirstLastCombo.SelectedIndex == IndexFirst;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user selects a new copy direction.
        /// We update our associated pipeline element here.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void NumPartsTxt_TextChanged(object sender, EventArgs e)
        {
            // Try parsing the string. Because we block non-numeric keys, if the
            // parsing fails, it means the edit box is empty - use 0 in that case.
            bool valid = int.TryParse(NumPartsTxt.Text, out int numParts);
            element.NumParts = valid ? numParts : 0;
            OnPipelineElementChanged(EventArgs.Empty);
        }

        /// <summary>
        /// Called when the user presses a key in the NumParts textbox.
        /// We filter out non-numeric keys.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event arguments.</param>
        private void NumPartsTxt_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!(e.KeyChar >= '0' && e.KeyChar <= '9' || e.KeyChar == (char) Keys.Back)) {
                e.Handled = true;
            }
        }
    }
}
