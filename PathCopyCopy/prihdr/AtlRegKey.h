// AtlRegKey.h
// (c) 2015-2020, Charles Lechasseur
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
                        AtlRegKey() noexcept(false);
                        AtlRegKey(HKEY p_hParent,
                                  const wchar_t* p_pKeyPath,
                                  bool p_Create,
                                  REGSAM p_SecurityAccess = KEY_READ | KEY_WRITE);
    explicit            AtlRegKey(HKEY p_hKey) noexcept(false);
                        AtlRegKey(const AtlRegKey&) = delete;
    AtlRegKey&          operator=(const AtlRegKey&) = delete;

    bool                Valid() const noexcept(false) override;
    HKEY                GetHKEY() const noexcept;
    long                Open(HKEY p_hParent,
                             const wchar_t* p_pKeyPath,
                             bool p_Create,
                             REGSAM p_SecurityAccess = KEY_READ | KEY_WRITE) noexcept(false);

    long                QueryDWORDValue(const wchar_t* p_pValueName,
                                        DWORD& p_rValue) const noexcept(false) override;
    long                QueryQWORDValue(const wchar_t* p_pValueName,
                                        ULONGLONG& p_rValue) const noexcept(false) override;
    long                QueryGUIDValue(const wchar_t* p_pValueName,
                                       GUID& p_rValue) const noexcept(false) override;
    long                QueryValue(const wchar_t* p_pValueName,
                                   DWORD* p_pValueType,
                                   void* p_pValue,
                                   DWORD* p_pValueSize) const noexcept(false) override;

    void                GetValues(ValueInfoV& p_rvValues) const override;
    void                GetSubKeys(SubkeyInfoV& p_rvSubkeys) const override;

    long                SetDWORDValue(const wchar_t* p_pValueName,
                                      DWORD p_Value) noexcept(false) override;
    long                SetQWORDValue(const wchar_t* p_pValueName,
                                      ULONGLONG p_Value) noexcept(false) override;
    long                SetGUIDValue(const wchar_t* p_pValueName,
                                     const GUID& p_Value) noexcept(false) override;
    long                SetStringValue(const wchar_t* p_pValueName,
                                       const wchar_t* p_pValue) noexcept(false) override;

    long                DeleteValue(const wchar_t* p_pValueName) noexcept(false) override;

private:
    mutable ATL::CRegKey m_Key;     // Wrapper for the registry key.
};
