﻿// PipelinePlugins.cs
// (c) 2011-2021, Charles Lechasseur
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
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text;
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
                CallPropertyChanged(nameof(IconFile));
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
            Info = info ?? throw new ArgumentNullException(nameof(info));
        }
        
        /// <inheritDoc/>
        public override string GetPreview(UserSettings userSettings)
        {
            if (userSettings == null) {
                throw new ArgumentNullException(nameof(userSettings));
            }

            // Since this plugin might have been modified from its official
            // version (or it could not exist at all), we'll save it as a
            // temp pipeline plugin, which will override the real one (if any).
            // We also do not cache the preview since we could change during
            // the lifetime of the app.
            using (new TempPipelinePluginSaver(Info, userSettings)) {
                return new PCCExecutor().GetPathWithPlugin(Id, PreviewPath, true);
            }
        }
    }

    /// <summary>
    /// Possible edit modes for a pipeline plugin.
    /// </summary>
    public enum PipelinePluginEditMode
    {
        /// <summary>
        /// This pipeline plugin was edited in Simple mode.
        /// </summary>
        Simple = 1,

        /// <summary>
        /// This pipeline plugin was edited in Expert mode.
        /// </summary>
        Expert = 2,
    }

    /// <summary>
    /// Enum of different versions of the pipeline plugin XML serialization process.
    /// Will be updated every time the serialization supports something new.
    /// </summary>
    public enum PipelinePluginXmlSerializerVersion
    {
        /// <summary>
        /// First version. Does not serialize RequiredVersion or EditMode.
        /// </summary>
        V1 = 1,

        /// <summary>
        /// Second version. Does not serialize EditMode.
        /// </summary>
        V2 = 2,

        /// <summary>
        /// Third and current version. Serializes everything.
        /// </summary>
        V3 = 3,
    }
    
    /// <summary>
    /// Wrapper for info about a pipeline plugin found in the registry.
    /// </summary>
    public sealed class PipelinePluginInfo : IComparable<PipelinePluginInfo>, IComparable,
        IEquatable<PipelinePluginInfo>
    {
        /// <summary>
        /// Default version used for the required version when none is specified via the
        /// <see cref="RequiredVersion"/> property. This corresponds to the first version
        /// of Path Copy Copy that supported pipeline plugins.
        /// </summary>
        public static readonly Version DefaultRequiredVersion = new Version(9, 0, 0, 0);

        /// <summary>
        /// Default version used for the required version, as a string.
        /// </summary>
        /// <seealso cref="DefaultRequiredVersion"/>
        public const string DefaultRequiredVersionAsString = "9.0.0.0";

        /// Minimum required version to use this pipeline plugin.
        private Version minVersion = DefaultRequiredVersion;

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
                return Id.ToString("B", CultureInfo.InvariantCulture);
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
        /// How the pipeline plugin was last edited in the UI.
        /// </summary>
        /// <remarks>
        /// If this is set to <c>null</c>, the proper edit mode is auto-detected.
        /// </remarks>
        [XmlIgnore]
        public PipelinePluginEditMode? EditMode
        {
            get;
            set;
        }

        /// <summary>
        /// Value of <see cref="EditMode"/> as a string.
        /// Used for XML serialization.
        /// </summary>
        /// <remarks>
        /// We serialize the enum value as a string because otherwise, xsd.exe
        /// generates a mandatory field in the schema, which is wrong.
        /// </remarks>
        [XmlElement("EditMode")]
        public string EditModeAsString
        {
            get {
                return EditMode.HasValue ? EditMode.Value.ToString() : null;
            }
            set {
                if (!string.IsNullOrEmpty(value)) {
                    EditMode = (PipelinePluginEditMode) Enum.Parse(typeof(PipelinePluginEditMode), value);
                } else {
                    EditMode = null;
                }
            }
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
        /// <seealso cref="DefaultRequiredVersion"/>
        [XmlIgnore]
        public Version RequiredVersion
        {
            get {
                return minVersion;
            }
            set {
                minVersion = value ?? DefaultRequiredVersion;
            }
        }

        /// <summary>
        /// Value of <see cref="RequiredVersion"/> as a string.
        /// Used for XML serialization.
        /// </summary>
        /// <remarks>
        /// If this is set to <c>null</c> or an empty string, the default required Version will be used,
        /// which corresponds to the first version of Path Copy Copy that supported pipeline plugins.
        /// </remarks>
        /// <seealso cref="DefaultRequiredVersion"/>
        [XmlElement("RequiredVersion"), DefaultValue(DefaultRequiredVersionAsString)]
        public string RequiredVersionAsString
        {
            get {
                return RequiredVersion.ToString();
            }
            set {
                RequiredVersion = !string.IsNullOrEmpty(value) ? new Version(value) : DefaultRequiredVersion;
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
        /// <param name="editMode">How the plugin was last edited in the UI.</param>
        /// <param name="isGlobal">Whether this plugin is in the global key.</param>
        /// <param name="minVersion">Minimum required version to use plugin.</param>
        public PipelinePluginInfo(Guid id, string description, string encodedElements,
            string iconFile, PipelinePluginEditMode? editMode, bool isGlobal, Version minVersion)
        {
            Id = id;
            Description = description;
            EncodedElements = encodedElements;
            IconFile = iconFile;
            EditMode = editMode;
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
                description = string.Format(CultureInfo.CurrentCulture,
                    Resources.PipelinePluginInfo_DescriptionWithRequiredVersion,
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
            if (pluginInfos == null) {
                throw new ArgumentNullException(nameof(pluginInfos));
            }
            return pluginInfos.ConvertAll(info => info.ToPlugin());
        }
        
        /// <summary>
        /// Creates a copy of this <see cref="PipelinePluginInfo"/>.
        /// All fields will be the same as <c>this</c>, except for the
        /// <see cref="Id"/> which will be randomly-generated.
        /// </summary>
        /// <returns>New <see cref="PipelinePluginInfo"/> instance.</returns>
        public PipelinePluginInfo Duplicate()
        {
            PipelinePluginInfo dup = (PipelinePluginInfo) MemberwiseClone();
            dup.Id = Guid.NewGuid();
            return dup;
        }
        
        /// <summary>
        /// Compares this pipeline plugin info with another. They are compared
        /// through their <see cref="Id"/>.
        /// </summary>
        /// <param name="other">Other pipeline plug to compare ourselves to.</param>
        /// <returns>Relative position of <c>this</c> compared to
        /// <paramref name="other"/>.</returns>
        public int CompareTo(PipelinePluginInfo other)
        {
            return other is object ? Id.CompareTo(other.Id) : 1;
        }
        
        /// <summary>
        /// Compares this pipeline plugin info with another. They are compared
        /// through their <see cref="Id"/>. This is the generic version for the
        /// <see cref="IComparable"/> interface.
        /// </summary>
        /// <param name="obj">Other pipeline plug to compare ourselves to.</param>
        /// <returns>Relative position of <c>this</c> compared to
        /// <paramref name="obj"/>.</returns>
        int IComparable.CompareTo(object obj)
        {
            return obj is PipelinePluginInfo info ? CompareTo(info) : 1;
        }
        
        /// <summary>
        /// Compares two pipeline plugin infos for equality. They are compared
        /// through their <see cref="Id"/>.
        /// </summary>
        /// <param name="other">Pipeline plugin info to compare ourselves with.</param>
        /// <returns>Whether <c>this</c> is equal to <paramref name="other"/>.</returns>
        public bool Equals(PipelinePluginInfo other)
        {
            return other is object && Id.Equals(other.Id);
        }

        /// <summary>
        /// Compares this pipeline plugin info with another object for equality.
        /// This only works if the other object is a <see cref="PipelinePluginInfo"/>.
        /// They are compared through their <see cref="Id"/>.
        /// </summary>
        /// <param name="obj">Object to compare ourselves with.</param>
        /// <returns>Whether <c>this</c> is equal to <paramref name="obj"/>.</returns>
        public override bool Equals(object obj)
        {
            return obj is PipelinePluginInfo info && Equals(info);
        }

        /// <summary>
        /// Returns a hash code for this object. In our case, corresponds
        /// with our <see cref="Id"/>'s hash code.
        /// </summary>
        /// <returns>Hash code.</returns>
        public override int GetHashCode()
        {
            return Id.GetHashCode();
        }

        /// <summary>
        /// Equality operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is equal to <paramref name="right"/>.</returns>
        public static bool operator==(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return !(left is object) ? !(right is object) : left.Equals(right);
        }

        /// <summary>
        /// Inequality operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is different from <paramref name="right"/>.</returns>
        public static bool operator!=(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return !(left == right);
        }

        /// <summary>
        /// Less-than operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is less than <paramref name="right"/>.</returns>
        public static bool operator<(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return right is object && (!(left is object) || left.CompareTo(right) < 0);
        }

        /// <summary>
        /// Less-than-or-equal operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is less than or
        /// equal to <paramref name="right"/>.</returns>
        public static bool operator<=(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return !(right < left);
        }

        /// <summary>
        /// Greater-than operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is greater than <paramref name="right"/>.</returns>
        public static bool operator>(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return right < left;
        }

        /// <summary>
        /// Greater-than-or-equal-to operator.
        /// </summary>
        /// <param name="left">Left pipeline plugin info.</param>
        /// <param name="right">Right pipeline plugin info.</param>
        /// <returns>Whether <paramref name="left"/> is greater than or
        /// equal to <paramref name="right"/>.</returns>
        public static bool operator>=(PipelinePluginInfo left, PipelinePluginInfo right)
        {
            return !(left < right);
        }
    }
    
    /// <summary>
    /// Collection of <see cref="PipelinePluginInfo"/> objects. Can be used
    /// to save and load pipeline plugins to and from XML.
    /// </summary>
    [XmlRoot(Namespace = PipelinePluginsXmlNamespace)]
    public sealed class PipelinePluginCollection
    {
        /// <summary>
        /// XML namespace used to serialize pipeline plugins.
        /// </summary>
        public const string PipelinePluginsXmlNamespace = "http://pathcopycopy.codeplex.com/xsd/PipelinePlugins/V1";

        /// Dictionary storing XML serializer instances. Late-filled.
        private static readonly IDictionary<PipelinePluginXmlSerializerVersion, XmlSerializer> xmlSerializers =
            new Dictionary<PipelinePluginXmlSerializerVersion, XmlSerializer>();

        /// Lock used to protect creation of XmlSerializer objects.
        private static readonly object xmlSerializersLock = new object();

        /// <summary>
        /// Pipeline plugins in the collection.
        /// </summary>
        [XmlArray]
        public List<PipelinePluginInfo> Plugins
        {
            get;
        } = new List<PipelinePluginInfo>();

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline plugin collection.
        /// This corresponds to the highest required version of all plugins in the collection.
        /// </summary>
        [XmlIgnore]
        public Version RequiredVersion
        {
            get {
                return Plugins.Max(info => info.RequiredVersion);
            }
        }

        /// <summary>
        /// Checks whether all pipeline plugins in the collection are <see cref="PipelinePluginInfo.Compatible"/>.
        /// </summary>
        [XmlIgnore]
        public bool Compatible
        {
            get {
                return Plugins.All(info => info.Compatible);
            }
        }
        
        /// <summary>
        /// Serializes this pipeline plugins collection to XML format that will
        /// later be accepted by the <see cref="FromXML"/> method.
        /// </summary>
        /// <param name="stream"><see cref="Stream"/> where to store the XML data.</param>
        /// <param name="serializerVersion">Which version of the XML serializer to use.
        /// Depending on the version, certain attributes are not serialized.</param>
        public void ToXML(Stream stream, PipelinePluginXmlSerializerVersion serializerVersion)
        {
            using (XmlWriter writer = XmlWriter.Create(stream ?? throw new ArgumentNullException(nameof(stream)))) {
                GetXmlSerializer(serializerVersion).Serialize(writer, this);
            }
        }
        
        /// <summary>
        /// Deserializes an <see cref="PipelinePluginCollection"/> object from
        /// XML data.
        /// </summary>
        /// <param name="stream"><see cref="Stream"/> containing the XML.</param>
        /// <returns>New pipeline plugins collection from XML.</returns>
        public static PipelinePluginCollection FromXML(Stream stream)
        {
            using (XmlReader reader = XmlReader.Create(stream ?? throw new ArgumentNullException(nameof(stream)))) {
                return (PipelinePluginCollection) GetXmlSerializer(PipelinePluginXmlSerializerVersion.V3).Deserialize(reader);
            }
        }
        
        /// <summary>
        /// Returns an <see cref="XmlSerializer"/> to use for XML
        /// serialization/deserialization.
        /// </summary>
        /// <param name="serializerVersion">Version of the XML serializer requested.
        /// Depending on the version, certain attributes are not serialized.</param>
        /// <returns>Global <see cref="XmlSerializer"/> instance.</returns>
        private static XmlSerializer GetXmlSerializer(PipelinePluginXmlSerializerVersion serializerVersion)
        {
            XmlSerializer xmlSerializer;
            
            lock(xmlSerializersLock) {
                if (!xmlSerializers.TryGetValue(serializerVersion, out xmlSerializer)) {
                    // Need to create this XML serializer on the first call.
                    if (serializerVersion == PipelinePluginXmlSerializerVersion.V3) {
                        // Current version: serialize everything.
                        xmlSerializer = new XmlSerializer(typeof(PipelinePluginCollection), PipelinePluginsXmlNamespace);
                    } else if (serializerVersion == PipelinePluginXmlSerializerVersion.V2) {
                        // Second version: do not serialize EditMode.
                        XmlAttributeOverrides overrides = new XmlAttributeOverrides();
                        XmlAttributes attributes = new XmlAttributes() {
                            XmlIgnore = true,
                        };
                        overrides.Add(typeof(PipelinePluginInfo), "EditModeAsString", attributes);
                        xmlSerializer = new XmlSerializer(typeof(PipelinePluginCollection),
                            overrides, Array.Empty<Type>(), null, PipelinePluginsXmlNamespace);
                    } else if (serializerVersion == PipelinePluginXmlSerializerVersion.V1) {
                        // First version: do not serializer RequiredVersion or EditMode.
                        XmlAttributeOverrides overrides = new XmlAttributeOverrides();
                        XmlAttributes attributes = new XmlAttributes() {
                            XmlIgnore = true,
                        };
                        overrides.Add(typeof(PipelinePluginInfo), "RequiredVersionAsString", attributes);
                        overrides.Add(typeof(PipelinePluginInfo), "EditModeAsString", attributes);
                        xmlSerializer = new XmlSerializer(typeof(PipelinePluginCollection),
                            overrides, Array.Empty<Type>(), null, PipelinePluginsXmlNamespace);
                    }
                    xmlSerializers.Add(serializerVersion, xmlSerializer);
                }
            }

            Debug.Assert(xmlSerializer != null);
            return xmlSerializer;
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
                return PipelinePluginInfo.DefaultRequiredVersion;
            }
        }
        
        /// <summary>
        /// Method that must be overridden in subclasses to encode the data of
        /// the element in a string. This will be saved in the registry in the
        /// plugin's encoded elements.
        /// </summary>
        /// <returns>Element's encoded data.</returns>
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
        public virtual PipelineElementUserControl GetEditingControl()
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
            if (toEncode >= 10000) {
                throw new ArgumentOutOfRangeException(nameof(toEncode));
            }

            // Encode as four characters.
            return toEncode.ToString("D4", CultureInfo.InvariantCulture);
        }
        
        /// <summary>
        /// Encodes the specified string in a format suitable to be included in
        /// encoded element data.
        /// </summary>
        /// <param name="toEncode">String to encode.</param>
        /// <returns>Encoded string.</returns>
        public static string EncodeString(string toEncode)
        {
            if (toEncode == null) {
                throw new ArgumentNullException(nameof(toEncode));
            }

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
        /// <summary>
        /// List containing all the pipeline elements.
        /// </summary>
        public List<PipelineElement> Elements
        {
            get;
        } = new List<PipelineElement>();

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline. This
        /// amounts to the highest required version of all its pipeline elements.
        /// </summary>
        public Version RequiredVersion
        {
            get {
                return Elements.Max(element => element.RequiredVersion) ?? PipelinePluginInfo.DefaultRequiredVersion;
            }
        }
        
        /// <summary>
        /// Encodes the pipeline's elements in a string to be saved in the plugin data.
        /// </summary>
        /// <returns>Encoded pipeline elements.</returns>
        public string Encode()
        {
            // First write the number of elements in the pipeline as a two-char number.
            if (Elements.Count >= 100) {
                throw new IndexOutOfRangeException();
            }
            StringBuilder encodedElements = new StringBuilder();
            encodedElements.Append(Elements.Count.ToString("D2", CultureInfo.InvariantCulture));

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
            Elements.Clear();
        }
    }
    
    /// <summary>
    /// Exception class used to represent an invalid pipeline encoded string.
    /// </summary>
    [Serializable]
    public class InvalidPipelineException : Exception
    {
#pragma warning disable CA1303 // Do not pass literals as localized parameters

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
            : base($"Invalid pipeline: {encodedElements}")
        {
            EncodedElements = encodedElements;
        }
        
        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements.</param>
        /// <param name="innerException">Inner exception object.</param>
        public InvalidPipelineException(string encodedElements, Exception innerException)
            : base($"Invalid pipeline: {encodedElements}", innerException)
        {
            EncodedElements = encodedElements;
        }

        /// <summary>
        /// Serialization constructor.
        /// </summary>
        /// <param name="serializationInfo">Serialization info.</param>
        /// <param name="streamingContext">Streaming context.</param>
        protected InvalidPipelineException(SerializationInfo serializationInfo, StreamingContext streamingContext)
            : base(serializationInfo, streamingContext)
        {
        }

#pragma warning restore CA1303 // Do not pass literals as localized parameters
    }

    /// <summary>
    /// Method used by <see cref="PushToStackPipelineElement"/>
    /// to push a value on the stack.
    /// </summary>
    public enum PushToStackMethod : int
    {
        /// <summary>
        /// Push the entire path to the stack
        /// </summary>
        Entire = 1,

        /// <summary>
        /// Push a range in the path to the stack
        /// </summary>
        Range = 2,

        /// <summary>
        /// Push the first match of a regex in the path to the stack
        /// </summary>
        Regex = 3,

        /// <summary>
        /// Push a fixed value to the stack
        /// </summary>
        Fixed = 4,
    }

    /// <summary>
    /// Possible locations where to store value popped by
    /// <see cref="PopFromStackPipelineElement"/>.
    /// </summary>
    public enum PopFromStackLocation : int
    {
        /// <summary>
        /// Replace the entire path with the popped value
        /// </summary>
        Entire = 1,

        /// <summary>
        /// Replace a range in the path with the popped value
        /// </summary>
        Range = 2,

        /// <summary>
        /// Replace the first match of a regex with the popped value
        /// </summary>
        Regex = 3,

        /// <summary>
        /// Insert the popped value at the beginning of the path
        /// </summary>
        Start = 4,

        /// <summary>
        /// Insert the popped value at the end of the path
        /// </summary>
        End = 5,

        /// <summary>
        /// Simply drop the popped value
        /// </summary>
        Nowhere = 6,
    }

    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to follow
    /// any symlinks in the path.
    /// </summary>
    public class FollowSymlinkPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'k';

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
                return Resources.PipelineElement_FollowSymlink;
            }
        }

        /// <summary>
        /// Minimum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(18, 0, 0, 0);
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
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
            return string.Empty;
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
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
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
            return string.Empty;
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
            return string.Empty;
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
            return string.Empty;
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
            return string.Empty;
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
            return string.Empty;
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
            return string.Empty;
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
            OldValue = string.Empty;
            NewValue = string.Empty;
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

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new FindReplacePipelineElementUserControl(this);
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
        public const int InitialVersion = 1;

        /// <summary>
        /// Maximum data version understood by this code.
        /// </summary>
        public const int MaxVersion = InitialVersion;

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
            Regex = string.Empty;
            Format = string.Empty;
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
            encoder.Append(EncodeInt(InitialVersion));

            // Now encode regex, format string and ignore case flag.
            encoder.Append(EncodeString(Regex));
            encoder.Append(EncodeString(Format));
            encoder.Append(EncodeBool(IgnoreCase));

            return encoder.ToString();
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new RegexPipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that instructs Path Copy Copy to replace parts
    /// of the path with environment variable references if possible.
    /// </summary>
    public class UnexpandEnvironmentStringsPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'e';

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
                return Resources.PipelineElement_UnexpandEnvStrings;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(18, 0, 0, 0);
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
        }
    }

    /// <summary>
    /// Pipeline element that replaces all instances of %DRIVELABEL%
    /// with the drive's label.
    /// </summary>
    public class InjectDriveLabelPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = ':';

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
                return Resources.PipelineElement_InjectDriveLabel;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(19, 0, 0, 0);
            }
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new InjectDriveLabelPipelineElementUserControl();
        }
    }

    /// <summary>
    /// Pipeline element that copies the N first or last parts
    /// of the path.
    /// </summary>
    public class CopyNPathPartsPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'n';

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
                return Resources.PipelineElement_CopyNPathParts;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(19, 0, 0, 0);
            }
        }

        /// <summary>
        /// Number of path parts to copy.
        /// </summary>
        public int NumParts
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to copy the first (<c>true</c>) or last (<c>false</c>)
        /// parts of the path.
        /// </summary>
        public bool First
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public CopyNPathPartsPipelineElement()
        {
            NumParts = 1;
            First = false;
        }

        public CopyNPathPartsPipelineElement(int numParts, bool first)
        {
            NumParts = numParts;
            First = first;
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode number of path parts and first flag.
            return EncodeInt(NumParts) + EncodeBool(First);
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new CopyNPathPartsPipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Base class for pipeline elements that store a plugin ID.
    /// </summary>
    abstract public class PipelineElementWithPluginID : PipelineElement
    {
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
        public PipelineElementWithPluginID()
        {
            PluginID = Guid.Empty;
        }
        
        /// <summary>
        /// Constructor with plugin ID.
        /// </summary>
        /// <param name="pluginId">ID of plugin to apply to the path.</param>
        public PipelineElementWithPluginID(Guid pluginId)
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
            return PluginID.ToString("B", CultureInfo.InvariantCulture);
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new PipelineElementWithPluginIDUserControl(this,
                IncludePipelineElementsInEditingControl());
        }

        /// <summary>
        /// Determines if we accept pipeline plugins as possible plugins
        /// for our element. Used for our editing control.
        /// </summary>
        /// <returns><c>true</c> to include pipeline plugins.</returns>
        abstract protected bool IncludePipelineElementsInEditingControl();
    }
    
    /// <summary>
    /// Pipeline element that applies the effect of another plugin on the path.
    /// </summary>
    public class ApplyPluginPipelineElement : PipelineElementWithPluginID
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
        /// Default constructor.
        /// </summary>
        public ApplyPluginPipelineElement()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with plugin ID.
        /// </summary>
        /// <param name="pluginId">ID of plugin to apply to the path.</param>
        public ApplyPluginPipelineElement(Guid pluginId)
            : base(pluginId)
        {
        }

        /// <summary>
        /// Determines if we accept pipeline plugins as possible plugins
        /// for our element. Used for our editing control.
        /// </summary>
        /// <returns><c>true</c> to include pipeline plugins.</returns>
        protected override bool IncludePipelineElementsInEditingControl()
        {
            return false;
        }
    }

    /// <summary>
    /// Pipeline element that applies the effect of another plugin on the path.
    /// This element supports pipeline plugins.
    /// </summary>
    public class ApplyPipelinePluginPipelineElement : PipelineElementWithPluginID
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '}';

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
                return Resources.PipelineElement_ApplyPipelinePlugin;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(18, 0, 0, 0);
            }
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public ApplyPipelinePluginPipelineElement()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with plugin ID.
        /// </summary>
        /// <param name="pluginId">ID of plugin to apply to the path.</param>
        public ApplyPipelinePluginPipelineElement(Guid pluginId)
            : base(pluginId)
        {
        }

        /// <summary>
        /// Determines if we accept pipeline plugins as possible plugins
        /// for our element. Used for our editing control.
        /// </summary>
        /// <returns><c>true</c> to include pipeline plugins.</returns>
        protected override bool IncludePipelineElementsInEditingControl()
        {
            return true;
        }
    }

    /// <summary>
    /// Base class for pipeline elements that deal with the stack.
    /// Simply used to identify such elements as expert-mode-only.
    /// </summary>
    abstract public class StackPipelineElement : PipelineElement
    {
        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(19, 0, 0, 0);
            }
        }
    }

    /// <summary>
    /// Pipeline element that pushes a value on the stack. The value
    /// can be fixed or come from part of the path.
    /// </summary>
    public class PushToStackPipelineElement : StackPipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'u';

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
                return Resources.PipelineElement_PushToStack;
            }
        }

        /// <summary>
        /// Method used to determine what to push on the stack.
        /// </summary>
        /// <seealso cref="PushToStackMethod"/>
        public PushToStackMethod Method
        {
            get;
            set;
        } = PushToStackMethod.Entire;

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Range"/>,
        /// stores the beginning of the range in the path to push to the stack.
        /// </summary>
        public int Begin
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Range"/>,
        /// stores the end of the range in the path to push to the stack.
        /// </summary>
        public int End
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Regex"/>,
        /// stores the regular expression used to locate the part of the path
        /// to push to the stack.
        /// </summary>
        public string Regex
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Regex"/>,
        /// stores the flag indicating whether regular expression is
        /// case-sensitive or not.
        /// </summary>
        public bool IgnoreCase
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Regex"/>,
        /// stores the index of the group in the regex match to push to the
        /// stack. 1-based, but can be set to 0 to push the entire match.
        /// </summary>
        public int Group
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Method"/> is <see cref="PushToStackMethod.Fixed"/>,
        /// stores the fixed string to push to the stack.
        /// </summary>
        public string FixedString
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor. Element will push entire path to the stack.
        /// </summary>
        public PushToStackPipelineElement()
        {
        }

        /// <summary>
        /// Constructor for an element that pushes a range in the path
        /// to the stack.
        /// </summary>
        /// <param name="begin">Start of range in path to push to the stack.</param>
        /// <param name="end">End of range in path to push to the stack.</param>
        public PushToStackPipelineElement(int begin, int end)
        {
            Method = PushToStackMethod.Range;
            Begin = begin;
            End = end;
        }

        /// <summary>
        /// Constructor for an element that uses a regular expression
        /// to find the part of the path to push to the stack.
        /// </summary>
        /// <param name="regex">Regex used to locate the part of
        /// the path to push to the stack.</param>
        /// <param name="ignoreCase">Whether <paramref name="regex"/>
        /// is a case-sensitive regex or not.</param>
        /// <param name="group">Index of group in the regex match
        /// to push to the stack. 1-based, but 0 can be used to
        /// push the entire match.</param>
        public PushToStackPipelineElement(string regex, bool ignoreCase, int group)
        {
            Method = PushToStackMethod.Regex;
            Regex = regex;
            IgnoreCase = ignoreCase;
            Group = group;
        }

        /// <summary>
        /// Constructor for an element that pushes a fixed value
        /// to the stack.
        /// </summary>
        /// <param name="fixedString">Fixed value to push to
        /// the stack.</param>
        public PushToStackPipelineElement(string fixedString)
        {
            Method = PushToStackMethod.Fixed;
            FixedString = fixedString;
        }
        
        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // How we encode depend on the push method.
            StringBuilder encoder = new StringBuilder();
            encoder.Append(EncodeInt((int) Method));
            switch (Method) {
                case PushToStackMethod.Entire: {
                    // Nothing more to encode.
                    break;
                }
                case PushToStackMethod.Range: {
                    // Encode beginning and end of range.
                    encoder.Append(EncodeInt(Begin));
                    encoder.Append(EncodeInt(End));
                    break;
                }
                case PushToStackMethod.Regex: {
                    // Encode regex, ignore case flag and group index.
                    encoder.Append(EncodeString(Regex ?? string.Empty));
                    encoder.Append(EncodeBool(IgnoreCase));
                    encoder.Append(EncodeInt(Group));
                    break;
                }
                case PushToStackMethod.Fixed: {
                    // Encode the fixed string value.
                    encoder.Append(EncodeString(FixedString ?? string.Empty));
                    break;
                }
                default: {
                    Debug.Fail($"Unknown push to stack method: {Method}");
                    break;
                }
            }
            return encoder.ToString();
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new PushToStackPipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that pops a value from the stack
    /// and optionally stores it somewhere in the path
    /// (perhaps replacing part of it).
    /// </summary>
    public class PopFromStackPipelineElement : StackPipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'o';

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
                return Resources.PipelineElement_PopFromStack;
            }
        }

        /// <summary>
        /// Location where to store the popped value.
        /// </summary>
        /// <seealso cref="PopFromStackLocation"/>
        public PopFromStackLocation Location
        {
            get;
            set;
        } = PopFromStackLocation.Entire;

        /// <summary>
        /// If <see cref="Location"/> is <see cref="PopFromStackLocation.Range"/>,
        /// contains the beginning of the range to replace with the popped value.
        /// </summary>
        public int Begin
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Location"/> is <see cref="PopFromStackLocation.Range"/>,
        /// contains the end of the range to replace with the popped value.
        /// </summary>
        public int End
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Location"/> is <see cref="PopFromStackLocation.Regex"/>,
        /// contains the regular expression used to locate the part of the path
        /// to replace with the popped value.
        /// </summary>
        public string Regex
        {
            get;
            set;
        }

        /// <summary>
        /// If <see cref="Location"/> is <see cref="PopFromStackLocation.Regex"/>,
        /// contains the flag telling if <see cref="Regex"/> is case-sensitive or not.
        /// </summary>
        public bool IgnoreCase
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor. Element will replace the
        /// entire path with the popped value.
        /// </summary>
        public PopFromStackPipelineElement()
        {
        }

        /// <summary>
        /// Constructor with location where to store the popped value.
        /// Used when no other data is needed.
        /// </summary>
        /// <param name="location">Popped value store location.</param>
        /// <seealso cref="PopFromStackLocation.Entire"/>
        /// <seealso cref="PopFromStackLocation.Start"/>
        /// <seealso cref="PopFromStackLocation.End"/>
        /// <seealso cref="PopFromStackLocation.Nowhere"/>
        public PopFromStackPipelineElement(PopFromStackLocation location)
        {
            Location = location;
        }

        /// <summary>
        /// Constructor for an element that replaces a range in the
        /// path with the popped value.
        /// </summary>
        /// <param name="begin">Beginning of range to replace with
        /// the popped value (inclusive).</param>
        /// <param name="end">End of range to replace with the
        /// popped value (exclusive).</param>
        public PopFromStackPipelineElement(int begin, int end)
        {
            Location = PopFromStackLocation.Range;
            Begin = begin;
            End = end;
        }

        /// <summary>
        /// Constructor for an element that uses a regex to locate the
        /// part of the path to replace with the popped value.
        /// </summary>
        /// <param name="regex">Regex to use to locate store location.</param>
        /// <param name="ignoreCase">Whether <paramref name="regex"/>
        /// is case-sensitive or not.</param>
        public PopFromStackPipelineElement(string regex, bool ignoreCase)
        {
            Location = PopFromStackLocation.Regex;
            Regex = regex;
            IgnoreCase = ignoreCase;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // How we encode depend on the store location.
            StringBuilder encoder = new StringBuilder();
            encoder.Append(EncodeInt((int) Location));
            switch (Location) {
                case PopFromStackLocation.Entire:
                case PopFromStackLocation.Start:
                case PopFromStackLocation.End:
                case PopFromStackLocation.Nowhere: {
                    // No other data to encode.
                    break;
                }
                case PopFromStackLocation.Range: {
                    // Encode begin and end of range.
                    encoder.Append(EncodeInt(Begin));
                    encoder.Append(EncodeInt(End));
                    break;
                }
                case PopFromStackLocation.Regex: {
                    // Encode regex and ignore case flag.
                    encoder.Append(EncodeString(Regex ?? string.Empty));
                    encoder.Append(EncodeBool(IgnoreCase));
                    break;
                }
                default: {
                    Debug.Fail($"Unknown pop from stack location: {Location}");
                    break;
                }
            }
            return encoder.ToString();
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new PopFromStackPipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that swaps the two top values on the stack.
    /// </summary>
    public class SwapStackValuesPipelineElement : StackPipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'w';

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
                return Resources.PipelineElement_SwapStackValues;
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
        }
    }

    /// <summary>
    /// Pipeline element that duplicates the top value on the stack.
    /// </summary>
    public class DuplicateStackValuePipelineElement : StackPipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'd';

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
                return Resources.PipelineElement_DuplicateStackValue;
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
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
            PathsSeparator = string.Empty;
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

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new PathsSeparatorPipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to copy paths recursively.
    /// </summary>
    public class RecursiveCopyPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = 'v';

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
                return Resources.PipelineElement_RecursiveCopy;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(19, 0, 0, 0);
            }
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // No other data to encode.
            return string.Empty;
        }
    }

    /// <summary>
    /// Abstract subclass of <see cref="PipelineElement"/> that has a
    /// single data member: an executable path. Used to be able to edit
    /// both types of executable-based pipeline elements in a common way.
    /// </summary>
    abstract public class PipelineElementWithExecutable : PipelineElement
    {
        /// <summary>
        /// Path to executable.
        /// </summary>
        public string Executable
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public PipelineElementWithExecutable()
        {
            Executable = string.Empty;
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public PipelineElementWithExecutable(string executable)
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

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new PipelineElementWithExecutableUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to launch an executable with path(s) as argument
    /// instead of copying them to the clipboard.
    /// </summary>
    public class ExecutablePipelineElement : PipelineElementWithExecutable
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
        /// Default constructor.
        /// </summary>
        public ExecutablePipelineElement()
            : base()
        {
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public ExecutablePipelineElement(string executable)
            : base(executable)
        {
        }
    }

    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to launch an executable with filelist as argument
    /// instead of copying them to the clipboard.
    /// </summary>
    public class ExecutableWithFilelistPipelineElement : PipelineElementWithExecutable
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
        /// Default constructor.
        /// </summary>
        public ExecutableWithFilelistPipelineElement()
            : base()
        {
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public ExecutableWithFilelistPipelineElement(string executable)
            : base(executable)
        {
        }
    }
    
    /// <summary>
    /// Pipeline element that does not modify the path but instructs
    /// Path Copy Copy to launch a command-line, passing the paths in
    /// the arguments either directly or via a filelist.
    /// </summary>
    public class CommandLinePipelineElement : PipelineElementWithExecutable
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '>';

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
                return Resources.PipelineElement_CommandLine;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(19, 0, 0, 0);
            }
        }

        /// <summary>
        /// Arguments to pass to the executable. Can include %FILES%,
        /// which will be replaced with the paths (or the path to the
        /// filelist if <see cref="UseFilelist"/> is <c>true</c>).
        /// </summary>
        public string Arguments
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to use a filelist to pass the paths to the executable.
        /// </summary>
        public bool UseFilelist
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public CommandLinePipelineElement()
            : base()
        {
            Arguments = string.Empty;
            UseFilelist = false;
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="executable">Path to executable.</param>
        public CommandLinePipelineElement(string executable, string arguments, bool useFilelist)
            : base(executable)
        {
            Arguments = arguments;
            UseFilelist = useFilelist;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode the executable path, arguments and use filelist flag.
            return base.Encode() + EncodeString(Arguments) + EncodeBool(UseFilelist);
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new CommandLinePipelineElementUserControl(this);
        }
    }

    /// <summary>
    /// Pipeline element that determines whether the plugin will be displayed
    /// when files and/or folders are selected.
    /// </summary>
    public class DisplayForSelectionPipelineElement : PipelineElement
    {
        /// <summary>
        /// Code representing this pipeline element type.
        /// </summary>
        public const char CODE = '!';

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
                return Resources.PipelineElement_DisplayForSelection;
            }
        }

        /// <summary>
        /// Minumum version of Path Copy Copy required to use this pipeline element.
        /// </summary>
        public override Version RequiredVersion
        {
            get {
                return new Version(20, 0, 0, 0);
            }
        }

        /// <summary>
        /// Whether to display plugin when files are selected.
        /// </summary>
        public bool ShowForFiles
        {
            get;
            set;
        }

        /// <summary>
        /// Whether to display plugin when folders are selected.
        /// </summary>
        public bool ShowForFolders
        {
            get;
            set;
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        public DisplayForSelectionPipelineElement()
            : base()
        {
            ShowForFiles = true;
            ShowForFolders = true;
        }

        /// <summary>
        /// Constructor with arguments.
        /// </summary>
        /// <param name="showForFiles">Whether to display plugin when files are selected.</param>
        /// <param name="showForFolders">Whether to display plugin when folders are selected.</param>
        public DisplayForSelectionPipelineElement(bool showForFiles, bool showForFolders)
            : base()
        {
            ShowForFiles = showForFiles;
            ShowForFolders = showForFolders;
        }

        /// <summary>
        /// Encodes this pipeline element in a string.
        /// </summary>
        /// <returns>Encoded element data.</returns>
        public override string Encode()
        {
            // Encode both flags.
            return EncodeBool(ShowForFiles) + EncodeBool(ShowForFolders);
        }

        /// <summary>
        /// Returns a user control to configure this pipeline element.
        /// </summary>
        /// <returns>User control.</returns>
        public override PipelineElementUserControl GetEditingControl()
        {
            return new DisplayForSelectionPipelineElementUserControl(this);
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
            if (encodedElements == null) {
                throw new ArgumentNullException(nameof(encodedElements));
            }

            // Initialize new pipeline object.
            Pipeline pipeline = new Pipeline();

            try {
                // The first two characters of the string should be the number of elements.
                if (encodedElements.Length < 2) {
                    throw new InvalidPipelineException();
                }
                int numElements = int.Parse(encodedElements.Substring(0, 2), CultureInfo.InvariantCulture);

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
            PipelineElement element;

            // First read the element code.
            if (curChar == encodedElements.Length) {
                throw new InvalidPipelineException();
            }
            char elementCode = encodedElements[curChar++];
            switch (elementCode) {
                case FollowSymlinkPipelineElement.CODE: {
                    element = new FollowSymlinkPipelineElement();
                    break;
                }
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
                case UnexpandEnvironmentStringsPipelineElement.CODE: {
                    element = new UnexpandEnvironmentStringsPipelineElement();
                    break;
                }
                case InjectDriveLabelPipelineElement.CODE: {
                    element = new InjectDriveLabelPipelineElement();
                    break;
                }
                case CopyNPathPartsPipelineElement.CODE: {
                    element = DecodeCopyNPathPartsPipelineElement(encodedElements, ref curChar);
                    break;
                }
                case ApplyPluginPipelineElement.CODE:
                case ApplyPipelinePluginPipelineElement.CODE: {
                    element = DecodeApplyPluginElement(elementCode, encodedElements, ref curChar);
                    break;
                }
                case PushToStackPipelineElement.CODE: {
                    element = DecodePushToStackElement(encodedElements, ref curChar);
                    break;
                }
                case PopFromStackPipelineElement.CODE: {
                    element = DecodePopFromStackElement(encodedElements, ref curChar);
                    break;
                }
                case SwapStackValuesPipelineElement.CODE: {
                    element = new SwapStackValuesPipelineElement();
                    break;
                }
                case DuplicateStackValuePipelineElement.CODE: {
                    element = new DuplicateStackValuePipelineElement();
                    break;
                }
                case PathsSeparatorPipelineElement.CODE: {
                    element = DecodePathsSeparatorElement(encodedElements, ref curChar);
                    break;
                }
                case RecursiveCopyPipelineElement.CODE: {
                    element = new RecursiveCopyPipelineElement();
                    break;
                }
                case ExecutablePipelineElement.CODE:
                case ExecutableWithFilelistPipelineElement.CODE: {
                    element = DecodeExecutableElement(elementCode, encodedElements, ref curChar);
                    break;
                }
                case CommandLinePipelineElement.CODE: {
                    element = DecodeCommandLinePipelineElement(encodedElements, ref curChar);
                    break;
                }
                case DisplayForSelectionPipelineElement.CODE: {
                    element = DecodeDisplayForSelectionPipelineElement(encodedElements, ref curChar);
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
            if (version > RegexPipelineElement.MaxVersion) {
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
        /// Decodes a <see cref="CopyNPathPartsPipelineElement"/> from an
        /// encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static CopyNPathPartsPipelineElement DecodeCopyNPathPartsPipelineElement(
            string encodedElements, ref int curChar)
        {
            // The element data contains the number of parts and copy direction flag.
            int numParts = DecodeInt(encodedElements, ref curChar);
            bool first = DecodeBool(encodedElements, ref curChar);
            return new CopyNPathPartsPipelineElement(numParts, first);
        }
        
        /// <summary>
        /// Decodes an <see cref="ApplyPluginPipelineElement"/> or
        /// <see cref="ApplyPipelinePluginPipelineElement"/> from an encoded element string.
        /// </summary>
        /// <param name="elementCode">Element code.</param>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static PipelineElement DecodeApplyPluginElement(
            char elementCode, string encodedElements, ref int curChar)
        {
            // The element data is the ID of the plugin to apply. There's no length saved
            // for this since it's always the same length. Format is as follows:
            // {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
            int guidLength = Guid.Empty.ToString("B", CultureInfo.InvariantCulture).Length;
            if (curChar > (encodedElements.Length - guidLength)) {
                throw new InvalidPipelineException();
            }
            Guid pluginId = new Guid(encodedElements.Substring(curChar, guidLength));
            curChar += guidLength;
            switch (elementCode) {
                case ApplyPluginPipelineElement.CODE: {
                    return new ApplyPluginPipelineElement(pluginId);
                }
                case ApplyPipelinePluginPipelineElement.CODE: {
                    return new ApplyPipelinePluginPipelineElement(pluginId);
                }
                default:
                    throw new InvalidPipelineException();
            }
        }

        /// <summary>
        /// Decodes a <see cref="PushToStackPipelineElement"/> from an encoded
        /// element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static PushToStackPipelineElement DecodePushToStackElement(
            string encodedElements, ref int curChar)
        {
            // First read the push method.
            switch ((PushToStackMethod) DecodeInt(encodedElements, ref curChar)) {
                case PushToStackMethod.Entire: {
                    // No more data in encoded string.
                    return new PushToStackPipelineElement();
                }
                case PushToStackMethod.Range: {
                    // Encoded string contains beginning and end of range.
                    int begin = DecodeInt(encodedElements, ref curChar);
                    int end = DecodeInt(encodedElements, ref curChar);
                    return new PushToStackPipelineElement(begin, end);
                }
                case PushToStackMethod.Regex: {
                    // Encoded string contains the regex, ignore case flag and group number.
                    string regex = DecodeString(encodedElements, ref curChar);
                    bool ignoreCase = DecodeBool(encodedElements, ref curChar);
                    int group = DecodeInt(encodedElements, ref curChar);
                    return new PushToStackPipelineElement(regex, ignoreCase, group);
                }
                case PushToStackMethod.Fixed: {
                    // Encoded string contains the fixed string.
                    string fixedString = DecodeString(encodedElements, ref curChar);
                    return new PushToStackPipelineElement(fixedString);
                }
                default:
                    throw new InvalidPipelineException();
            }
        }

        /// <summary>
        /// Decodes a <see cref="PopFromStackPipelineElement"/> from an encoded
        /// element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static PopFromStackPipelineElement DecodePopFromStackElement(
            string encodedElements, ref int curChar)
        {
            // First read pop location.
            PopFromStackLocation location = (PopFromStackLocation) DecodeInt(encodedElements, ref curChar);
            switch (location) {
                case PopFromStackLocation.Entire:
                case PopFromStackLocation.Start:
                case PopFromStackLocation.End:
                case PopFromStackLocation.Nowhere: {
                    // No more data to decode.
                    return new PopFromStackPipelineElement(location);
                }
                case PopFromStackLocation.Range: {
                    // Encoded string contains beginning and end of range.
                    int begin = DecodeInt(encodedElements, ref curChar);
                    int end = DecodeInt(encodedElements, ref curChar);
                    return new PopFromStackPipelineElement(begin, end);
                }
                case PopFromStackLocation.Regex: {
                    // Encoded string contains regex and ignore case flag.
                    string regex = DecodeString(encodedElements, ref curChar);
                    bool ignoreCase = DecodeBool(encodedElements, ref curChar);
                    return new PopFromStackPipelineElement(regex, ignoreCase);
                }
                default:
                    throw new InvalidPipelineException();
            }
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
        /// Decodes a <see cref="CommandLinePipelineElement"/> from
        /// an encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static CommandLinePipelineElement DecodeCommandLinePipelineElement(
            string encodedElements, ref int curChar)
        {
            // The element data contains the executable path, arguments and
            // a flag telling whether to use a filelist or not.
            string executable = DecodeString(encodedElements, ref curChar);
            string arguments = DecodeString(encodedElements, ref curChar);
            bool useFilelist = DecodeBool(encodedElements, ref curChar);
            return new CommandLinePipelineElement(executable, arguments, useFilelist);
        }

        /// <summary>
        /// Decodes a <see cref="DisplayForSelectionPipelineElement"/> from
        /// an encoded element string.
        /// </summary>
        /// <param name="encodedElements">String of encoded elements data.</param>
        /// <param name="curChar">Position where the element data is to be found
        /// in the string (not counting the element code). Upon return, this will
        /// point just after the element data.</param>
        private static DisplayForSelectionPipelineElement DecodeDisplayForSelectionPipelineElement(
            string encodedElements, ref int curChar)
        {
            // The element data contains two flags.
            bool showForFiles = DecodeBool(encodedElements, ref curChar);
            bool showForFolders = DecodeBool(encodedElements, ref curChar);
            return new DisplayForSelectionPipelineElement(showForFiles, showForFolders);
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
            int intValue = int.Parse(encodedElements.Substring(curChar, 4), CultureInfo.InvariantCulture);
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
        /// <summary>
        /// Dictionary storing information about overwritten plugins. The key
        /// is the new pipeline plugin infos and the value is the container
        /// containing information about the old plugin being overwritten.
        /// </summary>
        /// <seealso cref="ImportedPipelinePluginOverwriteInfo"/>
        public Dictionary<PipelinePluginInfo, ImportedPipelinePluginOverwriteInfo> OverwriteInfos
        {
            get;
        } = new Dictionary<PipelinePluginInfo, ImportedPipelinePluginOverwriteInfo>();
    }

    /// <summary>
    /// Information about an overwritten pipeline plugin.
    /// </summary>
    public sealed class ImportedPipelinePluginOverwriteInfo
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
        public ImportedPipelinePluginOverwriteInfo(PipelinePluginInfo oldInfo, int oldIndex)
        {
            OldInfo = oldInfo ?? throw new ArgumentNullException(nameof(oldInfo));
            OldIndex = oldIndex;
        }
    }
}
