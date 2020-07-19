namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class PipelineElementWithPluginIDUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PipelineElementWithPluginIDUserControl));
            this.BasePluginLst = new System.Windows.Forms.ListBox();
            this.BasePluginToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // BasePluginLst
            // 
            resources.ApplyResources(this.BasePluginLst, "BasePluginLst");
            this.BasePluginLst.FormattingEnabled = true;
            this.BasePluginLst.Name = "BasePluginLst";
            this.BasePluginToolTip.SetToolTip(this.BasePluginLst, resources.GetString("BasePluginLst.ToolTip"));
            this.BasePluginLst.SelectedIndexChanged += new System.EventHandler(this.BasePluginLst_SelectedIndexChanged);
            // 
            // PipelineElementWithPluginIDUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.BasePluginLst);
            this.Name = "PipelineElementWithPluginIDUserControl";
            this.BasePluginToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox BasePluginLst;
        private System.Windows.Forms.ToolTip BasePluginToolTip;
    }
}
