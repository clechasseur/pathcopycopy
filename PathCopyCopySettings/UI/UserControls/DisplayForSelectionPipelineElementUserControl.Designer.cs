
namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class DisplayForSelectionPipelineElementUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DisplayForSelectionPipelineElementUserControl));
            this.ShowForFilesCheck = new System.Windows.Forms.CheckBox();
            this.ShowForFoldersCheck = new System.Windows.Forms.CheckBox();
            this.DisplayOnSelectionToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ShowForFilesCheck
            // 
            resources.ApplyResources(this.ShowForFilesCheck, "ShowForFilesCheck");
            this.ShowForFilesCheck.Name = "ShowForFilesCheck";
            this.DisplayOnSelectionToolTip.SetToolTip(this.ShowForFilesCheck, resources.GetString("ShowForFilesCheck.ToolTip"));
            this.ShowForFilesCheck.UseVisualStyleBackColor = true;
            this.ShowForFilesCheck.CheckedChanged += new System.EventHandler(this.ShowForFilesCheck_CheckedChanged);
            // 
            // ShowForFoldersCheck
            // 
            resources.ApplyResources(this.ShowForFoldersCheck, "ShowForFoldersCheck");
            this.ShowForFoldersCheck.Name = "ShowForFoldersCheck";
            this.DisplayOnSelectionToolTip.SetToolTip(this.ShowForFoldersCheck, resources.GetString("ShowForFoldersCheck.ToolTip"));
            this.ShowForFoldersCheck.UseVisualStyleBackColor = true;
            this.ShowForFoldersCheck.CheckedChanged += new System.EventHandler(this.ShowForFoldersCheck_CheckedChanged);
            // 
            // DisplayForSelectionPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.ShowForFoldersCheck);
            this.Controls.Add(this.ShowForFilesCheck);
            this.Name = "DisplayForSelectionPipelineElementUserControl";
            this.DisplayOnSelectionToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox ShowForFilesCheck;
        private System.Windows.Forms.CheckBox ShowForFoldersCheck;
        private System.Windows.Forms.ToolTip DisplayOnSelectionToolTip;
    }
}
