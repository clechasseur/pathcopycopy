// Plugin.cs
// (c) 2011-2018, Charles Lechasseur
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
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// Base class representing a Path Copy Copy plugin. Contains the plugin
    /// description as well as its unique ID.
    /// </summary>
    abstract public class Plugin : IComparable<Plugin>, IComparable,
        IEquatable<Plugin>, INotifyPropertyChanged
    {
        /// Plugin ID.
        private Guid id;

        /// Plugin description.
        private string description;

        /// Cached version of preview as returned by PCC DLL.
        private string preview;

        /// <summary>
        /// Path to use to generate previews.
        /// </summary>
        public static readonly string PREVIEW_PATH;

        /// <summary>
        /// Unique plugin ID: a GUID.
        /// </summary>
        public Guid Id
        {
            get {
                return id;
            }
            protected set {
                id = value;
                CallPropertyChanged("Id");
            }
        }

        /// <summary>
        /// Plugin description, as it appears in the PCC contextual menu.
        /// </summary>
        public string Description
        {
            get {
                return description;
            }
            protected set {
                description = value;
                CallPropertyChanged("Description");
            }
        }

        /// <summary>
        /// Path to the icon file to use for this plugin. If an empty string
        /// is set, the plugin will use the default icon. If this value is
        /// set to <c>null</c>, the plugin will display no icon.
        /// </summary>
        /// <remarks>
        /// The <see cref="Plugin"/> class doesn't support this by default
        /// and will always return <c>null</c>. Calling the setter will
        /// cause an assertion to be raised. Subclasses that support icon files
        /// must override this to implement it the way they see fit.
        /// </remarks>
        public virtual string IconFile
        {
            get {
                return null;
            }
            set {
                Debug.Fail("This plugin type doesn't support icon files");
            }
        }

        /// <summary>
        /// Event fired when a property of the plugin changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;
        
        /// <summary>
        /// Static constructor. Initializes our read-only static fields.
        /// </summary>
        static Plugin()
        {
            // Use path to this executable as preview. It will most likely contain
            // spaces and long names, which is perfect to showcase short paths.
            string pathToThisExe = new Uri(Assembly.GetEntryAssembly().CodeBase).LocalPath;
            PREVIEW_PATH = Path.ChangeExtension(pathToThisExe, Path.GetExtension(pathToThisExe).ToLower());
        }
        
        /// <summary>
        /// Creates a new Plugin bean.
        /// </summary>
        /// <param name="id">Plugin unique ID.</param>
        /// <param name="description">Plugin description.</param>
        public Plugin(Guid id, string description)
        {
            this.id = id;
            this.description = description;
        }
        
        /// <summary>
        /// Creates a new Plugin bean from a string ID.
        /// </summary>
        /// <param name="id">Plugin unique ID's string representation.</param>
        /// <param name="description">Plugin description.</param>
        public Plugin(string id, string description)
            : this(new Guid(id), description)
        {
        }
        
        /// <summary>
        /// Returns the preview of the effect of the plugin on a hardcoded path.
        /// </summary>
        /// <param name="userSettings">Object to access user settings.</param>
        /// <returns>Preview of the effect of this plugin on a path.</returns>
        public virtual string GetPreview(UserSettings userSettings)
        {
            if (preview == null) {
                try {
                    preview = new PCCExecutor().GetPathWithPlugin(Id, PREVIEW_PATH);
                } catch (PCCExecutorException) {
                    preview = Resources.Plugin_PreviewError;
                }
            }
            Debug.Assert(preview != null);
            return preview;
        }
        
        /// <summary>
        /// Returns a string representation of the plugin (its description).
        /// </summary>
        public override string ToString()
        {
            return Description;
        }

        /// <summary>
        /// Compares this plugin to <paramref name="other"/> using their IDs.
        /// </summary>
        /// <param name="other">Plugin whose ID to compare to this one's.</param>
        /// <returns>Comparison result.</returns>
        public int CompareTo(Plugin other)
        {
            // Null elements are "smaller"
            return other != null ? Id.CompareTo(other.Id) : 1;
        }

        /// <summary>
        /// Compares this plugin to another arbitrary object.
        /// If <paramref name="obj"/> is a <see cref="Plugin"/>,
        /// this will compare the plugins using their IDs;
        /// otherwise the result is undefined.
        /// </summary>
        /// <param name="obj">Object to compare to this plugin.</param>
        /// <returns>Comparison result.</returns>
        public int CompareTo(object obj)
        {
            return (obj != null && obj is Plugin) ? CompareTo((Plugin) obj) : 1;
        }

        /// <summary>
        /// Checks if two <see cref="Plugin"/>s are equal (e.g. have the same ID).
        /// </summary>
        /// <param name="other">Plugin whose ID to compare to this one's.</param>
        /// <returns><c>true</c> if <paramref name="other"/> has the same ID
        /// as this plugin.</returns>
        public bool Equals(Plugin other)
        {
            return other != null ? Id.Equals(other.Id) : false;
        }

        /// <summary>
        /// Calls the <see cref="PropertyChanged"/> event to indicate that
        /// the property <paramref name="propertyName"/> has changed.
        /// </summary>
        /// <param name="propertyName">Name of property that has changed.
        /// Passing <c>null</c> or an empty string means all properties
        /// have changed.</param>
        protected void CallPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    
    /// <summary>
    /// Special implementation of the <see cref="Plugin"/> class that represents
    /// a separator in the contextual menu.
    /// </summary>
    public sealed class SeparatorPlugin : Plugin
    {
        /// Unique ID of the separator plugin.
        private static readonly Guid PLUGIN_SEPARATOR_ID = new Guid("{AFA4D1E1-BA73-4330-A6AB-E574FF39ECC3}");

        /// Description used for the separator plugin.
        private const string PLUGIN_SEPARATOR_DESCRIPTION = "-----------------------------------------";
        
        /// <summary>
        /// Creates a separator plugin bean.
        /// </summary>
        public SeparatorPlugin()
            : base(PLUGIN_SEPARATOR_ID, PLUGIN_SEPARATOR_DESCRIPTION)
        {
        }
        
        /// <inheritDoc/>
        public override string GetPreview(UserSettings userSettings)
        {
            // Separators don't need previews; this shouldn't be called.
            Debug.Fail("Separator plugins don't have previews");
            return String.Empty;
        }
    }

    /// <summary>
    /// <see cref="Plugin"/> subclass used for default (e.g. builtin) plugins.
    /// </summary>
    public sealed class DefaultPlugin : Plugin
    {
        /// Path to icon file for plugin.
        private string iconFile;

        /// <inheritDoc/>
        public override string IconFile
        {
            get {
                return iconFile;
            }
            set {
                iconFile = value;
                CallPropertyChanged("IconFile");
            }
        }
        
        /// <summary>
        /// Creates a new default plugin bean.
        /// </summary>
        /// <param name="id">Plugin unique ID's string representation.</param>
        /// <param name="description">Plugin description.</param>
        /// <param name="iconFile">Path to icon file for plugin, or an empty
        /// string to use the default icon, or <c>null</c> to avoid showing
        /// an icon for this plugin.</param>
        public DefaultPlugin(string id, string description, string iconFile)
            : base(id, description)
        {
            this.iconFile = iconFile;
        }
    }
}
