// AllPluginsProvider.h
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

#pragma once

#include "Plugin.h"
#include "PluginProvider.h"


namespace PCC
{
    //
    // Object to access plugins found in a set of all plugins.
    // Only stores a reference to the set.
    //
    class AllPluginsProvider : public PluginProvider
    {
    public:
        explicit        AllPluginsProvider(const PluginSPS& p_sspAllPlugins);
                        AllPluginsProvider(const AllPluginsProvider&) = delete;
        AllPluginsProvider&
                        operator=(const AllPluginsProvider&) = delete;

        virtual PluginSP GetPlugin(const GUID& p_PluginId) const override;

    private:
        const PluginSPS& m_sspAllPlugins;   // Set containing all plugins. We do not assume ownership.
    };

} // namespace PCC
