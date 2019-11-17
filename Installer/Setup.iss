; Inno Setup script file for PathCopyCopy
; Copyright (c) 2008-2019 Charles Lechasseur
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.

; PER_USER can be defined on the command-line or in a script before including this.
; It will generate an installer for the portable version of Path Copy Copy.

#ifndef PCC_DEBUG
  #define MyConfiguration "Release"
  #ifdef PER_USER
    #define MyAppName "Path Copy Copy (Portable)"
    #define MyAppVersion "17.1.1"
    #define MyAppFullVersion "17.1.1"
    #define MyAppVerName "Path Copy Copy (Portable) 17.1.1"
  #else
    #define MyAppName "Path Copy Copy"
    #define MyAppVersion "17.1.1"
    #define MyAppFullVersion "17.1.1"
    #define MyAppVerName "Path Copy Copy 17.1.1"
  #endif
#else
  #define MyConfiguration "Debug"
  #define MyAppName "Path Copy Copy DEBUG"
  #define MyAppVersion "17.1.1"
  #define MyAppFullVersion "17.1.1"
  #define MyAppVerName "Path Copy Copy DEBUG 17.1.1"
#endif
#define MyAppPublisher "Charles Lechasseur"
#define MyAppURL "https://pathcopycopy.github.io/"
#define MyAppDescription "Path Copy Copy Shell Contextual Menu Extension"
#define MyAppCopyright "(c) 2008-2019, Charles Lechasseur. See LICENSE.TXT for details."
#define MyLicenseFile "..\LICENSE"

