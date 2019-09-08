// PathCopyCopyDataHandler.cpp
// (c) 2008-2019, Charles Lechasseur
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
#include <PathCopyCopyDataHandler.h>
#include <DefaultPlugin.h>
#include <StGlobalBlock.h>
#include <StGlobalLock.h>

#include <fstream>
#include <sstream>

// Uncomment this to enable logging in this class.
//#define PCC_DATA_HANDLER_LOGGING


// CPathCopyCopyContextMenuExt

//
// Constructor.
//
CPathCopyCopyDataHandler::CPathCopyCopyDataHandler() noexcept(false)
    : m_FileName()
{
}

//
// IPersistFile::IsDirty
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::IsDirty()
{
    return E_NOTIMPL;
}

//
// IPersistFile::Load
// This method is called by the Windows Shell to initialize
// the data handler for a particular file. We'll use this opportunity
// to note the file name for future use.
//
// @param pszFileName Name of the file to act upon.
// @param dwMode File open mode. We ignore this as we don't need to actually open the file.
// @return S_OK if successful, otherwise an error code.
//
STDMETHODIMP CPathCopyCopyDataHandler::Load(
    LPCOLESTR pszFileName,
    DWORD /*dwMode*/)
{
    HRESULT hRes = S_OK;
    try {
        m_FileName = pszFileName;
    } catch (...) {
        hRes = E_UNEXPECTED;
        m_FileName.clear();
    }
    return hRes;
}

//
// IPersistFile::Save
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::Save(
    LPCOLESTR /*pszFileName*/,
    BOOL /*fRemember*/)
{
    return E_NOTIMPL;
}

//
// IPersistFile::SaveCompleted
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::SaveCompleted(
    LPCOLESTR /*pszFileName*/)
{
    return E_NOTIMPL;
}

//
// IPersistFile::GetCurFile
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::GetCurFile(
    LPOLESTR* /*ppszFileName*/)
{
    return E_NOTIMPL;
}

//
// IPersist::GetClassID
// Must return the CLSID of the class capable of handling the data.
// We will return our own CLSID.
//
// @param pClassID Where to store our CLSID.
// @return S_OK if successful, otherwise an error code.
//
STDMETHODIMP CPathCopyCopyDataHandler::GetClassID(
    CLSID *pClassID)
{
    HRESULT hRes = E_INVALIDARG;
    if (pClassID != nullptr) {
        *pClassID = __uuidof(PathCopyCopyDataHandler);
        hRes = S_OK;
    }
    return hRes;
}

//
// IDataObject::GetData
// Called by the shell when data is actually required.
//
// @param pformatetcIn Format descriptor for the format requested.
// @param pmedium Where to save the storage medium containing our data.
// @return S_OK if successful, otherwise an error code.
//
STDMETHODIMP CPathCopyCopyDataHandler::GetData(
    FORMATETC *pformatetcIn,
    STGMEDIUM *pmedium)
{
#ifdef PCC_DATA_HANDLER_LOGGING
    GUID filg;
    ::CoCreateGuid(&filg);
    OLECHAR filn[256];
    ::StringFromGUID2(filg, filn, 256);
    std::wstringstream fils;
    fils << L"C:\\" << filn << L".txt";
    std::wstring filns = fils.str();
    std::wofstream fil(ATL::CW2A(filns.c_str()));
#endif // PCC_DATA_HANDLER_LOGGING

    HRESULT hRes = E_INVALIDARG;
    if (pformatetcIn != nullptr && pmedium != nullptr) {
        try {
            // Make sure we support this particular format. Since
            // we only support one combination, we'll call QueryGetData.
            hRes = this->QueryGetData(pformatetcIn);
#ifdef PCC_DATA_HANDLER_LOGGING
            fil << L"QueryGetDataResult: 0x" << std::hex << hRes << std::endl;
#endif // PCC_DATA_HANDLER_LOGGING
            if (SUCCEEDED(hRes)) {
                // It's the format we support.

                // First get the path of the file using the default plugin.
                std::wstring newPath = PCC::Plugins::DefaultPlugin().GetPath(m_FileName);
#ifdef PCC_DATA_HANDLER_LOGGING
                fil << L"Filename: " << m_FileName << std::endl
                    << L"New path: " << newPath << std::endl;
#endif // PCC_DATA_HANDLER_LOGGING

                // Allocate an HGLOBAL to store the string.
                const SIZE_T memSizeInBytes = (newPath.size() + 1) * sizeof(wchar_t);
                StGlobalBlock globalBlock(GHND, memSizeInBytes);
                if (globalBlock.Get() == NULL) {
#ifdef PCC_DATA_HANDLER_LOGGING
                    fil << L"GlobalAlloc failed!" << std::endl;
#endif // PCC_DATA_HANDLER_LOGGING
                    hRes = STG_E_MEDIUMFULL;
                } else {
                    // Lock the handle to access the memory and copy the path.
                    StGlobalLock rawBlock(globalBlock.Get());
                    void* pBlock = rawBlock.GetPtr();
                    if (pBlock == nullptr) {
#ifdef PCC_DATA_HANDLER_LOGGING
                        fil << L"GlobalLock failed!" << std::endl;
#endif // PCC_DATA_HANDLER_LOGGING
                        hRes = STG_E_MEDIUMFULL;
                    } else {
                        ::memcpy(pBlock, newPath.c_str(), memSizeInBytes);
                        hRes = S_OK;
                    }
                }

                // If we made it here in one piece, release the ownership of the
                // global block and save the handle in the STGMEDIUM struct.
                pmedium->hGlobal = globalBlock.Release();
                pmedium->tymed = TYMED_HGLOBAL;
                pmedium->pUnkForRelease = nullptr;
            }
        } catch (...) {
            hRes = E_UNEXPECTED;
        }
    }
    return hRes;
}

