#pragma once

namespace GUIronny {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Grafer_data
	/// </summary>
	public ref class Grafer_data : public System::Windows::Forms::Form
	{
	public:
		Grafer_data(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Grafer_data()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sträcka;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Vinkel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sensorfront;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sensorfront_Left;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sensorfront_Right;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sensorback_Left;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sensorback_Right;
	private: System::Windows::Forms::Label^  Sensorvalues;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Sträcka = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Vinkel = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorfront = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorfront_Left = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorfront_Right = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorback_Left = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorback_Right = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Sensorvalues = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {
				this->Sträcka,
					this->Vinkel, this->Sensorfront, this->Sensorfront_Left, this->Sensorfront_Right, this->Sensorback_Left, this->Sensorback_Right
			});
			this->dataGridView1->Location = System::Drawing::Point(12, 38);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->Size = System::Drawing::Size(743, 349);
			this->dataGridView1->TabIndex = 0;
			// 
			// Sträcka
			// 
			this->Sträcka->HeaderText = L"Sträcka";
			this->Sträcka->Name = L"Sträcka";
			this->Sträcka->ReadOnly = true;
			// 
			// Vinkel
			// 
			this->Vinkel->HeaderText = L"Vinkel";
			this->Vinkel->Name = L"Vinkel";
			this->Vinkel->ReadOnly = true;
			// 
			// Sensor front
			// 
			this->Sensorfront->HeaderText = L"Sensor front";
			this->Sensorfront->Name = L"Sensor front";
			this->Sensorfront->ReadOnly = true;
			// 
			// Sensor front-left
			// 
			this->Sensorfront_Left->HeaderText = L"Sensor front-left";
			this->Sensorfront_Left->Name = L"Sensor front-left";
			this->Sensorfront_Left->ReadOnly = true;
			// 
			// SensorbakL
			// 
			this->Sensorfront_Right->HeaderText = L"Sensor front-right";
			this->Sensorfront_Right->Name = L"Sensor front-right";
			this->Sensorfront_Right->ReadOnly = true;
			// 
			// SensorframH
			// 
			this->Sensorback_Left->HeaderText = L"Sensor back-left";
			this->Sensorback_Left->Name = L"Sensor back-left";
			this->Sensorback_Left->ReadOnly = true;
			// 
			// SensorframL
			// 
			this->Sensorback_Right->HeaderText = L"Sensor back-right";
			this->Sensorback_Right->Name = L"Sensor back-right";
			this->Sensorback_Right->ReadOnly = true;
			// 
			// Sensorvalues
			// 
			this->Sensorvalues->AutoSize = true;
			this->Sensorvalues->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Sensorvalues->Location = System::Drawing::Point(312, 9);
			this->Sensorvalues->Name = L"Sensorvalues";
			this->Sensorvalues->Size = System::Drawing::Size(106, 17);
			this->Sensorvalues->TabIndex = 1;
			this->Sensorvalues->Text = L"Sensorvalues";
			// 
			// Grafer_data
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(766, 451);
			this->Controls->Add(this->Sensorvalues);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"Grafer_data";
			this->Text = L"Grafer_data";
			this->Load += gcnew System::EventHandler(this, &Grafer_data::Grafer_data_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: System::Void Grafer_data_Load(System::Object^  sender, System::EventArgs^  e) {
	}

	public: void sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){

		int rowcount = this->dataGridView1->RowCount;
		Console::WriteLine(rowcount);
		this->dataGridView1->Rows->Add();
		this->dataGridView1->Rows[rowcount - 1]->Cells[2]->Value = byte1 + 1;
		this->dataGridView1->Rows[rowcount - 1]->Cells[3]->Value = byte2 + 1;
		this->dataGridView1->Rows[rowcount - 1]->Cells[4]->Value = byte3 + 1;
		this->dataGridView1->Rows[rowcount - 1]->Cells[5]->Value = byte4 + 1;
		this->dataGridView1->Rows[rowcount - 1]->Cells[6]->Value = byte5 + 1;
		
	}

	};
}
