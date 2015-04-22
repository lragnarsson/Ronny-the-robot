#pragma once

namespace PC_module {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::Text;
	using namespace System::Windows;
	using namespace System::Drawing;

	bool Header = false;
	bool Recieved_all_bytes = false;
	
	

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			
			

			testbitmap(image1, prevx, prevy, Color::White);
			testbitmap(image1, prevx, prevy - 15, Color::White);
			testbitmap(image1, prevx, prevy - 30, Color::White);
			testbitmap(image1, prevx, prevy - 45, Color::White);
			testbitmap(image1, prevx, prevy - 60, Color::White);
			testbitmap(image1, prevx, prevy - 75, Color::White);

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		Bitmap^ image1 = gcnew Bitmap(200, 200);
		int prevx = 100;
		int prevy = 200;
		array < System::Byte >^ data_recieved = gcnew array < System::Byte >(16);
		int write_position = 0;
		int expected_length = 0;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	protected:
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::DataGridView^  dataGridView1;

	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Sträcka;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Vinkel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  sensorfram;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  SensorframH;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  SensorbakH;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  SensorbakL;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  SensorframL;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Sträcka = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Vinkel = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->sensorfram = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SensorframH = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SensorbakH = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SensorbakL = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->SensorframL = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(24, 100);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(165, 20);
			this->textBox1->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 84);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"skickat";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(24, 43);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(72, 26);
			this->button1->TabIndex = 2;
			this->button1->Text = L"open";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(116, 44);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(73, 24);
			this->button2->TabIndex = 3;
			this->button2->Text = L"close";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {
				this->Sträcka,
					this->Vinkel, this->sensorfram, this->SensorframH, this->SensorbakH, this->SensorbakL, this->SensorframL
			});
			this->dataGridView1->Location = System::Drawing::Point(401, 14);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->Size = System::Drawing::Size(281, 235);
			this->dataGridView1->TabIndex = 4;
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
			// sensorfram
			// 
			this->sensorfram->HeaderText = L"sensorfram";
			this->sensorfram->Name = L"sensorfram";
			this->sensorfram->ReadOnly = true;
			// 
			// SensorframH
			// 
			this->SensorframH->HeaderText = L"SensorframH";
			this->SensorframH->Name = L"SensorframH";
			this->SensorframH->ReadOnly = true;
			// 
			// SensorbakH
			// 
			this->SensorbakH->HeaderText = L"SensorbakH";
			this->SensorbakH->Name = L"SensorbakH";
			this->SensorbakH->ReadOnly = true;
			// 
			// SensorbakL
			// 
			this->SensorbakL->HeaderText = L"SensorbakL";
			this->SensorbakL->Name = L"SensorbakL";
			this->SensorbakL->ReadOnly = true;
			// 
			// SensorframL
			// 
			this->SensorframL->HeaderText = L"SensorframL";
			this->SensorframL->Name = L"SensorframL";
			this->SensorframL->ReadOnly = true;
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			this->serialPort1->PortName = L"COM7";
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &MyForm::serialPort1_DataReceived);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::Control;
			this->pictureBox1->Location = System::Drawing::Point(195, 43);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(200, 200);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 5;
			this->pictureBox1->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(694, 261);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->KeyPreview = true;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::MyForm_KeyPress);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		//auto data = gcnew array < System::Byte > { 12 };
		if (!this->serialPort1->IsOpen){
			Console::WriteLine("opened serialport");
			this->serialPort1->PortName = "COM7";
			this->textBox1->Text = "port openeing, waiting";
			this->serialPort1->Open();
			this->textBox1->Text = "port open";
		}
		else{
			this->textBox1->Text = "something went wrong";
		}
	}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->serialPort1->IsOpen){
			this->serialPort1->Close();
			this->textBox1->Text = "connection closed";
		}
	}

	private: System::Void MyForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		auto dataforward = gcnew array < System::Byte > { 0xC1 };
		auto dataright = gcnew array < System::Byte > { 0xC2 };
		auto dataleft = gcnew array < System::Byte > { 0xC3 };
		auto databack = gcnew array < System::Byte > { 0xC4 };
		auto datarforwardright = gcnew array < System::Byte > { 0xC5 };
		auto dataforwardleft = gcnew array < System::Byte > { 0xC6 };

		switch (e->KeyChar){

		case 'a':

			this->textBox1->Text = "sväng vänster 1100 0011";
			this->serialPort1->Write(dataleft, 0, dataleft->Length);
			break;

		case 's':

			this->textBox1->Text = "bakåt 1100 0100";
			this->serialPort1->Write(databack, 0, databack->Length);
			break;

		case 'w':

			this->textBox1->Text = "framåt 1100 0001";
			this->serialPort1->Write(dataforward, 0, dataforward->Length);
			break;

		case 'd':

			this->textBox1->Text = "sväng höger 1100 0010";
			this->serialPort1->Write(dataright, 0, dataright->Length);
			break;

		case 'q':
			this->textBox1->Text = "sväng vänster-fram 1100 0110 ";
			this->serialPort1->Write(dataforward, 0, dataforwardleft->Length);
			break;

		case 'e':
			this->textBox1->Text = "sväng höger-fram 1100 0101";
			this->serialPort1->Write(datarforwardright, 0, datarforwardright->Length);
			break;

		default:
			this->textBox1->Text = "fel styrkommando";
			break;
		}
	}


	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
		SerialPort^ sp = (SerialPort^)sender;
		//auto data_recieved = gcnew array < System::Byte >(5);
		System::Byte byte = sp->ReadByte();
		//System::String^ info = sp->ReadExisting();
		//Console::Write("case:");
		//System::^ cases = info[0];
		//Console::WriteLine(cases);
		
		//switch (cases){
			//Console::WriteLine("recieving data");
			//Sensormodulen till PC
		/*case 0x40: //Sensordata
			Console::WriteLine("in switch, case0x40");
			for (int i; i < 5; i++){
				data_recieved[i] = sp->ReadByte();
			}
			sensorvalues(data_recieved[0], data_recieved[1], data_recieved[2], data_recieved[3], data_recieved[4]);
		case 0x41: //sträcka + vinkel
			Console::WriteLine("In switch, case0x41");
			for (int i; i < 2; i++){
				data_recieved[i] = sp->ReadByte();
			}
			wheelvalues(data_recieved[0], data_recieved[1]);
			break;

		default:
			break;
		}*/
		
		//Console::WriteLine("In switch, case0x41");
		/*for (int i; i < 2; i++){
			data[i] = sp->ReadExisting();
		}
		//wheelvalues(data[0], data[1]);
		Console::WriteLine();*/
		
		if (write_position == 0)
		{
			switch (byte)
			{
			case 0x40:
				expected_length = 11;
				data_recieved[write_position] = byte;
				++write_position;
				break;
			case 0x41:
				expected_length = 3;
				data_recieved[write_position] = byte;
				++write_position;
				break;
			default:
				break;
			}
			return;
		}

		data_recieved[write_position] = byte;
		
		if (++write_position == expected_length)
		{
			switch (data_recieved[0])
			{
			case 0x40:
			{
				int rear_left = data_recieved[1] << 8;
				rear_left |= data_recieved[2];
				int front_left = data_recieved[3] << 8;
				front_left |= data_recieved[4];
				int rear_right = data_recieved[5] << 8;
				rear_right |= data_recieved[6];
				int front_right = data_recieved[7] << 8;
				front_right |= data_recieved[8];
				int front = data_recieved[9] << 8;
				front |= data_recieved[10];
				Console::WriteLine(rear_left + " " + front_left + " " + rear_right + " " + front_right + " " + front);
				//sensorvalues(front, front_right, rear_right, rear_left, front_left);

				break;
			}
			default:
				break;
			}

			write_position = 0;
			expected_length = 0;
		}

		/*
		//Console::Write("incomming data:");
		if (indata != 0x40){
			for (size_t i = 0; i < length; i++)
			{

			}
			Console::Write(indata);
		}
		else{
			Console::WriteLine("newbyte");
		}*/
	}

	private: void wheelvalues(System::Byte^ byte1, System::Byte^ byte2){
		
		this->dataGridView1->Rows->Add(byte1, byte2);
		//this->dataGridView1->Rows->Add(byte1, byte2);
	}
	private: void sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){
		this->dataGridView1->Rows[0]->Cells[2]->Value = byte1;
		this->dataGridView1->Rows[0]->Cells[3]->Value = byte2;
		this->dataGridView1->Rows[0]->Cells[4]->Value = byte3;
		this->dataGridView1->Rows[0]->Cells[5]->Value = byte4;
		this->dataGridView1->Rows[0]->Cells[6]->Value = byte5;
		this->dataGridView1->Rows->Add();

	}

	private: void testbitmap(Bitmap^ image1, int prevx, int prevy, Color color){


		for (int x = prevx; x < prevx + 15; x++)
		{
			for (int y = prevy - 15; y < prevy; y++)
			{
				//Color pixelColor = image1->GetPixel(x, y);
				//Color newColor = Color::FromArgb(pixelColor.R, 250, 60);
				image1->SetPixel(x, y, color);
				image1->SetPixel(x + 15, y, Color::Black);
				if (y + 15 < 200){
					image1->SetPixel(x - 15, y, Color::Black);
				}
				else
				{
					image1->SetPixel(x - 15, y, Color::Black);
				}
			}
		}

		this->pictureBox1->Image = image1;

	}
	};
}