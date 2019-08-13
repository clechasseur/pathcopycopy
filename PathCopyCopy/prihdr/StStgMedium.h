// StStgMedium.h
// (c) 2008-2019, Charles Lechasseur
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
#include <ole2.h>


//
// StStgMedium
//
// Stack-based class for proper disposal of a STGMEDIUM struct.
//
class StStgMedium final
{
public:
                        //
                        // Default constructor. Initializes the STGMEDIUM to empty values.
                        //
                        StStgMedium()
                            : m_StgMedium()
                        {
                            ::ZeroMemory(&m_StgMedium, sizeof(m_StgMedium));
                        }

                        //
                        // Copying not supported.
                        //
                        StStgMedium(const StStgMedium&) = delete;
    StStgMedium&        operator=(const StStgMedium&) = delete;

                        //
                        // Destructor. Releases the content of the STGMEDIUM.
                        //
                        ~StStgMedium()
                        {
                            ::ReleaseStgMedium(&m_StgMedium);
                            ::ZeroMemory(&m_StgMedium, sizeof(m_StgMedium));
                        }

                        //
                        // Returns reference to internal STGMEDIUM structure.
                        //
    STGMEDIUM&          Get()
                        {
                            return m_StgMedium;
                        }

                        //
                        // Returns reference to internal STGMEDIUM structure.
                        //
                        operator STGMEDIUM()
                        {
                            return m_StgMedium;
                        }

                        //
                        // Returns address of internal STGMEDIUM structure.
                        // This allows object to be used as an STGMEDIUM in calls:
                        //
                        // StStgMedium stgmed;
                        // obj->GetData(&fmtetc, &stgmed);
                        //
    STGMEDIUM*          operator&()
                        {
                            return &m_StgMedium;
                        }

private:
    STGMEDIUM           m_StgMedium;
};
