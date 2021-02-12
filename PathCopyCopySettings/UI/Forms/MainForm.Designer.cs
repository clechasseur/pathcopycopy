namespace PathCopyCopy.Settings.UI.Forms
{
    partial class MainForm
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
            if (disposing) {
                components?.Dispose();

                // Dispose of temp pipeline plugins helper, which will
                // remove all tracked plugins from registry.
                tempPipelinePluginsHelper?.Dispose();
                tempPipelinePluginsHelper = null;
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.ApplyBtn = new System.Windows.Forms.Button();
            this.OKBtn = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.MainTabCtrl = new System.Windows.Forms.TabControl();
            this.PluginsPage = new System.Windows.Forms.TabPage();
            this.DuplicatePipelinePluginBtn = new System.Windows.Forms.Button();
            this.PreviewCtrl = new PathCopyCopy.Settings.UI.UserControls.PluginPreviewUserControl();
            this.AddSeparatorBtn = new System.Windows.Forms.Button();
            this.ImportPipelinePluginsBtn = new System.Windows.Forms.Button();
            this.ExportPipelinePluginsBtn = new System.Windows.Forms.Button();
            this.BevelLineLbl2 = new System.Windows.Forms.Label();
            this.RemovePluginBtn = new System.Windows.Forms.Button();
            this.EditPipelinePluginBtn = new System.Windows.Forms.Button();
            this.AddPipelinePluginBtn = new System.Windows.Forms.Button();
            this.BevelLineLbl = new System.Windows.Forms.Label();
            this.MovePluginDownBtn = new System.Windows.Forms.Button();
            this.MovePluginUpBtn = new System.Windows.Forms.Button();
            this.PluginsExplanationLbl2 = new System.Windows.Forms.Label();
            this.PluginsDataGrid = new System.Windows.Forms.DataGridView();
            this.IconCol = new System.Windows.Forms.DataGridViewImageColumn();
            this.PluginCol = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.InMainMenuCol = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.InSubmenuCol = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.PluginsDataGridBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.PluginsExplanationLbl = new System.Windows.Forms.Label();
            this.MiscOptionsPage = new System.Windows.Forms.TabPage();
            this.RecursiveCopyChk = new System.Windows.Forms.CheckBox();
            this.LanguageCombo = new System.Windows.Forms.ComboBox();
            this.LanguageComboBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.LanguageLbl = new System.Windows.Forms.Label();
            this.TrueLnkPathsChk = new System.Windows.Forms.CheckBox();
            this.UsePreviewModeInMainMenuChk = new System.Windows.Forms.CheckBox();
            this.AppendSepForDirChk = new System.Windows.Forms.CheckBox();
            this.UseFQDNChk = new System.Windows.Forms.CheckBox();
            this.AreQuotesOptionalChk = new System.Windows.Forms.CheckBox();
            this.CtrlKeyPluginCombo = new System.Windows.Forms.ComboBox();
            this.CtrlKeyPluginComboBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.CtrlKeyPluginChk = new System.Windows.Forms.CheckBox();
            this.EncodeURICharsChk = new System.Windows.Forms.CheckBox();
            this.EncodeURIWhitespaceChk = new System.Windows.Forms.CheckBox();
            this.CopyOnSameLineChk = new System.Windows.Forms.CheckBox();
            this.DropRedundantWordsChk = new System.Windows.Forms.CheckBox();
            this.UsePreviewModeChk = new System.Windows.Forms.CheckBox();
            this.UseIconForSubmenuChk = new System.Windows.Forms.CheckBox();
            this.EmailLinksChk = new System.Windows.Forms.CheckBox();
            this.EnableSoftwareUpdateChk = new System.Windows.Forms.CheckBox();
            this.AlwaysShowSubmenuChk = new System.Windows.Forms.CheckBox();
            this.HiddenSharesChk = new System.Windows.Forms.CheckBox();
            this.AddQuotesChk = new System.Windows.Forms.CheckBox();
            this.AboutPage = new System.Windows.Forms.TabPage();
            this.AboutTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.ProductAndVersionLbl = new System.Windows.Forms.Label();
            this.CopyrightLbl = new System.Windows.Forms.Label();
            this.VisitWebsiteLbl = new System.Windows.Forms.Label();
            this.SiteLinkLbl = new System.Windows.Forms.LinkLabel();
            this.LicenseExplanationLbl = new System.Windows.Forms.Label();
            this.LicenseTxtLinkLbl = new System.Windows.Forms.LinkLabel();
            this.DonationLinkLbl = new System.Windows.Forms.LinkLabel();
            this.ExportPipelinePluginsSaveDlg = new System.Windows.Forms.SaveFileDialog();
            this.ImportPipelinePluginsOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.ChoosePluginIconOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.ExportUserSettingsSaveDlg = new System.Windows.Forms.SaveFileDialog();
            this.ExportUserSettingsBtn = new System.Windows.Forms.Button();
            this.MainToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.MainTabCtrl.SuspendLayout();
            this.PluginsPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGrid)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGridBindingSource)).BeginInit();
            this.MiscOptionsPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.LanguageComboBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CtrlKeyPluginComboBindingSource)).BeginInit();
            this.AboutPage.SuspendLayout();
            this.AboutTableLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // ApplyBtn
            // 
            resources.ApplyResources(this.ApplyBtn, "ApplyBtn");
            this.ApplyBtn.Name = "ApplyBtn";
            this.MainToolTip.SetToolTip(this.ApplyBtn, resources.GetString("ApplyBtn.ToolTip"));
            this.ApplyBtn.UseVisualStyleBackColor = true;
            this.ApplyBtn.Click += new System.EventHandler(this.ApplyBtn_Click);
            // 
            // OKBtn
            // 
            resources.ApplyResources(this.OKBtn, "OKBtn");
            this.OKBtn.Name = "OKBtn";
            this.MainToolTip.SetToolTip(this.OKBtn, resources.GetString("OKBtn.ToolTip"));
            this.OKBtn.UseVisualStyleBackColor = true;
            this.OKBtn.Click += new System.EventHandler(this.OKBtn_Click);
            // 
            // CancelBtn
            // 
            resources.ApplyResources(this.CancelBtn, "CancelBtn");
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Name = "CancelBtn";
            this.MainToolTip.SetToolTip(this.CancelBtn, resources.GetString("CancelBtn.ToolTip"));
            this.CancelBtn.UseVisualStyleBackColor = true;
            this.CancelBtn.Click += new System.EventHandler(this.CancelBtn_Click);
            // 
            // MainTabCtrl
            // 
            resources.ApplyResources(this.MainTabCtrl, "MainTabCtrl");
            this.MainTabCtrl.Controls.Add(this.PluginsPage);
            this.MainTabCtrl.Controls.Add(this.MiscOptionsPage);
            this.MainTabCtrl.Controls.Add(this.AboutPage);
            this.MainTabCtrl.Name = "MainTabCtrl";
            this.MainTabCtrl.SelectedIndex = 0;
            // 
            // PluginsPage
            // 
            this.PluginsPage.Controls.Add(this.DuplicatePipelinePluginBtn);
            this.PluginsPage.Controls.Add(this.PreviewCtrl);
            this.PluginsPage.Controls.Add(this.AddSeparatorBtn);
            this.PluginsPage.Controls.Add(this.ImportPipelinePluginsBtn);
            this.PluginsPage.Controls.Add(this.ExportPipelinePluginsBtn);
            this.PluginsPage.Controls.Add(this.BevelLineLbl2);
            this.PluginsPage.Controls.Add(this.RemovePluginBtn);
            this.PluginsPage.Controls.Add(this.EditPipelinePluginBtn);
            this.PluginsPage.Controls.Add(this.AddPipelinePluginBtn);
            this.PluginsPage.Controls.Add(this.BevelLineLbl);
            this.PluginsPage.Controls.Add(this.MovePluginDownBtn);
            this.PluginsPage.Controls.Add(this.MovePluginUpBtn);
            this.PluginsPage.Controls.Add(this.PluginsExplanationLbl2);
            this.PluginsPage.Controls.Add(this.PluginsDataGrid);
            this.PluginsPage.Controls.Add(this.PluginsExplanationLbl);
            resources.ApplyResources(this.PluginsPage, "PluginsPage");
            this.PluginsPage.Name = "PluginsPage";
            this.PluginsPage.UseVisualStyleBackColor = true;
            // 
            // DuplicatePipelinePluginBtn
            // 
            resources.ApplyResources(this.DuplicatePipelinePluginBtn, "DuplicatePipelinePluginBtn");
            this.DuplicatePipelinePluginBtn.Name = "DuplicatePipelinePluginBtn";
            this.MainToolTip.SetToolTip(this.DuplicatePipelinePluginBtn, resources.GetString("DuplicatePipelinePluginBtn.ToolTip"));
            this.DuplicatePipelinePluginBtn.UseVisualStyleBackColor = true;
            this.DuplicatePipelinePluginBtn.Click += new System.EventHandler(this.DuplicatePipelinePluginBtn_Click);
            // 
            // PreviewCtrl
            // 
            resources.ApplyResources(this.PreviewCtrl, "PreviewCtrl");
            this.PreviewCtrl.Name = "PreviewCtrl";
            this.PreviewCtrl.Plugin = null;
            // 
            // AddSeparatorBtn
            // 
            resources.ApplyResources(this.AddSeparatorBtn, "AddSeparatorBtn");
            this.AddSeparatorBtn.Name = "AddSeparatorBtn";
            this.MainToolTip.SetToolTip(this.AddSeparatorBtn, resources.GetString("AddSeparatorBtn.ToolTip"));
            this.AddSeparatorBtn.UseVisualStyleBackColor = true;
            this.AddSeparatorBtn.Click += new System.EventHandler(this.AddSeparatorBtn_Click);
            // 
            // ImportPipelinePluginsBtn
            // 
            resources.ApplyResources(this.ImportPipelinePluginsBtn, "ImportPipelinePluginsBtn");
            this.ImportPipelinePluginsBtn.Name = "ImportPipelinePluginsBtn";
            this.MainToolTip.SetToolTip(this.ImportPipelinePluginsBtn, resources.GetString("ImportPipelinePluginsBtn.ToolTip"));
            this.ImportPipelinePluginsBtn.UseVisualStyleBackColor = true;
            this.ImportPipelinePluginsBtn.Click += new System.EventHandler(this.ImportPipelinePluginsBtn_Click);
            // 
            // ExportPipelinePluginsBtn
            // 
            resources.ApplyResources(this.ExportPipelinePluginsBtn, "ExportPipelinePluginsBtn");
            this.ExportPipelinePluginsBtn.Name = "ExportPipelinePluginsBtn";
            this.MainToolTip.SetToolTip(this.ExportPipelinePluginsBtn, resources.GetString("ExportPipelinePluginsBtn.ToolTip"));
            this.ExportPipelinePluginsBtn.UseVisualStyleBackColor = true;
            this.ExportPipelinePluginsBtn.Click += new System.EventHandler(this.ExportPipelinePluginsBtn_Click);
            // 
            // BevelLineLbl2
            // 
            resources.ApplyResources(this.BevelLineLbl2, "BevelLineLbl2");
            this.BevelLineLbl2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.BevelLineLbl2.Name = "BevelLineLbl2";
            // 
            // RemovePluginBtn
            // 
            resources.ApplyResources(this.RemovePluginBtn, "RemovePluginBtn");
            this.RemovePluginBtn.Name = "RemovePluginBtn";
            this.MainToolTip.SetToolTip(this.RemovePluginBtn, resources.GetString("RemovePluginBtn.ToolTip"));
            this.RemovePluginBtn.UseVisualStyleBackColor = true;
            this.RemovePluginBtn.Click += new System.EventHandler(this.RemovePluginBtn_Click);
            // 
            // EditPipelinePluginBtn
            // 
            resources.ApplyResources(this.EditPipelinePluginBtn, "EditPipelinePluginBtn");
            this.EditPipelinePluginBtn.Name = "EditPipelinePluginBtn";
            this.MainToolTip.SetToolTip(this.EditPipelinePluginBtn, resources.GetString("EditPipelinePluginBtn.ToolTip"));
            this.EditPipelinePluginBtn.UseVisualStyleBackColor = true;
            this.EditPipelinePluginBtn.Click += new System.EventHandler(this.EditPipelinePluginBtn_Click);
            // 
            // AddPipelinePluginBtn
            // 
            resources.ApplyResources(this.AddPipelinePluginBtn, "AddPipelinePluginBtn");
            this.AddPipelinePluginBtn.Name = "AddPipelinePluginBtn";
            this.MainToolTip.SetToolTip(this.AddPipelinePluginBtn, resources.GetString("AddPipelinePluginBtn.ToolTip"));
            this.AddPipelinePluginBtn.UseVisualStyleBackColor = true;
            this.AddPipelinePluginBtn.Click += new System.EventHandler(this.AddPipelinePluginBtn_Click);
            // 
            // BevelLineLbl
            // 
            resources.ApplyResources(this.BevelLineLbl, "BevelLineLbl");
            this.BevelLineLbl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.BevelLineLbl.Name = "BevelLineLbl";
            // 
            // MovePluginDownBtn
            // 
            resources.ApplyResources(this.MovePluginDownBtn, "MovePluginDownBtn");
            this.MovePluginDownBtn.Name = "MovePluginDownBtn";
            this.MainToolTip.SetToolTip(this.MovePluginDownBtn, resources.GetString("MovePluginDownBtn.ToolTip"));
            this.MovePluginDownBtn.UseVisualStyleBackColor = true;
            this.MovePluginDownBtn.Click += new System.EventHandler(this.MovePluginDownBtn_Click);
            // 
            // MovePluginUpBtn
            // 
            resources.ApplyResources(this.MovePluginUpBtn, "MovePluginUpBtn");
            this.MovePluginUpBtn.Name = "MovePluginUpBtn";
            this.MainToolTip.SetToolTip(this.MovePluginUpBtn, resources.GetString("MovePluginUpBtn.ToolTip"));
            this.MovePluginUpBtn.UseVisualStyleBackColor = true;
            this.MovePluginUpBtn.Click += new System.EventHandler(this.MovePluginUpBtn_Click);
            // 
            // PluginsExplanationLbl2
            // 
            resources.ApplyResources(this.PluginsExplanationLbl2, "PluginsExplanationLbl2");
            this.PluginsExplanationLbl2.Name = "PluginsExplanationLbl2";
            // 
            // PluginsDataGrid
            // 
            this.PluginsDataGrid.AllowUserToAddRows = false;
            this.PluginsDataGrid.AllowUserToDeleteRows = false;
            this.PluginsDataGrid.AllowUserToResizeColumns = false;
            this.PluginsDataGrid.AllowUserToResizeRows = false;
            resources.ApplyResources(this.PluginsDataGrid, "PluginsDataGrid");
            this.PluginsDataGrid.AutoGenerateColumns = false;
            this.PluginsDataGrid.BackgroundColor = System.Drawing.SystemColors.Window;
            this.PluginsDataGrid.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
            this.PluginsDataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.PluginsDataGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IconCol,
            this.PluginCol,
            this.InMainMenuCol,
            this.InSubmenuCol});
            this.PluginsDataGrid.DataSource = this.PluginsDataGridBindingSource;
            this.PluginsDataGrid.Name = "PluginsDataGrid";
            this.PluginsDataGrid.RowHeadersVisible = false;
            this.PluginsDataGrid.RowTemplate.Height = 18;
            this.PluginsDataGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.PluginsDataGrid.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.PluginsDataGrid_CellClick);
            this.PluginsDataGrid.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.PluginsDataGrid_CellDoubleClick);
            this.PluginsDataGrid.CellMouseEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.PluginsDataGrid_CellMouseEnter);
            this.PluginsDataGrid.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.PluginsDataGrid_CellValueChanged);
            this.PluginsDataGrid.CurrentCellDirtyStateChanged += new System.EventHandler(this.PluginsDataGrid_CurrentCellDirtyStateChanged);
            this.PluginsDataGrid.RowsAdded += new System.Windows.Forms.DataGridViewRowsAddedEventHandler(this.PluginsDataGrid_RowsAdded);
            this.PluginsDataGrid.SelectionChanged += new System.EventHandler(this.PluginsDataGrid_SelectionChanged);
            // 
            // IconCol
            // 
            this.IconCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            resources.ApplyResources(this.IconCol, "IconCol");
            this.IconCol.Image = global::PathCopyCopy.Settings.Properties.Resources.BlankIcon;
            this.IconCol.Name = "IconCol";
            this.IconCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // PluginCol
            // 
            this.PluginCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.PluginCol.DataPropertyName = "Name";
            resources.ApplyResources(this.PluginCol, "PluginCol");
            this.PluginCol.Name = "PluginCol";
            this.PluginCol.ReadOnly = true;
            this.PluginCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.PluginCol.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // InMainMenuCol
            // 
            this.InMainMenuCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.InMainMenuCol.DataPropertyName = "ShowInMainMenuStr";
            resources.ApplyResources(this.InMainMenuCol, "InMainMenuCol");
            this.InMainMenuCol.Name = "InMainMenuCol";
            this.InMainMenuCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // InSubmenuCol
            // 
            this.InSubmenuCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.InSubmenuCol.DataPropertyName = "ShowInSubmenuStr";
            resources.ApplyResources(this.InSubmenuCol, "InSubmenuCol");
            this.InSubmenuCol.Name = "InSubmenuCol";
            this.InSubmenuCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            // 
            // PluginsExplanationLbl
            // 
            resources.ApplyResources(this.PluginsExplanationLbl, "PluginsExplanationLbl");
            this.PluginsExplanationLbl.Name = "PluginsExplanationLbl";
            // 
            // MiscOptionsPage
            // 
            this.MiscOptionsPage.Controls.Add(this.RecursiveCopyChk);
            this.MiscOptionsPage.Controls.Add(this.LanguageCombo);
            this.MiscOptionsPage.Controls.Add(this.LanguageLbl);
            this.MiscOptionsPage.Controls.Add(this.TrueLnkPathsChk);
            this.MiscOptionsPage.Controls.Add(this.UsePreviewModeInMainMenuChk);
            this.MiscOptionsPage.Controls.Add(this.AppendSepForDirChk);
            this.MiscOptionsPage.Controls.Add(this.UseFQDNChk);
            this.MiscOptionsPage.Controls.Add(this.AreQuotesOptionalChk);
            this.MiscOptionsPage.Controls.Add(this.CtrlKeyPluginCombo);
            this.MiscOptionsPage.Controls.Add(this.CtrlKeyPluginChk);
            this.MiscOptionsPage.Controls.Add(this.EncodeURICharsChk);
            this.MiscOptionsPage.Controls.Add(this.EncodeURIWhitespaceChk);
            this.MiscOptionsPage.Controls.Add(this.CopyOnSameLineChk);
            this.MiscOptionsPage.Controls.Add(this.DropRedundantWordsChk);
            this.MiscOptionsPage.Controls.Add(this.UsePreviewModeChk);
            this.MiscOptionsPage.Controls.Add(this.UseIconForSubmenuChk);
            this.MiscOptionsPage.Controls.Add(this.EmailLinksChk);
            this.MiscOptionsPage.Controls.Add(this.EnableSoftwareUpdateChk);
            this.MiscOptionsPage.Controls.Add(this.AlwaysShowSubmenuChk);
            this.MiscOptionsPage.Controls.Add(this.HiddenSharesChk);
            this.MiscOptionsPage.Controls.Add(this.AddQuotesChk);
            resources.ApplyResources(this.MiscOptionsPage, "MiscOptionsPage");
            this.MiscOptionsPage.Name = "MiscOptionsPage";
            this.MiscOptionsPage.UseVisualStyleBackColor = true;
            // 
            // RecursiveCopyChk
            // 
            resources.ApplyResources(this.RecursiveCopyChk, "RecursiveCopyChk");
            this.RecursiveCopyChk.Name = "RecursiveCopyChk";
            this.MainToolTip.SetToolTip(this.RecursiveCopyChk, resources.GetString("RecursiveCopyChk.ToolTip"));
            this.RecursiveCopyChk.UseVisualStyleBackColor = true;
            // 
            // LanguageCombo
            // 
            resources.ApplyResources(this.LanguageCombo, "LanguageCombo");
            this.LanguageCombo.DataSource = this.LanguageComboBindingSource;
            this.LanguageCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.LanguageCombo.FormattingEnabled = true;
            this.LanguageCombo.Name = "LanguageCombo";
            this.MainToolTip.SetToolTip(this.LanguageCombo, resources.GetString("LanguageCombo.ToolTip"));
            this.LanguageCombo.SelectedIndexChanged += new System.EventHandler(this.LanguageCombo_SelectedIndexChanged);
            // 
            // LanguageLbl
            // 
            resources.ApplyResources(this.LanguageLbl, "LanguageLbl");
            this.LanguageLbl.Name = "LanguageLbl";
            // 
            // TrueLnkPathsChk
            // 
            resources.ApplyResources(this.TrueLnkPathsChk, "TrueLnkPathsChk");
            this.TrueLnkPathsChk.Name = "TrueLnkPathsChk";
            this.MainToolTip.SetToolTip(this.TrueLnkPathsChk, resources.GetString("TrueLnkPathsChk.ToolTip"));
            this.TrueLnkPathsChk.UseVisualStyleBackColor = true;
            // 
            // UsePreviewModeInMainMenuChk
            // 
            resources.ApplyResources(this.UsePreviewModeInMainMenuChk, "UsePreviewModeInMainMenuChk");
            this.UsePreviewModeInMainMenuChk.Name = "UsePreviewModeInMainMenuChk";
            this.MainToolTip.SetToolTip(this.UsePreviewModeInMainMenuChk, resources.GetString("UsePreviewModeInMainMenuChk.ToolTip"));
            this.UsePreviewModeInMainMenuChk.UseVisualStyleBackColor = true;
            // 
            // AppendSepForDirChk
            // 
            resources.ApplyResources(this.AppendSepForDirChk, "AppendSepForDirChk");
            this.AppendSepForDirChk.Name = "AppendSepForDirChk";
            this.MainToolTip.SetToolTip(this.AppendSepForDirChk, resources.GetString("AppendSepForDirChk.ToolTip"));
            this.AppendSepForDirChk.UseVisualStyleBackColor = true;
            // 
            // UseFQDNChk
            // 
            resources.ApplyResources(this.UseFQDNChk, "UseFQDNChk");
            this.UseFQDNChk.Name = "UseFQDNChk";
            this.MainToolTip.SetToolTip(this.UseFQDNChk, resources.GetString("UseFQDNChk.ToolTip"));
            this.UseFQDNChk.UseVisualStyleBackColor = true;
            // 
            // AreQuotesOptionalChk
            // 
            resources.ApplyResources(this.AreQuotesOptionalChk, "AreQuotesOptionalChk");
            this.AreQuotesOptionalChk.Name = "AreQuotesOptionalChk";
            this.MainToolTip.SetToolTip(this.AreQuotesOptionalChk, resources.GetString("AreQuotesOptionalChk.ToolTip"));
            this.AreQuotesOptionalChk.UseVisualStyleBackColor = true;
            // 
            // CtrlKeyPluginCombo
            // 
            resources.ApplyResources(this.CtrlKeyPluginCombo, "CtrlKeyPluginCombo");
            this.CtrlKeyPluginCombo.DataSource = this.CtrlKeyPluginComboBindingSource;
            this.CtrlKeyPluginCombo.DisplayMember = "Plugin";
            this.CtrlKeyPluginCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CtrlKeyPluginCombo.FormattingEnabled = true;
            this.CtrlKeyPluginCombo.Name = "CtrlKeyPluginCombo";
            this.MainToolTip.SetToolTip(this.CtrlKeyPluginCombo, resources.GetString("CtrlKeyPluginCombo.ToolTip"));
            this.CtrlKeyPluginCombo.ValueMember = "Plugin";
            this.CtrlKeyPluginCombo.SelectedIndexChanged += new System.EventHandler(this.CtrlKeyPluginCombo_SelectedIndexChanged);
            // 
            // CtrlKeyPluginChk
            // 
            resources.ApplyResources(this.CtrlKeyPluginChk, "CtrlKeyPluginChk");
            this.CtrlKeyPluginChk.Name = "CtrlKeyPluginChk";
            this.MainToolTip.SetToolTip(this.CtrlKeyPluginChk, resources.GetString("CtrlKeyPluginChk.ToolTip"));
            this.CtrlKeyPluginChk.UseVisualStyleBackColor = true;
            this.CtrlKeyPluginChk.CheckedChanged += new System.EventHandler(this.CtrlKeyPluginChk_CheckedChanged);
            // 
            // EncodeURICharsChk
            // 
            resources.ApplyResources(this.EncodeURICharsChk, "EncodeURICharsChk");
            this.EncodeURICharsChk.Name = "EncodeURICharsChk";
            this.MainToolTip.SetToolTip(this.EncodeURICharsChk, resources.GetString("EncodeURICharsChk.ToolTip"));
            this.EncodeURICharsChk.UseVisualStyleBackColor = true;
            // 
            // EncodeURIWhitespaceChk
            // 
            resources.ApplyResources(this.EncodeURIWhitespaceChk, "EncodeURIWhitespaceChk");
            this.EncodeURIWhitespaceChk.Name = "EncodeURIWhitespaceChk";
            this.MainToolTip.SetToolTip(this.EncodeURIWhitespaceChk, resources.GetString("EncodeURIWhitespaceChk.ToolTip"));
            this.EncodeURIWhitespaceChk.UseVisualStyleBackColor = true;
            this.EncodeURIWhitespaceChk.CheckedChanged += new System.EventHandler(this.EncodeURIWhitespaceChk_CheckedChanged);
            // 
            // CopyOnSameLineChk
            // 
            resources.ApplyResources(this.CopyOnSameLineChk, "CopyOnSameLineChk");
            this.CopyOnSameLineChk.Name = "CopyOnSameLineChk";
            this.MainToolTip.SetToolTip(this.CopyOnSameLineChk, resources.GetString("CopyOnSameLineChk.ToolTip"));
            this.CopyOnSameLineChk.UseVisualStyleBackColor = true;
            // 
            // DropRedundantWordsChk
            // 
            resources.ApplyResources(this.DropRedundantWordsChk, "DropRedundantWordsChk");
            this.DropRedundantWordsChk.Name = "DropRedundantWordsChk";
            this.MainToolTip.SetToolTip(this.DropRedundantWordsChk, resources.GetString("DropRedundantWordsChk.ToolTip"));
            this.DropRedundantWordsChk.UseVisualStyleBackColor = true;
            // 
            // UsePreviewModeChk
            // 
            resources.ApplyResources(this.UsePreviewModeChk, "UsePreviewModeChk");
            this.UsePreviewModeChk.Name = "UsePreviewModeChk";
            this.MainToolTip.SetToolTip(this.UsePreviewModeChk, resources.GetString("UsePreviewModeChk.ToolTip"));
            this.UsePreviewModeChk.UseVisualStyleBackColor = true;
            this.UsePreviewModeChk.CheckedChanged += new System.EventHandler(this.UsePreviewModeChk_CheckedChanged);
            // 
            // UseIconForSubmenuChk
            // 
            resources.ApplyResources(this.UseIconForSubmenuChk, "UseIconForSubmenuChk");
            this.UseIconForSubmenuChk.Name = "UseIconForSubmenuChk";
            this.MainToolTip.SetToolTip(this.UseIconForSubmenuChk, resources.GetString("UseIconForSubmenuChk.ToolTip"));
            this.UseIconForSubmenuChk.UseVisualStyleBackColor = true;
            // 
            // EmailLinksChk
            // 
            resources.ApplyResources(this.EmailLinksChk, "EmailLinksChk");
            this.EmailLinksChk.Name = "EmailLinksChk";
            this.MainToolTip.SetToolTip(this.EmailLinksChk, resources.GetString("EmailLinksChk.ToolTip"));
            this.EmailLinksChk.UseVisualStyleBackColor = true;
            // 
            // EnableSoftwareUpdateChk
            // 
            resources.ApplyResources(this.EnableSoftwareUpdateChk, "EnableSoftwareUpdateChk");
            this.EnableSoftwareUpdateChk.Name = "EnableSoftwareUpdateChk";
            this.MainToolTip.SetToolTip(this.EnableSoftwareUpdateChk, resources.GetString("EnableSoftwareUpdateChk.ToolTip"));
            this.EnableSoftwareUpdateChk.UseVisualStyleBackColor = true;
            // 
            // AlwaysShowSubmenuChk
            // 
            resources.ApplyResources(this.AlwaysShowSubmenuChk, "AlwaysShowSubmenuChk");
            this.AlwaysShowSubmenuChk.Name = "AlwaysShowSubmenuChk";
            this.MainToolTip.SetToolTip(this.AlwaysShowSubmenuChk, resources.GetString("AlwaysShowSubmenuChk.ToolTip"));
            this.AlwaysShowSubmenuChk.UseVisualStyleBackColor = true;
            // 
            // HiddenSharesChk
            // 
            resources.ApplyResources(this.HiddenSharesChk, "HiddenSharesChk");
            this.HiddenSharesChk.Name = "HiddenSharesChk";
            this.MainToolTip.SetToolTip(this.HiddenSharesChk, resources.GetString("HiddenSharesChk.ToolTip"));
            this.HiddenSharesChk.UseVisualStyleBackColor = true;
            // 
            // AddQuotesChk
            // 
            resources.ApplyResources(this.AddQuotesChk, "AddQuotesChk");
            this.AddQuotesChk.Name = "AddQuotesChk";
            this.MainToolTip.SetToolTip(this.AddQuotesChk, resources.GetString("AddQuotesChk.ToolTip"));
            this.AddQuotesChk.UseVisualStyleBackColor = true;
            this.AddQuotesChk.CheckedChanged += new System.EventHandler(this.AddQuotesChk_CheckedChanged);
            // 
            // AboutPage
            // 
            this.AboutPage.Controls.Add(this.AboutTableLayoutPanel);
            resources.ApplyResources(this.AboutPage, "AboutPage");
            this.AboutPage.Name = "AboutPage";
            this.AboutPage.UseVisualStyleBackColor = true;
            // 
            // AboutTableLayoutPanel
            // 
            resources.ApplyResources(this.AboutTableLayoutPanel, "AboutTableLayoutPanel");
            this.AboutTableLayoutPanel.Controls.Add(this.ProductAndVersionLbl, 0, 0);
            this.AboutTableLayoutPanel.Controls.Add(this.CopyrightLbl, 0, 1);
            this.AboutTableLayoutPanel.Controls.Add(this.VisitWebsiteLbl, 0, 2);
            this.AboutTableLayoutPanel.Controls.Add(this.SiteLinkLbl, 0, 3);
            this.AboutTableLayoutPanel.Controls.Add(this.LicenseExplanationLbl, 0, 4);
            this.AboutTableLayoutPanel.Controls.Add(this.LicenseTxtLinkLbl, 0, 5);
            this.AboutTableLayoutPanel.Controls.Add(this.DonationLinkLbl, 0, 6);
            this.AboutTableLayoutPanel.Name = "AboutTableLayoutPanel";
            // 
            // ProductAndVersionLbl
            // 
            resources.ApplyResources(this.ProductAndVersionLbl, "ProductAndVersionLbl");
            this.ProductAndVersionLbl.Name = "ProductAndVersionLbl";
            // 
            // CopyrightLbl
            // 
            resources.ApplyResources(this.CopyrightLbl, "CopyrightLbl");
            this.CopyrightLbl.Name = "CopyrightLbl";
            // 
            // VisitWebsiteLbl
            // 
            resources.ApplyResources(this.VisitWebsiteLbl, "VisitWebsiteLbl");
            this.VisitWebsiteLbl.Name = "VisitWebsiteLbl";
            // 
            // SiteLinkLbl
            // 
            resources.ApplyResources(this.SiteLinkLbl, "SiteLinkLbl");
            this.SiteLinkLbl.Name = "SiteLinkLbl";
            this.SiteLinkLbl.TabStop = true;
            this.SiteLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.SiteLinkLbl_LinkClicked);
            // 
            // LicenseExplanationLbl
            // 
            resources.ApplyResources(this.LicenseExplanationLbl, "LicenseExplanationLbl");
            this.LicenseExplanationLbl.Name = "LicenseExplanationLbl";
            // 
            // LicenseTxtLinkLbl
            // 
            resources.ApplyResources(this.LicenseTxtLinkLbl, "LicenseTxtLinkLbl");
            this.LicenseTxtLinkLbl.Name = "LicenseTxtLinkLbl";
            this.LicenseTxtLinkLbl.TabStop = true;
            this.LicenseTxtLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.LicenseTxtLinkLbl_LinkClicked);
            // 
            // DonationLinkLbl
            // 
            resources.ApplyResources(this.DonationLinkLbl, "DonationLinkLbl");
            this.DonationLinkLbl.Name = "DonationLinkLbl";
            this.DonationLinkLbl.TabStop = true;
            this.DonationLinkLbl.UseCompatibleTextRendering = true;
            this.DonationLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.DonationLinkLbl_LinkClicked);
            // 
            // ExportPipelinePluginsSaveDlg
            // 
            this.ExportPipelinePluginsSaveDlg.DefaultExt = "ecc";
            resources.ApplyResources(this.ExportPipelinePluginsSaveDlg, "ExportPipelinePluginsSaveDlg");
            // 
            // ImportPipelinePluginsOpenDlg
            // 
            this.ImportPipelinePluginsOpenDlg.DefaultExt = "ecc";
            resources.ApplyResources(this.ImportPipelinePluginsOpenDlg, "ImportPipelinePluginsOpenDlg");
            // 
            // ChoosePluginIconOpenDlg
            // 
            resources.ApplyResources(this.ChoosePluginIconOpenDlg, "ChoosePluginIconOpenDlg");
            // 
            // ExportUserSettingsSaveDlg
            // 
            this.ExportUserSettingsSaveDlg.DefaultExt = "reg";
            resources.ApplyResources(this.ExportUserSettingsSaveDlg, "ExportUserSettingsSaveDlg");
            // 
            // ExportUserSettingsBtn
            // 
            resources.ApplyResources(this.ExportUserSettingsBtn, "ExportUserSettingsBtn");
            this.ExportUserSettingsBtn.Name = "ExportUserSettingsBtn";
            this.MainToolTip.SetToolTip(this.ExportUserSettingsBtn, resources.GetString("ExportUserSettingsBtn.ToolTip"));
            this.ExportUserSettingsBtn.UseVisualStyleBackColor = true;
            this.ExportUserSettingsBtn.Click += new System.EventHandler(this.ExportUserSettingsBtn_Click);
            // 
            // MainForm
            // 
            this.AcceptButton = this.OKBtn;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.Controls.Add(this.ExportUserSettingsBtn);
            this.Controls.Add(this.MainTabCtrl);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.ApplyBtn);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.MainForm_HelpButtonClicked);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.MainTabCtrl.ResumeLayout(false);
            this.PluginsPage.ResumeLayout(false);
            this.PluginsPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGrid)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGridBindingSource)).EndInit();
            this.MiscOptionsPage.ResumeLayout(false);
            this.MiscOptionsPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.LanguageComboBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CtrlKeyPluginComboBindingSource)).EndInit();
            this.AboutPage.ResumeLayout(false);
            this.AboutTableLayoutPanel.ResumeLayout(false);
            this.AboutTableLayoutPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button ApplyBtn;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.TabControl MainTabCtrl;
        private System.Windows.Forms.TabPage MiscOptionsPage;
        private System.Windows.Forms.TabPage PluginsPage;
        private System.Windows.Forms.CheckBox AddQuotesChk;
        private System.Windows.Forms.CheckBox HiddenSharesChk;
        private System.Windows.Forms.CheckBox AlwaysShowSubmenuChk;
        private System.Windows.Forms.Label PluginsExplanationLbl;
        private System.Windows.Forms.SaveFileDialog ExportPipelinePluginsSaveDlg;
        private System.Windows.Forms.OpenFileDialog ImportPipelinePluginsOpenDlg;
        private System.Windows.Forms.TabPage AboutPage;
        private System.Windows.Forms.TableLayoutPanel AboutTableLayoutPanel;
        private System.Windows.Forms.Label ProductAndVersionLbl;
        private System.Windows.Forms.Label CopyrightLbl;
        private System.Windows.Forms.Label VisitWebsiteLbl;
        private System.Windows.Forms.LinkLabel SiteLinkLbl;
        private System.Windows.Forms.Label LicenseExplanationLbl;
        private System.Windows.Forms.LinkLabel LicenseTxtLinkLbl;
        private System.Windows.Forms.LinkLabel DonationLinkLbl;
        private System.Windows.Forms.CheckBox EnableSoftwareUpdateChk;
        private System.Windows.Forms.CheckBox UseIconForSubmenuChk;
        private System.Windows.Forms.CheckBox EmailLinksChk;
        private System.Windows.Forms.CheckBox UsePreviewModeChk;
        private System.Windows.Forms.CheckBox DropRedundantWordsChk;
        private System.Windows.Forms.OpenFileDialog ChoosePluginIconOpenDlg;
        private System.Windows.Forms.CheckBox CopyOnSameLineChk;
        private System.Windows.Forms.CheckBox EncodeURIWhitespaceChk;
        private System.Windows.Forms.CheckBox EncodeURICharsChk;
        private System.Windows.Forms.SaveFileDialog ExportUserSettingsSaveDlg;
        private System.Windows.Forms.Button ExportUserSettingsBtn;
        private System.Windows.Forms.ToolTip MainToolTip;
        private System.Windows.Forms.Label PluginsExplanationLbl2;
        private System.Windows.Forms.DataGridView PluginsDataGrid;
        private System.Windows.Forms.Button ImportPipelinePluginsBtn;
        private System.Windows.Forms.Button ExportPipelinePluginsBtn;
        private System.Windows.Forms.Label BevelLineLbl2;
        private System.Windows.Forms.Button RemovePluginBtn;
        private System.Windows.Forms.Button EditPipelinePluginBtn;
        private System.Windows.Forms.Button AddPipelinePluginBtn;
        private System.Windows.Forms.Label BevelLineLbl;
        private System.Windows.Forms.Button MovePluginDownBtn;
        private System.Windows.Forms.Button MovePluginUpBtn;
        private System.Windows.Forms.Button AddSeparatorBtn;
        private System.Windows.Forms.ComboBox CtrlKeyPluginCombo;
        private System.Windows.Forms.CheckBox CtrlKeyPluginChk;
        private System.Windows.Forms.CheckBox AreQuotesOptionalChk;
        private System.Windows.Forms.CheckBox UseFQDNChk;
        private System.Windows.Forms.CheckBox AppendSepForDirChk;
        private System.Windows.Forms.CheckBox UsePreviewModeInMainMenuChk;
        private System.Windows.Forms.BindingSource PluginsDataGridBindingSource;
        private System.Windows.Forms.BindingSource CtrlKeyPluginComboBindingSource;
        private UserControls.PluginPreviewUserControl PreviewCtrl;
        private System.Windows.Forms.CheckBox TrueLnkPathsChk;
        private System.Windows.Forms.Button DuplicatePipelinePluginBtn;
        private System.Windows.Forms.DataGridViewImageColumn IconCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn PluginCol;
        private System.Windows.Forms.DataGridViewCheckBoxColumn InMainMenuCol;
        private System.Windows.Forms.DataGridViewCheckBoxColumn InSubmenuCol;
        private System.Windows.Forms.ComboBox LanguageCombo;
        private System.Windows.Forms.Label LanguageLbl;
        private System.Windows.Forms.BindingSource LanguageComboBindingSource;
        private System.Windows.Forms.CheckBox RecursiveCopyChk;
    }
}

