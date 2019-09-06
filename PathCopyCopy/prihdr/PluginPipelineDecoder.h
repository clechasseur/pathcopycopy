// PluginPipelineDecoder.h
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

#include "PathCopyCopyPrivateTypes.h"

#include <exception>
#include <string>


namespace PCC
{
    //
    // PipelineDecoder
    //
    // Utility class that knows the various types of pipeline
    // elements and can be used to decode a pipeline from string.
    //
    class PipelineDecoder final
    {
    public:
                        PipelineDecoder() = delete;
                        ~PipelineDecoder() = delete;

        static auto     DecodePipeline(const std::wstring& p_EncodedElements) -> PipelineElementSPV;

    private:
        static auto     DecodePipelineElement(const std::wstring& p_EncodedElements,
                                              std::wstring::size_type& p_rElementIndex) -> PipelineElementSP;

        static auto     DecodeFindReplaceElement(const std::wstring& p_EncodedElements,
                                                 std::wstring::size_type& p_rElementIndex) -> PipelineElementSP;
        static void     DecodeRegexElement(std::wstring::const_iterator& p_rElementIt,
                                           const std::wstring::const_iterator& p_ElementEnd,
                                           PipelineElementSP& p_rspElement);
        static void     DecodeApplyPluginElement(std::wstring::const_iterator& p_rElementIt,
                                                 const std::wstring::const_iterator& p_ElementEnd,
                                                 PipelineElementSP& p_rspElement);
        static void     DecodePathsSeparatorElement(std::wstring::const_iterator& p_rElementIt,
                                                    const std::wstring::const_iterator& p_ElementEnd,
                                                    PipelineElementSP& p_rspElement);
        static void     DecodeExecutableElement(const wchar_t p_Code,
                                                std::wstring::const_iterator& p_rElementIt,
                                                const std::wstring::const_iterator& p_ElementEnd,
                                                PipelineElementSP& p_rspElement);

        static long     DecodePipelineInt(std::wstring::const_iterator& p_rElementIt,
                                          const std::wstring::const_iterator& p_ElementEnd);
        static void     DecodePipelineString(std::wstring::const_iterator& p_rElementIt,
                                             const std::wstring::const_iterator& p_ElementEnd,
                                             std::wstring& p_rString);
        static bool     DecodePipelineBool(std::wstring::const_iterator& p_rElementIt,
                                           const std::wstring::const_iterator& p_ElementEnd);
    };

    //
    // Exception type thrown when a encoded pipeline string is invalid.
    //
    class InvalidPipelineException : public std::exception
    {
    public:
                        InvalidPipelineException() noexcept;
        explicit        InvalidPipelineException(const std::wstring& p_EncodedElements);

        const std::wstring&
                        EncodedElements() const noexcept;

        const char*     what() const noexcept override;

    private:
        std::wstring    m_EncodedElements;      // The pipeline's encoded string.
    };

} // namespace PCC
