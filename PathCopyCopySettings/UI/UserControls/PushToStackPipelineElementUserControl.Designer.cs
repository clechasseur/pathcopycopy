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
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.RegexTxt = new System.Windows.Forms.TextBox();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.TestRegexBtn = new System.Windows.Forms.Button();
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
            this.EntirePathRadio.Text = "The &entire path";
            this.EntirePathRadio.UseVisualStyleBackColor = true;
            // 
            // RangeRadio
            // 
            this.RangeRadio.AutoSize = true;
            this.RangeRadio.Location = new System.Drawing.Point(0, 39);
            this.RangeRadio.Name = "RangeRadio";
            this.RangeRadio.Size = new System.Drawing.Size(216, 17);
            this.RangeRadio.TabIndex = 2;
            this.RangeRadio.TabStop = true;
            this.RangeRadio.Text = "Everything in the path between &positions";
            this.RangeRadio.UseVisualStyleBackColor = true;
            // 
            // RangeBeginTxt
            // 
            this.RangeBeginTxt.Location = new System.Drawing.Point(222, 38);
            this.RangeBeginTxt.Name = "RangeBeginTxt";
            this.RangeBeginTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeBeginTxt.TabIndex = 3;
            this.RangeBeginTxt.Text = "0";
            // 
            // BeginAndEndLbl
            // 
            this.BeginAndEndLbl.AutoSize = true;
            this.BeginAndEndLbl.Location = new System.Drawing.Point(255, 43);
            this.BeginAndEndLbl.Name = "BeginAndEndLbl";
            this.BeginAndEndLbl.Size = new System.Drawing.Size(25, 13);
            this.BeginAndEndLbl.TabIndex = 4;
            this.BeginAndEndLbl.Text = "and";
            // 
            // RangeEndTxt
            // 
            this.RangeEndTxt.Location = new System.Drawing.Point(286, 38);
            this.RangeEndTxt.Name = "RangeEndTxt";
            this.RangeEndTxt.Size = new System.Drawing.Size(27, 20);
            this.RangeEndTxt.TabIndex = 5;
            this.RangeEndTxt.Text = "0";
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(0, 62);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(195, 17);
            this.radioButton1.TabIndex = 6;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "The first match of &regular expression";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // RegexTxt
            // 
            this.RegexTxt.Location = new System.Drawing.Point(201, 61);
            this.RegexTxt.Name = "RegexTxt";
            this.RegexTxt.Size = new System.Drawing.Size(169, 20);
            this.RegexTxt.TabIndex = 7;
            // 
            // IgnoreCaseChk
            // 
            this.IgnoreCaseChk.AutoSize = true;
            this.IgnoreCaseChk.Location = new System.Drawing.Point(201, 91);
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.IgnoreCaseChk.Size = new System.Drawing.Size(82, 17);
            this.IgnoreCaseChk.TabIndex = 8;
            this.IgnoreCaseChk.Text = "&Ignore case";
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            // 
            // TestRegexBtn
            // 
            this.TestRegexBtn.Location = new System.Drawing.Point(295, 87);
            this.TestRegexBtn.Name = "TestRegexBtn";
            this.TestRegexBtn.Size = new System.Drawing.Size(75, 23);
            this.TestRegexBtn.TabIndex = 9;
            this.TestRegexBtn.Text = "&Test...";
            this.TestRegexBtn.UseVisualStyleBackColor = true;
            // 
            // PushToStackPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Controls.Add(this.TestRegexBtn);
            this.Controls.Add(this.IgnoreCaseChk);
            this.Controls.Add(this.RegexTxt);
            this.Controls.Add(this.radioButton1);
            this.Controls.Add(this.RangeEndTxt);
            this.Controls.Add(this.BeginAndEndLbl);
            this.Controls.Add(this.RangeBeginTxt);
            this.Controls.Add(this.RangeRadio);
            this.Controls.Add(this.EntirePathRadio);
            this.Controls.Add(this.PushToStackLbl);
            this.Name = "PushToStackPipelineElementUserControl";
            this.Size = new System.Drawing.Size(370, 257);
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
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.TextBox RegexTxt;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.Button TestRegexBtn;
    }
}
