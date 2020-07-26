// LaunchExecutablePathAction.cpp
// (c) 2017-2020, Charles Lechasseur
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
#include <LaunchExecutablePathAction.h>

#include <fstream>
#include <regex>


namespace
{
    const wchar_t* const    FILES_ARGUMENT_PLACEHOLDER  = L"%FILES%";   // Placeholder where to put file paths in arguments.

} // anonymous namespace

namespace PCC
{
    namespace Actions
    {
        //
        // Constructor.
        //
        // @param p_Executable Name of executable to launch.
        // @param p_Arguments Arguments to pass to the executable.
        // @param p_UseFilelist Whether to use a filelist to launch executable
        //                      instead of passing paths directly.
        //
        LaunchExecutablePathAction::LaunchExecutablePathAction(const std::wstring& p_Executable,
                                                               const std::wstring& p_Arguments,
                                                               const bool p_UseFilelist)
            : PCC::PathAction(),
              m_Executable(p_Executable),
              m_Arguments(p_Arguments),
              m_UseFilelist(p_UseFilelist)
        {
        }
        
        //
        // Launches executable with the path or paths as argument.
        //
        // @param p_Paths Path or paths to pass as arguments,
        //                pre-bundled in a single string.
        // @param p_hWnd Parent window handle, if needed.
        //
        void LaunchExecutablePathAction::Act(const std::wstring& p_Paths,
                                             HWND const p_hWnd) const
        {
            auto files = p_Paths;
            if (m_UseFilelist) {
                // Get path to temp directory
                std::wstring tempDirPath(MAX_PATH + 1, L'\0');
                if (::GetTempPathW(gsl::narrow<DWORD>(tempDirPath.size()), &*tempDirPath.begin()) == 0) {
                    throw LaunchExecutableException();
                }

                // Generate temp file for the paths
                std::wstring tempFilePath(MAX_PATH + 1, L'\0');
                if (::GetTempFileNameW(tempDirPath.c_str(), L"pcc", 0, &*tempFilePath.begin()) == 0) {
                    throw LaunchExecutableException();
                }

                // Convert paths to single-byte string
                const ATL::CStringA mbPaths(p_Paths.c_str());

                // Write paths to the temp file and use path to that file in arguments to executable
                std::ofstream of(tempFilePath, std::ios::out | std::ios::binary);
                of.write((LPCSTR) mbPaths, mbPaths.GetLength());
                files = tempFilePath;
            }

            // Look for files placeholder in arguments. If it's not there, append the files.
            auto arguments = m_Arguments;
            std::wregex placeholderRegex(FILES_ARGUMENT_PLACEHOLDER,
                                         std::regex_constants::ECMAScript | std::regex_constants::icase);
            std::wsmatch match;
            if (std::regex_search(arguments, match, placeholderRegex)) {
                arguments = match.prefix().str() + files + match.suffix().str();
            } else {
                if (arguments.empty() || arguments.back() != L' ') {
                    arguments += L' ';
                }
                arguments += files;
            }

            // Need reinterpret_cast here because of legacy Win32 API
            [[gsl::suppress(type.1)]]
            {
                const auto res = reinterpret_cast<size_t>(::ShellExecuteW(p_hWnd,
                                                                          nullptr,
                                                                          m_Executable.c_str(),
                                                                          arguments.c_str(),
                                                                          nullptr,
                                                                          SW_SHOWDEFAULT));
                if (res <= 32) {
                    throw LaunchExecutableException();
                }
            }
        }

        //
        // Returns a textual description of the exception.
        //
        // @return Exception textual description.
        //
        const char* LaunchExecutableException::what() const noexcept(false)
        {
            return "LaunchExecutableException";
        }

    } // namespace Actions

} // namespace PCC
