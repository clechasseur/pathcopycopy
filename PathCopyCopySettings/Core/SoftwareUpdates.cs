// SoftwareUpdates.cs
// (c) 2012-2020, Charles Lechasseur
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
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Reflection;
using System.Runtime.Serialization;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using PathCopyCopy.Settings.UI.Forms;

namespace PathCopyCopy.Settings.Core
{
    /// <summary>
    /// Bean containing info about a software update. Can be read from XML.
    /// </summary>
    public sealed class SoftwareUpdateInfo
    {
        /// <summary>
        /// Version number of the latest build.
        /// </summary>
        [XmlIgnore]
        public Version Version
        {
            get;
            set;
        } = new Version(1, 0, 0, 0);

        /// <summary>
        /// String representation of <see cref="Version"/>.
        /// Used for XML serialization.
        /// </summary>
        [XmlElement("Version")]
        public string VersionAsString
        {
            get {
                return Version.ToString();
            }
            set {
                Version = new Version(value);
            }
        }

        /// <summary>
        /// Minimum required version of Windows that is needed to install this update.
        /// </summary>
        /// <remarks>
        /// Defaults to Windows XP.
        /// </remarks>
        /// <seealso cref="System.Environment.OSVersion"/>
        /// <seealso cref="System.OperatingSystem.Version"/>
        [XmlIgnore]
        public Version RequiredWindowsVersion
        {
            get;
            set;
        } = new Version(5, 1, 0, 0);

        /// <summary>
        /// String representation of <see cref="RequiredWindowsVersion"/>.
        /// Used for XML serialization.
        /// </summary>
        [XmlElement("RequiredWindowsVersion")]
        public string RequiredWindowsVersionAsString
        {
            get {
                return RequiredWindowsVersion.ToString();
            }
            set {
                RequiredWindowsVersion = new Version(value);
            }
        }

        /// <summary>
        /// Types of installation covered by this software update. This is compared
        /// to <see cref="UserSettings.InstallSource"/> to see if this update can be used.
        /// </summary>
        [XmlArray]
        public List<string> InstallSources
        {
            get;
        } = new List<string>();

        /// <summary>
        /// Name of the latest build.
        /// </summary>
        [XmlElement]
        public string Name
        {
            get;
            set;
        }

#pragma warning disable CA1056 // Uri properties should not be strings

        /// <summary>
        /// URL of the build's web page.
        /// </summary>
        [XmlElement]
        public string Url
        {
            get;
            set;
        }

#pragma warning restore CA1056 // Uri properties should not be strings

        /// <summary>
        /// Lines containing build's release notes.
        /// </summary>
        [XmlArray]
        public List<string> ReleaseNotes
        {
            get;
        } = new List<string>();
    }
    
    /// <summary>
    /// Collection of <see cref="SoftwareUpdateInfo"/>. Can be read from XML.
    /// </summary>
    [XmlRoot(Namespace = SoftwareUpdateV2XmlNamespace)]
    public sealed class SoftwareUpdateCollection
    {
        /// <summary>
        /// XML namespace used by the software update info schema.
        /// </summary>
        /// <remarks>
        /// Version history:
        /// 
        /// <list type="table">
        ///     <listheader>
        ///         <term>Version</term>
        ///     </listheader>
        ///     <listheader>
        ///         <term>Description</term>
        ///     </listheader>
        ///     <item>
        ///         <description>http://pathcopycopy.codeplex.com/xsd/SoftwareUpdate/V1</description>
        ///     </item>
        ///     <item>
        ///         <description>Initial version.</description>
        ///     </item>
        ///     <item>
        ///         <description>http://pathcopycopy.codeplex.com/xsd/SoftwareUpdate/V2</description>
        ///     </item>
        ///     <item>
        ///         <description>Support for multiple updates, added Windows version and InstallSource requirements.</description>
        ///     </item>
        /// </list>
        /// </remarks>
        public const string SoftwareUpdateV2XmlNamespace = "http://pathcopycopy.codeplex.com/xsd/SoftwareUpdate/V2";

        /// XML serializer used to deserialize software update objects.
        private static XmlSerializer xmlSerializer;

        /// Object used to synchronize access to the XML serializer.
        private static readonly object xmlSerializerLock = new object();

        /// <summary>
        /// List of <see cref="SoftwareUpdateInfo"/> beans containing info about the software
        /// updates that are available. They are checked in the order they appear in the list.
        /// </summary>
        [XmlArray]
        public List<SoftwareUpdateInfo> UpdateInfos
        {
            get;
        } = new List<SoftwareUpdateInfo>();
        
        /// <summary>
        /// Deserializes a <see cref="SoftwareUpdateCollection"/> object from XML data.
        /// </summary>
        /// <param name="xml">String containing the XML.</param>
        /// <returns>New software updates collection from XML.</returns>
        public static SoftwareUpdateCollection FromXML(string xml)
        {
            if (xml == null) {
                throw new ArgumentNullException(nameof(xml));
            }

            using (StringReader textReader = new StringReader(xml)) {
                using (XmlReader xmlReader = XmlReader.Create(textReader)) {
                    return (SoftwareUpdateCollection) GetXmlSerializer().Deserialize(xmlReader);
                }
            }
        }
        
        /// <summary>
        /// Returns the <see cref="XmlSerializer"/> to use for XML deserialization.
        /// </summary>
        /// <returns>Global <see cref="XmlSerializer"/> instance.</returns>
        private static XmlSerializer GetXmlSerializer()
        {
            // Create XmlSerializer on the first call.
            if (xmlSerializer == null) {
                lock (xmlSerializerLock) {
                    if (xmlSerializer == null) {
                        xmlSerializer = new XmlSerializer(typeof(SoftwareUpdateCollection),
                            SoftwareUpdateV2XmlNamespace);
                    }
                }
            }
            return xmlSerializer;
        }
    }
    
