// PathCopyCopyContextMenuExt.cpp
// (c) 2008-2020, Charles Lechasseur
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
#include <PathCopyCopyContextMenuExt.h>
#include <DefaultPlugin.h>
#include <dllmain.h>
#include <PathCopyCopyPluginsRegistry.h>
#include <PathCopyCopySettings.h>
#include <PathCopyCopySettingsApp.h>
#include <PluginUtils.h>
#include <PathAction.h>
#include <StGdiplusStartup.h>
#include <StGlobalBlock.h>
#include <StGlobalLock.h>
#include <StStgMedium.h>

#include <algorithm>
#include <functional>
#include <set>

#include <gdiplus.h>

#pragma warning(disable: 26426) // Globals are only used by COM objects created later, so we're OK here
#pragma warning(disable: 26490) // Need reinterpret_cast to use some weird Win32 APIs

namespace {

const wchar_t* const DEFAULT_PATHS_SEPARATOR = L"\r\n"; // Default separator used between paths when copying multiple file names.

}

// CPathCopyCopyContextMenuExt

// Static members
CPathCopyCopyContextMenuExt::ExtToMenuPairV CPathCopyCopyContextMenuExt::s_vExtToMenus;
std::mutex                                  CPathCopyCopyContextMenuExt::s_ExtToMenusLock;

//
// Constructor.
//
CPathCopyCopyContextMenuExt::CPathCopyCopyContextMenuExt() noexcept(false)
    : m_spSettings(),
      m_vspPluginsInDefaultOrder(),
      m_sspAllPlugins(),
      m_spPluginProvider(),
      m_vFiles(),
      m_FirstCmdId(),
      m_SubMenuCmdId(),
      m_SettingsCmdId(),
      m_mPluginsByCmdId(),
      m_spPCCIcon(),
      m_mspIcons()
{
}

//
// Destructor.
//
CPathCopyCopyContextMenuExt::~CPathCopyCopyContextMenuExt()
{
    try {
        // Remove this instance from the map of instances modifying menus (if it's there).
        RemoveFromExtToMenu();

        // Check for updates, but ONLY if settings were created. Otherwise, it means
        // that either COM object hasn't been used by the shell or it was used to register plugins.
        // In both cases we don't want to check for updates.
        if (m_spSettings != nullptr) {
            CheckForUpdates();
        }
    } catch (...) {
        // Don't throw from destructor, it could crash COM app
    }
}

//
// IPathCopyCopyContextMenuExt::RegisterPlugin
//
// Registers a plugin component in the Path Copy Copy registry. The plugin
// will be displayed in the Path Copy Copy contextual menu.
//
// @param p_CLSID ID of plugin COM class.
// @return S_OK if successful, S_FALSE if the plugin was already registered, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::RegisterPlugin(
    REFCLSID p_CLSID)
{
    HRESULT hRes = E_FAIL;
    try {
        // Use a local settings object here so that we don't check for updates if only this is called.
        hRes = PCC::Settings().RegisterCOMPlugin(p_CLSID, false) ? S_OK : S_FALSE;
    } catch (const PCC::SettingsException& se) {
        hRes = HRESULT_FROM_WIN32(se.ErrorCode());
    } catch (...) {
        hRes = E_UNEXPECTED;
    }
    return hRes;
}

//
// IPathCopyCopyContextMenuExt::UnregisterPlugin
//
// Removes a plugin component from the Path Copy Copy registry.
//
// @param p_pCLSID ID of plugin COM class, as a string.
// @return S_OK if successful, S_FALSE if the component was not registered, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::UnregisterPlugin(
    REFCLSID p_CLSID)
{
    HRESULT hRes = E_FAIL;
    try {
        // Use a local settings object here so that we don't check for updates if only this is called.
        hRes = PCC::Settings().UnregisterCOMPlugin(p_CLSID, false) ? S_OK : S_FALSE;
    } catch (const PCC::SettingsException& se) {
        hRes = HRESULT_FROM_WIN32(se.ErrorCode());
    } catch (...) {
        hRes = E_UNEXPECTED;
    }
    return hRes;
}

#ifndef PCC_NO_CONTEXT_MENU_EXT2

//
// IPathCopyCopyContextMenuExt2::RegisterPlugin2
//
// Registers a plugin component in the Path Copy Copy registry. The plugin
// will be displayed in the Path Copy Copy contextual menu. This version
// allows the user to register per-user and per-machine components.
//
// @param p_CLSID ID of plugin COM class.
// @param p_User Whether to register the plugin per-user (VARIANT_TRUE) or per-machine (VARIANT_FALSE).
// @return S_OK if successful, S_FALSE if the plugin was already registered, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::RegisterPlugin2(
    REFCLSID p_CLSID,
    VARIANT_BOOL p_User)
{
    HRESULT hRes = E_FAIL;
    try {
        // Use a local settings object here so that we don't check for updates if only this is called.
        hRes = PCC::Settings().RegisterCOMPlugin(p_CLSID, p_User != VARIANT_FALSE) ? S_OK : S_FALSE;
    } catch (const PCC::SettingsException& se) {
        hRes = HRESULT_FROM_WIN32(se.ErrorCode());
    } catch (...) {
        hRes = E_UNEXPECTED;
    }
    return hRes;
}

//
// IPathCopyCopyContextMenuExt2::UnregisterPlugin2
//
// Removes a plugin component from the Path Copy Copy registry. This version
// allows the user to unregister per-user and per-machine components.
//
// @param p_pCLSID ID of plugin COM class, as a string.
// @param p_User Whether the plugin was registered per-user (VARIANT_TRUE) or per-machine (VARIANT_FALSE).
// @return S_OK if successful, S_FALSE if the component was not registered, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::UnregisterPlugin2(
    REFCLSID p_CLSID,
    VARIANT_BOOL p_User)
{
    HRESULT hRes = E_FAIL;
    try {
        // Use a local settings object here so that we don't check for updates if only this is called.
        hRes = PCC::Settings().UnregisterCOMPlugin(p_CLSID, p_User != VARIANT_FALSE) ? S_OK : S_FALSE;
    } catch (const PCC::SettingsException& se) {
        hRes = HRESULT_FROM_WIN32(se.ErrorCode());
    } catch (...) {
        hRes = E_UNEXPECTED;
    }
    return hRes;
}

