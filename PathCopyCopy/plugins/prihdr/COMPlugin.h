// COMPlugin.h
// (c) 2008-2018, Charles Lechasseur
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

#include <PathCopyCopy_i.h>
#include <Plugin.h>

#include <exception>

#include <atlcomcli.h>


namespace PCC
{
    namespace Plugins
    {
        //
        // COMPlugin
        //
        // Plugin that wraps a COM plugin implementation.
        //
        class COMPlugin final : public Plugin
        {
        public:
            explicit                COMPlugin(const CLSID& p_CLSID);
                                    COMPlugin(const COMPlugin&) = delete;
            COMPlugin&              operator=(const COMPlugin&) = delete;

            virtual const GUID&     Id() const override;
            ULONG                   GroupId() const;
            ULONG                   GroupPosition() const;

            virtual std::wstring    Description() const override;
            virtual std::wstring    HelpText() const override;
            virtual std::wstring    IconFile() const override;
            virtual bool            UseDefaultIcon() const override;
            virtual bool            Enabled(const std::wstring& p_ParentPath,
                                            const std::wstring& p_File) const override;

            virtual std::wstring    GetPath(const std::wstring& p_File) const override;

            virtual bool            CanDropRedundantWords() const override;

        private:
            GUID                    m_Id;               // Unique plugin ID.
            ATL::CComPtr<IPathCopyCopyPlugin>
                                    m_cpPlugin;         // Pointer to COM plugin implementation.
            ATL::CComQIPtr<IPathCopyCopyPluginGroupInfo>
                                    m_cpPluginGroup;    // Reference to grouping interface implementation. Can be NULL.
            ATL::CComQIPtr<IPathCopyCopyPluginStateInfo>
                                    m_cpPluginState;    // Reference to state changing interface implementation. Can be NULL.
            ATL::CComQIPtr<IPathCopyCopyPluginIconInfo>
                                    m_cpPluginIcon;     // Reference to icon interface implementation. Can be NULL.
            std::wstring            m_Description;      // Plugin description.
        };

        //
        // COMPluginError
        //
        // Exception class thrown when a COMPlugin encounters an error.
        //
        class COMPluginError : public std::exception
        {
        public:
            explicit                COMPluginError(const HRESULT p_Result);

            virtual const char*     what() const override;

        private:
            HRESULT                 m_Result;   // Contains an HRESULT returned by the call.
        };

    } // namespace Plugins

} // namespace PCC
