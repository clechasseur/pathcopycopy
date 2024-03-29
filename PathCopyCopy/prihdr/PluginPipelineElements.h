// PluginPipelineElements.h
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

#include <PluginPipeline.h>

#include <memory>
#include <regex>
#include <string>

#include <windows.h>


namespace PCC
{
    //
    // PushToStackMethod
    //
    // Enum of the various ways PushToStackPipelineElement
    // can find the part of the path to push to the stack.
    //
    enum class PushToStackMethod : long
    {
        Entire  = 1,    // Push the entire path
        Range   = 2,    // Push a range as defined by bounds
        Regex   = 3,    // Push the first part that matches a given regex
        Fixed   = 4,    // Push a fixed string instead of part of the path
    };

    //
    // PopFromStackLocation
    //
    // Enum of the possible locations where to store a value
    // popped from the stack by PopFromStackPipelineElement.
    //
    enum class PopFromStackLocation : long
    {
        Entire  = 1,    // Replace entire path with the popped value
        Range   = 2,    // Replace a range in the path with the popped value
        Regex   = 3,    // Replace a regex match with the popped value
        Start   = 4,    // Insert popped value at start of path
        End     = 5,    // Insert popped value at end of path
        Nowhere = 6,    // Simply pop and drop the value
    };

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
    // InjectDriveLabelPipelineElement
    //
    // Pipeline element that replaces %DRIVELABEL% with the
    // label of the current drive.
    //
    class InjectDriveLabelPipelineElement : public PipelineElement
    {
    public:
                        InjectDriveLabelPipelineElement() = default;
                        InjectDriveLabelPipelineElement(const InjectDriveLabelPipelineElement&) = delete;
        InjectDriveLabelPipelineElement&
                        operator=(const InjectDriveLabelPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // CopyNPathPartsPipelineElement
    //
    // Pipeline element that copies the N first or last parts of the path.
    //
    class CopyNPathPartsPipelineElement : public PipelineElement
    {
    public:
                        CopyNPathPartsPipelineElement(size_t p_NumParts,
                                                      bool p_First);
                        CopyNPathPartsPipelineElement(const CopyNPathPartsPipelineElement&) = delete;
        CopyNPathPartsPipelineElement&
                        operator=(const CopyNPathPartsPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   const PluginProvider* p_pPluginProvider) const override;

    private:
        const size_t    m_NumParts;     // Number of path parts to copy.
        const bool      m_First;        // Whether to copy the first (true) or last (false) path parts.
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

        void            Validate(const PluginProvider* p_pPluginProvider,
                                 GUIDS& p_rsSeenPluginIds) const override;

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

        void            Validate(const PluginProvider* p_pPluginProvider,
                                 GUIDS& p_rsSeenPluginIds) const override;
    };

    //
    // PushToStackPipelineElement
    //
    // Pipeline element that pushes part of the path (possibly the entire thing)
    // to the stack. Can use different methods to identify the part of the path
    // to push to the stack, like regex.
    //
    class PushToStackPipelineElement : public PipelineElement
    {
    public:
                        PushToStackPipelineElement();
                        PushToStackPipelineElement(size_t p_Begin,
                                                   size_t p_End);
                        PushToStackPipelineElement(const std::wstring& p_Regex,
                                                   bool p_IgnoreCase,
                                                   size_t p_Group);
        explicit        PushToStackPipelineElement(const std::wstring& p_FixedString);
                        PushToStackPipelineElement(const PushToStackPipelineElement&) = delete;
        PushToStackPipelineElement&
                        operator=(const PushToStackPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   std::stack<std::wstring>& p_rStack,
                                   const PluginProvider* p_pPluginProvider) const override;

    private:
        const PushToStackMethod
                        m_Method;               // How to find the part of the path to push.
        const size_t    m_Begin = 0;            // Start of the range to push to the stack (if m_Method is Range).
        const size_t    m_End = 0;              // End of the range to push to the stack (if m_Method is Range).
        const std::wstring
                        m_Regex{};              // Regex to use to find the part of the path to push (if m_Method is Regex).
        const bool      m_IgnoreCase = false;   // Whether to ignore case in regex (if m_Method is Regex).
        const size_t    m_Group = 0;            // Index of regex group to push to the stack (if m_Method is Regex).
        const std::wstring
                        m_FixedString{};        // Fixed string to push to the stack (if m_Method is Fixed).

        std::wstring    PartToPush(const std::wstring& p_Path) const;
    };

    //
    // PopFromStackPipelineElement
    //
    // Pipeline element that pops a value from the stack and stores it
    // somewhere in the path. Can store value in different locations,
    // including nowhere or replacing the entire path.
    //
    // If the stack is empty when invoked, nothing happens.
    //
    class PopFromStackPipelineElement : public PipelineElement
    {
    public:
        explicit        PopFromStackPipelineElement(PopFromStackLocation p_Location);
                        PopFromStackPipelineElement(size_t m_Begin,
                                                    size_t m_End);
                        PopFromStackPipelineElement(const std::wstring& p_Regex,
                                                    bool p_IgnoreCase);
                        PopFromStackPipelineElement(const PopFromStackPipelineElement&) = delete;
        PopFromStackPipelineElement&
                        operator=(const PopFromStackPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   std::stack<std::wstring>& p_rStack,
                                   const PluginProvider* p_pPluginProvider) const override;

    private:
        const PopFromStackLocation
                        m_Location;             // Where to store the popped value.
        const size_t    m_Begin = 0;            // Start of range to replace with the popped value (if m_Location is Range).
        const size_t    m_End = 0;              // End of range to replace with the popped value (if m_Location is Range).
        const std::wstring
                        m_Regex{};              // Regex to replace with the popped value (if m_Location is Regex).
        const bool      m_IgnoreCase = false;   // Whether to ignore case in regex (if m_Location is Regex).
    };

    //
    // SwapStackValuesPipelineElement
    //
    // Pipeline element that pops two values from the stack and swaps them.
    //
    // If the stack contains less than two values, nothing happens.
    //
    class SwapStackValuesPipelineElement : public PipelineElement
    {
    public:
                        SwapStackValuesPipelineElement() = default;
                        SwapStackValuesPipelineElement(const SwapStackValuesPipelineElement&) = delete;
        SwapStackValuesPipelineElement&
                        operator=(const SwapStackValuesPipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   std::stack<std::wstring>& p_rStack,
                                   const PluginProvider* p_pPluginProvider) const override;
    };

    //
    // DuplicateStackValuePipelineElement
    //
    // Pipeline element that duplicates the top value on the stack.
    //
    // If the stack is empty, nothing happens.
    //
    class DuplicateStackValuePipelineElement : public PipelineElement
    {
    public:
                        DuplicateStackValuePipelineElement() = default;
                        DuplicateStackValuePipelineElement(const DuplicateStackValuePipelineElement&) = delete;
        DuplicateStackValuePipelineElement&
                        operator=(const DuplicateStackValuePipelineElement&) = delete;

        void            ModifyPath(std::wstring& p_rPath,
                                   std::stack<std::wstring>& p_rStack,
                                   const PluginProvider* p_pPluginProvider) const override;
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
    // RecursiveCopyPipelineElement
    //
    // Pipeline element that does not modify the path but instructs
    // Path Copy Copy to copy paths recursively.
    //
    class RecursiveCopyPipelineElement : public PipelineElement
    {
    public:
                        RecursiveCopyPipelineElement() = default;
                        RecursiveCopyPipelineElement(const RecursiveCopyPipelineElement&) = delete;
        RecursiveCopyPipelineElement&
                        operator=(const RecursiveCopyPipelineElement&) = delete;

        void            ModifyOptions(PipelineOptions& p_rOptions) const override;
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

    //
    // CommandLinePipelineElement
    //
    // Pipeline element that instructs Path Copy Copy to launch an executable
    // instead of copying paths to the clipboard, additionally providing
    // optional arguments to pass to the executable. Can also instruct
    // Path Copy Copy to use a filelist instead of paths directly.
    //
    class CommandLinePipelineElement : public ExecutablePipelineElement
    {
    public:
                        CommandLinePipelineElement(const std::wstring& p_Executable,
                                                   const std::wstring& p_Arguments,
                                                   bool p_UseFilelist);
                        CommandLinePipelineElement(const CommandLinePipelineElement&) = delete;
        CommandLinePipelineElement&
                        operator=(const CommandLinePipelineElement&) = delete;

        void            ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        const std::wstring
                        m_Arguments;        // Arguments to pass to executable.
        const bool      m_UseFilelist;      // Whether to pass arguments in a filelist.
    };

    //
    // DisplayForSelectionPipelineElement
    //
    // Pipeline element that does not modify the path but specifies whether
    // the plugin should be displayed when files and/or folders are selected.
    //
    class DisplayForSelectionPipelineElement : public PipelineElement
    {
    public:
                        DisplayForSelectionPipelineElement(bool p_ShowForFiles,
                                                           bool p_ShowForFolders);
                        DisplayForSelectionPipelineElement(const DisplayForSelectionPipelineElement&) = delete;
        DisplayForSelectionPipelineElement&
                        operator=(const DisplayForSelectionPipelineElement&) = delete;

        void            ModifyOptions(PipelineOptions& p_rOptions) const override;

    private:
        bool            m_ShowForFiles;     // Whether the plugin should be displayed when files are selected.
        bool            m_ShowForFolders;   // Whether the plugin should be displayed when folders are selected.
    };

} // namespace PCC