#endif // PCC_NO_CONTEXT_MENU_EXT2

//
// IShellExtInit::Initialize
//
// Called by the shell to initialize our contextual menu extension.
// We need to use this opportunity to look at the files/folders to act upon.
//
// @param p_pFolderPIDL Pointer to ITEMIDLIST representing selected folder.
// @param p_pDataObject Pointer to data object containing information about selected files/folders.
// @param p_hKeyFileClass Handle to file class key; unused.
// @return S_OK if successful, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::Initialize(
    PCIDLIST_ABSOLUTE p_pFolderPIDL,
    IDataObject *p_pDataObject,
    HKEY /*p_hKeyFileClass*/)
{
    HRESULT hRes = S_OK;

    try {
        // Make sure we have a data object.
        if (p_pDataObject != nullptr) {
            // Extract HDROP from data object.
            StStgMedium stgMedium;
            FORMATETC formatEtc = {CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
            if (SUCCEEDED(p_pDataObject->GetData(&formatEtc, &stgMedium))) {
                // Get number of files included in the selection.
                const UINT fileCount = ::DragQueryFileW(
                    static_cast<HDROP>(stgMedium.Get().hGlobal), 0xFFFFFFFF, 0, 0);
                if (fileCount > 0) {
                    // Pre-allocate space in vector to store files.
                    m_vFiles.reserve(fileCount);

                    // Get each file in turn.
                    std::wstring buffer(MAX_PATH + 1, L'\0');
                    for(UINT i = 0; i < fileCount; ++i) {
                        const UINT copiedCount = ::DragQueryFileW(static_cast<HDROP>(stgMedium.Get().hGlobal),
                            i, &*buffer.begin(), gsl::narrow<UINT>(buffer.size()));
                        m_vFiles.emplace_back(buffer.c_str());
                    }
                } else {
                    // It's difficult to display a menu item without files to act upon.
                    hRes = E_FAIL;
                }
            }
        } else if (p_pFolderPIDL != nullptr) {
            // No data object, but maybe it's because user clicked on a folder's
            // background. Get the folder path from the ID list.
            std::wstring buffer(MAX_PATH + 1, L'\0');
            if (::SHGetPathFromIDList(p_pFolderPIDL, &*buffer.begin()) != FALSE) {
                m_vFiles.emplace_back(buffer.c_str());
            } else {
                // Nothing like that either, problem.
                hRes = E_FAIL;
            }
        } else {
            hRes = E_POINTER;
        }
    } catch (...) {
        hRes = E_UNEXPECTED;
    }

    return hRes;
}

//
// IContextMenu::QueryContextMenu
//
// Invoked by the shell to populate a contextual menu.
// We need to use this opportunity to add our menu items.
//
// @param p_hMenu Handle to the contextual menu to populate.
// @param p_Index Index in the menu where to add items.
// @param p_FirstCmdId First available command ID for our commands.
// @param p_LastCmdId Last available command ID for our commands.
// @param p_Flags Flags indicating how the menu can be changed; see MSDN for details.
// @return If successful, a success code with code value set to the largest command ID
//         used, plus one; otherwise, an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::QueryContextMenu(
    HMENU p_hMenu,
    UINT p_Index,
    UINT p_FirstCmdId,
    UINT p_LastCmdId,
    UINT p_Flags)
{
    HRESULT hRes = S_OK;

    try {
        if (p_hMenu == nullptr) {
            hRes = E_INVALIDARG;
        } else {
            // Make sure this menu hasn't been modified by another instance.
            CPathCopyCopyContextMenuExt* pOtherInstance = nullptr;
            {
                std::lock_guard<std::mutex> lock(s_ExtToMenusLock);
                auto it = std::find_if(s_vExtToMenus.begin(), s_vExtToMenus.end(), [&](const auto& extToMenu) {
                    return extToMenu.second == p_hMenu;
                });
                if (it != s_vExtToMenus.end()) {
                    pOtherInstance = it->first;
                }
            }

            // Do not add items if the default action is chosen, if we have no files,
            // if menu has been modified by another instance or if this is a menu for
            // a shortcut (.lnk) file and we want to copy true .lnk paths (in such a
            // case, another instance is coming after us to do the job).
            bool skipBecauseOfLnk = false;
            if ((p_Flags & CMF_VERBSONLY) != 0) {
                skipBecauseOfLnk = GetSettings().GetTrueLnkPaths();
            }
            if (m_vFiles.empty() || (p_Flags & CMF_DEFAULTONLY) != 0 || pOtherInstance != nullptr || skipBecauseOfLnk) {
                hRes = E_FAIL;
            } else {
                UINT cmdId = p_FirstCmdId;
                UINT position = p_Index;

                // Fetch reference to settings.
                PCC::Settings& rSettings = GetSettings();

                // Get all plugins in default order. Do not include temp pipeline plugins.
                m_vspPluginsInDefaultOrder = PCC::PluginsRegistry::GetPluginsInDefaultOrder(
                    &rSettings, &rSettings, PCC::PipelinePluginsOptions::FetchPipelinePlugins);

                // Get set of all plugins from the above vector.
                m_sspAllPlugins.insert(m_vspPluginsInDefaultOrder.cbegin(), m_vspPluginsInDefaultOrder.cend());

                // Create plugin provider object wrapping our set of all plugins.
                m_spPluginProvider = std::make_shared<PCC::AllPluginsProvider>(m_sspAllPlugins);

                // Provide each plugin with settings object and plugin provider, since some require this to work.
                for (const PCC::PluginSP& spPlugin : m_sspAllPlugins) {
                    spPlugin->SetSettings(&rSettings);
                    spPlugin->SetPluginProvider(m_spPluginProvider.get());
                }

                // Quick helper to create a default plugin if needed later.
                const auto createDefaultPlugin = [&]() {
                    PCC::PluginSP spDefaultPlugin = std::make_shared<PCC::Plugins::DefaultPlugin>();
                    spDefaultPlugin->SetSettings(&rSettings);
                    spDefaultPlugin->SetPluginProvider(m_spPluginProvider.get());
                    return spDefaultPlugin;
                };

                // Get a few setting values.
                const bool useIconForDefaultPlugin = rSettings.GetUseIconForDefaultPlugin();
                const bool usePreviewMode = rSettings.GetUsePreviewMode();
                const bool usePreviewModeInMainMenu = rSettings.GetUsePreviewModeInMainMenu();
                const bool dropRedundantWords = rSettings.GetDropRedundantWords();
                const bool alwaysShowSubmenu = rSettings.GetAlwaysShowSubmenu();
                PCC::GUIDV vKnownPlugins;
                const PCC::GUIDV* const pvKnownPlugins = rSettings.GetKnownPlugins(vKnownPlugins) ? &vKnownPlugins : nullptr;
                GUID ctrlKeyPluginId;
                const GUID* const pCtrlKeyPluginId = rSettings.GetCtrlKeyPlugin(ctrlKeyPluginId) ? &ctrlKeyPluginId : nullptr;

                // Check if user held down Ctrl key and we have a plugin to use when this happens.
                if ((::GetKeyState(VK_CONTROL) & 0x8000) != 0 && pCtrlKeyPluginId != nullptr) {
                    // Find plugin to use.
                    const auto pluginIt = m_sspAllPlugins.find(*pCtrlKeyPluginId);
                    if (pluginIt != m_sspAllPlugins.end() && !(*pluginIt)->IsSeparator()) {
                        ActOnFiles(*pluginIt, nullptr);
                    }
                }

                // Add all plugins requested to the main menu.
                PCC::GUIDV vPluginIds;
                if (rSettings.GetMainMenuPluginDisplayOrder(vPluginIds)) {
                    // TODO why are we not using vspPlugins here?
                    PCC::PluginSPV vspPlugins = PCC::PluginsRegistry::OrderPluginsToDisplay(
                        m_sspAllPlugins, vPluginIds, pvKnownPlugins, &m_vspPluginsInDefaultOrder);
                    if (!vPluginIds.empty()) {
                        if (vPluginIds.size() != 1 || !::IsEqualGUID(vPluginIds.front(), PCC::Plugins::LongPathPlugin::ID)) {
                            for (auto it = vPluginIds.cbegin(); SUCCEEDED(hRes) && cmdId <= p_LastCmdId && it != vPluginIds.cend(); ++it) {
                                hRes = AddPluginToMenu(*it, p_hMenu, useIconForDefaultPlugin, usePreviewModeInMainMenu, false, true, cmdId, position);
                            }
                        } else {
                            // Default plugin is specified, use our own instead.
                            hRes = AddPluginToMenu(createDefaultPlugin(), p_hMenu, useIconForDefaultPlugin, usePreviewModeInMainMenu, false, true, cmdId, position);
                        }
                    }
                } else {
                    // No setting specified for items in the main menu. Add our default plugin.
                    hRes = AddPluginToMenu(createDefaultPlugin(), p_hMenu, useIconForDefaultPlugin, usePreviewModeInMainMenu, false, true, cmdId, position);
                }

                // Create sub-menu to populate it with the other plugins.
                // If we don't always show the submenu, user needs to ask
                // for extended verbs to get it (by holding Shift).
                if (SUCCEEDED(hRes) && cmdId <= p_LastCmdId && (alwaysShowSubmenu || (p_Flags & CMF_EXTENDEDVERBS) != 0)) {
                    HMENU hSubMenu = ::CreatePopupMenu();
                    try {
                        // Fetch list of plugins to display in the submenu.
                        PCC::PluginSPV vspPlugins;
                        const PCC::PluginSPV* pvspPlugins = nullptr;
                        vPluginIds.clear();
                        rSettings.GetSubmenuPluginDisplayOrder(vPluginIds);
                        if (!vPluginIds.empty()) {
                            vspPlugins = PCC::PluginsRegistry::OrderPluginsToDisplay(m_sspAllPlugins, vPluginIds,
                                pvKnownPlugins, &m_vspPluginsInDefaultOrder);
                            pvspPlugins = &vspPlugins;
                        } else {
                            // No plugin specified, use all plugins in default order.
                            pvspPlugins = &m_vspPluginsInDefaultOrder;
                        }

                        // Iterate plugins and try to add them to the submenu.
                        UINT subPosition = 0;
                        bool prevWasSeparator = true;
                        for (auto it = pvspPlugins->cbegin(); SUCCEEDED(hRes) && cmdId <= p_LastCmdId && it != pvspPlugins->cend(); ++it) {
                            // Try to insert this plugin in the menu.
                            const PCC::PluginSP& spPlugin = *it;
                            if (!spPlugin->IsSeparator()) {
                                hRes = AddPluginToMenu(spPlugin, hSubMenu, false, usePreviewMode, dropRedundantWords, false, cmdId, subPosition);
                                prevWasSeparator = false;
                            } else {
                                // This is a proxy to insert a separator.
                                // Note: there's a chance that we may double up the separators
                                // if not all plugins are shown in the submenu. Avoid this if possible.
                                if (!prevWasSeparator) {
                                    if (::InsertMenuW(hSubMenu, subPosition, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr)) {
                                        ++subPosition;
                                        // No need to increment cmdId.
                                    } else {
                                        hRes = E_FAIL;
                                    }
                                    prevWasSeparator = true;
                                }
                            }
                        }

                        // Add item to open the settings app, unless editing
                        // the settings has been locked out by the administrator.
                        if (SUCCEEDED(hRes) && cmdId <= p_LastCmdId && !rSettings.GetEditingDisabled()) {
                            if (!prevWasSeparator) {
                                if (::InsertMenuW(hSubMenu, subPosition, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr)) {
                                    ++subPosition;
                                } else {
                                    hRes = E_FAIL;
                                }
                            }
                            if (SUCCEEDED(hRes)) {
                                ATL::CStringW settingsCaption(MAKEINTRESOURCEW(IDS_PCC_SETTINGS_DESCRIPTION));
                                if (::InsertMenuW(hSubMenu, subPosition, MF_BYPOSITION | MF_STRING, cmdId, settingsCaption)) {
                                    m_SettingsCmdId = cmdId;
                                    ++cmdId;
                                    ++subPosition;
                                } else {
                                    hRes = E_FAIL;
                                }
                            }
                        }
                    } catch (...) {
                        ::DestroyMenu(hSubMenu);
                        throw;
                    }

                    if (SUCCEEDED(hRes) && ::GetMenuItemCount(hSubMenu) > 0) {
                        // Submenu was populated. Add it to the contextual menu.
                        std::wstring subMenuCaption = GetMenuCaptionWithShortcut(p_hMenu, (LPCWSTR) ATL::CStringW(MAKEINTRESOURCEW(IDS_PATH_COPY_MENU_ITEM)));
                        PCCDEBUGCODE(subMenuCaption += L" (DEBUG)");
                        MENUITEMINFOW menuItemInfo;
                        menuItemInfo.cbSize = sizeof(MENUITEMINFOW);
                        menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_SUBMENU;
                        menuItemInfo.fType = MFT_STRING;
                        menuItemInfo.wID = cmdId;
                        menuItemInfo.hSubMenu = hSubMenu;
                        menuItemInfo.dwTypeData = &*subMenuCaption.begin();
                        if (rSettings.GetUseIconForSubmenu()) {
                            // Add an icon next to the submenu.
                            HBITMAP hIconBitmap = GetPCCIcon();
                            if (hIconBitmap != nullptr) {
                                menuItemInfo.fMask |= MIIM_BITMAP;
                                menuItemInfo.hbmpItem = hIconBitmap;
                            }
                        }
                        if (::InsertMenuItemW(p_hMenu, position, TRUE, &menuItemInfo)) {
                            m_SubMenuCmdId = cmdId;
                            ++cmdId;
                            ++position;
                        } else {
                            hRes = E_FAIL;
                        }
                    }

                    if (FAILED(hRes)) {
                        ::DestroyMenu(hSubMenu);
                    }
                }

                if (SUCCEEDED(hRes)) {
                    // Strange return value requirement... see MSDN for details.
                    hRes = MAKE_HRESULT(SEVERITY_SUCCESS, 0, cmdId - p_FirstCmdId + 1);

                    // Mark us as being the instance that modified this menu.
                    std::lock_guard<std::mutex> lock(s_ExtToMenusLock);
                    s_vExtToMenus.emplace_back(this, p_hMenu);
                }
            }
        }
    } catch (...) {
        hRes = E_UNEXPECTED;
    }

    return hRes;
}

