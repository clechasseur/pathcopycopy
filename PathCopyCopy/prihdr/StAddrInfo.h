// StAddrInfo.h
// (c) 2019, Charles Lechasseur
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

#include <ws2def.h>
#include <WS2tcpip.h>


//
// StAddrInfo
//
// Wrapper for a WinSock addrinfoW pointer. Designed to be used in a block of code.
// Will automatically free the addrinfoW pointer in the destructor using FreeAddrInfoW.
//
class StAddrInfo final
{
public:
                        //
                        // Default constructor.
                        // Initializes addrinfoW pointer to null.
                        //
                        StAddrInfo() noexcept
                            : m_pAddrInfo(nullptr)
                        {
                        }

                        //
                        // Constructor that acquires a given addrinfoW pointer.
                        //
                        // @param p_pAddrInfo addrinfoW pointer. We assume ownership.
                        //
                        StAddrInfo(PADDRINFOW const p_pAddrInfo) noexcept
                            : m_pAddrInfo(p_pAddrInfo)
                        {
                        }

                        // Copying/moving not supported.
                        StAddrInfo(const StAddrInfo&) = delete;
                        StAddrInfo(StAddrInfo&&) = delete;
    StAddrInfo&         operator=(const StAddrInfo&) = delete;
    StAddrInfo&         operator=(StAddrInfo&&) = delete;

                        //
                        // Destructor. Frees the addrinfoW pointer if set.
                        //
                        ~StAddrInfo()
                        {
                            if (m_pAddrInfo != nullptr) {
                                ::FreeAddrInfoW(m_pAddrInfo);
                            }
                        }

                        //
                        // Returns a reference to the internal addrinfoW pointer.
                        //
    PADDRINFOW&         Get() noexcept
                        {
                            return m_pAddrInfo;
                        }

                        //
                        // Returns a reference to the internal addrinfoW pointer.
                        //
                        operator PADDRINFOW() noexcept
                        {
                            return m_pAddrInfo;
                        }

                        //
                        // Dereferences the internal addrinfoW pointer.
                        //
   addrinfoW&           operator*() noexcept
                        {
                            return *m_pAddrInfo;
                        }

                        //
                        // Dereferences the internal addrinfoW pointer.
                        //
   PADDRINFOW           operator->() noexcept
                        {
                            return m_pAddrInfo;
                        }

                        //
                        // Returns the address of the internal addrinfoW pointer.
                        // Designed to be used for API calls like this:
                        //
                        // StAddrInfo addrinfo;
                        // GetAddrInfoW(..., &addrinfo);
                        //
    PADDRINFOW*         operator&() noexcept
                        {
                            return &m_pAddrInfo;
                        }

private:
    PADDRINFOW          m_pAddrInfo;    // addrinfoW pointer. Can be null.
};
