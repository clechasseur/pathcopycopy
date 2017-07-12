// dllmain.h
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

#include <PathCopyCopy_i.h>

#include <atlbase.h>
#include <windows.h>

#include <resource.h>


class CPathCopyCopyModule final : public ATL::CAtlDllModuleT<CPathCopyCopyModule>
{
public :
	DECLARE_LIBID(LIBID_PathCopyCopyLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PATHCOPYCOPY, "{44F7E5A2-1286-45F5-9A7A-A95A41B72918}")

    HRESULT             DllRegisterServer(BOOL p_RegisterTypeLib = TRUE) throw();
    HRESULT             DllUnregisterServer(BOOL p_UnregisterTypeLib = TRUE) throw();

    static HINSTANCE    HInstance();
};

extern class CPathCopyCopyModule _AtlModule;
