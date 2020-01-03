// PipelinePlugin.cpp
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
#include <PipelinePlugin.h>
#include <PluginPipeline.h>
#include <PluginPipelineDecoder.h>
#include <LaunchExecutablePathAction.h>

#include <assert.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        // @param p_PluginId ID of this pipeline plugin.
        // @param p_PluginDescription Description of the pipeline plugin.
        // @param p_PluginIconFile Path to icon file for plugin, or an empty string.
        // @param p_UseDefaultIcon Whether to use the default icon for this plugin.
        // @param p_EncodedElements String containing encoded pipeline elements.
        //
        PipelinePlugin::PipelinePlugin(const GUID& p_PluginId,
                                       const std::wstring& p_PluginDescription,
                                       const std::wstring& p_PluginIconFile,
                                       const bool p_UseDefaultIcon,
                                       const std::wstring& p_EncodedElements)
            : Plugin(),
              m_Id(p_PluginId),
              m_Description(p_PluginDescription),
              m_IconFile(p_PluginIconFile),
              m_UseDefaultIcon(p_UseDefaultIcon),
              m_spPipeline()
        {
            // Try decoding the encoded pipeline. If that fails, we'll
            // keep the plugin alive but it will be disabled.
            try {
                m_spPipeline = std::make_shared<Pipeline>(p_EncodedElements);
            } catch (const InvalidPipelineException&) {
                assert(m_spPipeline == nullptr);
            }
        }

        //
        // Returns the ID of the pipeline plugin, allowing it to be uniquely identified.
        //
        // @return Plugin ID.
        //
        const GUID& PipelinePlugin::Id() const noexcept(false)
        {
            return m_Id;
        }

        //
        // Returns a description of the pipeline plugin, to be used to display it
        // in the contextual menu.
        //
        // @return Plugin description.
        //
        std::wstring PipelinePlugin::Description() const
        {
            return m_Description;
        }

        //
        // Returns the path to the icon to use for this plugin.
        //
        // @return Path to icon file, or an empty string if not specified.
        //
        std::wstring PipelinePlugin::IconFile() const
        {
            return m_IconFile;
        }

        //
        // Whether to use the default icon for this plugin.
        //
        // @return true to use default icon, false otherwise.
        //
        bool PipelinePlugin::UseDefaultIcon() const noexcept(false)
        {
            return m_UseDefaultIcon;
        }

        //
        // Checks whether the plugin should be enabled or not in the contextual menu.
        // In our case, if we failed to decode our pipeline or if the pipeline complains,
        // we'll show up as disabled.
        //
        // @param p_ParentPath Path of the parent folder of all files; unused.
        // @param p_File Path of one selected file; unused.
        //
        bool PipelinePlugin::Enabled(const std::wstring& p_ParentPath,
                                     const std::wstring& p_File) const
        {
            return m_spPipeline != nullptr &&
                   m_spPipeline->ShouldBeEnabledFor(p_ParentPath, p_File, m_pPluginProvider);
        }

        //
        // Modifies a path using all elements in our pipeline.
        //
        // @param p_File Path of file to modify.
        // @return Modified path.
        //
        std::wstring PipelinePlugin::GetPath(const std::wstring& p_File) const
        {
            std::wstring modifiedPath(p_File);
            if (m_spPipeline != nullptr) {
                m_spPipeline->ModifyPath(modifiedPath, m_pPluginProvider);
            }
            return modifiedPath;
        }

        //
        // Returns the separator to use between each path when using this plugin.
        // The default value is the empty string, which instructs PCC to use the
        // default value (usually a newline).
        //
        // @return Separator between each path, or empty string to use default value.
        //
        std::wstring PipelinePlugin::PathsSeparator() const
        {
            // This is stored in pipeline options.
            std::wstring separator;
            if (m_spPipeline != nullptr) {
                PipelineOptions options;
                m_spPipeline->ModifyOptions(options);
                separator = options.GetPathsSeparator();
            }
            return separator;
        }

        //
        // Returns the action to perform on the path or paths when using this plugin.
        //
        // @return Path action instance to use.
        //
        PCC::PathActionSP PipelinePlugin::Action() const
        {
            // Pipeline options can modify the behavior.
            std::wstring executable;
            bool useFilelist = false;
            if (m_spPipeline != nullptr) {
                PipelineOptions options;
                m_spPipeline->ModifyOptions(options);
                executable = options.GetExecutable();
                useFilelist = options.GetUseFilelist();
            }
            
            PCC::PathActionSP spAction;
            if (!executable.empty()) {
                // Launch executable with paths as argument
                spAction = std::make_shared<PCC::Actions::LaunchExecutablePathAction>(executable, useFilelist);
            } else {
                // Use default behavior.
                spAction = Plugin::Action();
            }

            return spAction;
        }

        //
        // Called by Path Copy Copy to know if it should honor the
        // "Drop redundant words" setting for this plugin. In our
        // case, the plugin description has been set by the user
        // so we never want to modify it.
        //
        // @return Always false to indicate PCC should not drop
        //         redundant words like "copy" from plugin's description.
        //
        bool PipelinePlugin::CanDropRedundantWords() const noexcept(false)
        {
            return false;
        }

    } // namespace Plugins

} // namespace PCC
