// CopyToClipboardPathAction.h
// (c) 2017-2020, Charles Lechasseur
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

#include <PathAction.h>

#include <exception>


namespace PCC
{
    namespace Actions
    {
        //
        // CopyToClipboardPathAction
        //
        // Path action that copies all paths to the clipboard.
        //
        class CopyToClipboardPathAction final : public PCC::PathAction
        {
        public:
                                    CopyToClipboardPathAction() = default;
                                    CopyToClipboardPathAction(const CopyToClipboardPathAction&) = delete;
                                    CopyToClipboardPathAction(CopyToClipboardPathAction&&) = delete;
            CopyToClipboardPathAction&
                                    operator=(const CopyToClipboardPathAction&) = delete;
            CopyToClipboardPathAction&
                                    operator=(CopyToClipboardPathAction&&) = delete;

            void                    Act(const std::wstring& p_Paths,
                                        HWND p_hWnd) const override;
        };

        //
        // CopyToClipboardException
        //
        // Exception thrown when an error occurs while copying data to the clipboard.
        //
        class CopyToClipboardException : public std::exception
        {
        public:
            const char*             what() const noexcept(false) override;
        };

    } // namespace Actions

} // namespace PCC
