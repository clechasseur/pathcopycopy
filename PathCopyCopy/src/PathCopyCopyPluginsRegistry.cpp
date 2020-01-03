// PathCopyCopyPluginsRegistry.cpp
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
#include <PathCopyCopyPluginsRegistry.h>
#include <PathCopyCopySettings.h>
#include <PluginSeparator.h>

#include <CygwinPathPlugin.h>
#include <InternetPathPlugin.h>
#include <LongFolderPlugin.h>
#include <LongNamePlugin.h>
#include <LongPathPlugin.h>
#include <LongUNCFolderPlugin.h>
#include <LongUNCPathPlugin.h>
#include <SambaPathPlugin.h>
#include <ShortFolderPlugin.h>
#include <ShortNamePlugin.h>
#include <ShortPathPlugin.h>
#include <ShortUNCFolderPlugin.h>
#include <ShortUNCPathPlugin.h>
#include <UnixPathPlugin.h>
#include <WSLPathPlugin.h>
#include <MSYSPathPlugin.h>

#include <algorithm>
#include <iterator>
#include <utility>


namespace PCC
{
    //
    // Returns all plugins supported by PCC in the default order. This is how plugins will be
    // displayed in the submenu if the user hasn't specified something different.
    //
    // @param p_pCOMPluginProvider Optional object to acces COM plugins. If nullptr, COM plugins
    //                             will not be included in the returned vector.
    // @param p_pPipelinePluginProvider Optional object to access pipeline plugins. If nullptr,
    //                                  pipeline plugins will not be included in the returned vector.
    // @param p_IncludeTempPipelinePlugins Whether to also return temporary pipeline plugins.
    //                                     Ignored if p_pPipelinePluginProvider is nullptr.
    // @return Vector of plugins in default order.
    //
    PluginSPV PluginsRegistry::GetPluginsInDefaultOrder(const COMPluginProvider* const p_pCOMPluginProvider,
                                                        const PipelinePluginProvider* const p_pPipelinePluginProvider,
                                                        const bool p_IncludeTempPipelinePlugins)
    {
        PluginSPV vspPlugins;
        
        // Default plugins
        GetDefaultPlugins(vspPlugins);

        // COM plugins
        if (p_pCOMPluginProvider != nullptr) {
            GetCOMPlugins(*p_pCOMPluginProvider, vspPlugins);
        }

        // Pipeline plugins
        if (p_pPipelinePluginProvider != nullptr) {
            GetPipelinePlugins(*p_pPipelinePluginProvider, p_IncludeTempPipelinePlugins, vspPlugins);
        }

        return vspPlugins;
    }

