// PathCopyCopyCOMPluginExecutor.cpp
// (c) 2014-2019, Charles Lechasseur
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

#include <StCoInitialize.h>
#include <PathCopyCopy_i.h>


// Functions in this file
int GetDescription(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);
int GetHelpText(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);
int GetGroupId(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);
int GetGroupPosition(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);
int GetIconFile(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);
int GetUseDefaultIcon(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput);


//
// Main program entry point.
//
// @param argc Number of command-line arguments received
// @param argv Array of command-line arguments
// @return Process exit code
//
#pragma warning(suppress: 26485) // Not sure what to do with this one, this is generated code
int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
    // Assume we'll fail.
    int retVal = -1;

    // Initialize COM since we're dealing with COM plugins.
    StCoInitialize initCom;
    if (SUCCEEDED(initCom.GetInitResult())) {
        // Ask user for plugin ID and command to execute.
        std::wstring pluginIdString, command;
        std::wcout << L"Plugin ID: ";
        std::getline(std::wcin, pluginIdString);
        std::wcout << L"Command: ";
        std::getline(std::wcin, command);

        // Output a line ending here. We need this as a separator for the settings app.
        std::wcout << std::endl;

        // Convert the plugin ID to a CLSID.
        CLSID pluginId = { 0 };
        HRESULT hRes = ::CLSIDFromString(pluginIdString.c_str(), &pluginId);
        if (SUCCEEDED(hRes)) {
            // Create the COM plugin.
            ATL::CComPtr<IPathCopyCopyPlugin> cpPlugin;
            hRes = cpPlugin.CoCreateInstance(pluginId);
            if (SUCCEEDED(hRes)) {
                // Check the command.
                std::wstring output;
                if (command == L"get_Description") {
                    retVal = GetDescription(cpPlugin, output);
                } else if (command == L"get_HelpText") {
                    retVal = GetHelpText(cpPlugin, output);
                } else if (command == L"get_GroupId") {
                    retVal =  GetGroupId(cpPlugin, output);
                } else if (command == L"get_GroupPosition") {
                    retVal = GetGroupPosition(cpPlugin, output);
                } else if (command == L"get_IconFile") {
                    retVal = GetIconFile(cpPlugin, output);
                } else if (command == L"get_UseDefaultIcon") {
                    retVal = GetUseDefaultIcon(cpPlugin, output);
                } else {
                    std::wcout << L"ERROR! Invalid command: " << command << std::endl;
                }

                // If everything went fine, write output.
                if (retVal >= 0) {
                    std::wcout << L"Output: " << output << std::endl;
                }
            } else {
                std::wcout << L"ERROR! Failed to create plugin: 0x" << std::hex << hRes << std::endl;
            }
        } else {
            std::wcout << L"ERROR! Invalid plugin ID: 0x" << std::hex << hRes << std::endl;
        }
    } else {
        std::wcout << L"ERROR! Could not initialize COM: 0x" << std::hex << initCom.GetInitResult() << std::endl;
    }

	return retVal;
}

//
// Called to get the description for a COM plugin. Command: get_Description
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetDescription(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    if (p_pPlugin != nullptr) {
        ATL::CComBSTR bstrDescription;
        if (SUCCEEDED(p_pPlugin->get_Description(&bstrDescription)) && bstrDescription != nullptr) {
            p_rOutput = bstrDescription.m_str;
        }
    }
    
    return 0;
}

//
// Called to get the help text for a COM plugin. Command: get_Description
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetHelpText(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    if (p_pPlugin != nullptr) {
        ATL::CComBSTR bstrHelpText;
        if (SUCCEEDED(p_pPlugin->get_HelpText(&bstrHelpText)) && bstrHelpText != nullptr) {
            p_rOutput = bstrHelpText.m_str;
        }
    }
    
    return 0;
}

//
// Called to get the group ID for a COM plugin. Command: get_GroupId
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetGroupId(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    // Ask for the group state interface. If it's not supported or if the
    // method returns an error code, assume default group ID.
    ATL::CComQIPtr<IPathCopyCopyPluginGroupInfo> cpPluginGroupInfo(p_pPlugin);
    if (cpPluginGroupInfo.p != nullptr) {
        ULONG groupId = 0;
        if (SUCCEEDED(cpPluginGroupInfo->get_GroupId(&groupId))) {
            std::wstringstream wss;
            wss << groupId;
            p_rOutput = wss.str();
        }
    }
    
    return 0;
}

//
// Called to get the group position for a COM plugin. Command: get_GroupPosition
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetGroupPosition(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    // Ask for the group state interface. If it's not supported or if the
    // method returns an error code, assume default group position.
    ATL::CComQIPtr<IPathCopyCopyPluginGroupInfo> cpPluginGroupInfo(p_pPlugin);
    if (cpPluginGroupInfo.p != nullptr) {
        ULONG groupPosition = 0;
        if (SUCCEEDED(cpPluginGroupInfo->get_GroupPosition(&groupPosition))) {
            std::wstringstream wss;
            wss << groupPosition;
            p_rOutput = wss.str();
        }
    }
    
    return 0;
}

//
// Called to get the icon file for a COM plugin. Command: get_IconFile
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetIconFile(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    // Ask for the icon state interface. If it's not supported or if the
    // method returns an error code, assume no icon file is specified.
    ATL::CComQIPtr<IPathCopyCopyPluginIconInfo> cpPluginIconInfo(p_pPlugin);
    if (cpPluginIconInfo.p != nullptr) {
        ATL::CComBSTR bstrIconFile;
        if (SUCCEEDED(cpPluginIconInfo->get_IconFile(&bstrIconFile)) && bstrIconFile != nullptr) {
            p_rOutput = bstrIconFile.m_str;
        }
    }
    
    return 0;
}

//
// Called to check if a COM plugin should use the default icon. Command: get_UseDefaultIcon
//
// @param p_pPlugin Plugin to query.
// @param p_rOutput Where to store output.
// @return Result code to return as program exit code.
//
int GetUseDefaultIcon(IPathCopyCopyPlugin* p_pPlugin, std::wstring& p_rOutput)
{
    assert(p_rOutput.empty());

    // Ask for the icon state interface. If it's not supported or if the
    // method returns an error code, assume we don't use default icon.
    bool useDefaultIcon = false;
    ATL::CComQIPtr<IPathCopyCopyPluginIconInfo> cpPluginIconInfo(p_pPlugin);
    if (cpPluginIconInfo.p != nullptr) {
        VARIANT_BOOL useDefaultIconVar = VARIANT_FALSE;
        if (SUCCEEDED(cpPluginIconInfo->get_UseDefaultIcon(&useDefaultIconVar))) {
            useDefaultIcon = useDefaultIconVar != VARIANT_FALSE;
        }
    }

    // Convert result to string form.
    p_rOutput = (useDefaultIcon ? L"true" : L"false");
    
    return 0;
}
