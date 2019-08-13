// StringUtils.cpp
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
#include <StringUtils.h>

#include <assert.h>
#include <functional>
#include <sstream>


//
// Replaces all instance of p_OldValue in p_rString with p_NewValue.
//
// @param p_rString String to modify (in-place).
// @param p_OldValue Old value to look for.
// @param p_NewValue Replacement value.
//
void StringUtils::ReplaceAll(std::wstring& p_rString,
                             const std::wstring& p_OldValue,
                             const std::wstring& p_NewValue)
{
    std::wstring::size_type pos;
    std::wstring::size_type from = 0;
    do {
        // Look for the next instance of the old value.
        pos = p_rString.find(p_OldValue, from);
        if (pos != std::wstring::npos) {
            // Replace this instance with our replacement value.
            p_rString.replace(pos, p_OldValue.size(), p_NewValue);

            // Start looking after the replacement next time.
            from = pos + p_NewValue.size();
        }
    } while (pos != std::wstring::npos);
}

//
// Splits the given string using the given separator into parts.
//
// @param p_String String to split. Upon return, this string is unusable.
// @param p_Separator Separator to use for splitting.
// @param p_rParts Where to store the string parts.
//
void StringUtils::Split(std::wstring& p_rString,
                        const wchar_t p_Separator,
                        PCC::WStringV& p_rParts)
{
    p_rParts.clear();
    if (!p_rString.empty()) {
        const wchar_t separators[] = { p_Separator, L'\0' };
        wchar_t* context = 0;
        wchar_t* pCurToken = ::wcstok_s(&*p_rString.begin(), separators, &context);
        while (pCurToken != 0) {
            p_rParts.push_back(pCurToken);
            pCurToken = ::wcstok_s(NULL, separators, &context);
        }
    }
}

//
// Encodes characters in the given string that are invalid
// in an URI in percent-encoding (e.g., %20 for space, etc.)
//
// @param p_rString String whose characters to encode. Upon return,
//                  will contain the string with encoded characters.
// @param p_EncodeParam What characters to encode.
//
void StringUtils::EncodeURICharacters(std::wstring& p_rString,
                                      const EncodeParam p_EncodeParam)
{
    // The characters identified as "invalid" by this method were chosen
    // based on the information found in the following StackOverflow thread:
    //
    // http://stackoverflow.com/questions/1547899/which-characters-make-a-url-invalid/13500078#13500078
    //
    // This has been suggested in the Feature request that led to this code:
    //
    // https://pathcopycopy.codeplex.com/workitem/11374

    if (p_EncodeParam != EncodeParam::None) {
        // Choose function to identify characters to encode.
        std::function<bool(wchar_t)> identifyFunc;
        switch (p_EncodeParam) {
            case EncodeParam::Whitespace: {
                identifyFunc = [](const wchar_t p_Char) {
                    // #clp TODO validate
                    const unsigned int val = static_cast<unsigned int>(p_Char);
                    return val <= 0x1F || val == 0x20 || val == 0x7F;
                };
                break;
            }
            case EncodeParam::All: {
                identifyFunc = [](const wchar_t p_Char) {
                    const unsigned int val = static_cast<unsigned int>(p_Char);
                    return val <= 0x1F || val == 0x20 || val == 0x7F || p_Char == L'<' || p_Char == L'>' ||
                        p_Char == L'#' || p_Char == L'%' || p_Char == L'"' || p_Char == L'{' ||
                        p_Char == L'}' || p_Char == L'|' || p_Char == L'\\' || p_Char == L'^' ||
                        p_Char == L'[' || p_Char == L']' || p_Char == L'`' || p_Char == L'+';
                };
                break;
            }
            default:
                assert(false);
        }

        std::wstringstream wss;
        wss << std::hex;
        for (std::wstring::const_iterator it = p_rString.cbegin(); it != p_rString.cend(); ++it) {
            const wchar_t curChar = *it;
            if (identifyFunc(curChar)) {
                wss << L"%" << static_cast<unsigned int>(curChar);
            } else {
                wss << curChar;
            }
        }
        p_rString = wss.str();
    }
}
