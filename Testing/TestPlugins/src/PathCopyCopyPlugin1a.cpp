// PathCopyCopyPlugin1a.cpp
// (c) 2011-2021, Charles Lechasseur
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
#include "PathCopyCopyPlugin1a.h"


// CPathCopyCopyPlugin1a

// Method that must return the plugin description, displayed in the contextual menu.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::get_Description(BSTR *p_ppDescription)
{
    if (p_ppDescription == nullptr) {
        return E_INVALIDARG;
    }
    *p_ppDescription = ::SysAllocString(L"PCC Test Plugin 1a");
    return S_OK;
}

// Method that can return a help text to be displayed in the status bar when the cursor is over the plugin's menu item.
// It is legal to return NULL or an empty string if no help text can be provided.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::get_HelpText(BSTR *p_ppHelpText)
{
    if (p_ppHelpText == nullptr) {
        return E_INVALIDARG;
    }
    *p_ppHelpText = ::SysAllocString(L"Path Copy Copy test plugin 1a. Will return the path, appended with 1a.");
    return S_OK;
}

// Method that must return the path, with plugin-specific alteration.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::GetPath(BSTR p_pPath, BSTR *p_ppNewPath)
{
    if (p_pPath == nullptr || p_ppNewPath == nullptr) {
        return E_INVALIDARG;
    }
    std::wstring newPath(p_pPath);
    newPath += L"1a";
    *p_ppNewPath = ::SysAllocString(newPath.c_str());
    return S_OK;
}

// Method that must return the ID of the plugin group to which this plugin belongs.
// All plugins in the same group will appear together in the contextual menu.
// Different groups will be split by menu separators.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::get_GroupId(ULONG *p_pGroupId)
{
    if (p_pGroupId == nullptr) {
        return E_INVALIDARG;
    }
    *p_pGroupId = 'tpg1';
    return S_OK;
}

// Method that must return the position of the plugin in the plugin group.
// This is only important if get_GroupId returns a non-zero value.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::get_GroupPosition(ULONG *p_pPosition)
{
    if (p_pPosition == nullptr) {
        return E_INVALIDARG;
    }
    *p_pPosition = 0;
    return S_OK;
}

// Method that determines whether the plugin should be enabled in the contextual menu.
// The method must return VARIANT_TRUE otherwise it will be grayed out.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::Enabled(BSTR /*p_pParentPath*/,
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
STDMETHODIMP CPathCopyCopyPlugin1a::get_IconFile(BSTR *p_ppIconFile)
{
    if (p_ppIconFile == nullptr) {
        return E_INVALIDARG;
    }
    *p_ppIconFile = nullptr;
    return S_OK;
}

// Method that determines whether the plugin uses the default icon.
[[gsl::suppress(c.128), gsl::suppress(f.6)]]
STDMETHODIMP CPathCopyCopyPlugin1a::get_UseDefaultIcon(VARIANT_BOOL *p_pUseDefaultIcon)
{
    if (p_pUseDefaultIcon == nullptr) {
        return E_INVALIDARG;
    }
    *p_pUseDefaultIcon = VARIANT_TRUE;
    return S_OK;
}
