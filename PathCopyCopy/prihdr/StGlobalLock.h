// StGlobalLock.h
// (c) 2008-2020, Charles Lechasseur
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
// StGlobalLock
//
// Locks a moveable memory block allocated via GlobalAlloc, and unlocks
// it when object goes out of scope.
//
class StGlobalLock final
{
public:
                        //
                        // Constructor.
                        // Locks the given moveable memory block.
                        // Pointer to the block can be fetched by calling GetPtr.
                        // If GetPtr returns null, it means an error occured;
                        // call GetLastError to get more information.
                        //
                        // @param p_hMem Handle of memory block to lock.
                        //
    explicit            StGlobalLock(HANDLE const p_hMem) noexcept
                            : m_hMem(p_hMem),
                              m_pMem(::GlobalLock(m_hMem))
                        {
                        }

                        //
                        // Copying/moving not supported.
                        //
                        StGlobalLock(const StGlobalLock&) = delete;
                        StGlobalLock(StGlobalLock&&) = delete;
    StGlobalLock&       operator=(const StGlobalLock&) = delete;
    StGlobalLock&       operator=(StGlobalLock&&) = delete;

                        //
                        // Destructor.
                        // Unlocks the memory block if it was successfully locked
                        // by the constructor.
                        //
                        ~StGlobalLock()
                        {
                            if (m_pMem != nullptr) {
                                ::GlobalUnlock(m_hMem);
                            }
                        }

                        //
                        // Returns the pointer to the memory block. Accessible freely
                        // while the block is locked. If this returns null, call GetLastError
                        // to get more information about the error.
                        //
                        // @return Block pointer.
                        //
    void*               GetPtr() const noexcept
                        {
                            return m_pMem;
                        }

private:
    HANDLE const        m_hMem;     // Memory block that we locked.
    void* const         m_pMem;     // Pointer to memory block, accessible while we're locked.
};