// PathCopyCopyPluginsRegistry.h
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

#pragma once

#include <COMPlugin.h>
#include "COMPluginProvider.h"
#include "PathCopyCopyPrivateTypes.h"
#include "PipelinePluginProvider.h"
#include "Plugin.h"

#include <memory>
#include <vector>


namespace PCC
{
    //
    // PluginsRegistry
    //
    // Static class responsible for providing all known PCC plugins.
    //
    class PluginsRegistry final
    {
    public:
                        PluginsRegistry() = delete;
                        ~PluginsRegistry() = delete;

        static PluginSPV GetPluginsInDefaultOrder(const COMPluginProvider* const p_pCOMPluginProvider,
                                                  const PipelinePluginProvider* const p_pPipelinePluginProvider,
                                                  const bool p_IncludeTempPipelinePlugins);

        static PluginSPV OrderPluginsToDisplay(const PluginSPS& p_sspAllPlugins,
                                               const GUIDV& p_vPluginDisplayOrder,
                                               const GUIDV* const p_pvKnownPlugins,
                                               const PluginSPV* const p_pvspPluginsInDefaultOrder);

    private:
        // Reference to a COM plugin.
        typedef std::shared_ptr<Plugins::COMPlugin> COMPluginSP;

        // Bean containing information about a COM plugin.
        struct COMPluginInfo {
            CLSID       m_CLSID;            // CLSID of COM object implementing the plugin.
            COMPluginSP m_spPlugin;         // The actual COM plugin.
            ULONG       m_GroupId;          // ID of plugin group.
            ULONG       m_GroupPosition;    // Position of plugin in the group.

            bool        operator<(const COMPluginInfo& p_Right) const;
        };

        // Vector of COM plugin info beans.
        typedef std::vector<COMPluginInfo> COMPluginInfoV;

        static void     GetDefaultPlugins(PluginSPV& p_rvspPlugins);

        static void     GetCOMPlugins(const COMPluginProvider& p_COMPluginProvider,
                                      PluginSPV& p_rvspPlugins);
        static void     GetPipelinePlugins(const PipelinePluginProvider& p_PipelinePluginProvider,
                                           const bool p_IncludeTempPipelinePlugins,
                                           PluginSPV& p_rvspPlugins);
    };

} // namespace PCC