//
// IContextMenu::InvokeCommand
//
// Invoked by the shell when the user selects one of our menu items.
// We do our stuff here.
//
// @param p_pCommandInfo Pointer to struct containing command information.
// @return S_OK if successful, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::InvokeCommand(
    CMINVOKECOMMANDINFO* p_pCommandInfo)
{
    HRESULT hRes = S_OK;

    try {
        if ((p_pCommandInfo == nullptr) || (p_pCommandInfo->cbSize < sizeof(CMINVOKECOMMANDINFO))) {
            hRes = E_INVALIDARG;
        } else {
            // Get offset of invoked command.
#pragma warning(suppress: 26490) // No choice but to use reinterpret_cast because of Win32 API
            const UINT_PTR cmdOffset = reinterpret_cast<UINT_PTR>(p_pCommandInfo->lpVerb);
            if ((cmdOffset & 0xFFFF0000) != 0) {
                // We do not support verb invokation.
                hRes = E_FAIL;
            } else if (!m_FirstCmdId.has_value()) {
                // We do not have menu items, so we can't invoke anything.
                hRes = E_INVALIDARG;
            } else {
                // Check which command it is that is invoked.
                const UINT_PTR cmdId = cmdOffset + *m_FirstCmdId;
                const auto itId = m_mPluginsByCmdId.find(cmdId);
                if (itId == m_mPluginsByCmdId.end()) {
                    // This is not a recognized plugin command ID.
                    if (m_SettingsCmdId.has_value() && *m_SettingsCmdId == cmdId) {
                        PCC::SettingsApp().Launch();
                        hRes = S_OK;
                    } else if (m_SubMenuCmdId.has_value() && *m_SubMenuCmdId == cmdId) {
                        hRes = E_INVALIDARG;
                    } else {
                        hRes = E_FAIL;
                    }
                } else {
                    // Fetch plugin reference for this command ID.
                    PCC::PluginSP spPlugin = itId->second;

                    // Act on the files using the plugin.
                    hRes = ActOnFiles(spPlugin, p_pCommandInfo->hwnd);
                }
            }
        }
    } catch (...) {
        hRes = E_UNEXPECTED;
    }

    return hRes;
}

