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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SoftwareUpdateForm));
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
            resources.ApplyResources(this.SoftwareUpdateAvailLbl, "SoftwareUpdateAvailLbl");
            this.SoftwareUpdateAvailLbl.Name = "SoftwareUpdateAvailLbl";
            this.SoftwareUpdatesTooltip.SetToolTip(this.SoftwareUpdateAvailLbl, resources.GetString("SoftwareUpdateAvailLbl.ToolTip"));
            // 
            // SoftwareUpdateTitleLbl
            // 
            resources.ApplyResources(this.SoftwareUpdateTitleLbl, "SoftwareUpdateTitleLbl");
            this.SoftwareUpdateTitleLbl.Name = "SoftwareUpdateTitleLbl";
            this.SoftwareUpdatesTooltip.SetToolTip(this.SoftwareUpdateTitleLbl, resources.GetString("SoftwareUpdateTitleLbl.ToolTip"));
            // 
            // SoftwareUpdateUrlLinkLbl
            // 
            resources.ApplyResources(this.SoftwareUpdateUrlLinkLbl, "SoftwareUpdateUrlLinkLbl");
            this.SoftwareUpdateUrlLinkLbl.Name = "SoftwareUpdateUrlLinkLbl";
            this.SoftwareUpdateUrlLinkLbl.TabStop = true;
            this.SoftwareUpdatesTooltip.SetToolTip(this.SoftwareUpdateUrlLinkLbl, resources.GetString("SoftwareUpdateUrlLinkLbl.ToolTip"));
            this.SoftwareUpdateUrlLinkLbl.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.SoftwareUpdateUrlLinkLbl_LinkClicked);
            // 
            // ReleaseNotesWebBrowser
            // 
            resources.ApplyResources(this.ReleaseNotesWebBrowser, "ReleaseNotesWebBrowser");
            this.ReleaseNotesWebBrowser.AllowNavigation = false;
            this.ReleaseNotesWebBrowser.AllowWebBrowserDrop = false;
            this.ReleaseNotesWebBrowser.IsWebBrowserContextMenuEnabled = false;
            this.ReleaseNotesWebBrowser.Name = "ReleaseNotesWebBrowser";
            this.ReleaseNotesWebBrowser.ScriptErrorsSuppressed = true;
            this.SoftwareUpdatesTooltip.SetToolTip(this.ReleaseNotesWebBrowser, resources.GetString("ReleaseNotesWebBrowser.ToolTip"));
            this.ReleaseNotesWebBrowser.WebBrowserShortcutsEnabled = false;
            // 
            // ReleaseNotesLbl
            // 
            resources.ApplyResources(this.ReleaseNotesLbl, "ReleaseNotesLbl");
            this.ReleaseNotesLbl.Name = "ReleaseNotesLbl";
            this.SoftwareUpdatesTooltip.SetToolTip(this.ReleaseNotesLbl, resources.GetString("ReleaseNotesLbl.ToolTip"));
            // 
            // CloseBtn
            // 
            resources.ApplyResources(this.CloseBtn, "CloseBtn");
            this.CloseBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CloseBtn.Name = "CloseBtn";
            this.SoftwareUpdatesTooltip.SetToolTip(this.CloseBtn, resources.GetString("CloseBtn.ToolTip"));
            this.CloseBtn.UseVisualStyleBackColor = true;
            // 
            // IgnoreUpdateBtn
            // 
            resources.ApplyResources(this.IgnoreUpdateBtn, "IgnoreUpdateBtn");
            this.IgnoreUpdateBtn.Name = "IgnoreUpdateBtn";
            this.SoftwareUpdatesTooltip.SetToolTip(this.IgnoreUpdateBtn, resources.GetString("IgnoreUpdateBtn.ToolTip"));
            this.IgnoreUpdateBtn.UseVisualStyleBackColor = true;
            this.IgnoreUpdateBtn.Click += new System.EventHandler(this.IgnoreUpdateBtn_Click);
            // 
            // SoftwareUpdateForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CloseBtn;
            this.Controls.Add(this.IgnoreUpdateBtn);
            this.Controls.Add(this.CloseBtn);
            this.Controls.Add(this.ReleaseNotesLbl);
            this.Controls.Add(this.ReleaseNotesWebBrowser);
            this.Controls.Add(this.SoftwareUpdateUrlLinkLbl);
            this.Controls.Add(this.SoftwareUpdateTitleLbl);
            this.Controls.Add(this.SoftwareUpdateAvailLbl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SoftwareUpdateForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.SoftwareUpdatesTooltip.SetToolTip(this, resources.GetString("$this.ToolTip"));
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