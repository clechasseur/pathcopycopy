// RegKey.h
// (c) 2015-2017, Charles Lechasseur
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

#include <string>
#include <vector>

#include <windows.h>


//
// RegKey
//
// Abstract interface to interact with a registry key.
// Not very generic; has methods needed for the PCC code.
//
class RegKey
{
public:
    // Info about values in this registry key.
    struct ValueInfo {
        HKEY            m_hKey;         // Key containing the value.
        std::wstring    m_ValueName;    // Name of this value.

                        ValueInfo();
                        ValueInfo(HKEY const p_hKey,
                                  const wchar_t* const p_pValueName);
    };
    typedef std::vector<ValueInfo> ValueInfoV;

    // Info about subkeys of this registry key.
    struct SubkeyInfo {
        HKEY            m_hParent;      // Parent of this subkey.
        std::wstring    m_KeyName;      // Name of the subkey.

                        SubkeyInfo();
                        SubkeyInfo(HKEY const p_hParent,
                                   const wchar_t* const p_pKeyName);
    };
    typedef std::vector<SubkeyInfo> SubkeyInfoV;

    virtual             ~RegKey();

                        //
                        // Checks if this wrapper's registry key is valid.
                        // This is required to be able to use other methods.
                        //
                        // @return true if registry key is valid.
                        //
    virtual bool        Valid() const = 0;

                        //
                        // Tries to load a DWORD value from the registry key.
                        //
                        // @param p_pValueName Name of value to load.
                        // @param p_rValue Where to store value.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        QueryDWORDValue(const wchar_t* const p_pValueName,
                                        DWORD& p_rValue) const = 0;

                        //
                        // Tries to load a QWORD value from the registry key.
                        //
                        // @param p_pValueName Name of value to load.
                        // @param p_rValue Where to store value.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        QueryQWORDValue(const wchar_t* const p_pValueName,
                                        ULONGLONG& p_rValue) const = 0;

                        //
                        // Tries to load a GUID value from the registry key (stored as a string).
                        //
                        // @param p_pValueName Name of value to load.
                        // @param p_rValue Where to store value.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        QueryGUIDValue(const wchar_t* const p_pValueName,
                                       GUID& p_rValue) const = 0;

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
    virtual long        QueryValue(const wchar_t* const p_pValueName,
                                   DWORD* const p_pValueType,
                                   void* const p_pValue,
                                   DWORD* const p_pValueSize) const = 0;

                        //
                        // Returns a list of values in this registry key.
                        //
                        // @param p_rvValues Where to store information about the values.
                        //
    virtual void        GetValues(ValueInfoV& p_rvValues) const = 0;

                        //
                        // Returns a list of subkeys of this registry key.
                        //
                        // @param p_rvSubkeys Where to store information about the subkeys.
                        //
    virtual void        GetSubKeys(SubkeyInfoV& p_rvSubkeys) const = 0;

                        //
                        // Tries to save a DWORD value in the registry key.
                        //
                        // @param p_pValueName Name of value to save.
                        // @param p_pValue Value to save.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        SetDWORDValue(const wchar_t* const p_pValueName,
                                      const DWORD p_Value) = 0;

                        //
                        // Tries to save a QWORD value in the registry key.
                        //
                        // @param p_pValueName Name of value to save.
                        // @param p_pValue Value to save.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        SetQWORDValue(const wchar_t* const p_pValueName,
                                      const ULONGLONG p_Value) = 0;

                        //
                        // Tries to save a GUID value in the registry key (as a string).
                        //
                        // @param p_pValueName Name of value to save.
                        // @param p_pValue Value to save.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        SetGUIDValue(const wchar_t* const p_pValueName,
                                     const GUID& p_Value) = 0;

                        //
                        // Tries to save a string value in the registry key.
                        //
                        // @param p_pValueName Name of value to save.
                        // @param p_pValue Value to save.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        SetStringValue(const wchar_t* const p_pValueName,
                                       const wchar_t* const p_pValue) = 0;

                        //
                        // Tries to delete a value from the registry key.
                        //
                        // @param p_pValueName Name of value to delete.
                        // @return Result code (ERROR_SUCCESS if it worked).
                        //
    virtual long        DeleteValue(const wchar_t* const p_pValueName) = 0;

protected:
                        RegKey() = default;
                        RegKey(const RegKey&) = default;
    RegKey&             operator=(const RegKey&) = default;
};
