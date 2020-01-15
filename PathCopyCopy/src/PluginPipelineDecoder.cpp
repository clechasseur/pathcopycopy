// PluginPipelineDecoder.cpp
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
#include <PluginPipelineDecoder.h>
#include <PluginPipelineElements.h>

#include <assert.h>
#include <sstream>


namespace
{
    // Each pipeline element type is represented using a single character
    // in the encoded string. This is a list of all possible values.
    constexpr wchar_t   ELEMENT_CODE_FOLLOW_SYMLINK             = L'k';
    constexpr wchar_t   ELEMENT_CODE_QUOTES                     = L'"';
    constexpr wchar_t   ELEMENT_CODE_OPTIONAL_QUOTES            = L'q';
    constexpr wchar_t   ELEMENT_CODE_EMAIL_LINKS                = L'<';
    constexpr wchar_t   ELEMENT_CODE_ENCODE_URI_WHITESPACE      = L's';
    constexpr wchar_t   ELEMENT_CODE_ENCODE_URI_CHARS           = L'%';
    constexpr wchar_t   ELEMENT_CODE_BACK_TO_FORWARD_SLASHES    = L'\\';
    constexpr wchar_t   ELEMENT_CODE_FORWARD_TO_BACKSLASHES     = L'/';
    constexpr wchar_t   ELEMENT_CODE_REMOVE_EXT                 = L'.';
    constexpr wchar_t   ELEMENT_CODE_FIND_REPLACE               = L'?';
    constexpr wchar_t   ELEMENT_CODE_REGEX                      = L'^';
    constexpr wchar_t   ELEMENT_CODE_UNEXPAND_ENV_STRINGS       = L'e';
    constexpr wchar_t   ELEMENT_CODE_APPLY_PLUGIN               = L'{';
    constexpr wchar_t   ELEMENT_CODE_APPLY_PIPELINE_PLUGIN      = L'}';
    constexpr wchar_t   ELEMENT_CODE_PATHS_SEPARATOR            = L',';
    constexpr wchar_t   ELEMENT_CODE_EXECUTABLE                 = L'x';
    constexpr wchar_t   ELEMENT_CODE_EXECUTABLE_WITH_FILELIST   = L'f';

    // Version numbers used for regex elements.
    constexpr long      REGEX_ELEMENT_INITIAL_VERSION           = 1;
    constexpr long      REGEX_ELEMENT_MAX_VERSION               = REGEX_ELEMENT_INITIAL_VERSION;

} // anonymous namespace

namespace PCC
{
    //
    // Decodes a series of elements that were encoded by the settings application
    // in a string and produces a list of corresponding pipeline element objects.
    //
    // @param p_EncodedElements Elements encoded in a string.
    // @return Vector of resulting elements.
    //
    auto PipelineDecoder::DecodePipeline(const std::wstring& p_EncodedElements) -> PipelineElementSPV
    {
        EncodedElementsStream stream(p_EncodedElements);
        const size_t numElements = stream.ReadElementCount();

        PipelineElementSPV vspPipelineElements;
        vspPipelineElements.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i) {
            vspPipelineElements.emplace_back(DecodePipelineElement(stream));
        }

