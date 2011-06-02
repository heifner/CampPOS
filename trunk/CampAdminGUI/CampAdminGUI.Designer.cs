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
            this.buttonCreate = new System.Windows.Forms.Button();
            this.textBoxFirstName = new System.Windows.Forms.TextBox();
            this.bRead = new System.Windows.Forms.Button();
            this.tReadID = new System.Windows.Forms.TextBox();
            this.bUpdate = new System.Windows.Forms.Button();
            this.tUpdateID = new System.Windows.Forms.TextBox();
            this.tUpdateDesc = new System.Windows.Forms.TextBox();
            this.bDelete = new System.Windows.Forms.Button();
            this.tDeleteID = new System.Windows.Forms.TextBox();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.labelFirstName = new System.Windows.Forms.Label();
            this.labelLastName = new System.Windows.Forms.Label();
            this.textBoxLastName = new System.Windows.Forms.TextBox();
            this.labelAmount = new System.Windows.Forms.Label();
            this.maskedTextBoxAmount = new System.Windows.Forms.MaskedTextBox();
            this.IdColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.FirstNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.LastNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.AmountColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // listView
            // 
            this.listView.Location = new System.Drawing.Point(12, 402);
            this.listView.Name = "listView";
            this.listView.Size = new System.Drawing.Size(468, 52);
            this.listView.TabIndex = 0;
            this.listView.UseCompatibleStateImageBehavior = false;
            this.listView.View = System.Windows.Forms.View.List;
            // 
            // buttonCreate
            // 
            this.buttonCreate.Location = new System.Drawing.Point(286, 26);
            this.buttonCreate.Name = "buttonCreate";
            this.buttonCreate.Size = new System.Drawing.Size(75, 23);
            this.buttonCreate.TabIndex = 4;
            this.buttonCreate.Text = "Create";
            this.buttonCreate.UseVisualStyleBackColor = true;
            this.buttonCreate.Click += new System.EventHandler(this.buttonCreate_Click);
            // 
            // textBoxFirstName
            // 
            this.textBoxFirstName.Location = new System.Drawing.Point(12, 29);
            this.textBoxFirstName.Name = "textBoxFirstName";
            this.textBoxFirstName.Size = new System.Drawing.Size(76, 20);
            this.textBoxFirstName.TabIndex = 1;
            this.textBoxFirstName.Validating += new System.ComponentModel.CancelEventHandler(this.textBoxFirstName_Validating);
            // 
            // bRead
            // 
            this.bRead.Location = new System.Drawing.Point(13, 195);
            this.bRead.Name = "bRead";
            this.bRead.Size = new System.Drawing.Size(75, 23);
            this.bRead.TabIndex = 33;
            this.bRead.Text = "Read";
            this.bRead.UseVisualStyleBackColor = true;
            this.bRead.Click += new System.EventHandler(this.bRead_Click);
            // 
            // tReadID
            // 
            this.tReadID.Location = new System.Drawing.Point(118, 198);
            this.tReadID.Name = "tReadID";
            this.tReadID.Size = new System.Drawing.Size(34, 20);
            this.tReadID.TabIndex = 44;
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
            this.dataGridView.Location = new System.Drawing.Point(12, 55);
            this.dataGridView.MultiSelect = false;
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            this.dataGridView.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            this.dataGridView.RowHeadersVisible = false;
            this.dataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView.Size = new System.Drawing.Size(468, 134);
            this.dataGridView.TabIndex = 10;
            this.dataGridView.TabStop = false;
            this.dataGridView.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            this.dataGridView.CellEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            // 
            // labelFirstName
            // 
            this.labelFirstName.AutoSize = true;
            this.labelFirstName.Location = new System.Drawing.Point(13, 13);
            this.labelFirstName.Name = "labelFirstName";
            this.labelFirstName.Size = new System.Drawing.Size(57, 13);
            this.labelFirstName.TabIndex = 11;
            this.labelFirstName.Text = "First Name";
            // 
            // labelLastName
            // 
            this.labelLastName.AutoSize = true;
            this.labelLastName.Location = new System.Drawing.Point(93, 13);
            this.labelLastName.Name = "labelLastName";
            this.labelLastName.Size = new System.Drawing.Size(58, 13);
            this.labelLastName.TabIndex = 12;
            this.labelLastName.Text = "Last Name";
            // 
            // textBoxLastName
            // 
            this.textBoxLastName.Location = new System.Drawing.Point(96, 29);
            this.textBoxLastName.Name = "textBoxLastName";
            this.textBoxLastName.Size = new System.Drawing.Size(100, 20);
            this.textBoxLastName.TabIndex = 2;
            this.textBoxLastName.TextChanged += new System.EventHandler(this.textBoxLastName_TextChanged);
            // 
            // labelAmount
            // 
            this.labelAmount.AutoSize = true;
            this.labelAmount.Location = new System.Drawing.Point(203, 13);
            this.labelAmount.Name = "labelAmount";
            this.labelAmount.Size = new System.Drawing.Size(43, 13);
            this.labelAmount.TabIndex = 14;
            this.labelAmount.Text = "Amount";
            // 
            // maskedTextBoxAmount
            // 
            this.maskedTextBoxAmount.AllowPromptAsInput = false;
            this.maskedTextBoxAmount.AsciiOnly = true;
            this.maskedTextBoxAmount.Location = new System.Drawing.Point(203, 29);
            this.maskedTextBoxAmount.Name = "maskedTextBoxAmount";
            this.maskedTextBoxAmount.Size = new System.Drawing.Size(77, 20);
            this.maskedTextBoxAmount.TabIndex = 3;
            this.maskedTextBoxAmount.Validating += new System.ComponentModel.CancelEventHandler(this.maskedTextBoxAmount_Validating);
            this.maskedTextBoxAmount.TextChanged += new System.EventHandler(this.maskedTextBoxAmount_TextChanged);
            // 
            // IdColumn
            // 
            this.IdColumn.HeaderText = "Id";
            this.IdColumn.Name = "IdColumn";
            this.IdColumn.ReadOnly = true;
            this.IdColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.IdColumn.Width = 60;
            // 
            // FirstNameColumn
            // 
            this.FirstNameColumn.HeaderText = "First Name";
            this.FirstNameColumn.Name = "FirstNameColumn";
            this.FirstNameColumn.ReadOnly = true;
            this.FirstNameColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.FirstNameColumn.Width = 125;
            // 
            // LastNameColumn
            // 
            this.LastNameColumn.HeaderText = "Last Name";
            this.LastNameColumn.Name = "LastNameColumn";
            this.LastNameColumn.ReadOnly = true;
            this.LastNameColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.LastNameColumn.Width = 175;
            // 
            // AmountColumn
            // 
            this.AmountColumn.HeaderText = "Amount";
            this.AmountColumn.Name = "AmountColumn";
            this.AmountColumn.ReadOnly = true;
            this.AmountColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.AmountColumn.Width = 80;
            // 
            // CampAdminGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(492, 466);
            this.Controls.Add(this.maskedTextBoxAmount);
            this.Controls.Add(this.labelAmount);
            this.Controls.Add(this.textBoxLastName);
            this.Controls.Add(this.labelLastName);
            this.Controls.Add(this.labelFirstName);
            this.Controls.Add(this.dataGridView);
            this.Controls.Add(this.tDeleteID);
            this.Controls.Add(this.bDelete);
            this.Controls.Add(this.tUpdateDesc);
            this.Controls.Add(this.tUpdateID);
            this.Controls.Add(this.bUpdate);
            this.Controls.Add(this.tReadID);
            this.Controls.Add(this.bRead);
            this.Controls.Add(this.textBoxFirstName);
            this.Controls.Add(this.buttonCreate);
            this.Controls.Add(this.listView);
            this.MaximumSize = new System.Drawing.Size(500, 500);
            this.MinimumSize = new System.Drawing.Size(500, 500);
            this.Name = "CampAdminGUI";
            this.Text = "CampAdmin";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Client_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listView;
        private System.Windows.Forms.Button buttonCreate;
        private System.Windows.Forms.TextBox textBoxFirstName;
        private System.Windows.Forms.Button bRead;
        private System.Windows.Forms.TextBox tReadID;
        private System.Windows.Forms.Button bUpdate;
        private System.Windows.Forms.TextBox tUpdateID;
        private System.Windows.Forms.TextBox tUpdateDesc;
        private System.Windows.Forms.Button bDelete;
        private System.Windows.Forms.TextBox tDeleteID;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Label labelFirstName;
        private System.Windows.Forms.Label labelLastName;
        private System.Windows.Forms.TextBox textBoxLastName;
        private System.Windows.Forms.Label labelAmount;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxAmount;
        private System.Windows.Forms.DataGridViewTextBoxColumn IdColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn FirstNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn LastNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn AmountColumn;
    }
}

