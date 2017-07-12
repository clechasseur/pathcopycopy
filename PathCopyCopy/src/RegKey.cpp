// RegKey.cpp
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

#include <stdafx.h>
#include <RegKey.h>

//
// Destructor.
//
RegKey::~RegKey()
{
}

//
// Default constructor.
//
RegKey::ValueInfo::ValueInfo()
    : m_hKey(NULL),
      m_ValueName()
{
}

//
// Parametrized constructor.
//
// @param p_hKey Handle of key containing the registry value.
// @param p_pValueName Name of value.
//
RegKey::ValueInfo::ValueInfo(HKEY const p_hKey,
                             const wchar_t* const p_pValueName)
    : m_hKey(p_hKey),
      m_ValueName(p_pValueName)
{
}

//
// Default constructor.
//
RegKey::SubkeyInfo::SubkeyInfo()
    : m_hParent(NULL),
      m_KeyName()
{
}

//
// Parametrized constructor.
//
// @param p_hParent Handle of parent key.
// @param p_pKeyName Name of subkey.
//
RegKey::SubkeyInfo::SubkeyInfo(HKEY const p_hParent,
                               const wchar_t* const p_pKeyName)
    : m_hParent(p_hParent),
      m_KeyName(p_pKeyName)
{
}
