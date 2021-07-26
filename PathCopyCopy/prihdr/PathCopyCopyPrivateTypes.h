// PathCopyCopyPrivateTypes.h
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

#pragma once

// This header contains definition of common private types used throughout the project.

#include <cstdint>
#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <string.h>

#include <windows.h>


namespace PCC
{
    // Forward declarations
    class Plugin;
    class PathAction;
    class PipelineElement;
    class Pipeline;
    class Settings;

    // Interface forward declarations.
    class PluginProvider;
    class COMPluginProvider;
    class PipelinePluginProvider;

    //
    // Predicate used to compare GUID or CLSID structures, like std::less.
    //
    struct GUIDLess {
        bool operator()(const GUID& p_Left, const GUID& p_Right) const noexcept {
            return ::memcmp(&p_Left, &p_Right, sizeof(GUID)) < 0;
        }
    };
    typedef GUIDLess                            CLSIDLess;

    //
    // Predicate used to check GUID or CLSID structures for equality, like std::equal_to.
    //
    struct GUIDEqualTo {
        bool operator()(const GUID& p_Left, const GUID& p_Right) const noexcept {
            return ::memcmp(&p_Left, &p_Right, sizeof(GUID)) == 0;
        }
    };
    typedef GUIDEqualTo                         CLSIDEqualTo;

    typedef std::shared_ptr<Plugin>             PluginSP;               // Shared pointer to a plugin.
    typedef std::shared_ptr<PathAction>         PathActionSP;           // Shared pointer to a path action.
    typedef std::shared_ptr<PipelineElement>    PipelineElementSP;      // Shared pointer to a plugin pipeline element.
    typedef std::vector<PipelineElementSP>      PipelineElementSPV;     // Vector of pipeline elements (via shared pointers).
    typedef std::shared_ptr<Pipeline>           PipelineSP;             // Shared pointer to a plugin pipeline.
    typedef std::shared_ptr<Settings>           SettingsSP;             // Shared pointer to a settings object.

    typedef std::vector<std::wstring>           WStringV;               // Vector of strings.
    typedef std::vector<GUID>                   GUIDV;                  // Vector of GUIDs.
    typedef std::set<GUID, GUIDLess>            GUIDS;                  // Set of GUIDs.
    typedef GUIDV                               CLSIDV;                 // Vector of class IDs (e.g. GUIDs).
    typedef GUIDS                               CLSIDS;                 // Set of class IDs (e.g. GUIDs).
    typedef std::vector<uint32_t>               UInt32V;                // Vector of 32-bit unsigned integers.

    typedef WStringV                            FilesV;                 // Vector of file paths.

    typedef std::shared_ptr<PluginProvider>     PluginProviderSP;       // Shared pointer to an object to access plugins.

} // namespace PCC

// Macro that includes code in debug only.
#ifdef _DEBUG
#   define PCCDEBUGCODE(C) C
#else
#   define PCCDEBUGCODE(C)
#endif // _DEBUG
