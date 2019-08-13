// UserOverrideableRegKey.cpp
// (c) 2011-2019, Charles Lechasseur
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
#include <UserOverrideableRegKey.h>

#include <algorithm>
#include <utility>

#include <string.h>


namespace
{
    // Registry value that is used to lock out users of overriding a key.
    const wchar_t   VALUE_NAME_LOCKED_OUT[]     = L"KeyLock";

} // anonymous namespace

//
// Constructor. Opens the registry keys immediately. The global key in HKLM
// is only open for reading; the user key is open in read/write mode.
//
// @param p_pKeyPath Path of registry key.
// @param p_pUserKeyPath If set to a non-null value, we will use p_pKeyPath
//                       for the global key only and this value for the user key.
//
UserOverrideableRegKey::UserOverrideableRegKey(const wchar_t* const p_pKeyPath,
                                               const wchar_t* const p_pUserKeyPath /*= nullptr*/)
    : RegKey(),
      m_GlobalKey(HKEY_LOCAL_MACHINE, p_pKeyPath, false, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS),
      m_UserKey(HKEY_CURRENT_USER, p_pUserKeyPath != nullptr ? p_pUserKeyPath : p_pKeyPath, true,
                KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS)
{
}

//
// Checks if this registry key is valid (e.g. has been opened successfully).
// This only checks the user key, since it's possible for the global key to be non-existent.
//
// @return true if registry key is valid.
//
bool UserOverrideableRegKey::Valid() const
{
    // We don't need the global key, but we do need the user key.
    return m_UserKey.Valid();
}

//
// Checks whether the user registry key is currently locked in the global key.
// This allows administrators to set options and disallow changing them.
//
// @return true if the user key is locked and cannot be edited.
//
bool UserOverrideableRegKey::Locked() const
{
    bool locked = false;
    DWORD regLocked = 0;
    if (m_GlobalKey.Valid() && m_GlobalKey.QueryDWORDValue(VALUE_NAME_LOCKED_OUT, regLocked) == ERROR_SUCCESS) {
        locked = regLocked != 0;
    }
    return locked;
}

//
// Returns a reference to the global key in HKEY_LOCAL_MACHINE.
//
// @return Reference to global key. This key might be invalid if user does not have proper
//         access or if the key does not exist.
//
const RegKey& UserOverrideableRegKey::GetGlobalKey() const
{
    return m_GlobalKey;
}

//
// Returns a reference to the user key in HKEY_CURRENT_USER.
//
// @return Reference to user key.
//
const RegKey& UserOverrideableRegKey::GetUserKey() const
{
    return m_UserKey;
}

//
// Tries to load a DWORD value from the registry key.
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::QueryDWORDValue(const wchar_t* const p_pValueName,
                                             DWORD& p_rValue) const
{
    long res = ERROR_FILE_NOT_FOUND;
    if (!Locked()) {
        res = m_UserKey.QueryDWORDValue(p_pValueName, p_rValue);
    } else {
        res = ERROR_ACCESS_DENIED;
    }
    if (res != ERROR_SUCCESS && m_GlobalKey.Valid()) {
        res = m_GlobalKey.QueryDWORDValue(p_pValueName, p_rValue);
    }
    return res;
}

//
// Tries to load a QWORD value from the registry key.
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::QueryQWORDValue(const wchar_t* const p_pValueName,
                                             ULONGLONG& p_rValue) const
{
    long res = ERROR_FILE_NOT_FOUND;
    if (!Locked()) {
        res = m_UserKey.QueryQWORDValue(p_pValueName, p_rValue);
    } else {
        res = ERROR_ACCESS_DENIED;
    }
    if (res != ERROR_SUCCESS && m_GlobalKey.Valid()) {
        res = m_GlobalKey.QueryQWORDValue(p_pValueName, p_rValue);
    }
    return res;
}

//
// Tries to load a GUID value from the registry key (stored as a string).
//
// @param p_pValueName Name of value to load.
// @param p_rValue Where to store value.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::QueryGUIDValue(const wchar_t* const p_pValueName,
                                            GUID& p_rValue) const
{
    long res = ERROR_FILE_NOT_FOUND;
    if (!Locked()) {
        res = m_UserKey.QueryGUIDValue(p_pValueName, p_rValue);
    } else {
        res = ERROR_ACCESS_DENIED;
    }
    if (res != ERROR_SUCCESS && m_GlobalKey.Valid()) {
        res = m_GlobalKey.QueryGUIDValue(p_pValueName, p_rValue);
    }
    return res;
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
long UserOverrideableRegKey::QueryValue(const wchar_t* const p_pValueName,
                                        DWORD* const p_pValueType,
                                        void* const p_pValue,
                                        DWORD* const p_pValueSize) const
{
    long res = ERROR_FILE_NOT_FOUND;
    if (!Locked()) {
        res = m_UserKey.QueryValue(p_pValueName, p_pValueType, p_pValue, p_pValueSize);
    } else {
        res = ERROR_ACCESS_DENIED;
    }
    if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA && m_GlobalKey.Valid()) {
        res = m_GlobalKey.QueryValue(p_pValueName, p_pValueType, p_pValue, p_pValueSize);
    }
    return res;
}

