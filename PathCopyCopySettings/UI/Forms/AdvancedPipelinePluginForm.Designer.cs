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
            this.CancelBtn = new System.Windows.Forms.Button();
            this.OKBtn = new System.Windows.Forms.Button();
            this.SwitchBtn = new System.Windows.Forms.Button();
            this.AdvancedPipelinePluginToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(411, 412);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 2;
            this.CancelBtn.Text = "Cancel";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.CancelBtn, "Do not save this custom command and close the window");
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Location = new System.Drawing.Point(330, 412);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 1;
            this.OKBtn.Text = "OK";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.OKBtn, "Save this custom command and close the window");
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // SwitchBtn
            // 
            this.SwitchBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SwitchBtn.DialogResult = System.Windows.Forms.DialogResult.Retry;
            this.SwitchBtn.Location = new System.Drawing.Point(12, 412);
            this.SwitchBtn.Name = "SwitchBtn";
            this.SwitchBtn.Size = new System.Drawing.Size(94, 23);
            this.SwitchBtn.TabIndex = 0;
            this.SwitchBtn.Text = "Simple Mode";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.SwitchBtn, "Switch to Simple Mode, which is easier to use but has less customzation options");
            this.SwitchBtn.UseVisualStyleBackColor = true;
            // 
            // AdvancedPipelinePluginForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(498, 447);
            this.Controls.Add(this.SwitchBtn);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.CancelBtn);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AdvancedPipelinePluginForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Custom Command (Expert Mode)";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AdvancedPipelinePluginForm_FormClosing);
            this.Load += new System.EventHandler(this.AdvancedPipelinePluginForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.Button SwitchBtn;
        private System.Windows.Forms.ToolTip AdvancedPipelinePluginToolTip;
    }
}