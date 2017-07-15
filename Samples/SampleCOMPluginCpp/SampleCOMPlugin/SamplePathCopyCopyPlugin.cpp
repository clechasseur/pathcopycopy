// SamplePathCopyCopyPlugin.cpp : Implementation of CSamplePathCopyCopyPlugin

#include "stdafx.h"
#include "SamplePathCopyCopyPlugin.h"


// CSamplePathCopyCopyPlugin

// Method that must return the plugin description, displayed in the contextual menu.
STDMETHODIMP CSamplePathCopyCopyPlugin::get_Description(BSTR *p_ppDescription)
{
    *p_ppDescription = ::SysAllocString(L"Sample C++ COM Plugin");
    return S_OK;
}

// Method that can return a help text to be displayed in the status bar when the cursor is over the plugin's menu item.
// It is legal to return NULL or an empty string if no help text can be provided.
STDMETHODIMP CSamplePathCopyCopyPlugin::get_HelpText(BSTR *p_ppHelpText)
{
    *p_ppHelpText = ::SysAllocString(L"This is a sample Path Copy Copy plugin implemented as a C++ COM object. Will return the path unmodified.");
    return S_OK;
}

// Method that must return the path, with plugin-specific alteration.
STDMETHODIMP CSamplePathCopyCopyPlugin::GetPath(BSTR p_pPath, BSTR *p_ppNewPath)
{
    *p_ppNewPath = ::SysAllocString(p_pPath);
    return S_OK;
}

// Method that must return the ID of the plugin group to which this plugin belongs.
// All plugins in the same group will appear together in the contextual menu.
// Different groups will be split by menu separators.
// See https://github.com/clechasseur/pathcopycopy/wiki for more info.
STDMETHODIMP CSamplePathCopyCopyPlugin::get_GroupId(ULONG *p_pGroupId)
{
    *p_pGroupId = 0x5A4781E0;
    return S_OK;
}

// Method that must return the position of the plugin in the plugin group.
// This is only important if get_GroupId returns a non-zero value.
STDMETHODIMP CSamplePathCopyCopyPlugin::get_GroupPosition(ULONG *p_pPosition)
{
    *p_pPosition = 0;
    return S_OK;
}

// Method that determines whether the plugin should be enabled in the contextual menu.
// The method must return VARIANT_TRUE otherwise it will be grayed out.
// The method has access to the path of the first selected file, as well as its parent.
STDMETHODIMP CSamplePathCopyCopyPlugin::Enabled(BSTR /*p_pParentPath*/, BSTR /*p_pFile*/, VARIANT_BOOL *p_pEnabled)
{
    *p_pEnabled = VARIANT_TRUE;
    return S_OK;
}
