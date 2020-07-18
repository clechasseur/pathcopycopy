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
            resources.ApplyResources(this.CancelBtn, "CancelBtn");
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Name = "CancelBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.CancelBtn, resources.GetString("CancelBtn.ToolTip"));
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            resources.ApplyResources(this.OKBtn, "OKBtn");
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Name = "OKBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.OKBtn, resources.GetString("OKBtn.ToolTip"));
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // SwitchBtn
            // 
            resources.ApplyResources(this.SwitchBtn, "SwitchBtn");
            this.SwitchBtn.DialogResult = System.Windows.Forms.DialogResult.Retry;
            this.SwitchBtn.Name = "SwitchBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.SwitchBtn, resources.GetString("SwitchBtn.ToolTip"));
            this.SwitchBtn.UseVisualStyleBackColor = true;
            // 
            // NameTxt
            // 
            resources.ApplyResources(this.NameTxt, "NameTxt");
            this.NameTxt.Name = "NameTxt";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NameTxt, resources.GetString("NameTxt.ToolTip"));
            // 
            // ElementsLst
            // 
            resources.ApplyResources(this.ElementsLst, "ElementsLst");
            this.ElementsLst.DisplayMember = "DisplayValue";
            this.ElementsLst.FormattingEnabled = true;
            this.ElementsLst.Name = "ElementsLst";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.ElementsLst, resources.GetString("ElementsLst.ToolTip"));
            this.ElementsLst.SelectedIndexChanged += new System.EventHandler(this.ElementsLst_SelectedIndexChanged);
            // 
            // NewElementBtn
            // 
            resources.ApplyResources(this.NewElementBtn, "NewElementBtn");
            this.NewElementBtn.ImageList = this.ElementsImageList;
            this.NewElementBtn.Name = "NewElementBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NewElementBtn, resources.GetString("NewElementBtn.ToolTip"));
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
            resources.ApplyResources(this.DeleteElementBtn, "DeleteElementBtn");
            this.DeleteElementBtn.ImageList = this.ElementsImageList;
            this.DeleteElementBtn.Name = "DeleteElementBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.DeleteElementBtn, resources.GetString("DeleteElementBtn.ToolTip"));
            this.DeleteElementBtn.UseVisualStyleBackColor = true;
            this.DeleteElementBtn.Click += new System.EventHandler(this.DeleteElementBtn_Click);
            // 
            // MoveElementUpBtn
            // 
            resources.ApplyResources(this.MoveElementUpBtn, "MoveElementUpBtn");
            this.MoveElementUpBtn.ImageList = this.ElementsImageList;
            this.MoveElementUpBtn.Name = "MoveElementUpBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.MoveElementUpBtn, resources.GetString("MoveElementUpBtn.ToolTip"));
            this.MoveElementUpBtn.UseVisualStyleBackColor = true;
            this.MoveElementUpBtn.Click += new System.EventHandler(this.MoveElementUpBtn_Click);
            // 
            // MoveElementDownBtn
            // 
            resources.ApplyResources(this.MoveElementDownBtn, "MoveElementDownBtn");
            this.MoveElementDownBtn.ImageList = this.ElementsImageList;
            this.MoveElementDownBtn.Name = "MoveElementDownBtn";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.MoveElementDownBtn, resources.GetString("MoveElementDownBtn.ToolTip"));
            this.MoveElementDownBtn.UseVisualStyleBackColor = true;
            this.MoveElementDownBtn.Click += new System.EventHandler(this.MoveElementDownBtn_Click);
            // 
            // NameLbl
            // 
            resources.ApplyResources(this.NameLbl, "NameLbl");
            this.NameLbl.Name = "NameLbl";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NameLbl, resources.GetString("NameLbl.ToolTip"));
            // 
            // ElementsLbl
            // 
            resources.ApplyResources(this.ElementsLbl, "ElementsLbl");
            this.ElementsLbl.Name = "ElementsLbl";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.ElementsLbl, resources.GetString("ElementsLbl.ToolTip"));
            // 
            // SelectElementLbl
            // 
            resources.ApplyResources(this.SelectElementLbl, "SelectElementLbl");
            this.SelectElementLbl.Name = "SelectElementLbl";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.SelectElementLbl, resources.GetString("SelectElementLbl.ToolTip"));
            // 
            // NewElementContextMenuStrip
            // 
            resources.ApplyResources(this.NewElementContextMenuStrip, "NewElementContextMenuStrip");
            this.NewElementContextMenuStrip.Name = "NewElementContextMenuStrip";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NewElementContextMenuStrip, resources.GetString("NewElementContextMenuStrip.ToolTip"));
            // 
            // PreviewCtrl
            // 
            resources.ApplyResources(this.PreviewCtrl, "PreviewCtrl");
            this.PreviewCtrl.Name = "PreviewCtrl";
            this.PreviewCtrl.Plugin = null;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.PreviewCtrl, resources.GetString("PreviewCtrl.ToolTip"));
            // 
            // UserControlPlacementPanel
            // 
            resources.ApplyResources(this.UserControlPlacementPanel, "UserControlPlacementPanel");
            this.UserControlPlacementPanel.Name = "UserControlPlacementPanel";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.UserControlPlacementPanel, resources.GetString("UserControlPlacementPanel.ToolTip"));
            // 
            // MinVersionLbl
            // 
            resources.ApplyResources(this.MinVersionLbl, "MinVersionLbl");
            this.MinVersionLbl.Name = "MinVersionLbl";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.MinVersionLbl, resources.GetString("MinVersionLbl.ToolTip"));
            // 
            // AdvancedPipelinePluginForm
            // 
            this.AcceptButton = this.OKBtn;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
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
            this.Name = "AdvancedPipelinePluginForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
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