// PluginPipeline.cpp
// (c) 2011-2017, Charles Lechasseur
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
#include <PluginPipeline.h>
#include <PluginPipelineDecoder.h>


namespace PCC
{
    //
    // Returns the separator to use between multiple copied paths.
    //
    // @return Separator between multiple paths. An empty string
    //         means to use the default value.
    //
    const std::wstring& PipelineOptions::GetPathsSeparator() const
    {
        return m_PathsSeparator;
    }

    //
    // Sets the separator to use between multiple paths. An empty
    // string means to use the default value.
    //
    // @param p_PathsSeparator Separator between multiple paths
    //                         or empty string for default value.
    void PipelineOptions::SetPathsSeparator(const std::wstring& p_PathsSeparator)
    {
        m_PathsSeparator = p_PathsSeparator;
    }

    //
    // Constructor with pre-built elements.
    //
    // @param p_vspElements List of elements in the pipeline.
    //
    Pipeline::Pipeline(const PipelineElementSPV& p_vspElements)
        : m_vspElements(p_vspElements)
    {
    }

    //
    // Constructor with encoded pipeline. The pipeline is
    // built by decoding the string. See PipelineDecoder.
    //
    // @param p_EncodedElements Elements encoded in a string.
    //
    Pipeline::Pipeline(const std::wstring& p_EncodedElements)
        : m_vspElements()
    {
        PipelineDecoder::DecodePipeline(p_EncodedElements, m_vspElements);
    }

    //
    // Modifies global pipeline options by successively applying all pipeline
    // elements to it. Returns the final version of the pipeline options.
    //
    // @param p_rOptions Global pipeline options to modify.
    //
    void Pipeline::ModifyOptions(PipelineOptions& p_rOptions) const
    {
        for (const PipelineElementSP& spElement : m_vspElements) {
            spElement->ModifyOptions(p_rOptions);
        }
    }

    //
    // Modifies a given path by successively applying all pipeline
    // elements to it. Returns the final version of the path.
    //
    // @param p_rPath Path to modify. Will be modified in-place.
    // @param p_pPluginProvider Optional object to access plugins.
    //
    void Pipeline::ModifyPath(std::wstring& p_rPath,
                              const PluginProvider* const p_pPluginProvider) const
    {
        for (const PipelineElementSP& spElement : m_vspElements) {
            spElement->ModifyPath(p_rPath, p_pPluginProvider);
        }
    }

    //
    // Checks if a plugin using this pipeline should be enabled or not.
    // Any part of the pipeline that returns false for this will make the item disabled.
    //
    // @param p_ParentPath Path of the parent folder for the file to check.
    // @param p_File Path of file to use for the check.
    // @param p_pPluginProvider Optional object to access plugins.
    // @return false if pipeline says plugin should be disabled for this path.
    //
    bool Pipeline::ShouldBeEnabledFor(const std::wstring& p_ParentPath,
                                      const std::wstring& p_File,
                                      const PluginProvider* const p_pPluginProvider) const
    {
        bool enabled = true;
        PipelineElementSPV::const_iterator it, end = m_vspElements.cend();
        for (it = m_vspElements.cbegin(); enabled && it != end; ++it) {
            enabled = (*it)->ShouldBeEnabledFor(p_ParentPath, p_File, p_pPluginProvider);
        }
        return enabled;
    }

    //
    // Default constructor.
    //
    PipelineElement::PipelineElement()
    {
    }

    //
    // Destructor.
    //
    PipelineElement::~PipelineElement()
    {
    }

    //
    // Modifies global pipeline options. Each element has the opportunity
    // to modify them when a path is modified.
    //
    // @param p_rOptions Global pipeline options to modify.
    //
    void PipelineElement::ModifyOptions(PipelineOptions& /*p_rOptions*/) const
    {
    }

    //
    // Checks if a plugin using this pipeline element should be enabled or not.
    // Any part of a pipeline that returns false for this will make the item disabled.
    //
    // @param p_ParentPath Path of the parent folder for the file to check.
    // @param p_File Path of file to use for the check.
    // @param p_pPluginProvider Optional object to access plugins.
    // @return false if pipeline element says plugin should be disabled for this path.
    //
    bool PipelineElement::ShouldBeEnabledFor(const std::wstring& /*p_ParentPath*/,
                                             const std::wstring& /*p_File*/,
                                             const PluginProvider* const /*p_pPluginProvider*/) const
    {
        return true;
    }

} // namespace PCC
