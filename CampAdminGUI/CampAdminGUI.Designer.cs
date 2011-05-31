namespace CampPOSNS
{
    partial class CampAdminGUI
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
            if (disposing && (components != null))
            {
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
            this.listView = new System.Windows.Forms.ListView();
            this.bCreate = new System.Windows.Forms.Button();
            this.tCreateDesc = new System.Windows.Forms.TextBox();
            this.bRead = new System.Windows.Forms.Button();
            this.tReadID = new System.Windows.Forms.TextBox();
            this.bUpdate = new System.Windows.Forms.Button();
            this.tUpdateID = new System.Windows.Forms.TextBox();
            this.tUpdateDesc = new System.Windows.Forms.TextBox();
            this.bDelete = new System.Windows.Forms.Button();
            this.tDeleteID = new System.Windows.Forms.TextBox();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.IdColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.FirstNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.LastNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.AmountColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // listView
            // 
            this.listView.Location = new System.Drawing.Point(13, 13);
            this.listView.Name = "listView";
            this.listView.Size = new System.Drawing.Size(267, 146);
            this.listView.TabIndex = 0;
            this.listView.UseCompatibleStateImageBehavior = false;
            this.listView.View = System.Windows.Forms.View.List;
            // 
            // bCreate
            // 
            this.bCreate.Location = new System.Drawing.Point(13, 166);
            this.bCreate.Name = "bCreate";
            this.bCreate.Size = new System.Drawing.Size(75, 23);
            this.bCreate.TabIndex = 1;
            this.bCreate.Text = "Create";
            this.bCreate.UseVisualStyleBackColor = true;
            this.bCreate.Click += new System.EventHandler(this.bCreate_Click);
            // 
            // tCreateDesc
            // 
            this.tCreateDesc.Location = new System.Drawing.Point(118, 168);
            this.tCreateDesc.Name = "tCreateDesc";
            this.tCreateDesc.Size = new System.Drawing.Size(161, 20);
            this.tCreateDesc.TabIndex = 2;
            // 
            // bRead
            // 
            this.bRead.Location = new System.Drawing.Point(13, 195);
            this.bRead.Name = "bRead";
            this.bRead.Size = new System.Drawing.Size(75, 23);
            this.bRead.TabIndex = 3;
            this.bRead.Text = "Read";
            this.bRead.UseVisualStyleBackColor = true;
            this.bRead.Click += new System.EventHandler(this.bRead_Click);
            // 
            // tReadID
            // 
            this.tReadID.Location = new System.Drawing.Point(118, 198);
            this.tReadID.Name = "tReadID";
            this.tReadID.Size = new System.Drawing.Size(34, 20);
            this.tReadID.TabIndex = 4;
            // 
            // bUpdate
            // 
            this.bUpdate.Location = new System.Drawing.Point(13, 224);
            this.bUpdate.Name = "bUpdate";
            this.bUpdate.Size = new System.Drawing.Size(75, 25);
            this.bUpdate.TabIndex = 5;
            this.bUpdate.Text = "Update";
            this.bUpdate.UseVisualStyleBackColor = true;
            this.bUpdate.Click += new System.EventHandler(this.bUpdate_Click);
            // 
            // tUpdateID
            // 
            this.tUpdateID.Location = new System.Drawing.Point(118, 229);
            this.tUpdateID.Name = "tUpdateID";
            this.tUpdateID.Size = new System.Drawing.Size(34, 20);
            this.tUpdateID.TabIndex = 6;
            // 
            // tUpdateDesc
            // 
            this.tUpdateDesc.Location = new System.Drawing.Point(158, 229);
            this.tUpdateDesc.Name = "tUpdateDesc";
            this.tUpdateDesc.Size = new System.Drawing.Size(120, 20);
            this.tUpdateDesc.TabIndex = 7;
            // 
            // bDelete
            // 
            this.bDelete.Location = new System.Drawing.Point(13, 255);
            this.bDelete.Name = "bDelete";
            this.bDelete.Size = new System.Drawing.Size(75, 23);
            this.bDelete.TabIndex = 8;
            this.bDelete.Text = "Delete";
            this.bDelete.UseVisualStyleBackColor = true;
            this.bDelete.Click += new System.EventHandler(this.bDelete_Click);
            // 
            // tDeleteID
            // 
            this.tDeleteID.Location = new System.Drawing.Point(118, 258);
            this.tDeleteID.Name = "tDeleteID";
            this.tDeleteID.Size = new System.Drawing.Size(33, 20);
            this.tDeleteID.TabIndex = 9;
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IdColumn,
            this.FirstNameColumn,
            this.LastNameColumn,
            this.AmountColumn});
            this.dataGridView.Location = new System.Drawing.Point(13, 285);
            this.dataGridView.MultiSelect = false;
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            this.dataGridView.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            this.dataGridView.RowHeadersVisible = false;
            this.dataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView.Size = new System.Drawing.Size(462, 165);
            this.dataGridView.TabIndex = 10;
            this.dataGridView.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            // 
            // IdColumn
            // 
            this.IdColumn.HeaderText = "Id";
            this.IdColumn.Name = "IdColumn";
            this.IdColumn.ReadOnly = true;
            this.IdColumn.Width = 50;
            // 
            // FirstNameColumn
            // 
            this.FirstNameColumn.HeaderText = "First Name";
            this.FirstNameColumn.Name = "FirstNameColumn";
            this.FirstNameColumn.ReadOnly = true;
            this.FirstNameColumn.Width = 150;
            // 
            // LastNameColumn
            // 
            this.LastNameColumn.HeaderText = "Last Name";
            this.LastNameColumn.Name = "LastNameColumn";
            this.LastNameColumn.ReadOnly = true;
            this.LastNameColumn.Width = 150;
            // 
            // AmountColumn
            // 
            this.AmountColumn.HeaderText = "Amount";
            this.AmountColumn.Name = "AmountColumn";
            this.AmountColumn.ReadOnly = true;
            // 
            // CampAdminGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(487, 462);
            this.Controls.Add(this.dataGridView);
            this.Controls.Add(this.tDeleteID);
            this.Controls.Add(this.bDelete);
            this.Controls.Add(this.tUpdateDesc);
            this.Controls.Add(this.tUpdateID);
            this.Controls.Add(this.bUpdate);
            this.Controls.Add(this.tReadID);
            this.Controls.Add(this.bRead);
            this.Controls.Add(this.tCreateDesc);
            this.Controls.Add(this.bCreate);
            this.Controls.Add(this.listView);
            this.MaximumSize = new System.Drawing.Size(495, 500);
            this.MinimumSize = new System.Drawing.Size(300, 450);
            this.Name = "CampAdminGUI";
            this.Text = "CampAdmin";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Client_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listView;
        private System.Windows.Forms.Button bCreate;
        private System.Windows.Forms.TextBox tCreateDesc;
        private System.Windows.Forms.Button bRead;
        private System.Windows.Forms.TextBox tReadID;
        private System.Windows.Forms.Button bUpdate;
        private System.Windows.Forms.TextBox tUpdateID;
        private System.Windows.Forms.TextBox tUpdateDesc;
        private System.Windows.Forms.Button bDelete;
        private System.Windows.Forms.TextBox tDeleteID;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.DataGridViewTextBoxColumn IdColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn FirstNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn LastNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn AmountColumn;
    }
}

