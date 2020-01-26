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
            this.ChoosePipelinePluginsLbl = new System.Windows.Forms.Label();
            this.PipelinePluginsLst = new System.Windows.Forms.ListBox();
            this.OKBtn = new System.Windows.Forms.Button();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.ImportPipelinePluginsToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ChoosePipelinePluginsLbl
            // 
            this.ChoosePipelinePluginsLbl.AutoSize = true;
            this.ChoosePipelinePluginsLbl.Location = new System.Drawing.Point(12, 9);
            this.ChoosePipelinePluginsLbl.Name = "ChoosePipelinePluginsLbl";
            this.ChoosePipelinePluginsLbl.Size = new System.Drawing.Size(214, 13);
            this.ChoosePipelinePluginsLbl.TabIndex = 0;
            this.ChoosePipelinePluginsLbl.Text = "Please choose custom commands to import:";
            // 
            // PipelinePluginsLst
            // 
            this.PipelinePluginsLst.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PipelinePluginsLst.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.PipelinePluginsLst.FormattingEnabled = true;
            this.PipelinePluginsLst.Location = new System.Drawing.Point(15, 32);
            this.PipelinePluginsLst.Name = "PipelinePluginsLst";
            this.PipelinePluginsLst.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.PipelinePluginsLst.Size = new System.Drawing.Size(364, 251);
            this.PipelinePluginsLst.TabIndex = 1;
            this.ImportPipelinePluginsToolTip.SetToolTip(this.PipelinePluginsLst, "Custom commands to be imported");
            this.PipelinePluginsLst.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.PipelinePluginsLst_DrawItem);
            this.PipelinePluginsLst.SelectedIndexChanged += new System.EventHandler(this.PipelinePluginsLst_SelectedIndexChanged);
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Location = new System.Drawing.Point(223, 295);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 2;
            this.OKBtn.Text = "OK";
            this.ImportPipelinePluginsToolTip.SetToolTip(this.OKBtn, "Import selected custom commands and close the window");
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(304, 295);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 3;
            this.CancelBtn.Text = "Cancel";
            this.ImportPipelinePluginsToolTip.SetToolTip(this.CancelBtn, "Cancel importing custom commands and close the window");
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // ImportPipelinePluginsForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(391, 330);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.PipelinePluginsLst);
            this.Controls.Add(this.ChoosePipelinePluginsLbl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(407, 369);
            this.Name = "ImportPipelinePluginsForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Select Custom Commands";
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