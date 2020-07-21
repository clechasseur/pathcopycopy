// Program.cs
// (c) 2011-2020, Charles Lechasseur
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
using System.Diagnostics;
using System.Globalization;
using System.Threading;
using System.Windows.Forms;
using CommandLine;
using PathCopyCopy.Settings.Core;
using PathCopyCopy.Settings.Properties;
using PathCopyCopy.Settings.UI.Forms;

namespace PathCopyCopy.Settings
{
    /// <summary>
    /// Main program class.
    /// </summary>
    static class Program
    {
        /// Name of global mutex used for blocking multiple instances.
        const string MutexName = "PathCopyCopySettings.Mutex";

        /// Name of global mutex used for blocking multiple update checks.
        const string UpdateCheckMutexName = "PathCopyCopySettings.UpdateCheck.Mutex";

        /// <summary>
        /// Possible bitness used when starting the program.
        /// <see cref="Default"/> will use the default bitness
        /// (64-bit on a 64-bit OS, otherwise 32-bit).
        /// </summary>
        enum Bitness
        {
            Default,
            x86,
            x64,
        }
        
        /// <summary>
        /// Bean class used to parse command-line arguments using <see cref="CommandLine.Parser"/>
        /// </summary>
        class PCCSettingsArgs
        {
#pragma warning disable 0649

            /// <summary>
            /// Indicates in which bitness the app should execute.
            /// If this is not specified, we use the OS bitness instead.
            /// </summary>
            [Argument(ArgumentType.AtMostOnce, DefaultValue=Bitness.Default)]
            public Bitness bitness;

            /// <summary>
            /// Indicates that app should check for updates instead of opening normally.
            /// </summary>
            [Argument(ArgumentType.AtMostOnce, DefaultValue=false)]
            public bool updatecheck;

            /// <summary>
            /// Indicates that we've been launched by the Path Copy Copy contextual menu
            /// extension. In such a case, we bypass the revise process.
            /// </summary>
            [Argument(ArgumentType.AtMostOnce, DefaultValue=false)]
            public bool frompcc;

#pragma warning restore 0649
        }
        
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            // Parse arguments.
            PCCSettingsArgs pccArgs = new PCCSettingsArgs();
            Parser.ParseArguments(args, pccArgs);

            // Set bitness in PCC environment.
            switch (pccArgs.bitness) {
                case Bitness.x86: {
                    PCCEnvironment.Is64Bit = false;
                    break;
                }
                case Bitness.x64: {
                    PCCEnvironment.Is64Bit = true;
                    break;
                }
                case Bitness.Default: {
                    PCCEnvironment.Is64Bit = PCCEnvironment.Is64BitOS;
                    break;
                }
                default: {
                    Debug.Fail($"Unknown Bitness value: {pccArgs.bitness}");
                    PCCEnvironment.Is64Bit = PCCEnvironment.Is64BitOS;
                    break;
                }
            }

            // Setup visual styles.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // Set culture info to switch language if needed.
            using (UserSettings settings = new UserSettings()) {
                string language = settings.Language;
                if (!string.IsNullOrWhiteSpace(language)) {
                    CultureInfo cultureInfo = CultureInfo.CreateSpecificCulture(language);
                    Thread.CurrentThread.CurrentUICulture = cultureInfo;
                    Thread.CurrentThread.CurrentCulture = cultureInfo;
                }
            }

            // Check if we need to simply check for updates.
            if (pccArgs.updatecheck) {
                // Make sure we only do this once.
                using (new Mutex(true, UpdateCheckMutexName, out bool created)) {
                    if (!created) {
                        return;
                    }

                    using (UserSettings settings = new UserSettings()) {
                        try {
                            SoftwareUpdater updater = new SoftwareUpdater(settings);
                            updater.CheckForUpdate(null);
                        } catch (SoftwareUpdateException) {
                            // Silence this. Can't check for updates, too bad.
                        }
                        return;
                    }
                }
            }

            // Block multiple instances.
            using (new Mutex(true, MutexName, out bool created)) {
                if (!created) {
                    return;
                }

                // Unless we're launched by the PCC extension, perform revise.
                if (!pccArgs.frompcc) {
                    new PCCExecutor().ApplyUserRevisions();
                }

                // Make sure editing is allowed.
                using (UserSettings settings = new UserSettings()) {
                    if (settings.EditingDisabled) {
                        MessageBox.Show(Resources.EDITING_DISABLED_MESSAGE, Resources.EDITING_DISABLED_TITLE,
                            MessageBoxButtons.OK, MessageBoxIcon.Stop);
                        return;
                    }
                }

                // If we reached this point, run the application.
                using (MainForm mainForm = new MainForm()) {
                    Application.Run(mainForm);
                }
            }
        }
    }
}
