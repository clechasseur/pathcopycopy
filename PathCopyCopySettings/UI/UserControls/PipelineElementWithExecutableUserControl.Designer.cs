namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class PipelineElementWithExecutableUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PipelineElementWithExecutableUserControl));
            this.ExecutableLbl = new System.Windows.Forms.Label();
            this.ExecutableTxt = new System.Windows.Forms.TextBox();
            this.BrowseForExecutableBtn = new System.Windows.Forms.Button();
            this.ChooseExecutableOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.ExecutableToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ExecutableLbl
            // 
            resources.ApplyResources(this.ExecutableLbl, "ExecutableLbl");
            this.ExecutableLbl.Name = "ExecutableLbl";
            // 
            // ExecutableTxt
            // 
            resources.ApplyResources(this.ExecutableTxt, "ExecutableTxt");
            this.ExecutableTxt.Name = "ExecutableTxt";
            this.ExecutableToolTip.SetToolTip(this.ExecutableTxt, resources.GetString("ExecutableTxt.ToolTip"));
            this.ExecutableTxt.TextChanged += new System.EventHandler(this.ExecutableTxt_TextChanged);
            // 
            // BrowseForExecutableBtn
            // 
            resources.ApplyResources(this.BrowseForExecutableBtn, "BrowseForExecutableBtn");
            this.BrowseForExecutableBtn.Name = "BrowseForExecutableBtn";
            this.ExecutableToolTip.SetToolTip(this.BrowseForExecutableBtn, resources.GetString("BrowseForExecutableBtn.ToolTip"));
            this.BrowseForExecutableBtn.UseVisualStyleBackColor = true;
            this.BrowseForExecutableBtn.Click += new System.EventHandler(this.BrowseForExecutableBtn_Click);
            // 
            // ChooseExecutableOpenDlg
            // 
            resources.ApplyResources(this.ChooseExecutableOpenDlg, "ChooseExecutableOpenDlg");
            // 
            // PipelineElementWithExecutableUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.BrowseForExecutableBtn);
            this.Controls.Add(this.ExecutableTxt);
            this.Controls.Add(this.ExecutableLbl);
            this.Name = "PipelineElementWithExecutableUserControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ExecutableLbl;
        private System.Windows.Forms.TextBox ExecutableTxt;
        private System.Windows.Forms.Button BrowseForExecutableBtn;
        private System.Windows.Forms.OpenFileDialog ChooseExecutableOpenDlg;
        private System.Windows.Forms.ToolTip ExecutableToolTip;
    }
}
