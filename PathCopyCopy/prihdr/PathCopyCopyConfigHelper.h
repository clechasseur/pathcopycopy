// PathCopyCopyConfigHelper.h
// (c) 2011-2017, Charles Lechasseur
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
#include "PathCopyCopyPrivateTypes.h"
#include "Plugin.h"
#include "resource.h"

#include <atlbase.h>
#include <atlcom.h>
#include <windows.h>


//
// CPathCopyCopyConfigHelper
//
// Helper COM object to get information about plugins. Now deprecated since
// the UI no longer uses it; left here for backward-compatibility purposes.
//
class ATL_NO_VTABLE CPathCopyCopyConfigHelper :
	public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
	public ATL::CComCoClass<CPathCopyCopyConfigHelper, &CLSID_PathCopyCopyConfigHelper>,
	public IPathCopyCopyConfigHelper
{
public:
	CPathCopyCopyConfigHelper();

    DECLARE_REGISTRY_RESOURCEID(IDR_PATHCOPYCOPYCONFIGHELPER)

    DECLARE_NOT_AGGREGATABLE(CPathCopyCopyConfigHelper)

    BEGIN_COM_MAP(CPathCopyCopyConfigHelper)
	    COM_INTERFACE_ENTRY(IPathCopyCopyConfigHelper)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    // IPathCopyCopyConfigHelper interface
    STDMETHOD(get_PluginCount)(ULONG *p_pCount);
    STDMETHOD(GetPluginInfo)(ULONG p_Index, BSTR *p_ppId, BSTR *p_ppDescription, VARIANT_BOOL *p_pIsSeparator);

private:
    PCC::SettingsSP     m_spSettings;               // Object to access PCC settings.
    PCC::PluginSPV      m_vspPluginsInDefaultOrder; // Vector of all plugins in default order.
    PCC::PluginSPS      m_sspAllPlugins;            // Set containing all plugins.
    PCC::PluginProviderSP
                        m_spPluginProvider;         // Object to access other plugins.
    PCC::PluginSPV      m_vspPlugins;               // Plugins accessible through this helper.

    void                Initialize();
};

OBJECT_ENTRY_AUTO(__uuidof(PathCopyCopyConfigHelper), CPathCopyCopyConfigHelper)
