namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class PushToStackPipelineElementUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PushToStackPipelineElementUserControl));
            this.PushToStackLbl = new System.Windows.Forms.Label();
            this.EntirePathRadio = new System.Windows.Forms.RadioButton();
            this.RangeRadio = new System.Windows.Forms.RadioButton();
            this.RangeBeginTxt = new System.Windows.Forms.TextBox();
            this.BeginAndEndLbl = new System.Windows.Forms.Label();
            this.RangeEndTxt = new System.Windows.Forms.TextBox();
            this.RegexRadio = new System.Windows.Forms.RadioButton();
            this.RegexTxt = new System.Windows.Forms.TextBox();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.TestRegexBtn = new System.Windows.Forms.Button();
            this.UseGroupLbl1 = new System.Windows.Forms.Label();
            this.RegexGroupTxt = new System.Windows.Forms.TextBox();
            this.UseGroupLbl2 = new System.Windows.Forms.Label();
            this.FixedStringRadio = new System.Windows.Forms.RadioButton();
            this.FixedStringTxt = new System.Windows.Forms.TextBox();
            this.PushToStackToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // PushToStackLbl
            // 
            resources.ApplyResources(this.PushToStackLbl, "PushToStackLbl");
            this.PushToStackLbl.Name = "PushToStackLbl";
            // 
            // EntirePathRadio
            // 
            resources.ApplyResources(this.EntirePathRadio, "EntirePathRadio");
            this.EntirePathRadio.Name = "EntirePathRadio";
            this.EntirePathRadio.TabStop = true;
            this.EntirePathRadio.Tag = "1";
            this.PushToStackToolTip.SetToolTip(this.EntirePathRadio, resources.GetString("EntirePathRadio.ToolTip"));
            this.EntirePathRadio.UseVisualStyleBackColor = true;
            this.EntirePathRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RangeRadio
            // 
            resources.ApplyResources(this.RangeRadio, "RangeRadio");
            this.RangeRadio.Name = "RangeRadio";
            this.RangeRadio.TabStop = true;
            this.RangeRadio.Tag = "2";
            this.PushToStackToolTip.SetToolTip(this.RangeRadio, resources.GetString("RangeRadio.ToolTip"));
            this.RangeRadio.UseVisualStyleBackColor = true;
            this.RangeRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RangeBeginTxt
            // 
            resources.ApplyResources(this.RangeBeginTxt, "RangeBeginTxt");
            this.RangeBeginTxt.Name = "RangeBeginTxt";
            this.PushToStackToolTip.SetToolTip(this.RangeBeginTxt, resources.GetString("RangeBeginTxt.ToolTip"));
            this.RangeBeginTxt.TextChanged += new System.EventHandler(this.RangeBeginTxt_TextChanged);
            this.RangeBeginTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // BeginAndEndLbl
            // 
            resources.ApplyResources(this.BeginAndEndLbl, "BeginAndEndLbl");
            this.BeginAndEndLbl.Name = "BeginAndEndLbl";
            // 
            // RangeEndTxt
            // 
            resources.ApplyResources(this.RangeEndTxt, "RangeEndTxt");
            this.RangeEndTxt.Name = "RangeEndTxt";
            this.PushToStackToolTip.SetToolTip(this.RangeEndTxt, resources.GetString("RangeEndTxt.ToolTip"));
            this.RangeEndTxt.TextChanged += new System.EventHandler(this.RangeEndTxt_TextChanged);
            this.RangeEndTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // RegexRadio
            // 
            resources.ApplyResources(this.RegexRadio, "RegexRadio");
            this.RegexRadio.Name = "RegexRadio";
            this.RegexRadio.TabStop = true;
            this.RegexRadio.Tag = "3";
            this.PushToStackToolTip.SetToolTip(this.RegexRadio, resources.GetString("RegexRadio.ToolTip"));
            this.RegexRadio.UseVisualStyleBackColor = true;
            this.RegexRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RegexTxt
            // 
            resources.ApplyResources(this.RegexTxt, "RegexTxt");
            this.RegexTxt.Name = "RegexTxt";
            this.PushToStackToolTip.SetToolTip(this.RegexTxt, resources.GetString("RegexTxt.ToolTip"));
            this.RegexTxt.TextChanged += new System.EventHandler(this.RegexTxt_TextChanged);
            // 
            // IgnoreCaseChk
            // 
            resources.ApplyResources(this.IgnoreCaseChk, "IgnoreCaseChk");
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.PushToStackToolTip.SetToolTip(this.IgnoreCaseChk, resources.GetString("IgnoreCaseChk.ToolTip"));
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            this.IgnoreCaseChk.CheckedChanged += new System.EventHandler(this.IgnoreCaseChk_CheckedChanged);
            // 
            // TestRegexBtn
            // 
            resources.ApplyResources(this.TestRegexBtn, "TestRegexBtn");
            this.TestRegexBtn.Name = "TestRegexBtn";
            this.PushToStackToolTip.SetToolTip(this.TestRegexBtn, resources.GetString("TestRegexBtn.ToolTip"));
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            this.TestRegexBtn.Click += new System.EventHandler(this.TestRegexBtn_Click);
            // 
            // UseGroupLbl1
            // 
            resources.ApplyResources(this.UseGroupLbl1, "UseGroupLbl1");
            this.UseGroupLbl1.Name = "UseGroupLbl1";
            // 
            // RegexGroupTxt
            // 
            resources.ApplyResources(this.RegexGroupTxt, "RegexGroupTxt");
            this.RegexGroupTxt.Name = "RegexGroupTxt";
            this.PushToStackToolTip.SetToolTip(this.RegexGroupTxt, resources.GetString("RegexGroupTxt.ToolTip"));
            this.RegexGroupTxt.TextChanged += new System.EventHandler(this.RegexGroupTxt_TextChanged);
            this.RegexGroupTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // UseGroupLbl2
            // 
            resources.ApplyResources(this.UseGroupLbl2, "UseGroupLbl2");
            this.UseGroupLbl2.Name = "UseGroupLbl2";
            // 
            // FixedStringRadio
            // 
            resources.ApplyResources(this.FixedStringRadio, "FixedStringRadio");
            this.FixedStringRadio.Name = "FixedStringRadio";
            this.FixedStringRadio.TabStop = true;
            this.FixedStringRadio.Tag = "4";
            this.PushToStackToolTip.SetToolTip(this.FixedStringRadio, resources.GetString("FixedStringRadio.ToolTip"));
            this.FixedStringRadio.UseVisualStyleBackColor = true;
            this.FixedStringRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // FixedStringTxt
            // 
            resources.ApplyResources(this.FixedStringTxt, "FixedStringTxt");
            this.FixedStringTxt.Name = "FixedStringTxt";
            this.PushToStackToolTip.SetToolTip(this.FixedStringTxt, resources.GetString("FixedStringTxt.ToolTip"));
            this.FixedStringTxt.TextChanged += new System.EventHandler(this.FixedStringTxt_TextChanged);
            // 
            // PushToStackPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.FixedStringTxt);
            this.Controls.Add(this.FixedStringRadio);
            this.Controls.Add(this.UseGroupLbl2);
            this.Controls.Add(this.RegexGroupTxt);
            this.Controls.Add(this.UseGroupLbl1);
            this.Controls.Add(this.TestRegexBtn);
            this.Controls.Add(this.IgnoreCaseChk);
            this.Controls.Add(this.RegexTxt);
            this.Controls.Add(this.RegexRadio);
            this.Controls.Add(this.RangeEndTxt);
            this.Controls.Add(this.BeginAndEndLbl);
            this.Controls.Add(this.RangeBeginTxt);
            this.Controls.Add(this.RangeRadio);
            this.Controls.Add(this.EntirePathRadio);
            this.Controls.Add(this.PushToStackLbl);
            this.Name = "PushToStackPipelineElementUserControl";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label PushToStackLbl;
        private System.Windows.Forms.RadioButton EntirePathRadio;
        private System.Windows.Forms.RadioButton RangeRadio;
        private System.Windows.Forms.TextBox RangeBeginTxt;
        private System.Windows.Forms.Label BeginAndEndLbl;
        private System.Windows.Forms.TextBox RangeEndTxt;
        private System.Windows.Forms.RadioButton RegexRadio;
        private System.Windows.Forms.TextBox RegexTxt;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.Button TestRegexBtn;
        private System.Windows.Forms.Label UseGroupLbl1;
        private System.Windows.Forms.TextBox RegexGroupTxt;
        private System.Windows.Forms.Label UseGroupLbl2;
        private System.Windows.Forms.RadioButton FixedStringRadio;
        private System.Windows.Forms.TextBox FixedStringTxt;
        private System.Windows.Forms.ToolTip PushToStackToolTip;
    }
}
