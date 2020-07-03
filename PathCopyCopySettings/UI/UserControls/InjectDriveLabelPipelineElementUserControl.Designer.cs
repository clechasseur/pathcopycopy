namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class InjectDriveLabelPipelineElementUserControl
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
            this.InjectDriveLabelInstructionsLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // InjectDriveLabelInstructionsLbl
            // 
            this.InjectDriveLabelInstructionsLbl.AutoSize = true;
            this.InjectDriveLabelInstructionsLbl.Location = new System.Drawing.Point(-3, 0);
            this.InjectDriveLabelInstructionsLbl.Name = "InjectDriveLabelInstructionsLbl";
            this.InjectDriveLabelInstructionsLbl.Size = new System.Drawing.Size(340, 13);
            this.InjectDriveLabelInstructionsLbl.TabIndex = 0;
            this.InjectDriveLabelInstructionsLbl.Text = "All instances of %DRIVELABEL% will be replaced with the drive\'s label.";
            // 
            // InjectDriveLabelPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Controls.Add(this.InjectDriveLabelInstructionsLbl);
            this.Name = "InjectDriveLabelPipelineElementUserControl";
            this.Size = new System.Drawing.Size(339, 16);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label InjectDriveLabelInstructionsLbl;
    }
}
