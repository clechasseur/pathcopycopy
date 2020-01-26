// dllmain.cpp
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

#include "stdafx.h"
#include "resource.h"
#include "TestPlugins_i.h"
#include <PathCopyCopy_i.h>
#include "dllmain.h"
#include "dlldatax.h"

#include <StAtlPerUserOverride.h>

namespace {

// Keeps the global instance passed to DllMain.
HINSTANCE g_hInstance = nullptr;

} // anonymous namespace

#pragma warning(suppress: ALL_CPPCORECHECK_WARNINGS)
CTestPluginsModule _AtlModule;

// Registers our COM object. Let's use the opportunity to register our plugins.
[[gsl::suppress(c.128)]]
HRESULT CTestPluginsModule::DllRegisterServer(BOOL bRegTypeLib /*= TRUE*/) throw()
{
    // Create PathCopyCopy registration object and check if it supports per-user install.
    ATL::CComPtr<IPathCopyCopyContextMenuExt> cpPccExt;
    HRESULT hRes = cpPccExt.CoCreateInstance(__uuidof(PathCopyCopyContextMenuExt));
    if (SUCCEEDED(hRes)) {
        ATL::CComQIPtr<IPathCopyCopyContextMenuExt2> cpPccExt2(cpPccExt);

        // Setup per-user registration. If user asked for a per-user install
        // but the registration object does not support it, fail.
        StAtlPerUserOverride perUserOverride;
        hRes = perUserOverride.Succeeded() ? S_OK : E_FAIL;
        if (SUCCEEDED(hRes) && perUserOverride.Overridden() && cpPccExt2.p == nullptr) {
            hRes = E_NOTIMPL;
        }
        if (SUCCEEDED(hRes)) {
            // First perform DLL registration.
            hRes = ATL::CAtlDllModuleT<CTestPluginsModule>::DllRegisterServer(bRegTypeLib);
            if (SUCCEEDED(hRes)) {
                // Perform per-user registration if available, otherwise fall back to
                // the default interface. We'll have verified that this is OK before.
                if (cpPccExt2.p != nullptr) {
                    const VARIANT_BOOL varPerUser = perUserOverride.Overridden() ? VARIANT_TRUE : VARIANT_FALSE;
                    const auto registerPlugin = [&](REFCLSID p_CLSID) {
                        if (SUCCEEDED(hRes)) {
                            hRes = cpPccExt2->RegisterPlugin2(p_CLSID, varPerUser);
                        }
                    };
                    registerPlugin(__uuidof(PathCopyCopyPlugin1a));
                    registerPlugin(__uuidof(PathCopyCopyPlugin1b));
                    registerPlugin(__uuidof(PathCopyCopyPlugin2a));
                    registerPlugin(__uuidof(PathCopyCopyPlugin2b));
                } else {
                    const auto registerPlugin = [&](REFCLSID p_CLSID) {
                        if (SUCCEEDED(hRes)) {
                            hRes = cpPccExt->RegisterPlugin(p_CLSID);
                        }
                    };
                    registerPlugin(__uuidof(PathCopyCopyPlugin1a));
                    registerPlugin(__uuidof(PathCopyCopyPlugin1b));
                    registerPlugin(__uuidof(PathCopyCopyPlugin2a));
                    registerPlugin(__uuidof(PathCopyCopyPlugin2b));
                }
            }
        }
    }
    return hRes;
}

// Unregisters our COM object. We will also remove our plugins from Path Copy Copy.
[[gsl::suppress(c.128)]]
HRESULT CTestPluginsModule::DllUnregisterServer(BOOL bUnRegTypeLib /*= TRUE*/) throw()
{
    // Setup per-user unregistration.
    StAtlPerUserOverride perUserOverride;
    HRESULT hRes = perUserOverride.Succeeded() ? S_OK : E_FAIL;
    if (SUCCEEDED(hRes)) {
        // First perform DLL unregistration.
        hRes = ATL::CAtlDllModuleT<CTestPluginsModule>::DllUnregisterServer(bUnRegTypeLib);
        if (SUCCEEDED(hRes)) {
            // Create PathCopyCopy registration object and check if it supports per-user uninstall.
            ATL::CComPtr<IPathCopyCopyContextMenuExt> cpPccExt;
            hRes = cpPccExt.CoCreateInstance(__uuidof(PathCopyCopyContextMenuExt));
            if (SUCCEEDED(hRes)) {
                ATL::CComQIPtr<IPathCopyCopyContextMenuExt2> cpPccExt2(cpPccExt);

                // If user asked for a per-user uninstall but the registration
                // object does not support it, fail.
                if (SUCCEEDED(hRes) && perUserOverride.Overridden() && cpPccExt2.p == nullptr) {
                    hRes = E_NOTIMPL;
                } else {
                    // Perform per-user unregistration if available, otherwise fall back to
                    // the default interface. We'll have verified that this is OK before.
                    if (cpPccExt2.p != nullptr) {
                        const VARIANT_BOOL varPerUser = perUserOverride.Overridden() ? VARIANT_TRUE : VARIANT_FALSE;
                        const auto unregisterPlugin = [&](REFCLSID p_CLSID) {
                            if (SUCCEEDED(hRes)) {
                                hRes = cpPccExt2->UnregisterPlugin2(p_CLSID, varPerUser);
                            }
                        };
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin1a));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin1b));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin2a));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin2b));
                    } else {
                        const auto unregisterPlugin = [&](REFCLSID p_CLSID) {
                            if (SUCCEEDED(hRes)) {
                                hRes = cpPccExt->UnregisterPlugin(p_CLSID);
                            }
                        };
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin1a));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin1b));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin2a));
                        unregisterPlugin(__uuidof(PathCopyCopyPlugin2b));
                    }
                }
            }
        }
    }
    return hRes;
}

// Returns the instance handle passed to our module's DllMain.
HINSTANCE CTestPluginsModule::HInstance() noexcept
{
    return g_hInstance;
}

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
