// COMPlugin.cpp
// (c) 2008-2019, Charles Lechasseur
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
#include <COMPlugin.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor. Will create an instance of the COM plugin.
        //
        // @param p_CLSID ID of COM co-class that implements the plugin.
        //
        COMPlugin::COMPlugin(const CLSID& p_CLSID)
            : Plugin(),
              m_Id(p_CLSID),
              m_cpPlugin(),
              m_cpPluginGroup(),
              m_cpPluginState(),
              m_cpPluginIcon(),
              m_Description()
        {
            // Immediately create the plugin instance and make sure it works.
            HRESULT hRes = m_cpPlugin.CoCreateInstance(p_CLSID);
            if (FAILED(hRes)) {
                throw COMPluginError(hRes);
            }
            if (m_cpPlugin == NULL) {
                throw COMPluginError(E_FAIL);
            }

            // Immediately query for other interface implementations. This can fail.
            m_cpPluginGroup = m_cpPlugin;
            m_cpPluginState = m_cpPlugin;
            m_cpPluginIcon = m_cpPlugin;

            // Immediately get plugin description. It must work and return a non-empty string.
            ATL::CComBSTR bstrDescription;
            hRes = m_cpPlugin->get_Description(&bstrDescription);
            if (FAILED(hRes)) {
                throw COMPluginError(hRes);
            }
            if (bstrDescription == NULL || bstrDescription.Length() == 0) {
                throw COMPluginError(E_UNEXPECTED);
            }
            m_Description = bstrDescription.m_str;
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& COMPlugin::Id() const
        {
            return m_Id;
        }

        //
        // Returns the ID of the group to which the plugin belongs.
        // If the plugin does not implement grouping, will return 0.
        //
        // @return Group ID of plugin, or 0 if not supported.
        //
        ULONG COMPlugin::GroupId() const
        {
            ULONG groupId = 0;
            if (m_cpPluginGroup != NULL) {
                ULONG groupId2;
                HRESULT hRes = m_cpPluginGroup->get_GroupId(&groupId2);
                if (SUCCEEDED(hRes)) {
                    groupId = groupId2;
                }
            }
            return groupId;
        }

        //
        // Returns the position of the plugin within its group.
        // If the plugin does not implement grouping, will return 0.
        //
        // @return Plugin position in group, or 0 if not supported.
        //
        ULONG COMPlugin::GroupPosition() const
        {
            ULONG groupPos = 0;
            if (m_cpPluginGroup != NULL) {
                ULONG groupPos2;
                HRESULT hRes = m_cpPluginGroup->get_GroupPosition(&groupPos2);
                if (SUCCEEDED(hRes)) {
                    groupPos = groupPos2;
                }
            }
            return groupPos;
        }

        //
        // Returns the plugin description.
        //
        // @return Plugin description.
        //
        std::wstring COMPlugin::Description() const
        {
            // Return cached description.
            return m_Description;
        }

        //
        // Returns the plugin help text.
        //
        // @return Plugin description.
        //
        std::wstring COMPlugin::HelpText() const
        {
            // Get plugin help text. If it doesn't work, no worry.
            std::wstring helpText;
            ATL::CComBSTR bstrHelpText;
            HRESULT hRes = m_cpPlugin->get_HelpText(&bstrHelpText);
            if (SUCCEEDED(hRes) && bstrHelpText != NULL && bstrHelpText.Length() > 0) {
                helpText = bstrHelpText.m_str;
            }

            // Return help text or an empty string.
            return helpText;
        }

        //
        // Returns the path to the icon to use for this plugin.
        //
        // @return Path to icon file, or an empty string if not specified.
        //
        std::wstring COMPlugin::IconFile() const
        {
            // Check if plugin supports specifying an icon. Otherwise return an empty string.
            std::wstring iconFile;
            if (m_cpPluginIcon != NULL) {
                // Ask plugin for an icon file path.
                ATL::CComBSTR bstrIconFile;
                HRESULT hRes = m_cpPluginIcon->get_IconFile(&bstrIconFile);
                if (SUCCEEDED(hRes) && bstrIconFile != NULL) {
                    iconFile = bstrIconFile.m_str;
                }
            }
            return iconFile;
        }

        //
        // Determines if we should use the default icon for this plugin.
        //
        // @return true to use default icon, false otherwise.
        //
        bool COMPlugin::UseDefaultIcon() const
        {
            // Check if plugin supports specifying an icon. Otherwise return false.
            bool useDefault = false;
            if (m_cpPluginIcon != NULL) {
                // Ask plugin whether to use default icon.
                VARIANT_BOOL useDefaultVar = VARIANT_FALSE;
                HRESULT hRes = m_cpPluginIcon->get_UseDefaultIcon(&useDefaultVar);
                if (SUCCEEDED(hRes)) {
                    useDefault = useDefaultVar != VARIANT_FALSE;
                }
            }
            return useDefault;
        }

        //
        // Returns whether plugin should be enabled in the contextual menu.
        //
        // @param p_ParentPath Path of the parent directory of files that
        //                     triggered the contextual menu.
        // @param p_File Path of one file that was selected.
        // @return Whether plugin should be enabled or not in the contextual menu.
        //
        bool COMPlugin::Enabled(const std::wstring& p_ParentPath,
                                const std::wstring& p_File) const
        {
            // Check if plugin supports state changes. Otherwise assume it is enabled.
            bool enabled = true;
            if (m_cpPluginState != NULL) {
                // Ask plugin if it should be enabled. If an error
                // code is returned, do not enable the plugin.
                ATL::CComBSTR bstrParentPath(p_ParentPath.c_str());
                ATL::CComBSTR bstrFile(p_File.c_str());
                VARIANT_BOOL bEnabled = VARIANT_FALSE;
                HRESULT hRes = m_cpPluginState->Enabled(bstrParentPath, bstrFile, &bEnabled);
                enabled = SUCCEEDED(hRes) && hRes != S_FALSE && bEnabled != VARIANT_FALSE;
            }
            return enabled;
        }

        //
        // Transforms the given path using the plugin.
        //
        // @param p_File Full path to file.
        // @return Transformed path.
        //
        std::wstring COMPlugin::GetPath(const std::wstring& p_File) const
        {
            // Call method and make sure it works.
            // Note that it is legal for the method to return NULL or an empty string.
            ATL::CComBSTR bstrPath(p_File.c_str());
            ATL::CComBSTR bstrNewPath;
            HRESULT hRes = m_cpPlugin->GetPath(bstrPath, &bstrNewPath);
            if (FAILED(hRes)) {
                throw COMPluginError(hRes);
            }

            // Convert path if provided, otherwise reuse same one.
            std::wstring newPath(p_File);
            if (bstrNewPath != NULL && bstrNewPath.Length() > 0) {
                newPath = bstrNewPath.m_str;
            }
            return newPath;
        }

        //
        // Called by Path Copy Copy to know if it should honor the
        // "Drop redundant words" setting for this plugin. In our
        // case, the plugin description has been set by the COM
        // plugin developer so we never want to modify it.
        //
        // @return Always false to indicate PCC should not drop
        //         redundant words like "copy" from plugin's description.
        //
        bool COMPlugin::CanDropRedundantWords() const
        {
            return false;
        }

        //
        // Constructor with HRESULT.
        //
        // @param p_Result Result of COM call that went wrong.
        //
        COMPluginError::COMPluginError(const HRESULT p_Result)
            : std::exception(),
              m_Result(p_Result)
        {
        }

        //
        // Returns a textual description of the exception.
        //
        // @return Exception textual description.
        //
        const char* COMPluginError::what() const
        {
            return "COMPluginError";
        }

    } // namespace Plugins

} // namespace PCC
