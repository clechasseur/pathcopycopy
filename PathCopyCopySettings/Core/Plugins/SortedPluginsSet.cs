// SortedPluginsSet.cs
// (c) 2016-2017, Charles Lechasseur
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
using PathCopyCopy.Settings.Core.Base;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// Sorted set of <see cref="Plugin"/>s, ordered by their <see cref="Plugin.Id"/>s.
    /// </summary>
    public sealed class SortedPluginsSet : SortedKeyedSet<Guid, Plugin>
    {
        /// <summary>
        /// Constructor. Creates an empty set.
        /// </summary>
        public SortedPluginsSet()
            : base(p => p.Id)
        {
        }

        /// <summary>
        /// Constructor with plugins.
        /// </summary>
        /// <param name="items">Plugins to add to the set.</param>
        public SortedPluginsSet(IEnumerable<Plugin> items)
            : base(p => p.Id, items)
        {
        }
    }
}
