// PipelinePlugin.h
// (c) 2008-2017, Charles Lechasseur
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

#include <PathCopyCopyPrivateTypes.h>
#include <Plugin.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // PipelinePlugin
        //
        // Plugin that loads an encoded pipeline of elements that can all
        // modify a given path and executes each element on any path received.
        //
        class PipelinePlugin final : public Plugin
        {
        public:
                                        PipelinePlugin(const GUID& p_PluginId,
                                                       const std::wstring& p_PluginDescription,
                                                       const std::wstring& p_PluginIconFile,
                                                       const bool p_UseDefaultIcon,
                                                       const std::wstring& p_EncodedElements);
                                        PipelinePlugin(const PipelinePlugin&) = delete;
            PipelinePlugin&             operator=(const PipelinePlugin&) = delete;

            virtual const GUID&         Id() const override;

            virtual std::wstring        Description() const override;
            virtual std::wstring        IconFile() const override;
            virtual bool                UseDefaultIcon() const override;
            virtual bool                Enabled(const std::wstring& p_ParentPath,
                                                const std::wstring& p_File) const override;

            virtual std::wstring        GetPath(const std::wstring& p_File) const override;
            virtual std::wstring        PathsSeparator() const override;

            virtual bool                CanDropRedundantWords() const override;

        private:
            GUID                        m_Id;               // Plugin ID.
            std::wstring                m_Description;      // Plugin description.
            std::wstring                m_IconFile;         // Plugin icon file.
            bool                        m_UseDefaultIcon;   // Whether to use default icon for plugin.
            PipelineSP                  m_spPipeline;       // Pipeline to execute on each path received.
        };

    } // namespace Plugins

} // namespace PCC
