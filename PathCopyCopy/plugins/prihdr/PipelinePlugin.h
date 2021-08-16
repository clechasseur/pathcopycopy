// PipelinePlugin.h
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

#include <PathCopyCopyPrivateTypes.h>
#include <Plugin.h>

#include <optional>


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
                                                       bool p_UseDefaultIcon,
                                                       const std::wstring& p_EncodedElements);
                                        PipelinePlugin(const PipelinePlugin&) = delete;
            PipelinePlugin&             operator=(const PipelinePlugin&) = delete;

            const Pipeline*             GetPipeline(GUIDS* p_psSeenPluginIds = nullptr) const;
            std::string                 GetPipelineError() const;

            const GUID&                 Id() const noexcept(false) override;

            std::wstring                Description() const override;
            std::wstring                IconFile() const override;
            bool                        UseDefaultIcon() const noexcept(false) override;
            bool                        Enabled(const std::wstring& p_ParentPath,
                                                const std::wstring& p_File) const override;

            std::wstring                GetPath(const std::wstring& p_File) const override;
            std::wstring                PathsSeparator() const override;
            bool                        CopyPathsRecursively() const override;

            PCC::PathActionSP           Action() const override;

            bool                        CanDropRedundantWords() const noexcept(false) override;

            bool                        ShowForFiles() const noexcept(false) override;
            bool                        ShowForFolders() const noexcept(false) override;

        private:
            const GUID                  m_Id;               // Plugin ID.
            const std::wstring          m_Description;      // Plugin description.
            const std::wstring          m_IconFile;         // Plugin icon file.
            const bool                  m_UseDefaultIcon;   // Whether to use default icon for plugin.
            const std::wstring          m_EncodedElements;  // Pipeline encoded elements.
            mutable std::optional<PipelineSP>
                                        m_spPipeline;       // Pipeline to execute on each path received.
            mutable std::string         m_PipelineError;    // If pipeline failed to load, can contain the error message.
        };

    } // namespace Plugins

} // namespace PCC
