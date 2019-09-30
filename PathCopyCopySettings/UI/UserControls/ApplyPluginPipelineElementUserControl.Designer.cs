namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class ApplyPluginPipelineElementUserControl
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
            this.BasePluginLst = new System.Windows.Forms.ListBox();
            this.BasePluginToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // BasePluginLst
            // 
            this.BasePluginLst.Dock = System.Windows.Forms.DockStyle.Fill;
            this.BasePluginLst.FormattingEnabled = true;
            this.BasePluginLst.Location = new System.Drawing.Point(0, 0);
            this.BasePluginLst.Name = "BasePluginLst";
            this.BasePluginLst.Size = new System.Drawing.Size(314, 306);
            this.BasePluginLst.TabIndex = 0;
            this.BasePluginToolTip.SetToolTip(this.BasePluginLst, "Base command to use to fetch path");
            this.BasePluginLst.SelectedIndexChanged += new System.EventHandler(this.BasePluginLst_SelectedIndexChanged);
            // 
            // ApplyPluginPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.BasePluginLst);
            this.Name = "ApplyPluginPipelineElementUserControl";
            this.Size = new System.Drawing.Size(314, 306);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox BasePluginLst;
        private System.Windows.Forms.ToolTip BasePluginToolTip;
    }
}
