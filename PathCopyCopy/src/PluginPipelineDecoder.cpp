// PluginPipelineDecoder.cpp
// (c) 2011-2018, Charles Lechasseur
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
    const wchar_t   ELEMENT_CODE_EMAIL_LINKS                = L'<';
    const wchar_t   ELEMENT_CODE_ENCODE_URI_WHITESPACE      = L's';
    const wchar_t   ELEMENT_CODE_ENCODE_URI_CHARS           = L'%';
    const wchar_t   ELEMENT_CODE_BACK_TO_FORWARD_SLASHES    = L'\\';
    const wchar_t   ELEMENT_CODE_FORWARD_TO_BACKSLASHES     = L'/';
    const wchar_t   ELEMENT_CODE_FIND_REPLACE               = L'?';
    const wchar_t   ELEMENT_CODE_REGEX                      = L'^';
    const wchar_t   ELEMENT_CODE_APPLY_PLUGIN               = L'{';
    const wchar_t   ELEMENT_CODE_PATHS_SEPARATOR            = L',';
    const wchar_t   ELEMENT_CODE_EXECUTABLE                 = L'x';

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
    // @param p_rvspElements Where to store the resulting elements.
    //
    void PipelineDecoder::DecodePipeline(const std::wstring& p_EncodedElements,
                                         PipelineElementSPV& p_rvspElements)
    {
        try {
            // The first two characters are a string representation of the number
            // of elements in the pipeline (99 being the maximum number of elements
            // there can be). Read that using a string stream.
            if (p_EncodedElements.size() < 2) {
                throw InvalidPipelineException();
            }
            std::wstring::const_iterator eIt = p_EncodedElements.begin();
            std::wstring::const_iterator eEnd = p_EncodedElements.end();
            size_t numElements = 0;
            {
                std::wstringstream wss;
                wss << *eIt++;
                wss << *eIt++;
                assert(wss.tellg().seekpos() == 0);
                wss >> numElements;
            }

            // Loop to read the appropriate number of elements.
            for (size_t i = 0; i < numElements; ++i) {
                DecodePipelineElement(eIt, eEnd, p_rvspElements);
            }
        } catch (const InvalidPipelineException&) {
            // Re-throw with the pipeline string.
            throw InvalidPipelineException(p_EncodedElements);
        }
    }

    //
    // Decodes a pipeline element found at the given position in an encoded string
    // and adds it to an existing list of pipeline elements.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data
    //                     in the encoded string. After the method returns, the
    //                     iterator points just pass the element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rvspElements Where to store the new elements.
    //
    void PipelineDecoder::DecodePipelineElement(std::wstring::const_iterator& p_rElementIt,
                                                const std::wstring::const_iterator& p_ElementEnd,
                                                PipelineElementSPV& p_rvspElements)
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
            case ELEMENT_CODE_EXECUTABLE: {
                DecodeExecutableElement(p_rElementIt, p_ElementEnd, spElement);
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
        long version = DecodePipelineInt(p_rElementIt, p_ElementEnd);

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
        wchar_t guidString[GUIDSTRING_MAX];
        std::copy(p_rElementIt, p_rElementIt + (GUIDSTRING_MAX - 1), guidString);
        p_rElementIt += (GUIDSTRING_MAX - 1);
        guidString[GUIDSTRING_MAX - 1] = L'\0';

        // Now that we have the data, convert it to a GUID.
        CLSID pluginGuid;
        if (FAILED(::CLSIDFromString(guidString, &pluginGuid))) {
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
    // Decodes an ExecutablePipelineElement found in an encoded string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the element data in the
    //                     encoded string. After the method returns, the iterator points
    //                     just past the pipeline element's data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rspElement Where to store the newly-created element.
    //
    void PipelineDecoder::DecodeExecutableElement(std::wstring::const_iterator& p_rElementIt,
                                                  const std::wstring::const_iterator& p_ElementEnd,
                                                  PipelineElementSP& p_rspElement)
    {
        // This type of element contains only a string containing the path to the executable.
        std::wstring executable;
        DecodePipelineString(p_rElementIt, p_ElementEnd, executable);
        p_rspElement = std::make_shared<ExecutablePipelineElement>(executable);
    }

    //
    // Standardized method to read an integer value encoded in a pipeline string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the integer data in
    //                     the encoded string. After the method returns, the iterator
    //                     points just past the integer data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @return The resulting integer value
    //
    long PipelineDecoder::DecodePipelineInt(std::wstring::const_iterator& p_rElementIt,
                                            const std::wstring::const_iterator& p_ElementEnd)
    {
        // Encoded as four consecutive characters corresponding the integer value.
        if (std::distance(p_rElementIt, p_ElementEnd) < 4) {
            throw InvalidPipelineException();
        }
        long intValue = 0;
        {
            std::wstringstream wss;
            wss << *p_rElementIt++;
            wss << *p_rElementIt++;
            wss << *p_rElementIt++;
            wss << *p_rElementIt++;
            assert(wss.tellg().seekpos() == 0);
            wss >> intValue;
        }
        return intValue;
    }

    //
    // Standardized method to read a string encoded in a pipeline string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the string data in
    //                     the encoded string. After the method returns, the iterator
    //                     points just past the string data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @param p_rString Where to store the resulting string.
    //
    void PipelineDecoder::DecodePipelineString(std::wstring::const_iterator& p_rElementIt,
                                               const std::wstring::const_iterator& p_ElementEnd,
                                               std::wstring& p_rString)
    {
        // First is encoded string size.
        std::wstring::size_type stringSize = static_cast<std::wstring::size_type>(DecodePipelineInt(p_rElementIt, p_ElementEnd));

        // Now that we know the length of the string that is encoded, we simply
        // need to copy that much characters from the encoded string.
        if (std::distance(p_rElementIt, p_ElementEnd) < static_cast<ptrdiff_t>(stringSize)) {
            throw InvalidPipelineException();
        }
        p_rString.assign(p_rElementIt, p_rElementIt + stringSize);
        p_rElementIt += stringSize;
    }

    //
    // Standardized method to read a boolean value encoded in a pipeline string.
    //
    // @param p_rElementIt Iterator pointing at the beginning of the boolean data in
    //                     the encoded string. After the method returns, the iterator
    //                     points just past the boolean data.
    // @param p_ElementEnd Iterator pointing at the end of the encoded string.
    // @return Resulting boolean value.
    //
    bool PipelineDecoder::DecodePipelineBool(std::wstring::const_iterator& p_rElementIt,
                                             const std::wstring::const_iterator& p_ElementEnd)
    {
        // Boolean values are merely encoded as 0 or 1.
        if (p_rElementIt == p_ElementEnd) {
            throw InvalidPipelineException();
        }
        if (*p_rElementIt != L'0' && *p_rElementIt != L'1') {
            throw InvalidPipelineException();
        }
        return (*p_rElementIt++ == L'1');
    }

    //
    // Default constructor. Does not set the encoded pipeline string.
    //
    InvalidPipelineException::InvalidPipelineException()
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
    const std::wstring& InvalidPipelineException::EncodedElements() const
    {
        return m_EncodedElements;
    }

    //
    // Standardized method returning an exception description.
    //
    // @return Exception description.
    //
    const char* InvalidPipelineException::what() const
    {
        return "PCC::InvalidPipelineException";
    }

} // namespace PCC
