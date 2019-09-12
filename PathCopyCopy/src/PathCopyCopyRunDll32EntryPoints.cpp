// PathCopyCopyRunDll32EntryPoints.cpp
// (c) 2012-2019, Charles Lechasseur
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
#include <AllPluginsProvider.h>
#include <AtlRegKey.h>
#include <PathCopyCopyRunDll32EntryPoints.h>
#include <PathCopyCopyPluginsRegistry.h>
#include <PathCopyCopySettings.h>
#include <StClipboard.h>
#include <StCoInitialize.h>
#include <StGlobalBlock.h>
#include <StGlobalLock.h>

#pragma warning(disable: 26461) // Some pointers could point to const, but per API they shouldn't


namespace
{
    // Separator used in the rundll32 command-line.
    const wchar_t           RUNDLL32_CMDLINE_SEPARATOR  = L',';

    // Registry key in HKEY_CURRENT_USER where to output rundll32 results.
    const wchar_t* const    PCC_RUNDLL32_OUTPUT_KEY     = L"Software\\clechasseur\\PathCopyCopy\\Rundll32Output";

} // anonymous namespace

//
// GetPathWithPluginW
//
// Function that can be called with rundll32.exe to invoke a specific
// plugin and save the resulting path in the clipboard. The command-line
// must first contain the plugin's GUID followed by the path to convert,
// separated by a comma. Call like this:
//
// rundll32.exe path\to\PCCxx.dll,GetPathWithPlugin {guid},path\to\convert
//
// p_hWnd         - Window handle to use as parent for our windows.
// p_hDllInstance - Instance handle for our DLL; ignored.
// p_pCmdLine     - Command-line passed to rundll32.
// p_ShowCmd      - How to show any window; ignored.
//
void CALLBACK GetPathWithPluginW(HWND p_hWnd,
                                 HINSTANCE /*p_hDllInstance*/,
                                 LPWSTR p_pCmdLine,
                                 int /*p_ShowCmd*/)
{
    // Initialize COM so that COM plugins can work.
    StCoInitialize coInit;

    // Assume we won't be able to convert the path.
    std::wstring resultingPath;

    try {
        // Parse command-line and separate the guid from the path.
        std::wstring cmdLine(p_pCmdLine);
        const auto sepPos = cmdLine.find(RUNDLL32_CMDLINE_SEPARATOR);
        if (sepPos != std::wstring::npos) {
            // Convert GUID and get the plugin.
            cmdLine.at(sepPos) = L'\0';
            CLSID pluginId = { 0 };
            if (SUCCEEDED(::CLSIDFromString(&*cmdLine.begin(), &pluginId))) {
                PCC::Settings settings;
                PCC::PluginSPV vspPlugins = PCC::PluginsRegistry::GetPluginsInDefaultOrder(&settings, &settings, true);
                PCC::PluginSPS sspAllPlugins(vspPlugins.cbegin(), vspPlugins.cend());
                PCC::AllPluginsProvider pluginProvider(sspAllPlugins);
                for (const PCC::PluginSP& spPlugin : sspAllPlugins) {
                    spPlugin->SetSettings(&settings);
                    spPlugin->SetPluginProvider(&pluginProvider);
                }
                auto it = sspAllPlugins.find(pluginId);
                if (it != sspAllPlugins.end()) {
                    // We got a plugin, now call its GetPath method.
                    const PCC::PluginSP& spPlugin = *it;
                    resultingPath = spPlugin->GetPath(std::wstring(cmdLine.begin() + sepPos + 1, cmdLine.end()));
                }
            }
        }
    } catch (...) {
        // Assume plugin won't work.
        resultingPath.clear();
    }

    // Copy resulting path to the clipboard.
    StClipboard acquireClipboard(p_hWnd);
    if (acquireClipboard.InitResult()) {
        // Allocate global block to store text.
        const size_t blockNumElements = resultingPath.size() + 1;
        const size_t blockSize = blockNumElements * sizeof(wchar_t);
        StGlobalBlock memBlock(GMEM_MOVEABLE, blockSize);
        if (memBlock.Get() != nullptr) {
            // Lock block and copy text in it.
            bool copied = false;
            {
                StGlobalLock lockBlock(memBlock.Get());
                void* pBlock = lockBlock.GetPtr();
                if (pBlock != nullptr) {
                    const errno_t copyErr = ::wcscpy_s(static_cast<wchar_t*>(pBlock),
                                                       blockNumElements,
                                                       resultingPath.c_str());
                    copied = copyErr == 0;
                }
            }

            if (copied) {
                // Save data in clipboard.
                const HANDLE hSavedData = ::SetClipboardData(CF_UNICODETEXT, memBlock.Get());
                if (hSavedData != nullptr) {
                    // Clipboard now owns the data, avoid freeing it.
                    memBlock.Release();
                }
            }
        }
    }
}

