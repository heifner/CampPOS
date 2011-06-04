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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CampAdminGUI));
            this.buttonCreate = new System.Windows.Forms.Button();
            this.textBoxFirstName = new System.Windows.Forms.TextBox();
            this.buttonUpdate = new System.Windows.Forms.Button();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.IdColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.FirstNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.LastNameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.AmountColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.labelFirstName = new System.Windows.Forms.Label();
            this.labelLastName = new System.Windows.Forms.Label();
            this.textBoxLastName = new System.Windows.Forms.TextBox();
            this.labelAmount = new System.Windows.Forms.Label();
            this.maskedTextBoxAmount = new System.Windows.Forms.MaskedTextBox();
            this.progressBarScan = new System.Windows.Forms.ProgressBar();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.labelAmountUpdate = new System.Windows.Forms.Label();
            this.textBoxUpdateAmount = new System.Windows.Forms.TextBox();
            this.buttonScan = new System.Windows.Forms.Button();
            this.labelProgress = new System.Windows.Forms.Label();
            this.buttonIcon = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonCreate
            // 
            this.buttonCreate.Location = new System.Drawing.Point(252, 27);
            this.buttonCreate.Name = "buttonCreate";
            this.buttonCreate.Size = new System.Drawing.Size(90, 23);
            this.buttonCreate.TabIndex = 4;
            this.buttonCreate.Text = "Scan/Create";
            this.toolTip.SetToolTip(this.buttonCreate, "Interacts with Finger Print Reader to add Camper.");
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
            // buttonUpdate
            // 
            this.buttonUpdate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonUpdate.Location = new System.Drawing.Point(121, 431);
            this.buttonUpdate.Name = "buttonUpdate";
            this.buttonUpdate.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdate.TabIndex = 5;
            this.buttonUpdate.TabStop = false;
            this.buttonUpdate.Text = "Update";
            this.buttonUpdate.UseVisualStyleBackColor = true;
            this.buttonUpdate.Click += new System.EventHandler(this.buttonUpdate_Click);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonDelete.Location = new System.Drawing.Point(405, 431);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(75, 23);
            this.buttonDelete.TabIndex = 8;
            this.buttonDelete.TabStop = false;
            this.buttonDelete.Text = "Delete";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.IdColumn,
            this.FirstNameColumn,
            this.LastNameColumn,
            this.AmountColumn});
            this.dataGridView.Location = new System.Drawing.Point(12, 84);
            this.dataGridView.MultiSelect = false;
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            this.dataGridView.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.None;
            this.dataGridView.RowHeadersVisible = false;
            this.dataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView.Size = new System.Drawing.Size(468, 341);
            this.dataGridView.TabIndex = 10;
            this.dataGridView.TabStop = false;
            this.dataGridView.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            this.dataGridView.CellEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            // 
            // IdColumn
            // 
            this.IdColumn.HeaderText = "Id";
            this.IdColumn.Name = "IdColumn";
            this.IdColumn.ReadOnly = true;
            this.IdColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.IdColumn.Width = 68;
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
            this.LastNameColumn.Width = 170;
            // 
            // AmountColumn
            // 
            this.AmountColumn.HeaderText = "Amount";
            this.AmountColumn.Name = "AmountColumn";
            this.AmountColumn.ReadOnly = true;
            this.AmountColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.AmountColumn.Width = 85;
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
            this.maskedTextBoxAmount.Size = new System.Drawing.Size(43, 20);
            this.maskedTextBoxAmount.TabIndex = 3;
            this.maskedTextBoxAmount.Validating += new System.ComponentModel.CancelEventHandler(this.maskedTextBoxAmount_Validating);
            this.maskedTextBoxAmount.TextChanged += new System.EventHandler(this.maskedTextBoxAmount_TextChanged);
            // 
            // progressBarScan
            // 
            this.progressBarScan.Location = new System.Drawing.Point(97, 55);
            this.progressBarScan.Name = "progressBarScan";
            this.progressBarScan.Size = new System.Drawing.Size(149, 23);
            this.progressBarScan.TabIndex = 45;
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(12, 55);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 5;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // labelAmountUpdate
            // 
            this.labelAmountUpdate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.labelAmountUpdate.AutoSize = true;
            this.labelAmountUpdate.Location = new System.Drawing.Point(14, 436);
            this.labelAmountUpdate.Name = "labelAmountUpdate";
            this.labelAmountUpdate.Size = new System.Drawing.Size(46, 13);
            this.labelAmountUpdate.TabIndex = 47;
            this.labelAmountUpdate.Text = "Amount:";
            // 
            // textBoxUpdateAmount
            // 
            this.textBoxUpdateAmount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.textBoxUpdateAmount.Location = new System.Drawing.Point(63, 433);
            this.textBoxUpdateAmount.Name = "textBoxUpdateAmount";
            this.textBoxUpdateAmount.Size = new System.Drawing.Size(52, 20);
            this.textBoxUpdateAmount.TabIndex = 48;
            this.textBoxUpdateAmount.TabStop = false;
            // 
            // buttonScan
            // 
            this.buttonScan.Location = new System.Drawing.Point(252, 55);
            this.buttonScan.Name = "buttonScan";
            this.buttonScan.Size = new System.Drawing.Size(90, 23);
            this.buttonScan.TabIndex = 49;
            this.buttonScan.TabStop = false;
            this.buttonScan.Text = "Scan/Find";
            this.buttonScan.UseVisualStyleBackColor = true;
            this.buttonScan.Click += new System.EventHandler(this.buttonScan_Click);
            // 
            // labelProgress
            // 
            this.labelProgress.AutoSize = true;
            this.labelProgress.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelProgress.Location = new System.Drawing.Point(348, 58);
            this.labelProgress.Name = "labelProgress";
            this.labelProgress.Size = new System.Drawing.Size(70, 18);
            this.labelProgress.TabIndex = 50;
            this.labelProgress.Text = "Ready...";
            // 
            // buttonIcon
            // 
            this.buttonIcon.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonIcon.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.buttonIcon.FlatAppearance.BorderSize = 0;
            this.buttonIcon.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.Control;
            this.buttonIcon.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.Control;
            this.buttonIcon.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonIcon.ForeColor = System.Drawing.SystemColors.Control;
            this.buttonIcon.Image = global::CampAdminGUI.Properties.Resources.CampPOS50x50;
            this.buttonIcon.Location = new System.Drawing.Point(430, 0);
            this.buttonIcon.Name = "buttonIcon";
            this.buttonIcon.Size = new System.Drawing.Size(60, 60);
            this.buttonIcon.TabIndex = 51;
            this.buttonIcon.TabStop = false;
            this.buttonIcon.UseVisualStyleBackColor = true;
            // 
            // CampAdminGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(492, 466);
            this.Controls.Add(this.buttonIcon);
            this.Controls.Add(this.labelProgress);
            this.Controls.Add(this.buttonScan);
            this.Controls.Add(this.textBoxUpdateAmount);
            this.Controls.Add(this.labelAmountUpdate);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.progressBarScan);
            this.Controls.Add(this.maskedTextBoxAmount);
            this.Controls.Add(this.labelAmount);
            this.Controls.Add(this.textBoxLastName);
            this.Controls.Add(this.labelLastName);
            this.Controls.Add(this.labelFirstName);
            this.Controls.Add(this.dataGridView);
            this.Controls.Add(this.buttonDelete);
            this.Controls.Add(this.buttonUpdate);
            this.Controls.Add(this.textBoxFirstName);
            this.Controls.Add(this.buttonCreate);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(500, 500);
            this.Name = "CampAdminGUI";
            this.Text = "CampAdmin";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Client_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonCreate;
        private System.Windows.Forms.TextBox textBoxFirstName;
        private System.Windows.Forms.Button buttonUpdate;
        private System.Windows.Forms.Button buttonDelete;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Label labelFirstName;
        private System.Windows.Forms.Label labelLastName;
        private System.Windows.Forms.TextBox textBoxLastName;
        private System.Windows.Forms.Label labelAmount;
        private System.Windows.Forms.MaskedTextBox maskedTextBoxAmount;
        private System.Windows.Forms.ProgressBar progressBarScan;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.Label labelAmountUpdate;
        private System.Windows.Forms.TextBox textBoxUpdateAmount;
        private System.Windows.Forms.Button buttonScan;
        private System.Windows.Forms.Label labelProgress;
        private System.Windows.Forms.DataGridViewTextBoxColumn IdColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn FirstNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn LastNameColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn AmountColumn;
        private System.Windows.Forms.Button buttonIcon;
    }
}

