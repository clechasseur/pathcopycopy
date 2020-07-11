// PipelineElementUserControl.cs
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

namespace PathCopyCopy.Settings.UI.UserControls
{
    /// <summary>
    /// Base class for pipeline element editing controls.
    /// </summary>
    public partial class PipelineElementUserControl : UserControl
    {
        /// <summary>
        /// Event fired when the underlying pipeline element has changed.
        /// </summary>
        public event EventHandler PipelineElementChanged;

        /// <summary>
        /// Whether the control is currently loading the pipeline
        /// element. While this is <c>true</c>, <see cref="PipelineElementChanged"/>
        /// events will not fire.
        /// </summary>
        public bool LoadingPipelineElement
        {
            get;
            protected set;
        }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PipelineElementUserControl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Called when the control loads. We set <see cref="LoadingPipelineElement"/>
        /// to <c>true</c> while calling <see cref="OnPipelineElementLoad(EventArgs)"/>.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            Debug.Assert(!LoadingPipelineElement, "Loop detected while loading pipeline elements");
            LoadingPipelineElement = true;
            try {
                OnPipelineElementLoad(e);
            } finally {
                LoadingPipelineElement = false;
            }
        }

        /// <summary>
        /// Called when the control loads its pipeline element. Subclasses
        /// can override. During the call to this method, no <see cref="PipelineElementChanged"/>
        /// events will fire if <see cref="OnPipelineElementChanged(EventArgs)"/> is called.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected virtual void OnPipelineElementLoad(EventArgs e)
        {
            // Subclasses can override.
        }

        /// <summary>
        /// Fires the <see cref="PipelineElementChanged"/> event.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected virtual void OnPipelineElementChanged(EventArgs e)
        {
            if (!LoadingPipelineElement) {
                PipelineElementChanged(this, e);
            }
        }
    }
}
