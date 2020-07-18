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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RegexTesterForm));
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
            resources.ApplyResources(this.ParamsGroupBox, "ParamsGroupBox");
            this.ParamsGroupBox.Controls.Add(this.IgnoreCaseChk);
            this.ParamsGroupBox.Controls.Add(this.ReplacementTxt);
            this.ParamsGroupBox.Controls.Add(this.RegexTxt);
            this.ParamsGroupBox.Controls.Add(this.ReplacementLbl);
            this.ParamsGroupBox.Controls.Add(this.RegexLbl);
            this.ParamsGroupBox.Name = "ParamsGroupBox";
            this.ParamsGroupBox.TabStop = false;
            this.RegexToolTip.SetToolTip(this.ParamsGroupBox, resources.GetString("ParamsGroupBox.ToolTip"));
            // 
            // IgnoreCaseChk
            // 
            resources.ApplyResources(this.IgnoreCaseChk, "IgnoreCaseChk");
            this.IgnoreCaseChk.Name = "IgnoreCaseChk";
            this.RegexToolTip.SetToolTip(this.IgnoreCaseChk, resources.GetString("IgnoreCaseChk.ToolTip"));
            this.IgnoreCaseChk.UseVisualStyleBackColor = true;
            // 
            // ReplacementTxt
            // 
            resources.ApplyResources(this.ReplacementTxt, "ReplacementTxt");
            this.ReplacementTxt.Name = "ReplacementTxt";
            this.RegexToolTip.SetToolTip(this.ReplacementTxt, resources.GetString("ReplacementTxt.ToolTip"));
            // 
            // RegexTxt
            // 
            resources.ApplyResources(this.RegexTxt, "RegexTxt");
            this.RegexTxt.Name = "RegexTxt";
            this.RegexToolTip.SetToolTip(this.RegexTxt, resources.GetString("RegexTxt.ToolTip"));
            // 
            // ReplacementLbl
            // 
            resources.ApplyResources(this.ReplacementLbl, "ReplacementLbl");
            this.ReplacementLbl.Name = "ReplacementLbl";
            this.RegexToolTip.SetToolTip(this.ReplacementLbl, resources.GetString("ReplacementLbl.ToolTip"));
            // 
            // RegexLbl
            // 
            resources.ApplyResources(this.RegexLbl, "RegexLbl");
            this.RegexLbl.Name = "RegexLbl";
            this.RegexToolTip.SetToolTip(this.RegexLbl, resources.GetString("RegexLbl.ToolTip"));
            // 
            // ExecutionGroupBox
            // 
            resources.ApplyResources(this.ExecutionGroupBox, "ExecutionGroupBox");
            this.ExecutionGroupBox.Controls.Add(this.InvalidNoticeLbl);
            this.ExecutionGroupBox.Controls.Add(this.TestBtn);
            this.ExecutionGroupBox.Controls.Add(this.ResultLbl);
            this.ExecutionGroupBox.Controls.Add(this.ResultTxt);
            this.ExecutionGroupBox.Controls.Add(this.SampleTxt);
            this.ExecutionGroupBox.Controls.Add(this.SampleLbl);
            this.ExecutionGroupBox.Controls.Add(this.ExecutionExplanationLbl);
            this.ExecutionGroupBox.Name = "ExecutionGroupBox";
            this.ExecutionGroupBox.TabStop = false;
            this.RegexToolTip.SetToolTip(this.ExecutionGroupBox, resources.GetString("ExecutionGroupBox.ToolTip"));
            // 
            // InvalidNoticeLbl
            // 
            resources.ApplyResources(this.InvalidNoticeLbl, "InvalidNoticeLbl");
            this.InvalidNoticeLbl.ForeColor = System.Drawing.Color.Red;
            this.InvalidNoticeLbl.Name = "InvalidNoticeLbl";
            this.RegexToolTip.SetToolTip(this.InvalidNoticeLbl, resources.GetString("InvalidNoticeLbl.ToolTip"));
            // 
            // TestBtn
            // 
            resources.ApplyResources(this.TestBtn, "TestBtn");
            this.TestBtn.Name = "TestBtn";
            this.RegexToolTip.SetToolTip(this.TestBtn, resources.GetString("TestBtn.ToolTip"));
            this.TestBtn.UseVisualStyleBackColor = true;
            this.TestBtn.Click += new System.EventHandler(this.TestBtn_Click);
            // 
            // ResultLbl
            // 
            resources.ApplyResources(this.ResultLbl, "ResultLbl");
            this.ResultLbl.Name = "ResultLbl";
            this.RegexToolTip.SetToolTip(this.ResultLbl, resources.GetString("ResultLbl.ToolTip"));
            // 
            // ResultTxt
            // 
            resources.ApplyResources(this.ResultTxt, "ResultTxt");
            this.ResultTxt.Name = "ResultTxt";
            this.ResultTxt.ReadOnly = true;
            this.RegexToolTip.SetToolTip(this.ResultTxt, resources.GetString("ResultTxt.ToolTip"));
            // 
            // SampleTxt
            // 
            resources.ApplyResources(this.SampleTxt, "SampleTxt");
            this.SampleTxt.Name = "SampleTxt";
            this.RegexToolTip.SetToolTip(this.SampleTxt, resources.GetString("SampleTxt.ToolTip"));
            // 
            // SampleLbl
            // 
            resources.ApplyResources(this.SampleLbl, "SampleLbl");
            this.SampleLbl.Name = "SampleLbl";
            this.RegexToolTip.SetToolTip(this.SampleLbl, resources.GetString("SampleLbl.ToolTip"));
            // 
            // ExecutionExplanationLbl
            // 
            resources.ApplyResources(this.ExecutionExplanationLbl, "ExecutionExplanationLbl");
            this.ExecutionExplanationLbl.Name = "ExecutionExplanationLbl";
            this.RegexToolTip.SetToolTip(this.ExecutionExplanationLbl, resources.GetString("ExecutionExplanationLbl.ToolTip"));
            // 
            // RegexSyntaxHelpLbl1
            // 
            resources.ApplyResources(this.RegexSyntaxHelpLbl1, "RegexSyntaxHelpLbl1");
            this.RegexSyntaxHelpLbl1.Name = "RegexSyntaxHelpLbl1";
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLbl1, resources.GetString("RegexSyntaxHelpLbl1.ToolTip"));
            // 
            // RegexSyntaxHelpLbl2
            // 
            resources.ApplyResources(this.RegexSyntaxHelpLbl2, "RegexSyntaxHelpLbl2");
            this.RegexSyntaxHelpLbl2.Name = "RegexSyntaxHelpLbl2";
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLbl2, resources.GetString("RegexSyntaxHelpLbl2.ToolTip"));
            // 
            // RegexSyntaxHelpLinkLbl1
            // 
            resources.ApplyResources(this.RegexSyntaxHelpLinkLbl1, "RegexSyntaxHelpLinkLbl1");
            this.RegexSyntaxHelpLinkLbl1.Name = "RegexSyntaxHelpLinkLbl1";
            this.RegexSyntaxHelpLinkLbl1.TabStop = true;
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLinkLbl1, resources.GetString("RegexSyntaxHelpLinkLbl1.ToolTip"));
            this.RegexSyntaxHelpLinkLbl1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.RegexSyntaxHelpLinkLbl_LinkClicked);
            // 
            // CancelBtn
            // 
            resources.ApplyResources(this.CancelBtn, "CancelBtn");
            this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelBtn.Name = "CancelBtn";
            this.RegexToolTip.SetToolTip(this.CancelBtn, resources.GetString("CancelBtn.ToolTip"));
            this.CancelBtn.UseVisualStyleBackColor = true;
            // 
            // OKBtn
            // 
            resources.ApplyResources(this.OKBtn, "OKBtn");
            this.OKBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OKBtn.Name = "OKBtn";
            this.RegexToolTip.SetToolTip(this.OKBtn, resources.GetString("OKBtn.ToolTip"));
            this.OKBtn.UseVisualStyleBackColor = true;
            // 
            // RegexSyntaxHelpLinkLbl2
            // 
            resources.ApplyResources(this.RegexSyntaxHelpLinkLbl2, "RegexSyntaxHelpLinkLbl2");
            this.RegexSyntaxHelpLinkLbl2.Name = "RegexSyntaxHelpLinkLbl2";
            this.RegexSyntaxHelpLinkLbl2.TabStop = true;
            this.RegexToolTip.SetToolTip(this.RegexSyntaxHelpLinkLbl2, resources.GetString("RegexSyntaxHelpLinkLbl2.ToolTip"));
            this.RegexSyntaxHelpLinkLbl2.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.RegexSyntaxHelpLinkLbl_LinkClicked);
            // 
            // RegexTesterForm
            // 
            this.AcceptButton = this.OKBtn;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelBtn;
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
            this.Name = "RegexTesterForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.RegexToolTip.SetToolTip(this, resources.GetString("$this.ToolTip"));
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