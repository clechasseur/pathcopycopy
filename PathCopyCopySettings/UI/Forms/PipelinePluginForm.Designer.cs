namespace PathCopyCopy.Settings.UI.Forms
{
    partial class PipelinePluginForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PipelinePluginForm));
            this.NameLbl = new System.Windows.Forms.Label();
            this.NameTxt = new System.Windows.Forms.TextBox();
            this.MainTabControl = new System.Windows.Forms.TabControl();
            this.BasePluginPage = new System.Windows.Forms.TabPage();
            this.BasePluginLst = new System.Windows.Forms.ListBox();
            this.BaseCommandLbl2 = new System.Windows.Forms.Label();
            this.BaseCommandLbl = new System.Windows.Forms.Label();
            this.OptionsPage = new System.Windows.Forms.TabPage();
            this.OptionsGroupBox = new System.Windows.Forms.GroupBox();
            this.WithFilelistChk = new System.Windows.Forms.CheckBox();
            this.BrowserForExecutableBtn = new System.Windows.Forms.Button();
            this.ExecutableTxt = new System.Windows.Forms.TextBox();
            this.ExecutableLbl = new System.Windows.Forms.Label();
            this.LaunchExecutableChk = new System.Windows.Forms.CheckBox();
            this.CopyOnSameLineChk = new System.Windows.Forms.CheckBox();
            this.FindReplaceGroupBox = new System.Windows.Forms.GroupBox();
            this.UnexpandEnvStringsChk = new System.Windows.Forms.CheckBox();
            this.TestRegexBtn = new System.Windows.Forms.Button();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.UseRegexChk = new System.Windows.Forms.CheckBox();
            this.ReplaceTxt = new System.Windows.Forms.TextBox();
            this.ReplaceLbl = new System.Windows.Forms.Label();
            this.FindTxt = new System.Windows.Forms.TextBox();
            this.FindLbl = new System.Windows.Forms.Label();
            this.SlashesGroupBox = new System.Windows.Forms.GroupBox();
            this.NoSlashesChangeRadio = new System.Windows.Forms.RadioButton();
            this.BackToForwardSlashesRadio = new System.Windows.Forms.RadioButton();
            this.ForwardToBackslashesRadio = new System.Windows.Forms.RadioButton();
            this.DecorationsGroupBox = new System.Windows.Forms.GroupBox();
            this.RemoveExtChk = new System.Windows.Forms.CheckBox();
            this.OptionalQuotesChk = new System.Windows.Forms.CheckBox();
            this.EncodeURICharsChk = new System.Windows.Forms.CheckBox();
            this.EncodeURIWhitespaceChk = new System.Windows.Forms.CheckBox();
            this.EmailLinksChk = new System.Windows.Forms.CheckBox();
            this.QuotesChk = new System.Windows.Forms.CheckBox();
            this.OKBtn = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.ChooseExecutableOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.PipelinePluginToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SwitchBtn = new System.Windows.Forms.Button();
            this.PreviewCtrl = new PathCopyCopy.Settings.UI.UserControls.PluginPreviewUserControl();
            this.MainTabControl.SuspendLayout();
            this.BasePluginPage.SuspendLayout();
            this.OptionsPage.SuspendLayout();
            this.OptionsGroupBox.SuspendLayout();
            this.FindReplaceGroupBox.SuspendLayout();
            this.SlashesGroupBox.SuspendLayout();
            this.DecorationsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // NameLbl
            // 
            resources.ApplyResources(this.NameLbl, "NameLbl");
            this.NameLbl.Name = "NameLbl";
            this.PipelinePluginToolTip.SetToolTip(this.NameLbl, resources.GetString("NameLbl.ToolTip"));
            // 
            // NameTxt
            // 
            resources.ApplyResources(this.NameTxt, "NameTxt");
            this.NameTxt.Name = "NameTxt";
            this.PipelinePluginToolTip.SetToolTip(this.NameTxt, resources.GetString("NameTxt.ToolTip"));
            // 
            // MainTabControl
            // 
            resources.ApplyResources(this.MainTabControl, "MainTabControl");
            this.MainTabControl.Controls.Add(this.BasePluginPage);
            this.MainTabControl.Controls.Add(this.OptionsPage);
            this.MainTabControl.Name = "MainTabControl";
            this.MainTabControl.SelectedIndex = 0;
            this.PipelinePluginToolTip.SetToolTip(this.MainTabControl, resources.GetString("MainTabControl.ToolTip"));
            // 
            // BasePluginPage
            // 
            resources.ApplyResources(this.BasePluginPage, "BasePluginPage");
            this.BasePluginPage.Controls.Add(this.BasePluginLst);
            this.BasePluginPage.Controls.Add(this.BaseCommandLbl2);
            this.BasePluginPage.Controls.Add(this.BaseCommandLbl);
            this.BasePluginPage.Name = "BasePluginPage";
            this.PipelinePluginToolTip.SetToolTip(this.BasePluginPage, resources.GetString("BasePluginPage.ToolTip"));
            this.BasePluginPage.UseVisualStyleBackColor = true;
            // 
            // BasePluginLst
            // 
            resources.ApplyResources(this.BasePluginLst, "BasePluginLst");
            this.BasePluginLst.FormattingEnabled = true;
            this.BasePluginLst.Name = "BasePluginLst";
            this.PipelinePluginToolTip.SetToolTip(this.BasePluginLst, resources.GetString("BasePluginLst.ToolTip"));
            this.BasePluginLst.SelectedIndexChanged += new System.EventHandler(this.BasePluginLst_SelectedIndexChanged);
            // 
            // BaseCommandLbl2
            // 
            resources.ApplyResources(this.BaseCommandLbl2, "BaseCommandLbl2");
            this.BaseCommandLbl2.Name = "BaseCommandLbl2";
            this.PipelinePluginToolTip.SetToolTip(this.BaseCommandLbl2, resources.GetString("BaseCommandLbl2.ToolTip"));
            // 
            // BaseCommandLbl
            // 
            resources.ApplyResources(this.BaseCommandLbl, "BaseCommandLbl");
            this.BaseCommandLbl.Name = "BaseCommandLbl";
            this.PipelinePluginToolTip.SetToolTip(this.BaseCommandLbl, resources.GetString("BaseCommandLbl.ToolTip"));
            // 
            // OptionsPage
            // 
            resources.ApplyResources(this.OptionsPage, "OptionsPage");
            this.OptionsPage.Controls.Add(this.OptionsGroupBox);
            this.OptionsPage.Controls.Add(this.FindReplaceGroupBox);
            this.OptionsPage.Controls.Add(this.SlashesGroupBox);
            this.OptionsPage.Controls.Add(this.DecorationsGroupBox);
            this.OptionsPage.Name = "OptionsPage";
            this.PipelinePluginToolTip.SetToolTip(this.OptionsPage, resources.GetString("OptionsPage.ToolTip"));
            this.OptionsPage.UseVisualStyleBackColor = true;
            // 
            // OptionsGroupBox
            // 
            resources.ApplyResources(this.OptionsGroupBox, "OptionsGroupBox");
            this.OptionsGroupBox.Controls.Add(this.WithFilelistChk);
            this.OptionsGroupBox.Controls.Add(this.BrowserForExecutableBtn);
            this.OptionsGroupBox.Controls.Add(this.ExecutableTxt);
            this.OptionsGroupBox.Controls.Add(this.ExecutableLbl);
            this.OptionsGroupBox.Controls.Add(this.LaunchExecutableChk);
            this.OptionsGroupBox.Controls.Add(this.CopyOnSameLineChk);
            this.OptionsGroupBox.Name = "OptionsGroupBox";
            this.OptionsGroupBox.TabStop = false;
            this.PipelinePluginToolTip.SetToolTip(this.OptionsGroupBox, resources.GetString("OptionsGroupBox.ToolTip"));
            // 
            // WithFilelistChk
            // 
            resources.ApplyResources(this.WithFilelistChk, "WithFilelistChk");
            this.WithFilelistChk.Name = "WithFilelistChk";
            this.PipelinePluginToolTip.SetToolTip(this.WithFilelistChk, resources.GetString("WithFilelistChk.ToolTip"));
            this.WithFilelistChk.UseVisualStyleBackColor = true;
            // 
            // BrowserForExecutableBtn
            // 
            resources.ApplyResources(this.BrowserForExecutableBtn, "BrowserForExecutableBtn");
            this.BrowserForExecutableBtn.Name = "BrowserForExecutableBtn";
            this.PipelinePluginToolTip.SetToolTip(this.BrowserForExecutableBtn, resources.GetString("BrowserForExecutableBtn.ToolTip"));
            this.BrowserForExecutableBtn.UseVisualStyleBackColor = true;
            this.BrowserForExecutableBtn.Click += new System.EventHandler(this.BrowserForExecutableBtn_Click);
            // 
            // ExecutableTxt
            // 
            resources.ApplyResources(this.ExecutableTxt, "ExecutableTxt");
            this.ExecutableTxt.Name = "ExecutableTxt";
            this.PipelinePluginToolTip.SetToolTip(this.ExecutableTxt, resources.GetString("ExecutableTxt.ToolTip"));
            // 
            // ExecutableLbl
            // 
            resources.ApplyResources(this.ExecutableLbl, "ExecutableLbl");
            this.ExecutableLbl.Name = "ExecutableLbl";
            this.PipelinePluginToolTip.SetToolTip(this.ExecutableLbl, resources.GetString("ExecutableLbl.ToolTip"));
            // 
            // LaunchExecutableChk
            // 
            resources.ApplyResources(this.LaunchExecutableChk, "LaunchExecutableChk");
            this.LaunchExecutableChk.Name = "LaunchExecutableChk";
            this.PipelinePluginToolTip.SetToolTip(this.LaunchExecutableChk, resources.GetString("LaunchExecutableChk.ToolTip"));
            this.LaunchExecutableChk.UseVisualStyleBackColor = true;
            this.LaunchExecutableChk.CheckedChanged += new System.EventHandler(this.LaunchExecutableChk_CheckedChanged);
            // 
            // CopyOnSameLineChk
            // 
            resources.ApplyResources(this.CopyOnSameLineChk, "CopyOnSameLineChk");
            this.CopyOnSameLineChk.Name = "CopyOnSameLineChk";
            this.PipelinePluginToolTip.SetToolTip(this.CopyOnSameLineChk, resources.GetString("CopyOnSameLineChk.ToolTip"));
            this.CopyOnSameLineChk.UseVisualStyleBackColor = true;
            // 
            // FindReplaceGroupBox
            // 
            resources.ApplyResources(this.FindReplaceGroupBox, "FindReplaceGroupBox");
            this.FindReplaceGroupBox.Controls.Add(this.UnexpandEnvStringsChk);
            this.FindReplaceGroupBox.Controls.Add(this.TestRegexBtn);
            this.FindReplaceGroupBox.Controls.Add(this.IgnoreCaseChk);
            this.FindReplaceGroupBox.Controls.Add(this.UseRegexChk);
            this.FindReplaceGroupBox.Controls.Add(this.ReplaceTxt);
            this.FindReplaceGroupBox.Controls.Add(this.ReplaceLbl);
            this.FindReplaceGroupBox.Controls.Add(this.FindTxt);
            this.FindReplaceGroupBox.Controls.Add(this.FindLbl);
            this.FindReplaceGroupBox.Name = "FindReplaceGroupBox";
            this.FindReplaceGroupBox.TabStop = false;
            this.PipelinePluginToolTip.SetToolTip(this.FindReplaceGroupBox, resources.GetString("FindReplaceGroupBox.ToolTip"));
            // 
            // UnexpandEnvStringsChk
            // 
            resources.ApplyResources(this.UnexpandEnvStringsChk, "UnexpandEnvStringsChk");
            this.UnexpandEnvStringsChk.Name = "UnexpandEnvStringsChk";
            this.PipelinePluginToolTip.SetToolTip(this.UnexpandEnvStringsChk, resources.GetString("UnexpandEnvStringsChk.ToolTip"));
            this.UnexpandEnvStringsChk.UseVisualStyleBackColor = true;
            this.UnexpandEnvStringsChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // TestRegexBtn
            // 
            resources.ApplyResources(this.TestRegexBtn, "TestRegexBtn");
            this.TestRegexBtn.Name = "TestRegexBtn";
            this.PipelinePluginToolTip.SetToolTip(this.TestRegexBtn, resources.GetString("TestRegexBtn.ToolTip"));
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            this.TestRegexBtn.Click += new System.EventHandler(this.TestRegexBtn_Click);
            // 
            // IgnoreCaseChk
            // 
            resources.ApplyResources(this.IgnoreCaseChk, "IgnoreCaseChk");
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.PipelinePluginToolTip.SetToolTip(this.IgnoreCaseChk, resources.GetString("IgnoreCaseChk.ToolTip"));
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            this.IgnoreCaseChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // UseRegexChk
            // 
            resources.ApplyResources(this.UseRegexChk, "UseRegexChk");
            this.UseRegexChk.Name = "UseRegexChk";
            this.PipelinePluginToolTip.SetToolTip(this.UseRegexChk, resources.GetString("UseRegexChk.ToolTip"));
            this.UseRegexChk.UseVisualStyleBackColor = true;
            this.UseRegexChk.CheckedChanged += new System.EventHandler(this.UseRegexChk_CheckedChanged);
            // 
            // ReplaceTxt
            // 
            resources.ApplyResources(this.ReplaceTxt, "ReplaceTxt");
            this.ReplaceTxt.Name = "ReplaceTxt";
            this.PipelinePluginToolTip.SetToolTip(this.ReplaceTxt, resources.GetString("ReplaceTxt.ToolTip"));
            this.ReplaceTxt.TextChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // ReplaceLbl
            // 
            resources.ApplyResources(this.ReplaceLbl, "ReplaceLbl");
            this.ReplaceLbl.Name = "ReplaceLbl";
            this.PipelinePluginToolTip.SetToolTip(this.ReplaceLbl, resources.GetString("ReplaceLbl.ToolTip"));
            // 
            // FindTxt
            // 
            resources.ApplyResources(this.FindTxt, "FindTxt");
            this.FindTxt.Name = "FindTxt";
            this.PipelinePluginToolTip.SetToolTip(this.FindTxt, resources.GetString("FindTxt.ToolTip"));
            this.FindTxt.TextChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // FindLbl
            // 
            resources.ApplyResources(this.FindLbl, "FindLbl");
            this.FindLbl.Name = "FindLbl";
            this.PipelinePluginToolTip.SetToolTip(this.FindLbl, resources.GetString("FindLbl.ToolTip"));
            // 
            // SlashesGroupBox
            // 
            resources.ApplyResources(this.SlashesGroupBox, "SlashesGroupBox");
            this.SlashesGroupBox.Controls.Add(this.NoSlashesChangeRadio);
            this.SlashesGroupBox.Controls.Add(this.BackToForwardSlashesRadio);
            this.SlashesGroupBox.Controls.Add(this.ForwardToBackslashesRadio);
            this.SlashesGroupBox.Name = "SlashesGroupBox";
            this.SlashesGroupBox.TabStop = false;
            this.PipelinePluginToolTip.SetToolTip(this.SlashesGroupBox, resources.GetString("SlashesGroupBox.ToolTip"));
            // 
            // NoSlashesChangeRadio
            // 
            resources.ApplyResources(this.NoSlashesChangeRadio, "NoSlashesChangeRadio");
            this.NoSlashesChangeRadio.Checked = true;
            this.NoSlashesChangeRadio.Name = "NoSlashesChangeRadio";
            this.NoSlashesChangeRadio.TabStop = true;
            this.PipelinePluginToolTip.SetToolTip(this.NoSlashesChangeRadio, resources.GetString("NoSlashesChangeRadio.ToolTip"));
            this.NoSlashesChangeRadio.UseVisualStyleBackColor = true;
            this.NoSlashesChangeRadio.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // BackToForwardSlashesRadio
            // 
            resources.ApplyResources(this.BackToForwardSlashesRadio, "BackToForwardSlashesRadio");
            this.BackToForwardSlashesRadio.Name = "BackToForwardSlashesRadio";
            this.PipelinePluginToolTip.SetToolTip(this.BackToForwardSlashesRadio, resources.GetString("BackToForwardSlashesRadio.ToolTip"));
            this.BackToForwardSlashesRadio.UseVisualStyleBackColor = true;
            this.BackToForwardSlashesRadio.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // ForwardToBackslashesRadio
            // 
            resources.ApplyResources(this.ForwardToBackslashesRadio, "ForwardToBackslashesRadio");
            this.ForwardToBackslashesRadio.Name = "ForwardToBackslashesRadio";
            this.PipelinePluginToolTip.SetToolTip(this.ForwardToBackslashesRadio, resources.GetString("ForwardToBackslashesRadio.ToolTip"));
            this.ForwardToBackslashesRadio.UseVisualStyleBackColor = true;
            this.ForwardToBackslashesRadio.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // DecorationsGroupBox
            // 
            resources.ApplyResources(this.DecorationsGroupBox, "DecorationsGroupBox");
            this.DecorationsGroupBox.Controls.Add(this.RemoveExtChk);
            this.DecorationsGroupBox.Controls.Add(this.OptionalQuotesChk);
            this.DecorationsGroupBox.Controls.Add(this.EncodeURICharsChk);
            this.DecorationsGroupBox.Controls.Add(this.EncodeURIWhitespaceChk);
            this.DecorationsGroupBox.Controls.Add(this.EmailLinksChk);
            this.DecorationsGroupBox.Controls.Add(this.QuotesChk);
            this.DecorationsGroupBox.Name = "DecorationsGroupBox";
            this.DecorationsGroupBox.TabStop = false;
            this.PipelinePluginToolTip.SetToolTip(this.DecorationsGroupBox, resources.GetString("DecorationsGroupBox.ToolTip"));
            // 
            // RemoveExtChk
            // 
            resources.ApplyResources(this.RemoveExtChk, "RemoveExtChk");
            this.RemoveExtChk.Name = "RemoveExtChk";
            this.PipelinePluginToolTip.SetToolTip(this.RemoveExtChk, resources.GetString("RemoveExtChk.ToolTip"));
            this.RemoveExtChk.UseVisualStyleBackColor = true;
            this.RemoveExtChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // OptionalQuotesChk
            // 
            resources.ApplyResources(this.OptionalQuotesChk, "OptionalQuotesChk");
            this.OptionalQuotesChk.Name = "OptionalQuotesChk";
            this.PipelinePluginToolTip.SetToolTip(this.OptionalQuotesChk, resources.GetString("OptionalQuotesChk.ToolTip"));
            this.OptionalQuotesChk.UseVisualStyleBackColor = true;
            this.OptionalQuotesChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // EncodeURICharsChk
            // 
            resources.ApplyResources(this.EncodeURICharsChk, "EncodeURICharsChk");
            this.EncodeURICharsChk.Name = "EncodeURICharsChk";
            this.PipelinePluginToolTip.SetToolTip(this.EncodeURICharsChk, resources.GetString("EncodeURICharsChk.ToolTip"));
            this.EncodeURICharsChk.UseVisualStyleBackColor = true;
            this.EncodeURICharsChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // EncodeURIWhitespaceChk
            // 
            resources.ApplyResources(this.EncodeURIWhitespaceChk, "EncodeURIWhitespaceChk");
            this.EncodeURIWhitespaceChk.Name = "EncodeURIWhitespaceChk";
            this.PipelinePluginToolTip.SetToolTip(this.EncodeURIWhitespaceChk, resources.GetString("EncodeURIWhitespaceChk.ToolTip"));
            this.EncodeURIWhitespaceChk.UseVisualStyleBackColor = true;
            this.EncodeURIWhitespaceChk.CheckedChanged += new System.EventHandler(this.EncodeURIWhitespaceChk_CheckedChanged);
            // 
            // EmailLinksChk
            // 
            resources.ApplyResources(this.EmailLinksChk, "EmailLinksChk");
            this.EmailLinksChk.Name = "EmailLinksChk";
            this.PipelinePluginToolTip.SetToolTip(this.EmailLinksChk, resources.GetString("EmailLinksChk.ToolTip"));
            this.EmailLinksChk.UseVisualStyleBackColor = true;
            this.EmailLinksChk.CheckedChanged += new System.EventHandler(this.PipelinePluginForm_UpdatePreview);
            // 
            // QuotesChk
            // 
            resources.ApplyResources(this.QuotesChk, "QuotesChk");
            this.QuotesChk.Name = "QuotesChk";
            this.PipelinePluginToolTip.SetToolTip(this.QuotesChk, resources.GetString("QuotesChk.ToolTip"));
            this.QuotesChk.UseVisualStyleBackColor = true;
            this.QuotesChk.CheckedChanged += new System.EventHandler(this.QuotesChk_CheckedChanged);
            // 
            // OKBtn
            // 
            resources.ApplyResources(this.OKBtn, "OKBtn");
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Name = "OKBtn";
            this.PipelinePluginToolTip.SetToolTip(this.OKBtn, resources.GetString("OKBtn.ToolTip"));
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // CancelBtn
            // 
            resources.ApplyResources(this.CancelBtn, "CancelBtn");
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Name = "CancelBtn";
            this.PipelinePluginToolTip.SetToolTip(this.CancelBtn, resources.GetString("CancelBtn.ToolTip"));
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // ChooseExecutableOpenDlg
            // 
            resources.ApplyResources(this.ChooseExecutableOpenDlg, "ChooseExecutableOpenDlg");
            // 
            // SwitchBtn
            // 
            resources.ApplyResources(this.SwitchBtn, "SwitchBtn");
            this.SwitchBtn.DialogResult = System.Windows.Forms.DialogResult.Retry;
            this.SwitchBtn.Name = "SwitchBtn";
            this.PipelinePluginToolTip.SetToolTip(this.SwitchBtn, resources.GetString("SwitchBtn.ToolTip"));
            this.SwitchBtn.UseVisualStyleBackColor = true;
            // 
            // PreviewCtrl
            // 
            resources.ApplyResources(this.PreviewCtrl, "PreviewCtrl");
            this.PreviewCtrl.Name = "PreviewCtrl";
            this.PreviewCtrl.Plugin = null;
            this.PipelinePluginToolTip.SetToolTip(this.PreviewCtrl, resources.GetString("PreviewCtrl.ToolTip"));
            // 
            // PipelinePluginForm
            // 
            this.AcceptButton = this.OKBtn;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.Controls.Add(this.PreviewCtrl);
            this.Controls.Add(this.SwitchBtn);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.MainTabControl);
            this.Controls.Add(this.NameTxt);
            this.Controls.Add(this.NameLbl);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "PipelinePluginForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.PipelinePluginToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.PipelinePluginForm_HelpButtonClicked);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PipelinePluginForm_FormClosing);
            this.Load += new System.EventHandler(this.PipelinePluginForm_Load);
            this.MainTabControl.ResumeLayout(false);
            this.BasePluginPage.ResumeLayout(false);
            this.BasePluginPage.PerformLayout();
            this.OptionsPage.ResumeLayout(false);
            this.OptionsGroupBox.ResumeLayout(false);
            this.OptionsGroupBox.PerformLayout();
            this.FindReplaceGroupBox.ResumeLayout(false);
            this.FindReplaceGroupBox.PerformLayout();
            this.SlashesGroupBox.ResumeLayout(false);
            this.SlashesGroupBox.PerformLayout();
            this.DecorationsGroupBox.ResumeLayout(false);
            this.DecorationsGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label NameLbl;
        private System.Windows.Forms.TextBox NameTxt;
        private System.Windows.Forms.TabControl MainTabControl;
        private System.Windows.Forms.TabPage BasePluginPage;
        private System.Windows.Forms.TabPage OptionsPage;
        private System.Windows.Forms.Label BaseCommandLbl2;
        private System.Windows.Forms.Label BaseCommandLbl;
        private System.Windows.Forms.ListBox BasePluginLst;
        private System.Windows.Forms.GroupBox DecorationsGroupBox;
        private System.Windows.Forms.CheckBox QuotesChk;
        private System.Windows.Forms.GroupBox SlashesGroupBox;
        private System.Windows.Forms.RadioButton NoSlashesChangeRadio;
        private System.Windows.Forms.RadioButton BackToForwardSlashesRadio;
        private System.Windows.Forms.RadioButton ForwardToBackslashesRadio;
        private System.Windows.Forms.GroupBox FindReplaceGroupBox;
        private System.Windows.Forms.TextBox ReplaceTxt;
        private System.Windows.Forms.Label ReplaceLbl;
        private System.Windows.Forms.TextBox FindTxt;
        private System.Windows.Forms.Label FindLbl;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.CheckBox UseRegexChk;
        private System.Windows.Forms.Button TestRegexBtn;
        private System.Windows.Forms.CheckBox EmailLinksChk;
        private System.Windows.Forms.CheckBox EncodeURIWhitespaceChk;
        private System.Windows.Forms.GroupBox OptionsGroupBox;
        private System.Windows.Forms.CheckBox CopyOnSameLineChk;
        private System.Windows.Forms.CheckBox EncodeURICharsChk;
        private System.Windows.Forms.Button BrowserForExecutableBtn;
        private System.Windows.Forms.TextBox ExecutableTxt;
        private System.Windows.Forms.Label ExecutableLbl;
        private System.Windows.Forms.CheckBox LaunchExecutableChk;
        private System.Windows.Forms.OpenFileDialog ChooseExecutableOpenDlg;
        private System.Windows.Forms.CheckBox OptionalQuotesChk;
        private System.Windows.Forms.CheckBox WithFilelistChk;
        private System.Windows.Forms.CheckBox RemoveExtChk;
        private System.Windows.Forms.ToolTip PipelinePluginToolTip;
        private System.Windows.Forms.Button SwitchBtn;
        private UserControls.PluginPreviewUserControl PreviewCtrl;
        private System.Windows.Forms.CheckBox UnexpandEnvStringsChk;
    }
}