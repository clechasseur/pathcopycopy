// IPCCSet.cs
// (c) 2016-2018, Charles Lechasseur
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

using System.Collections;
using System.Collections.Generic;

namespace PathCopyCopy.Settings.Core.Base
{
    /// <summary>
    /// Interface for a set of unique elements. Minimalistic for this project.
    /// </summary>
    /// <typeparam name="T">Type of elements in the set.</typeparam>
    /// <remarks>
    /// If we move to .NET 4 we can get rid of this and use
    /// <c>System.Collection.Generic.ISet&lt;T&gt;</c>.
    /// </remarks>
    public interface ISet<T> : ICollection<T>, IEnumerable<T>, IEnumerable
    {
        /// <summary>
        /// Adds an item to the set.
        /// </summary>
        /// <param name="item">Item to add.</param>
        /// <returns><c>true</c> if item was added, <c>false</c> if
        /// item was already in the set.</returns>
        new bool Add(T item);

        /// <summary>
        /// Performs a set difference by removing all elements in
        /// <paramref name="items"/> from this set.
        /// </summary>
        /// <param name="items">Elements to remove from this set.</param>
        void ExceptWith(IEnumerable<T> items);
    }
}
