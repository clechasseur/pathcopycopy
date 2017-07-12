README-DEV.TXT
PathCopyCopy project
====================

This file is a quick developer's guide to the files and folders found in the
Mercurial repository of the PathCopyCopy project.


1. PathCopyCopy.sln

Visual Studio solution file for the various PathCopyCopy projects. Built using
Visual Studio 2017 Community Edition, but should be compatible with later
versions as well.


2. PathCopyCopy

Contains the PathCopyCopy project and source files. This project contains the main
Path Copy Copy contextual menu extension, implemented as a COM object. The project
is written in C++ using ATL.


3. PathCopyCopySettings

Contains the PathCopyCopySettings project and source files. It builds the UI used
when the user selects "Settings" from the contextual menu to setup Path Copy Copy.
The project is written in C# and targets .NET 2.0. 


4. PathCopyCopyRegexTester

Contains the PathCopyCopyRegexTester project and source files. This is an
interactive tool written in C++ to test the regular expression capabilities
of PathCopyCopy's custom commands.


5. PathCopyCopyCOMPluginExecutor

Contains the PathCopyCopyCOMPluginExecutor project and source files. This is
an interactive tool written in C++ to be able to call a method on COM plugins
regardless of bittage from the Settings application.


6. icons

Contains icons used in resources of the various PathCopyCopy projects.


7. Testing

Contains an alternative shell program, mtExplorer, which I used to debug
PathCopyCopy during development. It's useful to avoid killing explorer.exe
every time you need to run again. Also contains some test files and
directories I used during development, as well as a project containing
test plugins.


8. 3rdParty

Contains third-party tools that are used by the Path Copy Copy projects -
in post-build steps, for example.


9. Installer\Setup.iss

The project's installation script. The script was made with Inno Setup
(http://www.jrsoftware.org/isinfo.php), which is free software. In order
to build the installer, you will have to download the Inno Setup Compiler.
Open the .iss script in the Compiler and choose "Compile". (Make sure
you built all PathCopyCopy projects with Visual Studio first, both
Release|Win32 and Release|x64 configurations.)


10. Installer\SetupPortable.iss

The project's portable installation script. This is similar to the
standard installer, but installs Path Copy Copy for the current user only.
It has the same requirements as Setup.iss to build; open the .iss script
in the Inno Setup Compiler and choose "Compile".


11. Installer\MSIPackage.xml

Configuration file to build the project's Windows Installer package. The
configuration file is for MSI Wrapper (http://www.exemsi.com/) - the free
version works fine. Open MSI Wrapper and choose "Load Settings". Navigate
to the Executable page and replace the "Setup Executable Input File Name"
and "MSI Output File Name" with full paths to their respective location.
Then, navigate to the last page and choose "Build" to build the package.
(Make sure you first built the regular installer with Inno Setup, above.)


12. Samples

Directory containing various sample codes related to Path Copy Copy,
like sample projects for COM plugins. Each project has its own folder.


13. Schemas

Contains XML schemas for various XML formats used in PathCopyCopy projects.


14. screenshots

Backup of the project screenshots featured on CodePlex.


15. HISTORY.TXT

Version history.


16. LICENSE.TXT

Free-software license used by the PathCopyCopy project. Corresponds to the MIT license.