    //
    // Given a vector of plugin IDs that specifies a plugin display order,
    // orders plugins found in a set of all plugins and returns a vector of
    // plugins in the order they should be displayed in the menu.
    //
    // @param p_sspAllPlugins Set containing all plugins.
    // @param p_vPluginDisplayOrder Vector of plugin IDs specifying display order.
    // @param p_pvKnownPlugins Optional list of known plugins. If set, all plugins
    //                         in p_sspAllPlugins that are not in this list will
    //                         be added at the end of the plugins to display.
    // @param p_pvspPluginsInDefaultOrder Optional list of plugins in default order.
    //                                    If set, should correspond to the default
    //                                    way to display plugins in p_sspAllPlugins.
    //                                    Ignored if p_pvKnownPlugins is nullptr.
    // @return Vector of plugins in the order they should be displayed.
    //
    PluginSPV PluginsRegistry::OrderPluginsToDisplay(const PluginSPS& p_sspAllPlugins,
                                                     const GUIDV& p_vPluginDisplayOrder,
                                                     const GUIDV* const p_pvKnownPlugins,
                                                     const PluginSPV* const p_pvspPluginsInDefaultOrder)
    {
        // First generate list of plugins from display order.
        PluginSPV vspPlugins;
        for (const GUID& pluginId : p_vPluginDisplayOrder) {
            const auto it = p_sspAllPlugins.find(pluginId);
            if (it != p_sspAllPlugins.end()) {
                vspPlugins.push_back(*it);
            }
        }

        // If we have a list of known plugins, add all unknown plugins
        // after those specified in the display order.
        if (p_pvKnownPlugins != nullptr) {
            // Sort known plugins to be able to perform a set difference.
            GUIDV vKnownPlugins(*p_pvKnownPlugins);
            GUIDLess guidLess;
            std::sort(vKnownPlugins.begin(), vKnownPlugins.end(), guidLess);

            // Create vector of plugin IDs for all plugins.
            GUIDV vAllPlugins;
            vAllPlugins.reserve(p_sspAllPlugins.size());
            for (const PluginSP& spPlugin : p_sspAllPlugins) {
                vAllPlugins.push_back(spPlugin->Id());
            }

            // Substract known plugins from list of all plugins to find unknown plugins.
            GUIDV vUnknownPlugins;
            std::set_difference(vAllPlugins.cbegin(), vAllPlugins.cend(),
                                vKnownPlugins.cbegin(), vKnownPlugins.cend(),
                                std::inserter(vUnknownPlugins, vUnknownPlugins.end()),
                                guidLess);
            if (!vUnknownPlugins.empty()) {
                // We have unknown plugins. Add a separator if needed, then add them
                // to the returned vector.
                if (!vspPlugins.empty() && !vspPlugins.back()->IsSeparator()) {
                    vspPlugins.push_back(std::make_shared<PluginSeparator>());
                }
                if (p_pvspPluginsInDefaultOrder != nullptr) {
                    // We know how to display plugins in default order: scan that
                    // list and add all unknown plugins. This will probably help
                    // display them in correct order.
                    const auto defEnd = p_pvspPluginsInDefaultOrder->cend();
                    for (auto defIt = p_pvspPluginsInDefaultOrder->cbegin(); defIt != defEnd; ++defIt) {
                        if (std::binary_search(vUnknownPlugins.cbegin(), vUnknownPlugins.cend(), (*defIt)->Id(), guidLess)) {
                            // This is an unknown plugin, add it.
                            vspPlugins.push_back(*defIt);

                            // If it's followed by a separator, add it also. This takes care
                            // of preserving COM plugin grouping.
                            auto defNext(defIt);
                            ++defNext;
                            if (defNext != defEnd && (*defNext)->IsSeparator()) {
                                vspPlugins.push_back(*defNext);
                            }
                        }
                    }
                } else {
                    // No info on how to display plugins, simply add them in
                    // a possibly-random order.
                    for (const GUID& unknownPluginId : vUnknownPlugins) {
                        const auto it = p_sspAllPlugins.find(unknownPluginId);
                        if (it != p_sspAllPlugins.end()) {
                            vspPlugins.push_back(*it);
                        }
                    }
                }
            }
        }

        // Return final list of plugins.
        return vspPlugins;
    }

    //
    // Returns all default (e.g. built-in) plugins in the default order.
    //
    // @param p_rvspPlugins Vector where to store plugins.
    //
    void PluginsRegistry::GetDefaultPlugins(PluginSPV& p_rvspPlugins)
    {
        PluginSP spSeparator = std::make_shared<PluginSeparator>();

        // Name plugins
        p_rvspPlugins.push_back(std::make_shared<Plugins::ShortNamePlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::LongNamePlugin>());

        // Path plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::ShortPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::LongPathPlugin>());

        // Folder plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::ShortFolderPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::LongFolderPlugin>());

        // UNC path plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::ShortUNCPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::LongUNCPathPlugin>());

        // UNC folder plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::ShortUNCFolderPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::LongUNCFolderPlugin>());

        // Internet plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::InternetPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::SambaPathPlugin>());

        // *NIX plugins
        p_rvspPlugins.push_back(spSeparator);
        p_rvspPlugins.push_back(std::make_shared<Plugins::UnixPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::CygwinPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::WSLPathPlugin>());
        p_rvspPlugins.push_back(std::make_shared<Plugins::MSYSPathPlugin>());
    }

