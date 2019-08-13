// PluginSeparator.cpp
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
#include <PluginSeparator.h>


namespace
{
    // Plugin unique ID: {AFA4D1E1-BA73-4330-A6AB-E574FF39ECC3}
    const GUID PLUGIN_SEPARATOR_ID = { 0xafa4d1e1, 0xba73, 0x4330, { 0xa6, 0xab, 0xe5, 0x74, 0xff, 0x39, 0xec, 0xc3 } };

} // anonymous namespace

namespace PCC
{
    //
    // Constructor.
    //
    PluginSeparator::PluginSeparator()
        : Plugin()
    {
    }

    //
    // Returns the plugin's unique identifier.
    //
    // @return Unique identifier.
    //
    const GUID& PluginSeparator::Id() const
    {
        return PLUGIN_SEPARATOR_ID;
    }

    //
    // Placeholder description method that returns an empty string
    // since it is never called.
    //
    // @return Empty string.
    //
    std::wstring PluginSeparator::Description() const
    {
        return L"";
    }

    //
    // Placeholder path method that does nothing since it is never called.
    //
    // @param p_File File path; unused.
    // @return Empty string.
    //
    std::wstring PluginSeparator::GetPath(const std::wstring& /*p_File*/) const
    {
        return L"";
    }

    //
    // Checks if this plugin is a separator. In our case, we return true.
    //
    // @return true since we are a separator proxy.
    //
    bool PluginSeparator::IsSeparator() const
    {
        return true;
    }

} // namespace PCC
