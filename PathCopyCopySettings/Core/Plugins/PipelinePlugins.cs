// PipelinePlugins.cs
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

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.UserControls;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// <see cref="Plugin"/> subclass used to identify pipeline plugins.
    /// </summary>
    public sealed class PipelinePlugin : Plugin
    {
        /// <inheritDoc/>
        public override string IconFile
        {
            get {
                return Info.IconFile;
            }
            set {
                Info.IconFile = value;
                CallPropertyChanged("IconFile");
            }
        }

        /// <summary>
        /// Link to the <see cref="PipelinePluginInfo"/> describing this plugin.
        /// </summary>
        public PipelinePluginInfo Info
        {
            get;
            private set;
        }
        
        /// <summary>
        /// Creates a new pipeline plugin bean.
        /// </summary>
        /// <param name="id">Plugin unique ID.</param>
        /// <param name="description">Plugin description.</param>
        /// <param name="info"><see cref="PipelinePluginInfo"/> bean describing
        /// this plugin. Cannot be <c>null</c>.</param>
        public PipelinePlugin(Guid id, string description, PipelinePluginInfo info)
            : base(id, description)
        {
            Debug.Assert(info != null);
            Info = info;
        }
        
        /// <inheritDoc/>
        public override string GetPreview(UserSettings userSettings)
        {
            Debug.Assert(userSettings != null);

            // Since this plugin might have been modified from its official
            // version (or it could not exist at all), we'll create a temp
            // copy, save it to user settings and ask the PCCExecutor to use
            // this temp copy to get the preview. We also do not cache the
            // preview since we could change during the lifetime of the app.
            PipelinePluginInfo tempInfo = Info.CreateTemp();
            using (new TempPipelinePluginSaver(tempInfo, userSettings)) {
                return new PCCExecutor().GetPathWithPlugin(tempInfo.Id, Plugin.PREVIEW_PATH);
            }
        }
        
        /// <summary>
        /// Class intended to be used to save a temporary <see cref="PipelinePluginInfo"/>
        /// to <see cref="UserSettings"/>, deleting it when the object is disposed.
        /// </summary>
        private sealed class TempPipelinePluginSaver : IDisposable
        {
            /// Pipeline plugin info to save and delete.
            private PipelinePluginInfo pluginInfo;

            /// Object to access user settings.
            private UserSettings settings;
            
            /// <summary>
            /// Constructor.
            /// </summary>
            /// <param name="pluginInfo"><see cref="PipelinePluginInfo"/> to save.</param>
            /// <param name="settings">Object to access user settings.</param>
            public TempPipelinePluginSaver(PipelinePluginInfo pluginInfo, UserSettings settings)
            {
                Debug.Assert(pluginInfo != null);
                Debug.Assert(settings != null);

                this.pluginInfo = pluginInfo;
                this.settings = settings;

                this.settings.SaveTempPipelinePlugin(this.pluginInfo);
            }
            
            /// <summary>
            /// Finalizer. Deletes the temp pipeline plugin from settings if object
            /// hasn't been properly disposed of.
            /// </summary>
            ~TempPipelinePluginSaver()
            {
                settings.DeleteTempPipelinePlugin(pluginInfo);
            }
            
            /// <inheritDoc/>
            public void Dispose()
            {
                settings.DeleteTempPipelinePlugin(pluginInfo);
                GC.SuppressFinalize(this);
            }
        }
    }
    
    /// <summary>
    /// Wrapper for info about a pipeline plugin found in the registry.
    /// </summary>
    public sealed class PipelinePluginInfo : IComparable<PipelinePluginInfo>, IComparable,
        IEquatable<PipelinePluginInfo>
    {
        /// <summary>
        /// Default version used for the required version when none is specified via the
        /// <see cref="PipelinePluginInfo.RequiredVersion"/> property. This corresponds to
        /// the first version of Path Copy Copy that supported pipeline plugins.
        /// </summary>
        public static readonly Version DEFAULT_REQUIRED_VERSION = new Version(9, 0, 0, 0);

        /// <summary>
        /// Default version used for the required version, as a string.
        /// </summary>
        /// <seealso cref="PipelinePluginInfo.DEFAULT_REQUIRED_VERSION"/>
        public const string DEFAULT_REQUIRED_VERSION_AS_STRING = "9.0.0.0";

        /// Minimum required version to use this pipeline plugin.
        private Version minVersion = DEFAULT_REQUIRED_VERSION;

        /// <summary>
        /// ID of this pipeline plugin.
        /// </summary>
        [XmlIgnore]
        public Guid Id
        {
            get;
            set;
        }

        /// <summary>
        /// ID of this pipeline plugin, as a string. Used for XML serialization.
        /// </summary>
        /// <seealso cref="Id"/>
        [XmlElement("Id")]
        public string IdAsString
        {
            get {
                return Id.ToString("B");
            }
            set {
                Id = new Guid(value);
            }
        }

        /// <summary>
        /// Description associated with the pipeline plugin.
        /// </summary>
        [XmlElement]
        public string Description
        {
            get;
            set;
        }

        /// <summary>
        /// String containing encoded pipeline elements.
        /// </summary>
        [XmlElement("Pipeline")]
        public string EncodedElements
        {
            get;
            set;
        }

        /// <summary>
        /// Path to icon file to use for this plugin.
        /// </summary>
        /// <remarks>
        /// Because this points to a local file, this value is not saved in XML.
        /// Exported pipeline plugins will never have icon files.
        /// </remarks>
        [XmlIgnore]
        public string IconFile
        {
            get;
            set;
        }

        /// <summary>
        /// Whether this pipeline plugin is stored in the global key.
        /// Global plugins cannot be deleted or modified.
        /// </summary>
        [XmlElement]
        public bool Global
        {
            get;
            set;
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline plugin.
        /// This can be used when importing to avoid importing plugins we won't support.
        /// </summary>
        /// <remarks>
        /// If this is set to <c>null</c>, the default required Version will be used,
        /// which corresponds to the first version of Path Copy Copy that supported pipeline plugins.
        /// </remarks>
        /// <seealso cref="DEFAULT_REQUIRED_VERSION"/>
        [XmlIgnore]
        public Version RequiredVersion
        {
            get {
                return minVersion;
            }
            set {
                minVersion = value ?? DEFAULT_REQUIRED_VERSION;
            }
        }

        /// <summary>
        /// Value of <see cref="PipelinePluginInfo.RequiredVersion"/> as a string.
        /// Used for XML serialization.
        /// </summary>
        /// <remarks>
        /// If this is set to <c>null</c> or an empty string, the default required Version will be used,
        /// which corresponds to the first version of Path Copy Copy that supported pipeline plugins.
        /// </remarks>
        /// <seealso cref="DEFAULT_REQUIRED_VERSION"/>
        [XmlElement("RequiredVersion"), DefaultValue(DEFAULT_REQUIRED_VERSION_AS_STRING)]
        public string RequiredVersionAsString
        {
            get {
                return RequiredVersion.ToString();
            }
            set {
                RequiredVersion = !String.IsNullOrEmpty(value) ? new Version(value) : DEFAULT_REQUIRED_VERSION;
            }
        }

        /// <summary>
        /// Checks if this pipeline plugin's <see cref="RequiredVersion"/> is equal to or less than
        /// the current Path Copy Copy version.
        /// </summary>
        [XmlIgnore]
        public bool Compatible
        {
            get {
                return Assembly.GetExecutingAssembly().GetName().Version.CompareTo(RequiredVersion) >= 0;
            }
        }
        
        /// <summary>
        /// Constructor with no arguments.
        /// </summary>
        public PipelinePluginInfo()
        {
        }
        
        /// <summary>
        /// Constructor with information.
        /// </summary>
        /// <param name="id">ID of the pipeline plugin.</param>
        /// <param name="description">Pipeline plugin description.</param>
        /// <param name="encodedElements">String containing encoded pipeline
        /// elements.</param>
        /// <param name="iconFile">Path to icon file to use for plugin, or an empty
        /// string to use the default icon, or <c>null</c> to avoid showing an icon.</param>
        /// <param name="isGlobal">Whether this plugin is in the global key.</param>
        /// <param name="minVersion">Minimum required version to use plugin.</param>
        public PipelinePluginInfo(Guid id, string description, string encodedElements,
            string iconFile, bool isGlobal, Version minVersion)
        {
            Id = id;
            Description = description;
            EncodedElements = encodedElements;
            IconFile = iconFile;
            Global = isGlobal;
            RequiredVersion = minVersion;
        }
        
        /// <summary>
        /// Returns a textual representation of the plugin info (e.g. its description).
        /// </summary>
        /// <returns>Plugin info description.</returns>
        /// <remarks>If this pipeline plugin's required version is higher than
        /// the current Path Copy Copy version, the description will include the
        /// minimum required version for the pipeline plugin.</remarks>
        public override string ToString()
        {
            string description;
            if (Compatible) {
                description = Description;
            } else {
                description = String.Format(Resources.PipelinePluginInfo_DescriptionWithRequiredVersion,
                    Description, RequiredVersionAsString);
            }
            return description;
        }
        
        /// <summary>
        /// Converts this pipeline plugin info into a plugin wrapper.
        /// </summary>
        /// <returns>Plugin wrapper. Really a <see cref="PipelinePlugin"/>.</returns>
        public Plugin ToPlugin()
        {
            return new PipelinePlugin(Id, Description, this);
        }
        
        /// <summary>
        /// Utility method to convert a list of pipeline plugin infos into plugins.
        /// </summary>
        /// <param name="pluginInfos">List of pipeline plugin infos.</param>
        /// <returns>List of plugins. Really contains
        /// <see cref="PipelinePlugin"/>s.</returns>
        public static List<Plugin> ToPlugins(List<PipelinePluginInfo> pluginInfos)
        {
            Debug.Assert(pluginInfos != null);
            return pluginInfos.ConvertAll(info => info.ToPlugin());
        }
        
        /// <summary>
        /// Creates a temporary copy of this <see cref="PipelinePluginInfo"/>.
        /// All fields will be the same as <c>this</c>, except for the
        /// <see cref="Id"/> which will be randomly-generated.
        /// </summary>
        /// <returns>New <see cref="PipelinePluginInfo"/> instance.</returns>
        public PipelinePluginInfo CreateTemp()
        {
            PipelinePluginInfo temp = (PipelinePluginInfo) MemberwiseClone();
            temp.Id = Guid.NewGuid();
            return temp;
        }
        
        /// <summary>
        /// Compares this pipeline plugin info with another. They are compared
        /// through their <see cref="PipelinePluginInfo.Id"/>.
        /// </summary>
        /// <param name="other">Other pipeline plug to compare ourselves to.</param>
        /// <returns>Relative position of <c>this</c> compared to
        /// <paramref name="other"/>.</returns>
        public int CompareTo(PipelinePluginInfo other)
        {
            return other != null ? Id.CompareTo(other.Id) : 1;
        }
        
        /// <summary>
        /// Compares this pipeline plugin info with another. They are compared
        /// through their <see cref="PipelinePluginInfo.Id"/>. This is the generic
        /// version for the <see cref="T:IComparable"/> interface.
        /// </summary>
        /// <param name="obj">Other pipeline plug to compare ourselves to.</param>
        /// <returns>Relative position of <c>this</c> compared to
        /// <paramref name="obj"/>.</returns>
        int IComparable.CompareTo(object obj)
        {
            return (obj != null && obj is PipelinePluginInfo) ? CompareTo((PipelinePluginInfo) obj) : 1;
        }
        
        /// <summary>
        /// Compares two pipeline plugin infos for equality. They are compared
        /// through their <see cref="PipelinePluginInfo.Id"/>.
        /// </summary>
        /// <param name="other">Pipeline plugin info to compare ourselves with.</param>
        /// <returns>Whether <c>this</c> is equal to <paramref name="other"/>.</returns>
        public bool Equals(PipelinePluginInfo other)
        {
            return other != null ? Id.Equals(other.Id) : false;
        }
    }
    
    /// <summary>
    /// Collection of <see cref="T:PipelinePluginInfo"/> objects. Can be used
    /// to save and load pipeline plugins to and from XML.
    /// </summary>
    [XmlRoot(Namespace = PIPELINE_PLUGINS_XML_NAMESPACE)]
    public sealed class PipelinePluginCollection
    {
        /// <summary>
        /// XML namespace used to serialize pipeline plugins.
        /// </summary>
        public const string PIPELINE_PLUGINS_XML_NAMESPACE = "http://pathcopycopy.codeplex.com/xsd/PipelinePlugins/V1";

        /// XmlSerializer used for regular XML serialization/deserialization.
        private static XmlSerializer xmlSerializer;

        /// XmlSerializer used for legacy XML serialization. Does not serialize required versions of pipeline plugins.
        private static XmlSerializer legacyXmlSerializer;

        /// Object used to protect creation of XmlSerializer objects.
        private static readonly object xmlSerializersLock = new Object();

        /// List storing the pipeline plugins in the collection.
        private List<PipelinePluginInfo> infos = new List<PipelinePluginInfo>();

        /// <summary>
        /// Pipeline plugins in the collection.
        /// </summary>
        [XmlArray]
        public List<PipelinePluginInfo> Plugins
        {
            get {
                return infos;
            }
            set {
                infos = value;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline plugin collection.
        /// This corresponds to the highest required version of all plugins in the collection.
        /// </summary>
        [XmlIgnore]
        public Version RequiredVersion
        {
            get {
                Version reqdVersion = PipelinePluginInfo.DEFAULT_REQUIRED_VERSION;
                foreach (PipelinePluginInfo info in infos) {
                    if (info.RequiredVersion.CompareTo(reqdVersion) > 0) {
                        reqdVersion = info.RequiredVersion;
                    }
                }
                return reqdVersion;
            }
        }

        /// <summary>
        /// Checks whether all pipeline plugins in the collection are <see cref="PipelinePluginInfo.Compatible"/>.
        /// </summary>
        [XmlIgnore]
        public bool Compatible
        {
            get {
                bool compatible = true;
                for (int i = 0; compatible && i < infos.Count; ++i) {
                    compatible = infos[i].Compatible;
                }
                return compatible;
            }
        }
        
        /// <summary>
        /// Serializes this pipeline plugins collection to XML format that will
        /// later be accepted by the <see cref="FromXML"/> method.
        /// </summary>
        /// <param name="stream"><see cref="T:Stream"/> where to store the XML data.</param>
        /// <param name="legacy">Whether to use the legacy <see cref="XmlSerializer"/>.
        /// If <c>true</c>, required versions won't be written for pipeline plugins.</param>
        public void ToXML(Stream stream, bool legacy)
        {
            Debug.Assert(stream != null);

            using (XmlWriter writer = XmlWriter.Create(stream)) {
                XmlSerializer serializer = legacy ? GetLegacyXmlSerializer() : GetXmlSerializer();
                serializer.Serialize(writer, this);
            }
        }
        
        /// <summary>
        /// Deserializes an <see cref="T:PipelinePluginCollection"/> object from
        /// XML data.
        /// </summary>
        /// <param name="stream"><see cref="T:Stream"/> containing the XML.</param>
        /// <returns>New pipeline plugins collection from XML.</returns>
        public static PipelinePluginCollection FromXML(Stream stream)
        {
            Debug.Assert(stream != null);

            using (XmlReader reader = XmlReader.Create(stream)) {
                return (PipelinePluginCollection) GetXmlSerializer().Deserialize(reader);
            }
        }
        
        /// <summary>
        /// Returns the <see cref="XmlSerializer"/> to use for regular XML
        /// serialization/deserialization.
        /// </summary>
        /// <returns>Global <see cref="XmlSerializer"/> instance.</returns>
        private static XmlSerializer GetXmlSerializer()
        {
            // Create XmlSerializer on the first call.
            if (xmlSerializer == null) {
                lock (xmlSerializersLock) {
                    if (xmlSerializer == null) {
                        xmlSerializer = new XmlSerializer(typeof(PipelinePluginCollection), PIPELINE_PLUGINS_XML_NAMESPACE);
                    }
                }
            }
            return xmlSerializer;
        }
        
        /// <summary>
        /// Returns the <see cref="XmlSerializer"/> to use for legacy serialization.
        /// This XmlSerializer will not output required versions for pipeline plugins.
        /// </summary>
        /// <returns>Global legacy <see cref="XmlSerializer"/> instance.</returns>
        private static XmlSerializer GetLegacyXmlSerializer()
        {
            // Create XmlSerializer on the first call.
            if (legacyXmlSerializer == null) {
                lock (xmlSerializersLock) {
                    if (legacyXmlSerializer == null) {
                        // Ignore the "RequiredVersionAsString" property of PipelinePluginInfo.
                        XmlAttributeOverrides overrides = new XmlAttributeOverrides();
                        XmlAttributes attributes = new XmlAttributes();
                        attributes.XmlIgnore = true;
                        overrides.Add(typeof(PipelinePluginInfo), "RequiredVersionAsString", attributes);
                        legacyXmlSerializer = new XmlSerializer(typeof(PipelinePluginCollection),
                            overrides, new Type[0], null, PIPELINE_PLUGINS_XML_NAMESPACE);
                    }
                }
            }
            return legacyXmlSerializer;
        }
    }
    
    /// <summary>
    /// Base class for pipeline elements to be chained in a pipeline plugin.
    /// </summary>
    abstract public class PipelineElement
    {
        /// <summary>
        /// Every pipeline element type must be represented by a specific code.
        /// The pipeline uses this code to encode its elements.
        /// </summary>
        abstract public char Code
        {
            get;
        }

        /// <summary>
        /// Every pipeline element type must have a display value.
        /// It will be used when the element is displayed in Expert Mode in the UI.
        /// </summary>
        abstract public string DisplayValue
        {
            get;
        }

        /// <summary>
        /// Returns the minimum version of Path Copy Copy required to use this pipeline
        /// element. This will be used when re-importing plugins, to ensure we can warn the
        /// user of incompatible plugins.
        /// </summary>
        /// <remarks>
        /// By default, this property returns the first version of Path Copy Copy
        /// that supported pipeline plugins. Pipeline element types added after this
        /// should redefine this to the version they are introduced in.
        /// </remarks>
        public virtual Version RequiredVersion
        {
            get {
                return PipelinePluginInfo.DEFAULT_REQUIRED_VERSION;
            }
        }
        
        /// <summary>
        /// Method that must be overridden in subclasses to encode the data of
        /// the element in a string. This will be saved in the registry in the
        /// plugin's encoded elements.
        /// </summary>
        /// <param name="p_Param">Parameter summary</param>
        /// <returns>Return value summary</returns>
        abstract public string Encode();

        /// <summary>
        /// Returns a user control to edit this pipeline element. The control
        /// must be data-bound to the element somehow.
        /// </summary>
        /// <returns>User control instance.</returns>
        /// <remarks>
        /// A new control will be created every time this is called.
        /// User assumes "ownership" of the returned control.
        /// </remarks>
        public virtual UserControl GetEditingControl()
        {
            // By default, we return a user control that has no additional
            // configuration required. Subclasses that require custom
            // user controls must override this.
            return new ConfiglessPipelineElementUserControl();
        }
        
        /// <summary>
        /// Encodes the specified int value in a format suitable to be included in
        /// encoded element data.
        /// </summary>
        /// <param name="toEncode">Int value to encode.</param>
        /// <returns>Encoded string.</returns>
        public static string EncodeInt(int toEncode)
        {
            // Encode as four characters.
            Debug.Assert(toEncode < 10000);
            return toEncode.ToString("D4");
        }
        
        /// <summary>
        /// Encodes the specified string in a format suitable to be included in
        /// encoded element data.
        /// </summary>
        /// <param name="toEncode">String to encode.</param>
        /// <returns>Encoded string.</returns>
        public static string EncodeString(string toEncode)
        {
            // First write the string length, then the string content.
            return EncodeInt(toEncode.Length) + toEncode;
        }
        
        /// <summary>
        /// Encodes the specified boolean in a format suitable to be included in
        /// encoded element data.
        /// </summary>
        /// <param name="toEncode">Bool to encode.</param>
        /// <returns>Encoded string.</returns>
        public static string EncodeBool(bool toEncode)
        {
            // Encode as 0 or 1.
            return toEncode ? "1" : "0";
        }
    }
    
    /// <summary>
    /// Class representing the pipeline used by a pipeline plugin. Contains an
    /// ordered list of pipeline elements.
    /// </summary>
    /// <remarks>
    /// Please refer to the C++ code in "PluginPipelineElements.cpp" and
    /// "PluginPipelineDecoder.cpp" to see the opposite side of the
    /// encoding/decoding work.</remarks>
    public sealed class Pipeline
    {
        /// List containing all pipeline elements.
        private List<PipelineElement> pipelineElements = new List<PipelineElement>();

        /// <summary>
        /// List containing all the pipeline elements.
        /// </summary>
        public List<PipelineElement> Elements
        {
            get {
                return pipelineElements;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline. This
        /// amounts to the highest required version of all its pipeline elements.
        /// </summary>
        public Version RequiredVersion
        {
            get {
                Version reqdVersion = PipelinePluginInfo.DEFAULT_REQUIRED_VERSION;
                foreach (PipelineElement element in pipelineElements) {
                    if (element.RequiredVersion.CompareTo(reqdVersion) > 0) {
                        reqdVersion = element.RequiredVersion;
                    }
                }
                return reqdVersion;
            }
        }
        
        /// <summary>
        /// Encodes the pipeline's elements in a string to be saved in the plugin data.
        /// </summary>
        /// <returns>Encoded pipeline elements.</returns>
        public string Encode()
        {
            // First write the number of elements in the pipeline as a two-char number.
            Debug.Assert(Elements.Count < 100);
            StringBuilder encodedElements = new StringBuilder();
            encodedElements.Append(Elements.Count.ToString("D2"));

            // Encode each pipeline element by first appending its code then
            // asking the element itself to encode its data.
            foreach (PipelineElement element in Elements) {
                encodedElements.Append(element.Code);
                encodedElements.Append(element.Encode());
            }

            return encodedElements.ToString();
        }
        
        /// <summary>
        /// Clears the pipeline of all its elements.
        /// </summary>
        public void Clear()
        {
            pipelineElements.Clear();
        }
    }
    
    /// <summary>
    /// Exception class used to represent an invalid pipeline encoded string.
    /// </summary>
    public class InvalidPipelineException : Exception
    {
        /// <summary>
        /// Encoded elements in the invalid pipeline.
        /// </summary>
        public string EncodedElements
        {
            get;
            protected set;
        }
        
        /// <summary>
        /// Constructor.
        /// </summary>
        public InvalidPipelineException()
            : base("Invalid pipeline.")
        {
        }
        
        /// <summary>
        /// Constructor with encoded elements.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements.</param>
        public InvalidPipelineException(string encodedElements)
            : base(String.Format("Invalid pipeline: {0}", encodedElements))
        {
            EncodedElements = encodedElements;
        }
        
        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements.</param>
        /// <param name="innerException">Inner exception object.</param>
        public InvalidPipelineException(string encodedElements, Exception innerException)
            : base(String.Format("Invalid pipeline: {0}", encodedElements), innerException)
        {
            EncodedElements = encodedElements;
        }
    }
    
    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to add quotes around
    /// the returned path.
    /// </summary>
    public class QuotesPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '"';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_Quotes;
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }

    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to optionally add quotes
    /// around the returned path if it contains spaces.
    /// </summary>
    public class OptionalQuotesPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'q';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_OptionalQuotes;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(16, 0, 0, 0);
            }
        }

        /// <summary>
        /// Encodes this pipeliine element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }
    
    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to turn the path into
    /// an e-mail link by surrounding it with &lt; and &gt;.
    /// </summary>
    public class EmailLinksPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '<';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_EmailLinks;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(11, 1, 0, 0);
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }

    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to encode
    /// whitespace in an URI using percent-encoding.
    /// </summary>
    public class EncodeURIWhitespacePipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 's';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_EncodeURIWhitespace;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(13, 0, 0, 0);
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }
    
    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to encode characters in
    /// the path that are invalid in an URI using percent-encoding.
    /// </summary>
    public class EncodeURICharsPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '%';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_EncodeURIChars;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(12, 0, 0, 0);
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }
    
    /// <summary>
    /// Pipeline element that replaces all backslashes found in the path with
    /// forward slashes.
    /// </summary>
    public class BackToForwardSlashesPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '\\';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_BackToForwardSlashes;
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }
    
    /// <summary>
    /// Pipeline element that replaces all forward slashes found in the path
    /// with backslashes.
    /// </summary>
    public class ForwardToBackslashesPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '/';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_ForwardToBackslashes;
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }

    /// <summary>
    /// Pipeline element that removes any extension from the file at
    /// the end of the path.
    /// </summary>
    public class RemoveExtPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '.';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_RemoveExt;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(17, 0, 0, 0);
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return String.Empty;
        }
    }
    
    /// <summary>
    /// Pipeline element that performs a find & replace operation in the path.
    /// </summary>
    public class FindReplacePipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '?';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_FindReplace;
            }
        }

        /// <summary>
        /// Value to look for in the path.
        /// </summary>
        /// <seealso cref="NewValue"/>
        public string OldValue
        {
            get;
            set;
        }

        /// <summary>
        /// Replacement value.
        /// </summary>
        /// <seealso cref="OldValue"/>
        public string NewValue
        {
            get;
            set;
        }
        
        /// <summary>
        /// Default constructor.
        /// </summary>
        public FindReplacePipelineElement()
        {
        }
        
        /// <summary>
        /// Constructor with old and new values.
        /// </summary>
        /// <param name="oldValue">Value to look for.</param>
        /// <param name="newValue">Replacement value.</param>
        public FindReplacePipelineElement(string oldValue, string newValue)
        {
            OldValue = oldValue;
            NewValue = newValue;
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode old and new value one after the other.
            return EncodeString(OldValue) + EncodeString(NewValue);
        }
    }
    
    /// <summary>
    /// Pipeline element that performs find/replace operations using regex.
    /// </summary>
    public class RegexPipelineElement : PipelineElement
    {
        /// <summary>
        /// Version number used to identify encoded data.
        /// </summary>
        public const int INITIAL_VERSION = 1;

        /// <summary>
        /// Maximum data version understood by this code.
        /// </summary>
        public const int MAX_VERSION = INITIAL_VERSION;

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '^';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_Regex;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(10, 0, 0, 0);
            }
        }

        /// <summary>
        /// Regular expression to use to find matches.
        /// </summary>
        /// <remarks>
        /// The shell extension doesn't use the .NET regex syntax since it
        /// uses the std::regex class to perform matches. It uses the
        /// ECMAScript regex syntax.
        /// </remarks>
        public string Regex
        {
            get;
            set;
        }

        /// <summary>
        /// Format of replacement string.
        /// </summary>
        public string Format
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to ignore case when looking for matches.
        /// </summary>
        public bool IgnoreCase
        {
            get;
            set;
        }
        
        /// <summary>
        /// Default constructor.
        /// </summary>
        public RegexPipelineElement()
        {
        }
        
        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="regex">Regular expression to use.</param>
        /// <param name="format">Format of replacement string.</param>
        /// <param name="ignoreCase">Whether to ignore case when looking for
        /// matches.</param>
        public RegexPipelineElement(string regex, string format, bool ignoreCase)
        {
            Regex = regex;
            Format = format;
            IgnoreCase = ignoreCase;
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // First write currently-used version number.
            StringBuilder encoder = new StringBuilder();
            encoder.Append(EncodeInt(INITIAL_VERSION));

            // Now encode regex, format string and ignore case flag.
            encoder.Append(EncodeString(Regex));
            encoder.Append(EncodeString(Format));
            encoder.Append(EncodeBool(IgnoreCase));

            return encoder.ToString();
        }
    }
    
    /// <summary>
    /// Pipeline element that applies the effect of another plugin on the path.
    /// </summary>
    public class ApplyPluginPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '{';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_ApplyPlugin;
            }
        }

        /// <summary>
        /// ID of plugin to apply to the path.
        /// </summary>
        public Guid PluginID
        {
            get;
            set;
        }
        
        /// <summary>
        /// Default constructor.
        /// </summary>
        public ApplyPluginPipelineElement()
        {
        }
        
        /// <summary>
        /// Constructor with plugin ID.
        /// </summary>
        /// <param name="pluginId">ID of plugin to apply to the path.</param>
        public ApplyPluginPipelineElement(Guid pluginId)
        {
            PluginID = pluginId;
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // We encode our guid without length info, since the C++
            // side will know how many characters to read.
            return PluginID.ToString("B");
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but changes the
    /// separator used between multiple copied paths.
    /// </summary>
    public class PathsSeparatorPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = ',';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_PathsSeparator;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(13, 0, 0, 0);
            }
        }

        /// <summary>
        /// Separator to use between multiple paths.
        /// </summary>
        public string PathsSeparator
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public PathsSeparatorPipelineElement()
        {
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="pathsSeparator">Separator to use between multiple paths.</param>
        public PathsSeparatorPipelineElement(string pathsSeparator)
        {
            PathsSeparator = pathsSeparator;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode the paths separator.
            return EncodeString(PathsSeparator);
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to launch an executable with path(s) as argument
    /// instead of copying them to the clipboard.
    /// </summary>
    public class ExecutablePipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'x';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_Executable;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(15, 0, 0, 0);
            }
        }

        /// <summary>
        /// Path to executable to launch with path(s) as argument.
        /// </summary>
        public string Executable
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public ExecutablePipelineElement()
        {
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public ExecutablePipelineElement(string executable)
        {
            Executable = executable;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode the executable path.
            return EncodeString(Executable);
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to launch an executable with filelist as argument
    /// instead of copying them to the clipboard.
    /// </summary>
    public class ExecutableWithFilelistPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'f';

        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public override char Code
        {
            get {
                return CODE;
            }
        }

        /// <summary>
        /// Pipeline element display value for the UI.
        /// </summary>
        public override string DisplayValue
        {
            get {
                return Resources.PipelineElement_ExecutableWithFilelist;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(17, 0, 0, 0);
            }
        }

        /// <summary>
        /// Path to executable to launch with filelist as argument.
        /// </summary>
        public string Executable
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public ExecutableWithFilelistPipelineElement()
        {
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public ExecutableWithFilelistPipelineElement(string executable)
        {
            Executable = executable;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode the executable path.
            return EncodeString(Executable);
        }
    }

    /// <summary>
    /// Static class that can decode a pipeline of multiple elements from an
    /// encoded string. This is the C# equivalent of the C++'s PipelineDecoder.
    /// </summary>
    public static class PipelineDecoder
    {
        /// <summary>
        /// Decodes the content of a pipeline using encoded element data.
        /// </summary>
        /// <param name="encodedElements">Encoded elements data.</param>
        /// <returns>New <see cref="Pipeline"/> object.</returns>
        /// <exception cref="InvalidPipelineException">Thrown when the content of
        /// <paramref name="encodedElements"/> contain invalid data.</exception>
        public static Pipeline DecodePipeline(string encodedElements)
        {
            // Initialize new pipeline object.
            Pipeline pipeline = new Pipeline();

            try {
                // The first two characters of the string should be the number of elements.
                if (encodedElements.Length < 2) {
                    throw new InvalidPipelineException();
                }
                int numElements = Int32.Parse(encodedElements.Substring(0, 2));

                // Loop to read each element.
                int curChar = 2;
                for (int i = 0; i < numElements; ++i) {
                    pipeline.Elements.Add(DecodeElement(encodedElements, ref curChar));
                }
                if (curChar < encodedElements.Length) {
                    throw new InvalidPipelineException();
                }
            } catch (Exception e) {
                // Re-throw an exception with encoded elements info.
                throw new InvalidPipelineException(encodedElements, e);
            }

            return pipeline;
        }
        
        /// <summary>
        /// Decodes a pipeline element at a specific point in an encoded string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is located.
        /// Upon return, this value will point just after the element data.</param>
        /// <returns>New <see cref="PipelineElement"/> instance.</returns>
        private static PipelineElement DecodeElement(string encodedElements, ref int curChar)
        {
            PipelineElement element = null;

            // First read the element code.
            if (curChar == encodedElements.Length) {
                throw new InvalidPipelineException();
            }
            char elementCode = encodedElements[curChar++];
            switch (elementCode) {
                case QuotesPipelineElement.CODE: {
                    element = new QuotesPipelineElement();
                    break;
                }
                case OptionalQuotesPipelineElement.CODE: {
                    element = new OptionalQuotesPipelineElement();
                    break;
                }
                case EmailLinksPipelineElement.CODE: {
                    element = new EmailLinksPipelineElement();
                    break;
                }
                case EncodeURIWhitespacePipelineElement.CODE: {
                    element = new EncodeURIWhitespacePipelineElement();
                    break;
                }
                case EncodeURICharsPipelineElement.CODE: {
                    element = new EncodeURICharsPipelineElement();
                    break;
                }
                case BackToForwardSlashesPipelineElement.CODE: {
                    element = new BackToForwardSlashesPipelineElement();
                    break;
                }
                case ForwardToBackslashesPipelineElement.CODE: {
                    element = new ForwardToBackslashesPipelineElement();
                    break;
                }
                case RemoveExtPipelineElement.CODE: {
                    element = new RemoveExtPipelineElement();
                    break;
                }
                case FindReplacePipelineElement.CODE: {
                    element = DecodeFindReplaceElement(encodedElements, ref curChar);
                    break;
                }
                case RegexPipelineElement.CODE: {
                    element = DecodeRegexPipelineElement(encodedElements, ref curChar);
                    break;
                }
                case ApplyPluginPipelineElement.CODE: {
                    element = DecodeApplyPluginElement(encodedElements, ref curChar);
                    break;
                }
                case PathsSeparatorPipelineElement.CODE: {
                    element = DecodePathsSeparatorElement(encodedElements, ref curChar);
                    break;
                }
                case ExecutablePipelineElement.CODE:
                case ExecutableWithFilelistPipelineElement.CODE: {
                    element = DecodeExecutableElement(elementCode, encodedElements, ref curChar);
                    break;
                }
                default:
                    // Invalid pipeline. PCC downgrade, maybe?
                    throw new InvalidPipelineException();
            }

            Debug.Assert(element != null);
            return element;
        }
        
        /// <summary>
        /// Decodes a <see cref="FindReplacePipelineElement"/> from an encoded
        /// element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static FindReplacePipelineElement DecodeFindReplaceElement(string encodedElements,
            ref int curChar)
        {
            // The element data contains the old and new value.
            string oldValue = DecodeString(encodedElements, ref curChar);
            string newValue = DecodeString(encodedElements, ref curChar);
            return new FindReplacePipelineElement(oldValue, newValue);
        }
        
        /// <summary>
        /// Decodes a <see cref="RegexPipelineElement"/> from an encoded element
        /// string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static RegexPipelineElement DecodeRegexPipelineElement(string encodedElements,
            ref int curChar)
        {
            // First read version number and validate.
            int version = DecodeInt(encodedElements, ref curChar);
            if (version > RegexPipelineElement.MAX_VERSION) {
                throw new InvalidPipelineException();
            }

            // Read initial version data: regex, format and ignore case flag.
            string regex = DecodeString(encodedElements, ref curChar);
            string format = DecodeString(encodedElements, ref curChar);
            bool ignoreCase = DecodeBool(encodedElements, ref curChar);

            // Create and return element object.
            return new RegexPipelineElement(regex, format, ignoreCase);
        }
        
        /// <summary>
        /// Decodes an <see cref="ApplyPluginPipelineElement"/> from an encoded
        /// element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static ApplyPluginPipelineElement DecodeApplyPluginElement(string encodedElements,
            ref int curChar)
        {
            // The element data is the ID of the plugin to apply. There's no length saved
            // for this since it's always the same length. Format is as follows:
            // {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
            int guidLength = Guid.Empty.ToString("B").Length;
            if (curChar > (encodedElements.Length - guidLength)) {
                throw new InvalidPipelineException();
            }
            Guid pluginId = new Guid(encodedElements.Substring(curChar, guidLength));
            curChar += guidLength;
            return new ApplyPluginPipelineElement(pluginId);
        }

        /// <summary>
        /// Decodes a <see cref="PathsSeparatorPipelineElement"/> from an encoded
        /// element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static PathsSeparatorPipelineElement DecodePathsSeparatorElement(
            string encodedElements, ref int curChar)
        {
            // The element data contains the paths separator.
            string pathsSeparator = DecodeString(encodedElements, ref curChar);
            return new PathsSeparatorPipelineElement(pathsSeparator);
        }

        /// <summary>
        /// Decodes an <see cref="ExecutablePipelineElement"/> or
        /// <see cref="ExecutableWithFilelistPipelineElement"/> from
        /// an encoded element string.
        /// </summary>
        /// <param name="elementCode">Element code.</param>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static PipelineElement DecodeExecutableElement(
            char elementCode, string encodedElements, ref int curChar)
        {
            // The element data contains the executable path.
            string executable = DecodeString(encodedElements, ref curChar);
            switch (elementCode) {
                case ExecutablePipelineElement.CODE: {
                    return new ExecutablePipelineElement(executable);
                }
                case ExecutableWithFilelistPipelineElement.CODE: {
                    return new ExecutableWithFilelistPipelineElement(executable);
                }
                default:
                    throw new InvalidPipelineException();
            }
        }
        
        /// <summary>
        /// Decodes an integer value from an encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the integer data is to be found
        /// in <paramref name="encodedElements"/>. Upon return, this will point
        /// just after the integer data.</param>
        /// <returns>Decoded integer value.</returns>
        private static int DecodeInt(string encodedElements, ref int curChar)
        {
            // Encoded as four characters.
            if (curChar > (encodedElements.Length - 4)) {
                throw new InvalidPipelineException();
            }
            int intValue = int.Parse(encodedElements.Substring(curChar, 4));
            curChar += 4;
            return intValue;
        }
        
        /// <summary>
        /// Decodes a string from an encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the string data is to be found
        /// in <paramref name="encodedElements"/>. Upon return, this will point
        /// just after the string data.</param>
        /// <returns>Decoded string.</returns>
        private static string DecodeString(string encodedElements, ref int curChar)
        {
            // Read string length.
            int stringLength = DecodeInt(encodedElements, ref curChar);

            // Return string content.
            if (curChar > (encodedElements.Length - stringLength)) {
                throw new InvalidPipelineException();
            }
            int prevChar = curChar;
            curChar += stringLength;
            return encodedElements.Substring(prevChar, stringLength);
        }
        
        /// <summary>
        /// Decodes a boolean from an encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the boolean data is to be found
        /// in <paramref name="encodedElements"/>. Upon return, this will point
        /// just after the boolean data.</param>
        /// <returns>Decoded boolean value.</returns>
        private static bool DecodeBool(string encodedElements, ref int curChar)
        {
            // Encoded as 0 or 1.
            if (curChar == encodedElements.Length) {
                throw new InvalidPipelineException();
            }
            if (encodedElements[curChar] != '0' && encodedElements[curChar] != '1') {
                throw new InvalidPipelineException();
            }
            return encodedElements[curChar++] == '1';
        }
    }
    
    /// <summary>
    /// Container of information used when importing pipeline plugins. It will
    /// store information about plugins overwriting existing ones.
    /// </summary>
    public sealed class ImportedPipelinePluginOverwrites
    {
        /// Dictionary storing overwrite information.
        private Dictionary<PipelinePluginInfo, OverwriteInfo> overwriteInfos = new Dictionary<PipelinePluginInfo, OverwriteInfo>();

        /// <summary>
        /// Dictionary storing information about overwritten plugins. The key
        /// is the new pipeline plugin infos and the value is the container
        /// containing information about the old plugin being overwritten.
        /// </summary>
        /// <seealso cref="T:ImportedPipelinePluginOverwrites.OverwriteInfo"/>
        public Dictionary<PipelinePluginInfo, OverwriteInfo> OverwriteInfos
        {
            get {
                return overwriteInfos;
            }
        }
        
        /// <summary>
        /// Information about an overwritten pipeline plugin.
        /// </summary>
        public sealed class OverwriteInfo
        {
            /// <summary>
            /// Reference to the old pipeline plugin info that will be overwritten.
            /// </summary>
            public PipelinePluginInfo OldInfo
            {
                get;
                private set;
            }

            /// <summary>
            /// Index of the pipeline plugin that will be overwritten in the list
            /// of existing pipeline plugins.
            /// </summary>
            public int OldIndex
            {
                get;
                private set;
            }
            
            /// <summary>
            /// Constructor.
            /// </summary>
            /// <param name="oldInfo">Reference to old pipeline plugin info.</param>
            /// <param name="oldIndex">Index of old pipeline plugin in the list of
            /// existing pipeline plugins.</param>
            public OverwriteInfo(PipelinePluginInfo oldInfo, int oldIndex)
            {
                Debug.Assert(oldInfo != null);

                this.OldInfo = oldInfo;
                this.OldIndex = oldIndex;
            }
        }
    }
}
