// CopyToClipboardPathAction.cpp
// (c) 2017-2019, Charles Lechasseur
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

#include <stdafx.h>
#include <CopyToClipboardPathAction.h>

#include <StClipboard.h>
#include <StGlobalBlock.h>
#include <StGlobalLock.h>

#include <string.h>
#include <windows.h>


namespace PCC
{
    namespace Actions
    {
        //
        // Copies the given paths to the clipboard.
        //
        // @param p_Paths Path or paths to copy to the clipboard,
        //                pre-bundled in a single string.
        // @param p_hWnd Parent window handle, if needed.
        //
        void CopyToClipboardPathAction::Act(const std::wstring& p_Paths,
                                            const HWND          p_hWnd) const
        {
            // Now store the copied paths in the clipboard.
            StClipboard acquireClipboard(p_hWnd);
            if (!acquireClipboard.InitResult()) {
                throw CopyToClipboardException();
            }

            // Allocate global block to store text.
            const size_t blockNumElements = p_Paths.size() + 1;
            const size_t blockSize = blockNumElements * sizeof(wchar_t);
            StGlobalBlock memBlock(GMEM_MOVEABLE, blockSize);
            if (memBlock.Get() == NULL) {
                throw CopyToClipboardException();
            }

            // Lock block and copy text in it.
            {
                StGlobalLock lockBlock(memBlock.Get());
                void* pBlock = lockBlock.GetPtr();
                if (pBlock == nullptr) {
                    throw CopyToClipboardException();
                }

                errno_t copyErr = ::wcscpy_s(static_cast<wchar_t*>(pBlock),
                                             blockNumElements,
                                             p_Paths.c_str());
                if (copyErr != 0) {
                    // Could not copy data in block.
                    throw CopyToClipboardException();
                }
            }

            // Save data in clipboard.
            HANDLE hSavedData = ::SetClipboardData(CF_UNICODETEXT, memBlock.Get());
            if (hSavedData != NULL) {
                // Clipboard now owns the data, avoid freeing it.
                memBlock.Release();
            } else {
                // Could not save clipboard data.
                throw CopyToClipboardException();
            }
        }

        //
        // Returns a textual description of the exception.
        //
        // @return Exception textual description.
        //
        const char* CopyToClipboardException::what() const
        {
            return "CopyToClipboardException";
        }

    } // namespace Actions

} // namespace PCC
