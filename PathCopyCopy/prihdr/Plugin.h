// Plugin.h
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

#include "PathCopyCopyPrivateTypes.h"

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include <windows.h>


namespace PCC
{
    //
    // Plugin
    //
    // Base class for PathCopyCopy plugins. A plugin is responsible for
    // computing a particular kind of path for a given file. A plugin
    // will be used to add a menu items to the contextual menu shown
    // in Windows Explorer in the PCC submenu.
    //
    class Plugin
    {
    public:
                                    Plugin(const Plugin&) = delete;
                                    Plugin(Plugin&&) = delete;
        Plugin&                     operator=(const Plugin&) = delete;
        Plugin&                     operator=(Plugin&&) = delete;
        virtual                     ~Plugin() = default;

                                    //
                                    // Returns a unique identifier for the plugin. This will
                                    // be used to identify plugins in the settings.
                                    //
                                    // @return Unique plugin identifier (a GUID).
                                    //
        virtual const GUID&         Id() const = 0;
        virtual const GUID&         IdForIcon() const;

                                    //
                                    // Returns a description of the plugin. Used by PCC
                                    // as the caption for the plugin in the contextual menu.
                                    //
                                    // @return Plugin description.
                                    //
        virtual std::wstring        Description() const = 0;
        virtual std::wstring        HelpText() const;
        virtual std::wstring        IconFile() const;
        virtual bool                UseDefaultIcon() const noexcept(false);
        virtual bool                Enabled(const std::wstring& p_ParentPath,
                                            const std::wstring& p_File) const noexcept(false);

                                    //
                                    // Returns the path of the given file, as determined
                                    // by the plugin's own path scheme.
                                    //
                                    // @param p_File Full path to the file to get the path for.
                                    // @return Path of the file according to plugin.
                                    //
        virtual std::wstring        GetPath(const std::wstring& p_File) const = 0;
        virtual std::wstring        PathsSeparator() const;
        virtual bool                CopyPathsRecursively() const noexcept(false);

        virtual PathActionSP        Action() const;

        virtual bool                IsSeparator() const noexcept;
        virtual bool                CanDropRedundantWords() const noexcept(false);

        void                        SetSettings(const Settings* p_pSettings) noexcept;
        void                        SetPluginProvider(const PluginProvider* p_pPluginProvider) noexcept;

    protected:
        const Settings*             m_pSettings = nullptr;          // Optional object to access PCC settings.
        const PluginProvider*       m_pPluginProvider = nullptr;    // Optional object to access other plugins.

                                    Plugin() = default;
    };

    // Comparison operators for plugins and unique identifiers (e.g. GUIDs).
    bool operator<(const PluginSP& p_spPlugin1, const PluginSP& p_spPlugin2);
    bool operator<(const PluginSP& p_spPlugin1, const GUID& p_PluginId2);
    bool operator<(const GUID& p_PluginId1, const PluginSP& p_spPlugin2);

    // Containers for plugins stored in shared pointers.
    typedef std::vector<PluginSP>           PluginSPV;
    typedef std::set<PluginSP, std::less<>> PluginSPS;

} // namespace PCC
