namespace PathCopyCopy.Settings.UI.Forms
{
    partial class ImportPipelinePluginsForm
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
                listItemRegularBrush?.Dispose();
                listItemDisabledBrush?.Dispose();
                listItemSelectedBrush?.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ImportPipelinePluginsForm));
            this.ChoosePipelinePluginsLbl = new System.Windows.Forms.Label();
            this.PipelinePluginsLst = new System.Windows.Forms.ListBox();
            this.OKBtn = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.ImportPipelinePluginsToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ChoosePipelinePluginsLbl
            // 
            resources.ApplyResources(this.ChoosePipelinePluginsLbl, "ChoosePipelinePluginsLbl");
            this.ChoosePipelinePluginsLbl.Name = "ChoosePipelinePluginsLbl";
            this.ImportPipelinePluginsToolTip.SetToolTip(this.ChoosePipelinePluginsLbl, resources.GetString("ChoosePipelinePluginsLbl.ToolTip"));
            // 
            // PipelinePluginsLst
            // 
            resources.ApplyResources(this.PipelinePluginsLst, "PipelinePluginsLst");
            this.PipelinePluginsLst.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.PipelinePluginsLst.FormattingEnabled = true;
            this.PipelinePluginsLst.Name = "PipelinePluginsLst";
            this.PipelinePluginsLst.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.ImportPipelinePluginsToolTip.SetToolTip(this.PipelinePluginsLst, resources.GetString("PipelinePluginsLst.ToolTip"));
            this.PipelinePluginsLst.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.PipelinePluginsLst_DrawItem);
            this.PipelinePluginsLst.SelectedIndexChanged += new System.EventHandler(this.PipelinePluginsLst_SelectedIndexChanged);
            // 
            // OKBtn
            // 
            resources.ApplyResources(this.OKBtn, "OKBtn");
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Name = "OKBtn";
            this.ImportPipelinePluginsToolTip.SetToolTip(this.OKBtn, resources.GetString("OKBtn.ToolTip"));
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // CancelBtn
            // 
            resources.ApplyResources(this.CancelBtn, "CancelBtn");
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Name = "CancelBtn";
            this.ImportPipelinePluginsToolTip.SetToolTip(this.CancelBtn, resources.GetString("CancelBtn.ToolTip"));
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // ImportPipelinePluginsForm
            // 
            this.AcceptButton = this.OKBtn;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.PipelinePluginsLst);
            this.Controls.Add(this.ChoosePipelinePluginsLbl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ImportPipelinePluginsForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.ImportPipelinePluginsToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.Load += new System.EventHandler(this.ImportPipelinePluginsForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ChoosePipelinePluginsLbl;
        private System.Windows.Forms.ListBox PipelinePluginsLst;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.ToolTip ImportPipelinePluginsToolTip;
    }
}