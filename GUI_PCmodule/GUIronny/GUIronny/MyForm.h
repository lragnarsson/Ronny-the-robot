#pragma once
#include "Grafer_data.h"
#include <windows.h>


/*
TODO v17:
* Bluetooth till data:
-> lägga till cases för inkommande data. ---> CECK
-> hantera inkommande data.
* Utritning:
-> Skapa bitmap array med lämplig storlek
* Visning av sensorvärden på robot.
* Visa sensorvärden i tabell i nytt fönster.    --->TESTA
* Möjlighet att ändra reglerkonstanter.
* Grafer till sensorvärden.
*/

#define ABSOLUTEVALUE 0x30
#define AUTOMODE 0x31
#define MANUALMODE 0x32
#define DRIVABLE_SQUARE 0x33
#define WALL 0x34
#define DISTRESSEDFOUND 0x35
#define SENSOR_VALUES 0x40
#define WHEELENCODERS 0x41
#define TEJP_FOUND 0x42
#define TEJP_REF 0x70

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
	using namespace System::Threading;


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
			//CheckForIllegalCrossThreadCalls = false;
			InitializeComponent();
			findPorts();
			Console::WriteLine("in MyForm");
			//serialPort1->DataReceived += gcnew SerialDataReceivedEventHandler(DataReceivedHandler);

			/*testbitmap(image1, prevx, prevy, Color::White);
			testbitmap(image1, prevx, prevy - 15, Color::White);
			testbitmap(image1, prevx, prevy - 30, Color::White);
			testbitmap(image1, prevx, prevy - 45, Color::White);
			testbitmap(image1, prevx, prevy - 60, Color::White);
			testbitmap(image1, prevx, prevy - 75, Color::White);*/

			//changeIR(10, 2, 23, 6, 2);

			/*sensorwindow->sensorvalues(10, 10, 10, 10, 10);
			sensorwindow->sensorvalues(11, 11, 11, 11, 11);*/

			createarray(image1);
			fillkarta(image1, 16, 8, DRIVABLE_SQUARE);
			fillkarta(image1, 16, 9, WALL);
			fillkarta(image1, 16, 7, WALL);

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
	protected:
		static Bitmap^ image1 = gcnew Bitmap(600, 600);
		static int prevx = 450;
		static int prevy = 480;
		//static array < System::Byte >^ data_recieved = gcnew array < System::Byte >(16);
		static array < System::Byte >^ data_recieved_buffer = gcnew array < System::Byte >(16);

		static array <int, 2 >^ driveblesquares = gcnew array < int, 2 >(150, 2);

		static array <int, 2 >^ walls = gcnew array < int, 2 >(150, 2);
		static array <int, 2 >^ Karta = gcnew array < int, 2 >(17, 17);

		static int write_position = 0;
		static int expected_length = 0;
		static bool automode = false;
		static bool tejp_found = false;

		static Grafer_data^ sensorwindow = (gcnew Grafer_data());
		static bool showing_sensor_window = false;
		static Byte header = 0x00;
		static unsigned int current_xpos = 0;
		static unsigned int current_ypos = 0;
		static unsigned int current_angle = 0;
		static unsigned int drivablesquare_xpos = 0;
		static unsigned int drivablesquare_ypos = 0;
		static unsigned int distressedfound_xpos = 0;
		static unsigned int distressedfound_ypos = 0;
		static unsigned int wall_xpos = 0;
		static unsigned int wall_ypos = 0;
		static unsigned int tejp_ref_value = 0;
		static unsigned int rear_left = 0;
		static unsigned int front_left = 0;
		static unsigned int rear_right = 0;
		static unsigned int front_right = 0;
		static unsigned int front = 0;
		static unsigned int x_recieved_current = 16;
		static unsigned int y_recieved_current = 16;
		static unsigned int x_GUIcurrent = 16;
		static unsigned int y_GUIcurrent = 8;
		static int drivable_cell = 0;
		static int wall_cell = 0;

		static int squaresize = 35;

		static int stracka = 0;
		static int vinkel = 0;

		int w = 0;
		int d = 0;
		int en = 0;

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
	private: static System::Windows::Forms::PictureBox^  Ronny_robot;
	private: static System::Windows::Forms::Label^  Sensorvärden;
	private: static System::Windows::Forms::PictureBox^  Leftarrow_unpressed;
	private: static System::Windows::Forms::PictureBox^  Leftarrow_pressed;
	private: static System::Windows::Forms::PictureBox^  Uparrow_unpressed;
	private: static System::Windows::Forms::PictureBox^  Downarrow_unpressed;
	private: static System::Windows::Forms::PictureBox^  Rightarrow_unpressed;
	private: static System::Windows::Forms::PictureBox^  Uparrow_pressed;
	private: static System::Windows::Forms::PictureBox^  Downarrow_pressed;
	private: static System::Windows::Forms::PictureBox^  Rightarrow_pressed;
	
	public: static System::Windows::Forms::TextBox^  IRsensor_VF;
	public: static System::Windows::Forms::TextBox^  IRsensor_VB;
	public: static System::Windows::Forms::TextBox^  IRsensor_HF;
	public: static System::Windows::Forms::TextBox^  IRsensor_HB;
	public: static System::Windows::Forms::TextBox^  IRsensor_Front;

	private: static System::IO::Ports::SerialPort^  serialPort1;
	private: static System::Windows::Forms::Button^  button3;
	private: static System::Windows::Forms::Button^  button4;
	private: static System::Windows::Forms::TextBox^  Kommandon;
	private: static System::Windows::Forms::Button^  Sensordata;
	private: static System::Windows::Forms::Label^  label1;
	private: static System::Windows::Forms::ComboBox^  comboBox1;
	private: static System::Windows::Forms::Label^  label2;
	private: static System::Windows::Forms::TextBox^  open_closed;
	private: static System::Windows::Forms::PictureBox^  pictureBox1;
	private: static System::ComponentModel::IContainer^  components;

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
			this->IRsensor_VF->Text = L"0";
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
			this->IRsensor_VB->Text = L"0";
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
			this->IRsensor_HF->Text = L"0";
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
			this->IRsensor_HB->Text = L"0";
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
			this->IRsensor_Front->Text = L"0";
			// 
			// serialPort1
			// 
			this->serialPort1->BaudRate = 115200;
			this->serialPort1->PortName = L"COM3";
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &MyForm::serialPort1_DataReceived_1);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(42, 76);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(88, 28);
			this->button3->TabIndex = 15;
			this->button3->Text = L"Open";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click_1);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(136, 76);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(78, 28);
			this->button4->TabIndex = 16;
			this->button4->Text = L"Close";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click_1);
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
			this->Sensordata->Click += gcnew System::EventHandler(this, &MyForm::Sensordata_Click_1);
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
			this->pictureBox1->Location = System::Drawing::Point(321, 38);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(600, 600);
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



			 // Keypressevents
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

		if (!automode)
		{

			switch (e->KeyChar){

			case 'a':

				this->Kommandon->Text = "sväng vänster 1100 0011";
				this->serialPort1->Write(dataleft, 0, dataleft->Length);
				d = d - 1;
				//Console::WriteLine("d=" + d);
				break;

			case 's':

				this->Kommandon->Text = "bakåt 1100 0100";
				this->serialPort1->Write(databack, 0, databack->Length);
				w = w - 1;
				//Console::WriteLine("w=" + w);
				break;

			case 'w':

				this->Kommandon->Text = "framåt 1100 0001";
				this->serialPort1->Write(dataforward, 0, dataforward->Length);
				w = w + 1;
				//Console::WriteLine("w =" + w);
				break;

			case 'd':

				this->Kommandon->Text = "sväng höger1100 0010";
				this->serialPort1->Write(dataright, 0, dataright->Length);
				d = d + 1;
				//Console::WriteLine("d = " + d);

				break;

			case 'q':
				this->Kommandon->Text = "sväng vänster-fram 1100 0110 ";
				this->serialPort1->Write(dataforward, 0, dataforwardleft->Length);
				break;

			case 'e':
				this->Kommandon->Text = "sväng höger-fram 1100 0101";
				this->serialPort1->Write(datarforwardright, 0, datarforwardright->Length);
				en = en + 1;
				//Console::WriteLine("speed = " + en);
				break;
			default:
				this->Kommandon->Text = "fel styrkommando";
				break;
			}
		}
		else
		{
			this->Kommandon->Text = "Autonomt läge";
		}
	}

			 //Data recieved from serialport.

	private:  System::Void serialPort1_DataReceived_1(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
		Console::WriteLine("TOTAL BUFFER " + this->serialPort1->BytesToRead);
		if (write_position == 0){
		
			myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&myrecievedata);
			this->Invoke(d, gcnew array < Object^ > {serialPort1, 'h'});
		}
		else if (this->serialPort1->BytesToRead > expected_length - 1)
		{
			myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&myrecievedata);
			this->Invoke(d, gcnew array < Object^ > {serialPort1, 'b'});
		}
		else{
			return;
		}
	}

	private: delegate void myrecievedata_delegate(SerialPort^ sender, char status);
	private: static void myrecievedata(SerialPort^ sender, char status){
		if (status == 'h'){
			sender->BaseStream->ReadAsync(data_recieved_buffer, 0, 1);
			handleheader(data_recieved_buffer[0]);
		}
		else if (status == 'b'){
			sender->BaseStream->ReadAsync(data_recieved_buffer, 0, 1);
			handlebyte();
		}
	}

	private: delegate void hanldebyte(unsigned char byte);

	private: static void handleheader(unsigned char byte){

		if (write_position == 0)  //If readposition = 0 we have a header. 
		{
			switch (byte)
			{
			case ABSOLUTEVALUE: //Absolutevalue x,y (position)
				expected_length = 4;
				header = byte;
				++write_position;
				break;
			case AUTOMODE: //Auto mode
				write_position = 0;
				automode = true;
				break;
			case MANUALMODE: //Manual mode
				write_position = 0;
				automode = false;
				break;
			case DRIVABLE_SQUARE: //drivalbe square
				expected_length = 3;
				header = byte;
				++write_position;
				break;
			case DISTRESSEDFOUND: //Distressed found
				expected_length = 3;
				header = byte;
				break;
			case SENSOR_VALUES: //Sensorvalues
				expected_length = 11;
				header = byte;
				++write_position;
				break;
			case WALL:
				expected_length = 3;
				header = byte;
				++write_position;
				break;
			case WHEELENCODERS://Wheelencodervärden
				expected_length = 3;
				header = byte;
				++write_position;
				break;
			case TEJP_FOUND: //Tejpbit funnen.
				write_position = 0;
				tejp_found = true;
				break;
			case TEJP_REF: //Referensvärde tejp
				expected_length = 2;
				header = byte;
				++write_position;
				break;
			default:
				break;
			}
			return;
		}
	}

	private: static void handlebyte(){

		//this->serialPort1->Read(data_recieved_buffer, 0, expected_length - 2);

		switch (header)

		{
		case ABSOLUTEVALUE: //Absolutvärde x,y (alltså position)
			current_xpos = data_recieved_buffer[0];
			current_ypos = data_recieved_buffer[1];
			current_angle = data_recieved_buffer[2];
			fillkarta(image1, current_xpos, current_ypos, ABSOLUTEVALUE);
			break;

		case DRIVABLE_SQUARE: //Körbar ruta x,y
			drivablesquare_xpos = data_recieved_buffer[0];
			drivablesquare_ypos = data_recieved_buffer[1];
			change_coordinates(drivablesquare_xpos, drivablesquare_ypos);
			driveblesquares[drivable_cell, 0] = x_GUIcurrent; // Vi hade ju tänkt helt fel, det är ju våra koord på våran grid som ska sparas
			driveblesquares[drivable_cell, 1] = y_GUIcurrent;
			move_squares(x_GUIcurrent, y_GUIcurrent); // ändrar 
			fillkarta(image1, x_GUIcurrent, y_GUIcurrent, DRIVABLE_SQUARE); // eller skulle vi byta plats på x o y när vi skickar in i fillkartan här eller fixade funktionen switchen?! så att de ritas ut rätt
			++drivable_cell;
			break;
		case DISTRESSEDFOUND: //Nödställd funnen
			distressedfound_xpos = data_recieved_buffer[0];
			distressedfound_ypos = data_recieved_buffer[1];
			fillkarta(image1, distressedfound_xpos, distressedfound_ypos, DISTRESSEDFOUND);
			break;

		case WALL:
			wall_xpos = data_recieved_buffer[0];
			wall_ypos = data_recieved_buffer[1];
			change_coordinates(wall_xpos, wall_ypos);// måste tänka om med walls!! walls dyker ju på sidan av körbarruta! på båda sidor
			walls[wall_cell, 0] = wall_xpos; //kom ihåg att ändra vilka koord som ska läggas in, inte den mottagna positionen utan våra koord!! 
			walls[wall_cell, 1] = wall_ypos;
			move_squares(x_GUIcurrent, y_GUIcurrent);
			fillkarta(image1, x_GUIcurrent, y_GUIcurrent, WALL);
			++wall_cell;
			break;
		case SENSOR_VALUES:  //Dealing with sensorvalues
		{
			//Console::WriteLine("HANDLING RECIEVED DATA!!");
			rear_left = data_recieved_buffer[0];
			rear_left = rear_left << 8;
			rear_left |= data_recieved_buffer[1];

			front_left = data_recieved_buffer[2] << 8;
			front_left |= data_recieved_buffer[3];

			rear_right = data_recieved_buffer[4] << 8;
			rear_right |= data_recieved_buffer[5];

			front_right = data_recieved_buffer[6] << 8;
			front_right |= data_recieved_buffer[7];

			front = data_recieved_buffer[8] << 8;
			front |= data_recieved_buffer[9];

			//Console::WriteLine("buffer" + this->serialPort1->BytesToRead);
			//Console::WriteLine("Leftside: " + rear_left + ", " + front_left + " " + "right side: " + rear_right + ", " + front_right + " front" + front);
			SetText(Convert::ToString(front), IRsensor_Front);
			SetText(Convert::ToString(front_left), IRsensor_VF);
			SetText(Convert::ToString(front_right), IRsensor_HF);
			SetText(Convert::ToString(rear_left), IRsensor_VB);
			SetText(Convert::ToString(rear_right), IRsensor_HB);
			//Console::
		}
			break;
		case WHEELENCODERS:
			break;

		case TEJP_FOUND: //Tejpbit funnen.
			break;
		case TEJP_REF: //Referensvärde tejp
			tejp_ref_value = data_recieved_buffer[0];
			break;

		default:
			break;
		}

		//if (showing_sensor_window){
		//this->sensorvalues(front, front_right, rear_right, rear_left, front_left);

		/*if (this->serialPort1->BytesToRead > 15)
		{
		this->serialPort1->DiscardInBuffer();
		}*/
		write_position = 0;
		expected_length = 0;

	}


			 //Invoke(gcnew EventHandler(this, MyForm::Read_data));

			 /*private: void wheelvalues(System::Byte^ byte1, System::Byte^ byte2){

				 Grafer_data^ grafer = gcnew Grafer_data();
				 grafer->dataGridView1->Rows->Add(byte1, byte2);
				 //this->dataGridView1->Rows->Add(byte1, byte2);*/

	private: void sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){
		Grafer_data^ grafer = gcnew Grafer_data();
		grafer->dataGridView1->Rows[0]->Cells[2]->Value = byte1;
		grafer->dataGridView1->Rows[0]->Cells[3]->Value = byte2;
		grafer->dataGridView1->Rows[0]->Cells[4]->Value = byte3;
		grafer->dataGridView1->Rows[0]->Cells[5]->Value = byte4;
		grafer->dataGridView1->Rows[0]->Cells[6]->Value = byte5;
		grafer->dataGridView1->Rows->Add();
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

	private: System::Void Sensordata_Click_1(System::Object^  sender, System::EventArgs^  e) {
		showing_sensor_window = true;
		sensorwindow->Show();
		//iform->sensorvalues(front, front_left, front_right, rear_left, rear_right);

	}

	private: System::Void changeIR(unsigned int front, unsigned int front_left, unsigned int front_right, unsigned int rear_left, unsigned int rear_right){

		front = front * 10 ^ -2;
		String^ s_front = Convert::ToString(front);
		this->IRsensor_Front->Text = s_front;

		//front_left = front_left * 10 ^ -2;
		String^ s_front_left = Convert::ToString(front_left);
		this->IRsensor_VF->Text = s_front_left;

		//front_right = front_right * 10 ^ -2;
		String^ s_front_right = Convert::ToString(front_right);
		this->IRsensor_HF->Text = s_front_right;

		//rear_left = rear_left * 10 ^ -2;
		String^ s_rear_left = Convert::ToString(rear_left);
		this->IRsensor_VB->Text = s_rear_left;

		//rear_right = rear_right * 10 ^ -2;
		String^ s_rear_right = Convert::ToString(rear_left);
		this->IRsensor_HB->Text = s_rear_right;


	}

	private: delegate void SetTextDelegate(String^ text, TextBox^ textbox);

	private: static void SetText(String^ text, TextBox^ textbox){
		if (textbox->InvokeRequired){
			SetTextDelegate^ d = gcnew SetTextDelegate(&MyForm::SetText);
			textbox->Invoke(d, gcnew array < Object^ > { text, textbox });
		}
		else {
			textbox->Text = text;
		}
	}

	private: static void createarray(Bitmap^ image1){

		//array < array<int, 1>^, 2 >^ Karta = gcnew array < array<int, 1>^, 2 >(32, 32);
		//array < int >^ KartaX = gcnew array < int >(32);


		for (int y = 0; y < 17; y++)
		{
			for (int x = 0; x < 17; x++)
			{
				Karta[x, y] = 254;
			}
		}


		for (int yp = 0; yp < pictureBox1->Height; yp++)
		{
			for (int xp = 0; xp < pictureBox1->Width; xp++)
			{
				if (xp % squaresize == 0)
				{
					image1->SetPixel(xp, yp, Color::Black);
				}
				else if (yp % squaresize == 0)
				{
					image1->SetPixel(xp, yp, Color::Black);
				}
				else
				{
					image1->SetPixel(xp, yp, Color::Gray);
				}

			}
		}
		pictureBox1->Image = image1;
	}


	private: static void change_coordinates(int unsigned newrecieved_x, unsigned int newrecieved_y){
		if (newrecieved_x == x_recieved_current && newrecieved_y < y_recieved_current){
			--y_recieved_current;
			--y_GUIcurrent;
		}
		else if (newrecieved_x == x_recieved_current && newrecieved_y > y_recieved_current) {
			++y_recieved_current;
			++y_GUIcurrent;
		}
		else if (newrecieved_y == y_recieved_current && newrecieved_x < x_recieved_current){
			--x_recieved_current;
			--x_GUIcurrent;
		}
		else {
			++x_recieved_current;
			++x_GUIcurrent; // kan behöva köra else if på sista fallet för att utesluta/visa att det är de enda fallen vi får, antar nu att koord vi får
		}
	}
	private: static void move_squares(int unsigned x_new, unsigned int y_new){
		if (y_new < 1){
			for (int i = 0; i < drivable_cell; i++){
				++driveblesquares[i, 1]; // vill räkna upp alla y-värden i arrayen vilket ligger på [0,1], [1,1] osv gör jag rätt?
			}
			for (int i = 0; i < wall_cell; i++){
				++walls[i, 1]; // samma som ovan
			}
			++y_new;
			++y_recieved_current;
		}
		else if (y_new > 15){
			for (int i = 0; i < drivable_cell; i++){
				--driveblesquares[i, 1]; // om vi hamnar utanför på höger sida vill vi flytta alla rutor åt vänster.
			}
			for (int i = 0; i < wall_cell; i++){
				--walls[i, 1];
			}
			--y_new;
			--y_recieved_current;
		}
		else if (x_new > 15){
			for (int i = 0; i < drivable_cell; i++){
				--driveblesquares[i, 0];
			}
			for (int i = 0; i < wall_cell; i++){
				--walls[i, 0];
			}
			--x_new;
			--x_recieved_current;
		}
		else if (x_new < 1){
			for (int i = 0; i < drivable_cell; i++){
				++driveblesquares[i, 0];
			}
			for (int i = 0; i < wall_cell; i++){
				++walls[i, 0];
			}
			++x_new;
			++x_recieved_current;
		}
		else{
			return;
		}
	}


	public: static void fillkarta(Bitmap^ Karta, int x_ny, int y_ny, int status){

		switch (status)
		{
		case DRIVABLE_SQUARE:

			//if (drivablesquare_xpos )


			for (int x = squaresize * x_ny; x < squaresize * x_ny + squaresize; x++)
			{
				for (int y = squaresize * y_ny; y < squaresize * y_ny + squaresize; y++)
				{
					if (x % squaresize == 0)
					{
						image1->SetPixel(y, x, Color::Black);
					}
					else if (y % squaresize == 0)
					{
						image1->SetPixel(y, x, Color::Black);
					}
					else
					{
						image1->SetPixel(y, x, Color::White);
					}
				}
			}
			break;

		case WALL:
			for (int x = squaresize * x_ny; x < squaresize * x_ny + squaresize; x++)
			{
				for (int y = squaresize * y_ny; y < squaresize * y_ny + squaresize; y++)
				{
					image1->SetPixel(y, x, Color::Black);
				}
			}
			break;

		case DISTRESSEDFOUND:
			break;

		case ABSOLUTEVALUE:
			break;

		default:
			for (int x = squaresize * x_ny; x < squaresize * x_ny + squaresize; x++)
			{
				for (int y = squaresize * y_ny; y < squaresize * y_ny + squaresize; y++)
				{
					if (x % squaresize == 0)
					{
						image1->SetPixel(y, x, Color::Black);
					}
					else if (y % squaresize == 0)
					{
						image1->SetPixel(y, x, Color::Black);
					}
					else
					{
						image1->SetPixel(y, x, Color::Gray);
					}
				}
			}
			break;
		}
		pictureBox1->Image = image1;
	}
	private: System::Void button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
		auto data = gcnew array < System::Byte > { 12 };
		if (!this->serialPort1->IsOpen){

			this->serialPort1->PortName = this->comboBox1->Text;
			this->open_closed->Text = "port openeing, waiting";
			this->serialPort1->Open();
			this->open_closed->Text = "port open";
		}
	}
	private: System::Void button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
		if (this->serialPort1->IsOpen){
			this->serialPort1->Close();
			this->open_closed->Text = "connection closed";
		}
	}

			/* byte[] buffer = new byte[blockLimit];
			 Action kickoffRead = delegate { port.BaseStream.BeginRead(buffer, 0, buffer.Length, delegate(IAsyncResult ar) {
				 try {
					 int actualLength = port.BaseStream.EndRead(ar);
					 byte[] received = new byte[actualLength];
					 Buffer.BlockCopy(buffer, 0, received, 0, actualLength);
					 raiseAppSerialDataEvent(received);
				 }
				 catch (IOException exc) {
					 handleAppSerialError(exc);
				 }
				 kickoffRead();
			 }, null);
			 }
			 kickoffRead();
			 */





			 

	};
}
