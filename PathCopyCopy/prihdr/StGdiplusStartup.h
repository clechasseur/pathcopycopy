// StCoInitialize.h
// (c) 2014-2017, Charles Lechasseur
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
#include <objidl.h>
#include <gdiplus.h>


//
// StGdiplusStartup
//
// Stack-based class that initializes the GDI+ library in a block of code.
// Destructor will take care of shutting down GDI+.
//
class StGdiplusStartup final
{
public:
                        //
                        // Constructor. Initializes GDI+ with the specified parameters.
                        // The default values are the same as those of the GdiplusStartupInput struct;
                        // see MSDN for details.
                        //
                        // @param p_pDebugEventCallback Pointer to optional debug callback. GDI+ will
                        //                              call this function to provide debug info. [default: nullptr]
                        // @param p_SuppressExternalCodecs Whether to suppress loading external codecs.
                        //                                 MSDN says this is unused. [default: false]
                        //
    explicit            StGdiplusStartup(Gdiplus::DebugEventProc const p_pDebugEventCallback = nullptr,
                                         const bool p_SuppressExternalCodecs = false)
                            : m_StartupInput(p_pDebugEventCallback, FALSE, p_SuppressExternalCodecs ? TRUE : FALSE),
                              m_Token(0),
                              m_StartupRes(Gdiplus::GdiplusStartup(&m_Token, &m_StartupInput, nullptr))
                        {
                        }

                        //
                        // Copying not supported.
                        //
                        StGdiplusStartup(const StGdiplusStartup&) = delete;
    StGdiplusStartup&   operator=(const StGdiplusStartup&) = delete;

                        //
                        // Destructor. Shuts down the GDI+ library that was initialized
                        // by the constructor.
                        //
                        ~StGdiplusStartup()
                        {
                            if (m_StartupRes == Gdiplus::Ok) {
                                Gdiplus::GdiplusShutdown(m_Token);
                            }
                        }

                        //
                        // Returns the result of the call to GdiplusStartup in constructor.
                        //
                        // @return Startup result, as a GDI+ Status enum value.
                        //
    Gdiplus::Status     StartupResult() const
                        {
                            return m_StartupRes;
                        }

                        //
                        // Checks if the call to GdiplusStartup in constructor was successful.
                        //
                        // @return true if GDI+ was started successfully.
                        //
    bool                Started() const
                        {
                            return m_StartupRes == Gdiplus::Ok;
                        }

private:
    Gdiplus::GdiplusStartupInput
                        m_StartupInput;     // Struct with info for startup proc.
    ULONG_PTR           m_Token;            // Token to pass to shutdown proc.
    Gdiplus::Status     m_StartupRes;       // Result of GdiplusStartup call.
};
