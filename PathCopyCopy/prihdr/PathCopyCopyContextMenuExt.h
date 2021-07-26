// PathCopyCopyContextMenuExt.h
// (c) 2008-2021, Charles Lechasseur
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

#include <PathCopyCopy_i.h>
#include "PathCopyCopyPrivateTypes.h"
#include "Plugin.h"
#include "resource.h"
#include "StImage.h"

#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <utility>

#include <atlbase.h>
#include <atlcom.h>
#include <windows.h>


//
// CPathCopyCopyContextMenuExt
//
// Shell contextual menu extension that allows the user to copy the path of a file
// or a group of files into various formats.
//
class ATL_NO_VTABLE CPathCopyCopyContextMenuExt :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<CPathCopyCopyContextMenuExt, &CLSID_PathCopyCopyContextMenuExt>,
#ifndef PCC_NO_CONTEXT_MENU_EXT2
    public IPathCopyCopyContextMenuExt2,
#else
    public IPathCopyCopyContextMenuExt,
#endif // PCC_NO_CONTEXT_MENU_EXT2
    public IShellExtInit,
    public IContextMenu
{
public:
    CPathCopyCopyContextMenuExt() noexcept(false);
    CPathCopyCopyContextMenuExt(const CPathCopyCopyContextMenuExt&) = delete;
    CPathCopyCopyContextMenuExt(CPathCopyCopyContextMenuExt&&) = delete;
    CPathCopyCopyContextMenuExt& operator=(const CPathCopyCopyContextMenuExt&) = delete;
    CPathCopyCopyContextMenuExt& operator=(CPathCopyCopyContextMenuExt&&) = delete;
    virtual ~CPathCopyCopyContextMenuExt();

#pragma warning(push)
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)

    DECLARE_REGISTRY_RESOURCEID(IDR_PATHCOPYCOPYCONTEXTMENUEXT)

    DECLARE_NOT_AGGREGATABLE(CPathCopyCopyContextMenuExt)

    BEGIN_COM_MAP(CPathCopyCopyContextMenuExt)
        COM_INTERFACE_ENTRY(IPathCopyCopyContextMenuExt)
#ifndef PCC_NO_CONTEXT_MENU_EXT2
        COM_INTERFACE_ENTRY(IPathCopyCopyContextMenuExt2)
#endif // PCC_NO_CONTEXT_MENU_EXT2
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

#pragma warning(pop)

    [[gsl::suppress(c.128)]]
    HRESULT FinalConstruct() noexcept
    {
        return S_OK;
    }

    [[gsl::suppress(c.128)]]
    void FinalRelease() noexcept
    {
    }

public:
    // IPathCopyCopyContextMenuExt methods
    STDMETHOD(RegisterPlugin)(REFCLSID p_CLSID);
    STDMETHOD(UnregisterPlugin)(REFCLSID p_CLSID);

#ifndef PCC_NO_CONTEXT_MENU_EXT2
    // IPathCopyCopyContextMenuExt2 methods
    STDMETHOD(RegisterPlugin2)(REFCLSID p_CLSID, VARIANT_BOOL p_User);
    STDMETHOD(UnregisterPlugin2)(REFCLSID p_CLSID, VARIANT_BOOL p_User);
#endif // PCC_NO_CONTEXT_MENU_EXT2

    // IShellExtInit methods
    STDMETHOD(Initialize)(PCIDLIST_ABSOLUTE p_pFolderPIDL, IDataObject *p_pDataObject, HKEY p_hKeyFileClass);

    // IContextMenu methods
    STDMETHOD(QueryContextMenu)(HMENU p_hMenu, UINT p_Index, UINT p_FirstCmdId,
                                UINT p_LastCmdId, UINT p_Flags);
    STDMETHOD(InvokeCommand)(CMINVOKECOMMANDINFO* p_pCommandInfo);
    STDMETHOD(GetCommandString)(UINT_PTR p_CmdId, UINT p_Flags, UINT* p_pReserved,
                                LPSTR p_pBuffer, UINT p_BufferSize);

private:
    typedef std::map<UINT_PTR, PCC::PluginSP>               CmdIdPluginM;   // Map of plugins by command ID.

    typedef std::pair<CPathCopyCopyContextMenuExt*, HMENU>  ExtToMenuPair;  // Pairing an extension instance to an HMENU.
    typedef std::vector<ExtToMenuPair>                      ExtToMenuPairV; // Vector mapping extension instances to HMENUs.

    typedef std::shared_ptr<StImage>                        StImageSP;      // Shared pointer to a Win32 image wrapper.
    typedef std::map<std::wstring, StImageSP>               IconFilesM;     // Map of shared points to Win32 image wrappers, per icon file.

    PCC::SettingsSP     m_spSettings;               // Object to access program settings.
    PCC::PluginSPV      m_vspPluginsInDefaultOrder; // Vector of all plugins in default order.
    PCC::PluginSPS      m_sspAllPlugins;            // Set containing all plugins.
    PCC::PluginProviderSP
                        m_spPluginProvider;         // Plugin provider object.

    PCC::FilesV         m_vFiles;                   // Files selected in Shell.

    std::optional<UINT_PTR>
                        m_FirstCmdId;               // ID of first command menu item.
    std::optional<UINT_PTR>
                        m_SubMenuCmdId;             // ID of the menu item that opens our submenu.
    std::optional<UINT_PTR>
                        m_SettingsCmdId;            // ID of the menu item that triggers the options.
    CmdIdPluginM        m_mPluginsByCmdId;          // Map storing plugins by their command IDs.

    StImageSP           m_spPCCIcon;                // Holds the PCC icon that can be used in menus.
    IconFilesM          m_mspIcons;                 // Map of icons per icon file.

    static ExtToMenuPairV
                        s_vExtToMenus;              // Static map keeping track of which extensions modified which menu.
    static std::mutex   s_ExtToMenusLock;           // Lock to protect the static map.

    PCC::Settings&      GetSettings();
    
    std::wstring        GetParentPath() const;

    HRESULT             AddPluginToMenu(const GUID& p_PluginId,
                                        HMENU p_hMenu,
                                        bool p_UsePCCIcon,
                                        bool p_UsePreviewMode,
                                        bool p_DropRedundantWords,
                                        bool p_ComputeShortcut,
                                        UINT& p_rCmdId,
                                        UINT& p_rPosition);
    HRESULT             AddPluginToMenu(const PCC::PluginSP& p_spPlugin,
                                        HMENU p_hMenu,
                                        bool p_UsePCCIcon,
                                        bool p_UsePreviewMode,
                                        bool p_DropRedundantWords,
                                        bool p_ComputeShortcut,
                                        UINT& p_rCmdId,
                                        UINT& p_rPosition);
    std::wstring        GetMenuCaptionWithShortcut(HMENU p_hMenu,
                                                   const std::wstring& p_Caption) const;

    HBITMAP             GetPCCIcon();
    HBITMAP             GetIconForIconFile(const std::wstring& p_IconFile);

    HRESULT             ActOnFiles(const PCC::PluginSP& p_spPlugin,
                                   HWND p_hWnd);
    void                AddQuotes(std::wstring& p_rName,
                                  bool p_Optional) const;
    PCC::FilesV         GetFilesToActOn(bool p_Recursively) const;

    void                RemoveFromExtToMenu();
    void                CheckForUpdates();
};

#pragma warning(suppress: ALL_CPPCORECHECK_WARNINGS)
OBJECT_ENTRY_AUTO(__uuidof(PathCopyCopyContextMenuExt), CPathCopyCopyContextMenuExt)
