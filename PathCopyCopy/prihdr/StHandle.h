// StHandle.h
// (c) 2020, Charles Lechasseur
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
// StHandle
//
// Wrapper for a Windows HANDLE. Designed to be used in a block of code.
// Will automatically dispose of the handle using CloseHandle.
//
class StHandle final
{
public:
                        //
                        // Default constructor.
                        // Initializes a null handle.
                        //
                        StHandle() noexcept = default;

                        //
                        // Constructor with a handle.
                        // We assume ownership of p_hContent.
                        //
                        // @param p_hContent Handle. Can be null.
                        //
                        StHandle(HANDLE const p_hContent) noexcept
                        {
                            Assign(p_hContent);
                        }

                        //
                        // Assignment operator with a handle.
                        // We assume ownership of p_hContent.
                        // Any existing wrapped handle is closed.
                        //
                        // @param p_hContent Handle. Can be null.
                        // @return Reference to this.
                        //
    StHandle&           operator=(HANDLE const p_hContent) noexcept
                        {
                            Assign(p_hContent);
                            return *this;
                        }

                        //
                        // Copying/moving not supported.
                        //
                        StHandle(const StHandle&) = delete;
                        StHandle(StHandle&&) = delete;
    StHandle&           operator=(const StHandle&) = delete;
    StHandle&           operator=(StHandle&&) = delete;

                        //
                        // Destructor. Closes our handle if we have one.
                        //
                        ~StHandle()
                        {
                            Dispose();
                        }

                        //
                        // Returns our wrapped handle.
                        //
                        // @return Wrapped handle. Can be null.
                        //
    HANDLE              Get() const noexcept
                        {
                            return m_hContent;
                        }

                        //
                        // Returns our wrapped handle.
                        //
                        // @return Wrapped handle. Can be null.
                        //
                        operator HANDLE() const noexcept
                        {
                            return m_hContent;
                        }

                        //
                        // Checks if our wrapped handle is null.
                        //
                        // @return Whether our wrapped handle is null.
                        //
    bool                operator==(std::nullptr_t) const noexcept
                        {
                            return m_hContent == nullptr;
                        }

                        //
                        // Checks if our wrapped handle is non-null.
                        //
                        // @return Whether our wrapped handle is non-null.
                        //
    bool                operator!=(std::nullptr_t) const noexcept
                        {
                            return m_hContent != nullptr;
                        }

private:
    HANDLE              m_hContent = nullptr;   // Wrapped handle.

                        //
                        // Assigns a new handle value.
                        // Any existing wrapped handle is closed.
                        //
                        // @param p_hContent New handle. Can be null.
                        //
    void                Assign(HANDLE const p_hContent) noexcept
                        {
                            Dispose();
                            m_hContent = p_hContent;
                        }

                        //
                        // Closes our handle if we have one.
                        //
    void                Dispose() noexcept
                        {
                            if (m_hContent != nullptr) {
                                ::CloseHandle(m_hContent);
                                m_hContent = nullptr;
                            }
                        }
};