//
// IDataObject::GetDataHere
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::GetDataHere(
    FORMATETC* /*pformatetc*/,
    STGMEDIUM* /*pmedium*/)
{
    return E_NOTIMPL;
}

//
// IDataObject::QueryGetData
// Method called by the shell to validate whether we
// can provide data in a specific format.
// In our case, we only supply text through an HGLOBAL.
//
// @param pformatetc Pointer to a FORMATETC struct describing the format.
// @return S_OK if we support the format, otherwise an error code.
//
STDMETHODIMP CPathCopyCopyDataHandler::QueryGetData(
    FORMATETC *pformatetc)
{
    HRESULT hRes = E_INVALIDARG;
    if (pformatetc != nullptr) {
        if (pformatetc->lindex != -1) {
            hRes = DV_E_LINDEX;
        } else if (pformatetc->dwAspect != DVASPECT_CONTENT) {
            hRes = DV_E_DVASPECT;
        } else if (pformatetc->cfFormat != CF_UNICODETEXT) {
            hRes = DV_E_CLIPFORMAT;
        } else if (pformatetc->tymed != TYMED_HGLOBAL) {
            hRes = DV_E_TYMED;
        } else {
            // Caller is asking for text in the proper format, it will work.
            hRes = S_OK;
        }
    }
    return hRes;
}

//
// IDataObject::GetCanonicalFormatEtc
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::GetCanonicalFormatEtc(
    FORMATETC* /*pformatectIn*/,
    FORMATETC* /*pformatetcOut*/)
{
    return E_NOTIMPL;
}

//
// IDataObject::SetData
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::SetData(
    FORMATETC* /*pformatetc*/,
    STGMEDIUM* /*pmedium*/,
    BOOL /*fRelease*/)
{
    return E_NOTIMPL;
}

//
// IDataObject::EnumFormatEtc
// Called by the shell to enumerate the formats we support.
//
// @param dwDirection Whether to return formats supported for setting or for getting.
// @param ppenumFormatEtc Pointer to location where to store an IEnumFORMATETC implementation
//                        that will provide all formats we support.
// @return S_OK if successful, otherwise an error code.
//
STDMETHODIMP CPathCopyCopyDataHandler::EnumFormatEtc(
    DWORD dwDirection,
    IEnumFORMATETC **ppenumFormatEtc)
{
    // We save the supported formats in the registry, so
    // we can use this simple API call to get the enumerator.
    HRESULT hRes = E_INVALIDARG;
    if (ppenumFormatEtc != nullptr) {
        hRes = ::OleRegEnumFormatEtc(__uuidof(PathCopyCopyDataHandler), dwDirection, ppenumFormatEtc);
    }
    return hRes;
}

//
// IDataObject::DAdvise
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::DAdvise(
    FORMATETC* /*pformatetc*/,
    DWORD /*advf*/,
    IAdviseSink* /*pAdvSink*/,
    DWORD* /*pdwConnection*/)
{
    return E_NOTIMPL;
}

//
// IDataObject::DUnadvise
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::DUnadvise(
    DWORD /*dwConnection*/)
{
    return E_NOTIMPL;
}

//
// IDataObject::EnumDAdvise
// This method is unused for shell data handlers.
//
STDMETHODIMP CPathCopyCopyDataHandler::EnumDAdvise(
    IEnumSTATDATA** /*ppenumAdvise*/)
{
    return E_NOTIMPL;
}
