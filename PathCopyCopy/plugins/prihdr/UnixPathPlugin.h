// UnixPathPlugin.h
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

#include "LongPathPlugin.h"


namespace PCC
{
    namespace Plugins
    {
        //
        // UnixPathPlugin
        //
        // Plugin that returns the path of a file/folder "Unix-style",
        // e.g. by replacing backslashes with forward slashes.
        //
        class UnixPathPlugin : public LongPathPlugin
        {
        public:
                                    UnixPathPlugin();
                                    UnixPathPlugin(const UnixPathPlugin&) = delete;
            UnixPathPlugin&         operator=(const UnixPathPlugin&) = delete;

            virtual const GUID&     Id() const override;

            virtual std::wstring    GetPath(const std::wstring& p_File) const override;

        protected:
                                    UnixPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                                   const unsigned short p_HelpTextStringResourceID);

            virtual bool            IsAndrogynous() const override;
        };

    } // namespace Plugins

} // namespace PCC
