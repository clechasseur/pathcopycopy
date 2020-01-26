// PathAction.h
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

#pragma once

#include <windows.h>


namespace PCC
{
    //
    // PathAction
    //
    // Base class for actions to take on paths, like copying to clipboard.
    // Each plugin will be responsible for returning such an action to
    // perform on the path(s) returned.
    //
    class PathAction
    {
    public:
                        PathAction(const PathAction&) = delete;
                        PathAction(PathAction&&) = delete;
        PathAction&     operator=(const PathAction&) = delete;
        PathAction&     operator=(PathAction&&) = delete;
        virtual         ~PathAction() = default;

                        //
                        // Performs the action on the given path or paths.
                        // The paths will have been modified by the plugin already.
                        //
                        // @param p_Paths Path or paths to act upon, pre-computed
                        //                and bundled in a single string.
                        // @param p_hWnd Parent window handle, if needed.
                        //
        virtual void    Act(const std::wstring& p_Paths,
                            HWND p_hWnd) const = 0;

    protected:
                        PathAction() = default;
    };

} // namespace PCC