//
// IContextMenu::GetCommandString
//
// Invoked by the shell to get a text description for our menu item.
//
// @param p_CmdId Offset of ID of command for which to query text, relative
//                to our first command ID.
// @param p_Flags Type of information or action requested; see MSDN for details.
// @param p_pReserved Reserved; unused.
// @param p_pBuffer Pointer to memory buffer where to copy a null-terminated string.
//                  Note: if GCS_UNICODE is found in p_Flags, the string must
//                  be cast to a LPWSTR and a Unicode string must be stored there.
// @param p_BufferSize Max size of p_pBuffer, in characters.
// @return S_OK if successful, otherwise an error code.
//
[[gsl::suppress(c.128)]]
STDMETHODIMP CPathCopyCopyContextMenuExt::GetCommandString(
    UINT_PTR p_CmdId,
    UINT p_Flags,
    UINT* p_pReserved,
    LPSTR p_pBuffer,
    UINT p_BufferSize)
{
    HRESULT hRes = S_OK;

    try {
        // Check what is requested.
        if ((p_Flags == GCS_VERBA) || (p_Flags == GCS_VERBW)) {
            // We do not support verb invokation.
            hRes = E_NOTIMPL;
        } else if ((p_Flags == GCS_VALIDATEA) || (p_Flags == GCS_VALIDATEW)) {
            // We need to validate command ID.
            hRes = S_FALSE;
            if (m_FirstCmdId.has_value()) {
                if ((m_mPluginsByCmdId.find(*m_FirstCmdId + p_CmdId) != m_mPluginsByCmdId.end()) ||
                    (m_SettingsCmdId.has_value() && (*m_FirstCmdId + p_CmdId) == *m_SettingsCmdId)) {

                    // Either it's a plugin or a special menu item.
                    hRes = S_OK;
                }
            }
        } else if (p_Flags == GCS_HELPTEXTA) {
            // Call this method to get the Unicode version.
            std::wstring buffer(p_BufferSize, L'\0');
            hRes = this->GetCommandString(p_CmdId,
                                          GCS_HELPTEXTW,
                                          p_pReserved,
                                          reinterpret_cast<LPSTR>(&*buffer.begin()),
                                          p_BufferSize);
            if (SUCCEEDED(hRes)) {
                // Convert it to a single-byte string and return it.
                ATL::CStringA aBuffer(buffer.c_str());
                if (::strcpy_s(p_pBuffer, p_BufferSize, aBuffer) == 0) {
                    hRes = S_OK;
                } else {
                    hRes = E_FAIL;
                }
            }
        } else if (p_Flags == GCS_HELPTEXTW) {
            // A Unicode help string is requested.
            if (p_pBuffer != nullptr) {
                // Try finding the plugin that handles this command ID.
                CmdIdPluginM::const_iterator it = m_mPluginsByCmdId.end();
                if (m_FirstCmdId.has_value()) {
                    it = m_mPluginsByCmdId.find(*m_FirstCmdId + p_CmdId);
                }
                if (it != m_mPluginsByCmdId.end()) {
                    // Found the plugin, ask for its help text.
                    std::wstring helpText = it->second->HelpText();

                    // Return help text.
                    if (::wcscpy_s(reinterpret_cast<LPWSTR>(p_pBuffer), p_BufferSize, helpText.c_str()) != 0) {
                        hRes = E_FAIL;
                    }
                } else {
                    // Check for non-plugin menu items.
                    if (m_FirstCmdId.has_value() && m_SubMenuCmdId.has_value() && (*m_FirstCmdId + p_CmdId) == *m_SubMenuCmdId) {
                        // The sub-menu itself.
                        ATL::CStringW subMenuHint(MAKEINTRESOURCEW(IDS_PATH_COPY_HINT));
                        if (::wcscpy_s(reinterpret_cast<LPWSTR>(p_pBuffer), p_BufferSize, subMenuHint) != 0) {
                            hRes = E_FAIL;
                        }
                    } else if (m_FirstCmdId.has_value() && m_SettingsCmdId.has_value() && (*m_FirstCmdId + p_CmdId) == *m_SettingsCmdId) {
                        // The item to open the settings application.
                        ATL::CStringW settingsHint(MAKEINTRESOURCEW(IDS_PCC_SETTINGS_HINT));
                        if (::wcscpy_s(reinterpret_cast<LPWSTR>(p_pBuffer), p_BufferSize, settingsHint) != 0) {
                            hRes = E_FAIL;
                        }
                    } else {
                        hRes = E_INVALIDARG;
                    }
                }
            } else {
                hRes = E_INVALIDARG;
            }
        } else {
            // Unknown, unsupported flag.
            hRes = E_NOTIMPL;
        }
    } catch (...) {
        hRes = E_UNEXPECTED;
    }

    return hRes;
}

