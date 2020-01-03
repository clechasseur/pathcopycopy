// dllmain.cpp
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

#include <stdafx.h>
#include <dllmain.h>
#include <dlldatax.h>
#include <PathCopyCopy_i.h>

#include <AtlRegKey.h>
#include <StAtlPerUserOverride.h>

namespace {

// Keeps the global instance passed to DllMain.
HINSTANCE g_hInstance = nullptr;

} // anonymous namespace

//
// CPathCopyCopyModule::DllRegisterServer
//
// Registers this module's classes and type libraries.
//
// @param p_RegisterTypeLib Whether to register type libraries.
// @return Result code.
//
[[gsl::suppress(c.128)]]
HRESULT CPathCopyCopyModule::DllRegisterServer(BOOL p_RegisterTypeLib /*= TRUE*/) throw()
{
    try {
        // Setup per-user registration if needed.
        StAtlPerUserOverride perUserOverride;
        HRESULT hRes = perUserOverride.Succeeded() ? S_OK : E_FAIL;
        if (SUCCEEDED(hRes)) {
            hRes = ATL::CAtlDllModuleT<CPathCopyCopyModule>::DllRegisterServer(p_RegisterTypeLib);
            if (SUCCEEDED(hRes)) {
                // Register our shell extensions as "approved". We do this here so that
                // it can work in per-user installations.
                AtlRegKey approvedKey;
                LONG regRes = approvedKey.Open(perUserOverride.Overridden() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
                                               L"Software\\Microsoft\\Windows\\CurrentVersion\\Extensions\\Approved",
                                               true, KEY_QUERY_VALUE | KEY_SET_VALUE);
                if (regRes == ERROR_SUCCESS) {
                    regRes = approvedKey.SetStringValue(L"{82CB99A2-2F18-4D5D-9476-54347E3B6720}",
                                                        L"PathCopyCopy Contextual Menu Handler");
                }
                if (regRes == ERROR_SUCCESS) {
                    regRes = approvedKey.SetStringValue(L"{16170CA5-25CA-4e6d-928C-7A3A974F4B56}",
                                                        L"PathCopyCopy Data Handler");
                }
                if (regRes != ERROR_SUCCESS) {
                    hRes = HRESULT_FROM_WIN32(regRes);
                }
            }
        }
        return hRes;
    } catch (...) {
        return E_UNEXPECTED;
    }
}

//
// CPathCopyCopyModule::DllUnregisterServer
//
// Unregisters this module's classes and type libraries.
//
// @param p_UnregisterTypeLib Whether to unregister type libraries.
// @return Result code.
//
[[gsl::suppress(c.128)]]
HRESULT CPathCopyCopyModule::DllUnregisterServer(BOOL p_UnregisterTypeLib /*= TRUE*/) throw()
{
    try {
        // Setup per-user unregistration if needed.
        StAtlPerUserOverride perUserOverride;
        HRESULT hRes = perUserOverride.Succeeded() ? S_OK : E_FAIL;
        if (SUCCEEDED(hRes)) {
            hRes = ATL::CAtlDllModuleT<CPathCopyCopyModule>::DllUnregisterServer(p_UnregisterTypeLib);
            if (SUCCEEDED(hRes)) {
                // Unregister our approved shell extensions. We do this here so that
                // it can work in per-user installations.
                // Note that if it doesn't exist, we consider it a success.
                AtlRegKey approvedKey;
                const LONG regRes = approvedKey.Open(perUserOverride.Overridden() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
                                                     L"Software\\Microsoft\\Windows\\CurrentVersion\\Extensions\\Approved",
                                                     false, KEY_QUERY_VALUE | KEY_SET_VALUE);
                if (regRes != ERROR_SUCCESS && regRes != ERROR_FILE_NOT_FOUND) {
                    hRes = HRESULT_FROM_WIN32(regRes);
                } else if (regRes == ERROR_SUCCESS) {
                    approvedKey.DeleteValue(L"{82CB99A2-2F18-4D5D-9476-54347E3B6720}");
                    approvedKey.DeleteValue(L"{16170CA5-25CA-4e6d-928C-7A3A974F4B56}");
                }
            }
        }
        return hRes;
    } catch (...) {
        return E_UNEXPECTED;
    }
}

//
// CPathCopyCopyModule::HInstance
//
// Returns the instance handle passed to our module's /DllMain/.
//
// @return Module instance handle.
//
HINSTANCE CPathCopyCopyModule::HInstance() noexcept
{
    return g_hInstance;
}

#pragma warning(suppress: ALL_CPPCORECHECK_WARNINGS)
CPathCopyCopyModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	g_hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
