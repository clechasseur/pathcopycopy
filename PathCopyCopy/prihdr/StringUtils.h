// StringUtils.h
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


//
// Static class containing some helper methods to deal with strings.
//
class StringUtils final
{
public:
    // Possible ways to encode URI characters
    enum class EncodeParam {
        None,           // No encoding
        Whitespace,     // Encode only whitespace characters
        All,            // Encode all invalid URI characters
    };

                        StringUtils() = delete;
                        ~StringUtils() = delete;

    static std::wstring ToUppercase(std::wstring p_String);

    static void         ReplaceAll(std::wstring& p_rString,
                                   const std::wstring& p_OldValue,
                                   const std::wstring& p_NewValue);
    static PCC::WStringV Split(std::wstring p_String,
                               const std::wstring& p_Separators);
    static PCC::WStringV Split(std::wstring p_String,
                               wchar_t p_Separator);

    static void         EncodeURICharacters(std::wstring& p_rString,
                                            EncodeParam p_EncodeParam);
};