    //
    // Adds all COM plugins to the given vector.
    // Takes care of sorting them by group info (ID and position).
    //
    // @param p_COMPluginProvider Object to access registered COM plugins.
    // @param p_rvspPlugins Vector where to store plugins.
    //
    void PluginsRegistry::GetCOMPlugins(const COMPluginProvider& p_COMPluginProvider,
                                        PluginSPV& p_rvspPlugins)
    {
        // Get list of COM plugins from settings.
        CLSIDV vPluginCLSIDs = p_COMPluginProvider.GetCOMPlugins();
        if (!vPluginCLSIDs.empty()) {
            // Load list of plugins by creating the COM objects and store group IDs and positions.
            COMPluginInfoV vCOMPluginInfos;            
            for (const CLSID& clsid : vPluginCLSIDs) {
                try {
                    COMPluginInfo pluginInfo;
                    pluginInfo.m_CLSID = clsid;
                    pluginInfo.m_spPlugin = std::make_shared<Plugins::COMPlugin>(clsid);
                    pluginInfo.m_GroupId = pluginInfo.m_spPlugin->GroupId();
                    pluginInfo.m_GroupPosition = pluginInfo.m_spPlugin->GroupPosition();

                    // Only care about position if the group ID is set.
                    if (pluginInfo.m_GroupId == 0) {
                        pluginInfo.m_GroupPosition = 0;
                    }
                    vCOMPluginInfos.push_back(std::move(pluginInfo));
                } catch (const Plugins::COMPluginError&) {
                    // The plugin is not functional, simply skip it.
                }
            }

            // Check if we have some plugins left. We might fail to load them.
            if (!vCOMPluginInfos.empty()) {
                // Sort plugins by group ID and position.
                if (vCOMPluginInfos.size() > 1) {
                    std::stable_sort(vCOMPluginInfos.begin(), vCOMPluginInfos.end());
                }

                // Register all plugins. Insert a separator between each group.
                PluginSP spSeparator = std::make_shared<PluginSeparator>();
                ULONG currentGroup = 0xFFFFFFFF;
                for (const COMPluginInfo& pluginInfo : vCOMPluginInfos) {
                    if (pluginInfo.m_GroupId != currentGroup) {
                        p_rvspPlugins.push_back(spSeparator);
                    }
                    p_rvspPlugins.push_back(pluginInfo.m_spPlugin);
                    currentGroup = pluginInfo.m_GroupId;
                }
            }
        }
    }

    //
    // Adds all pipeline plugins to the given vector.
    //
    // @param p_PipelinePluginProvider Object to access pipeline plugins.
    // @param p_IncludeTempPipelinePlugins Whether to also return temporary pipeline plugins.
    // @param p_rvspPlugins Vector where to store plugins.
    //
    void PluginsRegistry::GetPipelinePlugins(const PipelinePluginProvider& p_PipelinePluginProvider,
                                             const bool p_IncludeTempPipelinePlugins,
                                             PluginSPV& p_rvspPlugins)
    {
        PluginSPV vspPipelinePlugins;
        p_PipelinePluginProvider.GetPipelinePlugins(vspPipelinePlugins);
        if (p_IncludeTempPipelinePlugins) {
            p_PipelinePluginProvider.GetTempPipelinePlugins(vspPipelinePlugins);
        }
        if (!vspPipelinePlugins.empty()) {
            if (!p_rvspPlugins.empty() && !p_rvspPlugins.back()->IsSeparator()) {
                p_rvspPlugins.push_back(std::make_shared<PluginSeparator>());
            }
            std::move(vspPipelinePlugins.begin(), vspPipelinePlugins.end(), std::back_inserter(p_rvspPlugins));
        }
    }

    //
    // Compares two COMPluginInfo beans, sorting them by ascending
    // order of group ID and position.
    //
    // @param p_Right Bean to compare with this one.
    // @return true if this bean is before p_Right.
    //
    bool PluginsRegistry::COMPluginInfo::operator<(const COMPluginInfo& p_Right) const noexcept
    {
        return m_GroupId < p_Right.m_GroupId ||
               (m_GroupId == p_Right.m_GroupId && m_GroupPosition < p_Right.m_GroupPosition);
    }

} // namespace PCC
