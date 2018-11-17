// PCCSortedSet.cs
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
    /// Sorted set of unique elements.
    /// </summary>
    /// <typeparam name="T">Type of elements in the set.</typeparam>
    /// <remarks>
    /// If we move to .NET 4, we can get rid of this and use
    /// <c>System.Collection.Generic.SortedSet&lt;T&gt;</c>.
    /// </remarks>
    public class SortedSet<T> : ISet<T>, ICollection<T>, IEnumerable<T>, IEnumerable
    {
        /// Dictionary containing sorted elements, to simplify things.
        /// Value will always be 0.
        private SortedDictionary<T, int> dic = new SortedDictionary<T, int>();

        /// <summary>
        /// Number of elements in the set.
        /// </summary>
        public int Count
        {
            get {
                return dic.Count;
            }
        }

        /// <summary>
        /// Whether this is a read-only set.
        /// </summary>
        public bool IsReadOnly
        {
            get {
                // This class is never read-only.
                return false;
            }
        }

        /// <summary>
        /// Constructor. Creates an empty set.
        /// </summary>
        public SortedSet()
        {
        }

        /// <summary>
        /// Constructor with elements.
        /// </summary>
        /// <param name="items">Elements to add to the set.</param>
        public SortedSet(IEnumerable<T> items)
        {
            foreach (T item in items) {
                Add(item);
            }
        }

        /// <summary>
        /// Performs a set difference by removing all elements from
        /// <paramref name="items"/> from this set.
        /// </summary>
        /// <param name="items">Elements to remove from this set.</param>
        public void ExceptWith(IEnumerable<T> items)
        {
            foreach (T item in items) {
                Remove(item);
            }
        }

        /// <summary>
        /// Adds an item to the set.
        /// </summary>
        /// <param name="item">Item to add.</param>
        /// <returns><c>true</c> if item was added, <c>false</c> if
        /// item was already in the set.</returns>
        public bool Add(T item)
        {
            bool add = !dic.ContainsKey(item);
            if (add) {
                dic.Add(item, 0);
            }
            return add;
        }

        /// <summary>
        /// Adds an item to the set without checking if
        /// it was already there.
        /// </summary>
        /// <param name="item">Item to add.</param>
        void ICollection<T>.Add(T item)
        {
            dic.Add(item, 0);
        }

        /// <summary>
        /// Clears the set of all elements.
        /// </summary>
        public void Clear()
        {
            dic.Clear();
        }

        /// <summary>
        /// Checks whether set contains <paramref name="item"/>.
        /// </summary>
        /// <param name="item">Item to look for.</param>
        /// <returns><c>true</c> if <paramref name="item"/>
        /// is in the set.</returns>
        public bool Contains(T item)
        {
            return dic.ContainsKey(item);
        }

        /// <summary>
        /// Copies set elements to <paramref name="array"/>
        /// starting at index <paramref name="arrayIndex"/>.
        /// </summary>
        /// <param name="array">Array where to copy elements.
        /// Caller is responsible to ensure the array is large
        /// enough to receive all set elements.</param>
        /// <param name="arrayIndex">Index where to start
        /// copying the items in <paramref name="array"/>.</param>
        public void CopyTo(T[] array, int arrayIndex)
        {
            dic.Keys.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// Removes <paramref name="item"/> from the set.
        /// </summary>
        /// <param name="item">Item to remove.</param>
        /// <returns><c>true</c> if item was removed from the set,
        /// <c>false</c> if item was not in the set to begin with.</returns>
        public bool Remove(T item)
        {
            return dic.Remove(item);
        }

        /// <summary>
        /// Returns an enumerator for the items in the set.
        /// </summary>
        /// <returns>Enumerator for set elements.</returns>
        public IEnumerator<T> GetEnumerator()
        {
            return dic.Keys.GetEnumerator();
        }

        /// <summary>
        /// Returns a generic enumerator for the items in the set.
        /// </summary>
        /// <returns>Enumerator for set elements.</returns>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return dic.Keys.GetEnumerator();
        }
    }
}