//
// Returns a reference to the object used to access user settings.
// The object is created on the first call.
//
// @return Reference to settings object.
//
PCC::Settings& CPathCopyCopyContextMenuExt::GetSettings()
{
    // Create on first call.
    if (m_spSettings == nullptr) {
        m_spSettings = std::make_shared<PCC::Settings>();
    }
    return *m_spSettings;
}

//
// Returns the path of the parent directory of all selected files.
//
// @return Parent directory path.
//
std::wstring CPathCopyCopyContextMenuExt::GetParentPath() const
{
    std::wstring parentPath;

    // Extract the parent path of the first file. We'll assume that all
    // files have the same parent. This might not be strictly true in all
    // cases (for example, in a custom shell view) but we're only using it
    // for validation purposes, so it's good enough.
    if (!m_vFiles.empty()) {
        parentPath = m_vFiles.front();
        PCC::PluginUtils::ExtractFolderFromPath(parentPath);
    }

    return parentPath;
}

//
// Adds a new item to the given menu (either the main contextual menu or
// our submenu can be used) and assigns the given plugin to it. This will
// be called multiple times by the QueryContextMenu method to fill in the
// menu with all the items.
//
// @param p_PluginId ID of plugin to add to the menu.
// @param p_hMenu Handle of menu to add the item to.
// @param p_UsePCCIcon Whether to use the PCC icon for this plugin.
// @param p_UsePreviewMode Whether to use plugin result instead of
//                         its description as caption in the menu.
// @param p_DropRedundantWords Whether to drop redundant words in the plugin
//                             description, like "copy".
// @param p_ComputeShortcut Whether to compute the best shortcut for
//                          the menu item according to existing menu items.
// @param p_rCmdId Command ID to use for the item. Upon return, this
//                 will be incremented to the next command ID to use.
// @param p_rPosition Position where to insert the item in the menu.
//                    Upon return, this will be incremented to the
//                    next position to use.
// @return Error code, or S_OK if the item was added to the menu.
//
HRESULT CPathCopyCopyContextMenuExt::AddPluginToMenu(const GUID& p_PluginId,
                                                     HMENU const p_hMenu,
                                                     const bool p_UsePCCIcon,
                                                     const bool p_UsePreviewMode,
                                                     const bool p_DropRedundantWords,
                                                     const bool p_ComputeShortcut,
                                                     UINT& p_rCmdId,
                                                     UINT& p_rPosition)
{
    // Look for the plugin in the set of all plugins.
    const auto pluginIt = m_sspAllPlugins.find(p_PluginId);

    // If we have a plugin, continue.
    HRESULT hRes = E_INVALIDARG;
    if (pluginIt != m_sspAllPlugins.end()) {
        hRes = AddPluginToMenu(*pluginIt, p_hMenu, p_UsePCCIcon, p_UsePreviewMode,
            p_DropRedundantWords, p_ComputeShortcut, p_rCmdId, p_rPosition);
    }

    return hRes;
}

