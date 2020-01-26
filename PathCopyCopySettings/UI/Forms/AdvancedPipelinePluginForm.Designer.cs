namespace PathCopyCopy.Settings.UI.Forms
{
    partial class AdvancedPipelinePluginForm
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
                currentUserControl?.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AdvancedPipelinePluginForm));
            this.CancelBtn = new System.Windows.Forms.Button();
            this.OKBtn = new System.Windows.Forms.Button();
            this.SwitchBtn = new System.Windows.Forms.Button();
            this.AdvancedPipelinePluginToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.NameTxt = new System.Windows.Forms.TextBox();
            this.ElementsLst = new System.Windows.Forms.ListBox();
            this.NewElementBtn = new System.Windows.Forms.Button();
            this.ElementsImageList = new System.Windows.Forms.ImageList(this.components);
            this.DeleteElementBtn = new System.Windows.Forms.Button();
            this.MoveElementUpBtn = new System.Windows.Forms.Button();
            this.MoveElementDownBtn = new System.Windows.Forms.Button();
            this.NameLbl = new System.Windows.Forms.Label();
            this.ElementsLbl = new System.Windows.Forms.Label();
            this.SelectElementLbl = new System.Windows.Forms.Label();
            this.NewElementContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.PreviewCtrl = new PathCopyCopy.Settings.UI.UserControls.PluginPreviewUserControl();
            this.UserControlPlacementPanel = new System.Windows.Forms.Panel();
            this.MinVersionLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(551, 395);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 14;
            this.CancelBtn.Text = "Cancel";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.CancelBtn, "Do not save this custom command and close the window");
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Location = new System.Drawing.Point(470, 395);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 13;
            this.OKBtn.Text = "OK";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.OKBtn, "Save this custom command and close the window");
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // SwitchBtn
            // 
            this.SwitchBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SwitchBtn.DialogResult = System.Windows.Forms.DialogResult.Retry;
            this.SwitchBtn.Location = new System.Drawing.Point(12, 395);
            this.SwitchBtn.Name = "SwitchBtn";
            this.SwitchBtn.Size = new System.Drawing.Size(94, 23);
            this.SwitchBtn.TabIndex = 12;
            this.SwitchBtn.Text = "Simple Mode";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.SwitchBtn, "Switch to Simple Mode, which is easier to use but has less customzation options");
            this.SwitchBtn.UseVisualStyleBackColor = true;
            // 
            // NameTxt
            // 
            this.NameTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.NameTxt.Location = new System.Drawing.Point(56, 12);
            this.NameTxt.Name = "NameTxt";
            this.NameTxt.Size = new System.Drawing.Size(570, 20);
            this.NameTxt.TabIndex = 1;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NameTxt, "Name of this custom command");
            // 
            // ElementsLst
            // 
            this.ElementsLst.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.ElementsLst.DisplayMember = "DisplayValue";
            this.ElementsLst.FormattingEnabled = true;
            this.ElementsLst.Location = new System.Drawing.Point(12, 67);
            this.ElementsLst.Name = "ElementsLst";
            this.ElementsLst.Size = new System.Drawing.Size(238, 316);
            this.ElementsLst.TabIndex = 3;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.ElementsLst, "List of elements in this custom command");
            this.ElementsLst.SelectedIndexChanged += new System.EventHandler(this.ElementsLst_SelectedIndexChanged);
            // 
            // NewElementBtn
            // 
            this.NewElementBtn.ImageIndex = 0;
            this.NewElementBtn.ImageList = this.ElementsImageList;
            this.NewElementBtn.Location = new System.Drawing.Point(140, 38);
            this.NewElementBtn.Name = "NewElementBtn";
            this.NewElementBtn.Size = new System.Drawing.Size(23, 23);
            this.NewElementBtn.TabIndex = 4;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NewElementBtn, "Add a new element to this custom command");
            this.NewElementBtn.UseVisualStyleBackColor = true;
            this.NewElementBtn.Click += new System.EventHandler(this.NewElementBtn_Click);
            // 
            // ElementsImageList
            // 
            this.ElementsImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ElementsImageList.ImageStream")));
            this.ElementsImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ElementsImageList.Images.SetKeyName(0, "NewFilter_16x.png");
            this.ElementsImageList.Images.SetKeyName(1, "DeleteFilter_16x.png");
            this.ElementsImageList.Images.SetKeyName(2, "CollapseChevronUp_16x.png");
            this.ElementsImageList.Images.SetKeyName(3, "CollapseChevronDown_16x.png");
            // 
            // DeleteElementBtn
            // 
            this.DeleteElementBtn.Enabled = false;
            this.DeleteElementBtn.ImageIndex = 1;
            this.DeleteElementBtn.ImageList = this.ElementsImageList;
            this.DeleteElementBtn.Location = new System.Drawing.Point(169, 38);
            this.DeleteElementBtn.Name = "DeleteElementBtn";
            this.DeleteElementBtn.Size = new System.Drawing.Size(23, 23);
            this.DeleteElementBtn.TabIndex = 5;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.DeleteElementBtn, "Remove the selected element from this custom command");
            this.DeleteElementBtn.UseVisualStyleBackColor = true;
            this.DeleteElementBtn.Click += new System.EventHandler(this.DeleteElementBtn_Click);
            // 
            // MoveElementUpBtn
            // 
            this.MoveElementUpBtn.Enabled = false;
            this.MoveElementUpBtn.ImageIndex = 2;
            this.MoveElementUpBtn.ImageList = this.ElementsImageList;
            this.MoveElementUpBtn.Location = new System.Drawing.Point(198, 38);
            this.MoveElementUpBtn.Name = "MoveElementUpBtn";
            this.MoveElementUpBtn.Size = new System.Drawing.Size(23, 23);
            this.MoveElementUpBtn.TabIndex = 6;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.MoveElementUpBtn, "Move the selected element up one position");
            this.MoveElementUpBtn.UseVisualStyleBackColor = true;
            this.MoveElementUpBtn.Click += new System.EventHandler(this.MoveElementUpBtn_Click);
            // 
            // MoveElementDownBtn
            // 
            this.MoveElementDownBtn.Enabled = false;
            this.MoveElementDownBtn.ImageIndex = 3;
            this.MoveElementDownBtn.ImageList = this.ElementsImageList;
            this.MoveElementDownBtn.Location = new System.Drawing.Point(227, 38);
            this.MoveElementDownBtn.Name = "MoveElementDownBtn";
            this.MoveElementDownBtn.Size = new System.Drawing.Size(23, 23);
            this.MoveElementDownBtn.TabIndex = 7;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.MoveElementDownBtn, "Move the selected element down one position");
            this.MoveElementDownBtn.UseVisualStyleBackColor = true;
            this.MoveElementDownBtn.Click += new System.EventHandler(this.MoveElementDownBtn_Click);
            // 
            // NameLbl
            // 
            this.NameLbl.AutoSize = true;
            this.NameLbl.Location = new System.Drawing.Point(12, 15);
            this.NameLbl.Name = "NameLbl";
            this.NameLbl.Size = new System.Drawing.Size(38, 13);
            this.NameLbl.TabIndex = 0;
            this.NameLbl.Text = "&Name:";
            // 
            // ElementsLbl
            // 
            this.ElementsLbl.AutoSize = true;
            this.ElementsLbl.Location = new System.Drawing.Point(12, 48);
            this.ElementsLbl.Name = "ElementsLbl";
            this.ElementsLbl.Size = new System.Drawing.Size(53, 13);
            this.ElementsLbl.TabIndex = 2;
            this.ElementsLbl.Text = "&Elements:";
            // 
            // SelectElementLbl
            // 
            this.SelectElementLbl.AutoSize = true;
            this.SelectElementLbl.Location = new System.Drawing.Point(256, 67);
            this.SelectElementLbl.Name = "SelectElementLbl";
            this.SelectElementLbl.Size = new System.Drawing.Size(266, 13);
            this.SelectElementLbl.TabIndex = 9;
            this.SelectElementLbl.Text = "Please select an element in the list to edit its properties.";
            // 
            // NewElementContextMenuStrip
            // 
            this.NewElementContextMenuStrip.Name = "NewElementContextMenuStrip";
            this.NewElementContextMenuStrip.Size = new System.Drawing.Size(61, 4);
            // 
            // PreviewCtrl
            // 
            this.PreviewCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PreviewCtrl.Location = new System.Drawing.Point(256, 330);
            this.PreviewCtrl.Name = "PreviewCtrl";
            this.PreviewCtrl.Plugin = null;
            this.PreviewCtrl.Size = new System.Drawing.Size(370, 53);
            this.PreviewCtrl.TabIndex = 11;
            // 
            // UserControlPlacementPanel
            // 
            this.UserControlPlacementPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.UserControlPlacementPanel.Location = new System.Drawing.Point(256, 67);
            this.UserControlPlacementPanel.Name = "UserControlPlacementPanel";
            this.UserControlPlacementPanel.Size = new System.Drawing.Size(370, 257);
            this.UserControlPlacementPanel.TabIndex = 10;
            this.UserControlPlacementPanel.Visible = false;
            // 
            // MinVersionLbl
            // 
            this.MinVersionLbl.AutoSize = true;
            this.MinVersionLbl.Location = new System.Drawing.Point(256, 43);
            this.MinVersionLbl.Name = "MinVersionLbl";
            this.MinVersionLbl.Size = new System.Drawing.Size(304, 13);
            this.MinVersionLbl.TabIndex = 8;
            this.MinVersionLbl.Text = "Minimum Path Copy Copy version needed to use command: {0}";
            // 
            // AdvancedPipelinePluginForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(638, 430);
            this.Controls.Add(this.MinVersionLbl);
            this.Controls.Add(this.PreviewCtrl);
            this.Controls.Add(this.SelectElementLbl);
            this.Controls.Add(this.MoveElementDownBtn);
            this.Controls.Add(this.MoveElementUpBtn);
            this.Controls.Add(this.DeleteElementBtn);
            this.Controls.Add(this.NewElementBtn);
            this.Controls.Add(this.ElementsLst);
            this.Controls.Add(this.ElementsLbl);
            this.Controls.Add(this.NameTxt);
            this.Controls.Add(this.NameLbl);
            this.Controls.Add(this.SwitchBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.UserControlPlacementPanel);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(654, 469);
            this.Name = "AdvancedPipelinePluginForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Custom Command (Expert Mode)";
            this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.AdvancedPipelinePluginForm_HelpButtonClicked);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AdvancedPipelinePluginForm_FormClosing);
            this.Load += new System.EventHandler(this.AdvancedPipelinePluginForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.Button SwitchBtn;
        private System.Windows.Forms.ToolTip AdvancedPipelinePluginToolTip;
        private System.Windows.Forms.Label NameLbl;
        private System.Windows.Forms.TextBox NameTxt;
        private System.Windows.Forms.Label ElementsLbl;
        private System.Windows.Forms.ListBox ElementsLst;
        private System.Windows.Forms.Button NewElementBtn;
        private System.Windows.Forms.ImageList ElementsImageList;
        private System.Windows.Forms.Button DeleteElementBtn;
        private System.Windows.Forms.Button MoveElementUpBtn;
        private System.Windows.Forms.Button MoveElementDownBtn;
        private System.Windows.Forms.Label SelectElementLbl;
        private System.Windows.Forms.ContextMenuStrip NewElementContextMenuStrip;
        private UserControls.PluginPreviewUserControl PreviewCtrl;
        private System.Windows.Forms.Panel UserControlPlacementPanel;
        private System.Windows.Forms.Label MinVersionLbl;
    }
}