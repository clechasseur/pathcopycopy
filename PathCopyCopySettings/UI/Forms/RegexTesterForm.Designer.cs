namespace PathCopyCopy.Settings.UI.Forms
{
    partial class RegexTesterForm
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
            this.ParamsGroupBox = new System.Windows.Forms.GroupBox();
            this.IgnoreCaseChk = new System.Windows.Forms.CheckBox();
            this.ReplacementTxt = new System.Windows.Forms.TextBox();
            this.RegexTxt = new System.Windows.Forms.TextBox();
            this.ReplacementLbl = new System.Windows.Forms.Label();
            this.RegexLbl = new System.Windows.Forms.Label();
            this.ExecutionGroupBox = new System.Windows.Forms.GroupBox();
            this.InvalidNoticeLbl = new System.Windows.Forms.Label();
            this.TestBtn = new System.Windows.Forms.Button();
            this.ResultLbl = new System.Windows.Forms.Label();
            this.ResultTxt = new System.Windows.Forms.TextBox();
            this.SampleTxt = new System.Windows.Forms.TextBox();
            this.SampleLbl = new System.Windows.Forms.Label();
            this.ExecutionExplanationLbl = new System.Windows.Forms.Label();
            this.RegexSyntaxHelpLbl1 = new System.Windows.Forms.Label();
            this.RegexSyntaxHelpLbl2 = new System.Windows.Forms.Label();
            this.RegexSyntaxHelpLinkLbl1 = new System.Windows.Forms.LinkLabel();
            this.CancelBtn = new System.Windows.Forms.Button();
            this.OKBtn = new System.Windows.Forms.Button();
            this.RegexSyntaxHelpLinkLbl2 = new System.Windows.Forms.LinkLabel();
            this.RegexToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.ParamsGroupBox.SuspendLayout();
            this.ExecutionGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // ParamsGroupBox
            // 
            this.ParamsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ParamsGroupBox.Controls.Add(this.IgnoreCaseChk);
            this.ParamsGroupBox.Controls.Add(this.ReplacementTxt);
            this.ParamsGroupBox.Controls.Add(this.RegexTxt);
            this.ParamsGroupBox.Controls.Add(this.ReplacementLbl);
            this.ParamsGroupBox.Controls.Add(this.RegexLbl);
            this.ParamsGroupBox.Location = new System.Drawing.Point(12, 12);
            this.ParamsGroupBox.Name = "ParamsGroupBox";
            this.ParamsGroupBox.Size = new System.Drawing.Size(422, 96);
            this.ParamsGroupBox.TabIndex = 0;
            this.ParamsGroupBox.TabStop = false;
            this.ParamsGroupBox.Text = "Parameters";
            // 
            // IgnoreCaseChk
            // 
            this.IgnoreCaseChk.AutoSize = true;
            this.IgnoreCaseChk.Location = new System.Drawing.Point(9, 71);
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.IgnoreCaseChk.Size = new System.Drawing.Size(82, 17);
            this.IgnoreCaseChk.TabIndex = 4;
            this.IgnoreCaseChk.Text = "&Ignore case";
            this.RegexToolTip.SetToolTip(this.IgnoreCaseChk, "Whether to ignore case when performing find/replace operations");
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            // 
            // ReplacementTxt
            // 
            this.ReplacementTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ReplacementTxt.Location = new System.Drawing.Point(138, 45);
            this.ReplacementTxt.Name = "ReplacementTxt";
            this.ReplacementTxt.Size = new System.Drawing.Size(272, 20);
            this.ReplacementTxt.TabIndex = 3;
            this.RegexToolTip.SetToolTip(this.ReplacementTxt, "The replacement expression used to perform find/replace operations");
            // 
            // RegexTxt
            // 
            this.RegexTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RegexTxt.Location = new System.Drawing.Point(112, 19);
            this.RegexTxt.Name = "RegexTxt";
            this.RegexTxt.Size = new System.Drawing.Size(298, 20);
            this.RegexTxt.TabIndex = 1;
            this.RegexToolTip.SetToolTip(this.RegexTxt, "The regular expression used to perform find/replace operations");
            // 
            // ReplacementLbl
            // 
            this.ReplacementLbl.AutoSize = true;
            this.ReplacementLbl.Location = new System.Drawing.Point(6, 48);
            this.ReplacementLbl.Name = "ReplacementLbl";
            this.ReplacementLbl.Size = new System.Drawing.Size(126, 13);
            this.ReplacementLbl.TabIndex = 2;
            this.ReplacementLbl.Text = "&Replacement expression:";
            // 
            // RegexLbl
            // 
            this.RegexLbl.AutoSize = true;
            this.RegexLbl.Location = new System.Drawing.Point(6, 22);
            this.RegexLbl.Name = "RegexLbl";
            this.RegexLbl.Size = new System.Drawing.Size(100, 13);
            this.RegexLbl.TabIndex = 0;
            this.RegexLbl.Text = "Regular e&xpression:";
            // 
            // ExecutionGroupBox
            // 
            this.ExecutionGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ExecutionGroupBox.Controls.Add(this.InvalidNoticeLbl);
            this.ExecutionGroupBox.Controls.Add(this.TestBtn);
            this.ExecutionGroupBox.Controls.Add(this.ResultLbl);
            this.ExecutionGroupBox.Controls.Add(this.ResultTxt);
            this.ExecutionGroupBox.Controls.Add(this.SampleTxt);
            this.ExecutionGroupBox.Controls.Add(this.SampleLbl);
            this.ExecutionGroupBox.Controls.Add(this.ExecutionExplanationLbl);
            this.ExecutionGroupBox.Location = new System.Drawing.Point(12, 114);
            this.ExecutionGroupBox.Name = "ExecutionGroupBox";
            this.ExecutionGroupBox.Size = new System.Drawing.Size(422, 129);
            this.ExecutionGroupBox.TabIndex = 1;
            this.ExecutionGroupBox.TabStop = false;
            this.ExecutionGroupBox.Text = "Execution";
            // 
            // InvalidNoticeLbl
            // 
            this.InvalidNoticeLbl.AutoSize = true;
            this.InvalidNoticeLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InvalidNoticeLbl.ForeColor = System.Drawing.Color.Red;
            this.InvalidNoticeLbl.Location = new System.Drawing.Point(6, 98);
            this.InvalidNoticeLbl.Name = "InvalidNoticeLbl";
            this.InvalidNoticeLbl.Size = new System.Drawing.Size(152, 13);
            this.InvalidNoticeLbl.TabIndex = 5;
            this.InvalidNoticeLbl.Text = "Invalid regular expression";
            this.InvalidNoticeLbl.Visible = false;
            // 
            // TestBtn
            // 
            this.TestBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.TestBtn.Location = new System.Drawing.Point(335, 93);
            this.TestBtn.Name = "TestBtn";
            this.TestBtn.Size = new System.Drawing.Size(75, 23);
            this.TestBtn.TabIndex = 6;
            this.TestBtn.Text = "&Test";
            this.RegexToolTip.SetToolTip(this.TestBtn, "Execute a find/replace operation with the provided regular and replacement expres" +
        "sions, applying them to the provided sample string");
            this.TestBtn.UseVisualStyleBackColor = true;
            this.TestBtn.Click += new System.EventHandler(this.TestBtn_Click);
            // 
            // ResultLbl
            // 
            this.ResultLbl.AutoSize = true;
            this.ResultLbl.Location = new System.Drawing.Point(6, 70);
            this.ResultLbl.Name = "ResultLbl";
            this.ResultLbl.Size = new System.Drawing.Size(40, 13);
            this.ResultLbl.TabIndex = 3;
            this.ResultLbl.Text = "Result:";
            // 
            // ResultTxt
            // 
            this.ResultTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ResultTxt.Location = new System.Drawing.Point(52, 67);
            this.ResultTxt.Name = "ResultTxt";
            this.ResultTxt.ReadOnly = true;
            this.ResultTxt.Size = new System.Drawing.Size(358, 20);
            this.ResultTxt.TabIndex = 4;
            this.RegexToolTip.SetToolTip(this.ResultTxt, "Result of the find/replace operation");
            // 
            // SampleTxt
            // 
            this.SampleTxt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SampleTxt.Location = new System.Drawing.Point(57, 41);
            this.SampleTxt.Name = "SampleTxt";
            this.SampleTxt.Size = new System.Drawing.Size(353, 20);
            this.SampleTxt.TabIndex = 2;
            this.SampleTxt.Text = "C:\\Program Files\\Internet Explorer\\iexplore.exe";
            this.RegexToolTip.SetToolTip(this.SampleTxt, "Sample text on which the regular and replacement expressions will be applied");
            // 
            // SampleLbl
            // 
            this.SampleLbl.AutoSize = true;
            this.SampleLbl.Location = new System.Drawing.Point(6, 44);
            this.SampleLbl.Name = "SampleLbl";
            this.SampleLbl.Size = new System.Drawing.Size(45, 13);
            this.SampleLbl.TabIndex = 1;
            this.SampleLbl.Text = "&Sample:";
            // 
            // ExecutionExplanationLbl
            // 
            this.ExecutionExplanationLbl.AutoSize = true;
            this.ExecutionExplanationLbl.Location = new System.Drawing.Point(6, 22);
            this.ExecutionExplanationLbl.Name = "ExecutionExplanationLbl";
            this.ExecutionExplanationLbl.Size = new System.Drawing.Size(368, 13);
            this.ExecutionExplanationLbl.TabIndex = 0;
            this.ExecutionExplanationLbl.Text = "To test your regular expression, enter a sample string below and click \"Test\".";
            // 
            // RegexSyntaxHelpLbl1
            // 
            this.RegexSyntaxHelpLbl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.RegexSyntaxHelpLbl1.AutoSize = true;
            this.RegexSyntaxHelpLbl1.Location = new System.Drawing.Point(18, 255);
            this.RegexSyntaxHelpLbl1.Name = "RegexSyntaxHelpLbl1";
            this.RegexSyntaxHelpLbl1.Size = new System.Drawing.Size(365, 13);
            this.RegexSyntaxHelpLbl1.TabIndex = 2;
            this.RegexSyntaxHelpLbl1.Text = "Path Copy Copy uses the JavaScript/ECMAScript regular expression syntax.";
            // 
            // RegexSyntaxHelpLbl2
            // 
            this.RegexSyntaxHelpLbl2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.RegexSyntaxHelpLbl2.AutoSize = true;
            this.RegexSyntaxHelpLbl2.Location = new System.Drawing.Point(18, 268);
            this.RegexSyntaxHelpLbl2.Name = "RegexSyntaxHelpLbl2";
            this.RegexSyntaxHelpLbl2.Size = new System.Drawing.Size(267, 13);
            this.RegexSyntaxHelpLbl2.TabIndex = 3;
            this.RegexSyntaxHelpLbl2.Text = "Good regular expression references can be found here:";
            // 
            // RegexSyntaxHelpLinkLbl1
            // 
            this.RegexSyntaxHelpLinkLbl1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.RegexSyntaxHelpLinkLbl1.AutoSize = true;
            this.RegexSyntaxHelpLinkLbl1.Location = new System.Drawing.Point(18, 281);
            this.RegexSyntaxHelpLinkLbl1.Name = "RegexSyntaxHelpLinkLbl1";
            this.RegexSyntaxHelpLinkLbl1.Size = new System.Drawing.Size(247, 13);
            this.RegexSyntaxHelpLinkLbl1.TabIndex = 4;
            this.RegexSyntaxHelpLinkLbl1.TabStop = true;
            this.RegexSyntaxHelpLinkLbl1.Text = "http://www.regular-expressions.info/reference.html";
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLinkLbl1, "http://www.regular-expressions.info/reference.html");
            this.RegexSyntaxHelpLinkLbl1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.RegexSyntaxHelpLinkLbl_LinkClicked);
            // 
            // CancelBtn
            // 
            this.CancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Location = new System.Drawing.Point(359, 290);
            this.CancelBtn.Name = "CancelBtn";
            this.CancelBtn.Size = new System.Drawing.Size(75, 23);
            this.CancelBtn.TabIndex = 7;
            this.CancelBtn.Text = "Cancel";
            this.RegexToolTip.SetToolTip(this.CancelBtn, "Go back to the parent dialog without changing regular or replacement expression");
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            this.OKBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Location = new System.Drawing.Point(278, 290);
            this.OKBtn.Name = "OKBtn";
            this.OKBtn.Size = new System.Drawing.Size(75, 23);
            this.OKBtn.TabIndex = 6;
            this.OKBtn.Text = "OK";
            this.RegexToolTip.SetToolTip(this.OKBtn, "Copy changes to the regular and replacement expression back to the parent dialog " +
        "and close this window");
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // RegexSyntaxHelpLinkLbl2
            // 
            this.RegexSyntaxHelpLinkLbl2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.RegexSyntaxHelpLinkLbl2.AutoSize = true;
            this.RegexSyntaxHelpLinkLbl2.Location = new System.Drawing.Point(18, 295);
            this.RegexSyntaxHelpLinkLbl2.Name = "RegexSyntaxHelpLinkLbl2";
            this.RegexSyntaxHelpLinkLbl2.Size = new System.Drawing.Size(247, 13);
            this.RegexSyntaxHelpLinkLbl2.TabIndex = 5;
            this.RegexSyntaxHelpLinkLbl2.TabStop = true;
            this.RegexSyntaxHelpLinkLbl2.Text = "http://www.regular-expressions.info/javascript.html";
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLinkLbl2, "http://www.regular-expressions.info/javascript.html");
            this.RegexSyntaxHelpLinkLbl2.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.RegexSyntaxHelpLinkLbl_LinkClicked);
            // 
            // RegexTesterForm
            // 
            this.AcceptButton = this.OKBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
            this.ClientSize = new System.Drawing.Size(446, 325);
            this.Controls.Add(this.RegexSyntaxHelpLinkLbl2);
            this.Controls.Add(this.OKBtn);
            this.Controls.Add(this.CancelBtn);
            this.Controls.Add(this.RegexSyntaxHelpLinkLbl1);
            this.Controls.Add(this.RegexSyntaxHelpLbl2);
            this.Controls.Add(this.RegexSyntaxHelpLbl1);
            this.Controls.Add(this.ExecutionGroupBox);
            this.Controls.Add(this.ParamsGroupBox);
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(462, 364);
            this.Name = "RegexTesterForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Regular Expression Testing";
            this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.RegexTesterForm_HelpButtonClicked);
            this.ParamsGroupBox.ResumeLayout(false);
            this.ParamsGroupBox.PerformLayout();
            this.ExecutionGroupBox.ResumeLayout(false);
            this.ExecutionGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox ParamsGroupBox;
        private System.Windows.Forms.CheckBox IgnoreCaseChk;
        private System.Windows.Forms.TextBox ReplacementTxt;
        private System.Windows.Forms.TextBox RegexTxt;
        private System.Windows.Forms.Label ReplacementLbl;
        private System.Windows.Forms.Label RegexLbl;
        private System.Windows.Forms.GroupBox ExecutionGroupBox;
        private System.Windows.Forms.Label SampleLbl;
        private System.Windows.Forms.Label ExecutionExplanationLbl;
        private System.Windows.Forms.TextBox SampleTxt;
        private System.Windows.Forms.Label ResultLbl;
        private System.Windows.Forms.TextBox ResultTxt;
        private System.Windows.Forms.Button TestBtn;
        private System.Windows.Forms.Label InvalidNoticeLbl;
        private System.Windows.Forms.Label RegexSyntaxHelpLbl1;
        private System.Windows.Forms.Label RegexSyntaxHelpLbl2;
        private System.Windows.Forms.LinkLabel RegexSyntaxHelpLinkLbl1;
        private System.Windows.Forms.Button CancelBtn;
        private System.Windows.Forms.Button OKBtn;
        private System.Windows.Forms.LinkLabel RegexSyntaxHelpLinkLbl2;
        private System.Windows.Forms.ToolTip RegexToolTip;
    }
}