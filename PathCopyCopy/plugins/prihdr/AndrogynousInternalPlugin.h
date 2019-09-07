// AndrogynousInternalPlugin.h
// (c) 2015-2019, Charles Lechasseur
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

#include "InternalPlugin.h"
#include <PathCopyCopyPrivateTypes.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // AndrogynousInternalPlugin
        //
        // Base class for internal plugins whose description can
        // change depending on the circumstances. Subclasses should
        // provide their "androgynous" description and override
        // the IsAndrogynous method to use this feature.
        //
        class AndrogynousInternalPlugin : public InternalPlugin
        {
        public:
                                    AndrogynousInternalPlugin(const AndrogynousInternalPlugin&) = delete;
            AndrogynousInternalPlugin&
                                    operator=(const AndrogynousInternalPlugin&) = delete;

            std::wstring            Description() const override;

        protected:
            ATL::CStringW           m_AndrogynousDescriptionString;     // String containing plugin androgynous description.

                                    AndrogynousInternalPlugin(const unsigned short p_DescriptionStringResourceID,
                                                              const unsigned short p_AndrogynousDescriptionStringResourceID,
                                                              const unsigned short p_HelpTextStringResourceID);

                                    //
                                    // Method that determines which description will be returned.
                                    // If this method returns true, the plugin's androgynous description
                                    // is used. If it returns false, its normal description is used.
                                    //
                                    // @return true to use androgynous description, false to use normal description.
                                    //
            virtual bool            IsAndrogynous() const = 0;
        };

    } // namespace Plugins

} // namespace PCC
