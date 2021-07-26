// AndrogynousInternalPlugin.cpp
// (c) 2015-2021, Charles Lechasseur
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
#include <AndrogynousInternalPlugin.h>
#include <PathCopyCopySettings.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // Returns plugin description, depending on whether it is androgynous or not.
        //
        // @return Plugin description, taken from resources.
        //
        std::wstring AndrogynousInternalPlugin::Description() const
        {
            // Return description depending on whether plugin is androgynous.
            return IsAndrogynous() ? (LPCWSTR) m_AndrogynousDescriptionString
                                   : InternalPlugin::Description();
        }

        //
        // Constructor.
        //
        // @param p_DescriptionStringResourceID ID of string resource containing normal plugin description.
        // @param p_AndrogynousDescriptionStringResourceID ID of string resource containing androgynous plugin description.
        // @param p_HelpTextStringResourceID ID of string resource containing plugin help text.
        //
        AndrogynousInternalPlugin::AndrogynousInternalPlugin(const unsigned short p_DescriptionStringResourceID,
                                                             const unsigned short p_AndrogynousDescriptionStringResourceID,
                                                             const unsigned short p_HelpTextStringResourceID)
            : InternalPlugin(p_DescriptionStringResourceID, p_HelpTextStringResourceID),
              m_AndrogynousDescriptionString(MAKEINTRESOURCEW(p_AndrogynousDescriptionStringResourceID))
        {
        }

    } // namespace Plugins

} // namespace PCC
