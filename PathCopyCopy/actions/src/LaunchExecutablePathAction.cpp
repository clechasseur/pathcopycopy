// LaunchExecutablePathAction.cpp
// (c) 2017-2018, Charles Lechasseur
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


namespace PCC
{
    namespace Actions
    {
        //
        // Constructor.
        //
        // @param p_Executable Name of executable to launch.
        //
        LaunchExecutablePathAction::LaunchExecutablePathAction(const std::wstring& p_Executable)
            : PCC::PathAction(),
              m_Executable(p_Executable)
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
            auto res = reinterpret_cast<size_t>(::ShellExecuteW(p_hWnd, nullptr, m_Executable.c_str(), p_Paths.c_str(), nullptr, SW_SHOWDEFAULT));
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
