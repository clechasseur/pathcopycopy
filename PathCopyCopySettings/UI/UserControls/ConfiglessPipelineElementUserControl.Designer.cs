namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class ConfiglessPipelineElementUserControl
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
            this.ConfiglessLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ConfiglessLbl
            // 
            this.ConfiglessLbl.AutoSize = true;
            this.ConfiglessLbl.Location = new System.Drawing.Point(-3, 0);
            this.ConfiglessLbl.Name = "ConfiglessLbl";
            this.ConfiglessLbl.Size = new System.Drawing.Size(177, 13);
            this.ConfiglessLbl.TabIndex = 0;
            this.ConfiglessLbl.Text = "No additional configuration required.";
            // 
            // ConfiglessPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ConfiglessLbl);
            this.Name = "ConfiglessPipelineElementUserControl";
            this.Size = new System.Drawing.Size(173, 15);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ConfiglessLbl;
    }
}
