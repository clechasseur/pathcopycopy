// dllmain.h : Declaration of module class.

class CSampleCOMPluginModule : public CAtlDllModuleT< CSampleCOMPluginModule >
{
public :
	DECLARE_LIBID(LIBID_SampleCOMPluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SAMPLECOMPLUGIN, "{AB26800F-6F38-4A2A-8206-B4C60D52A783}")

    HRESULT DllRegisterServer(BOOL bRegTypeLib = TRUE) throw();
    HRESULT DllUnregisterServer(BOOL bUnRegTypeLib = TRUE) throw();
};

extern class CSampleCOMPluginModule _AtlModule;
