// LongUNCFolderPlugin.h
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
#include <PathCopyCopySettings.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // LongUNCFolderPlugin
        //
        // Plugin that returns the long UNC path of a file/folder's parent directory
        // if it has one, otherwise the parent directory's long path.
        //
        class LongUNCFolderPlugin : public LongPathPlugin
        {
        public:
            // ID of this type of plugin.
            static const GUID       ID;

                                    LongUNCFolderPlugin() noexcept(false);
                                    LongUNCFolderPlugin(const LongUNCFolderPlugin&) = delete;
            LongUNCFolderPlugin&    operator=(const LongUNCFolderPlugin&) = delete;

            const GUID&             Id() const noexcept(false) override;

            bool                    Enabled(const std::wstring& p_ParentPath,
                                            const std::wstring& p_File) const override;

            std::wstring            GetPath(const std::wstring& p_File) const override;

        protected:
                                    LongUNCFolderPlugin(unsigned short p_DescriptionStringResourceID,
                                                        unsigned short p_AndrogynousDescriptionStringResourceID,
                                                        unsigned short p_HelpTextStringResourceID);

            bool                    IsAndrogynous() const override;

            bool                    InternalGetPath(std::wstring& p_rPath,
                                                    bool p_ExtractFolder) const;
        };

    } // namespace Plugins

} // namespace PCC
