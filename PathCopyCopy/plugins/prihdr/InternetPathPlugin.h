// InternetPathPlugin.h
// (c) 2010-2018, Charles Lechasseur
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

#include "LongUNCPathPlugin.h"


namespace PCC
{
    namespace Plugins
    {
        //
        // InternetPathPlugin
        //
        // Plugin that returns path of a file/folder in URI format, like
        // file://path/to/file     
        //
        class InternetPathPlugin : public LongUNCPathPlugin
        {
        public:
                                    InternetPathPlugin();
                                    InternetPathPlugin(const InternetPathPlugin&) = delete;
            InternetPathPlugin&     operator=(const InternetPathPlugin&) = delete;

            virtual const GUID&     Id() const override;

            virtual bool            Enabled(const std::wstring& p_ParentPath,
                                            const std::wstring& p_File) const override;

            virtual std::wstring    GetPath(const std::wstring& p_File) const override;

        protected:
            virtual bool            IsAndrogynous() const override;
        };

    } // namespace Plugins

} // namespace PCC
