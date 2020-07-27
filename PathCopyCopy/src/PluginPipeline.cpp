// PluginPipeline.cpp
// (c) 2011-2020, Charles Lechasseur
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
#include <PluginPipeline.h>
#include <PluginPipelineDecoder.h>


namespace PCC
{
    //
    // Returns the separator to use between multiple copied paths.
    //
    // @return Separator between multiple paths. An empty string
    //         means to use the default value.
    //
    const std::wstring& PipelineOptions::GetPathsSeparator() const noexcept
    {
        return m_PathsSeparator;
    }

    //
    // Sets the separator to use between multiple paths. An empty
    // string means to use the default value.
    //
    // @param p_PathsSeparator Separator between multiple paths
    //                         or empty string for default value.
    //
    void PipelineOptions::SetPathsSeparator(const std::wstring& p_PathsSeparator)
    {
        m_PathsSeparator = p_PathsSeparator;
    }

    //
    // Returns whether to copy paths recursively or not.
    //
    // @return Whether to copy paths recursively.
    //
    bool PipelineOptions::GetCopyPathsRecursively() const noexcept
    {
        return m_Recursively;
    }

    //
    // Sets whether to copy path recursively or not.
    //
    // @param p_Recursively Whether to copy paths recursively.
    //
    void PipelineOptions::SetCopyPathsRecursively(const bool p_Recursively) noexcept
    {
        m_Recursively = p_Recursively;
    }

    //
    // Returns the executable to start with the path or paths as argument.
    //
    // @return Path of executable to start. An empty string means
    //         to copy to clipboard instead.
    //
    const std::wstring& PipelineOptions::GetExecutable() const noexcept
    {
        return m_Executable;
    }

    //
    // Sets the executable to start with the path or paths as argument.
    // An empty string means to copy paths to the clipboard instead.
    //
    // @param p_Executable Executable to start or empty string to
    //                     use default behavior of copying to clipboard.
    //
    void PipelineOptions::SetExecutable(const std::wstring& p_Executable)
    {
        m_Executable = p_Executable;
        // When executable changes, reset arguments.
        m_Arguments = {};
    }

    //
    // Returns the arguments to pass to the executable.
    //
    // @return Arguments to pass to the executable. Can contain the
    //         %FILES% placeholder to replace with the files.
    //
    const std::wstring& PipelineOptions::GetArguments() const noexcept
    {
        return m_Arguments;
    }

    //
    // Sets the arguments to pass to the executable.
    //
    // @param p_Arguments Arguments to pass to the executable. Can contain the
    //                    %FILES% placeholder to replace with the files.
    //
    void PipelineOptions::SetArguments(const std::wstring& p_Arguments)
    {
        m_Arguments = p_Arguments;
    }

    //
    // Returns whether to launch executable with filelist instead of paths directly.
    //
    // @return Whether to launch executable with filelist instead of paths.
    //
    bool PipelineOptions::GetUseFilelist() const noexcept
    {
        return m_UseFilelist;
    }

    //
    // Sets whether to launch executable with filelist instead of paths directly.
    //
    // @param p_UseFilelist true to launch executable with filelist instead of paths.
    //
    void PipelineOptions::SetUseFilelist(const bool p_UseFilelist) noexcept
    {
        m_UseFilelist = p_UseFilelist;
    }

    //
    // Constructor with pre-built elements.
    //
    // @param p_vspElements List of elements in the pipeline.
    //
    Pipeline::Pipeline(const PipelineElementSPV& p_vspElements)
        : m_vspElements(p_vspElements)
    {
    }

    //
    // Constructor with encoded pipeline. The pipeline is
    // built by decoding the string. See PipelineDecoder.
    //
    // @param p_EncodedElements Elements encoded in a string.
    //
    Pipeline::Pipeline(const std::wstring& p_EncodedElements)
        : m_vspElements(PipelineDecoder::DecodePipeline(p_EncodedElements))
    {
    }

    //
    // Validates the pipeline. A pipeline is valid if all its elements
    // are considered valid.
    //
    // When the pipeline is invalid, an InvalidPipelineException is thrown.
    //
    // @param p_pPluginProvider Optional plugin provider that can be used
    //                          during validation.
    // @param p_rsSeenPluginIds Set that should be used to store seen plugin IDs.
    //                          Any collision means a loop is detected and
    //                          pipeline should be considered invalid.
    //
    void Pipeline::Validate(const PluginProvider* const p_pPluginProvider,
                            GUIDS& p_rsSeenPluginIds) const
    {
        for (const auto& spElement : m_vspElements) {
            spElement->Validate(p_pPluginProvider, p_rsSeenPluginIds);
        }
    }


