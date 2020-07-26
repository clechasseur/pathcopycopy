namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class CommandLinePipelineElementUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CommandLinePipelineElementUserControl));
            this.ArgumentsTxt = new System.Windows.Forms.TextBox();
            this.ArgumentsLbl = new System.Windows.Forms.Label();
            this.UseFilelistChk = new System.Windows.Forms.CheckBox();
            this.CmdLineToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // ArgumentsTxt
            // 
            resources.ApplyResources(this.ArgumentsTxt, "ArgumentsTxt");
            this.ArgumentsTxt.Name = "ArgumentsTxt";
            this.CmdLineToolTip.SetToolTip(this.ArgumentsTxt, resources.GetString("ArgumentsTxt.ToolTip"));
            this.ArgumentsTxt.TextChanged += new System.EventHandler(this.ArgumentsTxt_TextChanged);
            // 
            // ArgumentsLbl
            // 
            resources.ApplyResources(this.ArgumentsLbl, "ArgumentsLbl");
            this.ArgumentsLbl.Name = "ArgumentsLbl";
            this.CmdLineToolTip.SetToolTip(this.ArgumentsLbl, resources.GetString("ArgumentsLbl.ToolTip"));
            // 
            // UseFilelistChk
            // 
            resources.ApplyResources(this.UseFilelistChk, "UseFilelistChk");
            this.UseFilelistChk.Name = "UseFilelistChk";
            this.CmdLineToolTip.SetToolTip(this.UseFilelistChk, resources.GetString("UseFilelistChk.ToolTip"));
            this.UseFilelistChk.UseVisualStyleBackColor = true;
            this.UseFilelistChk.CheckedChanged += new System.EventHandler(this.UseFilelistChk_CheckedChanged);
            // 
            // CommandLinePipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.UseFilelistChk);
            this.Controls.Add(this.ArgumentsLbl);
            this.Controls.Add(this.ArgumentsTxt);
            this.Name = "CommandLinePipelineElementUserControl";
            this.CmdLineToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.Controls.SetChildIndex(this.ArgumentsTxt, 0);
            this.Controls.SetChildIndex(this.ArgumentsLbl, 0);
            this.Controls.SetChildIndex(this.UseFilelistChk, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ArgumentsTxt;
        private System.Windows.Forms.Label ArgumentsLbl;
        private System.Windows.Forms.CheckBox UseFilelistChk;
        private System.Windows.Forms.ToolTip CmdLineToolTip;
    }
}