//
// RegGetPathWithPluginW
//
// Function that can be called with rundll32.exe to invoke a specific
// plugin and save the resulting path to a registry value. The command-line
// must first contain the plugin's GUID, then the value name, then the path
// to convert, all separated by commas. Call like this:
//
// rundll32.exe path\to\PCCxx.dll,RegGetPathWithPlugin {guid},reg_value_name,path\to\convert
//
// The resulting path will be saved in the specified registry value in
//
// HKEY_CURRENT_USER\Software\clechasseur\PathCopyCopy\Rundll32Output
//
// p_hWnd         - Window handle to use as parent for our windows.
// p_hDllInstance - Instance handle for our DLL; ignored.
// p_pCmdLine     - Command-line passed to rundll32.
// p_ShowCmd      - How to show any window; ignored.
//
void CALLBACK RegGetPathWithPluginW(HWND /*p_hWnd*/,
                                    HINSTANCE /*p_hDllInstance*/,
                                    LPWSTR p_pCmdLine,
                                    int /*p_ShowCmd*/)
{
    // Initialize COM so that COM plugins can work.
    StCoInitialize coInit;

    // Assume we won't be able to convert the path.
    std::wstring resultingPath;
    std::wstring regValueName;

    try {
        // Parse command-line and separate the guid from the value name and the path.
        std::wstring cmdLine(p_pCmdLine);
        std::wstring::size_type sepPos = cmdLine.find(RUNDLL32_CMDLINE_SEPARATOR);
        if (sepPos != std::wstring::npos) {
            // Convert GUID and get the plugin.
            cmdLine.at(sepPos) = L'\0';
            CLSID pluginId = { 0 };
            if (SUCCEEDED(::CLSIDFromString(&*cmdLine.begin(), &pluginId))) {
                PCC::Settings settings;
                PCC::PluginSPV vspPlugins = PCC::PluginsRegistry::GetPluginsInDefaultOrder(&settings, &settings, true);
                PCC::PluginSPS sspAllPlugins(vspPlugins.cbegin(), vspPlugins.cend());
                PCC::AllPluginsProvider pluginProvider(sspAllPlugins);
                for (const PCC::PluginSP& spPlugin : sspAllPlugins) {
                    spPlugin->SetSettings(&settings);
                    spPlugin->SetPluginProvider(&pluginProvider);
                }
                auto it = sspAllPlugins.find(pluginId);
                if (it != sspAllPlugins.end()) {
                    // Separate the value name from the path.
                    cmdLine.erase(cmdLine.begin(), cmdLine.begin() + sepPos + 1);
                    sepPos = cmdLine.find(RUNDLL32_CMDLINE_SEPARATOR);
                    if (sepPos != std::wstring::npos) {
                        // Extract registry value name and call GetPath method on plugin.
                        regValueName.assign(cmdLine.begin(), cmdLine.begin() + sepPos);
                        const PCC::PluginSP& spPlugin = *it;
                        resultingPath = spPlugin->GetPath(std::wstring(cmdLine.begin() + sepPos + 1, cmdLine.end()));
                    }
                }
            }
        }
    } catch (...) {
        // Assume plugin won't work.
        resultingPath.clear();
        regValueName.clear();
    }

    // Save resulting path to the registry.
    if (!resultingPath.empty() && !regValueName.empty()) {
        AtlRegKey outputKey(HKEY_CURRENT_USER, PCC_RUNDLL32_OUTPUT_KEY, true, KEY_SET_VALUE);
        if (outputKey.Valid()) {
            outputKey.SetStringValue(regValueName.c_str(), resultingPath.c_str());
        }
    }
}

//
// ApplyGlobalRevisionsW
//
// Function that can be called with rundll32.exe to apply config revisions
// to the global config registry key in HKEY_LOCAL_MACHINE. This is meant to
// be called by the setup. Call like this:
//
// rundll32.exe path\to\PCCxx.dll,ApplyGlobalRevisions
//
// p_hWnd         - Window handle to use as parent for our windows.
// p_hDllInstance - Instance handle for our DLL; ignored.
// p_pCmdLine     - Command-line passed to rundll32.
// p_ShowCmd      - How to show any window; ignored.
//
void CALLBACK ApplyGlobalRevisionsW(HWND /*p_hWnd*/,
                                    HINSTANCE /*p_hDllInstance*/,
                                    LPWSTR /*p_pCmdLine*/,
                                    int /*p_ShowCmd*/)
{
    try {
        PCC::Settings::ApplyGlobalRevisions();
    } catch (...) {
        // Can't do much, don't crash rundll32.
    }
}

//
// ApplyUserRevisionsW
//
// Function that can be called with rundll32.exe to apply config revisions
// to the user config registry key in HKEY_CURRENT_USER. This is meant to
// be called by the setup. Call like this:
//
// rundll32.exe path\to\PCCxx.dll,ApplyUserRevisions
//
// p_hWnd         - Window handle to use as parent for our windows.
// p_hDllInstance - Instance handle for our DLL; ignored.
// p_pCmdLine     - Command-line passed to rundll32.
// p_ShowCmd      - How to show any window; ignored.
//
void CALLBACK ApplyUserRevisionsW(HWND /*p_hWnd*/,
                                  HINSTANCE /*p_hDllInstance*/,
                                  LPWSTR /*p_pCmdLine*/,
                                  int /*p_ShowCmd*/)
{
    try {
        PCC::Settings().ApplyRevisions();
    } catch (...) {
        // Can't do much, don't crash rundll32.
    }
}
