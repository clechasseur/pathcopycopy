// StOleStr.h
// (c) 2011-2020, Charles Lechasseur
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
// StOleStr
//
// Wrapper for a Windows OLESTR pointer. Designed to be used in a block of code.
// Will automatically free the OLESTR in the destructor using CoTaskMemFree.
//
class StOleStr final
{
public:
                        //
                        // Default constructor.
                        // Initializes a null string.
                        //
                        StOleStr() noexcept
                            : m_pString(nullptr)
                        {
                        }

                        //
                        // Constructor that acquires a given OLESTR pointer.
                        //
                        // @param p_pString String pointer to acquire. Must have been created using CoTaskMemAlloc.
                        //
    explicit            StOleStr(LPOLESTR const p_pString) noexcept
                            : m_pString(p_pString)
                        {
                        }

                        //
                        // Copying/moving not supported.
                        //
                        StOleStr(const StOleStr&) = delete;
                        StOleStr(StOleStr&&) = delete;
    StOleStr&           operator=(const StOleStr&) = delete;
    StOleStr&           operator=(StOleStr&&) = delete;

                        //
                        // Destructor. Frees the OLESTR if set.
                        //
                        ~StOleStr()
                        {
                            if (m_pString != nullptr) {
                                ::CoTaskMemFree(m_pString);
                            }
                        }

                        //
                        // Returns a reference to the internal OLESTR pointer.
                        //
    LPOLESTR&           Get() noexcept
                        {
                            return m_pString;
                        }

                        //
                        // Returns a reference to the internal OLESTR pointer.
                        //
                        operator LPOLESTR() noexcept
                        {
                            return m_pString;
                        }

                        //
                        // Returns the address of the internal OLESTR pointer.
                        // Designed to be used for API calls like this:
                        //
                        // StOleStr olestr;
                        // StringFromCLSID(clsid, &olestr);
                        //
    LPOLESTR*           operator&() noexcept
                        {
                            return &m_pString;
                        }

private:
    LPOLESTR            m_pString;      // String content. Can be null.
};
