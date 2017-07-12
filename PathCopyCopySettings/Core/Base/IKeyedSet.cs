// IPCCKeyedSet.cs
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

namespace PathCopyCopy.Settings.Core.Base
{
    /// <summary>
    /// Interface for a set of unique elements identified by a key.
    /// Minimalistic for this project.
    /// </summary>
    /// <typeparam name="K">Type of key identifying each element.</typeparam>
    /// <typeparam name="T">Type of elements in the set.</typeparam>
    public interface IKeyedSet<K, T> : ISet<T>
    {
        /// <summary>
        /// Check if the set contains an element whose key is <paramref name="key"/>.
        /// </summary>
        /// <param name="key">Key of element to look for.</param>
        /// <returns><c>true</c> if set contains an element whose key
        /// is <paramref name="key"/>, otherwise <c>false</c>.</returns>
        bool ContainsKey(K key);

        /// <summary>
        /// Looks for an element whose key is <paramref name="key"/> in the set
        /// and returns it if found.
        /// </summary>
        /// <param name="key">Key of element to look for.</param>
        /// <returns>Element whose key is <paramref name="key"/> found in
        /// the set, or the default value for <typeparamref name="T"/> if
        /// not found.</returns>
        T FindKey(K key);

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
        bool FindKey(K key, out T item);

        /// <summary>
        /// Removes an element from the set using its key.
        /// </summary>
        /// <param name="key">Key of element to remove.</param>
        /// <returns><c>true</c> if element was in the set and was
        /// removed, otherwise <c>false</c>.</returns>
        bool RemoveKey(K key);
    }
}
