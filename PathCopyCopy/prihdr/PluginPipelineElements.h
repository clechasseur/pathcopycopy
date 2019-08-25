// PluginPipelineElements.h
// (c) 2011-2019, Charles Lechasseur
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
    // QuotesPipelineElement
    //
    // Pipeline element that adds quotes around the path.
    //
    class QuotesPipelineElement : public PipelineElement
    {
    public:
                        QuotesPipelineElement();
                        QuotesPipelineElement(const QuotesPipelineElement&) = delete;
        QuotesPipelineElement&
                        operator=(const QuotesPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        OptionalQuotesPipelineElement();
                        OptionalQuotesPipelineElement(const OptionalQuotesPipelineElement&) = delete;
        OptionalQuotesPipelineElement&
                        operator=(const OptionalQuotesPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        EmailLinksPipelineElement();
                        EmailLinksPipelineElement(const EmailLinksPipelineElement&) = delete;
        EmailLinksPipelineElement&
                        operator=(const EmailLinksPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        EncodeURIWhitespacePipelineElement();
                        EncodeURIWhitespacePipelineElement(const EncodeURIWhitespacePipelineElement&) = delete;
        EncodeURIWhitespacePipelineElement&
                        operator=(const EncodeURIWhitespacePipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        EncodeURICharsPipelineElement();
                        EncodeURICharsPipelineElement(const EncodeURICharsPipelineElement&) = delete;
        EncodeURICharsPipelineElement&
                        operator=(const EncodeURICharsPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        BackToForwardSlashesPipelineElement();
                        BackToForwardSlashesPipelineElement(const BackToForwardSlashesPipelineElement&) = delete;
        BackToForwardSlashesPipelineElement&
                        operator=(const BackToForwardSlashesPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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
                        ForwardToBackslashesPipelineElement();
                        ForwardToBackslashesPipelineElement(const ForwardToBackslashesPipelineElement&) = delete;
        ForwardToBackslashesPipelineElement&
                        operator=(const ForwardToBackslashesPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
    };

    //
    // RemoveFileExtPipelineElement
    //
    // Pipeline element that removes the file extension in a path.
    //
    class RemoveFileExtPipelineElement : public PipelineElement
    {
    public:
                        RemoveFileExtPipelineElement();
                        RemoveFileExtPipelineElement(const RemoveFileExtPipelineElement&) = delete;
        RemoveFileExtPipelineElement&
                        operator=(const RemoveFileExtPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
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

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;

    private:
        std::wstring    m_OldValue;     // Value to replace.
        std::wstring    m_NewValue;     // Replacement value.
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
                                             const bool p_IgnoreCase);
                        RegexPipelineElement(const RegexPipelineElement&) = delete;
        RegexPipelineElement&
                        operator=(const RegexPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
        virtual bool    ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* const p_pPluginProvider) const override;

    private:
        std::wstring    m_Regex;        // Regex to use to find matches.
        std::wstring    m_Format;       // Format of replacement string.
        bool            m_IgnoreCase;   // Whether to ignore case when looking for matches.
        mutable std::unique_ptr<std::wregex>
                        m_upRegex;      // Regex object to use to perform lookups.
        mutable bool    m_RegexTested;  // true if we tried to create m_upRegex.

        void            InitRegex() const;
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
        explicit        ApplyPluginPipelineElement(const GUID& p_PluginId);
                        ApplyPluginPipelineElement(const ApplyPluginPipelineElement&) = delete;
        ApplyPluginPipelineElement&
                        operator=(const ApplyPluginPipelineElement&) = delete;

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
        virtual bool    ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                           const std::wstring& p_File,
                                           const PluginProvider* const p_pPluginProvider) const override;

    private:
        GUID            m_PluginId;     // ID of plugin to apply.
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

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
        virtual void    ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        std::wstring    m_PathsSeparator;   // Separator to use between multiple paths.
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

        virtual void    ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* const p_pPluginProvider) const override;
        virtual void    ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        std::wstring    m_Executable;       // Executable to launch.
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

        virtual void    ModifyOptions(PipelineOptions& p_rOptions) const override;
    };

} // namespace PCC
