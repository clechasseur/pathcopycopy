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
            this.SuspendLayout();
            // 
            // PopValueAndLbl
            // 
            this.PopValueAndLbl.AutoSize = true;
            this.PopValueAndLbl.Location = new System.Drawing.Point(-3, 0);
            this.PopValueAndLbl.Name = "PopValueAndLbl";
            this.PopValueAndLbl.Size = new System.Drawing.Size(158, 13);
            this.PopValueAndLbl.TabIndex = 0;
            this.PopValueAndLbl.Text = "Pop a value from the stack and:";
            // 
            // EntireRadio
            // 
            this.EntireRadio.AutoSize = true;
            this.EntireRadio.Location = new System.Drawing.Point(0, 16);
            this.EntireRadio.Name = "EntireRadio";
            this.EntireRadio.Size = new System.Drawing.Size(184, 17);
            this.EntireRadio.TabIndex = 1;
            this.EntireRadio.TabStop = true;
            this.EntireRadio.Tag = "1";
            this.EntireRadio.Text = "Replace the the &entire path with it";
            this.EntireRadio.UseVisualStyleBackColor = true;
            this.EntireRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RangeRadio
            // 
            this.RangeRadio.AutoSize = true;
            this.RangeRadio.Location = new System.Drawing.Point(0, 39);
            this.RangeRadio.Name = "RangeRadio";
            this.RangeRadio.Size = new System.Drawing.Size(258, 17);
            this.RangeRadio.TabIndex = 2;
            this.RangeRadio.TabStop = true;
            this.RangeRadio.Tag = "2";
            this.RangeRadio.Text = "Replace everything in the path between &positions";
            this.RangeRadio.UseVisualStyleBackColor = true;
            this.RangeRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RangeBeginTxt
            // 
            this.RangeBeginTxt.Location = new System.Drawing.Point(264, 38);
            this.RangeBeginTxt.MaxLength = 3;
            this.RangeBeginTxt.Name = "RangeBeginTxt";
            this.RangeBeginTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeBeginTxt.TabIndex = 7;
            this.RangeBeginTxt.Text = "0";
            this.RangeBeginTxt.TextChanged += new System.EventHandler(this.RangeBeginTxt_TextChanged);
            this.RangeBeginTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // BeginAndEndLbl
            // 
            this.BeginAndEndLbl.AutoSize = true;
            this.BeginAndEndLbl.Location = new System.Drawing.Point(297, 41);
            this.BeginAndEndLbl.Name = "BeginAndEndLbl";
            this.BeginAndEndLbl.Size = new System.Drawing.Size(25, 13);
            this.BeginAndEndLbl.TabIndex = 8;
            this.BeginAndEndLbl.Text = "and";
            // 
            // RangeEndTxt
            // 
            this.RangeEndTxt.Location = new System.Drawing.Point(328, 38);
            this.RangeEndTxt.MaxLength = 3;
            this.RangeEndTxt.Name = "RangeEndTxt";
            this.RangeEndTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeEndTxt.TabIndex = 9;
            this.RangeEndTxt.Text = "0";
            this.RangeEndTxt.TextChanged += new System.EventHandler(this.RangeEndTxt_TextChanged);
            this.RangeEndTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // RegexRadio
            // 
            this.RegexRadio.AutoSize = true;
            this.RegexRadio.Location = new System.Drawing.Point(0, 62);
            this.RegexRadio.Name = "RegexRadio";
            this.RegexRadio.Size = new System.Drawing.Size(234, 17);
            this.RegexRadio.TabIndex = 3;
            this.RegexRadio.TabStop = true;
            this.RegexRadio.Tag = "3";
            this.RegexRadio.Text = "Replace the first match of &regular expression";
            this.RegexRadio.UseVisualStyleBackColor = true;
            this.RegexRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // RegexTxt
            // 
            this.RegexTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RegexTxt.Location = new System.Drawing.Point(240, 61);
            this.RegexTxt.Name = "RegexTxt";
            this.RegexTxt.Size = new System.Drawing.Size(130, 20);
            this.RegexTxt.TabIndex = 10;
            this.RegexTxt.TextChanged += new System.EventHandler(this.RegexTxt_TextChanged);
            // 
            // IgnoreCaseChk
            // 
            this.IgnoreCaseChk.AutoSize = true;
            this.IgnoreCaseChk.Location = new System.Drawing.Point(207, 91);
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.IgnoreCaseChk.Size = new System.Drawing.Size(82, 17);
            this.IgnoreCaseChk.TabIndex = 11;
            this.IgnoreCaseChk.Text = "&Ignore case";
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            this.IgnoreCaseChk.CheckedChanged += new System.EventHandler(this.IgnoreCaseChk_CheckedChanged);
            // 
            // TestRegexBtn
            // 
            this.TestRegexBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.TestRegexBtn.Location = new System.Drawing.Point(295, 87);
            this.TestRegexBtn.Name = "TestRegexBtn";
            this.TestRegexBtn.Size = new System.Drawing.Size(75, 23);
            this.TestRegexBtn.TabIndex = 12;
            this.TestRegexBtn.Text = "&Test...";
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            this.TestRegexBtn.Click += new System.EventHandler(this.TestRegexBtn_Click);
            // 
            // StartRadio
            // 
            this.StartRadio.AutoSize = true;
            this.StartRadio.Location = new System.Drawing.Point(0, 114);
            this.StartRadio.Name = "StartRadio";
            this.StartRadio.Size = new System.Drawing.Size(231, 17);
            this.StartRadio.TabIndex = 4;
            this.StartRadio.TabStop = true;
            this.StartRadio.Tag = "4";
            this.StartRadio.Text = "Insert the value at the &beginning of the path";
            this.StartRadio.UseVisualStyleBackColor = true;
            this.StartRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // EndRadio
            // 
            this.EndRadio.AutoSize = true;
            this.EndRadio.Location = new System.Drawing.Point(0, 137);
            this.EndRadio.Name = "EndRadio";
            this.EndRadio.Size = new System.Drawing.Size(203, 17);
            this.EndRadio.TabIndex = 5;
            this.EndRadio.TabStop = true;
            this.EndRadio.Tag = "5";
            this.EndRadio.Text = "Insert the value at the e&nd of the path";
            this.EndRadio.UseVisualStyleBackColor = true;
            this.EndRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // NowhereRadio
            // 
            this.NowhereRadio.AutoSize = true;
            this.NowhereRadio.Location = new System.Drawing.Point(0, 160);
            this.NowhereRadio.Name = "NowhereRadio";
            this.NowhereRadio.Size = new System.Drawing.Size(166, 17);
            this.NowhereRadio.TabIndex = 6;
            this.NowhereRadio.TabStop = true;
            this.NowhereRadio.Tag = "6";
            this.NowhereRadio.Text = "Simply drop it &without storing it";
            this.NowhereRadio.UseVisualStyleBackColor = true;
            this.NowhereRadio.CheckedChanged += new System.EventHandler(this.LocationRadio_CheckedChanged);
            // 
            // PopFromStackPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
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
            this.Size = new System.Drawing.Size(370, 180);
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
    }
}