; Comment this to avoid signing the resulting executable.
#define SIGNCODE

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
; Note: use the same app ID for debug and release installations, since
; it's really the same app and uses the same registry values and everything.
#ifdef PER_USER
AppId={{97C7820F-10CF-4528-AE14-6E6EF5E6459C}
#else
AppId={{3C01F274-867C-4D1D-BE8C-CB488C31B0C9}
#endif
AppName={#MyAppName}
AppVersion={#MyAppFullVersion}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
#ifdef PER_USER
DefaultDirName={userpf}\{#MyAppName}
#else
DefaultDirName={pf}\{#MyAppName}
#endif
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#MyLicenseFile}
#ifndef PCC_DEBUG
#ifdef PER_USER
OutputBaseFilename=PathCopyCopyPortable{#MyAppVersion}
#else
OutputBaseFilename=PathCopyCopy{#MyAppVersion}
#endif
#else
OutputBaseFilename=PathCopyCopyDebug{#MyAppVersion}
#endif
Compression=lzma
SolidCompression=yes
VersionInfoVersion={#MyAppFullVersion}
VersionInfoDescription={#MyAppDescription}
VersionInfoTextVersion={#MyAppFullVersion}
ArchitecturesInstallIn64BitMode=x64
VersionInfoCopyright={#MyAppCopyright}
DisableReadyMemo=yes
AlwaysShowComponentsList=no
DisableReadyPage=yes
CloseApplications=no
OutputDir=Output
MinVersion=5.1
#ifdef SIGNCODE
SignTool=certum
#endif
#ifdef PER_USER
PrivilegesRequired=lowest
#endif

[Languages]
Name: english; MessagesFile: compiler:Default.isl

#ifdef PER_USER
[Messages]
WelcomeLabel2=This will install [name/ver] for this user only.%n%nIt is recommended that you close all other applications before continuing.
#endif

[Files]
Source: ..\bin\Win32\{#MyConfiguration}\PathCopyCopy.dll; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly; DestName: PCC32.dll
Source: ..\bin\x64\{#MyConfiguration}\PathCopyCopy.dll; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly; DestName: PCC64.dll; Check: Is64BitInstallMode
Source: ..\bin\Win32\{#MyConfiguration}\PathCopyCopySettings.exe; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly
Source: ..\bin\Win32\{#MyConfiguration}\PathCopyCopyRegexTester.exe; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly
Source: ..\bin\Win32\{#MyConfiguration}\PathCopyCopyCOMPluginExecutor32.exe; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly
Source: ..\bin\x64\{#MyConfiguration}\PathCopyCopyCOMPluginExecutor64.exe; DestDir: {app}; Flags: ignoreversion restartreplace overwritereadonly uninsrestartdelete uninsremovereadonly; Check: Is64BitInstallMode
Source: ..\LICENSE; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly; DestName: LICENSE.TXT
Source: ..\LICENSE.cl_optional; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly; DestName: LICENSE.cl_optional.TXT
Source: ..\LICENSE.CommandLineArguments; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly; DestName: LICENSE.CommandLineArguments.TXT
Source: ..\HISTORY; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly; DestName: HISTORY.TXT
Source: ..\Schemas\PipelinePluginCollection.xsd; DestDir: {app}\Schemas; Flags: overwritereadonly uninsremovereadonly
Source: ..\obj\Win32\{#MyConfiguration}\PathCopyCopy\PathCopyCopy.tlb; DestDir: {app}\Type Libraries\Win32; Flags: overwritereadonly uninsremovereadonly
Source: ..\obj\x64\{#MyConfiguration}\PathCopyCopy\PathCopyCopy.tlb; DestDir: {app}\Type Libraries\x64; Flags: overwritereadonly uninsremovereadonly
Source: ..\Samples\SampleCOMPluginCpp\SampleCOMPlugin.sln; DestDir: {app}\Samples\Plugins\COM\C++
Source: ..\Samples\SampleCOMPluginCpp\SampleCOMPlugin\*; DestDir: {app}\Samples\Plugins\COM\C++\SampleCOMPlugin; Excludes: dlldata.c,SampleCOMPlugin_i.h,SampleCOMPlugin_i.c,SampleCOMPlugin_p.c,*.aps,*.user,*.ncb,*.suo,*.sdf,*.VC.db,*.ipch
Source: ..\Samples\SampleCOMPluginCSharp\SampleCOMPlugin.sln; DestDir: {app}\Samples\Plugins\COM\C#
Source: ..\Samples\SampleCOMPluginCSharp\SampleCOMPlugin\*; DestDir: {app}\Samples\Plugins\COM\C#\SampleCOMPlugin; Excludes: *.aps,*.user,*.ncb,*.suo,*.sdf,*.VC.db,*.ipch
Source: ..\Samples\SampleCOMPluginCSharp\SampleCOMPlugin\api\*; DestDir: {app}\Samples\Plugins\COM\C#\SampleCOMPlugin\api; Flags: recursesubdirs
Source: ..\Samples\SampleCOMPluginCSharp\SampleCOMPlugin\Properties\*; DestDir: {app}\Samples\Plugins\COM\C#\SampleCOMPlugin\Properties

[Icons]
Name: {group}\Path Copy Copy Settings; Filename: {app}\PathCopyCopySettings.exe; Flags: excludefromshowinnewinstall

[Registry]
#ifdef PER_USER
Root: HKCU; Subkey: Software\clechasseur\PathCopyCopy; ValueType: string; ValueName: InstallSource; ValueData: {code:InstallSourceValue}; Flags: uninsdeletevalue
#else
Root: HKLM; Subkey: Software\clechasseur\PathCopyCopy; ValueType: string; ValueName: InstallSource; ValueData: {code:InstallSourceValue}; Flags: uninsdeletevalue
Root: HKLM32; Subkey: Software\clechasseur\PathCopyCopy; ValueType: string; ValueName: InstallSource; ValueData: {code:InstallSourceValue}; Flags: uninsdeletevalue; Check: Is64BitInstallMode
Root: HKLM32; Subkey: Software\clechasseur\PathCopyCopy\PipelinePlugins; Flags: uninsdeletekeyifempty 
Root: HKLM32; Subkey: Software\clechasseur\PathCopyCopy\Plugins; Flags: uninsdeletekeyifempty
Root: HKLM32; Subkey: Software\clechasseur\PathCopyCopy; Flags: uninsdeletekeyifempty
Root: HKLM64; Subkey: Software\clechasseur\PathCopyCopy\PipelinePlugins; Flags: uninsdeletekeyifempty; Check: IsWin64
Root: HKLM64; Subkey: Software\clechasseur\PathCopyCopy\Plugins; Flags: uninsdeletekeyifempty; Check: IsWin64
Root: HKLM64; Subkey: Software\clechasseur\PathCopyCopy; Flags: uninsdeletekeyifempty; Check: IsWin64

; A typo made previous versions of the DLL register approved extensions in the wrong key.
Root: HKCR; Subkey: Software\Microsoft\Windows\CurrentVersion\Extensions\Approved; ValueName: {{82CB99A2-2F18-4D5D-9476-54347E3B6720}; Flags: deletevalue
Root: HKCR; Subkey: Software\Microsoft\Windows\CurrentVersion\Extensions\Approved; ValueName: {{16170CA5-25CA-4e6d-928C-7A3A974F4B56}; Flags: deletevalue
Root: HKCR; Subkey: Software\Microsoft\Windows\CurrentVersion\Extensions\Approved; Flags: uninsdeletekeyifempty
Root: HKCR; Subkey: Software\Microsoft\Windows\CurrentVersion\Extensions; Flags: uninsdeletekeyifempty
Root: HKCR; Subkey: Software\Microsoft\Windows\CurrentVersion; Flags: uninsdeletekeyifempty
Root: HKCR; Subkey: Software\Microsoft\Windows; Flags: uninsdeletekeyifempty
Root: HKCR; Subkey: Software\Microsoft; Flags: uninsdeletekeyifempty
Root: HKCR; Subkey: Software; Flags: uninsdeletekeyifempty
#endif

[InstallDelete]
Type: files; Name: {group}\Settings.lnk
Type: files; Name: {group}\License.lnk
Type: files; Name: {group}\Path Copy Copy License.lnk
Type: files; Name: {app}\Path Copy Copy on CodePlex.url

[Run]
Filename: {sys}\regsvr32.exe; Parameters: "{code:Regsvr32InstallParameters} ""{app}\PCC32.dll"""; WorkingDir: {app}; StatusMsg: {code:GetStatusRegisterFiles}; Flags: runhidden 32bit
Filename: {sys}\regsvr32.exe; Parameters: "{code:Regsvr32InstallParameters} ""{app}\PCC64.dll"""; WorkingDir: {app}; StatusMsg: {code:GetStatusRegisterFiles}; Flags: runhidden 64bit; Check: Is64BitInstallMode

[UninstallRun]
Filename: {sys}\regsvr32.exe; Parameters: "{code:Regsvr32InstallParameters|/u} ""{app}\PCC32.dll"""; WorkingDir: {app}; RunOnceId: UnregisterPCC32; Flags: runhidden 32bit
Filename: {sys}\regsvr32.exe; Parameters: "{code:Regsvr32InstallParameters|/u} ""{app}\PCC64.dll"""; WorkingDir: {app}; RunOnceId: UnregisterPCC64; Flags: runhidden 64bit; Check: Is64BitInstallMode

[Code]
const
  // Root key where to write settings during installation. Will be set to HKLM
  // for regular installations and HKCU for portable installations.
#ifdef PER_USER
  CSettingsRootKey = HKEY_CURRENT_USER;
#else
  CSettingsRootKey = HKEY_LOCAL_MACHINE;
#endif

  CDwordSwitch = '/PCCREGVALUEDWORD=';
  CStringSwitch = '/PCCREGVALUESZ=';
  CPipelinePluginSwitch = '/PCCREGPIPELINEPLUGIN=';
  CLegacyPipelinePluginsDisplayOrderSwitch = '/PCCREGPIPELINEPLUGINSDISPLAYORDER=';
  CInitialCommandsChoiceSwitch = '/INITIALCOMMANDSCHOICE=';
  CInstallSourceSwitch = '/INSTALLSOURCE=';
  CStringSeparator = ',';
    
  CCommonCommandsChoice = 0;
  CNetworkCommandsChoice = 1;
  CAllCommandsChoice = 2;
  
#ifdef PER_USER
  CDefaultInstallSource = 'Portable';
#else
  CDefaultInstallSource = 'Inno';
#endif

  // This must be updated every release that contains a new plugin.
  CKnownPlugins = '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +
    '{c8757dca-1ba8-48d0-8145-b93bbb9237af},{3a42b5c6-72d6-4a8a-ba44-014fa64aa11a},' +
    '{93b362d1-2b13-467a-b618-6784f24f3ba1},{331a3b60-af49-44f4-b30d-56adff6d25e8},' +
    '{0929538b-5f52-4480-9902-2bbec9c84905},{349939d7-780f-43fd-a98e-83d9add44e22},' +
    '{891f9e3e-0b70-49fd-a86c-53b21c6193c7},{ea24bcc3-35f5-46b9-a2a5-a12a0aed2d28},' +
    '{73188fb3-8e14-409c-95ef-ba608fdc1274},{e2c942ac-917c-4aee-a867-8f6ab960ba76},' +
    '{8f2adccc-9693-407d-9300-fccb9a12b982},{5b5da5cb-3284-45a9-a1e5-4d6b03107924},' +
    '{cd50dce3-9a5c-4adf-b552-1741361567d6},{bd574871-5df9-4b64-83d1-2af9c0c17f66},' +
    '{7da6a4a2-ae54-40e0-9910-ebd9ef3f017e},{31022a3d-6fee-4b36-843e-bbb4556ab35b}';
  
var
  GCommandsPage: TInputOptionWizardPage;
  GIsUpgrade: Boolean;
  GLastUserPageID: Integer;
  
// This function will remove one layer of quotes around the given string, if any.
function MyRemoveQuotes(const AString: string): string;
begin
  if (AString[1] = '"') and (AString[Length(AString)] = '"') then
    Result := RemoveQuotes(AString)
  else
    Result := AString;
end;
  
// This function will check if a specific switch is present on the command-line.
// Make sure switch is uppercase.
function SwitchPresent(const ASwitch: string): Boolean;
var
  I: Integer;
begin
  Result := False;
  for I := 1 to ParamCount do
  begin
    if Uppercase(ParamStr(I)) = ASwitch then
    begin
      Result := True;
      Break;
    end;
  end;
end;

// This function will return the initial choice to select in the
// Command choice page that is shown during first installs.
function InitialCommandsChoice: Integer;
var
  I: Integer;
  S, ChoiceStr: string;
begin
  Result := -1;
  for I := 1 to ParamCount do
  begin
    S := ParamStr(I);
    if Pos(CInitialCommandsChoiceSwitch, Uppercase(S)) = 1 then
    begin
      // This is the argument we're looking for; choice follows.
      ChoiceStr := MyRemoveQuotes(
        Copy(S, Length(CInitialCommandsChoiceSwitch) + 1, MaxInt));
      Result := StrToIntDef(ChoiceStr, -1);
      Break;
    end;
  end;
  
  // If the switch wasn't found or if it contained a value
  // outside range, use the default value.
  if (Result < CCommonCommandsChoice) or (Result > CAllCommandsChoice) then
    Result := CCommonCommandsChoice;
end;

// Called to know the install source when creating registry entries.
// It can be overridden on the command-line.
function InstallSourceValue(Params: string): string;
var
  I: Integer;
  S: string;
begin
  // Return default install source unless overridden on the command-line.
  Result := CDefaultInstallSource;
  for I := 1 to ParamCount do
  begin
    S := ParamStr(I);
    if Pos(CInstallSourceSwitch, Uppercase(S)) = 1 then
    begin
      // This is the argument we're looking for; install source follows.
      Result := MyRemoveQuotes(
        Copy(S, Length(CInstallSourceSwitch) + 1, MaxInt));
      Break;
    end;
  end;
end;

// Called to get the parameters to pass to regsvr32 to register/unregister
// the Path Copy Copy DLL. This varies depending on installation type.
function Regsvr32InstallParameters(Params: string): string;
var
  InstallParam: string;
begin
  // If this is a portable installation, pass "user" as a parameter to
  // DllInstall to have the DLL register its objects in per-user mode.
#ifdef PER_USER
  InstallParam := '/i:user';
#else
  InstallParam := '/i';
#endif

  Result := '/s /n ' + InstallParam;
  if Params <> '' then
    Result := Result + ' ' + Params;
end;

// Called to get the 'StatusRegisterFiles' setup message, to
// display it when we register our COM DLLs.
function GetStatusRegisterFiles(Params: string): string;
begin
  Result := SetupMessage(msgStatusRegisterFiles);
end;

// This procedure scans the command-line for values to write
// to the settings key in HKLM (or HKCU for portable installations).
procedure CreatePCCRegValues(const ARegKeyPath: string);
var
  I: Integer;
  S, UpperS, V, N: string;
  VI: Integer;
begin
  // Scan command-line arguments to find ours.
  for I := 1 to ParamCount do
  begin
    S := ParamStr(I);
    UpperS := Uppercase(S);
    if Pos(CDwordSwitch, UpperS) = 1 then
    begin
      // Value follows.
      S := MyRemoveQuotes(Copy(S, Length(CDwordSwitch) + 1, MaxInt));
        
      // String begins with the value name and a separator is used for the value.
      VI := StrToInt(Copy(S, Pos(CStringSeparator, S) + 1, MaxInt));
      S := Copy(S, 1, Pos(CStringSeparator, S) - 1);
      
      // Save in registry.
      RegWriteDWordValue(CSettingsRootKey, ARegKeyPath, S, VI);
    end
    else if Pos(CStringSwitch, UpperS) = 1 then
    begin
      // Value follows.
      S := MyRemoveQuotes(Copy(S, Length(CStringSwitch) + 1, MaxInt));
        
      // String begins with the value name and a separator is used for the value.
      V := Copy(S, Pos(CStringSeparator, S) + 1, MaxInt);
      S := Copy(S, 1, Pos(CStringSeparator, S) - 1);
        
       // Save in registry.
       RegWriteStringValue(CSettingsRootKey, ARegKeyPath, S, V);
    end
    else if Pos(CPipelinePluginSwitch, UpperS) = 1 then
    begin
      // Data follows.
      S := MyRemoveQuotes(Copy(S, Length(CPipelinePluginSwitch) + 1, MaxInt));
        
      // In order, the string contains: plugin ID, pipeline and description.
      V := Copy(S, Pos(CStringSeparator, S) + 1, MaxInt);
      N := Copy(V, Pos(CStringSeparator, V) + 1, MaxInt);
      V := Copy(V, 1, Pos(CStringSeparator, V) - 1);
      S := Copy(S, 1, Pos(CStringSeparator, S) - 1);
      
      // Create key for the plugin.
      RegWriteStringValue(CSettingsRootKey, ARegKeyPath +
        '\PipelinePlugins\' + S, '', V);
      RegWriteStringValue(CSettingsRootKey, ARegKeyPath +
        '\PipelinePlugins\' + S, 'Description', N);
    end
    else if Pos(CLegacyPipelinePluginsDisplayOrderSwitch, UpperS) = 1 then
    begin
      // Display order follows.
      S := MyRemoveQuotes(
        Copy(S, Length(CLegacyPipelinePluginsDisplayOrderSwitch) + 1, MaxInt));
        
      // Display order is a comma-separated list of plugin IDs. We don't
      // have to pre-process them, merely write them to the registry.
      // Note: this is a legacy setting, but it will be honored during revise
      // the first time new settings are created.
      RegWriteStringValue(CSettingsRootKey, ARegKeyPath + '\PipelinePlugins',
        'LegacyDisplayOrder', S);
    end;
  end;
end;

// This procedure uses the user choice of the "commands"
// page to pre-configure default settings in the registry.
procedure ConfigureDefaultSettings(const ARegKeyPath: string);
var
  DisplayOrder: string;
begin
  // Here we need to build the list of plugins to display in the submenu in
  // display order. If we do not set anything, all plugins will be used.
  if not GCommandsPage.Values[CAllCommandsChoice] then
  begin
    // Add all common commands.
    DisplayOrder := '{3a42b5c6-72d6-4a8a-ba44-014fa64aa11a},' + // Long Name
      '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +               // -
      '{331a3b60-af49-44f4-b30d-56adff6d25e8},' +               // Long Path
      '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +               // -
      '{349939d7-780f-43fd-a98e-83d9add44e22}';                 // Long Parent
    if GCommandsPage.Values[CNetworkCommandsChoice] then
    begin
      // Add network commands after the common ones.
      DisplayOrder := DisplayOrder + ',' +
        '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +             // -
        '{ea24bcc3-35f5-46b9-a2a5-a12a0aed2d28},' +             // Long UNC
        '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +             // -
        '{e2c942ac-917c-4aee-a867-8f6ab960ba76},' +             // Long Parent UNC
        '{afa4d1e1-ba73-4330-a6ab-e574ff39ecc3},' +             // -
        '{8f2adccc-9693-407d-9300-fccb9a12b982},' +             // Internet
        '{7da6a4a2-ae54-40e0-9910-ebd9ef3f017e}';               // Samba
    end;
    RegWriteStringValue(CSettingsRootKey, ARegKeyPath,
      'SubmenuDisplayOrder', DisplayOrder);
      
    // If we set the submenu display order, we also need to specify known
    // plugins. Otherwise, if pipeline plugins are added later through
    // command-line arguments, they won't be displayed by default.
    RegWriteStringValue(CSettingsRootKey, ARegKeyPath,
      'KnownPlugins', CKnownPlugins);
  end;
    
  // The setting "Drop redundant words" makes a lot of sense, but we can't
  // default it to true for everyone, since it might be confusing for existing
  // users. What we'll do is default it to true for initial installs, but
  // we won't touch the setting for upgrades.
  RegWriteDWordValue(CSettingsRootKey, ARegKeyPath, 'DropRedundantWords', 1);
end;

// Called when setup first initializes. Note that the
// wizard form does not exist at that point.
function InitializeSetup: Boolean;
begin
  // Check if this is an upgrade by checking the main DLL's AppID registry
  // entry. (We could also check the installer's AppID in the Uninstall key.)
#ifdef PER_USER
  GIsUpgrade := RegKeyExists(HKEY_CURRENT_USER,
    'Software\Classes\AppID\{44F7E5A2-1286-45F5-9A7A-A95A41B72918}');
#else
  GIsUpgrade := RegKeyExists(HKEY_CLASSES_ROOT,
    'AppID\{44F7E5A2-1286-45F5-9A7A-A95A41B72918}');
#endif
    
  // Return True so that setup can proceed.
  Result := True;
end;

// Called after the wizard form has been created.
// We can use this opportunity to add pages to it.
procedure InitializeWizard;
begin
  // Create a page to allow the user to select which commands
  // to include in the submenu by default.
  GCommandsPage := CreateInputOptionPage(wpSelectTasks,
    'Configuration', 'Pre-configure commands',
    'Path Copy Copy ships with multiple commands used to copy paths ' +
    '(long path, network path, etc.) Please choose which ones to include ' +
    'by default. This can be changed later via Settings.', True, False);
  GCommandsPage.Add('Commonly-used commands only');
  GCommandsPage.Add('Common commands and commands for network environments');
  GCommandsPage.Add('All commands, including power-user commands');
  GCommandsPage.SelectedValueIndex := InitialCommandsChoice;
  
  // Compute the ID of the last page the user is going to see before installation.
  // Since Ready page is disabled, it depends on what pages will be visible.
  if not GIsUpgrade then
    GLastUserPageID := GCommandsPage.ID
  else
    GLastUserPageID := wpLicense;
    
  // If this is an upgrade, the only page we'll show is the License page,
  // which means the CurPageChanged event will never be called to let us
  // change the next button caption. Change it here.
  if GIsUpgrade then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall);
end;

// Called for each wizard page. Returning True will skip that page.
function ShouldSkipPage(PageID: Integer): Boolean;
begin
  // Skip our "commands" page if this is an upgrade.
  // We do this to avoid overwriting existing settings and
  // since it's difficult to parse the "SubmenuDisplayOrder"
  // registry value, which can be modified by other means.
  if PageID = GCommandsPage.ID then
    Result := GIsUpgrade
  else
    Result := False;
end;

// Called for each wizard page when the page is shown. We use this opportunity
// to modify UI when appropriate.
procedure CurPageChanged(CurPageID: Integer);
begin
  // Since we're not showing the Ready page, we'll turn the Next button into
  // an Install button on the last user page. The code to do this has been
  // stolen from Inno Setup help - see DisableReadyPage.
  if CurPageID = GLastUserPageID then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall)
  else if CurPageID = wpFinished then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonFinish)
  else
    WizardForm.NextButton.Caption := SetupMessage(msgButtonNext);
end;

// Called during several installation steps. We'll use the
// opportunity to write admin registry keys at the end.
procedure CurStepChanged(CurStep: TSetupStep);
#ifndef PER_USER
var
  ResultCode: Integer;
#endif
begin
  if CurStep = ssPostInstall then
  begin
    // If this is not an upgrade, configure default settings.
    // Do this before setting registry values, this way the
    // command-line parameters will take precedence.
    if not GIsUpgrade then
    begin
      ConfigureDefaultSettings('Software\clechasseur\PathCopyCopy');
      if Is64BitInstallMode then
      begin
        // Also configure settings for the 32-bit version.
        ConfigureDefaultSettings('Software\Wow6432Node\clechasseur\PathCopyCopy');
      end;
    end;
    
    // Now process reg values on the command-line.  
    CreatePCCRegValues('Software\clechasseur\PathCopyCopy');
    if Is64BitInstallMode then
    begin
      // Also save the same values for the 32-bit version.
      CreatePCCRegValues('Software\Wow6432Node\clechasseur\PathCopyCopy');
    end;
    
#ifndef PER_USER
    // Revise global config. We perform this *after* processing command-line
    // arguments, so that old-style command-line values will get upgraded.
    // (We do this here and not in the [Run] section to be able to do it
    // after processing the command-line, since [Run] entries are processed
    // before this event is called.)
    Exec('rundll32.exe', ExpandConstant('"{app}\PCC32.dll",ApplyGlobalRevisions'),
      '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    if Is64BitInstallMode then
    begin
      Exec('rundll32.exe', ExpandConstant('"{app}\PCC64.dll",ApplyGlobalRevisions'),
        '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
#else
    // No need to perform revise in portable mode, it will be performed
    // the first time the PCC DLL is called.
#endif
  end;
end;