//
// Adds a new item to the given menu (either the main contextual menu or
// our submenu can be used) and assigns the given plugin to it. This will
// be called multiple times by the QueryContextMenu method to fill in the
// menu with all the items.
//
// @param p_spPlugin Plugin to add to the menu.
// @param p_hMenu Handle of menu to add the item to.
// @param p_UsePCCIcon Whether to use the PCC icon for this plugin.
// @param p_UsePreviewMode Whether to use plugin result instead of
//                         its description as caption in the menu.
// @param p_DropRedundantWords Whether to drop redundant words in the plugin
//                             description, like "copy".
// @param p_ComputeShortcut Whether to compute the best shortcut for
//                          the menu item according to existing menu items.
// @param p_rCmdId Command ID to use for the item. Upon return, this
//                 will be incremented to the next command ID to use.
// @param p_rPosition Position where to insert the item in the menu.
//                    Upon return, this will be incremented to the
//                    next position to use.
// @return Error code, or S_OK if the item was added to the menu.
//
HRESULT CPathCopyCopyContextMenuExt::AddPluginToMenu(const PCC::PluginSP& p_spPlugin,
                                                     HMENU const p_hMenu,
                                                     const bool p_UsePCCIcon,
                                                     const bool p_UsePreviewMode,
                                                     const bool p_DropRedundantWords,
                                                     const bool p_ComputeShortcut,
                                                     UINT& p_rCmdId,
                                                     UINT& p_rPosition)
{
    HRESULT hRes = S_OK;

    // Check if plugin should be enabled.
    const bool enabled = p_spPlugin->Enabled(GetParentPath(), m_vFiles.front());

    // Compile info about the menu item using the plugin object.
    std::wstring description;
    if (p_UsePreviewMode && enabled) { // Disabled plugins don't work so can't use preview mode.
        description = p_spPlugin->GetPath(m_vFiles.front());
        // Let's limit the size of menu items if possible.
        if (description.size() > MAX_PATH) {
            description.resize(MAX_PATH);
        }
        // If path contains ampersands, they will be treated as shortcuts.
        // We have to double them.
        StringUtils::ReplaceAll(description, L"&", L"&&");
    } else {
        description = p_spPlugin->Description();
        if (p_DropRedundantWords && p_spPlugin->CanDropRedundantWords()) {
            ATL::CStringW redundantCopy(MAKEINTRESOURCEW(IDS_REDUNDANT_WORD_COPY));
            if (description.size() >= gsl::narrow_cast<std::wstring::size_type>(redundantCopy.GetLength()) &&
                ::_wcsnicmp(description.c_str(), (LPCWSTR) redundantCopy, redundantCopy.GetLength()) == 0) {

                // The description starts with "Copy ", drop it.
                description.erase(0, redundantCopy.GetLength());

                // In some language, this will leave the first letter in lowercase.
                // Convert it to uppercase in that case.
                if (!description.empty() && ::iswlower(description[0])) {
                    description[0] = ::towupper(description[0]);
                }

                // ...or maybe the first letter is the shortcut character...
                if (description.size() >= 2 && description[0] == L'&' && ::iswlower(description[1])) {
                    description[1] = ::towupper(description[1]);
                }
            }
        }
        if (p_ComputeShortcut) {
            description = GetMenuCaptionWithShortcut(p_hMenu, description);
        }
    }
    MENUITEMINFOW menuItemInfo;
    menuItemInfo.cbSize = sizeof(MENUITEMINFOW);
    menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING;
    menuItemInfo.fType = MFT_STRING;
    menuItemInfo.fState = enabled ? MFS_ENABLED : MFS_DISABLED;
    menuItemInfo.wID = p_rCmdId;
    menuItemInfo.dwTypeData = description.data();
    HBITMAP hIconBitmap = nullptr;
    if (p_UsePCCIcon || p_spPlugin->UseDefaultIcon()) {
        hIconBitmap = GetPCCIcon();
    } else {
        std::wstring iconFile = p_spPlugin->IconFile();
        if (!iconFile.empty()) {
            hIconBitmap = GetIconForIconFile(iconFile);
        } else {
            auto possibleIconFile = GetSettings().GetIconFileForPlugin(p_spPlugin->IdForIcon());
            if (possibleIconFile.has_value()) {
                if (possibleIconFile->empty()) {
                    hIconBitmap = GetPCCIcon();
                } else {
                    hIconBitmap = GetIconForIconFile(*possibleIconFile);
                }
            }
        }
    }
    if (hIconBitmap != nullptr) {
        menuItemInfo.fMask |= MIIM_BITMAP;
        menuItemInfo.hbmpItem = hIconBitmap;
    }

    // Insert the item in the menu.
    if (::InsertMenuItemW(p_hMenu, p_rPosition, TRUE, &menuItemInfo)) {
        m_mPluginsByCmdId[p_rCmdId] = p_spPlugin;
        if (!m_FirstCmdId.has_value()) {
            m_FirstCmdId = p_rCmdId;
        }
        ++p_rCmdId;
        ++p_rPosition;
    } else {
        hRes = E_FAIL;
    }

    return hRes;
}

