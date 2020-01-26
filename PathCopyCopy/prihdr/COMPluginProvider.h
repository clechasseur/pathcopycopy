// COMPluginProvider.h
// (c) 2016-2020, Charles Lechasseur
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

#include "PathCopyCopyPrivateTypes.h"


namespace PCC
{
    //
    // COMPluginProvider
    //
    // Interface for an object that can provide a list of registered COM plugins.
    //
    class COMPluginProvider
    {
    public:
                        COMPluginProvider() = default;
                        COMPluginProvider(const COMPluginProvider&) = delete;
                        COMPluginProvider(COMPluginProvider&&) = delete;
        COMPluginProvider&
                        operator=(const COMPluginProvider&) = delete;
        COMPluginProvider&
                        operator=(COMPluginProvider&&) = delete;
        virtual         ~COMPluginProvider() = default;

        //
        // Returns a list of all registered COM plugins.
        //
        // @return CLSIDs of registered COM plugins.
        //
        virtual CLSIDV  GetCOMPlugins() const = 0;
    };

} // namespace PCC
