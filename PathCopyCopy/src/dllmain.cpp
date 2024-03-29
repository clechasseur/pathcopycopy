// dllmain.cpp
// (c) 2008-2021, Charles Lechasseur
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
#include <PluginUtils.h>
#include <UserOverrideableRegKey.h>


namespace {

// Keeps the global instance passed to DllMain.
HINSTANCE g_hInstance = nullptr;

} // anonymous namespace

// Trick to get our image base, useful to get the path to this DLL.
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

//
// CPathCopyCopyModule::CPathCopyCopyModule
//
// Constructor.
//
CPathCopyCopyModule::CPathCopyCopyModule()
    : ATL::CAtlDllModuleT<CPathCopyCopyModule>()
{
    // Get path to this DLL. We need it to locate our resource DLL.
    std::wstring path(MAX_PATH + 1, L'\0');
#pragma warning(suppress: 26490) // Dirty trick is dirty
    if (::GetModuleFileNameW(reinterpret_cast<HMODULE>(&__ImageBase), path.data(), gsl::narrow<DWORD>(path.size())) != 0) {
        // Get parent folder containing our DLL and the resource DLLs also.
        PCC::PluginUtils::ExtractFolderFromPath(path);

        // First try loading the resource DLL specified in the registry.
        // If that fails, load the default one.
        const auto tryLoadResourceDll = [&](const std::wstring& language) {
            const auto dllPath = path + L"\\" + L"PathCopyCopyLocalization_" + language + L".dll";
            m_hResourceDll = ::LoadLibraryExW(dllPath.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
            if (m_hResourceDll != nullptr) {
                ATL::_AtlBaseModule.AddResourceInstance(m_hResourceDll);
            }
        };
        tryLoadResourceDll(GetResourceDllLanguage());
        if (m_hResourceDll == nullptr) {
            tryLoadResourceDll(L"en");
        }
    }
}

//
// CPathCopyCopyModule::~CPathCopyCopyModule
//
// Destructor.
//
CPathCopyCopyModule::~CPathCopyCopyModule()
{
    // If resource DLL was successfully loaded, free it here.
    if (m_hResourceDll != nullptr) {
        ATL::_AtlBaseModule.RemoveResourceInstance(m_hResourceDll);
        ::FreeLibrary(m_hResourceDll);
    }
}

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
        // Check for per-user registration.
        bool perUserRegistration = false;
        HRESULT hRes = ATL::AtlGetPerUserRegistration(&perUserRegistration);
        if (SUCCEEDED(hRes)) {
            hRes = ATL::CAtlDllModuleT<CPathCopyCopyModule>::DllRegisterServer(p_RegisterTypeLib);
            if (SUCCEEDED(hRes)) {
                // Register our shell extensions as "approved". We do this here so that
                // it can work in per-user installations.
                AtlRegKey approvedKey;
                LONG regRes = approvedKey.Open(perUserRegistration ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
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
        bool perUserRegistration = false;
        HRESULT hRes = ATL::AtlGetPerUserRegistration(&perUserRegistration);
        if (SUCCEEDED(hRes)) {
            hRes = ATL::CAtlDllModuleT<CPathCopyCopyModule>::DllUnregisterServer(p_UnregisterTypeLib);
            if (SUCCEEDED(hRes)) {
                // Unregister our approved shell extensions. We do this here so that
                // it can work in per-user installations.
                // Note that if it doesn't exist, we consider it a success.
                AtlRegKey approvedKey;
                const LONG regRes = approvedKey.Open(perUserRegistration ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
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

//
// CPathCopyCopyModule::GetResourceDllLanguage
//
// Returns the two-letter language code of the resource DLL.
//
// @return Language of resource DLL.
//
std::wstring CPathCopyCopyModule::GetResourceDllLanguage() const
{
    // By default the language is English.
    std::wstring language = L"en";

    // The resource DLL language can be overridden in the registry.
    UserOverrideableRegKey regKey(L"Software\\clechasseur\\PathCopyCopy");
    std::wstring regLanguage;
    if (PCC::PluginUtils::ReadRegistryStringValue(regKey, L"Language", regLanguage) == ERROR_SUCCESS) {
        // The language in the registry is a complete culture designation, like fr-CA.
        // We're only interested in the language part (first two letters).
        language = regLanguage.substr(0, 2);
    }

    return language;
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
