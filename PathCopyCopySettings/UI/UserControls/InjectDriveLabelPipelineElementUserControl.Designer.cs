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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(InjectDriveLabelPipelineElementUserControl));
            this.InjectDriveLabelInstructionsLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // InjectDriveLabelInstructionsLbl
            // 
            resources.ApplyResources(this.InjectDriveLabelInstructionsLbl, "InjectDriveLabelInstructionsLbl");
            this.InjectDriveLabelInstructionsLbl.Name = "InjectDriveLabelInstructionsLbl";
            // 
            // InjectDriveLabelPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.InjectDriveLabelInstructionsLbl);
            this.Name = "InjectDriveLabelPipelineElementUserControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label InjectDriveLabelInstructionsLbl;
    }
}
