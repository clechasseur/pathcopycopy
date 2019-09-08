// AtlRegKey.cpp
// (c) 2015-2019, Charles Lechasseur
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
#include <AtlRegKey.h>

#include <assert.h>


//
// Default constructor. Does not open the key.
//
AtlRegKey::AtlRegKey() noexcept(false)
    : RegKey(),
      m_Key()
{
}

//
// Constructor that opens or creates a key.
//
// @param p_hParent Handle of parent key.
// @param p_pKeyPath Path of registry key.
// @param p_Create Whether to create the registry key if it does not exist.
// @param p_SecurityAccess Desired security access. Defaults to read/write.
//
AtlRegKey::AtlRegKey(HKEY const p_hParent,
                     const wchar_t* const p_pKeyPath,
                     const bool p_Create,
                     const REGSAM p_SecurityAccess /*= KEY_READ | KEY_WRITE*/)
    : RegKey(),
      m_Key()
{
    Open(p_hParent, p_pKeyPath, p_Create, p_SecurityAccess);
}

//
// Constructor that takes ownership of a HKEY.
//
// @param p_hKey Existing key handle. We assume ownership.
//
AtlRegKey::AtlRegKey(HKEY const p_hKey) noexcept(false)
    : RegKey(),
      m_Key(p_hKey)
{
}

//
// Checks if the registry key has been successfully opened.
//
// @return true if registry key is valid.
//
bool AtlRegKey::Valid() const
{
    return m_Key.m_hKey != nullptr;
}

//
// Returns the handle of our registry key.
//
// @return Registry key handle. null if not open.
//
HKEY AtlRegKey::GetHKEY() const noexcept
{
    return m_Key.m_hKey;
}

//
// Opens or creates a key.
//
// @param p_hParent Handle of parent key.
// @param p_pKeyPath Path of registry key.
// @param p_Create Whether to create the registry key if it does not exist.
// @param p_SecurityAccess Desired security access. Defaults to read/write.
//
long AtlRegKey::Open(HKEY const p_hParent,
                     const wchar_t* const p_pKeyPath,
                     const bool p_Create,
                     const REGSAM p_SecurityAccess /*= KEY_READ | KEY_WRITE*/)
{
    assert(m_Key.m_hKey == nullptr);

    return p_Create
        ? m_Key.Create(p_hParent, p_pKeyPath, nullptr, REG_OPTION_NON_VOLATILE, p_SecurityAccess)
        : m_Key.Open(p_hParent, p_pKeyPath, p_SecurityAccess);
}

//
// Tries to load a DWORD value from the registry key.
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::QueryDWORDValue(const wchar_t* const p_pValueName,
                                DWORD& p_rValue) const
{
    return m_Key.QueryDWORDValue(p_pValueName, p_rValue);
}

//
// Tries to load a QWORD value from the registry key.
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::QueryQWORDValue(const wchar_t* const p_pValueName,
                                ULONGLONG& p_rValue) const
{
    return m_Key.QueryQWORDValue(p_pValueName, p_rValue);
}

//
// Tries to load a GUID value from the registry key (stored as a string).
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::QueryGUIDValue(const wchar_t* const p_pValueName,
                               GUID& p_rValue) const
{
    return m_Key.QueryGUIDValue(p_pValueName, p_rValue);
}

//
// Tries to load a value from the registry key.
//
// @param p_pValueName Name of value to load.
// @param p_pValueType If set, will receive the type of value.
// @param p_pValue Pointer to buffer where to store value. Can be null.
// @param p_pValueSize Pointer to variable containing the size of p_pValue.
//                     Upon exit, will contain the actual size of the
//                     value copied to p_pValue. Can be null only if p_pValue is too.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::QueryValue(const wchar_t* const p_pValueName,
                           DWORD* const p_pValueType,
                           void* const p_pValue,
                           DWORD* const p_pValueSize) const
{
    return m_Key.QueryValue(p_pValueName, p_pValueType, p_pValue, p_pValueSize);
}

//
// Returns a list of values in this registry key.
//
// @param p_rvValues Where to store information about the values.
//
void AtlRegKey::GetValues(ValueInfoV& p_rvValues) const
{
    // Scan values, starting with the first one.
    std::wstring valueName;
    valueName.resize(16384);    // See MSDN
    LONG res = ERROR_SUCCESS;
    for (DWORD index = 0; res == ERROR_SUCCESS; ++index) {
        DWORD valueNameSize = valueName.size();
        res = ::RegEnumValueW(m_Key.m_hKey, index, &*valueName.begin(), &valueNameSize,
                              nullptr, nullptr, nullptr, nullptr);
        if (res == ERROR_SUCCESS) {
            p_rvValues.emplace_back(m_Key.m_hKey, valueName.c_str());
        }
    }
}

//
// Returns a list of subkeys of this registry key.
//
// @param p_rvSubkeys Where to store information about the subkeys.
//
void AtlRegKey::GetSubKeys(SubkeyInfoV& p_rvSubkeys) const
{
    // Scan subkeys, starting with the first one.
    wchar_t subkeyName[256];        // See MSDN's RegEnumKeyEx.
    LONG res = ERROR_SUCCESS;
    for (DWORD index = 0; res == ERROR_SUCCESS; ++index) {
        DWORD subkeyNameSize = 256;
        res = m_Key.EnumKey(index, subkeyName, &subkeyNameSize);
        if (res == ERROR_SUCCESS) {
            p_rvSubkeys.emplace_back(m_Key.m_hKey, subkeyName);
        }
    }
}

//
// Tries to save a DWORD value in the registry key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::SetDWORDValue(const wchar_t* const p_pValueName,
                              const DWORD p_Value)
{
    return m_Key.SetDWORDValue(p_pValueName, p_Value);
}

//
// Tries to save a QWORD value in the registry key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::SetQWORDValue(const wchar_t* const p_pValueName,
                              const ULONGLONG p_Value)
{
    return m_Key.SetQWORDValue(p_pValueName, p_Value);
}

//
// Tries to save a GUID value in the registry key (as a string).
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::SetGUIDValue(const wchar_t* const p_pValueName,
                             const GUID& p_Value)
{
    return m_Key.SetGUIDValue(p_pValueName, p_Value);
}

//
// Tries to save a string value in the registry key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::SetStringValue(const wchar_t* const p_pValueName,
                               const wchar_t* const p_pValue)
{
    return m_Key.SetStringValue(p_pValueName, p_pValue);
}

//
// Tries to delete a value from the registry key.
//
// @param p_pValueName Name of value to delete.
// @return Result code (ERROR_SUCCESS if it worked).
//
long AtlRegKey::DeleteValue(const wchar_t* const p_pValueName)
{
    return m_Key.DeleteValue(p_pValueName);
}
