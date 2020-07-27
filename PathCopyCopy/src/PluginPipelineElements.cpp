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
#include <sstream>

#include <coveo/linq.h>


namespace
{
    const wchar_t* const    DRIVE_LABEL_IDENTIFIER  = L"%DRIVELABEL%";  // Identifier to replace with drive label

} // anonymous namespace

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
    // Modifies the given path by replacing all instances of %DRIVELABEL%
    // with the label of the current drive.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void InjectDriveLabelPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                     const PluginProvider* const /*p_pPluginProvider*/) const
    {
        using namespace coveo::linq;

        // Get drive letter for path.
        const auto vParents = PluginUtils::EnumerateParents(p_rPath);
        std::wstring drive;
        if (!vParents.empty()) {
            drive = vParents.back();
        }
        if (!drive.empty()) {
            if (drive.back() != L'\\') {
                drive += L'\\';
            }

            // Attempt to fetch volume info for drive, including its label.
            std::wstring volumeLabel(MAX_PATH + 1, L'\0');
            const auto gotInfo = ::GetVolumeInformationW(drive.c_str(),
                                                         &*volumeLabel.begin(),
                                                         MAX_PATH + 1,
                                                         nullptr,
                                                         nullptr,
                                                         nullptr,
                                                         nullptr,
                                                         0);
            if (gotInfo) {
                StringUtils::ReplaceAll(p_rPath, DRIVE_LABEL_IDENTIFIER, volumeLabel.c_str());
            }
        }
    }

    //
    // Constructor.
    //
    // @param p_NumParts Number of path parts to copy.
    // @param p_First Whether to copy the first (true) or last (false) path parts.
    //
    CopyNPathPartsPipelineElement::CopyNPathPartsPipelineElement(const size_t p_NumParts,
                                                                 const bool p_First)
        : m_NumParts(p_NumParts),
          m_First(p_First)
    {
    }

    //
    // Modifies the given path by keeping only some parts of the path.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void CopyNPathPartsPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                   const PluginProvider* const /*p_pPluginProvider*/) const
    {
        // First split the path into parts.
        auto vPathParts = StringUtils::Split(p_rPath, L"\\/");
        
        // If we have less parts than requested just return the path unmodified.
        if (m_NumParts < vPathParts.size()) {
            // Try auto-detecting the separator type used in this path.
            const auto separatorPos = p_rPath.find_first_of(L"\\/");
            const auto separator = separatorPos != std::wstring::npos ? p_rPath[separatorPos] : L'\\';

            // Keep only the required number of path parts and join them using separator.
            if (m_First) {
                vPathParts.erase(vPathParts.end() - (vPathParts.size() - m_NumParts), vPathParts.end());
            } else {
                vPathParts.erase(vPathParts.begin(), vPathParts.begin() + (vPathParts.size() - m_NumParts));
            }
            std::wstringstream wss;
            for (auto&& part : vPathParts) {
                if (wss.tellp() != 0) {
                    wss << separator;
                }
                wss << part;
            }
            p_rPath = wss.str();
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
    // Validates this pipeline element. In order to be valid,
    // the plugin to apply must exist.
    //
    // @param p_pPluginProvider Plugin provider used to fetch plugins.
    // @param p_rsSeenPluginIds Set used to store seen plugin IDs. Any
    //                          collision means a loop is detected and
    //                          pipeline element is invalid.
    //
    void ApplyPluginPipelineElement::Validate(const PluginProvider* const p_pPluginProvider,
                                              GUIDS& p_rsSeenPluginIds) const
    {
        PipelineElement::Validate(p_pPluginProvider, p_rsSeenPluginIds);

        if (p_pPluginProvider == nullptr) {
            throw InvalidPipelineException();
        }
        if (p_pPluginProvider->GetPlugin(m_PluginId) == nullptr) {
            throw InvalidPipelineException(ATL::CStringA(MAKEINTRESOURCEA(IDS_INVALIDPIPELINE_BASE_COMMAND_NOT_FOUND)));
        }
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
    [[gsl::suppress(f.23)]]
    void ApplyPipelinePluginPipelineElement::Validate(const PluginProvider* const p_pPluginProvider,
                                                      GUIDS& p_rsSeenPluginIds) const
    {
        ApplyPluginPipelineElement::Validate(p_pPluginProvider, p_rsSeenPluginIds);

        // To be valid, plugin either has to not be a pipeline plugin
        // OR it needs to have a valid pipeline.
        const auto spPlugin = p_pPluginProvider->GetPlugin(m_PluginId);
        const auto* const pPipelinePlugin = dynamic_cast<PCC::Plugins::PipelinePlugin*>(spPlugin.get());
        if (pPipelinePlugin != nullptr && pPipelinePlugin->GetPipeline(&p_rsSeenPluginIds) == nullptr) {
            throw InvalidPipelineException(pPipelinePlugin->GetPipelineError().c_str());
        }
    }

    //
    // Default constructor. The element will push the entire path
    // to the stack.
    //
    PushToStackPipelineElement::PushToStackPipelineElement()
        : m_Method(PushToStackMethod::Entire)
    {
    }

    //
    // Constructor for element that pushes a range in the path to
    // the stack, as defined by beginning and end indexes.
    //
    // @param p_Begin Index of start of range to push to the stack (inclusive).
    // @param p_End Index of end of range to push to the stack (exclusive).
    //
    PushToStackPipelineElement::PushToStackPipelineElement(const size_t p_Begin,
                                                           const size_t p_End)
        : m_Method(PushToStackMethod::Range),
          m_Begin(p_Begin),
          m_End(p_End)
    {
    }

    //
    // Constructor for element that uses a regex to find the part
    // of the path to push to the stack.
    //
    // @param p_Regex Regex to use to locate the part of the path to push to the stack.
    // @param p_IgnoreCase Whether to use case-sensitive regex or not.
    // @param p_Group Index of group to push to the stack. Use 0 for entire match.
    //
    PushToStackPipelineElement::PushToStackPipelineElement(const std::wstring& p_Regex,
                                                           const bool p_IgnoreCase,
                                                           const size_t p_Group)
        : m_Method(PushToStackMethod::Regex),
          m_Regex(p_Regex),
          m_IgnoreCase(p_IgnoreCase),
          m_Group(p_Group)
    {
    }

    //
    // Constructor for element that pushes a fixed string to the
    // stack instead of part of the path.
    //
    // @param p_FixedString Fixed string to push to the stack.
    //
    PushToStackPipelineElement::PushToStackPipelineElement(const std::wstring& p_FixedString)
        : m_Method(PushToStackMethod::Fixed),
          m_FixedString(p_FixedString)
    {
    }

    //
    // Locates the part of the path requested and pushes it to the stack.
    // If nothing is found, an empty string will be pushed.
    //
    // @param p_rPath Path to modify (in-place). Not actually modified.
    // @param p_rStack Stack where to push part of the path.
    // @param p_pPluginProvider Optional object to access plugins; unused.
    //
    void PushToStackPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                std::stack<std::wstring>& p_rStack,
                                                const PluginProvider* const /*p_pPluginProvider*/) const
    {
        p_rStack.emplace(PartToPush(p_rPath));
    }

    //
    // Given a path, returns the part to push to the stack.
    //
    // @param p_Path Path to locate part of.
    // @return Part of p_Path to push to the stack.
    //
    std::wstring PushToStackPipelineElement::PartToPush(const std::wstring& p_Path) const
    {
        std::wstring part;
        
        switch (m_Method) {
            case PushToStackMethod::Entire: {
                // Push the entire path.
                part = p_Path;
                break;
            }
            case PushToStackMethod::Range: {
                // Push a range in the path.
                if (m_Begin < p_Path.size() && m_Begin < m_End) {
                    part = p_Path.substr(m_Begin, m_End - m_Begin);
                }
                break;
            }
            case PushToStackMethod::Regex: {
                // Push the first match for a regex.
                try {
                    if (!m_Regex.empty()) {
#pragma warning(suppress: 26812)    // std::regex_constants::syntax_option_type could be enum class
                        std::regex_constants::syntax_option_type reOptions = std::regex_constants::ECMAScript;
                        if (m_IgnoreCase) {
                            reOptions |= std::regex_constants::icase;
                        }
                        std::wregex regex(m_Regex, reOptions);

                        std::wsmatch match;
                        if (std::regex_search(p_Path, match, regex)) {
                            part = match[m_Group];
                        }
                    }
                } catch (const std::regex_error&) {
                }
                break;
            }
            case PushToStackMethod::Fixed: {
                part = m_FixedString;
                break;
            }
            default:
                assert(false);
        }

        return part;
    }

    //
    // Constructor with pop location and no other information.
    //
    // @param p_Location Location where to store popped value.
    //
    PopFromStackPipelineElement::PopFromStackPipelineElement(const PopFromStackLocation p_Location)
        : m_Location(p_Location)
    {
    }

    //
    // Constructor for an element that replaces a range in the path
    // with the popped value.
    //
    // @param p_Begin Start of range to replace with the popped value.
    // @param p_End End of range to replace with the popped value.
    //
    PopFromStackPipelineElement::PopFromStackPipelineElement(const size_t p_Begin,
                                                             const size_t p_End)
        : m_Location(PopFromStackLocation::Range),
          m_Begin(p_Begin),
          m_End(p_End)
    {
    }

    //
    // Constructor for an element that uses a regex to replace
    // part of the path with the popped value.
    //
    // @param p_Regex Regex to use to find the part to replace.
    // @param p_IgnoreCase Whether to ignore case when using the regex.
    //
    PopFromStackPipelineElement::PopFromStackPipelineElement(const std::wstring& p_Regex,
                                                             const bool p_IgnoreCase)
        : m_Location(PopFromStackLocation::Regex),
          m_Regex(p_Regex),
          m_IgnoreCase(p_IgnoreCase)
    {
    }

    //
    // Attempts to pop a value from the stack. If it works, optionally stores
    // it in the path by replacing the entire path or part of it with the
    // popped value.
    //
    // @param p_rPath Path to modify (in-place).
    // @param p_rStack Stack from which to pop the value.
    // @param p_pPluginProvider Optional object to access plugins; unused.
    //
    void PopFromStackPipelineElement::ModifyPath(std::wstring& p_rPath,
                                                 std::stack<std::wstring>& p_rStack,
                                                 const PluginProvider* const /*p_pPluginProvider*/) const
    {
        if (!p_rStack.empty()) {
            const auto value = p_rStack.top();
            p_rStack.pop();

            switch (m_Location) {
                case PopFromStackLocation::Entire: {
                    // Replace the entire path with the value.
                    p_rPath = value;
                    break;
                }
                case PopFromStackLocation::Range: {
                    // Replace a range in the path with the value.
                    if (m_Begin < p_rPath.size() && m_Begin <= m_End) {
                        p_rPath.replace(m_Begin, m_End - m_Begin, value);
                    }
                    break;
                }
                case PopFromStackLocation::Regex: {
                    // Replace regex match with the value.
                    try {
                        if (!m_Regex.empty()) {
#pragma warning(suppress: 26812)    // std::regex_constants::syntax_option_type could be enum class
                            std::regex_constants::syntax_option_type reOptions = std::regex_constants::ECMAScript;
                            if (m_IgnoreCase) {
                                reOptions |= std::regex_constants::icase;
                            }
                            std::wregex regex(m_Regex, reOptions);

                            // We can't use std::regex_replace because if the value contains
                            // special characters it will get weird. So use regex_search and
                            // do the replacement by hand.
                            std::wsmatch match;
                            if (std::regex_search(p_rPath, match, regex)) {
                                p_rPath = match.prefix().str() + value + match.suffix().str();
                            }
                        }
                    } catch (const std::regex_error&) {
                    }
                    break;
                }
                case PopFromStackLocation::Start: {
                    // Insert value at start of path.
                    p_rPath = value + p_rPath;
                    break;
                }
                case PopFromStackLocation::End: {
                    // Insert value at end of path.
                    p_rPath += value;
                    break;
                }
                case PopFromStackLocation::Nowhere: {
                    // Simply drop the value.
                    break;
                }
                default:
                    assert(false);
            }
        }
    }

    //
    // Attempts to pop two values from the stack. If it works, pushes
    // them back in reverse order.
    //
    // @param p_rPath Path to modify; unused.
    // @param p_rStack Stack from which to pop the values.
    // @param p_pPluginProvider Optional object to access plugins; unused.
    //
    void SwapStackValuesPipelineElement::ModifyPath(std::wstring& /*p_rPath*/,
                                                    std::stack<std::wstring>& p_rStack,
                                                    const PluginProvider* const /*p_pPluginProvider*/) const
    {
        if (p_rStack.size() >= 2) {
            const auto value1 = p_rStack.top();
            p_rStack.pop();
            const auto value2 = p_rStack.top();
            p_rStack.pop();
            p_rStack.push(value1);
            p_rStack.push(value2);
        }
    }

    //
    // Attempts to pop a value from the stack. If it works, pushes
    // back two copies of the value.
    //
    // @param p_rPath Path to modify; unused.
    // @param p_rStack Stack from which to pop the value.
    // @param p_pPluginProvider Optional object to access plugins; unused.
    //
    void DuplicateStackValuePipelineElement::ModifyPath(std::wstring& /*p_rPath*/,
                                                        std::stack<std::wstring>& p_rStack,
                                                        const PluginProvider* const /*p_pPluginProvider*/) const
    {
        if (!p_rStack.empty()) {
            p_rStack.push(p_rStack.top());
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
    // Modify global pipeline options by specifying that paths should
    // be copied recursively.
    //
    // @param p_rOptions Global options to modify (in-place).
    //
    void RecursiveCopyPipelineElement::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        p_rOptions.SetCopyPathsRecursively(true);
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

    //
    // Constructor.
    //
    // @param p_Executable Path to the executable to launch.
    // @param p_Arguments Arguments to pass to the executable. Can contain the
    //                    %FILES% placeholder to replace with the files.
    // @param p_UseFilelist Whether to use a filelist instead of passing paths
    //                      directly to the executable.
    //
    CommandLinePipelineElement::CommandLinePipelineElement(const std::wstring& p_Executable,
                                                           const std::wstring& p_Arguments,
                                                           const bool p_UseFilelist)
        : ExecutablePipelineElement(p_Executable),
          m_Arguments(p_Arguments),
          m_UseFilelist(p_UseFilelist)
    {
    }

    //
    // Modifies the global pipeline options by specifying to launch
    // an executable with optional arguments as well as use of a filelist.
    //
    // @param p_rOptions Global options to modify.
    //
    void CommandLinePipelineElement::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        ExecutablePipelineElement::ModifyOptions(p_rOptions);
        p_rOptions.SetArguments(m_Arguments);
        p_rOptions.SetUseFilelist(m_UseFilelist);
    }

} // namespace PCC
