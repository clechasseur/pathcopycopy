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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfiglessPipelineElementUserControl));
            this.ConfiglessLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ConfiglessLbl
            // 
            resources.ApplyResources(this.ConfiglessLbl, "ConfiglessLbl");
            this.ConfiglessLbl.Name = "ConfiglessLbl";
            // 
            // ConfiglessPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ConfiglessLbl);
            this.Name = "ConfiglessPipelineElementUserControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ConfiglessLbl;
    }
}
