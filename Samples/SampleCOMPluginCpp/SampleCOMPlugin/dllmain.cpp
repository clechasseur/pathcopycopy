// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "SampleCOMPlugin_i.h"
#include "dllmain.h"
#include "dlldatax.h"
#include "PathCopyCopy_i.h"

CSampleCOMPluginModule _AtlModule;

// Registers our COM object. Let's use the opportunity to register ourselves as a Path Copy Copy plugin.
HRESULT CSampleCOMPluginModule::DllRegisterServer( BOOL bRegTypeLib /*= TRUE*/ ) throw()
{
    HRESULT hRes = CAtlDllModuleT< CSampleCOMPluginModule >::DllRegisterServer(bRegTypeLib);
    if (SUCCEEDED(hRes)) {
        CComPtr<IPathCopyCopyContextMenuExt> cpPccExt;
        hRes = cpPccExt.CoCreateInstance(__uuidof(PathCopyCopyContextMenuExt));
        if (SUCCEEDED(hRes)) {
            cpPccExt->RegisterPlugin(__uuidof(SamplePathCopyCopyPlugin));
        }
    }
    return hRes;
}

// Unregisters our COM object. We will also remove our plugin from Path Copy Copy.
HRESULT CSampleCOMPluginModule::DllUnregisterServer( BOOL bUnRegTypeLib /*= TRUE*/ ) throw()
{
    HRESULT hRes = CAtlDllModuleT< CSampleCOMPluginModule >::DllUnregisterServer(bUnRegTypeLib);
    if (SUCCEEDED(hRes)) {
        CComPtr<IPathCopyCopyContextMenuExt> cpPccExt;
        hRes = cpPccExt.CoCreateInstance(__uuidof(PathCopyCopyContextMenuExt));
        if (SUCCEEDED(hRes)) {
            cpPccExt->UnregisterPlugin(__uuidof(SamplePathCopyCopyPlugin));
        }
    }
    return hRes;
}

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
