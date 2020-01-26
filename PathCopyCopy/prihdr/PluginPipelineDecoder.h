// PluginPipelineDecoder.h
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

#include "PathCopyCopyPrivateTypes.h"

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
        //
        // Utility class that can be used to decode data from an encoded elements stream.
        // Keeps track of how much data remains in the stream to validate data.
        //
        class EncodedElementsStream final
        {
        public:
            explicit    EncodedElementsStream(const std::wstring& p_EncodedElements);
                        EncodedElementsStream(const EncodedElementsStream&) = delete;
            EncodedElementsStream&
                        operator=(const EncodedElementsStream&) = delete;

            auto        ReadData(std::wstring::size_type p_DataSize) -> std::wstring;
            auto        ReadElementCount() -> size_t;
            auto        ReadLong() -> long;
            auto        ReadString() -> std::wstring;
            auto        ReadBool() -> bool;

        private:
            const std::wstring
                        m_EncodedElements;      // The pipeline's encoded string.
            std::wstring::size_type
                        m_CurIndex;             // Position of read marker.
        };

        static auto     DecodePipelineElement(EncodedElementsStream& p_rStream) -> PipelineElementSP;

        static auto     DecodeFindReplaceElement(EncodedElementsStream& p_rStream) -> PipelineElementSP;
        static auto     DecodeRegexElement(EncodedElementsStream& p_rStream) -> PipelineElementSP;
        static auto     DecodeApplyPluginElement(wchar_t p_Code,
                                                 EncodedElementsStream& p_rStream) -> PipelineElementSP;
        static auto     DecodePathsSeparatorElement(EncodedElementsStream& p_rStream) -> PipelineElementSP;
        static auto     DecodeExecutableElement(wchar_t p_Code,
                                                EncodedElementsStream& p_rStream) -> PipelineElementSP;
    };

} // namespace PCC
