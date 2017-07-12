// SamplePathCopyCopyPlugin.h : Declaration of the CSamplePathCopyCopyPlugin

#pragma once
#include "resource.h"       // main symbols

#include "SampleCOMPlugin_i.h"
#include "PathCopyCopy_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CSamplePathCopyCopyPlugin

class ATL_NO_VTABLE CSamplePathCopyCopyPlugin :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSamplePathCopyCopyPlugin, &CLSID_SamplePathCopyCopyPlugin>,
	public ISamplePathCopyCopyPlugin,
    public IPathCopyCopyPlugin,
    public IPathCopyCopyPluginGroupInfo,
    public IPathCopyCopyPluginStateInfo
{
public:
	CSamplePathCopyCopyPlugin()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SAMPLEPATHCOPYCOPYPLUGIN)

DECLARE_NOT_AGGREGATABLE(CSamplePathCopyCopyPlugin)

BEGIN_COM_MAP(CSamplePathCopyCopyPlugin)
	COM_INTERFACE_ENTRY(ISamplePathCopyCopyPlugin)
    COM_INTERFACE_ENTRY(IPathCopyCopyPlugin)
    COM_INTERFACE_ENTRY(IPathCopyCopyPluginGroupInfo)
    COM_INTERFACE_ENTRY(IPathCopyCopyPluginStateInfo)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

    // IPathCopyCopyPlugin interface
    STDMETHOD(get_Description)(BSTR *p_ppDescription);
    STDMETHOD(get_HelpText)(BSTR *p_ppHelpText);
    STDMETHOD(GetPath)(BSTR p_pPath, BSTR *p_ppNewPath);

    // IPathCopyCopyPluginGroupInfo interface
    STDMETHOD(get_GroupId)(ULONG *p_pGroupId);
    STDMETHOD(get_GroupPosition)(ULONG *p_pPosition);

    // IPathCopyCopyPluginStateInfo
    STDMETHOD(Enabled)(BSTR p_pParentPath, BSTR p_pFile, VARIANT_BOOL *p_pEnabled);

public:

};

OBJECT_ENTRY_AUTO(__uuidof(SamplePathCopyCopyPlugin), CSamplePathCopyCopyPlugin)
