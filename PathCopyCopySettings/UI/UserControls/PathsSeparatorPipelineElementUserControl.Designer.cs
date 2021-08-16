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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PathsSeparatorPipelineElementUserControl));
            this.SeparatorLbl = new System.Windows.Forms.Label();
            this.SeparatorTxt = new System.Windows.Forms.TextBox();
            this.SeparatorToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // SeparatorLbl
            // 
            resources.ApplyResources(this.SeparatorLbl, "SeparatorLbl");
            this.SeparatorLbl.Name = "SeparatorLbl";
            // 
            // SeparatorTxt
            // 
            resources.ApplyResources(this.SeparatorTxt, "SeparatorTxt");
            this.SeparatorTxt.Name = "SeparatorTxt";
            this.SeparatorToolTip.SetToolTip(this.SeparatorTxt, resources.GetString("SeparatorTxt.ToolTip"));
            this.SeparatorTxt.TextChanged += new System.EventHandler(this.SeparatorTxt_TextChanged);
            // 
            // PathsSeparatorPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.SeparatorTxt);
            this.Controls.Add(this.SeparatorLbl);
            this.Name = "PathsSeparatorPipelineElementUserControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label SeparatorLbl;
        private System.Windows.Forms.TextBox SeparatorTxt;
        private System.Windows.Forms.ToolTip SeparatorToolTip;
    }
}
