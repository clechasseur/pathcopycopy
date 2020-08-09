// UNCPathPlugin.h
// (c) 2008-2020, Charles Lechasseur
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

#include <LongPathPlugin.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // UNCPathPlugin
        //
        // Plugin that returns the UNC path of a file/folder if it has one,
        // otherwise its long path.
        //
        class UNCPathPlugin : public LongPathPlugin
        {
        public:
                                    UNCPathPlugin();

            virtual const GUID&     Id() const;

            virtual std::wstring    GetPath(const std::wstring& p_File) const;

        protected:
                                    UNCPathPlugin(const unsigned short p_DescriptionStringResourceID,
                                                  const unsigned short p_HelpTextStringResourceID);

            bool                    GetMappedDriveFilePath(std::wstring& p_rFilePath) const;
            bool                    GetNetworkShareFilePath(std::wstring& p_rFilePath) const;

            const std::wstring&     GetLocalComputerName() const;
            void                    GetMultiStringLineBeginningWith(const std::wstring& p_MultiStringValue,
                                                                    const std::wstring& p_Prefix,
                                                                    std::wstring&       p_rLine) const;

        private:
            mutable std::wstring    m_ComputerName;     // Name of local computer.
        };

    } // namespace Plugins

} // namespace PCC