    //
    // Modifies a given path by successively applying all pipeline
    // elements to it. Returns the final version of the path.
    //
    // @param p_rPath Path to modify. Will be modified in-place.
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void Pipeline::ModifyPath(std::wstring& p_rPath,
                              const PluginProvider* const p_pPluginProvider) const
    {
        std::stack<std::wstring> aStack;
        for (const auto& spElement : m_vspElements) {
            spElement->ModifyPath(p_rPath, aStack, p_pPluginProvider);
        }
    }

    //
    // Modifies global pipeline options by successively applying all pipeline
    // elements to it. Returns the final version of the pipeline options.
    //
    // @param p_rOptions Global pipeline options to modify.
    //
    void Pipeline::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        for (const auto& spElement : m_vspElements) {
            spElement->ModifyOptions(p_rOptions);
        }
    }

    //
    // Checks if a plugin using this pipeline should be enabled or not.
    // Any part of the pipeline that returns false for this will make the item disabled.
    //
    // @param p_ParentPath Path of the parent folder for the file to check.
    // @param p_File Path of file to use for the check.
    // @param p_pPluginProvider Optional object to access plugins.
    // @return false if pipeline says plugin should be disabled for this path.
    //
    bool Pipeline::ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                      const std::wstring& p_File,
                                      const PluginProvider* const p_pPluginProvider) const
    {
        return std::all_of(m_vspElements.cbegin(), m_vspElements.cend(), [&](const auto& spElement) {
            return spElement->ShouldBeEnabledFor(p_ParentPath, p_File, p_pPluginProvider);
        });
    }

    //
    // Validates the pipeline element. Whether or not an element is "valid"
    // is implementation-specific, but should be used to detect recursion
    // in plugin usage, for instance.
    //
    // When the pipeline element is invalid, an InvalidPipelineException
    // should be thrown.
    //
    // @param p_pPluginProvider Optional plugin provider that can be used
    //                          during validation.
    // @param p_rsSeenPluginIds Set that should be used to store seen plugin IDs.
    //                          Any collision means a loop is detected and
    //                          pipeline element should be considered invalid.
    //
    void PipelineElement::Validate(const PluginProvider* const /*p_pPluginProvider*/,
                                   GUIDS& /*p_rsSeenPluginIds*/) const noexcept(false)
    {
        // All pipeline elements are considered valid by default.
    }

    //
    // Modifies the given path using element-specific logic.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_rStack Stack that can be used to execute operations.
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void PipelineElement::ModifyPath(std::wstring& p_rPath,
                                     std::stack<std::wstring>& /*p_rStack*/,
                                     const PluginProvider* const p_pPluginProvider) const noexcept(false)
    {
        // Most elements don't need the stack so just call the non-stack version.
        ModifyPath(p_rPath, p_pPluginProvider);
    }

    //
    // Modifies the given path using element-specific logic. This version does not
    // use the stack; the default implementation calls this, so elements that do
    // not interact with the stack can simply override this version.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void PipelineElement::ModifyPath(std::wstring& /*p_rPath*/,
                                     const PluginProvider* const /*p_pPluginProvider*/) const noexcept(false)
    {
        // Subclasses can override.
    }

    //
    // Modifies global pipeline options. Each element has the opportunity
    // to modify them when a path is modified.
    //
    // @param p_rOptions Global pipeline options to modify.
    //
    void PipelineElement::ModifyOptions(PipelineOptions& /*p_rOptions*/) const noexcept(false)
    {
    }

    //
    // Checks if a plugin using this pipeline element should be enabled or not.
    // Any part of a pipeline that returns false for this will make the item disabled.
    //
    // @param p_ParentPath Path of the parent folder for the file to check.
    // @param p_File Path of file to use for the check.
    // @param p_pPluginProvider Optional object to access plugins.
    // @return false if pipeline element says plugin should be disabled for this path.
    //
    bool PipelineElement::ShouldBeEnabledFor(const std::wstring& /*p_ParentPath*/,
                                             const std::wstring& /*p_File*/,
                                             const PluginProvider* const /*p_pPluginProvider*/) const noexcept(false)
    {
        return true;
    }

    //
    // Default constructor.
    //
    InvalidPipelineException::InvalidPipelineException()
        : std::runtime_error(ATL::CStringA(MAKEINTRESOURCEA(IDS_INVALIDPIPELINE)))
    {
    }

    //
    // Constructor with explanation string.
    //
    // @param p_pWhat Explanation string.
    //
    InvalidPipelineException::InvalidPipelineException(const char* const p_pWhat)
        : std::runtime_error(p_pWhat)
    {
    }

} // namespace PCC
