#pragma once
#include "Grafer_data.h"

namespace GUIronny {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::Text;
	using namespace System::Windows;

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
			findPorts();
			testbitmap(image1, prevx, prevy, Color::White);
			testbitmap(image1, prevx, prevy - 15, Color::White);
			testbitmap(image1, prevx, prevy - 30, Color::White);
			testbitmap(image1, prevx, prevy - 45, Color::White);
			testbitmap(image1, prevx, prevy - 60, Color::White);
			testbitmap(image1, prevx, prevy - 75, Color::White);

			//sensorvalues(10, 10, 10, 10, 10);

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
	protected:
		Bitmap^ image1 = gcnew Bitmap(744, 480);
		int prevx = 450;
		int prevy = 480;
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
	private: System::Windows::Forms::PictureBox^  Ronny_robot;
	private: System::Windows::Forms::Label^  Sensorvärden;
	private: System::Windows::Forms::PictureBox^  Leftarrow_unpressed;
	private: System::Windows::Forms::PictureBox^  Leftarrow_pressed;
	private: System::Windows::Forms::PictureBox^  Uparrow_unpressed;
	private: System::Windows::Forms::PictureBox^  Downarrow_unpressed;
	private: System::Windows::Forms::PictureBox^  Rightarrow_unpressed;
	private: System::Windows::Forms::PictureBox^  Uparrow_pressed;
	private: System::Windows::Forms::PictureBox^  Downarrow_pressed;
	private: System::Windows::Forms::PictureBox^  Rightarrow_pressed;
	private: System::Windows::Forms::TextBox^  IRsensor_VF;
	private: System::Windows::Forms::TextBox^  IRsensor_VB;
	private: System::Windows::Forms::TextBox^  IRsensor_HF;
	private: System::Windows::Forms::TextBox^  IRsensor_HB;
	private: System::Windows::Forms::TextBox^  IRsensor_Front;
	private: System::IO::Ports::SerialPort^  serialPort1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  Kommandon;
	private: System::Windows::Forms::Button^  Sensordata;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  open_closed;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		//	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->Ronny_robot = (gcnew System::Windows::Forms::PictureBox());
			this->Sensorvärden = (gcnew System::Windows::Forms::Label());
			this->Leftarrow_unpressed = (gcnew System::Windows::Forms::PictureBox());
			this->Leftarrow_pressed = (gcnew System::Windows::Forms::PictureBox());
			this->Uparrow_unpressed = (gcnew System::Windows::Forms::PictureBox());
			this->Downarrow_unpressed = (gcnew System::Windows::Forms::PictureBox());
			this->Rightarrow_unpressed = (gcnew System::Windows::Forms::PictureBox());
			this->Uparrow_pressed = (gcnew System::Windows::Forms::PictureBox());
			this->Downarrow_pressed = (gcnew System::Windows::Forms::PictureBox());
			this->Rightarrow_pressed = (gcnew System::Windows::Forms::PictureBox());
			this->IRsensor_VF = (gcnew System::Windows::Forms::TextBox());
			this->IRsensor_VB = (gcnew System::Windows::Forms::TextBox());
			this->IRsensor_HF = (gcnew System::Windows::Forms::TextBox());
			this->IRsensor_HB = (gcnew System::Windows::Forms::TextBox());
			this->IRsensor_Front = (gcnew System::Windows::Forms::TextBox());
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->Kommandon = (gcnew System::Windows::Forms::TextBox());
			this->Sensordata = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->open_closed = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ronny_robot))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Leftarrow_unpressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Leftarrow_pressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Uparrow_unpressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Downarrow_unpressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Rightarrow_unpressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Uparrow_pressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Downarrow_pressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Rightarrow_pressed))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// Ronny_robot
			// 
			this->Ronny_robot->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Ronny_robot.Image")));
			this->Ronny_robot->Location = System::Drawing::Point(69, 266);
			this->Ronny_robot->Name = L"Ronny_robot";
			this->Ronny_robot->Size = System::Drawing::Size(141, 147);
			this->Ronny_robot->TabIndex = 0;
			this->Ronny_robot->TabStop = false;
			// 
			// Sensorvärden
			// 
			this->Sensorvärden->AutoSize = true;
			this->Sensorvärden->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Sensorvärden->Location = System::Drawing::Point(96, 216);
			this->Sensorvärden->Name = L"Sensorvärden";
			this->Sensorvärden->Size = System::Drawing::Size(85, 13);
			this->Sensorvärden->TabIndex = 1;
			this->Sensorvärden->Text = L"Sensorvärden";
			// 
			// Leftarrow_unpressed
			// 
			this->Leftarrow_unpressed->BackColor = System::Drawing::Color::White;
			this->Leftarrow_unpressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Leftarrow_unpressed.Image")));
			this->Leftarrow_unpressed->Location = System::Drawing::Point(51, 530);
			this->Leftarrow_unpressed->Name = L"Leftarrow_unpressed";
			this->Leftarrow_unpressed->Size = System::Drawing::Size(61, 63);
			this->Leftarrow_unpressed->TabIndex = 2;
			this->Leftarrow_unpressed->TabStop = false;
			// 
			// Leftarrow_pressed
			// 
			this->Leftarrow_pressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Leftarrow_pressed.Image")));
			this->Leftarrow_pressed->Location = System::Drawing::Point(51, 530);
			this->Leftarrow_pressed->Name = L"Leftarrow_pressed";
			this->Leftarrow_pressed->Size = System::Drawing::Size(59, 63);
			this->Leftarrow_pressed->TabIndex = 4;
			this->Leftarrow_pressed->TabStop = false;
			// 
			// Uparrow_unpressed
			// 
			this->Uparrow_unpressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Uparrow_unpressed.Image")));
			this->Uparrow_unpressed->Location = System::Drawing::Point(106, 482);
			this->Uparrow_unpressed->Name = L"Uparrow_unpressed";
			this->Uparrow_unpressed->Size = System::Drawing::Size(59, 55);
			this->Uparrow_unpressed->TabIndex = 5;
			this->Uparrow_unpressed->TabStop = false;
			// 
			// Downarrow_unpressed
			// 
			this->Downarrow_unpressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Downarrow_unpressed.Image")));
			this->Downarrow_unpressed->Location = System::Drawing::Point(106, 536);
			this->Downarrow_unpressed->Name = L"Downarrow_unpressed";
			this->Downarrow_unpressed->Size = System::Drawing::Size(57, 57);
			this->Downarrow_unpressed->TabIndex = 6;
			this->Downarrow_unpressed->TabStop = false;
			// 
			// Rightarrow_unpressed
			// 
			this->Rightarrow_unpressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Rightarrow_unpressed.Image")));
			this->Rightarrow_unpressed->Location = System::Drawing::Point(160, 534);
			this->Rightarrow_unpressed->Name = L"Rightarrow_unpressed";
			this->Rightarrow_unpressed->Size = System::Drawing::Size(55, 59);
			this->Rightarrow_unpressed->TabIndex = 7;
			this->Rightarrow_unpressed->TabStop = false;
			// 
			// Uparrow_pressed
			// 
			this->Uparrow_pressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Uparrow_pressed.Image")));
			this->Uparrow_pressed->Location = System::Drawing::Point(106, 482);
			this->Uparrow_pressed->Name = L"Uparrow_pressed";
			this->Uparrow_pressed->Size = System::Drawing::Size(59, 55);
			this->Uparrow_pressed->TabIndex = 8;
			this->Uparrow_pressed->TabStop = false;
			// 
			// Downarrow_pressed
			// 
			this->Downarrow_pressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Downarrow_pressed.Image")));
			this->Downarrow_pressed->Location = System::Drawing::Point(106, 536);
			this->Downarrow_pressed->Name = L"Downarrow_pressed";
			this->Downarrow_pressed->Size = System::Drawing::Size(56, 57);
			this->Downarrow_pressed->TabIndex = 9;
			this->Downarrow_pressed->TabStop = false;
			// 
			// Rightarrow_pressed
			// 
			this->Rightarrow_pressed->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Rightarrow_pressed.Image")));
			this->Rightarrow_pressed->Location = System::Drawing::Point(160, 534);
			this->Rightarrow_pressed->Name = L"Rightarrow_pressed";
			this->Rightarrow_pressed->Size = System::Drawing::Size(55, 56);
			this->Rightarrow_pressed->TabIndex = 10;
			this->Rightarrow_pressed->TabStop = false;
			// 
			// IRsensor_VF
			// 
			this->IRsensor_VF->BackColor = System::Drawing::Color::White;
			this->IRsensor_VF->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->IRsensor_VF->Enabled = false;
			this->IRsensor_VF->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IRsensor_VF->Location = System::Drawing::Point(42, 318);
			this->IRsensor_VF->Name = L"IRsensor_VF";
			this->IRsensor_VF->Size = System::Drawing::Size(37, 13);
			this->IRsensor_VF->TabIndex = 11;
			this->IRsensor_VF->Text = L"20";
			this->IRsensor_VF->TextChanged += gcnew System::EventHandler(this, &MyForm::IRsensor_VF_TextChanged);
			// 
			// IRsensor_VB
			// 
			this->IRsensor_VB->BackColor = System::Drawing::Color::White;
			this->IRsensor_VB->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->IRsensor_VB->Enabled = false;
			this->IRsensor_VB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IRsensor_VB->ForeColor = System::Drawing::SystemColors::WindowText;
			this->IRsensor_VB->Location = System::Drawing::Point(42, 380);
			this->IRsensor_VB->Name = L"IRsensor_VB";
			this->IRsensor_VB->Size = System::Drawing::Size(37, 13);
			this->IRsensor_VB->TabIndex = 12;
			this->IRsensor_VB->Text = L"12";
			// 
			// IRsensor_HF
			// 
			this->IRsensor_HF->BackColor = System::Drawing::Color::White;
			this->IRsensor_HF->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->IRsensor_HF->Enabled = false;
			this->IRsensor_HF->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IRsensor_HF->Location = System::Drawing::Point(201, 318);
			this->IRsensor_HF->Name = L"IRsensor_HF";
			this->IRsensor_HF->Size = System::Drawing::Size(37, 13);
			this->IRsensor_HF->TabIndex = 11;
			this->IRsensor_HF->Text = L"20";
			// 
			// IRsensor_HB
			// 
			this->IRsensor_HB->BackColor = System::Drawing::Color::White;
			this->IRsensor_HB->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->IRsensor_HB->Enabled = false;
			this->IRsensor_HB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IRsensor_HB->Location = System::Drawing::Point(201, 380);
			this->IRsensor_HB->Name = L"IRsensor_HB";
			this->IRsensor_HB->Size = System::Drawing::Size(37, 13);
			this->IRsensor_HB->TabIndex = 13;
			this->IRsensor_HB->Text = L"20";
			// 
			// IRsensor_Front
			// 
			this->IRsensor_Front->BackColor = System::Drawing::Color::White;
			this->IRsensor_Front->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->IRsensor_Front->Enabled = false;
			this->IRsensor_Front->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IRsensor_Front->Location = System::Drawing::Point(126, 247);
			this->IRsensor_Front->Name = L"IRsensor_Front";
			this->IRsensor_Front->Size = System::Drawing::Size(37, 13);
			this->IRsensor_Front->TabIndex = 14;
			this->IRsensor_Front->Text = L"20";
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			this->serialPort1->PortName = L"COM3";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(42, 76);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(88, 28);
			this->button3->TabIndex = 15;
			this->button3->Text = L"Open";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(136, 76);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(78, 28);
			this->button4->TabIndex = 16;
			this->button4->Text = L"Close";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// Kommandon
			// 
			this->Kommandon->BackColor = System::Drawing::Color::White;
			this->Kommandon->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->Kommandon->Enabled = false;
			this->Kommandon->Location = System::Drawing::Point(126, 162);
			this->Kommandon->Name = L"Kommandon";
			this->Kommandon->Size = System::Drawing::Size(133, 13);
			this->Kommandon->TabIndex = 17;
			// 
			// Sensordata
			// 
			this->Sensordata->BackColor = System::Drawing::Color::Gainsboro;
			this->Sensordata->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->Sensordata->Location = System::Drawing::Point(12, 154);
			this->Sensordata->Name = L"Sensordata";
			this->Sensordata->Size = System::Drawing::Size(90, 28);
			this->Sensordata->TabIndex = 18;
			this->Sensordata->Text = L"Sensordata";
			this->Sensordata->UseVisualStyleBackColor = true;
			this->Sensordata->Click += gcnew System::EventHandler(this, &MyForm::Sensordata_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(12, 110);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 13);
			this->label1->TabIndex = 19;
			this->label1->Text = L"Status:";
			// 
			// comboBox1
			// 
			this->comboBox1->AccessibleName = L"";
			this->comboBox1->AccessibleRole = System::Windows::Forms::AccessibleRole::MenuBar;
			this->comboBox1->DisplayMember = L"hej";
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(69, 38);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 20;
			this->comboBox1->Tag = L"Välj Serieport";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(75, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(106, 13);
			this->label2->TabIndex = 21;
			this->label2->Text = L"Choose COM-port";
			// 
			// open_closed
			// 
			this->open_closed->BackColor = System::Drawing::Color::White;
			this->open_closed->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->open_closed->Enabled = false;
			this->open_closed->Location = System::Drawing::Point(59, 111);
			this->open_closed->Name = L"open_closed";
			this->open_closed->Size = System::Drawing::Size(100, 13);
			this->open_closed->TabIndex = 22;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(440, 110);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(744, 480);
			this->pictureBox1->TabIndex = 23;
			this->pictureBox1->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1300, 667);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->open_closed);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Sensordata);
			this->Controls->Add(this->Kommandon);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->IRsensor_Front);
			this->Controls->Add(this->IRsensor_HB);
			this->Controls->Add(this->IRsensor_VB);
			this->Controls->Add(this->IRsensor_HF);
			this->Controls->Add(this->IRsensor_VF);
			this->Controls->Add(this->Rightarrow_unpressed);
			this->Controls->Add(this->Sensorvärden);
			this->Controls->Add(this->Ronny_robot);
			this->Controls->Add(this->Rightarrow_pressed);
			this->Controls->Add(this->Uparrow_unpressed);
			this->Controls->Add(this->Downarrow_unpressed);
			this->Controls->Add(this->Uparrow_pressed);
			this->Controls->Add(this->Downarrow_pressed);
			this->Controls->Add(this->Leftarrow_unpressed);
			this->Controls->Add(this->Leftarrow_pressed);
			this->KeyPreview = true;
			this->Name = L"MyForm";
			this->Text = L"GUI Ronny";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::MyForm_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Ronny_robot))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Leftarrow_unpressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Leftarrow_pressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Uparrow_unpressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Downarrow_unpressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Rightarrow_unpressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Uparrow_pressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Downarrow_pressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Rightarrow_pressed))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}

			 // find available ports
	private: void findPorts(void)
	{
		// get port names
		array<Object^>^ objectArray = SerialPort::GetPortNames();
		// add string array to combobox
		this->comboBox1->Items->AddRange(objectArray);
	}

	private: System::Void non(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void MyForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		switch (e->KeyCode){
		case Keys::A:
			this->Leftarrow_unpressed->Visible = false;
			break;

		case Keys::D:
			this->Rightarrow_unpressed->Visible = false;
			break;

		case Keys::S:
			this->Downarrow_unpressed->Visible = false;
			break;

		case Keys::W:
			this->Uparrow_unpressed->Visible = false;
			break;
		}
	}
	private: System::Void MyForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		switch (e->KeyCode){
		case Keys::A:
			this->Leftarrow_unpressed->Visible = true;
			break;

		case Keys::D:
			this->Rightarrow_unpressed->Visible = true;
			break;

		case Keys::S:
			this->Downarrow_unpressed->Visible = true;
			break;

		case Keys::W:
			this->Uparrow_unpressed->Visible = true;
			break;
		}
	}
	private: System::Void MyForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		auto dataleft = gcnew array < System::Byte > { 0xC3 };
		auto databack = gcnew array < System::Byte > { 0xC4 };
		auto dataforward = gcnew array < System::Byte > { 0xC1 };
		auto dataright = gcnew array < System::Byte > { 0xC2 };
		auto datarforwardright = gcnew array < System::Byte > { 0xC5 };
		auto dataforwardleft = gcnew array < System::Byte > { 0xC6 };

		switch (e->KeyChar){

		case 'a':

			this->Kommandon->Text = "sväng vänster 1100 0011";
			this->serialPort1->Write(dataleft, 0, dataleft->Length);
			break;

		case 's':

			this->Kommandon->Text = "bakåt 1100 0100";
			this->serialPort1->Write(databack, 0, databack->Length);
			break;

		case 'w':

			this->Kommandon->Text = "framåt 1100 0001";
			this->serialPort1->Write(dataforward, 0, dataforward->Length);
			break;

		case 'd':

			this->Kommandon->Text = "sväng höger1100 0010";
			this->serialPort1->Write(dataright, 0, dataright->Length);
			break;

		case 'q':
			this->Kommandon->Text = "sväng vänster-fram 1100 0110 ";
			this->serialPort1->Write(dataforward, 0, dataforwardleft->Length);
			break;

		case 'e':
			this->Kommandon->Text = "sväng höger-fram 1100 0101";
			this->serialPort1->Write(datarforwardright, 0, datarforwardright->Length);
			break;
		default:
			this->Kommandon->Text = "fel styrkommando";
			break;
		}
	}

	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
		
		SerialPort^ sp = (SerialPort^)sender;
		System::Byte byte = sp->ReadByte();
		
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
	}

	private: void wheelvalues(System::Byte^ byte1, System::Byte^ byte2){

		Grafer_data^ grafer = gcnew Grafer_data();
		grafer->dataGridView1->Rows->Add(byte1, byte2);
		//this->dataGridView1->Rows->Add(byte1, byte2);
	}
	/*private: void sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){
		Grafer_data^ grafer = gcnew Grafer_data();
		grafer->dataGridView1->Rows[0]->Cells[2]->Value = byte1;
		grafer->dataGridView1->Rows[0]->Cells[3]->Value = byte2;
		grafer->dataGridView1->Rows[0]->Cells[4]->Value = byte3;
		grafer->dataGridView1->Rows[0]->Cells[5]->Value = byte4;
		grafer->dataGridView1->Rows[0]->Cells[6]->Value = byte5;
		grafer->dataGridView1->Rows->Add();

	}*/

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		auto data = gcnew array < System::Byte > { 12 };
		if (!this->serialPort1->IsOpen){
			this->serialPort1->PortName = this->comboBox1->Text;
			this->open_closed->Text = "port openeing, waiting";
			this->serialPort1->Open();
			this->open_closed->Text = "port open";
		}
		else{
			this->open_closed->Text = "something went wrong";
		}
	}

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		if (this->serialPort1->IsOpen){
			this->serialPort1->Close();
			this->open_closed->Text = "connection closed";
		}
	}

	private: void testbitmap(Bitmap^ image1, int prevx, int prevy, Color color){


		for (int x = prevx; x < prevx + 40; x++)
		{
			for (int y = prevy - 40; y < prevy; y++)
			{
				//Color pixelColor = image1->GetPixel(x, y);
				//Color newColor = Color::FromArgb(pixelColor.R, 250, 60);
				image1->SetPixel(x, y, color);
				image1->SetPixel(x + 40, y, Color::Black);
				if (y + 40 < 200){
					image1->SetPixel(x - 40, y, Color::Black);
				}
				else
				{
					image1->SetPixel(x - 40, y, Color::Black);
				}
			}
		}

		this->pictureBox1->Image = image1;

	}

	private: System::Void IRsensor_VF_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void Sensordata_Click(System::Object^  sender, System::EventArgs^  e) {
		Grafer_data^ iform = (gcnew Grafer_data());
		iform->Show();
		iform->sensorvalues(10, 10, 10, 10, 10);
		iform->sensorvalues(11, 11, 11, 11, 11);
		iform->sensorvalues(12, 12, 12, 12, 12);
	}
	};
}

