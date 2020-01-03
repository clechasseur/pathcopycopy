// StCoInitialize.h
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
// StCoInitialize
//
// Stack-based class to register COM in a block of code.
// Destructor will take care of uninitializing.
//
class StCoInitialize final
{
public:
                        //
                        // Default constructor. Will call CoInitialize without arguments.
                        //
                        StCoInitialize() noexcept(false)
                            : m_InitRes(::CoInitialize(nullptr))
                        {
                        }

                        //
                        // Constructor with initialization flags. Will call CoInitializeEx.
                        //
                        // @param p_InitFlags Initialization flags. See MSDN for details.
                        //
    explicit            StCoInitialize(const DWORD p_InitFlags) noexcept(false)
                            : m_InitRes(::CoInitializeEx(nullptr, p_InitFlags))
                        {
                        }

                        //
                        // Copying/moving not supported.
                        //
                        StCoInitialize(const StCoInitialize&) = delete;
                        StCoInitialize(StCoInitialize&&) = delete;
    StCoInitialize&     operator=(const StCoInitialize&) = delete;
    StCoInitialize&     operator=(StCoInitialize&&) = delete;

                        //
                        // Destructor. Will call CoUninitialize if the constructor
                        // successfully initialized COM.
                        //
                        ~StCoInitialize()
                        {
                            if (SUCCEEDED(m_InitRes)) {
                                ::CoUninitialize();
                            }
                        }

                        //
                        // Returns the result of the CoInitialize(Ex) call performed
                        // by the constructor. If this returns an error code, then
                        // COM was not properly initialized.
                        //
                        // @return Initialization result.
                        //
    HRESULT             GetInitResult() const noexcept
                        {
                            return m_InitRes;
                        }

private:
    const HRESULT       m_InitRes;      // Result of the CoInit call.
};
