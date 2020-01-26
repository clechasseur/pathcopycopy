// StAtlPerUserOverride.h
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

#include <atlbase.h>
#include <windows.h>


//
// StAtlPerUserOverride
//
// Stack-based class that will look at the value returned by the
// AtlGetPerUserRegistration function and if per-user registration
// is requested, will override the HKEY_CLASSES_ROOT predefined
// registry key with HKEY_CURRENT_USER\Software\Classes. Idea taken from:
//
// http://blogs.msdn.com/b/jaredpar/archive/2005/05/29/423000.aspx
//
class StAtlPerUserOverride final
{
public:
                        //
                        // Constructor. Will override HKEY_CLASSES_ROOT
                        // if this is a per-user registration.
                        //
                        StAtlPerUserOverride() noexcept(false)
                            : m_Overriden(false),
                              m_Succeeded(false)
                        {
                            // Check if this is a per-user registration.
                            bool perUser = false;
                            const HRESULT hRes = ATL::AtlGetPerUserRegistration(&perUser);
                            if (SUCCEEDED(hRes)) {
                                if (perUser) {
                                    // Override HKCR with the user CR key.
                                    ATL::CRegKey softwareClassesKey;
                                    softwareClassesKey.Create(HKEY_CURRENT_USER, L"Software\\Classes", nullptr, REG_OPTION_NON_VOLATILE,
                                                              KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY);
                                    if (softwareClassesKey.m_hKey != nullptr &&
                                        ::RegOverridePredefKey(HKEY_CLASSES_ROOT, softwareClassesKey) == ERROR_SUCCESS) {

                                        // Success overriding the key.
                                        m_Overriden = true;
                                        m_Succeeded = true;
                                    }
                                } else {
                                    // Not a per-user registration, but still a success.
                                    m_Succeeded = true;
                                }
                            }
                        }

                        //
                        // Copying/moving not supported.
                        //
                        StAtlPerUserOverride(const StAtlPerUserOverride&) = delete;
                        StAtlPerUserOverride(StAtlPerUserOverride&&) = delete;
    StAtlPerUserOverride&
                        operator=(const StAtlPerUserOverride&) = delete;
    StAtlPerUserOverride&
                        operator=(StAtlPerUserOverride&&) = delete;

                        //
                        // Destructor. Restores the default mapping of
                        // HKEY_CLASSES_ROOT if overridden in constructor.
                        //
                        ~StAtlPerUserOverride()
                        {
                            // If HKCR was overridden, restore default mapping.
                            if (m_Overriden) {
                                ::RegOverridePredefKey(HKEY_CLASSES_ROOT, nullptr);
                            }
                        }

                        //
                        // Check if the HKEY_CLASSES_ROOT has been overriden in the constructor.
                        //
                        // @return true if HKEY_CLASSES_ROOT has been overriden.
                        //
    bool                Overridden() const noexcept
                        {
                            return m_Overriden;
                        }

                        //
                        // Checks if everything succeeded in constructor. Note that it can
                        // succeed and still not override HKEY_CLASSES_ROOT if it's not needed.
                        //
                        // @return true if everything went smoothly in constructor.
                        //
    bool                Succeeded() const noexcept
                        {
                            return m_Succeeded;
                        }

private:
    bool                m_Overriden;        // Whether HKEY_CLASSES_ROOT has been overridden in constructor.
    bool                m_Succeeded;        // Whether everything worked in constructor.
};
