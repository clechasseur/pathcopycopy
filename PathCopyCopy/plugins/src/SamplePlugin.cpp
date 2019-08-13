// SamplePlugin.h
// (c) 2008-2019, Charles Lechasseur
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
#include <SamplePlugin.h>
#include <resource.h>


namespace
{
    // Plugin unique ID: {FB308FD8-9317-449d-A64E-273201EA7FF8}
    const GUID SAMPLE_PLUGIN_ID = { 0xfb308fd8, 0x9317, 0x449d, { 0xa6, 0x4e, 0x27, 0x32, 0x1, 0xea, 0x7f, 0xf8 } };

} // anonymous namespace

namespace PCC
{
    namespace Plugins
    {
        //
        // Constructor.
        //
        SamplePlugin::SamplePlugin()
            : InternalPlugin(IDS_SAMPLE_PLUGIN_DESCRIPTION, IDS_SAMPLE_PLUGIN_HINT)
        {
        }

        //
        // Returns the plugin's unique identifier.
        //
        // @return Unique identifier.
        //
        const GUID& SamplePlugin::Id() const
        {
            return SAMPLE_PLUGIN_ID;
        }

        //
        // Computes the path of the given file and returns it.
        // This sample plugin simply returns the file path as-is.
        //
        // @param p_File File path.
        // @return File path itself.
        //
        std::wstring SamplePlugin::GetPath(const std::wstring& p_File) const
        {
            return p_File;
        }

    } // namespace Plugins

} // namespace PCC
