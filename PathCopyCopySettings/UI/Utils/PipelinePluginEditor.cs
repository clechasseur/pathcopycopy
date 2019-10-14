// PipelinePluginForm.cs
// (c) 2011-2019, Charles Lechasseur
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
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using PathCopyCopy.Settings.Core.Plugins;
using PathCopyCopy.Settings.UI.Forms;

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Class that is responsible for editing new and existing
    /// pipeline plugins. Will show a form as appropriate.
    /// </summary>
    public sealed class PipelinePluginEditor
    {
        /// Paths separator that copies multiple paths on the same line.
        internal const string PATHS_SEPARATOR_ON_SAME_LINE = " ";

        /// Owner of any form we create.
        private readonly IWin32Window owner;

        /// Plugin info for the plugin we're editing.
        private readonly PipelinePluginInfo pluginInfo;

        /// Pipeline of the plugin info, if we have one.
        private readonly Pipeline pipeline;

        /// <summary>
        /// Edits a new or existing pipeline plugin.
        /// </summary>
        /// <param name="owner">Owner to use for new forms. Can be <c>null</c>.</param>
        /// <param name="oldInfo">Info about pipeline plugin to edit. If <c>null</c>,
        /// a new pipeline plugin will be edited.</param>
        /// <returns>Info about pipeline plugin edited. Will be <c>null</c> if the
        /// user cancelled the editing.</returns>
        public static PipelinePluginInfo EditPlugin(IWin32Window owner, PipelinePluginInfo oldInfo)
        {
            PipelinePluginEditor editor = new PipelinePluginEditor(owner, oldInfo);
            return editor.Edit();
        }

        /// <summary>
        /// Determines if the given pipeline is "simple" - e.g., can be
        /// edited with the simple form instead of the advanced form.
        /// </summary>
        /// <param name="pipeline">Pipeline to validate. Cannot be <c>null</c>.</param>
        /// <returns><c>true</c> if the pipeline can be edited with the
        /// simple form.</returns>
        internal static bool IsPipelineSimple(Pipeline pipeline)
        {
            if (pipeline == null) {
                throw new ArgumentNullException(nameof(pipeline));
            }

            // All elements must be of different types, and pipeline must contain
            // an ApplyPlugin element.
            return pipeline.Elements.Distinct(new PipelineElementEqualityComparerByClassType()).Count() == pipeline.Elements.Count &&
                pipeline.Elements.Find(el => el is ApplyPluginPipelineElement) != null;
        }

        /// <summary>
        /// Private constructor called via <see cref="EditPlugin"/>.
        /// </summary>
        /// <param name="owner">Owner to use for new forms. Can be <c>null</c>.</param>
        /// <param name="oldInfo">Info about pipeline plugin to edit. If <c>null</c>,
        /// a new pipeline plugin will be edited.</param>
        private PipelinePluginEditor(IWin32Window owner, PipelinePluginInfo oldInfo)
        {
            // Save owner for any form we create.
            this.owner = owner;

            // Save old plugin info if we have one.
            pluginInfo = oldInfo;

            // If a plugin info was specified, decode its pipeline immediately.
            // We want pipeline exceptions to propagate out *before* we show a form.
            if (pluginInfo != null) {
                pipeline = PipelineDecoder.DecodePipeline(pluginInfo.EncodedElements);
            }
        }

        /// <summary>
        /// Edits our pipeline plugin.
        /// </summary>
        /// <returns>Info about pipeline plugin edited. Will be <c>null</c> if the
        /// user cancelled the editing.</returns>
        private PipelinePluginInfo Edit()
        {
            // Determine what form to show initially depending on the complexity of the pipeline
            // and the last edit mode used for the plugin.
            PipelinePluginInfo info = pluginInfo;
            bool advanced = pluginInfo != null && pluginInfo.EditMode.HasValue
                ? pluginInfo.EditMode.Value == PipelinePluginEditMode.Expert
                : pipeline != null && !IsPipelineSimple(pipeline);

            // Loop until user is satisfied.
            bool switchMode = true;
            while (switchMode) {
                if (advanced) {
                    using (AdvancedPipelinePluginForm editForm = new AdvancedPipelinePluginForm()) {
                        info = editForm.EditPlugin(owner, info, out switchMode);
                    }
                } else {
                    using (PipelinePluginForm editForm = new PipelinePluginForm()) {
                        info = editForm.EditPlugin(owner, info, out switchMode);
                    }
                }
                if (switchMode) {
                    advanced = !advanced;
                }
            }

            return info;
        }

        /// <summary>
        /// Custom equality comparer for <see cref="PipelineElement"/>
        /// that compares using class type only.
        /// </summary>
        private sealed class PipelineElementEqualityComparerByClassType : IEqualityComparer<PipelineElement>
        {
            /// <summary>
            /// Determines if two pipeline elements are equal. In our case,
            /// we check their class types.
            /// </summary>
            /// <param name="x">First element to compare.</param>
            /// <param name="y">Second element to compare.</param>
            /// <returns><c>true</c> if <paramref name="x"/> is the same
            /// class type as <paramref name="y"/>.</returns>
            public bool Equals(PipelineElement x, PipelineElement y)
            {
                if (x == null) {
                    throw new ArgumentNullException(nameof(x));
                }
                if (y == null) {
                    throw new ArgumentNullException(nameof(y));
                }

                return GetElementType(x).Equals(GetElementType(y));
            }

            /// <summary>
            /// Returns a hash code for a pipeline element.
            /// </summary>
            /// <param name="obj">Element to get hash code for.</param>
            /// <returns>Hash code.</returns>
            public int GetHashCode(PipelineElement obj)
            {
                if (obj == null) {
                    throw new ArgumentNullException(nameof(obj));
                }

                return GetElementType(obj).GetHashCode();
            }

            /// <summary>
            /// Returns the <see cref="Type"/> of the given pipeline element.
            /// We do some substitution behind the scenes for some element types.
            /// </summary>
            /// <param name="obj">Element to get type of.</param>
            /// <returns><see cref="Type"/> of <paramref name="obj"/>.</returns>
            private Type GetElementType(PipelineElement obj)
            {
                Debug.Assert(obj != null);

                // Some elements are mutually exclusive, so we'll consider them
                // the same type so that using Distinct can detect duplicates.
                Type type = obj.GetType();
                if (obj is OptionalQuotesPipelineElement) {
                    type = typeof(QuotesPipelineElement);
                } else if (obj is EncodeURIWhitespacePipelineElement) {
                    type = typeof(EncodeURICharsPipelineElement);
                } else if (obj is ForwardToBackslashesPipelineElement) {
                    type = typeof(BackToForwardSlashesPipelineElement);
                } else if (obj is FindReplacePipelineElement) {
                    type = typeof(RegexPipelineElement);
                } else if (obj is ExecutableWithFilelistPipelineElement) {
                    type = typeof(ExecutablePipelineElement);
                }
                return type;
            }
        }
    }
}
