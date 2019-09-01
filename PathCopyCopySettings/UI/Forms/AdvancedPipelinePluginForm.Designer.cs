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
            this.NameLbl = new System.Windows.Forms.Label();
            this.NameTxt = new System.Windows.Forms.TextBox();
            this.ElementsLbl = new System.Windows.Forms.Label();
            this.ElementsLst = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(423, 395);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 6;
            this.CancelBtn.Text = "Cancel";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.CancelBtn, "Do not save this custom command and close the window");
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Location = new System.Drawing.Point(342, 395);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 5;
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
            this.SwitchBtn.TabIndex = 4;
            this.SwitchBtn.Text = "Simple Mode";
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.SwitchBtn, "Switch to Simple Mode, which is easier to use but has less customzation options");
            this.SwitchBtn.UseVisualStyleBackColor = true;
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
            // NameTxt
            // 
            this.NameTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.NameTxt.Location = new System.Drawing.Point(56, 12);
            this.NameTxt.Name = "NameTxt";
            this.NameTxt.Size = new System.Drawing.Size(442, 20);
            this.NameTxt.TabIndex = 1;
            this.AdvancedPipelinePluginToolTip.SetToolTip(this.NameTxt, "Name of this custom command");
            // 
            // ElementsLbl
            // 
            this.ElementsLbl.AutoSize = true;
            this.ElementsLbl.Location = new System.Drawing.Point(12, 38);
            this.ElementsLbl.Name = "ElementsLbl";
            this.ElementsLbl.Size = new System.Drawing.Size(53, 13);
            this.ElementsLbl.TabIndex = 2;
            this.ElementsLbl.Text = "Elements:";
            // 
            // ElementsLst
            // 
            this.ElementsLst.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.ElementsLst.FormattingEnabled = true;
            this.ElementsLst.Location = new System.Drawing.Point(12, 54);
            this.ElementsLst.Name = "ElementsLst";
            this.ElementsLst.Size = new System.Drawing.Size(158, 329);
            this.ElementsLst.TabIndex = 3;
            // 
            // AdvancedPipelinePluginForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(510, 430);
            this.Controls.Add(this.ElementsLst);
            this.Controls.Add(this.ElementsLbl);
            this.Controls.Add(this.NameTxt);
            this.Controls.Add(this.NameLbl);
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
    }
}