//
// Returns a caption usable for a menu item, with the first shortcut available.
//
// @param p_hMenu Handle to main menu.
// @param p_Caption Caption to add a shortcut to.
// @return Caption for the menu item.
//
std::wstring CPathCopyCopyContextMenuExt::GetMenuCaptionWithShortcut(HMENU const p_hMenu,
                                                                     const std::wstring& p_Caption) const
{
    // Scan provided menu and build a set of already-used shortcuts.
    std::set<wchar_t> shortcuts;
    const int itemsCount = ::GetMenuItemCount(p_hMenu);
    for (int i = 0; i < itemsCount; ++i) {
        std::wstring buffer(1000, L'\0');
        MENUITEMINFOW menuItemInfo;
        menuItemInfo.cbSize = sizeof(MENUITEMINFOW);
        menuItemInfo.fMask = MIIM_STRING | MIIM_STATE;
        menuItemInfo.dwTypeData = buffer.data();
        menuItemInfo.cch = gsl::narrow<UINT>(buffer.size());
        if (::GetMenuItemInfoW(p_hMenu, i, TRUE, &menuItemInfo) && (menuItemInfo.fState & MFS_DISABLED) == 0) {
            const std::wstring menuItemCaption = buffer.c_str();
            const auto shortcutCharIdx = menuItemCaption.find(L'&');
            if (shortcutCharIdx != std::wstring::npos && shortcutCharIdx < (menuItemCaption.size() - 1)) {
                shortcuts.insert(::towlower(menuItemCaption.at(shortcutCharIdx + 1)));
            }
        }
    }

    // Check if the caption already has a shortcut. If it's available, keep it.
    std::wstring caption = p_Caption;
    const auto idx = caption.find(L'&');
    if (idx == std::wstring::npos || idx == (caption.size() - 1) || shortcuts.find(::towlower(caption.at(idx + 1))) != shortcuts.end()) {
        // No caption or can't use this caption.
        StringUtils::ReplaceAll(caption, L"&", L"");

        // Scan our caption and select first available shortcut.
        for (auto it = caption.begin(); it != caption.end(); ++it) {
            if (::iswalpha(*it) && shortcuts.find(::towlower(*it)) == shortcuts.end()) {
                caption.insert(it, L'&');
                break;
            }
        }
    }

    return caption;
}

