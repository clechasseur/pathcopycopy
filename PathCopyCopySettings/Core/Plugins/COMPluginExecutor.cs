// COMPluginExecutor.cs
// (c) 2014-2019, Charles Lechasseur
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
using System.Text.RegularExpressions;
using PathCopyCopy.Settings.Properties;

namespace PathCopyCopy.Settings.Core.Plugins
{
    /// <summary>
    /// Wrapper for the COM plugin executor program used to invoke COM plugins
    /// from the Settings app. Will take care of calling the proper version
    /// of the executor depending on whether we need to call a 32-bit or
    /// 64-bit COM plugin.
    /// </summary>
    public sealed class COMPluginExecutor
    {
        /// Regex used to extract the output of the COM plugin executor program.
        private static readonly Regex OUTPUT_REGEX = new Regex(String.Format(@"^{0}(.*)$",
            Resources.COM_PLUGIN_EXECUTOR_OUTPUT_PREFIX), RegexOptions.Compiled);

        /// Prefix of output lines from COM plugin executor that indicates an error.
        private const string EXECUTOR_ERROR_PREFIX = "ERROR";

        /// Output returned by the COM plugin executor for a command that returns a boolean "true" result.
        private const string EXECUTOR_TRUE_OUTPUT = "true";

        /// Output returned by the COM plugin executor for a command that returns a boolean "false" result.
        private const string EXECUTOR_FALSE_OUTPUT = "false";
        
        /// <summary>
        /// Invokes the COM plugin executor program to get the description for a
        /// COM plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns>Plugin description.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public string GetDescription(Guid pluginId)
        {
            return Call(pluginId, "get_Description");
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to get the help text for a
        /// COM plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns>Plugin help text.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public string GetHelpText(Guid pluginId)
        {
            return Call(pluginId, "get_HelpText");
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to get the group ID for a
        /// COM plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns>ID of group for the plugin.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public int GetGroupId(Guid pluginId)
        {
            int groupId;
            return Int32.TryParse(Call(pluginId, "get_GroupId"), out groupId) ? groupId : 0;
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to get the group position for
        /// a COM plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns>Group position for the plugin.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public int GetGroupPosition(Guid pluginId)
        {
            int groupPosition;
            return Int32.TryParse(Call(pluginId, "get_GroupPosition"), out groupPosition) ? groupPosition : 0;
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to get the icon file for a
        /// COM plugin.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns>Icon file for the plugin.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public string GetIconFile(Guid pluginId)
        {
            return Call(pluginId, "get_IconFile");
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to check if a COM plugin wants
        /// to use the default icon.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <returns><c>true</c> to use the default icon, <c>false</c> otherwise.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        public bool GetUseDefaultIcon(Guid pluginId)
        {
            return Call(pluginId, "get_UseDefaultIcon") == EXECUTOR_TRUE_OUTPUT;
        }
        
        /// <summary>
        /// Invokes the COM plugin executor program to execute the given command
        /// and returns the result.
        /// </summary>
        /// <param name="pluginId">ID of plugin to invoke.</param>
        /// <param name="command">Command to execute.</param>
        /// <returns>Result of the execution.</returns>
        /// <exception cref="COMPluginExecutorException">Thrown when execution
        /// fails for some reason.</exception>
        private string Call(Guid pluginId, string command)
        {
            string output = null;

            // Find path to executor program. It's right beside our own executable.
            string assemblyPath = new Uri(Assembly.GetEntryAssembly().CodeBase).LocalPath;
            string executorFileName;
            if (PCCEnvironment.Is64Bit && !PCCEnvironment.IsWow64) {
                executorFileName = Resources.COM_PLUGIN_EXECUTOR_EXE_NAME_64;
            } else {
                executorFileName = Resources.COM_PLUGIN_EXECUTOR_EXE_NAME_32;
            }
            string executorPath = Path.Combine(Path.GetDirectoryName(assemblyPath), executorFileName);
            if (!File.Exists(executorPath)) {
                throw new COMPluginExecutorException("Could not find COM plugin executor program at: {0}", executorPath);
            }

            // Launch tester program, grabbing input and output.
            ProcessStartInfo startInfo = new ProcessStartInfo(executorPath) {
                CreateNoWindow = true,
                RedirectStandardError = true,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
            };
            try {
                using (Process executor = Process.Start(startInfo)) {
                    // Get standard input and enter the arguments.
                    StreamWriter cin = executor.StandardInput;
                    cin.WriteLine(pluginId.ToString("B"));
                    cin.WriteLine(command);

                    // Get standard output, parse lines and get command output.
                    StreamReader cout = executor.StandardOutput;
                    string line = cout.ReadLine();
                    while (line != null) {
                        Match match = OUTPUT_REGEX.Match(line);
                        if (match.Success) {
                            // We got our command output.
                            output = match.Groups[1].Value;
                            break;
                        } else if (line.StartsWith(EXECUTOR_ERROR_PREFIX)) {
                            // An error occured during execution.
                            throw new COMPluginExecutorException("COM plugin execution failed. Error: {0}", line);
                        }

                        // Read next line.
                        line = cout.ReadLine();
                    }

                    // Wait for the process to end.
                    executor.WaitForExit();

                    // Throw if the execution failed.
                    if (executor.ExitCode < 0) {
                        throw new COMPluginExecutorException("COM plugin execution failed. Exit code: {0}.", executor.ExitCode);
                    }

                    // Make sure we found output.
                    if (output == null) {
                        throw new COMPluginExecutorException("COM plugin execution did not return expected output. " +
                            "Exit code: {0}.", executor.ExitCode);
                    }
                }
            } catch (COMPluginExecutorException) {
                throw;
            } catch (Exception e) {
                throw new COMPluginExecutorException(e);
            }

            // Return the output to user.
            Debug.Assert(output != null);
            return output;
        }
    }
    
    /// <summary>
    /// Exception class used by the <see cref="COMPluginExecutor"/>.
    /// </summary>
    public class COMPluginExecutorException : Exception
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public COMPluginExecutorException()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with exception message.
        /// </summary>
        /// <param name="message">Exception message.</param>
        public COMPluginExecutorException(string message)
            : base(message)
        {
        }
        
        /// <summary>
        /// Constructor with formatted exception message.
        /// </summary>
        /// <param name="format">Format string.</param>
        /// <param name="args">Format arguments.</param>
        public COMPluginExecutorException(string format, params object[] args)
            : base(String.Format(format, args))
        {
        }
        
        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        public COMPluginExecutorException(Exception innerException)
            : base(innerException.Message, innerException)
        {
        }
    }
}
