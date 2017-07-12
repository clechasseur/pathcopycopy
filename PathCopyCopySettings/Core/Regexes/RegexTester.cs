// RegexTester.cs
// (c) 2012-2017, Charles Lechasseur
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

namespace PathCopyCopy.Settings.Core.Regexes
{
    /// <summary>
    /// Wrapper for the regex tester program used to test regular expressions.
    /// </summary>
    public sealed class RegexTester
    {
        /// Line prefix used by the regex tester program to indicate an error.
        private const string REGEX_TESTER_ERROR_PREFIX = "ERROR:";

        /// Regex used to identify the modified string output by the regex tester program.
        private static readonly Regex MODIFIED_STRING_REGEX = new Regex(String.Format(@"^{0}(.*)$",
                Resources.REGEX_TESTER_MODIFIED_STRING_PREFIX), RegexOptions.Compiled);
        
        /// <summary>
        /// Invokes the regex tester program with the given arguments and returns
        /// the modified string.
        /// </summary>
        /// <param name="sample">Sample string to modify.</param>
        /// <param name="regex">Regex used to find matches.</param>
        /// <param name="format">Format of replacement string.</param>
        /// <param name="ignoreCase">Whether to ignore case when looking for
        /// matches.</param>
        /// <returns>Modified string, as returned by the tester program.</returns>
        public string ModifyWithRegex(string sample, string regex, string format, bool ignoreCase)
        {
            string modified = null;

            // Find path to tester program. It's right beside our own executable.
            string assemblyPath = new Uri(Assembly.GetEntryAssembly().CodeBase).LocalPath;
            string testerPath = Path.Combine(Path.GetDirectoryName(assemblyPath), Resources.REGEX_TESTER_EXE_NAME);
            if (!File.Exists(testerPath)) {
                throw new RegexTesterException("Could not find regex tester program at: {0}", testerPath);
            }

            // Launch tester program, grabbing input and output.
            ProcessStartInfo startInfo = new ProcessStartInfo(testerPath) {
                CreateNoWindow = true,
                RedirectStandardError = true,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
            };
            try {
                using (Process tester = Process.Start(startInfo)) {
                    // Get standard input and enter the arguments.
                    StreamWriter cin = tester.StandardInput;
                    cin.WriteLine(sample);
                    cin.WriteLine(regex);
                    cin.WriteLine(format);
                    cin.WriteLine(ignoreCase ? 'y' : 'n');

                    // Get standard output, parse lines and get modified string.
                    StreamReader cout = tester.StandardOutput;
                    string line = cout.ReadLine();
                    while (line != null) {
                        Match match = MODIFIED_STRING_REGEX.Match(line);
                        if (match.Success) {
                            // We got our modified string.
                            modified = match.Groups[1].Value;
                            break;
                        } else if (line.StartsWith(REGEX_TESTER_ERROR_PREFIX)) {
                            // This indicates an invalid regular expression.
                            throw new RegexTesterException("Invalid regular expression.");
                        }

                        // Read next line.
                        line = cout.ReadLine();
                    }

                    // Wait for the process to end.
                    tester.WaitForExit();
                }
            } catch (RegexTesterException) {
                throw;
            } catch (Exception e) {
                throw new RegexTesterException(e);
            }

            // Make sure we found the modified string.
            if (modified == null) {
                throw new RegexTesterException("Could not find modified string in regex tester program output.");
            }

            // Return modified string as provided by the program.
            return modified;
        }
    }
    
    /// <summary>
    /// Exception class used by the <see cref="RegexTester"/>.
    /// </summary>
    public class RegexTesterException : Exception
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public RegexTesterException()
            : base()
        {
        }
        
        /// <summary>
        /// Constructor with exception message.
        /// </summary>
        /// <param name="message">Exception message.</param>
        public RegexTesterException(string message)
            : base(message)
        {
        }
        
        /// <summary>
        /// Constructor with formatted exception message.
        /// </summary>
        /// <param name="format">Format string.</param>
        /// <param name="args">Format arguments.</param>
        public RegexTesterException(string format, params object[] args)
            : base(String.Format(format, args))
        {
        }
        
        /// <summary>
        /// Constructor with inner exception.
        /// </summary>
        public RegexTesterException(Exception innerException)
            : base(innerException.Message, innerException)
        {
        }
    }
}
