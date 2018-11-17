// AtlRegKey.h
// (c) 2015-2018, Charles Lechasseur
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

#include <RegKey.h>

#include <atlbase.h>
#include <windows.h>


//
// AtlRegKey
//
// Wrapper for an ATL CRegKey that implements our registry key interface.
//
class AtlRegKey final : public RegKey
{
public:
                        AtlRegKey();
                        AtlRegKey(HKEY const p_hParent,
                                  const wchar_t* const p_pKeyPath,
                                  const bool p_Create,
                                  const REGSAM p_SecurityAccess = KEY_READ | KEY_WRITE);
    explicit            AtlRegKey(HKEY const p_hKey);
                        AtlRegKey(const AtlRegKey&) = delete;
    AtlRegKey&          operator=(const AtlRegKey&) = delete;

    virtual bool        Valid() const override;
    HKEY                GetHKEY() const;
    long                Open(HKEY const p_hParent,
                             const wchar_t* const p_pKeyPath,
                             const bool p_Create,
                             const REGSAM p_SecurityAccess = KEY_READ | KEY_WRITE);

    virtual long        QueryDWORDValue(const wchar_t* const p_pValueName,
                                        DWORD& p_rValue) const override;
    virtual long        QueryQWORDValue(const wchar_t* const p_pValueName,
                                        ULONGLONG& p_rValue) const override;
    virtual long        QueryGUIDValue(const wchar_t* const p_pValueName,
                                       GUID& p_rValue) const override;
    virtual long        QueryValue(const wchar_t* const p_pValueName,
                                   DWORD* const p_pValueType,
                                   void* const p_pValue,
                                   DWORD* const p_pValueSize) const override;

    virtual void        GetValues(ValueInfoV& p_rvValues) const override;
    virtual void        GetSubKeys(SubkeyInfoV& p_rvSubkeys) const override;

    virtual long        SetDWORDValue(const wchar_t* const p_pValueName,
                                      const DWORD p_Value) override;
    virtual long        SetQWORDValue(const wchar_t* const p_pValueName,
                                      const ULONGLONG p_Value) override;
    virtual long        SetGUIDValue(const wchar_t* const p_pValueName,
                                     const GUID& p_Value) override;
    virtual long        SetStringValue(const wchar_t* const p_pValueName,
                                       const wchar_t* const p_pValue) override;

    virtual long        DeleteValue(const wchar_t* const p_pValueName) override;

private:
    mutable ATL::CRegKey m_Key;     // Wrapper for the registry key.
};