//
// Returns a handle to the bitmap containing the Path Copy Copy icon
// that can be used for contextual menu items, loading it if necessary.
//
// @return Handle of bitmap containing the icon. Will be NULL if loading failed.
//
HBITMAP CPathCopyCopyContextMenuExt::GetPCCIcon()
{
    // Load on first call.
    if (m_spPCCIcon == nullptr) {
        // Load PNG resource.
        HRSRC hPngRsrcInfo = ::FindResourceW(CPathCopyCopyModule::HInstance(), MAKEINTRESOURCEW(IDB_PCCICON2), L"PNG");
        if (hPngRsrcInfo != nullptr) {
            HGLOBAL hPngRsrc = ::LoadResource(CPathCopyCopyModule::HInstance(), hPngRsrcInfo);
            const DWORD pngSize = ::SizeofResource(CPathCopyCopyModule::HInstance(), hPngRsrcInfo);
            if (hPngRsrc != nullptr && pngSize != 0) {
                void* pPngData = ::LockResource(hPngRsrc);
                if (pPngData != nullptr) {
                    // Store PNG data in a global block of memory.
                    StGlobalBlock globalPngData(GMEM_MOVEABLE, pngSize);
                    if (globalPngData.Get() != nullptr) {
                        StGlobalLock pngLock(globalPngData.Get());
                        if (pngLock.GetPtr() != nullptr) {
                            ::memcpy(pngLock.GetPtr(), pPngData, pngSize);
                        } else {
                            globalPngData.Acquire(nullptr);
                        }
                    }
                    if (globalPngData.Get() != nullptr) {
                        // Create IStream object on this HGLOBAL to be able to pass it to GDI+.
                        ATL::CComPtr<IStream> cpPngStream;
                        if (SUCCEEDED(::CreateStreamOnHGlobal(globalPngData.Get(), FALSE, &cpPngStream))) {
                            // Init GDI+ to be able to use its calls, since shell doesn't do it.
                            StGdiplusStartup gdiPlusStartup;
                            if (gdiPlusStartup.Started()) {
                                // Extract HBITMAP using GDI+.
                                HBITMAP hPngBitmap = nullptr;
                                Gdiplus::Bitmap pngBitmap(cpPngStream, FALSE);
                                if (pngBitmap.GetHBITMAP(Gdiplus::Color(), &hPngBitmap) == Gdiplus::Ok) {
                                    // We have HBITMAP, save it in our member.
                                    m_spPCCIcon = std::make_shared<StImage>(hPngBitmap, IMAGE_BITMAP, false);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return (m_spPCCIcon != nullptr && m_spPCCIcon->GetLoadResult() == ERROR_SUCCESS) ? m_spPCCIcon->GetBitmap() : nullptr;
}

//
// Given the path to an icon file, returns a handle to a Win32 bitmap for that icon
// if possible. If an empty string is passed, the method simply returns NULL.
//
// @param p_IconFile Path to icon file. Can be empty.
// @return Handle to icon bitmap, or NULL if p_IconFile was empty or if an error occured.
//
HBITMAP CPathCopyCopyContextMenuExt::GetIconForIconFile(const std::wstring& p_IconFile)
{
    // Assume we'll fail to locate an icon.
    HBITMAP hIconBitmap = nullptr;

    // Check if icon file was specified.
    if (!p_IconFile.empty()) {
        // First look for icon in our map, in case we've previously loaded it.
        // The map contains lowercase paths, so we need to convert ours.
        std::wstring lowerIconFile;
        const std::locale ourLocale("");
        lowerIconFile.resize(p_IconFile.size());
        std::transform(p_IconFile.cbegin(), p_IconFile.cend(), lowerIconFile.begin(),
            [&](wchar_t p_Char) { return std::tolower(p_Char, ourLocale); });
        const auto foundIconFile = m_mspIcons.find(lowerIconFile);
        if (foundIconFile != m_mspIcons.end()) {
            // We loaded this bitmap previously, return it again.
            const StImage& loadedImage = *foundIconFile->second;
            if (loadedImage.GetLoadResult() == ERROR_SUCCESS) {
                hIconBitmap = loadedImage.GetBitmap();
            }
        } else {
            // This icon file hasn't been loaded yet, load it now and insert it in the map.
            // First attempt to open the file on disk and get an IStream for it.
            ATL::CComPtr<IStream> cpIconFileStream;
            if (SUCCEEDED(::SHCreateStreamOnFileEx(p_IconFile.c_str(), STGM_READ | STGM_SHARE_DENY_WRITE | STGM_DIRECT,
                                                   FILE_ATTRIBUTE_NORMAL, FALSE, nullptr, &cpIconFileStream))) {
                // Successfully got stream for the icon, attempt to get a bitmap for it.

                // Init GDI+ to be able to use its calls, since shell doesn't do it.
                StGdiplusStartup gdiPlusStartup;
                if (gdiPlusStartup.Started()) {
                    // Extract HBITMAP using GDI+.
                    HBITMAP hFileBitmap = nullptr;
                    Gdiplus::Bitmap fileBitmap(cpIconFileStream, FALSE);
                    if (fileBitmap.GetHBITMAP(Gdiplus::Color(), &hFileBitmap) == Gdiplus::Ok) {
                        // We have HBITMAP, save it in the map.
#pragma warning(suppress: 26414) // spImage IS copied in emplace, but the compiler can't see it
                        const auto spImage = std::make_shared<StImage>(hFileBitmap, IMAGE_BITMAP, false);
                        m_mspIcons.emplace(lowerIconFile, spImage);
                        if (spImage->GetLoadResult() == ERROR_SUCCESS) {
                            hIconBitmap = spImage->GetBitmap();
                        }
                    }
                }
            }
        }
    }

    return hIconBitmap;
}

//
// Performs the plugin's default action on our saved files.
// Call this when user picks a plugin from the menu, for instance.
//
// @param p_spPlugin Plugin to apply.
// @param p_hWnd Handle to parent window, that can be used for
//               message boxes, etc.
// @return Result code.
//
HRESULT CPathCopyCopyContextMenuExt::ActOnFiles(const PCC::PluginSP& p_spPlugin,
                                                HWND const p_hWnd)
{
    HRESULT hRes = E_FAIL;

    if (p_spPlugin != nullptr) {
        // Loop through files and compute filenames using plugin.
        const bool addQuotes = GetSettings().GetAddQuotesAroundPaths();
        const bool areQuotesOptional = GetSettings().GetAreQuotesOptional();
        const bool makeEmailLinks = GetSettings().GetMakePathsIntoEmailLinks();
        const StringUtils::EncodeParam encodeParam = GetSettings().GetEncodeParam();
        std::wstring pathsSeparator = p_spPlugin->PathsSeparator();
        if (pathsSeparator.empty()) {
            pathsSeparator = GetSettings().GetPathsSeparator();
            if (pathsSeparator.empty()) {
                pathsSeparator = DEFAULT_PATHS_SEPARATOR;
            }
        }
        std::wstring newFiles;
        for (auto it = m_vFiles.cbegin(); it != m_vFiles.cend(); ++it) {
            // Ask plugin to compute filename using its scheme and save it.
            const std::wstring& oldName = *it;
            if (!newFiles.empty()) {
                newFiles += pathsSeparator;
            }
            if (makeEmailLinks) {
                newFiles += L"<";
            }
            std::wstring newName = p_spPlugin->GetPath(oldName);
            StringUtils::EncodeURICharacters(newName, encodeParam);
            if (addQuotes) {
                AddQuotes(newName, areQuotesOptional);
            }
            newFiles += newName;
            if (makeEmailLinks) {
                newFiles += L">";
            }
        }

        // Get action to perform on the filenames.
        PCC::PathActionSP spAction = p_spPlugin->Action();
        assert(spAction != nullptr);

        // Use the action to perform whatever is needed.
        try {
            spAction->Act(newFiles, p_hWnd);
            hRes = S_OK;
        } catch (...) {
            assert(hRes == E_FAIL);
        }
    }

    return hRes;
}

//
// Add quotes around the given file name.
//
// @param p_rName File name to add quotes to. Will be modified in place.
// @param p_Optional Whether quotes are optional, e.g. should only be
//                   added if there are spaces in the path.
//
void CPathCopyCopyContextMenuExt::AddQuotes(std::wstring& p_rName,
                                            const bool p_Optional) const
{
    bool needToAddQuotes = true;
    if (p_Optional) {
        needToAddQuotes = p_rName.find(' ') != std::wstring::npos;
    }
    if (needToAddQuotes) {
        p_rName = L"\"" + p_rName + L"\"";
    }
}

//
// Scans the vector keeping tracks of instances modifying menus
// and removes any reference to this instance from it.
//
void CPathCopyCopyContextMenuExt::RemoveFromExtToMenu()
{
    std::lock_guard<std::mutex> lock(s_ExtToMenusLock);
    s_vExtToMenus.erase(std::remove_if(s_vExtToMenus.begin(), s_vExtToMenus.end(), [&](const auto& extToMenu) {
        return extToMenu.first == this;
    }), s_vExtToMenus.end());
}

//
// Checks for software updates if needed.
//
void CPathCopyCopyContextMenuExt::CheckForUpdates()
{
    // See if we're due for an update check.
    if (GetSettings().NeedsUpdateCheck()) {
        // Mark the last update check as now so that we don't check too often.
        GetSettings().SetLastUpdateCheckNow();

        // Launch the settings app. It will handle the check.
        PCC::SettingsApp().Launch(PCC::SettingsApp::Options().WithUpdateCheck());
    }
}
