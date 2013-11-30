using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace AutoLogonTM
{
    public partial class AutoLogonTM : Form
    {
        StringBuilder tmpath = new StringBuilder("C:\\Program Files (x86)\\Tencent\\TM2008\\Bin\\TM.exe");
        public AutoLogonTM()
        {
            InitializeComponent();
        }

        public class libLogonTM
        {
            [DllImport("libLogonTM.dll")]//
            public static extern void logonTM(StringBuilder qqAccount);
            [DllImport("libLogonTM.dll")]
            public static extern void addAQQRec(StringBuilder qqAccount, StringBuilder qqPwd);
            [DllImport("libLogonTM.dll")]
            public static extern int getAllAccounts(StringBuilder qqs, int buflen);
            [DllImport("libLogonTM.dll")]
            public static extern void setQPath(StringBuilder qpath);
            [DllImport("libLogonTM.dll")]
            public static extern int getLastPath(StringBuilder qpath, int buflen);
        }

        private string getAllAccounts()
        {
            StringBuilder s = new StringBuilder(256);
            int len = libLogonTM.getAllAccounts(s, 256);
            if (len == 0)
            {
                return null;
            }
            if (len >= 256)
            {
                s = new StringBuilder(len + 1);
                libLogonTM.getAllAccounts(s, len + 1);
            }
            return s.ToString();
        }

        private string getQPath()
        {
            StringBuilder s = new StringBuilder(256);
            int len = libLogonTM.getLastPath(s, 256);
            if (len == 0)
            {
                return null;
            }
            if (len >= 256)
            {
                s = new StringBuilder(len + 1);
                libLogonTM.getLastPath(s, len + 1);
            }
            return s.ToString();
        }
//         private string getPwdByAccount(string acc)
//         {
//             StringBuilder s = new StringBuilder(20);
//             StringBuilder a = new StringBuilder(acc);
//             int len = libLogonTM.getPwdByAccounts(a, s);
//             if (len == 0)
//             {
//                 return null;
//             }
//             if (len >= 20)
//             {
//                 s = new StringBuilder(len + 1);
//                 libLogonTM.getPwdByAccounts(a, s);
//             }
//             return s.ToString();
//         }

        void logonTM(string acc)
        {
            StringBuilder a = new StringBuilder(acc);
            if (this.WindowState != FormWindowState.Minimized)
            {
                this.WindowState = FormWindowState.Minimized;
            }
            libLogonTM.logonTM(a);
        }

        private void btnLogon_Click(object sender, EventArgs e)
        {
            if (!(txBxAccount.Text.Equals("") || txBxPwd.Text.Equals("")))
            {
                libLogonTM.addAQQRec(new StringBuilder(txBxAccount.Text), new StringBuilder(txBxPwd.Text));
                logonTM(txBxAccount.Text);
            }
            foreach (DataGridViewRow row in dGVAccounts.Rows)
            {
                if (Convert.ToBoolean(row.Cells[0].Value))
                {
                    //MessageBox.Show(row.Cells[1].Value.ToString());
                    logonTM(row.Cells[1].Value.ToString());
                }
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            StringBuilder acc = new StringBuilder(txBxAccount.Text);
            StringBuilder pwd = new StringBuilder(txBxPwd.Text);
            libLogonTM.addAQQRec(acc, pwd);
            dGVAccounts.Rows.Add(false, txBxAccount.Text);
        }

        private void chkBxShowChar_CheckedChanged(object sender, EventArgs e)
        {
            if (!chkBxShowChar.Checked)
            {
                txBxPwd.UseSystemPasswordChar = true;
            }
            else
            {
                txBxPwd.UseSystemPasswordChar = false;
            }
        }

        private void AutoLogonTM_Load(object sender, EventArgs e)
        {
            string allq = getAllAccounts();
            string qpath = getQPath();
            if (qpath != null && !qpath.Equals(""))
            {
                cmbboxPath.Items.Add(qpath);
                cmbboxPath.SelectedIndex = 0;
            }
            if (allq == null || allq.Equals("")) return;//
            string[] allqq = allq.Split('-');
            foreach (string qstr in allqq)
            {
                dGVAccounts.Rows.Add(false, qstr);
            }
        }

        private void btnBrow_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.ShowDialog();
            cmbboxPath.Items.Add(ofd.FileName);
            cmbboxPath.SelectedIndex = 0;
            StringBuilder qpath = new StringBuilder(ofd.FileName);
            libLogonTM.setQPath(qpath);
           // MessageBox.Show(qpath.ToString());
        }
    }
}

