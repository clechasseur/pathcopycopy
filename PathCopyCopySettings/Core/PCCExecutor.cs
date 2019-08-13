// PCCExecutor.cs
// (c) 2015-2019, Charles Lechasseur
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
using System.IO;
using System.Reflection;
using Microsoft.Win32;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.Core
{
    /// <summary>
    /// Wrapper for executing Path Copy Copy DLL functions through rundll32.
    /// </summary>
    public sealed class PCCExecutor
    {
        /// Name of rundll32 executable file.
        private const string RUNDLL32_EXE_NAME = "rundll32.exe";
        
        /// <summary>
        /// Uses the Path Copy Copy DLL to execute the GetPath function for a
        /// given plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to get path from.</param>
        /// <param name="path">Path to pass to the plugin.</param>
        /// <returns>Path returned by the plugin, or an empty string if
        /// the DLL failed to provide the path.</returns>
        /// <exception cref="PCCExecutorException">Thrown when execution fails
        /// for some reason.</exception>
        public string GetPathWithPlugin(Guid pluginId, string path)
        {
            Debug.Assert(path != null);

            // Create a wrapper to know which registry value to use for output.
            string resultingPath;
            using (RegistryOutput output = new RegistryOutput()) {
                // Call PCC via rundll32 for the plugin.
                Call("RegGetPathWithPlugin", String.Format("{0},{1},{2}",
                    pluginId.ToString("B"), output.RegistryValueName, path));

                // Result should be in the registry.
                resultingPath = output.GetOutput();
            }

            return resultingPath ?? String.Empty;
        }
        
        /// <summary>
        /// Uses the Path Copy Copy DLL to apply config revisions to the user
        /// config registry keys. This can be called to ensure config is up-to-date
        /// before trying to modify it.
        /// </summary>
        public void ApplyUserRevisions()
        {
            // Call PCC via rundll32 to apply revisions.
            Call("ApplyUserRevisions", String.Empty);
        }
        
        /// <summary>
        /// Invokes the Path Copy Copy DLL through rundll32 to execute a function
        /// exported by the DLL.
        /// </summary>
        /// <param name="functionName">Name of function to call.</param>
        /// <param name="commandLine">Command-line to pass to the function. See
        /// function description for details.</param>
        /// <exception cref="PCCExecutorException">Thrown when execution fails
        /// for some reason.</exception>
        private void Call(string functionName, string commandLine)
        {
            Debug.Assert(!String.IsNullOrEmpty(functionName));
            Debug.Assert(commandLine != null);

            // Find path to PCC DLL. It's right beside our own executable.
            // In debug settings, it has the default name Visual Studio gave it.
            // When installed, it has a different name depending on bittage.
            string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetEntryAssembly().CodeBase).LocalPath);
            string pccDllPath = Path.Combine(assemblyDir, Resources.PCC_EXECUTOR_DLL_NAME_DEV);
            if (!File.Exists(pccDllPath)) {
                if (PCCEnvironment.Is64Bit && !PCCEnvironment.IsWow64) {
                    pccDllPath = Path.Combine(assemblyDir, Resources.PCC_EXECUTOR_DLL_NAME_64);
                } else {
                    pccDllPath = Path.Combine(assemblyDir, Resources.PCC_EXECUTOR_DLL_NAME_32);
                }
            }
            if (!File.Exists(pccDllPath)) {
                throw new PCCExecutorException("Could not find Path Copy Copy DLL at: {0}", pccDllPath);
            }

            // Execute rundll32. See documentation for rundll32 for details on the
            // way to pass it command-line arguments. Also note that we do not need
            // to execute a specific version of rundll32 (32- or 64-bit), because on
            // 64-bit OSes, the proper version is chosen automatically.
            ProcessStartInfo startInfo = new ProcessStartInfo() {
                FileName = RUNDLL32_EXE_NAME,
                Arguments = String.Format("\"{0}\",{1} {2}", pccDllPath, functionName, commandLine),
                CreateNoWindow = true,
                UseShellExecute = false,
            };
            try {
                using (Process executor = Process.Start(startInfo)) {
                    executor.WaitForExit();
                }
            } catch (Exception e) {
                throw new PCCExecutorException(e);
            }
        }
        
        /// <summary>
        /// Wrapper for the output of the <c>RegGetPathWithPlugin</c> function
        /// called in the PCC DLL through rundll32. The output is stored in a
        /// registry key. Create an instance of this class to call the PCC DLL,
        /// then use it to fetch the output. Disposing of the object will delete
        /// the registry value containing the output.
        /// </summary>
        private sealed class RegistryOutput : IDisposable
        {
            /// Path to registry key containing rundll32 outputs in CURRENT_USER.
            private const string PCC_RUNDLL32_OUTPUT_KEY = @"Software\clechasseur\PathCopyCopy\Rundll32Output";

            /// Registry key wrapper to access the rundll32 output.
            private RegistryKey rundll32OutputKey = Registry.CurrentUser.CreateSubKey(PCC_RUNDLL32_OUTPUT_KEY);

            /// Name of registry value to use for rundll32 output.
            private string regValueName = Guid.NewGuid().ToString("B");

            /// <summary>
            /// Name of registry value where to store the rundll32 output. Pass this
            /// value to the <c>RegGetPathWithPlugin</c> function.
            /// </summary>
            public string RegistryValueName
            {
                get {
                    return regValueName;
                }
            }
            
            /// <summary>
            /// Finalizer. Called if the object hasn't been properly disposed of.
            /// We perform cleanup anyway to make sure that registry value is deleted.
            /// </summary>
            ~RegistryOutput()
            {
                Cleanup();
            }
            
            /// <summary>
            /// Fetches the output of the rundll32 execution from the registry. Call
            /// this after using the <see cref="RegistryValueName"/> in a call to the
            /// PCC DLL through rundll32.
            /// </summary>
            /// <returns>Output of command, or <c>null</c> if output hasn't been
            /// written for some reason.</returns>
            public string GetOutput()
            {
                return (string) rundll32OutputKey.GetValue(regValueName);
            }
            
            /// <inheritDoc/>
            public void Dispose()
            {
 	            Cleanup();
                GC.SuppressFinalize(this);
            }
            
            /// <summary>
            /// Performs cleanup when object is disposed of or finalized. We use this
            /// opportunity to delete the registry value containing the value.
            /// </summary>
            private void Cleanup()
            {
                if (rundll32OutputKey != null) {
                    rundll32OutputKey.DeleteValue(regValueName, false);
                    rundll32OutputKey.Close();
                    rundll32OutputKey = null;
                }
            }
        }
    }
    
    /// <summary>
    /// Exception class used by the <see cref="PCCExecutor"/>.
    /// </summary>
    public class PCCExecutorException : Exception
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public PCCExecutorException()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with exception message.
        /// </summary>
        /// <param name="message">Exception message.</param>
        public PCCExecutorException(string message)
            : base(message)
        {
        }
        
        /// <summary>
        /// Constructor with formatted exception message.
        /// </summary>
        /// <param name="format">Format string.</param>
        /// <param name="args">Format arguments.</param>
        public PCCExecutorException(string format, params object[] args)
            : base(String.Format(format, args))
        {
        }
        
        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        public PCCExecutorException(Exception innerException)
            : base(innerException.Message, innerException)
        {
        }
    }
}
