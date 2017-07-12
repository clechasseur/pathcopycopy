// PathCopyCopyConfigHelper.cpp
// (c) 2011-2017, Charles Lechasseur
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
#include <PathCopyCopyConfigHelper.h>
#include <PathCopyCopyPluginsRegistry.h>
#include <PathCopyCopySettings.h>

#include <memory>


// CPathCopyCopyConfigHelper

//
// Constructor.
//
CPathCopyCopyConfigHelper::CPathCopyCopyConfigHelper()
    : m_spSettings(),
      m_sspAllPlugins(),
      m_spPluginProvider(),
      m_vspPlugins()
{
}

//
// IPathCopyCopyConfigHelper::PluginCount
//
// Returns the number of available plugins.
//
// @param p_pCount Will contain the number of available plugins.
// @return Result code.
//
STDMETHODIMP CPathCopyCopyConfigHelper::get_PluginCount(ULONG *p_pCount)
{
    Initialize();
    HRESULT hRes = S_OK;

    if (p_pCount != nullptr) {
        *p_pCount = static_cast<ULONG>(m_vspPlugins.size());
    } else {
        hRes = E_INVALIDARG;
    }

    return hRes;
}

//
// IPathCopyCopyConfigHelper::GetPluginInfo
//
// Returns information about an available plugin.
//
// @param p_Index 0-based index of the plugin requested.
// @param p_ppId Will contain a string representation of the plugin ID.
// @param p_ppDescription Will contain the plugin's description (what will appear in the contextual menu).
// @param p_pIsSeparator Will contain VARIANT_TRUE if this is a separator plugin, otherwise VARIANT_FALSE.
// @return Result code.
//
STDMETHODIMP CPathCopyCopyConfigHelper::GetPluginInfo(ULONG p_Index, BSTR *p_ppId, BSTR *p_ppDescription, VARIANT_BOOL *p_pIsSeparator)
{
    Initialize();
    HRESULT hRes = S_OK;

    if (p_ppId != nullptr && p_ppDescription != nullptr && p_Index < m_vspPlugins.size()) {
        const PCC::PluginSP& spPlugin = m_vspPlugins[p_Index];
        OLECHAR pluginId[40];
        if (::StringFromGUID2(spPlugin->Id(), pluginId, 40) != 0) {
            *p_ppId = ::SysAllocString(pluginId);
            *p_ppDescription = ::SysAllocString(spPlugin->Description().c_str());
            if (p_pIsSeparator != nullptr) {
                *p_pIsSeparator = spPlugin->IsSeparator() ? VARIANT_TRUE : VARIANT_FALSE;
            }
        } else {
            hRes = E_FAIL;
        }
    } else {
        hRes = E_INVALIDARG;
    }

    return hRes;
}

//
// Performs late-initialization of members.
//
void CPathCopyCopyConfigHelper::Initialize()
{
    // Only initialize once.
    if (m_spSettings == nullptr) {
        // This helper allowed the UI to access all known plugins in the proper order.
        // What we'll do is fetch set of all plugins, then order them as we would in the submenu.
        // We'll also include unknown plugins at the end.
        m_spSettings = std::make_shared<PCC::Settings>();
        m_vspPluginsInDefaultOrder = PCC::PluginsRegistry::GetPluginsInDefaultOrder(m_spSettings.get(), m_spSettings.get(), true);
        m_sspAllPlugins.insert(m_vspPluginsInDefaultOrder.cbegin(), m_vspPluginsInDefaultOrder.cend());
        m_spPluginProvider = std::make_shared<PCC::AllPluginsProvider>(m_sspAllPlugins);
        PCC::GUIDV vKnownPlugins, vSubmenuPluginDisplayOrder;
        const PCC::GUIDV* const pvKnownPlugins = m_spSettings->GetKnownPlugins(vKnownPlugins) ? &vKnownPlugins : nullptr;
        if (m_spSettings->GetSubmenuPluginDisplayOrder(vSubmenuPluginDisplayOrder)) {
            m_vspPlugins = PCC::PluginsRegistry::OrderPluginsToDisplay(m_sspAllPlugins,
                vSubmenuPluginDisplayOrder, pvKnownPlugins, &m_vspPluginsInDefaultOrder);
        } else {
            // Submenu plugin display order unspecified, use default.
            m_vspPlugins = m_vspPluginsInDefaultOrder;
        }

        // Provide each plugin with settings object and plugin provider, since some require this to work.
        for (const PCC::PluginSP& spPlugin : m_vspPlugins) {
            spPlugin->SetSettings(m_spSettings.get());
            spPlugin->SetPluginProvider(m_spPluginProvider.get());
        }
    }
}
