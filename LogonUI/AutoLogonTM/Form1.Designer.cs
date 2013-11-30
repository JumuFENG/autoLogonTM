namespace AutoLogonTM
{
    partial class AutoLogonTM
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.pnl_top = new System.Windows.Forms.Panel();
            this.cmbboxPath = new System.Windows.Forms.ComboBox();
            this.btnLogon = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.chkBxShowChar = new System.Windows.Forms.CheckBox();
            this.txBxPwd = new System.Windows.Forms.TextBox();
            this.txBxAccount = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnBrow = new System.Windows.Forms.Button();
            this.dGVAccounts = new System.Windows.Forms.DataGridView();
            this.ClmnFirst = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.ClmnAccount = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.pnl_top.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dGVAccounts)).BeginInit();
            this.SuspendLayout();
            // 
            // pnl_top
            // 
            this.pnl_top.Controls.Add(this.cmbboxPath);
            this.pnl_top.Controls.Add(this.btnLogon);
            this.pnl_top.Controls.Add(this.btnAdd);
            this.pnl_top.Controls.Add(this.chkBxShowChar);
            this.pnl_top.Controls.Add(this.txBxPwd);
            this.pnl_top.Controls.Add(this.txBxAccount);
            this.pnl_top.Controls.Add(this.label3);
            this.pnl_top.Controls.Add(this.label2);
            this.pnl_top.Controls.Add(this.label1);
            this.pnl_top.Controls.Add(this.btnBrow);
            this.pnl_top.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnl_top.Location = new System.Drawing.Point(0, 0);
            this.pnl_top.Name = "pnl_top";
            this.pnl_top.Size = new System.Drawing.Size(418, 156);
            this.pnl_top.TabIndex = 0;
            // 
            // cmbboxPath
            // 
            this.cmbboxPath.FormattingEnabled = true;
            this.cmbboxPath.Location = new System.Drawing.Point(96, 10);
            this.cmbboxPath.Margin = new System.Windows.Forms.Padding(4);
            this.cmbboxPath.Name = "cmbboxPath";
            this.cmbboxPath.Size = new System.Drawing.Size(228, 23);
            this.cmbboxPath.TabIndex = 18;
            // 
            // btnLogon
            // 
            this.btnLogon.Location = new System.Drawing.Point(165, 119);
            this.btnLogon.Margin = new System.Windows.Forms.Padding(4);
            this.btnLogon.Name = "btnLogon";
            this.btnLogon.Size = new System.Drawing.Size(100, 29);
            this.btnLogon.TabIndex = 17;
            this.btnLogon.Text = "登陆";
            this.btnLogon.UseVisualStyleBackColor = true;
            this.btnLogon.Click += new System.EventHandler(this.btnLogon_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(273, 46);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(4);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(100, 29);
            this.btnAdd.TabIndex = 16;
            this.btnAdd.Text = "保存信息";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // chkBxShowChar
            // 
            this.chkBxShowChar.AutoSize = true;
            this.chkBxShowChar.Checked = true;
            this.chkBxShowChar.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkBxShowChar.Location = new System.Drawing.Point(273, 89);
            this.chkBxShowChar.Margin = new System.Windows.Forms.Padding(4);
            this.chkBxShowChar.Name = "chkBxShowChar";
            this.chkBxShowChar.Size = new System.Drawing.Size(89, 19);
            this.chkBxShowChar.TabIndex = 15;
            this.chkBxShowChar.Text = "显示字符";
            this.chkBxShowChar.UseVisualStyleBackColor = true;
            this.chkBxShowChar.CheckedChanged += new System.EventHandler(this.chkBxShowChar_CheckedChanged);
            // 
            // txBxPwd
            // 
            this.txBxPwd.Location = new System.Drawing.Point(96, 85);
            this.txBxPwd.Margin = new System.Windows.Forms.Padding(4);
            this.txBxPwd.Name = "txBxPwd";
            this.txBxPwd.Size = new System.Drawing.Size(168, 25);
            this.txBxPwd.TabIndex = 14;
            // 
            // txBxAccount
            // 
            this.txBxAccount.Location = new System.Drawing.Point(96, 49);
            this.txBxAccount.Margin = new System.Windows.Forms.Padding(4);
            this.txBxAccount.Name = "txBxAccount";
            this.txBxAccount.Size = new System.Drawing.Size(168, 25);
            this.txBxAccount.TabIndex = 13;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(49, 89);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 15);
            this.label3.TabIndex = 11;
            this.label3.Text = "密码";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(49, 53);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 15);
            this.label2.TabIndex = 12;
            this.label2.Text = "账号";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 15);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 15);
            this.label1.TabIndex = 10;
            this.label1.Text = "安装路径";
            // 
            // btnBrow
            // 
            this.btnBrow.Location = new System.Drawing.Point(333, 10);
            this.btnBrow.Margin = new System.Windows.Forms.Padding(4);
            this.btnBrow.Name = "btnBrow";
            this.btnBrow.Size = new System.Drawing.Size(53, 26);
            this.btnBrow.TabIndex = 9;
            this.btnBrow.Text = "浏览";
            this.btnBrow.UseVisualStyleBackColor = true;
            this.btnBrow.Click += new System.EventHandler(this.btnBrow_Click);
            // 
            // dGVAccounts
            // 
            this.dGVAccounts.AllowUserToAddRows = false;
            this.dGVAccounts.AllowUserToDeleteRows = false;
            this.dGVAccounts.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dGVAccounts.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ClmnFirst,
            this.ClmnAccount});
            this.dGVAccounts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dGVAccounts.Location = new System.Drawing.Point(0, 156);
            this.dGVAccounts.Name = "dGVAccounts";
            this.dGVAccounts.RowHeadersVisible = false;
            this.dGVAccounts.RowTemplate.Height = 27;
            this.dGVAccounts.ShowEditingIcon = false;
            this.dGVAccounts.Size = new System.Drawing.Size(418, 227);
            this.dGVAccounts.TabIndex = 1;
            // 
            // ClmnFirst
            // 
            this.ClmnFirst.HeaderText = "选择";
            this.ClmnFirst.Name = "ClmnFirst";
            this.ClmnFirst.Width = 50;
            // 
            // ClmnAccount
            // 
            this.ClmnAccount.HeaderText = "帐号";
            this.ClmnAccount.Name = "ClmnAccount";
            // 
            // AutoLogonTM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(418, 383);
            this.Controls.Add(this.dGVAccounts);
            this.Controls.Add(this.pnl_top);
            this.Name = "AutoLogonTM";
            this.Text = "自动登录TM";
            this.Load += new System.EventHandler(this.AutoLogonTM_Load);
            this.pnl_top.ResumeLayout(false);
            this.pnl_top.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dGVAccounts)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnl_top;
        private System.Windows.Forms.ComboBox cmbboxPath;
        private System.Windows.Forms.Button btnLogon;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.CheckBox chkBxShowChar;
        private System.Windows.Forms.TextBox txBxPwd;
        private System.Windows.Forms.TextBox txBxAccount;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnBrow;
        private System.Windows.Forms.DataGridView dGVAccounts;
        private System.Windows.Forms.DataGridViewCheckBoxColumn ClmnFirst;
        private System.Windows.Forms.DataGridViewTextBoxColumn ClmnAccount;
    }
}

