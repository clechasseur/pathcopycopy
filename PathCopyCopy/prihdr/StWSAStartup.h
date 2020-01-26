// StWSAStartup.h
// (c) 2019-2020, Charles Lechasseur
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

#include <WinSock2.h>


//
// StWSAStartup
//
// Stack-based class that initializes the WinSock library in a block of code.
// Destructor will take care of cleaning up the WinSock library.
//
class StWSAStartup final
{
public:
                        //
                        // Constructor. Calls WSAStartup to initialize the WinSock library.
                        //
                        // @param p_VersionRequested Requested WinSock version. Defaults to 2.2.
                        //
    explicit            StWSAStartup(const WORD p_VersionRequested = MAKEWORD(2, 2)) noexcept
                            : m_WSAData(),
                              m_StartupRes(::WSAStartup(p_VersionRequested, &m_WSAData))
                        {
                        }

                        // Copying/moving not supported.
                        StWSAStartup(const StWSAStartup&) = delete;
                        StWSAStartup(StWSAStartup&&) = delete;
    StWSAStartup&       operator=(const StWSAStartup&) = delete;
    StWSAStartup&       operator=(StWSAStartup&&) = delete;

                        //
                        // Destructor. Calls WSACleanup if startup was successful in constructor.
                        //
                        ~StWSAStartup()
                        {
                            if (m_StartupRes == 0) {
                                ::WSACleanup();
                            }
                        }

                        //
                        // Returns result of call to WSAStartup in constructor.
                        //
    int                 StartupRes() const noexcept
                        {
                            return m_StartupRes;
                        }

                        //
                        // Checks whether startup was successful in constructor.
                        //
    bool                Started() const noexcept
                        {
                            return m_StartupRes == 0;
                        }

                        //
                        // Returns implementation details received by the startup in constructor.
                        // Only valid if Started() returns true.
                        //
    const WSADATA&      WinSockDetails() const noexcept
                        {
                            return m_WSAData;
                        }

private:
    WSADATA             m_WSAData;      // Receives details of WinSock implementation.
    int                 m_StartupRes;   // Result of call to WSAStartup.
};
