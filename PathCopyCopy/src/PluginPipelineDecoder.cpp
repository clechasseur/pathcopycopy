// PluginPipelineDecoder.cpp
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

#include <stdafx.h>
#include <PluginPipelineDecoder.h>
#include <PluginPipelineElements.h>

#include <assert.h>
#include <sstream>


namespace
{
    // Each pipeline element type is represented using a single character
    // in the encoded string. This is a list of all possible values.
    const wchar_t   ELEMENT_CODE_QUOTES                     = L'"';
    const wchar_t   ELEMENT_CODE_OPTIONAL_QUOTES            = L'q';
    const wchar_t   ELEMENT_CODE_EMAIL_LINKS                = L'<';
    const wchar_t   ELEMENT_CODE_ENCODE_URI_WHITESPACE      = L's';
    const wchar_t   ELEMENT_CODE_ENCODE_URI_CHARS           = L'%';
    const wchar_t   ELEMENT_CODE_BACK_TO_FORWARD_SLASHES    = L'\\';
    const wchar_t   ELEMENT_CODE_FORWARD_TO_BACKSLASHES     = L'/';
    const wchar_t   ELEMENT_CODE_REMOVE_EXT                 = L'.';
    const wchar_t   ELEMENT_CODE_FIND_REPLACE               = L'?';
    const wchar_t   ELEMENT_CODE_REGEX                      = L'^';
    const wchar_t   ELEMENT_CODE_APPLY_PLUGIN               = L'{';
    const wchar_t   ELEMENT_CODE_PATHS_SEPARATOR            = L',';
    const wchar_t   ELEMENT_CODE_EXECUTABLE                 = L'x';
    const wchar_t   ELEMENT_CODE_EXECUTABLE_WITH_FILELIST   = L'f';

    // Version numbers used for regex elements.
    const long      REGEX_ELEMENT_INITIAL_VERSION           = 1;
    const long      REGEX_ELEMENT_MAX_VERSION               = REGEX_ELEMENT_INITIAL_VERSION;

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
        // The first two characters are a string representation of the number
        // of elements in the pipeline (99 being the maximum number of elements
        // there can be).
        if (p_EncodedElements.size() < 2) {
            throw InvalidPipelineException();
        }
        size_t numElements = 0;
        {
            std::wstringstream wss;
            wss << p_EncodedElements[0];
            wss << p_EncodedElements[1];
            assert(wss.tellg() == std::streamoff{0});
            wss >> numElements;
        }

        // Loop to read the appropriate number of elements.
        PipelineElementSPV vspPipelineElements;
        vspPipelineElements.reserve(numElements);
        std::wstring::size_type elementIndex = 2;
        for (size_t i = 0; i < numElements; ++i) {
            vspPipelineElements.emplace_back(DecodePipelineElement(p_EncodedElements, elementIndex));
        }

