// AllPluginsProvider.cpp
// (c) 2016-2017, Charles Lechasseur
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


namespace PCC
{
    //
    // Constructor.
    //
    // @param p_sspAllPlugins Set containing all plugins. We only store the reference to this set;
    //                        it must remain alive for the lifetime of this plugin provider.
    //
    AllPluginsProvider::AllPluginsProvider(const PluginSPS& p_sspAllPlugins)
        : PluginProvider(),
          m_sspAllPlugins(p_sspAllPlugins)
    {
    }

    //
    // Looks for a specific plugin by ID.
    //
    // @param p_PluginId ID of plugin to look for.
    // @return Plugin with the given ID, or nullptr if no such plugin was found.
    //
    PluginSP AllPluginsProvider::GetPlugin(const GUID& p_PluginId) const
    {
        auto it = m_sspAllPlugins.find(p_PluginId);
        return it != m_sspAllPlugins.end() ? *it : nullptr;
    }

} // namespace PCC
