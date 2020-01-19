// stdafx.h
// (c) 2008-2020, Charles Lechasseur
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

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

//#define PCC_NO_CONTEXT_MENU_EXT2    // For testing purposes only

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <windows.h>

// Disable some warnings in GDI+ headers
#pragma warning( push )
#pragma warning( disable : 4458 )
#include <gdiplus.h>
#pragma warning( pop )

// Undef the Windows min and max macros, since they conflict with STL
// We can't define NOMINMAX because GDI actually needs those macros :(
#undef min
#undef max

#include <algorithm>
#include <cstdint>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include <assert.h>
#include <memory.h>

#include "PathCopyCopyPrivateTypes.h"

#include <resource.h>

// Including this header allows us to suppress C++ Core Guideline warnings more easily
#include <CppCoreCheck\warnings.h>

#include <gsl/gsl>

// Disable C++ Core checks warnings in coveo::linq since it seems to generate a lot
#pragma warning(push)
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#include <coveo/linq.h>
#pragma warning(pop)
