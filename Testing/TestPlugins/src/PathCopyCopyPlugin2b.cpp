// PathCopyCopyPlugin2b.cpp
// (c) 2011-2019, Charles Lechasseur
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
#include "PathCopyCopyPlugin2b.h"

#include <dllmain.h>


// CPathCopyCopyPlugin2b

// Method that must return the plugin description, displayed in the contextual menu.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_Description(BSTR *p_ppDescription)
{
    if (p_ppDescription == nullptr) {
        return E_INVALIDARG;
    }
    *p_ppDescription = ::SysAllocString(L"PCC Test Plugin 2b");
    return S_OK;
}

// Method that can return a help text to be displayed in the status bar when the cursor is over the plugin's menu item.
// It is legal to return NULL or an empty string if no help text can be provided.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_HelpText(BSTR *p_ppHelpText)
{
    if (p_ppHelpText == nullptr) {
        return E_INVALIDARG;
    }
    *p_ppHelpText = ::SysAllocString(L"Path Copy Copy test plugin 2b. Will return the path, appended with 2b.");
    return S_OK;
}

// Method that must return the path, with plugin-specific alteration.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::GetPath(BSTR p_pPath, BSTR *p_ppNewPath)
{
    if (p_pPath == nullptr || p_ppNewPath == nullptr) {
        return E_INVALIDARG;
    }
    std::wstring newPath(p_pPath);
    newPath += L"2b";
    *p_ppNewPath = ::SysAllocString(newPath.c_str());
    return S_OK;
}

// Method that must return the ID of the plugin group to which this plugin belongs.
// All plugins in the same group will appear together in the contextual menu.
// Different groups will be split by menu separators.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_GroupId(ULONG *p_pGroupId)
{
    if (p_pGroupId == nullptr) {
        return E_INVALIDARG;
    }
    *p_pGroupId = 'tpg2';
    return S_OK;
}

// Method that must return the position of the plugin in the plugin group.
// This is only important if get_GroupId returns a non-zero value.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_GroupPosition(ULONG *p_pPosition)
{
    if (p_pPosition == nullptr) {
        return E_INVALIDARG;
    }
    *p_pPosition = 1;
    return S_OK;
}

// Method that determines whether the plugin should be enabled in the contextual menu.
// The method must return VARIANT_TRUE otherwise it will be grayed out.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::Enabled(BSTR /*p_pParentPath*/,
                                            BSTR /*p_pFile*/,
                                            VARIANT_BOOL *p_pEnabled)
{
    if (p_pEnabled == nullptr) {
        return E_INVALIDARG;
    }
    *p_pEnabled = VARIANT_TRUE;
    return S_OK;
}

// Method that provides the path of a file containing the image to use
// for the icon of the plugin in the contextual menu.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_IconFile(BSTR *p_ppIconFile)
{
    // Initialize to NULL in case we fail to provide a file.
    if (p_ppIconFile != nullptr) {
        *p_ppIconFile = nullptr;
    } else {
        return E_INVALIDARG;
    }
    HRESULT hRes = E_FAIL;

    try {
        // Get path to this module.
        std::wstring thisModulePath(MAX_PATH + 1, L'\0');
        if (::GetModuleFileNameW(CTestPluginsModule::HInstance(), &*thisModulePath.begin(), thisModulePath.size()) != 0) {
            // Remove filename from path.
            std::wstring path(thisModulePath.c_str());
            std::wstring::size_type lastDelimPos = path.find_last_of(L"\\/");
            if (lastDelimPos != std::wstring::npos) {
                path.erase(lastDelimPos);
            } else {
                path.clear();
            }
        
            // Go up three levels then dig to find the icon file.
            for (int i = 0; i < 3; ++i) {
                lastDelimPos = path.find_last_of(L"\\/");
                if (lastDelimPos != std::wstring::npos) {
                    path.erase(lastDelimPos);
                } else {
                    path.clear();
                    break;
                }
            }
            path += L"\\icons\\settings.ico";

            // Return resulting path.
            *p_ppIconFile = ::SysAllocString(path.c_str());
            hRes = S_OK;
        }
    } catch (...) {
        hRes = E_UNEXPECTED;
    }

    return hRes;
}

// Method that determines whether the plugin uses the default icon.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin2b::get_UseDefaultIcon(VARIANT_BOOL *p_pUseDefaultIcon)
{
    if (p_pUseDefaultIcon == nullptr) {
        return E_INVALIDARG;
    }
    *p_pUseDefaultIcon = VARIANT_FALSE;
    return S_OK;
}
