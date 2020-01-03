// PathCopyCopyDataHandler.h
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

#pragma once

#include "PathCopyCopy_i.h"
#include "resource.h"

#include <string>

#include <atlbase.h>
#include <atlcom.h>
#include <windows.h>


//
// CPathCopyCopyDataHandler
//
// Shell data handler that will provide a textual representation of any file/folder
// being dragged and dropped containing the path of that file/folder.
// Currently in a "proof-of-concept" stage; it doesn't prove anything yet :)
//
class ATL_NO_VTABLE CPathCopyCopyDataHandler :
	public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
	public ATL::CComCoClass<CPathCopyCopyDataHandler, &CLSID_PathCopyCopyDataHandler>,
	public IPathCopyCopyDataHandler,
    public IPersistFile,
    public IDataObject
{
public:
	CPathCopyCopyDataHandler() noexcept(false);
    CPathCopyCopyDataHandler(const CPathCopyCopyDataHandler&) = delete;
    CPathCopyCopyDataHandler(CPathCopyCopyDataHandler&&) = delete;
    CPathCopyCopyDataHandler& operator=(const CPathCopyCopyDataHandler&) = delete;
    CPathCopyCopyDataHandler& operator=(CPathCopyCopyDataHandler&&) = delete;
    virtual ~CPathCopyCopyDataHandler() = default;

#pragma warning(push)
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)

    DECLARE_REGISTRY_RESOURCEID(IDR_PATHCOPYCOPYDATAHANDLER)

    DECLARE_NOT_AGGREGATABLE(CPathCopyCopyDataHandler)

    BEGIN_COM_MAP(CPathCopyCopyDataHandler)
	    COM_INTERFACE_ENTRY(IPathCopyCopyDataHandler)
        COM_INTERFACE_ENTRY(IPersistFile)
        COM_INTERFACE_ENTRY(IPersist)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

#pragma warning(pop)

    [[gsl::suppress(c.128)]]
	HRESULT FinalConstruct() noexcept
	{
		return S_OK;
	}

    [[gsl::suppress(c.128)]]
	void FinalRelease() noexcept
	{
	}

public:
    // IPersistFile interface
    STDMETHOD(IsDirty)();
    STDMETHOD(Load)(LPCOLESTR pszFileName, DWORD dwMode);
    STDMETHOD(Save)(LPCOLESTR pszFileName, BOOL fRemember);
    STDMETHOD(SaveCompleted)(LPCOLESTR pszFileName);
    STDMETHOD(GetCurFile)(LPOLESTR *ppszFileName);

    // IPersist interface
    STDMETHOD(GetClassID)(CLSID *pClassID);

    // IDataObject interface
    STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
    STDMETHOD(GetDataHere)(FORMATETC *pformatetc, STGMEDIUM *pmedium);
    STDMETHOD(QueryGetData)(FORMATETC *pformatetc);
    STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pformatectIn, FORMATETC *pformatetcOut);
    STDMETHOD(SetData)(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease);
    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc);
    STDMETHOD(DAdvise)(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
    STDMETHOD(DUnadvise)(DWORD dwConnection);
    STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise);

private:
    std::wstring        m_FileName;     // Name of file to act upon (only one).
};

#pragma warning(suppress: ALL_CPPCORECHECK_WARNINGS)
OBJECT_ENTRY_AUTO(__uuidof(PathCopyCopyDataHandler), CPathCopyCopyDataHandler)
