// SampleCOMPlugin.cs
// (c) 2015-2017, Charles Lechasseur
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
using System.Runtime.InteropServices;
using System.Text;
using PathCopyCopy.API;

namespace SampleCOMPlugin.CSharp
{
    /// <summary>
    /// Sample Path Copy Copy COM plugin implemented in C#.
    /// </summary>
    [Guid("7FAB47CD-E79D-46D9-AA67-C6AC59759757"), ComVisible(true), ClassInterface(ClassInterfaceType.None)]
    public sealed class SampleCOMPlugin : IPathCopyCopyPlugin, IPathCopyCopyPluginGroupInfo, IPathCopyCopyPluginStateInfo
    {
        /// <summary>
        /// Function called when our class library is registered. We use this
        /// opportunity to register our plugin in Path Copy Copy.
        /// </summary>
        /// <param name="t">Type registered; unused.</param>
        [ComRegisterFunction]
        public static void RegisterSampleCOMPlugin(Type t)
        {
            // We need to use the ContextMenuExt COM object for this.
            PathCopyCopyContextMenuExtClass ext = new PathCopyCopyContextMenuExtClass();
            try {
                Guid ourGuid = GetGuidForThisClass();
                ext.RegisterPlugin(ref ourGuid);
            } finally {
                Marshal.ReleaseComObject(ext);
            }
        }

        /// <summary>
        /// Function called when our class library is unregistered. We need to
        /// remove ourselves from Path Copy Copy's list of plugins.
        /// </summary>
        /// <param name="t">Type registered; unused.</param>
        [ComUnregisterFunction]
        public static void UnregisterSampleCOMPlugin(Type t)
        {
            // We need to use the ContextMenuExt COM object for this.
            PathCopyCopyContextMenuExtClass ext = new PathCopyCopyContextMenuExtClass();
            try {
                Guid ourGuid = GetGuidForThisClass();
                ext.UnregisterPlugin(ref ourGuid);
            } finally {
                Marshal.ReleaseComObject(ext);
            }
        }

        /// <summary>
        /// Returns the GUID of this class, as defined by its <see cref="GuidAttribute"/>.
        /// </summary>
        /// <returns>GUID of this class.</returns>
        private static Guid GetGuidForThisClass()
        {
            object[] guidAttributes = typeof(SampleCOMPlugin).GetCustomAttributes(typeof(GuidAttribute), false);
            Debug.Assert(guidAttributes != null);
            if (guidAttributes.Length != 1) {
                throw new NotSupportedException();
            }
            Debug.Assert(guidAttributes[0] is GuidAttribute);
            return new Guid((guidAttributes[0] as GuidAttribute).Value);
        }

        #region IPathCopyCopyPlugin Members

        /// <summary>
        /// Returns the plugin description, to be used to show the plugin in the
        /// Path Copy Copy contextual menu.
        /// </summary>
        public string Description
        {
            get {
                return "Sample C# COM Plugin";
            }
        }

        /// <summary>
        /// Returns a help text to display in the Windows Explorer status bar
        /// when the cursor is over this plugin in the contextual menu.
        /// </summary>
        public string HelpText
        {
            get {
                return "This is a sample Path Copy Copy plugin implemented as a C# COM object. Will return the path unmodified.";
            }
        }

        /// <summary>
        /// Returns the path as modified by this plugin.
        /// </summary>
        /// <param name="p_pPath">Path to the file/folder.</param>
        /// <returns>Modified path to copy to the clipboard.</returns>
        public string GetPath(string p_pPath)
        {
            if (p_pPath == null) {
                throw new ArgumentNullException("p_pPath");
            }
            return p_pPath;
        }

        #endregion

        #region IPathCopyCopyPluginGroupInfo Members

        /// <summary>
        /// ID of the group this plugin belongs to.
        /// </summary>
        /// <remarks>
        /// See http://pathcopycopy.codeplex.com/documentation for more info.
        /// </remarks>
        public uint GroupId
        {
            get {
                return 0x5A4781E0;
            }
        }

        /// <summary>
        /// Position of the plugin within its group.
        /// </summary>
        public uint GroupPosition
        {
            get {
                // 0-based; the C++ sample plugin comes first.
                return 1;
            }
        }

        #endregion

        #region IPathCopyCopyPluginStateInfo Members

        /// <summary>
        /// Checks if the plugin should be enabled in the contextual menu.
        /// </summary>
        /// <param name="p_pParentPath">Path to the parent directory of
        /// <paramref name="p_pFile"/>.</param>
        /// <param name="p_pFile">Path to a file/folder for which we need
        /// to copy the path.</param>
        /// <returns><c>true</c> if plugin should be enabled, <c>false</c> to
        /// have its contextual menu item grayed out.</returns>
        public bool Enabled(string p_pParentPath, string p_pFile)
        {
            return true;
        }

        #endregion
    }
}
