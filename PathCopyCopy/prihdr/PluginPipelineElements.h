// PluginPipelineElements.h
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

#pragma once

#include <PluginPipeline.h>

#include <memory>
#include <regex>
#include <string>

#include <windows.h>


namespace PCC
{
    //
    // FollowSymlinkPipelineElement
    //
    // Pipeline element that replaces paths to symlinks with the paths
    // to their targets.
    //
    class FollowSymlinkPipelineElement : public PipelineElement
    {
    public:
                        FollowSymlinkPipelineElement() = default;
                        FollowSymlinkPipelineElement(const FollowSymlinkPipelineElement&) = delete;
        FollowSymlinkPipelineElement&
                        operator=(const FollowSymlinkPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // QuotesPipelineElement
    //
    // Pipeline element that adds quotes around the path.
    //
    class QuotesPipelineElement : public PipelineElement
    {
    public:
                        QuotesPipelineElement() = default;
                        QuotesPipelineElement(const QuotesPipelineElement&) = delete;
        QuotesPipelineElement&
                        operator=(const QuotesPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // OptionalQuotesPipelineElement
    //
    // Pipeline element that adds quotes around the path if there
    // are spaces in the path.
    //
    class OptionalQuotesPipelineElement : public PipelineElement
    {
    public:
                        OptionalQuotesPipelineElement() = default;
                        OptionalQuotesPipelineElement(const OptionalQuotesPipelineElement&) = delete;
        OptionalQuotesPipelineElement&
                        operator=(const OptionalQuotesPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // EmailLinksPipelineElement
    //
    // Pipeline element that turns paths into e-mail links
    // by surrounding them with < and >.
    //
    class EmailLinksPipelineElement : public PipelineElement
    {
    public:
                        EmailLinksPipelineElement() = default;
                        EmailLinksPipelineElement(const EmailLinksPipelineElement&) = delete;
        EmailLinksPipelineElement&
                        operator=(const EmailLinksPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // EncodeURIWhitespacePipelineElement
    //
    // Pipeline element that encodes whitespace in URIs
    // with percent-encoding (e.g., %20)
    //
    class EncodeURIWhitespacePipelineElement : public PipelineElement
    {
    public:
                        EncodeURIWhitespacePipelineElement() = default;
                        EncodeURIWhitespacePipelineElement(const EncodeURIWhitespacePipelineElement&) = delete;
        EncodeURIWhitespacePipelineElement&
                        operator=(const EncodeURIWhitespacePipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // EncodeURICharsPipelineElement
    //
    // Pipeline element that encodes characters invalid in URIs
    // with percent-encoding (e.g., %20 for space, etc.)
    //
    class EncodeURICharsPipelineElement : public PipelineElement
    {
    public:
                        EncodeURICharsPipelineElement() = default;
                        EncodeURICharsPipelineElement(const EncodeURICharsPipelineElement&) = delete;
        EncodeURICharsPipelineElement&
                        operator=(const EncodeURICharsPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // BackToForwardSlashesPipelineElement
    //
    // Pipeline element that changes all backslashes in a path to
    // forward slashes.
    //
    class BackToForwardSlashesPipelineElement : public PipelineElement
    {
    public:
                        BackToForwardSlashesPipelineElement() = default;
                        BackToForwardSlashesPipelineElement(const BackToForwardSlashesPipelineElement&) = delete;
        BackToForwardSlashesPipelineElement&
                        operator=(const BackToForwardSlashesPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // ForwardToBackslashesPipelineElement
    //
    // Pipeline element that changes all forward slashes in a path
    // to backslashes.
    //
    class ForwardToBackslashesPipelineElement : public PipelineElement
    {
    public:
                        ForwardToBackslashesPipelineElement() = default;
                        ForwardToBackslashesPipelineElement(const ForwardToBackslashesPipelineElement&) = delete;
        ForwardToBackslashesPipelineElement&
                        operator=(const ForwardToBackslashesPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // RemoveFileExtPipelineElement
    //
    // Pipeline element that removes the file extension in a path.
    //
    class RemoveFileExtPipelineElement : public PipelineElement
    {
    public:
                        RemoveFileExtPipelineElement() = default;
                        RemoveFileExtPipelineElement(const RemoveFileExtPipelineElement&) = delete;
        RemoveFileExtPipelineElement&
                        operator=(const RemoveFileExtPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // FindReplacePipelineElement
    //
    // Pipeline element that replaces all instances of one string
    // in the path with another string.
    //
    class FindReplacePipelineElement : public PipelineElement
    {
    public:
                        FindReplacePipelineElement(const std::wstring& p_OldValue,
                                                   const std::wstring& p_NewValue);
                        FindReplacePipelineElement(const FindReplacePipelineElement&) = delete;
        FindReplacePipelineElement&
                        operator=(const FindReplacePipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;

    private:
        const std::wstring
                        m_OldValue;     // Value to replace.
        const std::wstring
                        m_NewValue;     // Replacement value.
    };

    //
    // RegexPipelineElement
    //
    // Pipeline element that performs find/replace operations
    // using regular expressions.
    //
    class RegexPipelineElement : public PipelineElement
    {
    public:
                        RegexPipelineElement(const std::wstring& p_Regex,
                                             const std::wstring& p_Format,
                                             bool p_IgnoreCase);
                        RegexPipelineElement(const RegexPipelineElement&) = delete;
        RegexPipelineElement&
                        operator=(const RegexPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
        bool            ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* p_pPluginProvider) const override;

    private:
        const std::wstring
                        m_Regex;        // Regex to use to find matches.
        const std::wstring
                        m_Format;       // Format of replacement string.
        const bool      m_IgnoreCase;   // Whether to ignore case when looking for matches.
        mutable std::unique_ptr<std::wregex>
                        m_upRegex;      // Regex object to use to perform lookups.
        mutable bool    m_RegexTested;  // true if we tried to create m_upRegex.

        void            InitRegex() const;
    };

    //
    // UnexpandEnvironmentStringsPipelineElement
    //
    // Pipeline element that replaces certain parts of the path
    // by environment variable references.
    //
    class UnexpandEnvironmentStringsPipelineElement : public PipelineElement
    {
    public:
                        UnexpandEnvironmentStringsPipelineElement() = default;
                        UnexpandEnvironmentStringsPipelineElement(const UnexpandEnvironmentStringsPipelineElement&) = delete;
        UnexpandEnvironmentStringsPipelineElement&
                        operator=(const UnexpandEnvironmentStringsPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // ApplyPluginPipelineElement
    //
    // Pipeline element that fetches an existing plugin via its
    // ID and calls its GetPath method to modify the path.
    //
    class ApplyPluginPipelineElement : public PipelineElement
    {
    public:
        explicit        ApplyPluginPipelineElement(const GUID& p_PluginId) noexcept;
                        ApplyPluginPipelineElement(const ApplyPluginPipelineElement&) = delete;
        ApplyPluginPipelineElement&
                        operator=(const ApplyPluginPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
        bool            ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* p_pPluginProvider) const override;

    protected:
        const GUID      m_PluginId;     // ID of plugin to apply.
    };

    //
    // ApplyPipelinePluginPipelineElement
    //
    // Subclass of ApplyPluginPipelineElement that allows use
    // of other pipeline plugins to modify the path.
    //
    class ApplyPipelinePluginPipelineElement : public ApplyPluginPipelineElement
    {
    public:
        explicit        ApplyPipelinePluginPipelineElement(const GUID& p_PluginId) noexcept;
                        ApplyPipelinePluginPipelineElement(const ApplyPipelinePluginPipelineElement&) = delete;
        ApplyPipelinePluginPipelineElement&
                        operator=(const ApplyPipelinePluginPipelineElement&) = delete;

        bool            Valid(const PluginProvider* p_pPluginProvider,
                              GUIDS& p_rsSeenPluginIds) const override;
    };

    //
    // PathsSeparatorPipelineElement
    //
    // Pipeline element that does not modify the path but changes the
    // separator used between multiple copied paths.
    //
    class PathsSeparatorPipelineElement : public PipelineElement
    {
    public:
        explicit        PathsSeparatorPipelineElement(const std::wstring& p_PathsSeparator);
                        PathsSeparatorPipelineElement(const PathsSeparatorPipelineElement&) = delete;
        PathsSeparatorPipelineElement&
                        operator=(const PathsSeparatorPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const noexcept(false) override;
        void            ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        const std::wstring
                        m_PathsSeparator;   // Separator to use between multiple paths.
    };

    //
    // ExecutablePipelineElement
    //
    // Pipeline element that does not modify the path but instructs
    // Path Copy Copy to launch an executable with the path or paths
    // as argument, instead of copying them to the clipboard.
    //
    class ExecutablePipelineElement : public PipelineElement
    {
    public:
        explicit        ExecutablePipelineElement(const std::wstring& p_Executable);
                        ExecutablePipelineElement(const ExecutablePipelineElement&) = delete;
        ExecutablePipelineElement&
                        operator=(const ExecutablePipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const noexcept(false) override;
        void            ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        const std::wstring
                        m_Executable;       // Executable to launch.
    };

    //
    // ExecutableWithFilelistPipelineElement
    //
    // Pipeline element similar to ExecutablePipelineElement, but that
    // additionally instructs Path Copy Copy to launch the executable
    // with a filelist instead of with paths directly.
    //
    class ExecutableWithFilelistPipelineElement : public ExecutablePipelineElement
    {
    public:
        explicit        ExecutableWithFilelistPipelineElement(const std::wstring& p_Executable);
                        ExecutableWithFilelistPipelineElement(const ExecutableWithFilelistPipelineElement&) = delete;
        ExecutableWithFilelistPipelineElement&
                        operator=(const ExecutableWithFilelistPipelineElement&) = delete;

        void            ModifyOptions(PipelineOptions& p_rOptions) const override;
    };

} // namespace PCC
