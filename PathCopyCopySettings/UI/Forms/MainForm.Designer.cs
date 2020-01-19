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
            if (disposing && (components != null)) {
                components.Dispose();
                settings?.Dispose();
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
            this.ApplyBtn = new System.Windows.Forms.Button();
            this.OKBtn = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.MainTabCtrl = new System.Windows.Forms.TabControl();
            this.PluginsPage = new System.Windows.Forms.TabPage();
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
            ((System.ComponentModel.ISupportInitialize)(this.CtrlKeyPluginComboBindingSource)).BeginInit();
            this.AboutPage.SuspendLayout();
            this.AboutTableLayoutPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // ApplyBtn
            // 
            this.ApplyBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ApplyBtn.Enabled = false;
            this.ApplyBtn.Location = new System.Drawing.Point(410, 563);
            this.ApplyBtn.Name = "ApplyBtn";
            this.ApplyBtn.Size = new System.Drawing.Size(75, 23);
            this.ApplyBtn.TabIndex = 4;
            this.ApplyBtn.Text = "&Apply";
            this.MainToolTip.SetToolTip(this.ApplyBtn, "Save changes made to the settings so far, leaving the window open");
            this.ApplyBtn.UseVisualStyleBackColor = true;
            this.ApplyBtn.Click += new System.EventHandler(this.ApplyBtn_Click);
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.Location = new System.Drawing.Point(248, 563);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 2;
            this.OKBtn.Text = "OK";
            this.MainToolTip.SetToolTip(this.OKBtn, "Save changes to settings and close the window");
            this.OKBtn.UseVisualStyleBackColor = true;
            this.OKBtn.Click += new System.EventHandler(this.OKBtn_Click);
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(329, 563);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 3;
            this.CancelBtn.Text = "&Cancel";
            this.MainToolTip.SetToolTip(this.CancelBtn, "Cancel all changes made so far and close the window");
            this.CancelBtn.UseVisualStyleBackColor = true;
            this.CancelBtn.Click += new System.EventHandler(this.CancelBtn_Click);
            // 
            // MainTabCtrl
            // 
            this.MainTabCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MainTabCtrl.Controls.Add(this.PluginsPage);
            this.MainTabCtrl.Controls.Add(this.MiscOptionsPage);
            this.MainTabCtrl.Controls.Add(this.AboutPage);
            this.MainTabCtrl.Location = new System.Drawing.Point(12, 12);
            this.MainTabCtrl.Name = "MainTabCtrl";
            this.MainTabCtrl.SelectedIndex = 0;
            this.MainTabCtrl.Size = new System.Drawing.Size(473, 545);
            this.MainTabCtrl.TabIndex = 0;
            // 
            // PluginsPage
            // 
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
            this.PluginsPage.Location = new System.Drawing.Point(4, 22);
            this.PluginsPage.Name = "PluginsPage";
            this.PluginsPage.Padding = new System.Windows.Forms.Padding(3);
            this.PluginsPage.Size = new System.Drawing.Size(465, 519);
            this.PluginsPage.TabIndex = 1;
            this.PluginsPage.Text = "Commands";
            this.PluginsPage.UseVisualStyleBackColor = true;
            // 
            // PreviewCtrl
            // 
            this.PreviewCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PreviewCtrl.Location = new System.Drawing.Point(9, 460);
            this.PreviewCtrl.Name = "PreviewCtrl";
            this.PreviewCtrl.Plugin = null;
            this.PreviewCtrl.Size = new System.Drawing.Size(369, 53);
            this.PreviewCtrl.TabIndex = 3;
            // 
            // AddSeparatorBtn
            // 
            this.AddSeparatorBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.AddSeparatorBtn.Location = new System.Drawing.Point(384, 139);
            this.AddSeparatorBtn.Name = "AddSeparatorBtn";
            this.AddSeparatorBtn.Size = new System.Drawing.Size(75, 23);
            this.AddSeparatorBtn.TabIndex = 8;
            this.AddSeparatorBtn.Text = "&Separator";
            this.MainToolTip.SetToolTip(this.AddSeparatorBtn, "Insert a separator after the currently selected command");
            this.AddSeparatorBtn.UseVisualStyleBackColor = true;
            this.AddSeparatorBtn.Click += new System.EventHandler(this.AddSeparatorBtn_Click);
            // 
            // ImportPipelinePluginsBtn
            // 
            this.ImportPipelinePluginsBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ImportPipelinePluginsBtn.Location = new System.Drawing.Point(384, 263);
            this.ImportPipelinePluginsBtn.Name = "ImportPipelinePluginsBtn";
            this.ImportPipelinePluginsBtn.Size = new System.Drawing.Size(75, 23);
            this.ImportPipelinePluginsBtn.TabIndex = 13;
            this.ImportPipelinePluginsBtn.Text = "&Import...";
            this.MainToolTip.SetToolTip(this.ImportPipelinePluginsBtn, "Import custom commands from a file on disk");
            this.ImportPipelinePluginsBtn.UseVisualStyleBackColor = true;
            this.ImportPipelinePluginsBtn.Click += new System.EventHandler(this.ImportPipelinePluginsBtn_Click);
            // 
            // ExportPipelinePluginsBtn
            // 
            this.ExportPipelinePluginsBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ExportPipelinePluginsBtn.Location = new System.Drawing.Point(384, 234);
            this.ExportPipelinePluginsBtn.Name = "ExportPipelinePluginsBtn";
            this.ExportPipelinePluginsBtn.Size = new System.Drawing.Size(75, 23);
            this.ExportPipelinePluginsBtn.TabIndex = 12;
            this.ExportPipelinePluginsBtn.Text = "Ex&port...";
            this.MainToolTip.SetToolTip(this.ExportPipelinePluginsBtn, "Export the selected custom commands to a file on disk");
            this.ExportPipelinePluginsBtn.UseVisualStyleBackColor = true;
            this.ExportPipelinePluginsBtn.Click += new System.EventHandler(this.ExportPipelinePluginsBtn_Click);
            // 
            // BevelLineLbl2
            // 
            this.BevelLineLbl2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.BevelLineLbl2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.BevelLineLbl2.Location = new System.Drawing.Point(384, 226);
            this.BevelLineLbl2.Name = "BevelLineLbl2";
            this.BevelLineLbl2.Size = new System.Drawing.Size(75, 2);
            this.BevelLineLbl2.TabIndex = 11;
            // 
            // RemovePluginBtn
            // 
            this.RemovePluginBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.RemovePluginBtn.Location = new System.Drawing.Point(384, 197);
            this.RemovePluginBtn.Name = "RemovePluginBtn";
            this.RemovePluginBtn.Size = new System.Drawing.Size(75, 23);
            this.RemovePluginBtn.TabIndex = 10;
            this.RemovePluginBtn.Text = "&Remove";
            this.MainToolTip.SetToolTip(this.RemovePluginBtn, "Delete the selected custom command or separator");
            this.RemovePluginBtn.UseVisualStyleBackColor = true;
            this.RemovePluginBtn.Click += new System.EventHandler(this.RemovePluginBtn_Click);
            // 
            // EditPipelinePluginBtn
            // 
            this.EditPipelinePluginBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.EditPipelinePluginBtn.Location = new System.Drawing.Point(384, 168);
            this.EditPipelinePluginBtn.Name = "EditPipelinePluginBtn";
            this.EditPipelinePluginBtn.Size = new System.Drawing.Size(75, 23);
            this.EditPipelinePluginBtn.TabIndex = 9;
            this.EditPipelinePluginBtn.Text = "&Edit...";
            this.MainToolTip.SetToolTip(this.EditPipelinePluginBtn, "Modify the selected custom command");
            this.EditPipelinePluginBtn.UseVisualStyleBackColor = true;
            this.EditPipelinePluginBtn.Click += new System.EventHandler(this.EditPipelinePluginBtn_Click);
            // 
            // AddPipelinePluginBtn
            // 
            this.AddPipelinePluginBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.AddPipelinePluginBtn.Location = new System.Drawing.Point(384, 110);
            this.AddPipelinePluginBtn.Name = "AddPipelinePluginBtn";
            this.AddPipelinePluginBtn.Size = new System.Drawing.Size(75, 23);
            this.AddPipelinePluginBtn.TabIndex = 7;
            this.AddPipelinePluginBtn.Text = "&New...";
            this.MainToolTip.SetToolTip(this.AddPipelinePluginBtn, "Create a new custom command");
            this.AddPipelinePluginBtn.UseVisualStyleBackColor = true;
            this.AddPipelinePluginBtn.Click += new System.EventHandler(this.AddPipelinePluginBtn_Click);
            // 
            // BevelLineLbl
            // 
            this.BevelLineLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.BevelLineLbl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.BevelLineLbl.Location = new System.Drawing.Point(384, 102);
            this.BevelLineLbl.Name = "BevelLineLbl";
            this.BevelLineLbl.Size = new System.Drawing.Size(75, 2);
            this.BevelLineLbl.TabIndex = 6;
            // 
            // MovePluginDownBtn
            // 
            this.MovePluginDownBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MovePluginDownBtn.Location = new System.Drawing.Point(384, 73);
            this.MovePluginDownBtn.Name = "MovePluginDownBtn";
            this.MovePluginDownBtn.Size = new System.Drawing.Size(75, 23);
            this.MovePluginDownBtn.TabIndex = 5;
            this.MovePluginDownBtn.Text = "&Down";
            this.MainToolTip.SetToolTip(this.MovePluginDownBtn, "Move the selected command down one position");
            this.MovePluginDownBtn.UseVisualStyleBackColor = true;
            this.MovePluginDownBtn.Click += new System.EventHandler(this.MovePluginDownBtn_Click);
            // 
            // MovePluginUpBtn
            // 
            this.MovePluginUpBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MovePluginUpBtn.Location = new System.Drawing.Point(384, 44);
            this.MovePluginUpBtn.Name = "MovePluginUpBtn";
            this.MovePluginUpBtn.Size = new System.Drawing.Size(75, 23);
            this.MovePluginUpBtn.TabIndex = 4;
            this.MovePluginUpBtn.Text = "&Up";
            this.MainToolTip.SetToolTip(this.MovePluginUpBtn, "Move the selected command up one position");
            this.MovePluginUpBtn.UseVisualStyleBackColor = true;
            this.MovePluginUpBtn.Click += new System.EventHandler(this.MovePluginUpBtn_Click);
            // 
            // PluginsExplanationLbl2
            // 
            this.PluginsExplanationLbl2.AutoSize = true;
            this.PluginsExplanationLbl2.Location = new System.Drawing.Point(6, 19);
            this.PluginsExplanationLbl2.Name = "PluginsExplanationLbl2";
            this.PluginsExplanationLbl2.Size = new System.Drawing.Size(429, 13);
            this.PluginsExplanationLbl2.TabIndex = 1;
            this.PluginsExplanationLbl2.Text = "well as Path Copy Copy\'s own submenu. You can also add, remove or reorder command" +
    "s.";
            // 
            // PluginsDataGrid
            // 
            this.PluginsDataGrid.AllowUserToAddRows = false;
            this.PluginsDataGrid.AllowUserToDeleteRows = false;
            this.PluginsDataGrid.AllowUserToResizeColumns = false;
            this.PluginsDataGrid.AllowUserToResizeRows = false;
            this.PluginsDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
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
            this.PluginsDataGrid.Location = new System.Drawing.Point(9, 44);
            this.PluginsDataGrid.Name = "PluginsDataGrid";
            this.PluginsDataGrid.RowHeadersVisible = false;
            this.PluginsDataGrid.RowTemplate.Height = 18;
            this.PluginsDataGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.PluginsDataGrid.Size = new System.Drawing.Size(369, 410);
            this.PluginsDataGrid.TabIndex = 2;
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
            this.IconCol.HeaderText = "Icon";
            this.IconCol.Image = global::PathCopyCopy.Settings.Properties.Resources.BlankIcon;
            this.IconCol.MinimumWidth = 16;
            this.IconCol.Name = "IconCol";
            this.IconCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.IconCol.Width = 34;
            // 
            // PluginCol
            // 
            this.PluginCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.PluginCol.DataPropertyName = "Name";
            this.PluginCol.HeaderText = "Command";
            this.PluginCol.Name = "PluginCol";
            this.PluginCol.ReadOnly = true;
            this.PluginCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.PluginCol.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // InMainMenuCol
            // 
            this.InMainMenuCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.InMainMenuCol.DataPropertyName = "ShowInMainMenuStr";
            this.InMainMenuCol.HeaderText = "Main menu";
            this.InMainMenuCol.Name = "InMainMenuCol";
            this.InMainMenuCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.InMainMenuCol.Width = 65;
            // 
            // InSubmenuCol
            // 
            this.InSubmenuCol.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.ColumnHeader;
            this.InSubmenuCol.DataPropertyName = "ShowInSubmenuStr";
            this.InSubmenuCol.HeaderText = "Submenu";
            this.InSubmenuCol.Name = "InSubmenuCol";
            this.InSubmenuCol.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.InSubmenuCol.Width = 58;
            // 
            // PluginsExplanationLbl
            // 
            this.PluginsExplanationLbl.AutoSize = true;
            this.PluginsExplanationLbl.Location = new System.Drawing.Point(6, 6);
            this.PluginsExplanationLbl.Name = "PluginsExplanationLbl";
            this.PluginsExplanationLbl.Size = new System.Drawing.Size(439, 13);
            this.PluginsExplanationLbl.TabIndex = 0;
            this.PluginsExplanationLbl.Text = "Here, you can choose which commands are shown in the main Explorer contextual men" +
    "u as";
            // 
            // MiscOptionsPage
            // 
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
            this.MiscOptionsPage.Location = new System.Drawing.Point(4, 22);
            this.MiscOptionsPage.Name = "MiscOptionsPage";
            this.MiscOptionsPage.Padding = new System.Windows.Forms.Padding(3);
            this.MiscOptionsPage.Size = new System.Drawing.Size(465, 519);
            this.MiscOptionsPage.TabIndex = 0;
            this.MiscOptionsPage.Text = "Options";
            this.MiscOptionsPage.UseVisualStyleBackColor = true;
            // 
            // TrueLnkPathsChk
            // 
            this.TrueLnkPathsChk.AutoSize = true;
            this.TrueLnkPathsChk.Location = new System.Drawing.Point(6, 328);
            this.TrueLnkPathsChk.Name = "TrueLnkPathsChk";
            this.TrueLnkPathsChk.Size = new System.Drawing.Size(235, 17);
            this.TrueLnkPathsChk.TabIndex = 14;
            this.TrueLnkPathsChk.Text = "Copy paths of sh&ortcut (.lnk) files themselves";
            this.MainToolTip.SetToolTip(this.TrueLnkPathsChk, "When copying path of a shortcut (.lnk) file, copy the path of the shortcut file i" +
        "tself instead of the path of its target");
            this.TrueLnkPathsChk.UseVisualStyleBackColor = true;
            // 
            // UsePreviewModeInMainMenuChk
            // 
            this.UsePreviewModeInMainMenuChk.AutoSize = true;
            this.UsePreviewModeInMainMenuChk.Enabled = false;
            this.UsePreviewModeInMainMenuChk.Location = new System.Drawing.Point(25, 259);
            this.UsePreviewModeInMainMenuChk.Name = "UsePreviewModeInMainMenuChk";
            this.UsePreviewModeInMainMenuChk.Size = new System.Drawing.Size(118, 17);
            this.UsePreviewModeInMainMenuChk.TabIndex = 11;
            this.UsePreviewModeInMainMenuChk.Text = "...and in mai&n menu";
            this.MainToolTip.SetToolTip(this.UsePreviewModeInMainMenuChk, "Also show previews for commands in the main contextual menu");
            this.UsePreviewModeInMainMenuChk.UseVisualStyleBackColor = true;
            // 
            // AppendSepForDirChk
            // 
            this.AppendSepForDirChk.AutoSize = true;
            this.AppendSepForDirChk.Location = new System.Drawing.Point(6, 121);
            this.AppendSepForDirChk.Name = "AppendSepForDirChk";
            this.AppendSepForDirChk.Size = new System.Drawing.Size(203, 17);
            this.AppendSepForDirChk.TabIndex = 5;
            this.AppendSepForDirChk.Text = "Append a s&eparator to directory paths";
            this.MainToolTip.SetToolTip(this.AppendSepForDirChk, "When copying directory paths, append a separator (e.g. \\ or / ) at the end of the" +
        " path");
            this.AppendSepForDirChk.UseVisualStyleBackColor = true;
            // 
            // UseFQDNChk
            // 
            this.UseFQDNChk.AutoSize = true;
            this.UseFQDNChk.Location = new System.Drawing.Point(6, 167);
            this.UseFQDNChk.Name = "UseFQDNChk";
            this.UseFQDNChk.Size = new System.Drawing.Size(303, 17);
            this.UseFQDNChk.TabIndex = 7;
            this.UseFQDNChk.Text = "Use &fully-qualified domain names when copying UNC paths";
            this.MainToolTip.SetToolTip(this.UseFQDNChk, "Use fully-qualified domain names (e.g. FQDN) when copying UNC paths");
            this.UseFQDNChk.UseVisualStyleBackColor = true;
            // 
            // AreQuotesOptionalChk
            // 
            this.AreQuotesOptionalChk.AutoSize = true;
            this.AreQuotesOptionalChk.Enabled = false;
            this.AreQuotesOptionalChk.Location = new System.Drawing.Point(25, 29);
            this.AreQuotesOptionalChk.Name = "AreQuotesOptionalChk";
            this.AreQuotesOptionalChk.Size = new System.Drawing.Size(166, 17);
            this.AreQuotesOptionalChk.TabIndex = 1;
            this.AreQuotesOptionalChk.Text = "...only if paths contain &spaces";
            this.MainToolTip.SetToolTip(this.AreQuotesOptionalChk, "Whether to only surround paths with quotes if the paths contain spaces");
            this.AreQuotesOptionalChk.UseVisualStyleBackColor = true;
            // 
            // CtrlKeyPluginCombo
            // 
            this.CtrlKeyPluginCombo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.CtrlKeyPluginCombo.DataSource = this.CtrlKeyPluginComboBindingSource;
            this.CtrlKeyPluginCombo.DisplayMember = "Plugin";
            this.CtrlKeyPluginCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CtrlKeyPluginCombo.Enabled = false;
            this.CtrlKeyPluginCombo.FormattingEnabled = true;
            this.CtrlKeyPluginCombo.Location = new System.Drawing.Point(25, 374);
            this.CtrlKeyPluginCombo.Name = "CtrlKeyPluginCombo";
            this.CtrlKeyPluginCombo.Size = new System.Drawing.Size(399, 21);
            this.CtrlKeyPluginCombo.TabIndex = 16;
            this.MainToolTip.SetToolTip(this.CtrlKeyPluginCombo, "Command to use automatically when user holds down the Ctrl key when Explorer\'s co" +
        "ntextual menu is shown");
            this.CtrlKeyPluginCombo.ValueMember = "Plugin";
            this.CtrlKeyPluginCombo.SelectedIndexChanged += new System.EventHandler(this.CtrlKeyPluginCombo_SelectedIndexChanged);
            // 
            // CtrlKeyPluginChk
            // 
            this.CtrlKeyPluginChk.AutoSize = true;
            this.CtrlKeyPluginChk.Location = new System.Drawing.Point(6, 351);
            this.CtrlKeyPluginChk.Name = "CtrlKeyPluginChk";
            this.CtrlKeyPluginChk.Size = new System.Drawing.Size(418, 17);
            this.CtrlKeyPluginChk.TabIndex = 15;
            this.CtrlKeyPluginChk.Text = "When user holds down Ctrl &key when opening contextual menu, use this command:";
            this.MainToolTip.SetToolTip(this.CtrlKeyPluginChk, "Choose a command to use automatically when user holds down the Ctrl key when Expl" +
        "orer\'s contextual menu is shown");
            this.CtrlKeyPluginChk.UseVisualStyleBackColor = true;
            this.CtrlKeyPluginChk.CheckedChanged += new System.EventHandler(this.CtrlKeyPluginChk_CheckedChanged);
            // 
            // EncodeURICharsChk
            // 
            this.EncodeURICharsChk.AutoSize = true;
            this.EncodeURICharsChk.Enabled = false;
            this.EncodeURICharsChk.Location = new System.Drawing.Point(25, 98);
            this.EncodeURICharsChk.Name = "EncodeURICharsChk";
            this.EncodeURICharsChk.Size = new System.Drawing.Size(225, 17);
            this.EncodeURICharsChk.TabIndex = 4;
            this.EncodeURICharsChk.Text = "...and all in&valid URI characters (e.g., %xx)";
            this.MainToolTip.SetToolTip(this.EncodeURICharsChk, "Replace all characters in copied paths that are invalid in a URI by percent-encod" +
        "ing ( %xx )");
            this.EncodeURICharsChk.UseVisualStyleBackColor = true;
            // 
            // EncodeURIWhitespaceChk
            // 
            this.EncodeURIWhitespaceChk.AutoSize = true;
            this.EncodeURIWhitespaceChk.Location = new System.Drawing.Point(6, 75);
            this.EncodeURIWhitespaceChk.Name = "EncodeURIWhitespaceChk";
            this.EncodeURIWhitespaceChk.Size = new System.Drawing.Size(287, 17);
            this.EncodeURIWhitespaceChk.TabIndex = 3;
            this.EncodeURIWhitespaceChk.Text = "Encode &whitespace using percent-encoding (e.g., %20)";
            this.MainToolTip.SetToolTip(this.EncodeURIWhitespaceChk, "Replace all whitespace characters in paths with %20");
            this.EncodeURIWhitespaceChk.UseVisualStyleBackColor = true;
            this.EncodeURIWhitespaceChk.CheckedChanged += new System.EventHandler(this.EncodeURIWhitespaceChk_CheckedChanged);
            // 
            // CopyOnSameLineChk
            // 
            this.CopyOnSameLineChk.AutoSize = true;
            this.CopyOnSameLineChk.Location = new System.Drawing.Point(6, 305);
            this.CopyOnSameLineChk.Name = "CopyOnSameLineChk";
            this.CopyOnSameLineChk.Size = new System.Drawing.Size(197, 17);
            this.CopyOnSameLineChk.TabIndex = 13;
            this.CopyOnSameLineChk.Text = "Copy mul&tiple paths on the same line";
            this.MainToolTip.SetToolTip(this.CopyOnSameLineChk, "When copying paths of multiple selected files/folders, copy them all on the same " +
        "line (separating them with whitespace) instead of copying them on different line" +
        "s (separated with newlines)");
            this.CopyOnSameLineChk.UseVisualStyleBackColor = true;
            // 
            // DropRedundantWordsChk
            // 
            this.DropRedundantWordsChk.AutoSize = true;
            this.DropRedundantWordsChk.Location = new System.Drawing.Point(6, 282);
            this.DropRedundantWordsChk.Name = "DropRedundantWordsChk";
            this.DropRedundantWordsChk.Size = new System.Drawing.Size(341, 17);
            this.DropRedundantWordsChk.TabIndex = 12;
            this.DropRedundantWordsChk.Text = "Drop &redundant words like \"Copy\" or \"Long/Short\" in the submenu";
            this.MainToolTip.SetToolTip(this.DropRedundantWordsChk, "Drop some redundant words when displaying commands in the submenu");
            this.DropRedundantWordsChk.UseVisualStyleBackColor = true;
            // 
            // UsePreviewModeChk
            // 
            this.UsePreviewModeChk.AutoSize = true;
            this.UsePreviewModeChk.Location = new System.Drawing.Point(6, 236);
            this.UsePreviewModeChk.Name = "UsePreviewModeChk";
            this.UsePreviewModeChk.Size = new System.Drawing.Size(312, 17);
            this.UsePreviewModeChk.TabIndex = 10;
            this.UsePreviewModeChk.Text = "Show &previews instead of command descriptions in submenu";
            this.MainToolTip.SetToolTip(this.UsePreviewModeChk, "When displaying commands in the submenu, show previews of what copied paths would" +
        " look like if such commands were selected");
            this.UsePreviewModeChk.UseVisualStyleBackColor = true;
            this.UsePreviewModeChk.CheckedChanged += new System.EventHandler(this.UsePreviewModeChk_CheckedChanged);
            // 
            // UseIconForSubmenuChk
            // 
            this.UseIconForSubmenuChk.AutoSize = true;
            this.UseIconForSubmenuChk.Location = new System.Drawing.Point(6, 213);
            this.UseIconForSubmenuChk.Name = "UseIconForSubmenuChk";
            this.UseIconForSubmenuChk.Size = new System.Drawing.Size(157, 17);
            this.UseIconForSubmenuChk.TabIndex = 9;
            this.UseIconForSubmenuChk.Text = "Show &icon next to submenu";
            this.MainToolTip.SetToolTip(this.UseIconForSubmenuChk, "Display a Path Copy Copy icon next to the submenu in Explorer\'s contextual menu");
            this.UseIconForSubmenuChk.UseVisualStyleBackColor = true;
            // 
            // EmailLinksChk
            // 
            this.EmailLinksChk.AutoSize = true;
            this.EmailLinksChk.Location = new System.Drawing.Point(6, 52);
            this.EmailLinksChk.Name = "EmailLinksChk";
            this.EmailLinksChk.Size = new System.Drawing.Size(289, 17);
            this.EmailLinksChk.TabIndex = 2;
            this.EmailLinksChk.Text = "Add < and > around copied paths (to create e-&mail links)";
            this.MainToolTip.SetToolTip(this.EmailLinksChk, "Surround all copied paths with < and > characters (this creates e-mail links)");
            this.EmailLinksChk.UseVisualStyleBackColor = true;
            // 
            // EnableSoftwareUpdateChk
            // 
            this.EnableSoftwareUpdateChk.AutoSize = true;
            this.EnableSoftwareUpdateChk.Location = new System.Drawing.Point(6, 401);
            this.EnableSoftwareUpdateChk.Name = "EnableSoftwareUpdateChk";
            this.EnableSoftwareUpdateChk.Size = new System.Drawing.Size(177, 17);
            this.EnableSoftwareUpdateChk.TabIndex = 17;
            this.EnableSoftwareUpdateChk.Text = "Check for &updates automatically";
            this.MainToolTip.SetToolTip(this.EnableSoftwareUpdateChk, "Automatically check for new versions of Path Copy Copy and offer them when they a" +
        "re released");
            this.EnableSoftwareUpdateChk.UseVisualStyleBackColor = true;
            // 
            // AlwaysShowSubmenuChk
            // 
            this.AlwaysShowSubmenuChk.AutoSize = true;
            this.AlwaysShowSubmenuChk.Location = new System.Drawing.Point(6, 190);
            this.AlwaysShowSubmenuChk.Name = "AlwaysShowSubmenuChk";
            this.AlwaysShowSubmenuChk.Size = new System.Drawing.Size(133, 17);
            this.AlwaysShowSubmenuChk.TabIndex = 8;
            this.AlwaysShowSubmenuChk.Text = "A&lways show submenu";
            this.MainToolTip.SetToolTip(this.AlwaysShowSubmenuChk, "Always display the Path Copy Copy submenu in the Explorer contextual menu (otherw" +
        "ise, only display it when the Shift key is held down)");
            this.AlwaysShowSubmenuChk.UseVisualStyleBackColor = true;
            // 
            // HiddenSharesChk
            // 
            this.HiddenSharesChk.AutoSize = true;
            this.HiddenSharesChk.Location = new System.Drawing.Point(6, 144);
            this.HiddenSharesChk.Name = "HiddenSharesChk";
            this.HiddenSharesChk.Size = new System.Drawing.Size(238, 17);
            this.HiddenSharesChk.TabIndex = 6;
            this.HiddenSharesChk.Text = "Use &hidden shares when copying UNC paths";
            this.MainToolTip.SetToolTip(this.HiddenSharesChk, "Consider hidden shares (shares ending with the $ character, including administrat" +
        "ive shares like \\\\server\\C$) when copying UNC paths");
            this.HiddenSharesChk.UseVisualStyleBackColor = true;
            // 
            // AddQuotesChk
            // 
            this.AddQuotesChk.AutoSize = true;
            this.AddQuotesChk.Location = new System.Drawing.Point(6, 6);
            this.AddQuotesChk.Name = "AddQuotesChk";
            this.AddQuotesChk.Size = new System.Drawing.Size(180, 17);
            this.AddQuotesChk.TabIndex = 0;
            this.AddQuotesChk.Text = "Add &quotes around copied paths";
            this.MainToolTip.SetToolTip(this.AddQuotesChk, "Surround all copied paths with quotes ( \" )");
            this.AddQuotesChk.UseVisualStyleBackColor = true;
            this.AddQuotesChk.CheckedChanged += new System.EventHandler(this.AddQuotesChk_CheckedChanged);
            // 
            // AboutPage
            // 
            this.AboutPage.Controls.Add(this.AboutTableLayoutPanel);
            this.AboutPage.Location = new System.Drawing.Point(4, 22);
            this.AboutPage.Name = "AboutPage";
            this.AboutPage.Padding = new System.Windows.Forms.Padding(3);
            this.AboutPage.Size = new System.Drawing.Size(465, 519);
            this.AboutPage.TabIndex = 4;
            this.AboutPage.Text = "About";
            this.AboutPage.UseVisualStyleBackColor = true;
            // 
            // AboutTableLayoutPanel
            // 
            this.AboutTableLayoutPanel.ColumnCount = 1;
            this.AboutTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.AboutTableLayoutPanel.Controls.Add(this.ProductAndVersionLbl, 0, 0);
            this.AboutTableLayoutPanel.Controls.Add(this.CopyrightLbl, 0, 1);
            this.AboutTableLayoutPanel.Controls.Add(this.VisitWebsiteLbl, 0, 2);
            this.AboutTableLayoutPanel.Controls.Add(this.SiteLinkLbl, 0, 3);
            this.AboutTableLayoutPanel.Controls.Add(this.LicenseExplanationLbl, 0, 4);
            this.AboutTableLayoutPanel.Controls.Add(this.LicenseTxtLinkLbl, 0, 5);
            this.AboutTableLayoutPanel.Controls.Add(this.DonationLinkLbl, 0, 6);
            this.AboutTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AboutTableLayoutPanel.Location = new System.Drawing.Point(3, 3);
            this.AboutTableLayoutPanel.Name = "AboutTableLayoutPanel";
            this.AboutTableLayoutPanel.RowCount = 7;
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.AboutTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.AboutTableLayoutPanel.Size = new System.Drawing.Size(459, 513);
            this.AboutTableLayoutPanel.TabIndex = 0;
            // 
            // ProductAndVersionLbl
            // 
            this.ProductAndVersionLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ProductAndVersionLbl.AutoSize = true;
            this.ProductAndVersionLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProductAndVersionLbl.Location = new System.Drawing.Point(3, 0);
            this.ProductAndVersionLbl.Name = "ProductAndVersionLbl";
            this.ProductAndVersionLbl.Size = new System.Drawing.Size(453, 80);
            this.ProductAndVersionLbl.TabIndex = 0;
            this.ProductAndVersionLbl.Text = "Path Copy Copy {0}";
            this.ProductAndVersionLbl.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // CopyrightLbl
            // 
            this.CopyrightLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.CopyrightLbl.AutoSize = true;
            this.CopyrightLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CopyrightLbl.Location = new System.Drawing.Point(3, 80);
            this.CopyrightLbl.Name = "CopyrightLbl";
            this.CopyrightLbl.Size = new System.Drawing.Size(453, 80);
            this.CopyrightLbl.TabIndex = 1;
            this.CopyrightLbl.Text = "{1}";
            this.CopyrightLbl.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // VisitWebsiteLbl
            // 
            this.VisitWebsiteLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.VisitWebsiteLbl.AutoSize = true;
            this.VisitWebsiteLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.VisitWebsiteLbl.Location = new System.Drawing.Point(3, 160);
            this.VisitWebsiteLbl.Name = "VisitWebsiteLbl";
            this.VisitWebsiteLbl.Size = new System.Drawing.Size(453, 40);
            this.VisitWebsiteLbl.TabIndex = 2;
            this.VisitWebsiteLbl.Text = "Visit our website:";
            this.VisitWebsiteLbl.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // SiteLinkLbl
            // 
            this.SiteLinkLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SiteLinkLbl.AutoSize = true;
            this.SiteLinkLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SiteLinkLbl.Location = new System.Drawing.Point(3, 200);
            this.SiteLinkLbl.Name = "SiteLinkLbl";
            this.SiteLinkLbl.Size = new System.Drawing.Size(453, 60);
            this.SiteLinkLbl.TabIndex = 3;
            this.SiteLinkLbl.TabStop = true;
            this.SiteLinkLbl.Text = "https://pathcopycopy.github.io/";
            this.SiteLinkLbl.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.SiteLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.SiteLinkLbl_LinkClicked);
            // 
            // LicenseExplanationLbl
            // 
            this.LicenseExplanationLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LicenseExplanationLbl.AutoSize = true;
            this.LicenseExplanationLbl.Location = new System.Drawing.Point(3, 260);
            this.LicenseExplanationLbl.Name = "LicenseExplanationLbl";
            this.LicenseExplanationLbl.Size = new System.Drawing.Size(453, 40);
            this.LicenseExplanationLbl.TabIndex = 4;
            this.LicenseExplanationLbl.Text = "Path Copy Copy is free software and is distributed under\r\nthe MIT License. For mo" +
    "re information, please see";
            this.LicenseExplanationLbl.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // LicenseTxtLinkLbl
            // 
            this.LicenseTxtLinkLbl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LicenseTxtLinkLbl.AutoSize = true;
            this.LicenseTxtLinkLbl.Location = new System.Drawing.Point(3, 300);
            this.LicenseTxtLinkLbl.Name = "LicenseTxtLinkLbl";
            this.LicenseTxtLinkLbl.Size = new System.Drawing.Size(453, 60);
            this.LicenseTxtLinkLbl.TabIndex = 5;
            this.LicenseTxtLinkLbl.TabStop = true;
            this.LicenseTxtLinkLbl.Text = "LICENSE";
            this.LicenseTxtLinkLbl.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.LicenseTxtLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.LicenseTxtLinkLbl_LinkClicked);
            // 
            // DonationLinkLbl
            // 
            this.DonationLinkLbl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DonationLinkLbl.AutoSize = true;
            this.DonationLinkLbl.LinkArea = new System.Windows.Forms.LinkArea(46, 6);
            this.DonationLinkLbl.Location = new System.Drawing.Point(3, 360);
            this.DonationLinkLbl.Name = "DonationLinkLbl";
            this.DonationLinkLbl.Size = new System.Drawing.Size(453, 30);
            this.DonationLinkLbl.TabIndex = 6;
            this.DonationLinkLbl.TabStop = true;
            this.DonationLinkLbl.Text = "If you like Path Copy Copy, don\'t hesitate to DONATE through PayPal!\r\nDonations h" +
    "elp fund future development and provide responsive support.";
            this.DonationLinkLbl.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.DonationLinkLbl.UseCompatibleTextRendering = true;
            this.DonationLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.DonationLinkLbl_LinkClicked);
            // 
            // ExportPipelinePluginsSaveDlg
            // 
            this.ExportPipelinePluginsSaveDlg.DefaultExt = "ecc";
            this.ExportPipelinePluginsSaveDlg.Filter = "Exported custom commands (*.eccv3)|*.eccv3|Exported custom commands (pre-17.1) (*" +
    ".ecc)|*.ecc|Exported custom commands (pre-12.0) (*.pccpp)|*.pccpp";
            // 
            // ImportPipelinePluginsOpenDlg
            // 
            this.ImportPipelinePluginsOpenDlg.DefaultExt = "ecc";
            this.ImportPipelinePluginsOpenDlg.Filter = "Exported custom commands (*.eccv3;*.ecc;*.pccpp)|*.eccv3;*.ecc;*.pccpp";
            // 
            // ChoosePluginIconOpenDlg
            // 
            this.ChoosePluginIconOpenDlg.Filter = "Image files (*.bmp;*.jpg;*.gif;*.png;*.ico)|*.bmp;*.jpg;*.gif;*.png;*.ico|All fil" +
    "es (*.*)|*.*";
            // 
            // ExportUserSettingsSaveDlg
            // 
            this.ExportUserSettingsSaveDlg.DefaultExt = "reg";
            this.ExportUserSettingsSaveDlg.Filter = "Exported user settings (*.reg)|*.reg";
            this.ExportUserSettingsSaveDlg.Title = "Export Settings As";
            // 
            // ExportUserSettingsBtn
            // 
            this.ExportUserSettingsBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.ExportUserSettingsBtn.Location = new System.Drawing.Point(12, 563);
            this.ExportUserSettingsBtn.Name = "ExportUserSettingsBtn";
            this.ExportUserSettingsBtn.Size = new System.Drawing.Size(107, 23);
            this.ExportUserSettingsBtn.TabIndex = 1;
            this.ExportUserSettingsBtn.Text = "E&xport Settings...";
            this.MainToolTip.SetToolTip(this.ExportUserSettingsBtn, "Export all settings to a file on disk");
            this.ExportUserSettingsBtn.UseVisualStyleBackColor = true;
            this.ExportUserSettingsBtn.Click += new System.EventHandler(this.ExportUserSettingsBtn_Click);
            // 
            // MainForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(497, 598);
            this.Controls.Add(this.ExportUserSettingsBtn);
            this.Controls.Add(this.MainTabCtrl);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.ApplyBtn);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(513, 637);
            this.Name = "MainForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Path Copy Copy";
            this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.MainForm_HelpButtonClicked);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.LocationChanged += new System.EventHandler(this.MainForm_LocationChanged);
            this.SizeChanged += new System.EventHandler(this.MainForm_SizeChanged);
            this.MainTabCtrl.ResumeLayout(false);
            this.PluginsPage.ResumeLayout(false);
            this.PluginsPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGrid)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PluginsDataGridBindingSource)).EndInit();
            this.MiscOptionsPage.ResumeLayout(false);
            this.MiscOptionsPage.PerformLayout();
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
        private System.Windows.Forms.DataGridViewImageColumn IconCol;
        private System.Windows.Forms.DataGridViewTextBoxColumn PluginCol;
        private System.Windows.Forms.DataGridViewCheckBoxColumn InMainMenuCol;
        private System.Windows.Forms.DataGridViewCheckBoxColumn InSubmenuCol;
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
    }
}

