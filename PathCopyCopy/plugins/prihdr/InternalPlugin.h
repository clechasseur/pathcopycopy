// InternalPlugin.h
// (c) 2008-2021, Charles Lechasseur
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

#include <Plugin.h>

#include <string>

#include <atlstr.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // InternalPlugin
        //
        // Plugin subclass used to implement internal plugins
        // in the PathCopyCopy DLL. The plugin's description
        // and help text will be pulled from resources.
        //
        class InternalPlugin : public Plugin
        {
        public:
                                    InternalPlugin(const InternalPlugin&) = delete;
            InternalPlugin&         operator=(const InternalPlugin&) = delete;

            std::wstring            Description() const override;
            std::wstring            HelpText() const override;

        protected:
            ATL::CStringW           m_DescriptionString;    // String containing plugin description.
            ATL::CStringW           m_HelpTextString;       // String containing plugin help text.

                                    InternalPlugin(unsigned short p_DescriptionStringResourceID,
                                                   unsigned short p_HelpTextStringResourceID);
        };

    } // namespace Plugins

} // namespace PCC
