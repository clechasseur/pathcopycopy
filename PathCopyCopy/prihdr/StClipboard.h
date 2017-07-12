// StClipboard.h
// (c) 2008-2017, Charles Lechasseur
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
// StClipboard
//
// Stack-based class that opens and empties the clipboard when created
// and takes care of closing the clipboard when destroyed.
//
class StClipboard final
{
public:
                        //
                        // Constructor.
                        // Opens and empties the clipboard, giving clipboard ownership
                        // to the specified window. The result of the initialization can
                        // be inspected through the InitResult method; if that method returns
                        // false, use the Windows API GetLastError to know what happened.
                        //
                        // @param p_hOwnerWnd Handle of window that will become the clipboard owner.
                        //
    explicit            StClipboard(HWND p_hOwnerWnd)
                            : m_Opened(::OpenClipboard(p_hOwnerWnd) != FALSE),
                              m_Result(m_Opened)
                        {
                            if (m_Result) {
                                m_Result = (::EmptyClipboard() != FALSE);
                            }
                        }

                        //
                        // Copying not supported.
                        //
                        StClipboard(const StClipboard&) = delete;
    StClipboard&        operator=(const StClipboard&) = delete;

                        //
                        // Destructor.
                        // Closes the clipboard if it was successfully opened in the constructor.
                        //
                        ~StClipboard()
                        {
                            if (m_Opened) {
                                ::CloseClipboard();
                            }
                        }

                        //
                        // Returns the result of the clipboard initialization performed
                        // by the constructor. If this returns false, the clipboard has
                        // not been acquired by the window and GetLastError can give more
                        // detailed error info.
                        //
                        // @return Clipboard initialization result.
                        //
    bool                InitResult() const
                        {
                            return m_Result;
                        }

private:
    bool                m_Opened;   // Whether clipboard has been opened.
    bool                m_Result;   // Result of clipboard initialization.
};
