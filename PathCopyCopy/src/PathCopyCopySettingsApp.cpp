// PathCopyCopySettingsApp.cpp
// (c) 2012-2017, Charles Lechasseur
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
#include <PathCopyCopySettingsApp.h>
#include <resource.h>

#include <assert.h>


// Image base is provided by the linker. We can use it to locate our DLL. See Launch().
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace PCC
{
    //
    // Constructor.
    //
    SettingsApp::SettingsApp()
    {
    }

    //
    // Launches the settings application using the given command.
    //
    // @param p_Options Program options. See header for default values.
    //
    void SettingsApp::Launch(const Options& p_Options /*= Options()*/)
    {
        // Get path to our module using the ImageBase trick.
        // See http://www.codeproject.com/KB/DLL/DLLModuleFileName.aspx
        wchar_t dllPath[MAX_PATH + 1];
        DWORD siz = ::GetModuleFileNameW((HINSTANCE)&__ImageBase, dllPath, MAX_PATH + 1);
        if (siz > 0) {
            // Remove the filename and replace it with the name of the settings app.
            std::wstring path(dllPath);
            std::wstring::size_type delimPos = path.find_last_of(L"/\\");
            if (delimPos != std::wstring::npos) {
                path.erase(delimPos);
            }
            ATL::CStringW settingsAppName(MAKEINTRESOURCEW(IDS_SETTINGS_APP_FILE_NAME));
            path += L"\\";
            path += (LPCWSTR) settingsAppName;

            // Build parameters.
            std::wstring params = L"/frompcc";
#ifndef _WIN64
            params += L" /bitness:x86";
#else
            params += L" /bitness:x64";
#endif
            if (p_Options.m_UpdateCheck) {
                params += L" /updatecheck";
            }

            // Launch the settings app and don't wait for the result.
            SHELLEXECUTEINFOW execInfo = { 0 };
            execInfo.cbSize = sizeof(execInfo);
            execInfo.fMask = 0;
            execInfo.hwnd = 0;
            execInfo.lpVerb = 0;
            execInfo.lpFile = path.c_str();
            execInfo.lpParameters = params.c_str();
            execInfo.lpDirectory = 0;
            execInfo.nShow = SW_SHOWNORMAL;
            ::ShellExecuteExW(&execInfo);
        }
    }

    //
    // Constructor.
    //
    SettingsApp::Options::Options()
        : m_UpdateCheck(false)
    {
    }

    //
    // Enables the flag that tells the settings app to check for updates.
    //
    // @return Reference to this, for chaining.
    //
    SettingsApp::Options& SettingsApp::Options::WithUpdateCheck()
    {
        m_UpdateCheck = true;
        return *this;
    }
}
