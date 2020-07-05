namespace PathCopyCopy.Settings.UI.UserControls
{
    partial class CopyNPathPartsPipelineElementUserControl
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
            this.NumPartsTxt = new System.Windows.Forms.TextBox();
            this.CopyTheNLbl = new System.Windows.Forms.Label();
            this.FirstLastCombo = new System.Windows.Forms.ComboBox();
            this.PathPartsLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // NumPartsTxt
            // 
            this.NumPartsTxt.Location = new System.Drawing.Point(100, 0);
            this.NumPartsTxt.MaxLength = 4;
            this.NumPartsTxt.Name = "NumPartsTxt";
            this.NumPartsTxt.Size = new System.Drawing.Size(38, 20);
            this.NumPartsTxt.TabIndex = 2;
            this.NumPartsTxt.Text = "0000";
            this.NumPartsTxt.TextChanged += new System.EventHandler(this.NumPartsTxt_TextChanged);
            this.NumPartsTxt.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumPartsTxt_KeyPress);
            // 
            // CopyTheNLbl
            // 
            this.CopyTheNLbl.AutoSize = true;
            this.CopyTheNLbl.Location = new System.Drawing.Point(-3, 3);
            this.CopyTheNLbl.Name = "CopyTheNLbl";
            this.CopyTheNLbl.Size = new System.Drawing.Size(49, 13);
            this.CopyTheNLbl.TabIndex = 0;
            this.CopyTheNLbl.Text = "&Copy the";
            // 
            // FirstLastCombo
            // 
            this.FirstLastCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.FirstLastCombo.FormattingEnabled = true;
            this.FirstLastCombo.Items.AddRange(new object[] {
            "first",
            "last"});
            this.FirstLastCombo.Location = new System.Drawing.Point(52, 0);
            this.FirstLastCombo.Name = "FirstLastCombo";
            this.FirstLastCombo.Size = new System.Drawing.Size(42, 21);
            this.FirstLastCombo.TabIndex = 1;
            this.FirstLastCombo.SelectedIndexChanged += new System.EventHandler(this.FirstLastCombo_SelectedIndexChanged);
            // 
            // PathPartsLbl
            // 
            this.PathPartsLbl.AutoSize = true;
            this.PathPartsLbl.Location = new System.Drawing.Point(144, 3);
            this.PathPartsLbl.Name = "PathPartsLbl";
            this.PathPartsLbl.Size = new System.Drawing.Size(90, 13);
            this.PathPartsLbl.TabIndex = 3;
            this.PathPartsLbl.Text = "part(s) of the path";
            // 
            // CopyNPathPartsPipelineElementUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Controls.Add(this.PathPartsLbl);
            this.Controls.Add(this.FirstLastCombo);
            this.Controls.Add(this.CopyTheNLbl);
            this.Controls.Add(this.NumPartsTxt);
            this.Name = "CopyNPathPartsPipelineElementUserControl";
            this.Size = new System.Drawing.Size(242, 23);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox NumPartsTxt;
        private System.Windows.Forms.Label CopyTheNLbl;
        private System.Windows.Forms.ComboBox FirstLastCombo;
        private System.Windows.Forms.Label PathPartsLbl;
    }
}
