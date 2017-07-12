// PCCEnvironment.cs
// (c) 2011-2017, Charles Lechasseur
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

namespace PathCopyCopy.Settings.Core
{
    /// <summary>
    /// Static class containing information pertaining to the environment in
    /// which this instance of Path Copy Copy is executing.
    /// </summary>
    public static class PCCEnvironment
    {
        /// <summary>
        /// Whether we're currently executing on a 64-bit OS.
        /// </summary>
        public static bool Is64BitOS
        {
            get {
                // Since this app is MSIL, we'll always run in 64-bit mode
                // on a 64-bit OS, so we can check IntPtr.Size.
                return IntPtr.Size == 8;
            }
        }

        /// <summary>
        /// Whether this instance of the Path Copy Copy settings is meant
        /// to edit the 32-bit or 64-bit settings. This is set at program launch.
        /// </summary>
        public static bool Is64Bit
        {
            get;
            internal set;
        }

        /// <summary>
        /// Whether this instance of the Path Copy Copy settings is meant
        /// to edit settings in the WOW64 emulation layer.
        /// </summary>
        public static bool IsWow64
        {
            get {
                return !Is64Bit && Is64BitOS;
            }
        }
    }
}
