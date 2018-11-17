// PCCSortedKeyedSet.cs
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
using System.Diagnostics;

namespace PathCopyCopy.Settings.Core.Base
{
    /// <summary>
    /// Sorted set of unique elements identified by a key.
    /// Uses a delegate to fetch the key of each element.
    /// </summary>
    /// <typeparam name="K">Type of key identifying each element.</typeparam>
    /// <typeparam name="T">Type of elements in the set.</typeparam>
    public class SortedKeyedSet<K, T> : IKeyedSet<K, T>, ISet<T>, ICollection<T>, IEnumerable<T>, IEnumerable
    {
        /// <summary>
        /// Delegate used to fetch a key of type <typeparamref name="K"/>
        /// for an item of type <typeparamref name="T"/>.
        /// </summary>
        /// <param name="item">Item to get a key for.</param>
        /// <returns>Key of <paramref name="item"/>.</returns>
        public delegate K GetKeyDelegate(T item);

        /// Dictionary used to store items, sorted by key.
        private SortedDictionary<K, T> dic = new SortedDictionary<K, T>();

        /// Delegate used to extract keys for items.
        private GetKeyDelegate vtok;

        /// <summary>
        /// Number of items in the set.
        /// </summary>
        public int Count
        {
            get {
                return dic.Count;
            }
        }

        /// <summary>
        /// Whether this set is read-only.
        /// </summary>
        public bool IsReadOnly
        {
            get {
                // This class is never read-only.
                return false;
            }
        }

        /// <summary>
        /// Default constructor. Creates an empty set.
        /// </summary>
        /// <param name="vtok">Delegate used by the set to get keys for
        /// items. Cannot be <c>null</c>.</param>
        public SortedKeyedSet(GetKeyDelegate vtok)
        {
            Debug.Assert(vtok != null);

            this.vtok = vtok;
        }

        /// <summary>
        /// Constructor with initial items.
        /// </summary>
        /// <param name="vtok">Delegate used by the set to get keys for
        /// items. Cannot be <c>null</c>.</param>
        /// <param name="items">Items to add to the set initially.</param>
        public SortedKeyedSet(GetKeyDelegate vtok, IEnumerable<T> items)
        {
            Debug.Assert(vtok != null);
            Debug.Assert(items != null);

            this.vtok = vtok;
            foreach (T item in items) {
                Add(item);
            }
        }

        /// <summary>
        /// Check if the set contains an element whose key is <paramref name="key"/>.
        /// </summary>
        /// <param name="key">Key of element to look for.</param>
        /// <returns><c>true</c> if set contains an element whose key
        /// is <paramref name="key"/>, otherwise <c>false</c>.</returns>
        public bool ContainsKey(K key)
        {
            return dic.ContainsKey(key);
        }

        /// <summary>
        /// Looks for an element whose key is <paramref name="key"/> in the set
        /// and returns it if found.
        /// </summary>
        /// <param name="key">Key of element to look for.</param>
        /// <returns>Element whose key is <paramref name="key"/> found in
        /// the set, or the default value for <typeparamref name="T"/> if
        /// not found.</returns>
        public T FindKey(K key)
        {
            T item;
            dic.TryGetValue(key, out item);
            return item;
        }

        /// <summary>
        /// Looks for an element whose key is <paramref name="key"/> in the set
        /// and stores it in <paramref name="item"/> if found.
        /// </summary>
        /// <param name="key">Key of element to look for.</param>
        /// <param name="item">Upon exit, if method returned <c>true</c>, will
        /// contain the element found, otherwise will contain the default value
        /// for type <typeparamref name="T"/>.</param>
        /// <returns><c>true</c> if element was found in set and stored in
        /// <paramref name="item"/>, otherwise <c>false</c>.</returns>
        public bool FindKey(K key, out T item)
        {
            return dic.TryGetValue(key, out item);
        }

        /// <summary>
        /// Removes an element from the set using its key.
        /// </summary>
        /// <param name="key">Key of element to remove.</param>
        /// <returns><c>true</c> if element was in the set and was
        /// removed, otherwise <c>false</c>.</returns>
        public bool RemoveKey(K key)
        {
            return dic.Remove(key);
        }

        /// <summary>
        /// Adds an item to the set.
        /// </summary>
        /// <param name="item">Item to add.</param>
        /// <returns><c>true</c> if item was added, <c>false</c> if
        /// item was already in the set.</returns>
        public bool Add(T item)
        {
            K key = vtok(item);
            bool add = !dic.ContainsKey(key);
            if (add) {
                dic.Add(key, item);
            }
            return add;
        }

        /// <summary>
        /// Performs a set difference by removing all elements in
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
        /// Adds an item to the set without checking if it worked.
        /// </summary>
        /// <param name="item">Item to add.</param>
        void ICollection<T>.Add(T item)
        {
            K key = vtok(item);
            if (!dic.ContainsKey(key)) {
                dic.Add(key, item);
            }
        }

        /// <summary>
        /// Removes all items from the set.
        /// </summary>
        public void Clear()
        {
            dic.Clear();
        }

        /// <summary>
        /// Checks if the set contains an item whose key is
        /// equivalent to the key of <paramref name="item"/>.
        /// </summary>
        /// <param name="item">Item to look for.</param>
        /// <returns><c>true</c> if set contains an item whose key
        /// is equivalent to the key of <paramref name="item"/>,
        /// otherwise <c>false</c>.</returns>
        public bool Contains(T item)
        {
            return dic.ContainsKey(vtok(item));
        }

        /// <summary>
        /// Copies all items in the set into an array.
        /// </summary>
        /// <param name="array">Array where to copy items.</param>
        /// <param name="arrayIndex">Index where to start copying
        /// in <paramref name="array"/>.</param>
        public void CopyTo(T[] array, int arrayIndex)
        {
            dic.Values.CopyTo(array, arrayIndex);
        }

        /// <summary>
        /// Removes an item from the set.
        /// </summary>
        /// <param name="item">Item to remove.</param>
        /// <returns><c>true</c> if item was in the set and has been removed,
        /// <c>false</c> if item was not in the set.</returns>
        public bool Remove(T item)
        {
            return dic.Remove(vtok(item));
        }

        /// <summary>
        /// Returns an enumerator for the plugins in the set.
        /// </summary>
        /// <returns>Enumerator for plugins in set.</returns>
        public IEnumerator<T> GetEnumerator()
        {
            return dic.Values.GetEnumerator();
        }

        /// <summary>
        /// Returns a generic enumerator for the plugins in the set.
        /// </summary>
        /// <returns>Generic enumerator for plugins in set.</returns>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return dic.Values.GetEnumerator();
        }
    }
}
