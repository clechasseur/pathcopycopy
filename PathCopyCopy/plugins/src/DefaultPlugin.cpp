// DefaultPlugin.h
// (c) 2011-2020, Charles Lechasseur
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
#include <DefaultPlugin.h>
#include <resource.h>


namespace
{
    // Plugin unique ID: {E222B721-5FEC-40b6-BFA1-9814DB35577A}
    const GUID DEFAULT_PLUGIN_ID = { 0xe222b721, 0x5fec, 0x40b6, { 0xbf, 0xa1, 0x98, 0x14, 0xdb, 0x35, 0x57, 0x7a } };

} // anonymous namespace

namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        DefaultPlugin::DefaultPlugin() noexcept(false)
            : LongPathPlugin(IDS_DEFAULT_PLUGIN_DESCRIPTION, IDS_DEFAULT_PLUGIN_DESCRIPTION, IDS_LONG_PATH_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& DefaultPlugin::Id() const noexcept(false)
        {
            return DEFAULT_PLUGIN_ID;
        }

        //
        // Called by Path Copy Copy to know if it should honor the
        // "Drop redundant words" setting for this plugin. In our
        // case, the plugin description comes from resources and since
        // this plugin will always be displayed alone in the main
        // contextual menu, we never want to modify it.
        //
        // @return Always false to indicate PCC should not drop
        //         redundant words like "copy" from plugin's description.
        //
        bool DefaultPlugin::CanDropRedundantWords() const noexcept(false)
        {
            return false;
        }

        //
        // Determines if this plugin is androgynous. In our case, it never is.
        //
        // @return true to use androgynous description, false to use normal description.
        //
        bool DefaultPlugin::IsAndrogynous() const noexcept(false)
        {
            return false;
        }

    } // namespace Plugins

} // namespace PCC
