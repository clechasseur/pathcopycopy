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
            this.ExecutableLbl = new System.Windows.Forms.Label();
            this.ExecutableTxt = new System.Windows.Forms.TextBox();
            this.BrowseForExecutableBtn = new System.Windows.Forms.Button();
            this.ChooseExecutableOpenDlg = new System.Windows.Forms.OpenFileDialog();
            this.ExecutableToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ExecutableLbl
            // 
            this.ExecutableLbl.AutoSize = true;
            this.ExecutableLbl.Location = new System.Drawing.Point(-3, 5);
            this.ExecutableLbl.Name = "ExecutableLbl";
            this.ExecutableLbl.Size = new System.Drawing.Size(63, 13);
            this.ExecutableLbl.TabIndex = 0;
            this.ExecutableLbl.Text = "E&xecutable:";
            // 
            // ExecutableTxt
            // 
            this.ExecutableTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ExecutableTxt.Location = new System.Drawing.Point(67, 2);
            this.ExecutableTxt.Name = "ExecutableTxt";
            this.ExecutableTxt.Size = new System.Drawing.Size(165, 20);
            this.ExecutableTxt.TabIndex = 1;
            this.ExecutableToolTip.SetToolTip(this.ExecutableTxt, "Path to the executable to launch");
            this.ExecutableTxt.TextChanged += new System.EventHandler(this.ExecutableTxt_TextChanged);
            // 
            // BrowseForExecutableBtn
            // 
            this.BrowseForExecutableBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.BrowseForExecutableBtn.Location = new System.Drawing.Point(238, 0);
            this.BrowseForExecutableBtn.Name = "BrowseForExecutableBtn";
            this.BrowseForExecutableBtn.Size = new System.Drawing.Size(80, 23);
            this.BrowseForExecutableBtn.TabIndex = 2;
            this.BrowseForExecutableBtn.Text = "&Browse";
            this.ExecutableToolTip.SetToolTip(this.BrowseForExecutableBtn, "Open a dialog to choose the executable to launch on disk");
            this.BrowseForExecutableBtn.UseVisualStyleBackColor = true;
            this.BrowseForExecutableBtn.Click += new System.EventHandler(this.BrowseForExecutableBtn_Click);
            // 
            // ChooseExecutableOpenDlg
            // 
            this.ChooseExecutableOpenDlg.Filter = "Executable files (*.exe;*.com;*.bat;*.cmd)|*.exe;*.com;*.bat;*.cmd|All files (*.*" +
    ")|*.*";
            // 
            // PipelineElementWithExecutableUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.BrowseForExecutableBtn);
            this.Controls.Add(this.ExecutableTxt);
            this.Controls.Add(this.ExecutableLbl);
            this.Name = "PipelineElementWithExecutableUserControl";
            this.Size = new System.Drawing.Size(318, 24);
            this.Load += new System.EventHandler(this.PipelineElementWithExecutableUserControl_Load);
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
