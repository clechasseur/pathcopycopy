// LanguageDisplayInfo.cs
// (c) 2020-2021, Charles Lechasseur
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

namespace PathCopyCopy.Settings.UI.Utils
{
    /// <summary>
    /// Bean containing information about an application display language.
    /// </summary>
    public sealed class LanguageDisplayInfo
    {
        /// <summary>
        /// Name of the language, as stored in the settings, e.g. "en-CA".
        /// </summary>
        public string Name
        {
            get;
            set;
        }

        /// <summary>
        /// Display name of the language, shown in the UI.
        /// </summary>
        public string DisplayName
        {
            get;
            set;
        }

        /// <summary>
        /// Returns a string representation of the object. This will be used
        /// to display objects of this type in the drop-down in the UI.
        /// </summary>
        /// <returns>Language display name.</returns>
        public override string ToString()
        {
            return DisplayName;
        }
    }
}