        return vspPipelineElements;
    }

    //
    // Decodes a pipeline element found at the given position in an encoded string
    // and adds it to an existing list of pipeline elements.
    //
    // @param p_EncodedElements String containing encoded elements.
    // @param p_rElementIndex Index of current element to decode in encoded elements string.
    // @return New pipeline element.
    //
    auto PipelineDecoder::DecodePipelineElement(const std::wstring& p_EncodedElements,
                                                std::wstring::size_type& p_rElementIndex) -> PipelineElementSP
    {
        PipelineElementSP spElement;

        // The first character represents a code telling us what element type it is.
        if (p_rElementIt == p_ElementEnd) {
            throw InvalidPipelineException();
        }
        wchar_t code = *p_rElementIt++;

        // Look at the code and create the appropriate element.
        switch (code) {
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
                DecodeFindReplaceElement(p_rElementIt, p_ElementEnd, spElement);
                break;
            }
            case ELEMENT_CODE_REGEX: {
                DecodeRegexElement(p_rElementIt, p_ElementEnd, spElement);
                break;
            }
            case ELEMENT_CODE_APPLY_PLUGIN: {
                DecodeApplyPluginElement(p_rElementIt, p_ElementEnd, spElement);
                break;
            }
            case ELEMENT_CODE_PATHS_SEPARATOR: {
                DecodePathsSeparatorElement(p_rElementIt, p_ElementEnd, spElement);
                break;
            }
            case ELEMENT_CODE_EXECUTABLE:
            case ELEMENT_CODE_EXECUTABLE_WITH_FILELIST: {
                DecodeExecutableElement(code, p_rElementIt, p_ElementEnd, spElement);
                break;
            }
            default:
                // Unknown element type, we can't add it and don't know
                // how to skip it. Possibly due to a downgrade of PCC?
                throw InvalidPipelineException();
        }

        // Add new element to the pipeline.
        if (spElement == nullptr) {
            throw InvalidPipelineException();
        }
        p_rvspElements.push_back(spElement);
    }

    //
    // Decodes a FindReplacePipelineElement found in an encoded string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodeFindReplaceElement(std::wstring::const_iterator& p_rElementIt,
                                                   const std::wstring::const_iterator& p_ElementEnd,
                                                   PipelineElementSP& p_rspElement)
    {
        // This type of element contains an old and a new value.
        std::wstring oldValue, newValue;
        DecodePipelineString(p_rElementIt, p_ElementEnd, oldValue);
        DecodePipelineString(p_rElementIt, p_ElementEnd, newValue);
        p_rspElement = std::make_shared<FindReplacePipelineElement>(oldValue, newValue);
    }

    //
    // Decodes a RegexPipelineElement found in an encoded string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodeRegexElement(std::wstring::const_iterator& p_rElementIt,
                                             const std::wstring::const_iterator& p_ElementEnd,
                                             PipelineElementSP& p_rspElement)
    {
        // The data starts by a version number. This is used in case we need to add
        // support for extra options in the future.
        const long version = DecodePipelineInt(p_rElementIt, p_ElementEnd);

        // Make sure it's a version we can support.
        if (version > REGEX_ELEMENT_MAX_VERSION) {
            throw InvalidPipelineException();
        }

        // Initial version: regex, format string and whether we should ignore case.
        std::wstring regex, format;
        DecodePipelineString(p_rElementIt, p_ElementEnd, regex);
        DecodePipelineString(p_rElementIt, p_ElementEnd, format);
        bool ignoreCase = DecodePipelineBool(p_rElementIt, p_ElementEnd);

        // Create the element and return it.
        p_rspElement = std::make_shared<RegexPipelineElement>(regex, format, ignoreCase);
    }

    //
    // Decodes an ApplyPluginPipelineElement found in an encoded string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodeApplyPluginElement(std::wstring::const_iterator& p_rElementIt,
                                                   const std::wstring::const_iterator& p_ElementEnd,
                                                   PipelineElementSP& p_rspElement)
    {
        // The element data is a string representation of the GUID of the plugin to apply.
        // A guid has the following format: {1B4B1405-84CF-48CC-B373-42FAD7744258}
        if (std::distance(p_rElementIt, p_ElementEnd) < (GUIDSTRING_MAX - 1)) {
            throw InvalidPipelineException();
        }
        std::wstring guidString(p_rElementIt, p_rElementIt + (GUIDSTRING_MAX - 1));
        p_rElementIt += (GUIDSTRING_MAX - 1);

        // Now that we have the data, convert it to a GUID.
        CLSID pluginGuid;
        if (FAILED(::CLSIDFromString(guidString.c_str(), &pluginGuid))) {
            // Invalid GUID format.
            throw InvalidPipelineException();
        }

        // We have the plugin GUID, return it.
        p_rspElement = std::make_shared<ApplyPluginPipelineElement>(pluginGuid);
    }

    //
    // Decodes an PathsSeparatorPipelineElement found in an encoded string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodePathsSeparatorElement(std::wstring::const_iterator& p_rElementIt,
                                                      const std::wstring::const_iterator& p_ElementEnd,
                                                      PipelineElementSP& p_rspElement)
    {
        // This type of element contains only the paths separator string.
        std::wstring pathsSeparator;
        DecodePipelineString(p_rElementIt, p_ElementEnd, pathsSeparator);
        p_rspElement = std::make_shared<PathsSeparatorPipelineElement>(pathsSeparator);
    }

    //
    // Decodes an ExecutablePipelineElement or ExecutableWithFilelistPipelineElement
    // found in an encoded string.
    //
    // @param p_Code Element code.
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodeExecutableElement(const wchar_t p_Code,
                                                  std::wstring::const_iterator& p_rElementIt,
                                                  const std::wstring::const_iterator& p_ElementEnd,
                                                  PipelineElementSP& p_rspElement)
    {
        // This type of element contains only a string containing the path to the executable.
        std::wstring executable;
        DecodePipelineString(p_rElementIt, p_ElementEnd, executable);
        if (p_Code == ELEMENT_CODE_EXECUTABLE) {
            p_rspElement = std::make_shared<ExecutablePipelineElement>(executable);
        } else if (p_Code == ELEMENT_CODE_EXECUTABLE_WITH_FILELIST) {
            p_rspElement = std::make_shared<ExecutableWithFilelistPipelineElement>(executable);
        } else {
            throw InvalidPipelineException();
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
        const auto stringSize = std::wstring::size_type{ReadLong()};

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
