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
            this.SuspendLayout();
            // 
            // PushToStackLbl
            // 
            this.PushToStackLbl.AutoSize = true;
            this.PushToStackLbl.Location = new System.Drawing.Point(-3, 0);
            this.PushToStackLbl.Name = "PushToStackLbl";
            this.PushToStackLbl.Size = new System.Drawing.Size(155, 13);
            this.PushToStackLbl.TabIndex = 0;
            this.PushToStackLbl.Text = "Push the following to the stack:";
            // 
            // EntirePathRadio
            // 
            this.EntirePathRadio.AutoSize = true;
            this.EntirePathRadio.Location = new System.Drawing.Point(0, 16);
            this.EntirePathRadio.Name = "EntirePathRadio";
            this.EntirePathRadio.Size = new System.Drawing.Size(97, 17);
            this.EntirePathRadio.TabIndex = 1;
            this.EntirePathRadio.TabStop = true;
            this.EntirePathRadio.Tag = "1";
            this.EntirePathRadio.Text = "The &entire path";
            this.EntirePathRadio.UseVisualStyleBackColor = true;
            this.EntirePathRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RangeRadio
            // 
            this.RangeRadio.AutoSize = true;
            this.RangeRadio.Location = new System.Drawing.Point(0, 39);
            this.RangeRadio.Name = "RangeRadio";
            this.RangeRadio.Size = new System.Drawing.Size(216, 17);
            this.RangeRadio.TabIndex = 2;
            this.RangeRadio.TabStop = true;
            this.RangeRadio.Tag = "2";
            this.RangeRadio.Text = "Everything in the path between &positions";
            this.RangeRadio.UseVisualStyleBackColor = true;
            this.RangeRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RangeBeginTxt
            // 
            this.RangeBeginTxt.Location = new System.Drawing.Point(222, 38);
            this.RangeBeginTxt.MaxLength = 3;
            this.RangeBeginTxt.Name = "RangeBeginTxt";
            this.RangeBeginTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeBeginTxt.TabIndex = 5;
            this.RangeBeginTxt.Text = "0";
            this.RangeBeginTxt.TextChanged += new System.EventHandler(this.RangeBeginTxt_TextChanged);
            this.RangeBeginTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // BeginAndEndLbl
            // 
            this.BeginAndEndLbl.AutoSize = true;
            this.BeginAndEndLbl.Location = new System.Drawing.Point(255, 43);
            this.BeginAndEndLbl.Name = "BeginAndEndLbl";
            this.BeginAndEndLbl.Size = new System.Drawing.Size(25, 13);
            this.BeginAndEndLbl.TabIndex = 6;
            this.BeginAndEndLbl.Text = "and";
            // 
            // RangeEndTxt
            // 
            this.RangeEndTxt.Location = new System.Drawing.Point(286, 38);
            this.RangeEndTxt.MaxLength = 3;
            this.RangeEndTxt.Name = "RangeEndTxt";
            this.RangeEndTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeEndTxt.TabIndex = 7;
            this.RangeEndTxt.Text = "0";
            this.RangeEndTxt.TextChanged += new System.EventHandler(this.RangeEndTxt_TextChanged);
            this.RangeEndTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // RegexRadio
            // 
            this.RegexRadio.AutoSize = true;
            this.RegexRadio.Location = new System.Drawing.Point(0, 62);
            this.RegexRadio.Name = "RegexRadio";
            this.RegexRadio.Size = new System.Drawing.Size(195, 17);
            this.RegexRadio.TabIndex = 3;
            this.RegexRadio.TabStop = true;
            this.RegexRadio.Tag = "3";
            this.RegexRadio.Text = "The first match of &regular expression";
            this.RegexRadio.UseVisualStyleBackColor = true;
            this.RegexRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // RegexTxt
            // 
            this.RegexTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RegexTxt.Location = new System.Drawing.Point(201, 61);
            this.RegexTxt.Name = "RegexTxt";
            this.RegexTxt.Size = new System.Drawing.Size(169, 20);
            this.RegexTxt.TabIndex = 8;
            this.RegexTxt.TextChanged += new System.EventHandler(this.RegexTxt_TextChanged);
            // 
            // IgnoreCaseChk
            // 
            this.IgnoreCaseChk.AutoSize = true;
            this.IgnoreCaseChk.Location = new System.Drawing.Point(201, 91);
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.IgnoreCaseChk.Size = new System.Drawing.Size(82, 17);
            this.IgnoreCaseChk.TabIndex = 12;
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
            this.TestRegexBtn.TabIndex = 13;
            this.TestRegexBtn.Text = "&Test...";
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            this.TestRegexBtn.Click += new System.EventHandler(this.TestRegexBtn_Click);
            // 
            // UseGroupLbl1
            // 
            this.UseGroupLbl1.AutoSize = true;
            this.UseGroupLbl1.Location = new System.Drawing.Point(19, 92);
            this.UseGroupLbl1.Name = "UseGroupLbl1";
            this.UseGroupLbl1.Size = new System.Drawing.Size(59, 13);
            this.UseGroupLbl1.TabIndex = 9;
            this.UseGroupLbl1.Text = "(Use group";
            // 
            // RegexGroupTxt
            // 
            this.RegexGroupTxt.Location = new System.Drawing.Point(84, 89);
            this.RegexGroupTxt.MaxLength = 3;
            this.RegexGroupTxt.Name = "RegexGroupTxt";
            this.RegexGroupTxt.Size = new System.Drawing.Size(27, 20);
            this.RegexGroupTxt.TabIndex = 10;
            this.RegexGroupTxt.Text = "0";
            this.RegexGroupTxt.TextChanged += new System.EventHandler(this.RegexGroupTxt_TextChanged);
            this.RegexGroupTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericTxt_KeyPress);
            // 
            // UseGroupLbl2
            // 
            this.UseGroupLbl2.AutoSize = true;
            this.UseGroupLbl2.Location = new System.Drawing.Point(117, 92);
            this.UseGroupLbl2.Name = "UseGroupLbl2";
            this.UseGroupLbl2.Size = new System.Drawing.Size(10, 13);
            this.UseGroupLbl2.TabIndex = 11;
            this.UseGroupLbl2.Text = ")";
            // 
            // FixedStringRadio
            // 
            this.FixedStringRadio.AutoSize = true;
            this.FixedStringRadio.Location = new System.Drawing.Point(0, 115);
            this.FixedStringRadio.Name = "FixedStringRadio";
            this.FixedStringRadio.Size = new System.Drawing.Size(73, 17);
            this.FixedStringRadio.TabIndex = 4;
            this.FixedStringRadio.TabStop = true;
            this.FixedStringRadio.Tag = "4";
            this.FixedStringRadio.Text = "The value";
            this.FixedStringRadio.UseVisualStyleBackColor = true;
            this.FixedStringRadio.CheckedChanged += new System.EventHandler(this.MethodRadio_CheckedChanged);
            // 
            // FixedStringTxt
            // 
            this.FixedStringTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FixedStringTxt.Location = new System.Drawing.Point(79, 114);
            this.FixedStringTxt.Name = "FixedStringTxt";
            this.FixedStringTxt.Size = new System.Drawing.Size(291, 20);
            this.FixedStringTxt.TabIndex = 14;
            this.FixedStringTxt.TextChanged += new System.EventHandler(this.FixedStringTxt_TextChanged);
            // 
            // PushToStackPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
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
            this.Size = new System.Drawing.Size(370, 138);
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
    }
}
