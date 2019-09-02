namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class FindReplacePipelineElementUserControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.FindLbl = new System.Windows.Forms.Label();
            this.ReplaceLbl = new System.Windows.Forms.Label();
            this.ReplaceTxt = new System.Windows.Forms.TextBox();
            this.FindTxt = new System.Windows.Forms.TextBox();
            this.FindReplaceToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // FindLbl
            // 
            this.FindLbl.AutoSize = true;
            this.FindLbl.Location = new System.Drawing.Point(-3, 3);
            this.FindLbl.Name = "FindLbl";
            this.FindLbl.Size = new System.Drawing.Size(56, 13);
            this.FindLbl.TabIndex = 0;
            this.FindLbl.Text = "Find &what:";
            // 
            // ReplaceLbl
            // 
            this.ReplaceLbl.AutoSize = true;
            this.ReplaceLbl.Location = new System.Drawing.Point(-3, 29);
            this.ReplaceLbl.Name = "ReplaceLbl";
            this.ReplaceLbl.Size = new System.Drawing.Size(72, 13);
            this.ReplaceLbl.TabIndex = 2;
            this.ReplaceLbl.Text = "&Replace with:";
            // 
            // ReplaceTxt
            // 
            this.ReplaceTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ReplaceTxt.Location = new System.Drawing.Point(75, 26);
            this.ReplaceTxt.Name = "ReplaceTxt";
            this.ReplaceTxt.Size = new System.Drawing.Size(154, 20);
            this.ReplaceTxt.TabIndex = 3;
            this.FindReplaceToolTip.SetToolTip(this.ReplaceTxt, "Character string to replace any found element in the path with");
            this.ReplaceTxt.Validated += new System.EventHandler(this.ReplaceTxt_Validated);
            // 
            // FindTxt
            // 
            this.FindTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FindTxt.Location = new System.Drawing.Point(59, 0);
            this.FindTxt.Name = "FindTxt";
            this.FindTxt.Size = new System.Drawing.Size(170, 20);
            this.FindTxt.TabIndex = 1;
            this.FindReplaceToolTip.SetToolTip(this.FindTxt, "Character string to look for and replace in the path");
            this.FindTxt.Validated += new System.EventHandler(this.FindTxt_Validated);
            // 
            // FindReplacePipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.FindTxt);
            this.Controls.Add(this.ReplaceTxt);
            this.Controls.Add(this.ReplaceLbl);
            this.Controls.Add(this.FindLbl);
            this.Name = "FindReplacePipelineElementUserControl";
            this.Size = new System.Drawing.Size(229, 47);
            this.Load += new System.EventHandler(this.FindReplacePipelineElementUserControl_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label FindLbl;
        private System.Windows.Forms.Label ReplaceLbl;
        private System.Windows.Forms.TextBox ReplaceTxt;
        private System.Windows.Forms.TextBox FindTxt;
        private System.Windows.Forms.ToolTip FindReplaceToolTip;
    }
}
