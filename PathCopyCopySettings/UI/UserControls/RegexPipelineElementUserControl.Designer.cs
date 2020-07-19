namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class RegexPipelineElementUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RegexPipelineElementUserControl));
            this.FindLbl = new System.Windows.Forms.Label();
            this.ReplaceLbl = new System.Windows.Forms.Label();
            this.ReplaceTxt = new System.Windows.Forms.TextBox();
            this.FindTxt = new System.Windows.Forms.TextBox();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.TestBtn = new System.Windows.Forms.Button();
            this.RegexToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // FindLbl
            // 
            resources.ApplyResources(this.FindLbl, "FindLbl");
            this.FindLbl.Name = "FindLbl";
            this.RegexToolTip.SetToolTip(this.FindLbl, resources.GetString("FindLbl.ToolTip"));
            // 
            // ReplaceLbl
            // 
            resources.ApplyResources(this.ReplaceLbl, "ReplaceLbl");
            this.ReplaceLbl.Name = "ReplaceLbl";
            this.RegexToolTip.SetToolTip(this.ReplaceLbl, resources.GetString("ReplaceLbl.ToolTip"));
            // 
            // ReplaceTxt
            // 
            resources.ApplyResources(this.ReplaceTxt, "ReplaceTxt");
            this.ReplaceTxt.Name = "ReplaceTxt";
            this.RegexToolTip.SetToolTip(this.ReplaceTxt, resources.GetString("ReplaceTxt.ToolTip"));
            this.ReplaceTxt.TextChanged += new System.EventHandler(this.ReplaceTxt_TextChanged);
            // 
            // FindTxt
            // 
            resources.ApplyResources(this.FindTxt, "FindTxt");
            this.FindTxt.Name = "FindTxt";
            this.RegexToolTip.SetToolTip(this.FindTxt, resources.GetString("FindTxt.ToolTip"));
            this.FindTxt.TextChanged += new System.EventHandler(this.FindTxt_TextChanged);
            // 
            // IgnoreCaseChk
            // 
            resources.ApplyResources(this.IgnoreCaseChk, "IgnoreCaseChk");
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.RegexToolTip.SetToolTip(this.IgnoreCaseChk, resources.GetString("IgnoreCaseChk.ToolTip"));
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            this.IgnoreCaseChk.CheckedChanged += new System.EventHandler(this.IgnoreCaseChk_CheckedChanged);
            // 
            // TestBtn
            // 
            resources.ApplyResources(this.TestBtn, "TestBtn");
            this.TestBtn.Name = "TestBtn";
            this.RegexToolTip.SetToolTip(this.TestBtn, resources.GetString("TestBtn.ToolTip"));
            this.TestBtn.UseVisualStyleBackColor = true;
            this.TestBtn.Click += new System.EventHandler(this.TestBtn_Click);
            // 
            // RegexPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.TestBtn);
            this.Controls.Add(this.IgnoreCaseChk);
            this.Controls.Add(this.FindTxt);
            this.Controls.Add(this.ReplaceTxt);
            this.Controls.Add(this.ReplaceLbl);
            this.Controls.Add(this.FindLbl);
            this.Name = "RegexPipelineElementUserControl";
            this.RegexToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label FindLbl;
        private System.Windows.Forms.Label ReplaceLbl;
        private System.Windows.Forms.TextBox ReplaceTxt;
        private System.Windows.Forms.TextBox FindTxt;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.Button TestBtn;
        private System.Windows.Forms.ToolTip RegexToolTip;
    }
}
