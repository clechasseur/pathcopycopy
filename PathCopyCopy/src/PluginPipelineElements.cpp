// PluginPipelineElements.cpp
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
#include <PluginPipelineElements.h>
#include <PipelinePlugin.h>
#include <Plugin.h>
#include <PluginUtils.h>
#include <StringUtils.h>

#include <algorithm>
#include <assert.h>


namespace PCC
{
    //
    // Modifies the given path by following the symlink if it
    // points to one.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void FollowSymlinkPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                  const PluginProvider* const /*p_pPluginProvider*/) const
    {
        PluginUtils::FollowSymlinkIfRequired(p_rPath);
    }

    //
    // Modifies the given path by surrounding it with quotes.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void QuotesPipelineElement::ModifyPath(std::wstring& p_rPath,
                                           const PluginProvider* const /*p_pPluginProvider*/) const
    {
        p_rPath.insert(p_rPath.begin(), 1, L'\"');
        p_rPath.append(1, L'\"');
    }

    //
    // Modifies the given path by surrounding it with quotes if the
    // path contains spaces.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void OptionalQuotesPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                   const PluginProvider* const /*p_pPluginProvider*/) const
    {
        if (p_rPath.find(' ') != std::wstring::npos) {
            p_rPath.insert(p_rPath.begin(), 1, L'\"');
            p_rPath.append(1, L'\"');
        }
    }

    //
    // Modifies the given path by turning it into an e-mail link.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void EmailLinksPipelineElement::ModifyPath(std::wstring& p_rPath,
                                               const PluginProvider* const /*p_pPluginProvider*/) const
    {
        p_rPath.insert(p_rPath.begin(), 1, L'<');
        p_rPath.append(1, L'>');
    }

    //
    // Modifies the given path by encoding URI whitespace.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void EncodeURIWhitespacePipelineElement::ModifyPath(std::wstring& p_rPath,
                                                        const PluginProvider* const /*p_pPluginProvider*/) const
    {
        StringUtils::EncodeURICharacters(p_rPath, StringUtils::EncodeParam::Whitespace);
    }

    //
    // Modifies the given path by encoding invalid URI characters.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void EncodeURICharsPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                   const PluginProvider* const /*p_pPluginProvider*/) const
    {
        StringUtils::EncodeURICharacters(p_rPath, StringUtils::EncodeParam::All);
    }

    //
    // Modifies the given path by replacing all backslashes by forward slashes.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void BackToForwardSlashesPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                         const PluginProvider* const /*p_pPluginProvider*/) const
    {
        std::replace(p_rPath.begin(), p_rPath.end(), L'\\', L'/');
    }

    //
    // Modifies the given path by replacing all forward slashes by backslashes.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void ForwardToBackslashesPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                         const PluginProvider* const /*p_pPluginProvider*/) const
    {
        std::replace(p_rPath.begin(), p_rPath.end(), L'/', L'\\');
    }

    //
    // Modified our path by removing any file extension at the end of it.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void RemoveFileExtPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                  const PluginProvider* const /*p_pPluginProvider*/) const
    {
        const std::wregex extRegex(L"^(.*[^\\\\/])(?:\\.[^\\\\/.]+)$", std::regex_constants::ECMAScript);
        p_rPath = std::regex_replace(p_rPath, extRegex, L"$1");
    }

    //
    // Constructor.
    //
    // @param p_OldValue Value to replace.
    // @param p_NewValue Replacement value.
    //
    FindReplacePipelineElement::FindReplacePipelineElement(const std::wstring& p_OldValue,
                                                           const std::wstring& p_NewValue)
        : PipelineElement(),
          m_OldValue(p_OldValue),
          m_NewValue(p_NewValue)
    {
    }

    //
    // Modifies the given path by replacing all instances of our old value
    // with our new value.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void FindReplacePipelineElement::ModifyPath(std::wstring& p_rPath,
                                                const PluginProvider* const /*p_pPluginProvider*/) const
    {
        if (!m_OldValue.empty()) {
            StringUtils::ReplaceAll(p_rPath, m_OldValue, m_NewValue);
        }
    }

    //
    // Constructor.
    //
    // @param p_Regex Regular expression to use to find matches.
    // @param p_Format Format of replacement string.
    // @param p_IgnoreCase Whether to ignore case when looking for matches.
    //
    RegexPipelineElement::RegexPipelineElement(const std::wstring& p_Regex,
                                               const std::wstring& p_Format,
                                               const bool p_IgnoreCase)
        : PipelineElement(),
          m_Regex(p_Regex),
          m_Format(p_Format),
          m_IgnoreCase(p_IgnoreCase),
          m_upRegex(),
          m_RegexTested(false)
    {
    }

    //
    // Modifies the given path by looking for matches using our regular
    // expression and replacing them using our format string.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void RegexPipelineElement::ModifyPath(std::wstring& p_rPath,
                                          const PluginProvider* const /*p_pPluginProvider*/) const
    {
        // Check if regex is valid.
        InitRegex();
        if (m_upRegex != nullptr) {
            try {
                // Perform the find-replace and return the modified string.
                p_rPath = std::regex_replace(p_rPath, *m_upRegex, m_Format);
            } catch (const std::regex_error&) {
                // Nothing much we can do, we didn't get this at init time...
                // Probably a problem with the replacement expression.
                // Leave path as-is.
            }
        }
    }

    //
    // Checks if a plugin using this pipeline element should be enabled or not.
    // In our case, we see if our regex is valid.
    //
    // @param p_ParentPath Path of the parent folder for the file to check; unused.
    // @param p_File Path of file to use for the check; unused.
    // @param p_psspAllPlugins Optional set containing all plugins; unused.
    // @return false if regular expression is invalid.
    //
    bool RegexPipelineElement::ShouldBeEnabledFor(const std::wstring& /*p_ParentPath*/,
                                                  const std::wstring& /*p_File*/,
                                                  const PluginProvider* const /*p_pPluginProvider*/) const
    {
        InitRegex();
        return m_upRegex != nullptr;
    }

    //
    // Initializes the m_apRegex member using the other members.
    // Call this method before needing to access the regex object.
    //
    // Note: m_apRegex will remain null if the regular expression is invalid.
    //
    void RegexPipelineElement::InitRegex() const
    {
        // Only init once.
        if (!m_RegexTested) {
            // Try creating regex. Keep null if the regex is invalid.
            try {
                if (!m_Regex.empty()) {
#pragma warning(suppress: 26812)    // std::regex_constants::syntax_option_type could be enum class
                    std::regex_constants::syntax_option_type reOptions = std::regex_constants::ECMAScript;
                    if (m_IgnoreCase) {
                        reOptions |= std::regex_constants::icase;
                    }
                    m_upRegex = std::make_unique<std::wregex>(m_Regex, reOptions);
                }
            } catch (const std::regex_error&) {
                assert(m_upRegex == nullptr);
            }
            m_RegexTested = true;
        }
    }

    //
    // Modifies the given path by replacing certain parts of the
    // path by environment variable references. See
    // https://docs.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathunexpandenvstringsw
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void UnexpandEnvironmentStringsPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                               const PluginProvider* const /*p_pPluginProvider*/) const
    {
        std::wstring unexpandedPath(MAX_PATH + 1, L'\0');
        if (::PathUnExpandEnvStringsW(p_rPath.c_str(), &*unexpandedPath.begin(), MAX_PATH + 1)) {
            p_rPath = unexpandedPath.c_str();
        }
    }

    //
    // Constructor.
    //
    // @param p_PluginId ID of plugin to apply.
    //
    ApplyPluginPipelineElement::ApplyPluginPipelineElement(const GUID& p_PluginId) noexcept
        : PipelineElement(),
          m_PluginId(p_PluginId)
    {
    }

    //
    // Modifies the given path by fetching a reference to the plugin we need
    // to apply and call its GetPath method on our path.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void ApplyPluginPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                const PluginProvider* const p_pPluginProvider) const
    {
        if (p_pPluginProvider != nullptr) {
            // Try finding the plugin we need.
            PluginSP spPlugin = p_pPluginProvider->GetPlugin(m_PluginId);
            if (spPlugin != nullptr) {
                // Success, call the plugin's GetPath method.
                p_rPath = spPlugin->GetPath(p_rPath);
            }
        }
    }

    //
    // Checks if a plugin using this pipeline element should be enabled or not.
    // In our case, we see if our plugin complains.
    //
    // @param p_ParentPath Path of the parent folder for the file to check.
    // @param p_File Path of file to use for the check.
    // @param p_pPluginProvider Optional object to access plugins.
    // @return false if pipeline says plugin should be disabled for this path.
    //
    bool ApplyPluginPipelineElement::ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                                        const std::wstring& p_File,
                                                        const PluginProvider* const p_pPluginProvider) const
    {
        bool enabled = false;
        if (p_pPluginProvider != nullptr) {
            // Try finding the plugin we need.
            PluginSP spPlugin = p_pPluginProvider->GetPlugin(m_PluginId);
            if (spPlugin != nullptr) {
                // Success, call the plugin's Enabled method.
                enabled = spPlugin->Enabled(p_ParentPath, p_File);
            }
        }
        return enabled;
    }

    //
    // Constructor.
    //
    // @param p_PluginId ID of pipeline plugin to apply.
    //
    ApplyPipelinePluginPipelineElement::ApplyPipelinePluginPipelineElement(const GUID& p_PluginId) noexcept
        : ApplyPluginPipelineElement(p_PluginId)
    {
    }

    //
    // Validates this pipeline element. In order to be valid,
    // it must not include a loop of pipeline elements.
    //
    // @param p_pPluginProvider Plugin provider used to fetch plugins.
    // @param p_rsSeenPluginIds Set used to store seen plugin IDs. Any
    //                          collision means a loop is detected and
    //                          pipeline element is invalid.
    //
    void ApplyPipelinePluginPipelineElement::Validate(const PluginProvider* const p_pPluginProvider,
                                                      GUIDS& p_rsSeenPluginIds) const
    {
        if (p_pPluginProvider == nullptr) {
            throw InvalidPipelineException();
        }

        // Try finding the plugin we need.
        const auto spPlugin = p_pPluginProvider->GetPlugin(m_PluginId);
        if (spPlugin == nullptr) {
            throw InvalidPipelineException();
        }

        // To be valid, plugin either has to not be a pipeline plugin
        // OR it needs to be one and have a valid pipeline.
        const auto* const pPipelinePlugin = dynamic_cast<PCC::Plugins::PipelinePlugin*>(spPlugin.get());
        if (pPipelinePlugin != nullptr && pPipelinePlugin->GetPipeline(&p_rsSeenPluginIds) == nullptr) {
            throw InvalidPipelineException(pPipelinePlugin->GetPipelineError().c_str());
        }
    }

    //
    // Constructor.
    //
    // @param p_PathsSeparator Separator to use between multiple paths.
    //
    PathsSeparatorPipelineElement::PathsSeparatorPipelineElement(const std::wstring& p_PathsSeparator)
        : PipelineElement(),
          m_PathsSeparator(p_PathsSeparator)
    {
    }

    //
    // Does not modify the path since this element only modifies pipeline options.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void PathsSeparatorPipelineElement::ModifyPath(std::wstring& /*p_rPath*/,
                                                   const PluginProvider* const /*p_pPluginProvider*/) const noexcept(false)
    {
    }

    //
    // Modifies global pipeline options by specifying the separator to use
    // between multiple copied paths.
    //
    // @param p_rOptions Global options to modify (in-place).
    //
    void PathsSeparatorPipelineElement::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        p_rOptions.SetPathsSeparator(m_PathsSeparator);
    }

    //
    // Constructor.
    //
    // @param p_Executable Path to executable to launch.
    //
    ExecutablePipelineElement::ExecutablePipelineElement(const std::wstring& p_Executable)
        : PipelineElement(),
          m_Executable(p_Executable)
    {
    }

    //
    // Does not modify the path since this element only modifies pipeline options.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void ExecutablePipelineElement::ModifyPath(std::wstring& /*p_rPath*/,
                                               const PluginProvider* const /*p_pPluginProvider*/) const noexcept(false)
    {
    }

    //
    // Modifies global pipeline options by specifying the path of the
    // executable to launch with paths as argument.
    //
    // @param p_rOptions Global options to modify (in-place).
    //
    void ExecutablePipelineElement::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        p_rOptions.SetExecutable(m_Executable);
    }

    //
    // Constructor.
    //
    // @param p_Executable Path to executable to launch.
    //
    ExecutableWithFilelistPipelineElement::ExecutableWithFilelistPipelineElement(const std::wstring& p_Executable)
        : ExecutablePipelineElement(p_Executable)
    {
    }

    //
    // Modifies global pipeline options by specifying the path of the
    // executable to launch, as well as specifying to launch it with
    // filelist instead of paths directly.
    //
    // @param p_rOptions Global options to modify (in-place).
    //
    void ExecutableWithFilelistPipelineElement::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        ExecutablePipelineElement::ModifyOptions(p_rOptions);
        p_rOptions.SetUseFilelist(true);
    }

} // namespace PCC
