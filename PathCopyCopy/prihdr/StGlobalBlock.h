// StGlobalLock.h
// (c) 2008-2019, Charles Lechasseur
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

#pragma once

#include <windows.h>


//
// StGlobalBlock
//
// Assumes ownership of a handle allocated via GlobalAlloc.
// The handle is destroyed via GlobalFree when object goes out of scope.
//
class StGlobalBlock final
{
public:
                        //
                        // Constructor.
                        // Allocates a new memory block using GlobalAlloc and assumes ownership.
                        //
                        // @param p_Flags Flags to pass to GlobalAlloc. See MSDN for details.
                        // @param p_Size Size of memory block to allocate.
                        //
                        StGlobalBlock(const UINT p_Flags, const SIZE_T p_Size)
                            : m_hBlock(NULL)
                        {
                            Connect(::GlobalAlloc(p_Flags, p_Size));
                        }

                        //
                        // Constructor.
                        // Assumes ownership of the given memory block.
                        // Block must have been allocated via GlobalAlloc.
                        //
                        // @param p_hBlock Handle of memory block to acquire.
                        //
    explicit            StGlobalBlock(HANDLE p_hBlock)
                            : m_hBlock(NULL)
                        {
                            Connect(p_hBlock);
                        }

                        //
                        // Copying not supported.
                        //
                        StGlobalBlock(const StGlobalBlock&) = delete;
    StGlobalBlock&      operator=(const StGlobalBlock&) = delete;

                        //
                        // Destructor.
                        // Frees our memory block, if any.
                        //
                        ~StGlobalBlock()
                        {
                            Disconnect();
                        }

                        //
                        // Returns the handle of our owned memory block.
                        //
                        // @return Memory block handle.
                        //
    HANDLE              Get() const
                        {
                            return m_hBlock;
                        }

                        //
                        // Assumes ownership of the given memory block,
                        // freeing any existing owned block. Block handle
                        // must have been allocated using GlobalAlloc.
                        //
                        // @param p_hBlock Handle of block to acquire.
                        //
    void                Acquire(HANDLE p_hBlock)
                        {
                            Connect(p_hBlock);
                        }

                        //
                        // Releases ownership of the memory block we own
                        // without freeing it. Caller is then responsible
                        // for freeing the memory block using GlobalFree.
                        //
                        // @return Handle of memory block. Caller assumes ownership.
                        //
    HANDLE              Release()
                        {
                            HANDLE hBlock = m_hBlock;
                            m_hBlock = NULL;
                            return hBlock;
                        }

private:
    HANDLE              m_hBlock;   // Handle of owned memory block.

                        //
                        // Connects to the given block handle, assuming its ownership.
                        // If we already owned a previous block, it is freed.
                        //
                        // @param p_hBlock Block to assume ownership of.
                        //
    void                Connect(HANDLE p_hBlock)
                        {
                            if (p_hBlock != m_hBlock) {
                                Disconnect();
                                m_hBlock = p_hBlock;
                            }
                        }

                        //
                        // Disconnects from our owned memory block, if any,
                        // by freeing it and setting it to 0.
                        //
    void                Disconnect()
                        {
                            if (m_hBlock != NULL) {
                                ::GlobalFree(m_hBlock);
                            }
                            m_hBlock = NULL;
                        }
};
