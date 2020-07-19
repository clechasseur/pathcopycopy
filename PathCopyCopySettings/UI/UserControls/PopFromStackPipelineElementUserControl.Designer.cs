namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class PopFromStackPipelineElementUserControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PopFromStackPipelineElementUserControl));
            this.PopValueAndLbl = new System.Windows.Forms.Label();
            this.EntireRadio = new System.Windows.Forms.RadioButton();
            this.RangeRadio = new System.Windows.Forms.RadioButton();
            this.RangeBeginTxt = new System.Windows.Forms.TextBox();
            this.BeginAndEndLbl = new System.Windows.Forms.Label();
            this.RangeEndTxt = new System.Windows.Forms.TextBox();
            this.RegexRadio = new System.Windows.Forms.RadioButton();
            this.RegexTxt = new System.Windows.Forms.TextBox();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.TestRegexBtn = new System.Windows.Forms.Button();
            this.StartRadio = new System.Windows.Forms.RadioButton();
            this.EndRadio = new System.Windows.Forms.RadioButton();
            this.NowhereRadio = new System.Windows.Forms.RadioButton();
            this.PopFromStackToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // PopValueAndLbl
            // 
            resources.ApplyResources(this.PopValueAndLbl, "PopValueAndLbl");
            this.PopValueAndLbl.Name = "PopValueAndLbl";
            this.PopFromStackToolTip.SetToolTip(this.PopValueAndLbl, resources.GetString("PopValueAndLbl.ToolTip"));
            // 
            // EntireRadio
            // 
            resources.ApplyResources(this.EntireRadio, "EntireRadio");
            this.EntireRadio.Name = "EntireRadio";
            this.EntireRadio.TabStop = true;
            this.EntireRadio.Tag = "1";
            this.PopFromStackToolTip.SetToolTip(this.EntireRadio, resources.GetString("EntireRadio.ToolTip"));
            this.EntireRadio.UseVisualStyleBackColor = true;
            this.EntireRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RangeRadio
            // 
            resources.ApplyResources(this.RangeRadio, "RangeRadio");
            this.RangeRadio.Name = "RangeRadio";
            this.RangeRadio.TabStop = true;
            this.RangeRadio.Tag = "2";
            this.PopFromStackToolTip.SetToolTip(this.RangeRadio, resources.GetString("RangeRadio.ToolTip"));
            this.RangeRadio.UseVisualStyleBackColor = true;
            this.RangeRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RangeBeginTxt
            // 
            resources.ApplyResources(this.RangeBeginTxt, "RangeBeginTxt");
            this.RangeBeginTxt.Name = "RangeBeginTxt";
            this.PopFromStackToolTip.SetToolTip(this.RangeBeginTxt, resources.GetString("RangeBeginTxt.ToolTip"));
            this.RangeBeginTxt.TextChanged += new System.EventHandler(this.RangeBeginTxt_TextChanged);
            this.RangeBeginTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // BeginAndEndLbl
            // 
            resources.ApplyResources(this.BeginAndEndLbl, "BeginAndEndLbl");
            this.BeginAndEndLbl.Name = "BeginAndEndLbl";
            this.PopFromStackToolTip.SetToolTip(this.BeginAndEndLbl, resources.GetString("BeginAndEndLbl.ToolTip"));
            // 
            // RangeEndTxt
            // 
            resources.ApplyResources(this.RangeEndTxt, "RangeEndTxt");
            this.RangeEndTxt.Name = "RangeEndTxt";
            this.PopFromStackToolTip.SetToolTip(this.RangeEndTxt, resources.GetString("RangeEndTxt.ToolTip"));
            this.RangeEndTxt.TextChanged += new System.EventHandler(this.RangeEndTxt_TextChanged);
            this.RangeEndTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // RegexRadio
            // 
            resources.ApplyResources(this.RegexRadio, "RegexRadio");
            this.RegexRadio.Name = "RegexRadio";
            this.RegexRadio.TabStop = true;
            this.RegexRadio.Tag = "3";
            this.PopFromStackToolTip.SetToolTip(this.RegexRadio, resources.GetString("RegexRadio.ToolTip"));
            this.RegexRadio.UseVisualStyleBackColor = true;
            this.RegexRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RegexTxt
            // 
            resources.ApplyResources(this.RegexTxt, "RegexTxt");
            this.RegexTxt.Name = "RegexTxt";
            this.PopFromStackToolTip.SetToolTip(this.RegexTxt, resources.GetString("RegexTxt.ToolTip"));
            this.RegexTxt.TextChanged += new System.EventHandler(this.RegexTxt_TextChanged);
            // 
            // IgnoreCaseChk
            // 
            resources.ApplyResources(this.IgnoreCaseChk, "IgnoreCaseChk");
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.PopFromStackToolTip.SetToolTip(this.IgnoreCaseChk, resources.GetString("IgnoreCaseChk.ToolTip"));
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            this.IgnoreCaseChk.CheckedChanged += new System.EventHandler(this.IgnoreCaseChk_CheckedChanged);
            // 
            // TestRegexBtn
            // 
            resources.ApplyResources(this.TestRegexBtn, "TestRegexBtn");
            this.TestRegexBtn.Name = "TestRegexBtn";
            this.PopFromStackToolTip.SetToolTip(this.TestRegexBtn, resources.GetString("TestRegexBtn.ToolTip"));
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            this.TestRegexBtn.Click += new System.EventHandler(this.TestRegexBtn_Click);
            // 
            // StartRadio
            // 
            resources.ApplyResources(this.StartRadio, "StartRadio");
            this.StartRadio.Name = "StartRadio";
            this.StartRadio.TabStop = true;
            this.StartRadio.Tag = "4";
            this.PopFromStackToolTip.SetToolTip(this.StartRadio, resources.GetString("StartRadio.ToolTip"));
            this.StartRadio.UseVisualStyleBackColor = true;
            this.StartRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // EndRadio
            // 
            resources.ApplyResources(this.EndRadio, "EndRadio");
            this.EndRadio.Name = "EndRadio";
            this.EndRadio.TabStop = true;
            this.EndRadio.Tag = "5";
            this.PopFromStackToolTip.SetToolTip(this.EndRadio, resources.GetString("EndRadio.ToolTip"));
            this.EndRadio.UseVisualStyleBackColor = true;
            this.EndRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // NowhereRadio
            // 
            resources.ApplyResources(this.NowhereRadio, "NowhereRadio");
            this.NowhereRadio.Name = "NowhereRadio";
            this.NowhereRadio.TabStop = true;
            this.NowhereRadio.Tag = "6";
            this.PopFromStackToolTip.SetToolTip(this.NowhereRadio, resources.GetString("NowhereRadio.ToolTip"));
            this.NowhereRadio.UseVisualStyleBackColor = true;
            this.NowhereRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // PopFromStackPipelineElementUserControl
            // 
            resources.ApplyResources(this, "$this");
            this.Controls.Add(this.NowhereRadio);
            this.Controls.Add(this.EndRadio);
            this.Controls.Add(this.StartRadio);
            this.Controls.Add(this.TestRegexBtn);
            this.Controls.Add(this.IgnoreCaseChk);
            this.Controls.Add(this.RegexTxt);
            this.Controls.Add(this.RegexRadio);
            this.Controls.Add(this.RangeEndTxt);
            this.Controls.Add(this.BeginAndEndLbl);
            this.Controls.Add(this.RangeBeginTxt);
            this.Controls.Add(this.RangeRadio);
            this.Controls.Add(this.EntireRadio);
            this.Controls.Add(this.PopValueAndLbl);
            this.Name = "PopFromStackPipelineElementUserControl";
            this.PopFromStackToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label PopValueAndLbl;
        private System.Windows.Forms.RadioButton EntireRadio;
        private System.Windows.Forms.RadioButton RangeRadio;
        private System.Windows.Forms.TextBox RangeBeginTxt;
        private System.Windows.Forms.Label BeginAndEndLbl;
        private System.Windows.Forms.TextBox RangeEndTxt;
        private System.Windows.Forms.RadioButton RegexRadio;
        private System.Windows.Forms.TextBox RegexTxt;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.Button TestRegexBtn;
        private System.Windows.Forms.RadioButton StartRadio;
        private System.Windows.Forms.RadioButton EndRadio;
        private System.Windows.Forms.RadioButton NowhereRadio;
        private System.Windows.Forms.ToolTip PopFromStackToolTip;
    }
}
