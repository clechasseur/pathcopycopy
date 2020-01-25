// UserOverrideableRegKey.h
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

#include "AtlRegKey.h"
#include "RegKey.h"


//
// UserOverrideableRegKey
//
// Wrapper for a registry key that exists in both CURRENT_USER and LOCAL_MACHINE.
// Values in HKCU will override those found in HKLM.
//
class UserOverrideableRegKey final : public RegKey
{
public:
    explicit            UserOverrideableRegKey(const wchar_t* p_pKeyPath,
                                               const wchar_t* p_pUserKeyPath = nullptr);
                        UserOverrideableRegKey(const UserOverrideableRegKey&) = delete;
    UserOverrideableRegKey&
                        operator=(const UserOverrideableRegKey&) = delete;

    bool                Valid() const override;
    bool                Locked() const;
    const RegKey&       GetGlobalKey() const noexcept;
    const RegKey&       GetUserKey() const noexcept;

    long                QueryDWORDValue(const wchar_t* p_pValueName,
                                        DWORD& p_rValue) const override;
    long                QueryQWORDValue(const wchar_t* p_pValueName,
                                        ULONGLONG& p_rValue) const override;
    long                QueryGUIDValue(const wchar_t* p_pValueName,
                                       GUID& p_rValue) const override;
    long                QueryValue(const wchar_t* p_pValueName,
                                   DWORD* p_pValueType,
                                   void* p_pValue,
                                   DWORD* p_pValueSize) const override;

    void                GetValues(ValueInfoV& p_rvValues) const override;
    void                GetSubKeys(SubkeyInfoV& p_rvSubkeys) const override;

    long                SetDWORDValue(const wchar_t* p_pValueName,
                                      DWORD p_Value) override;
    long                SetQWORDValue(const wchar_t* p_pValueName,
                                      ULONGLONG p_Value) override;
    long                SetGUIDValue(const wchar_t* p_pValueName,
                                     const GUID& p_Value) override;
    long                SetStringValue(const wchar_t* p_pValueName,
                                       const wchar_t* p_pValue) override;

    long                DeleteValue(const wchar_t* p_pValueName) override;

    std::shared_ptr<RegKey>
                        CreateSubKey(const wchar_t* p_pKeyName) override;

private:
    const std::wstring  m_KeyPath;          // Path of our registry key.
    const std::wstring  m_UserKeyPath;      // Path to the user part of our registry key.
    AtlRegKey           m_GlobalKey;        // Wrapper for the global key in HKLM.
    AtlRegKey           m_UserKey;          // Wrapper for user key in HKCU.
};
