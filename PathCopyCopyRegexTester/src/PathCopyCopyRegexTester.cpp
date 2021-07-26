// PathCopyCopyRegexTester.cpp
// (c) 2012-2021, Charles Lechasseur
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

#include <regex>


//
// Main program entry point.
//
// @param argc Number of command-line arguments received
// @param argv Array of command-line arguments
// @return Process exit code
//
#pragma warning(suppress: 26485) // Not sure what to do with this one, this is generated code
int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
    // Ask user to provide sample string, regex and replacement format.
    std::wstring sample, regex, format;
    std::wcout << L"Sample string: ";
    std::getline(std::wcin, sample);
    std::wcout << L"Regex: ";
    std::getline(std::wcin, regex);
    std::wcout << L"Format: ";
    std::getline(std::wcin, format);

    // Ask user whether to ignore case or not.
    std::wcout << "Ignore case? [y/n] ";
    wchar_t ignoreCase = L'n';
    std::wcin >> ignoreCase;

    // Output a line ending here. We need this as a separator for the settings app.
    std::wcout << std::endl;

    try {
        // Create regex object and perform replacement. This must be similar
        // to the way this is done in RegexPipelineElement in the main project.
#pragma warning(suppress: 26812)    // std::regex_constants::syntax_option_type should be enum class
        std::regex_constants::syntax_option_type reOptions = std::regex_constants::ECMAScript;
        if (ignoreCase == L'y') {
            reOptions |= std::regex_constants::icase;
        }
        const std::wregex re(regex, reOptions);
        const std::wstring modified = std::regex_replace(sample, re, format);

        // Output modified string:
        std::wcout << L"Modified string: " << modified << std::endl;
    } catch (const std::regex_error&) {
        std::wcout << L"ERROR: invalid regular expression detected!" << std::endl;
    }

    return 0;
}
