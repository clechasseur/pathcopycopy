// LaunchExecutablePathAction.cpp
// (c) 2017-2019, Charles Lechasseur
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
#include <memory>


namespace PCC
{
    namespace Actions
    {
        //
        // Constructor.
        //
        // @param p_Executable Name of executable to launch.
        // @param p_UseFilelist Whether to use a filelist to launch executable instead of passing paths directly.
        //
        LaunchExecutablePathAction::LaunchExecutablePathAction(const std::wstring& p_Executable,
                                                               const bool          p_UseFilelist)
            : PCC::PathAction(),
              m_Executable(p_Executable),
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
                                             const HWND          p_hWnd) const
        {
            std::wstring arguments = p_Paths;
            if (m_UseFilelist) {
                wchar_t tempPath[MAX_PATH + 1];
                if (::GetTempPathW(MAX_PATH + 1, tempPath) != 0) {
                    wchar_t tempFilePath[MAX_PATH + 1];
                    if (::GetTempFileNameW(tempPath, L"pcc", 0, tempFilePath) != 0) {
                        int reqdBufferSize = ::WideCharToMultiByte(CP_ACP, 0, p_Paths.c_str(), -1,
                                                                   nullptr, 0, nullptr, nullptr);
                        std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[reqdBufferSize]);
                        int retVal = ::WideCharToMultiByte(CP_ACP, 0, p_Paths.c_str(), -1, buffer.get(),
                                                           reqdBufferSize, nullptr, nullptr);
                        if (retVal != 0) {
                            {
                                std::ofstream of(tempFilePath);
                                of << std::string(buffer.get()) << std::endl;
                            }
                            arguments = tempFilePath;
                        }
                    }
                }
            }

            auto res = reinterpret_cast<size_t>(::ShellExecuteW(p_hWnd, nullptr, m_Executable.c_str(), arguments.c_str(), nullptr, SW_SHOWDEFAULT));
            if (res <= 32) {
                throw LaunchExecutableException();
            }
        }

        //
        // Returns a textual description of the exception.
        //
        // @return Exception textual description.
        //
        const char* LaunchExecutableException::what() const
        {
            return "LaunchExecutableException";
        }

    } // namespace Actions

} // namespace PCC
