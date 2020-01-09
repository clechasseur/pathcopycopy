// PathCopyCopySettings.h
// (c) 2009-2020, Charles Lechasseur
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

#include "AtlRegKey.h"
#include "COMPluginProvider.h"
#include "PathCopyCopyPrivateTypes.h"
#include "PipelinePluginProvider.h"
#include "Plugin.h"
#include "RegKey.h"
#include "StringUtils.h"
#include "UserOverrideableRegKey.h"

#include <cstdint>
#include <exception>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <windows.h>


namespace PCC
{
    //
    // Settings
    //
    // Class used to access the PathCopyCopy settings, be it per-user or globals.
    //
    // This class is not thread-safe. Each thread should create its own copy.
    //
    class Settings final : public COMPluginProvider,
                           public PipelinePluginProvider
    {
    public:
        explicit        Settings() noexcept(false);
                        Settings(const Settings&) = delete;
        Settings&       operator=(const Settings&) = delete;

        bool            GetUseHiddenShares() const;
        bool            GetUseFQDN() const;
        bool            GetAddQuotesAroundPaths() const;
        bool            GetAreQuotesOptional() const;
        bool            GetMakePathsIntoEmailLinks() const;
        StringUtils::EncodeParam
                        GetEncodeParam() const;
        bool            GetAppendSeparatorForDirectories() const;
        bool            GetUseIconForDefaultPlugin() const;
        bool            GetUseIconForSubmenu() const;
        bool            GetUsePreviewMode() const;
        bool            GetUsePreviewModeInMainMenu() const;
        bool            GetDropRedundantWords() const;
        bool            GetAlwaysShowSubmenu() const;
        std::wstring    GetPathsSeparator() const;
        bool            GetTrueLnkPaths() const;
        bool            GetCtrlKeyPlugin(GUID& p_rPluginId) const;
        bool            GetMainMenuPluginDisplayOrder(GUIDV& p_rvPluginIds) const;
        bool            GetSubmenuPluginDisplayOrder(GUIDV& p_rvPluginIds) const;
        bool            GetKnownPlugins(GUIDV& p_rvPluginIds) const;

        bool            NeedsUpdateCheck() const;
        void            SetLastUpdateCheckNow();

        std::optional<std::wstring>
                        GetIconFileForPlugin(const CLSID& p_PluginId) const;

        bool            GetEditingDisabled() const;

        CLSIDV          GetCOMPlugins() const override;
        bool            RegisterCOMPlugin(const CLSID& p_CLSID,
                                          const bool p_User);
        bool            UnregisterCOMPlugin(const CLSID& p_CLSID,
                                            const bool p_User);

        void            GetPipelinePlugins(PluginSPV& p_rvspPlugins) const override;
        void            GetTempPipelinePlugins(PluginSPV& p_rvspPlugins) const override;

        void            ApplyRevisions();
        static void     ApplyGlobalRevisions();

    private:
        mutable UserOverrideableRegKey
                        m_UserKey;                  // PCC user settings registry key.
        UserOverrideableRegKey
                        m_IconsKey;                 // PCC default plugin icons registry key.
        mutable UserOverrideableRegKey
                        m_PipelinePluginsKey;       // PCC user pipeline plugins registry key.
        UserOverrideableRegKey
                        m_TempPipelinePluginsKey;   // PCC user temporary pipeline plugins registry key.
        mutable AtlRegKey
                        m_UserFormsKey;             // PCC user forms registry key.
        AtlRegKey       m_UserPluginsKey;           // PCC user plugins registry key.
        AtlRegKey       m_GlobalPluginsKey;         // PCC global plugins registry key.
        bool            m_GlobalPluginsKeyReadOnly; // Whether the global plugins key is read-only.
        mutable bool    m_Revised;                  // Whether settings have been revised.

        void            Revise() const;

        static std::wstring
                        GetCOMPluginInfo(const CLSID& p_CLSID);
        static void     GetCOMPlugins(const RegKey& p_PluginsKey,
                                      CLSIDV& p_rvCLSIDs);
        static void     GetPipelinePlugins(const RegKey& p_PipelinePluginsKey,
                                           PluginSPV& p_rvspPlugins,
                                           const bool p_AddSeparator);

        // Helper to apply revisions to the settings
        class Reviser final
        {
        public:
                        Reviser() = delete;
                        ~Reviser() = delete;

            static void ApplyRevisions(RegKey& p_rUserKey,
                                       RegKey* p_pFormsKey,
                                       RegKey& p_rPipelinePluginsKey,
                                       const COMPluginProvider& p_COMPluginProvider);

        private:
            // Struct storing info passed to the revise functions.
            struct ReviseInfo {
                RegKey& m_rUserKey;                 // Reference to registry key storing user settings.
                RegKey* m_pFormsKey;                // Optional reference to registry key storing forms position/size.
                RegKey& m_rPipelinePluginsKey;      // Reference to registry key storing pipeline plugins.
                const COMPluginProvider&
                        m_COMPluginProvider;        // Reference to object to access COM plugins.

                        ReviseInfo(RegKey& p_rUserKey,
                                   RegKey* p_pFormsKey,
                                   RegKey& p_rPipelinePluginsKey,
                                   const COMPluginProvider& p_COMPluginProvider) noexcept;
                        ReviseInfo(const ReviseInfo&) = delete;
                ReviseInfo&
                        operator=(const ReviseInfo&) = delete;
            };

            // Function that can apply a revision on settings.
            typedef std::function<void(const ReviseInfo&)> RevisionFunc;

            // Map of numbered revisions with their associated function to apply them.
            typedef std::map<uint32_t, RevisionFunc> RevisionFuncM;

            static RevisionFuncM
                        CreateRevisionFuncMap();
            static void ApplyNewPluginOrganizationRevision201201311(const ReviseInfo& p_ReviseInfo);
            static void ApplyPluginsInMainMenuAdjustment201204051(const ReviseInfo& p_ReviseInfo);
            static void ApplyInitialMainMenuPluginDisplayOrder201601052(const ReviseInfo& p_ReviseInfo);
            static void ApplyInitialSubmenuPluginDisplayOrder201601053(const ReviseInfo& p_ReviseInfo);
            static void ApplyInitialKnownPlugins201601054(const ReviseInfo& p_ReviseInfo);
            static void ApplyInitialUIPluginDisplayOrder201707061(const ReviseInfo& p_ReviseInfo);
            static void ApplyNewPipelinePluginForm202001091(const ReviseInfo& p_ReviseInfo);
        };

        // Implementation of ICOMPluginProvider that uses a specific registry key.
        class RegCOMPluginProvider final : public COMPluginProvider
        {
        public:
            explicit    RegCOMPluginProvider(const RegKey& p_PluginsKey) noexcept;
                        RegCOMPluginProvider(const RegCOMPluginProvider&) = delete;
            RegCOMPluginProvider&
                        operator=(const RegCOMPluginProvider&) = delete;

            CLSIDV      GetCOMPlugins() const override;

        private:
            const RegKey&
                        m_PluginsKey;       // Registry key containing (COM) plugins.
        };

        // Implementation of IPipelinePluginProvider that uses a specific registry key.
        // Does not support temp pipeline plugins.
        class RegPipelinePluginProvider final : public PipelinePluginProvider
        {
        public:
            explicit    RegPipelinePluginProvider(const RegKey& p_PipelinePluginsKey) noexcept;
                        RegPipelinePluginProvider(const RegPipelinePluginProvider&) = delete;
            RegPipelinePluginProvider&
                        operator=(const RegPipelinePluginProvider&) = delete;
        
            void        GetPipelinePlugins(PluginSPV& p_rvspPlugins) const override;
            void        GetTempPipelinePlugins(PluginSPV& p_rvspPlugins) const noexcept(false) override;

        private:
            const RegKey&
                        m_PipelinePluginsKey;   // Registry key containing pipeline plugins.
        };
    };

    //
    // SettingsException
    //
    // Exception class used to indicate an error accessing the PathCopyCopy registry settings.
    //
    class SettingsException : public std::exception
    {
    public:
                        SettingsException(const LONG p_ErrorCode) noexcept;

        LONG            ErrorCode() const noexcept;

        const char*     what() const noexcept(false) override;

    private:
        LONG            m_ErrorCode;        // The Windows error code.
    };

} // namespace PCC
