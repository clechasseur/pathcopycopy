// PluginPipeline.h
// (c) 2011-2021, Charles Lechasseur
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
#include "PluginProvider.h"

#include <exception>
#include <stack>
#include <string>


namespace PCC
{

    //
    // PipelineOptions
    //
    // Bean class storing options for a plugin pipeline. Options
    // can be modified by pipeline elements but apply globally.
    //
    class PipelineOptions final
    {
    public:
        const std::wstring&
                        GetPathsSeparator() const noexcept;
        void            SetPathsSeparator(const std::wstring& p_PathsSeparator);

        bool            GetCopyPathsRecursively() const noexcept;
        void            SetCopyPathsRecursively(bool p_Recursively) noexcept;

        const std::wstring&
                        GetExecutable() const noexcept;
        void            SetExecutable(const std::wstring& p_Executable);

        const std::wstring&
                        GetArguments() const noexcept;
        void            SetArguments(const std::wstring& p_Arguments);

        bool            GetUseFilelist() const noexcept;
        void            SetUseFilelist(bool p_UseFilelist) noexcept;

        bool            GetShowForFiles() const noexcept;
        void            SetShowForFiles(bool p_ShowForFiles) noexcept;

        bool            GetShowForFolders() const noexcept;
        void            SetShowForFolders(bool p_ShowForFolders) noexcept;

    private:
        std::wstring    m_PathsSeparator;           // Separator to use between multiple paths.
        bool            m_Recursively = false;      // Whether to copy paths recursively.
        std::wstring    m_Executable;               // Path to executable to start.
        std::wstring    m_Arguments;                // Arguments to pass to the executable.
        bool            m_UseFilelist = false;      // Whether to launch executable with filelist instead of paths directly.
        bool            m_ShowForFiles = true;      // Whether to display the plugin when files are selected.
        bool            m_ShowForFolders = true;    // Whether to display the plugin when folders are selected.
    };

    //
    // Pipeline
    //
    // Class representing a pipeline in which each element can apply
    // changes to a path. Meant to be used by custom path plugins.
    //
    class Pipeline final
    {
    public:
        explicit        Pipeline(const PipelineElementSPV& p_vspElements);
        explicit        Pipeline(const std::wstring& p_EncodedElements);
                        Pipeline(const Pipeline&) = delete;
        Pipeline&       operator=(const Pipeline&) = delete;

        void            Validate(const PluginProvider* p_pPluginProvider,
                                 GUIDS& p_rsSeenPluginIds) const;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const;
        void            ModifyOptions(PipelineOptions& p_rOptions) const;
        bool            ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* p_pPluginProvider) const;

    private:
        const PipelineElementSPV
                        m_vspElements;      // Elements in the pipeline.
    };

    //
    // PipelineElement
    //
    // Element in a pipeline. Each element can apply its own modification to a path.
    //
    class PipelineElement
    {
    public:
                        PipelineElement() noexcept = default;
                        PipelineElement(const PipelineElement&) = delete;
                        PipelineElement(PipelineElement&&) = delete;
        PipelineElement& operator=(const PipelineElement&) = delete;
        PipelineElement& operator=(PipelineElement&&) = delete;
        virtual         ~PipelineElement() = default;

        virtual void    Validate(const PluginProvider* p_pPluginProvider,
                                 GUIDS& p_rsSeenPluginIds) const noexcept(false);

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   std::stack<std::wstring>& p_rStack,
                                   const PluginProvider* p_pPluginProvider) const noexcept(false);
        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const noexcept(false);
        virtual void    ModifyOptions(PipelineOptions& p_rOptions) const noexcept(false);
        virtual bool    ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* p_pPluginProvider) const noexcept(false);
    };

    //
    // Exception type thrown when a pipeline is invalid.
    //
    class InvalidPipelineException : public std::runtime_error
    {
    public:
                        [[gsl::suppress(f.6)]]
                        InvalidPipelineException();
        explicit        InvalidPipelineException(const char* p_pWhat);
    };

} // namespace PCC
