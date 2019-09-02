namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class PathsSeparatorPipelineElementUserControl
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
            this.SeparatorLbl = new System.Windows.Forms.Label();
            this.SeparatorTxt = new System.Windows.Forms.TextBox();
            this.SeparatorToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // SeparatorLbl
            // 
            this.SeparatorLbl.AutoSize = true;
            this.SeparatorLbl.Location = new System.Drawing.Point(-3, 3);
            this.SeparatorLbl.Name = "SeparatorLbl";
            this.SeparatorLbl.Size = new System.Drawing.Size(56, 13);
            this.SeparatorLbl.TabIndex = 0;
            this.SeparatorLbl.Text = "&Separator:";
            // 
            // SeparatorTxt
            // 
            this.SeparatorTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SeparatorTxt.Location = new System.Drawing.Point(59, 0);
            this.SeparatorTxt.Name = "SeparatorTxt";
            this.SeparatorTxt.Size = new System.Drawing.Size(172, 20);
            this.SeparatorTxt.TabIndex = 1;
            this.SeparatorToolTip.SetToolTip(this.SeparatorTxt, "Character string used to separate multiple copied paths; the default value (when " +
        "this option is not present) is a carriage return");
            this.SeparatorTxt.Validated += new System.EventHandler(this.SeparatorTxt_Validated);
            // 
            // PathsSeparatorPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.SeparatorTxt);
            this.Controls.Add(this.SeparatorLbl);
            this.Name = "PathsSeparatorPipelineElementUserControl";
            this.Size = new System.Drawing.Size(231, 20);
            this.Load += new System.EventHandler(this.PathsSeparatorPipelineElementUserControl_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label SeparatorLbl;
        private System.Windows.Forms.TextBox SeparatorTxt;
        private System.Windows.Forms.ToolTip SeparatorToolTip;
    }
}
