namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class CommandLinePipelineElementUserControl
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
            this.ArgumentsTxt = new System.Windows.Forms.TextBox();
            this.ArgumentsLbl = new System.Windows.Forms.Label();
            this.UseFilelistChk = new System.Windows.Forms.CheckBox();
            this.CmdLineToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ArgumentsTxt
            // 
            this.ArgumentsTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ArgumentsTxt.Location = new System.Drawing.Point(66, 28);
            this.ArgumentsTxt.Name = "ArgumentsTxt";
            this.ArgumentsTxt.Size = new System.Drawing.Size(252, 20);
            this.ArgumentsTxt.TabIndex = 3;
            this.CmdLineToolTip.SetToolTip(this.ArgumentsTxt, "Arguments to pass to the executable. Include %FILES% to have it replaced by the f" +
        "ile paths, otherwise paths will be appended.");
            this.ArgumentsTxt.TextChanged += new System.EventHandler(this.ArgumentsTxt_TextChanged);
            // 
            // ArgumentsLbl
            // 
            this.ArgumentsLbl.AutoSize = true;
            this.ArgumentsLbl.Location = new System.Drawing.Point(-3, 31);
            this.ArgumentsLbl.Name = "ArgumentsLbl";
            this.ArgumentsLbl.Size = new System.Drawing.Size(60, 13);
            this.ArgumentsLbl.TabIndex = 4;
            this.ArgumentsLbl.Text = "&Arguments:";
            // 
            // UseFilelistChk
            // 
            this.UseFilelistChk.AutoSize = true;
            this.UseFilelistChk.Location = new System.Drawing.Point(0, 54);
            this.UseFilelistChk.Name = "UseFilelistChk";
            this.UseFilelistChk.Size = new System.Drawing.Size(73, 17);
            this.UseFilelistChk.TabIndex = 5;
            this.UseFilelistChk.Text = "Use &filelist";
            this.CmdLineToolTip.SetToolTip(this.UseFilelistChk, "Whether to pass paths to executable directly (unchecked) or in a filelist.txt (ch" +
        "ecked)");
            this.UseFilelistChk.UseVisualStyleBackColor = true;
            this.UseFilelistChk.CheckedChanged += new System.EventHandler(this.UseFilelistChk_CheckedChanged);
            // 
            // CommandLinePipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Controls.Add(this.UseFilelistChk);
            this.Controls.Add(this.ArgumentsLbl);
            this.Controls.Add(this.ArgumentsTxt);
            this.Name = "CommandLinePipelineElementUserControl";
            this.Size = new System.Drawing.Size(318, 79);
            this.Controls.SetChildIndex(this.ArgumentsTxt, 0);
            this.Controls.SetChildIndex(this.ArgumentsLbl, 0);
            this.Controls.SetChildIndex(this.UseFilelistChk, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ArgumentsTxt;
        private System.Windows.Forms.Label ArgumentsLbl;
        private System.Windows.Forms.CheckBox UseFilelistChk;
        private System.Windows.Forms.ToolTip CmdLineToolTip;
    }
}