//
// Scans both the user and global keys and returns a list of all values
// in both keys. Values in the user key take precedence over those in
// the global key.
//
// @param p_rvValues Where to store information about the values.
//
void UserOverrideableRegKey::GetValues(ValueInfoV& p_rvValues) const
{
    ValueInfoV vValues;

    // Get values in user key first, then in global key.
    if (!Locked()) {
        m_UserKey.GetValues(vValues);
    }
    if (m_GlobalKey.Valid()) {
        m_GlobalKey.GetValues(vValues);
    }

    // Sort values in the vector according to their name using stable_sort.
    // This way, for values that appear in both keys, the value in the user
    // key will be before that of the global key.
    auto compareValueInfos = [](const ValueInfo& p_Value1, const ValueInfo& p_Value2) {
        return ::_wcsicmp(p_Value1.m_ValueName.c_str(), p_Value2.m_ValueName.c_str()) < 0;
    };
    std::stable_sort(vValues.begin(), vValues.end(), compareValueInfos);

    // Now remove duplicates, which will remove conflicting values from the global key.
    auto valueInfosEqual = [](const ValueInfo& p_Value1, const ValueInfo& p_Value2) {
        return ::_wcsicmp(p_Value1.m_ValueName.c_str(), p_Value2.m_ValueName.c_str()) == 0;
    };
    vValues.erase(std::unique(vValues.begin(), vValues.end(), valueInfosEqual), vValues.end());

    // Return the resulting values.
    if (p_rvValues.empty()) {
        p_rvValues = std::move(vValues);
    } else {
        p_rvValues.insert(p_rvValues.end(), vValues.cbegin(), vValues.cend());
    }
}

//
// Scans both the user and global keys to find subkeys and returns
// a list of all such subkeys. Subkeys in the user key take precedence
// over those in the global key.
//
// @param p_rvSubkeys Where to store information about the subkeys.
//
void UserOverrideableRegKey::GetSubKeys(SubkeyInfoV& p_rvSubkeys) const
{
    // This is pretty much the same thing as GetValues.
    SubkeyInfoV vSubkeys;

    if (!Locked()) {
        m_UserKey.GetSubKeys(vSubkeys);
    }
    if (m_GlobalKey.Valid()) {
        m_GlobalKey.GetSubKeys(vSubkeys);
    }

    auto compareSubkeyInfos = [](const SubkeyInfo& p_Subkey1, const SubkeyInfo& p_Subkey2) {
        return ::_wcsicmp(p_Subkey1.m_KeyName.c_str(), p_Subkey2.m_KeyName.c_str()) < 0;
    };
    std::stable_sort(vSubkeys.begin(), vSubkeys.end(), compareSubkeyInfos);

    auto subkeyInfosEqual = [](const SubkeyInfo& p_Subkey1, const SubkeyInfo& p_Subkey2) {
        return ::_wcsicmp(p_Subkey1.m_KeyName.c_str(), p_Subkey2.m_KeyName.c_str()) == 0;
    };
    vSubkeys.erase(std::unique(vSubkeys.begin(), vSubkeys.end(), subkeyInfosEqual), vSubkeys.end());

    if (p_rvSubkeys.empty()) {
        p_rvSubkeys = std::move(vSubkeys);
    } else {
        p_rvSubkeys.insert(p_rvSubkeys.end(), vSubkeys.cbegin(), vSubkeys.cend());
    }
}

//
// Tries to save a DWORD value in the registry key.
// This will always write the value to the user key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::SetDWORDValue(const wchar_t* const p_pValueName,
                                           const DWORD p_Value)
{
    long res = ERROR_ACCESS_DENIED;
    if (!Locked()) {
        res = m_UserKey.SetDWORDValue(p_pValueName, p_Value);
    }
    return res;
}

//
// Tries to save a QWORD value in the registry key.
// This will always write the value to the user key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::SetQWORDValue(const wchar_t* const p_pValueName,
                                           const ULONGLONG p_Value)
{
    long res = ERROR_ACCESS_DENIED;
    if (!Locked()) {
        res = m_UserKey.SetQWORDValue(p_pValueName, p_Value);
    }
    return res;
}

//
// Tries to save a GUID value in the registry key (as a string).
// This will always write the value to the user key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::SetGUIDValue(const wchar_t* const p_pValueName,
                                          const GUID& p_Value)
{
    long res = ERROR_ACCESS_DENIED;
    if (!Locked()) {
        res = m_UserKey.SetGUIDValue(p_pValueName, p_Value);
    }
    return res;
}

//
// Tries to save a string value in the registry key. This will always
// write the value to the user key.
//
// @param p_pValueName Name of value to save.
// @param p_pValue Value to save.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::SetStringValue(const wchar_t* const p_pValueName,
                                            const wchar_t* const p_pValue)
{
    long res = ERROR_ACCESS_DENIED;
    if (!Locked()) {
        res = m_UserKey.SetStringValue(p_pValueName, p_pValue);
    }
    return res;
}

//
// Tries to delete a value from the user registry key.
// NOTE: if a similar value is found in the global registry key,
// that value is unchanged and will thus come out on the next Query!
//
// @param p_pValueName Name of value to delete.
// @return Result code (ERROR_SUCCESS if it worked).
//
long UserOverrideableRegKey::DeleteValue(const wchar_t* const p_pValueName)
{
    long res = ERROR_ACCESS_DENIED;
    if (!Locked()) {
        res = m_UserKey.DeleteValue(p_pValueName);
    }
    return res;
}