        return vspPipelineElements;
    }

    //
    // Decodes a pipeline element found in an encoded pipeline elements stream.
    //
    // @param p_rStream Stream containing encoded pipeline element.
    // @return New pipeline element.
    //
    auto PipelineDecoder::DecodePipelineElement(PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        PipelineElementSP spElement;

        // The first character represents a code telling us what element type it is.
        const auto code = p_rStream.ReadData(1).front();

        // Look at the code and create the appropriate element.
        switch (code) {
            case ELEMENT_CODE_FOLLOW_SYMLINK: {
                spElement = std::make_shared<FollowSymlinkPipelineElement>();
                break;
            }
            case ELEMENT_CODE_QUOTES: {
                spElement = std::make_shared<QuotesPipelineElement>();
                break;
            }
            case ELEMENT_CODE_OPTIONAL_QUOTES: {
                spElement = std::make_shared<OptionalQuotesPipelineElement>();
                break;
            }
            case ELEMENT_CODE_EMAIL_LINKS: {
                spElement = std::make_shared<EmailLinksPipelineElement>();
                break;
            }
            case ELEMENT_CODE_ENCODE_URI_WHITESPACE: {
                spElement = std::make_shared<EncodeURIWhitespacePipelineElement>();
                break;
            }
            case ELEMENT_CODE_ENCODE_URI_CHARS: {
                spElement = std::make_shared<EncodeURICharsPipelineElement>();
                break;
            }
            case ELEMENT_CODE_BACK_TO_FORWARD_SLASHES: {
                spElement = std::make_shared<BackToForwardSlashesPipelineElement>();
                break;
            }
            case ELEMENT_CODE_FORWARD_TO_BACKSLASHES: {
                spElement = std::make_shared<ForwardToBackslashesPipelineElement>();
                break;
            }
            case ELEMENT_CODE_REMOVE_EXT: {
                spElement = std::make_shared<RemoveFileExtPipelineElement>();
                break;
            }
            case ELEMENT_CODE_FIND_REPLACE: {
                spElement = DecodeFindReplaceElement(p_rStream);
                break;
            }
            case ELEMENT_CODE_REGEX: {
                spElement = DecodeRegexElement(p_rStream);
                break;
            }
            case ELEMENT_CODE_UNEXPAND_ENV_STRINGS: {
                spElement = std::make_shared<UnexpandEnvironmentStringsPipelineElement>();
                break;
            }
            case ELEMENT_CODE_APPLY_PLUGIN:
            case ELEMENT_CODE_APPLY_PIPELINE_PLUGIN: {
                spElement = DecodeApplyPluginElement(code, p_rStream);
                break;
            }
            case ELEMENT_CODE_PATHS_SEPARATOR: {
                spElement = DecodePathsSeparatorElement(p_rStream);
                break;
            }
            case ELEMENT_CODE_EXECUTABLE:
            case ELEMENT_CODE_EXECUTABLE_WITH_FILELIST: {
                spElement = DecodeExecutableElement(code, p_rStream);
                break;
            }
            default:
                // Unknown element type, we can't add it and don't know
                // how to skip it. Possibly due to a downgrade of PCC?
                throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }

        // Add new element to the pipeline.
        if (spElement == nullptr) {
            throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }
        return spElement;
    }

    //
    // Decodes a FindReplacePipelineElement found in an encoded stream.
    //
    // @param p_rStream Stream containing encoded element.
    // @return Newly-created element.
    //
    auto PipelineDecoder::DecodeFindReplaceElement(PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        // This type of element contains an old and a new value.
        const auto oldValue = p_rStream.ReadString();
        const auto newValue = p_rStream.ReadString();
        return std::make_shared<FindReplacePipelineElement>(oldValue, newValue);
    }

    //
    // Decodes a RegexPipelineElement found in an encoded stream.
    //
    // @param p_rStream Stream containing encoded element.
    // @return Newly-created element.
    //
    auto PipelineDecoder::DecodeRegexElement(PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        // The data starts by a version number. This is used in case we need to add
        // support for extra options in the future.
        const auto version = p_rStream.ReadLong();

        // Make sure it's a version we can support.
        if (version > REGEX_ELEMENT_MAX_VERSION) {
            throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }

        // Initial version: regex, format string and whether we should ignore case.
        const auto regex = p_rStream.ReadString();
        const auto format = p_rStream.ReadString();
        const auto ignoreCase = p_rStream.ReadBool();

        // Create the element and return it.
        return std::make_shared<RegexPipelineElement>(regex, format, ignoreCase);
    }

    //
    // Decodes an ApplyPluginPipelineElement or ApplyPipelinePluginPipelineElement
    // found in an encoded stream.
    //
    // @param p_Code Element code.
    // @param p_rStream Stream containing encoded element.
    // @return Newly-created element.
    //
    auto PipelineDecoder::DecodeApplyPluginElement(const wchar_t p_Code,
                                                   PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        // The element data is a string representation of the GUID of the plugin to apply.
        // A guid has the following format: {1B4B1405-84CF-48CC-B373-42FAD7744258}
        // (Note: GUIDSTRING_MAX includes the terminating null)
        const auto guidString = p_rStream.ReadData(GUIDSTRING_MAX - 1);

        // Now that we have the data, convert it to a GUID.
        CLSID pluginGuid;
        if (FAILED(::CLSIDFromString(guidString.c_str(), &pluginGuid))) {
            // Invalid GUID format.
            throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }

        // We have the plugin GUID, return it.
        if (p_Code == ELEMENT_CODE_APPLY_PLUGIN) {
            return std::make_shared<ApplyPluginPipelineElement>(pluginGuid);
        } else if (p_Code == ELEMENT_CODE_APPLY_PIPELINE_PLUGIN) {
            return std::make_shared<ApplyPipelinePluginPipelineElement>(pluginGuid);
        } else {
            throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }
    }

    //
    // Decodes an PathsSeparatorPipelineElement found in an encoded stream.
    //
    // @param p_rStream Stream containing encoded element.
    // @return Newly-created element.
    //
    auto PipelineDecoder::DecodePathsSeparatorElement(PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        // This type of element contains only the paths separator string.
        const auto pathsSeparator = p_rStream.ReadString();
        return std::make_shared<PathsSeparatorPipelineElement>(pathsSeparator);
    }

    //
    // Decodes an ExecutablePipelineElement or ExecutableWithFilelistPipelineElement
    // found in an encoded stream.
    //
    // @param p_Code Element code.
    // @param p_rStream Stream containing encoded element.
    // @return Newly-created element.
    //
    auto PipelineDecoder::DecodeExecutableElement(const wchar_t p_Code,
                                                  PipelineDecoder::EncodedElementsStream& p_rStream) -> PipelineElementSP
    {
        // This type of element contains only a string containing the path to the executable.
        const auto executable = p_rStream.ReadString();
        if (p_Code == ELEMENT_CODE_EXECUTABLE) {
            return std::make_shared<ExecutablePipelineElement>(executable);
        } else if (p_Code == ELEMENT_CODE_EXECUTABLE_WITH_FILELIST) {
            return std::make_shared<ExecutableWithFilelistPipelineElement>(executable);
        } else {
            throw InvalidPipelineException(p_rStream.GetEncodedElements());
        }
    }

    //
    // Constructor.
    //
    // @param p_EncodedElements String containing encoded pipeline elements data.
    //
    PipelineDecoder::EncodedElementsStream::EncodedElementsStream(const std::wstring& p_EncodedElements)
        : m_EncodedElements(p_EncodedElements),
          m_CurIndex(0)
    {
    }

    //
    // Returns the string containing encoded elements backing this stream.
    //
    // @return String containing all encoded elements.
    //
    auto PipelineDecoder::EncodedElementsStream::GetEncodedElements() const noexcept -> const std::wstring&
    {
        return m_EncodedElements;
    }

    //
    // Reads a number of characters from the stream.
    //
    // @param p_DataSize Size of data to read, in number of characters.
    // @return String containing the data.
    //
    auto PipelineDecoder::EncodedElementsStream::ReadData(const std::wstring::size_type p_DataSize) -> std::wstring
    {
        if (m_EncodedElements.size() - m_CurIndex < p_DataSize) {
            throw InvalidPipelineException(m_EncodedElements);
        }
        std::wstring data = m_EncodedElements.substr(m_CurIndex, p_DataSize);
        m_CurIndex += p_DataSize;
        return data;
    }

    //
    // Reads the number of elements in the pipeline. This must be called
    // first before any other data is read to get the number of encoded
    // elements that follow.
    //
    // @return Number of encoded elements in the pipeline.
    //
    auto PipelineDecoder::EncodedElementsStream::ReadElementCount() -> size_t
    {
        assert(m_CurIndex == 0);

        // The first two characters are a string representation of the number
        // of elements in the pipeline (99 being the maximum number of elements
        // there can be).
        std::wstringstream wss(ReadData(2));
        size_t numElements = 0;
        wss >> numElements;
        return numElements;
    }

    //
    // Reads an encoded integer from the elements stream.
    //
    // @return Integer value.
    //
    auto PipelineDecoder::EncodedElementsStream::ReadLong() -> long
    {
        // Encoded as four consecutive characters corresponding to the integer value.
        std::wstringstream wss(ReadData(4));
        long intValue = 0;
        wss >> intValue;
        return intValue;
    }

    //
    // Reads an encoded string from the elements stream.
    //
    // @return String value.
    //
    auto PipelineDecoder::EncodedElementsStream::ReadString() -> std::wstring
    {
        // First is encoded string size.
        const auto stringSize = gsl::narrow<std::wstring::size_type>(ReadLong());

        // Now that we know the length of the string that is encoded, we simply
        // need to copy that much characters from the encoded string.
        return ReadData(stringSize);
    }

    //
    // Reads an encoded boolean value from the elements stream.
    //
    // @return Boolean value.
    //
    auto PipelineDecoder::EncodedElementsStream::ReadBool() -> bool
    {
        // Boolean values are merely encoded as 0 or 1.
        const wchar_t boolChar = ReadData(1).front();
        if (boolChar != L'0' && boolChar != L'1') {
            throw InvalidPipelineException(m_EncodedElements);
        }
        return boolChar == L'1';
    }

    //
    // Default constructor. Does not set the encoded pipeline string.
    //
    InvalidPipelineException::InvalidPipelineException() noexcept
        : std::exception(),
          m_EncodedElements()
    {
    }

    //
    // Constructor with encoded pipeline string.
    //
    // @param p_EncodedElements Encoded pipeline string.
    //
    InvalidPipelineException::InvalidPipelineException(const std::wstring& p_EncodedElements)
        : std::exception(),
          m_EncodedElements(p_EncodedElements)
    {
    }

    //
    // Returns a reference to the encoded pipeline string.
    //
    // @return Encoded pipeline string reference.
    //
    const std::wstring& InvalidPipelineException::EncodedElements() const noexcept
    {
        return m_EncodedElements;
    }

    //
    // Standardized method returning an exception description.
    //
    // @return Exception description.
    //
    const char* InvalidPipelineException::what() const noexcept
    {
        return "PCC::InvalidPipelineException";
    }

} // namespace PCC