    /// <summary>
    /// Class used to check for software updates give access to build information
    /// when updates are found.
    /// </summary>
    public sealed class SoftwareUpdater
    {
        /// <summary>
        /// Update channel value to set to disable software updates.
        /// </summary>
        public const string NullUpdateChannelUpperCase = "NONE";

        /// UserSettings object used to access settings.
        private readonly UserSettings userSettings;
        
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="userSettings"><see cref="UserSettings"/> object. Cannot
        /// be <c>null</c>.</param>
        public SoftwareUpdater(UserSettings userSettings)
        {
            this.userSettings = userSettings ?? throw new ArgumentNullException(nameof(userSettings));
        }
        
        /// <summary>
        /// Downloads the latest software update collection and checks if there
        /// is a compatible software update available. If so, shows a modal form
        /// proposing the update to the user.
        /// </summary>
        /// <param name="owner">Owner window of the modal form. Can be <c>null</c>.
        /// </param>
        /// <returns><c>true</c> if there was a software update shown, <c>false</c>
        /// if the software is up-to-date.</returns>
        public bool CheckForUpdate(IWin32Window owner)
        {
            bool hasUpdate = false;

            if (userSettings.UpdateChannel.ToUpperInvariant() != NullUpdateChannelUpperCase) {
                try {
                    // Download the latest software update collection XML.
                    string updatesXml;
                    using (WebClient client = new WebClient()) {
                        updatesXml = client.DownloadString(userSettings.UpdateChannel);
                    }

                    // Decode the software updates.
                    SoftwareUpdateCollection updates = SoftwareUpdateCollection.FromXML(updatesXml);

                    // Iterate the software updates in the collection in the order they appear
                    // until we find an update that we can use.
                    foreach (SoftwareUpdateInfo updateInfo in updates.UpdateInfos) {
                        if (CanBeUsed(updateInfo)) {
                            // This is an update we can use.
                            // Compare the latest version of the software with the version of this assembly,
                            // or to the latest ignored version if there is one (whichever is greater).
                            Version latestVersion = updateInfo.Version;
                            Version thisVersion = Assembly.GetExecutingAssembly().GetName().Version;
                            Version ignoredVersion = userSettings.IgnoredUpdate;
                            if (ignoredVersion != null && ignoredVersion.CompareTo(thisVersion) > 0) {
                                thisVersion = ignoredVersion;
                            }
                            hasUpdate = latestVersion != null && (latestVersion.CompareTo(thisVersion) > 0 ||
                                (userSettings.DevBuild && latestVersion.CompareTo(thisVersion) >= 0));

                            // If there is a software update, show the modal form.
                            bool ignoreUpdate = false;
                            if (hasUpdate) {
                                using (SoftwareUpdateForm updateForm = new SoftwareUpdateForm()) {
                                    ignoreUpdate = updateForm.ShowUpdate(updateInfo, owner);
                                }
                            }

                            // If we need to ignore this update, do so now.
                            if (ignoreUpdate) {
                                userSettings.IgnoredUpdate = latestVersion;
                            }

                            // Break since we don't want to offer multiple updates.
                            break;
                        }
                    }
                } catch (Exception e) {
#pragma warning disable CA1303 // Do not pass literals as localized parameters
                    throw new SoftwareUpdateException("Error during CheckForUpdate", e);
#pragma warning restore CA1303 // Do not pass literals as localized parameters
                }
            }

            return hasUpdate;
        }
        
        /// <summary>
        /// Determines if the given <see cref="SoftwareUpdateInfo"/> can be used
        /// to update this installation of Path Copy Copy. We check for required
        /// Windows version as well as supported installation sources.
        /// </summary>
        /// <param name="updateInfo"><see cref="SoftwareUpdateInfo"/> to evaluate.</param>
        /// <returns><c>true</c> if this software update can be used.</returns>
        private bool CanBeUsed(SoftwareUpdateInfo updateInfo)
        {
            if (updateInfo == null) {
                throw new ArgumentNullException(nameof(updateInfo));
            }

            // Make sure we meet the minimum Windows requirement and the update
            // supports the installation source of our own installation.
            Debug.Assert(Environment.OSVersion.Platform == PlatformID.Win32NT);
            return updateInfo.RequiredWindowsVersion.CompareTo(Environment.OSVersion.Version) <= 0 &&
                updateInfo.InstallSources.Contains(userSettings.InstallSource);
        }
    }
    
    /// <summary>
    /// Exception used by the <see cref="SoftwareUpdater"/> when an error occurs
    /// when checking for software updates.
    /// </summary>
    [Serializable]
    public class SoftwareUpdateException : Exception
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public SoftwareUpdateException()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with message.
        /// </summary>
        /// <param name="message">Exception message.</param>
        public SoftwareUpdateException(string message)
            : base(message)
        {
        }

        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        /// <param name="innerException">Inner exception.</param>
        public SoftwareUpdateException(Exception innerException)
            : base(innerException?.Message, innerException)
        {
        }
        
        /// <summary>
        /// Constructor with exception message and inner exception.
        /// </summary>
        /// <param name="message">Exception message.</param>
        /// <param name="innerException">Inner exception.</param>
        public SoftwareUpdateException(string message, Exception innerException)
            : base(message, innerException)
        {
        }

        /// <summary>
        /// Streaming constructor.
        /// </summary>
        /// <param name="serializationInfo">Serialization info.</param>
        /// <param name="streamingContext">Streaming context.</param>
        protected SoftwareUpdateException(SerializationInfo serializationInfo, StreamingContext streamingContext)
            : base(serializationInfo, streamingContext)
        {
        }
    }
}
