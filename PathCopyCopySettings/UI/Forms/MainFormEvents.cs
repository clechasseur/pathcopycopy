// MainFormEvents.cs
// (c) 2012-2017, Charles Lechasseur
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

namespace PathCopyCopy.Settings.UI.Forms
{
    /// <summary>
    /// Arguments class for events that concern a pipeline plugin in the main
    /// form. Includes reference to the index of the plugin.
    /// </summary>
    public class PipelinePluginEventArgs : EventArgs
    {
        /// <summary>
        /// Index of the pipeline plugin concerned, relative to the first
        /// pipeline plugin in the list.
        /// </summary>
        public int PluginIndex
        {
            get;
            protected set;
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="pluginIndex">Index of pipeline plugin concerned.</param>
        public PipelinePluginEventArgs(int pluginIndex)
            : base()
        {
            this.PluginIndex = pluginIndex;
        }
    }
    
    /// <summary>
    /// Argument class for events raised when (a) pipeline plugin(s) is(are)
    /// added in the main form. Includes a counter to specify the number of
    /// plugins added.
    /// </summary>
    public class PipelinePluginAddEventArgs : PipelinePluginEventArgs
    {
        /// <summary>
        /// Number of plugins added. Parent's <see cref="PluginIndex"/>
        /// is the index of the first plugin added.
        /// </summary>
        public int Count
        {
            get;
            protected set;
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="pluginIndex">Index of first pipeline plugin added.</param>
        /// <param name="count">Number of plugins added.</param>
        public PipelinePluginAddEventArgs(int pluginIndex, int count)
            : base(pluginIndex)
        {
            this.Count = count;
        }
    }
    
    /// <summary>
    /// Arguments class for events triggered when two pipeline plugins switch
    /// positions in the main form.
    /// </summary>
    public class PipelinePluginSwitchEventArgs : PipelinePluginEventArgs
    {
        /// <summary>
        /// Index of the second plugin with which the first plugin switches place.
        /// </summary>
        /// <seealso cref="PipelinePluginEventArgs.PluginIndex"/>
        public int SecondPluginIndex
        {
            get;
            protected set;
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="firstPluginIndex">Index of first pipeline plugin.</param>
        /// <param name="secondPluginIndex">Index of second pipeline plugin.</param>
        public PipelinePluginSwitchEventArgs(int firstPluginIndex, int secondPluginIndex)
            : base(firstPluginIndex)
        {
            this.SecondPluginIndex = secondPluginIndex;
        }
    }
    
    /// <summary>
    /// Arguments class for events triggered when pipeline plugins are imported.
    /// </summary>
    public class PipelinePluginsImportedEventArgs : EventArgs
    {
        /// <summary>
        /// Number of pipeline plugins that were imported.
        /// </summary>
        public int Count
        {
            get;
            protected set;
        }

        /// <summary>
        /// Number of pipeline plugins that were imported over existing ones.
        /// Note that these are also included in <see cref="Count"/>.
        /// </summary>
        public int OverwritesCount
        {
            get;
            protected set;
        }

        /// <summary>
        /// Number of pipeline plugins that were newly imported (e.g., that
        /// did not overwrite existing ones). Note that these are also
        /// included in <see cref="Count"/>.
        /// </summary>
        public int NewCount
        {
            get {
                return Count - OverwritesCount;
            }
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="count">Number of plugins imported.</param>
        /// <param name="overwritesCount">Number of plugins imported over existing
        /// ones. Also include those in <paramref name="count"/>.</param>
        public PipelinePluginsImportedEventArgs(int count, int overwritesCount)
            : base()
        {
            Debug.Assert(overwritesCount <= count);

            this.Count = count;
            this.OverwritesCount = overwritesCount;
        }
    }
}
