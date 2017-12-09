namespace PathCopyCopy.Settings.UI.Forms
{
    partial class SoftwareUpdateForm
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
            this.SoftwareUpdateAvailLbl = new System.Windows.Forms.Label();
            this.SoftwareUpdateTitleLbl = new System.Windows.Forms.Label();
            this.SoftwareUpdateUrlLinkLbl = new System.Windows.Forms.LinkLabel();
            this.ReleaseNotesWebBrowser = new System.Windows.Forms.WebBrowser();
            this.ReleaseNotesLbl = new System.Windows.Forms.Label();
            this.CloseBtn = new System.Windows.Forms.Button();
            this.IgnoreUpdateBtn = new System.Windows.Forms.Button();
            this.SoftwareUpdatesTooltip = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // SoftwareUpdateAvailLbl
            // 
            this.SoftwareUpdateAvailLbl.AutoSize = true;
            this.SoftwareUpdateAvailLbl.Location = new System.Drawing.Point(12, 9);
            this.SoftwareUpdateAvailLbl.Name = "SoftwareUpdateAvailLbl";
            this.SoftwareUpdateAvailLbl.Size = new System.Drawing.Size(211, 13);
            this.SoftwareUpdateAvailLbl.TabIndex = 0;
            this.SoftwareUpdateAvailLbl.Text = "A new Path Copy Copy version is available:";
            // 
            // SoftwareUpdateTitleLbl
            // 
            this.SoftwareUpdateTitleLbl.AutoSize = true;
            this.SoftwareUpdateTitleLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SoftwareUpdateTitleLbl.Location = new System.Drawing.Point(229, 9);
            this.SoftwareUpdateTitleLbl.Name = "SoftwareUpdateTitleLbl";
            this.SoftwareUpdateTitleLbl.Size = new System.Drawing.Size(96, 13);
            this.SoftwareUpdateTitleLbl.TabIndex = 1;
            this.SoftwareUpdateTitleLbl.Text = "<Update name>";
            // 
            // SoftwareUpdateUrlLinkLbl
            // 
            this.SoftwareUpdateUrlLinkLbl.AutoSize = true;
            this.SoftwareUpdateUrlLinkLbl.Location = new System.Drawing.Point(12, 31);
            this.SoftwareUpdateUrlLinkLbl.Name = "SoftwareUpdateUrlLinkLbl";
            this.SoftwareUpdateUrlLinkLbl.Size = new System.Drawing.Size(291, 13);
            this.SoftwareUpdateUrlLinkLbl.TabIndex = 2;
            this.SoftwareUpdateUrlLinkLbl.TabStop = true;
            this.SoftwareUpdateUrlLinkLbl.Text = "Click here to visit the website and download the new update";
            this.SoftwareUpdateUrlLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.SoftwareUpdateUrlLinkLbl_LinkClicked);
            // 
            // ReleaseNotesWebBrowser
            // 
            this.ReleaseNotesWebBrowser.AllowNavigation = false;
            this.ReleaseNotesWebBrowser.AllowWebBrowserDrop = false;
            this.ReleaseNotesWebBrowser.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ReleaseNotesWebBrowser.IsWebBrowserContextMenuEnabled = false;
            this.ReleaseNotesWebBrowser.Location = new System.Drawing.Point(96, 56);
            this.ReleaseNotesWebBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.ReleaseNotesWebBrowser.Name = "ReleaseNotesWebBrowser";
            this.ReleaseNotesWebBrowser.ScriptErrorsSuppressed = true;
            this.ReleaseNotesWebBrowser.Size = new System.Drawing.Size(374, 283);
            this.ReleaseNotesWebBrowser.TabIndex = 4;
            this.ReleaseNotesWebBrowser.WebBrowserShortcutsEnabled = false;
            // 
            // ReleaseNotesLbl
            // 
            this.ReleaseNotesLbl.AutoSize = true;
            this.ReleaseNotesLbl.Location = new System.Drawing.Point(12, 56);
            this.ReleaseNotesLbl.Name = "ReleaseNotesLbl";
            this.ReleaseNotesLbl.Size = new System.Drawing.Size(78, 13);
            this.ReleaseNotesLbl.TabIndex = 3;
            this.ReleaseNotesLbl.Text = "Release notes:";
            // 
            // CloseBtn
            // 
            this.CloseBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CloseBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CloseBtn.Location = new System.Drawing.Point(395, 345);
            this.CloseBtn.Name = "CloseBtn";
            this.CloseBtn.Size = new System.Drawing.Size(75, 23);
            this.CloseBtn.TabIndex = 6;
            this.CloseBtn.Text = "&Close";
            this.CloseBtn.UseVisualStyleBackColor = true;
            // 
            // IgnoreUpdateBtn
            // 
            this.IgnoreUpdateBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.IgnoreUpdateBtn.Location = new System.Drawing.Point(273, 345);
            this.IgnoreUpdateBtn.Name = "IgnoreUpdateBtn";
            this.IgnoreUpdateBtn.Size = new System.Drawing.Size(116, 23);
            this.IgnoreUpdateBtn.TabIndex = 5;
            this.IgnoreUpdateBtn.Text = "&Ignore this update";
            this.IgnoreUpdateBtn.UseVisualStyleBackColor = true;
            this.IgnoreUpdateBtn.Click += new System.EventHandler(this.IgnoreUpdateBtn_Click);
            // 
            // SoftwareUpdateForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CloseBtn;
            this.ClientSize = new System.Drawing.Size(482, 380);
            this.Controls.Add(this.IgnoreUpdateBtn);
            this.Controls.Add(this.CloseBtn);
            this.Controls.Add(this.ReleaseNotesLbl);
            this.Controls.Add(this.ReleaseNotesWebBrowser);
            this.Controls.Add(this.SoftwareUpdateUrlLinkLbl);
            this.Controls.Add(this.SoftwareUpdateTitleLbl);
            this.Controls.Add(this.SoftwareUpdateAvailLbl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(498, 419);
            this.Name = "SoftwareUpdateForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Path Copy Copy software update available";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label SoftwareUpdateAvailLbl;
        private System.Windows.Forms.Label SoftwareUpdateTitleLbl;
        private System.Windows.Forms.LinkLabel SoftwareUpdateUrlLinkLbl;
        private System.Windows.Forms.WebBrowser ReleaseNotesWebBrowser;
        private System.Windows.Forms.Label ReleaseNotesLbl;
        private System.Windows.Forms.Button CloseBtn;
        private System.Windows.Forms.Button IgnoreUpdateBtn;
        private System.Windows.Forms.ToolTip SoftwareUpdatesTooltip;
    }
}