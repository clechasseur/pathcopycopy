// PipelinenPluginProvider.h
// (c) 2016-2019, Charles Lechasseur
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

#include "PathCopyCopyPrivateTypes.h"
#include "Plugin.h"


namespace PCC
{
    //
    // PipelinePluginProvider
    //
    // Interface for an object that can provide pipeline plugins.
    //
    class PipelinePluginProvider
    {
    public:
                        PipelinePluginProvider() = default;
                        PipelinePluginProvider(const PipelinePluginProvider&) = delete;
                        PipelinePluginProvider(PipelinePluginProvider&&) = delete;
        PipelinePluginProvider&
                        operator=(const PipelinePluginProvider&) = delete;
        PipelinePluginProvider&
                        operator=(PipelinePluginProvider&&) = delete;
        virtual         ~PipelinePluginProvider() = default;

        //
        // Returns a list of pipeline plugins.
        //
        // @param p_rvspPlugins Where to store pipeline plugins. If the vector already
        //                      contains plugins, a separator will be added.
        //
        virtual void    GetPipelinePlugins(PluginSPV& p_rvspPlugins) const = 0;

        //
        // Returns a list of temp pipeline plugins. Such plugins are saved in a temp registry
        // entry by the settings app to handle pipeline plugins that are not yet saved.
        //
        // @param p_rvspPlugins Where to store pipeline plugins.
        //
        virtual void    GetTempPipelinePlugins(PluginSPV& p_rvspPlugins) const = 0;
    };

} // namespace PCC
