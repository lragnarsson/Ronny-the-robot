#pragma once
#include <windows.h>
//#include <stdafx.h>
//#pragma comment (lib, "th32.lib")

//#include <pplawait.h>


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
#define CALIBRATE_SENSORS 0xD0
#define P_PARAMETER 0xC7 // 2 bytes
#define D_PARAMETER 0xC8 // 2 bytes
#define MOTOR_TRIM 0xC9


namespace PC_unit_namespace {

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
	using namespace System::Collections::Generic;

	/// <summary>
	/// Summary for PC_unit
	/// </summary>
	public ref class PC_unit : public System::Windows::Forms::Form
	{
	public:
		PC_unit(void)
		{
			InitializeComponent();
			findPorts();

			createarray(image1);
		}


	protected:
		//Map components:
		static Bitmap^ image1 = gcnew Bitmap(600, 600);
		static array <int, 2 >^ Karta = gcnew array < int, 2 >(17, 17);
		static array <char, 2 >^ map_squares = gcnew array < char, 2>(33, 33);

		static unsigned int current_xpos = 16;
		static unsigned int current_ypos = 16;
		static unsigned int distressedfound_xpos = 0;
		static unsigned int distressedfound_ypos = 0;
		static unsigned int drivablesquare_xpos = 0;
		static unsigned int drivablesquare_ypos = 0;
		static unsigned int wall_xpos = 0;
		static unsigned int wall_ypos = 0;
		static Byte tejp_ref_value = 0;
		static unsigned int rear_left = 0;
		static unsigned int front_left = 0;
		static unsigned int rear_right = 0;
		static unsigned int front_right = 0;
		static unsigned int front = 0;
		static unsigned int x_start = 0;
		static unsigned int y_start = 8;
		static int squaresize = 35;

		bool Header = false;
		bool Recieved_all_bytes = false;
		static bool current_robot = false;
		static bool first_square = true;


		//Data received
		static array < System::Byte >^ data_recieved_buffer = gcnew array < System::Byte >(1000);
		static array < System::Byte >^ remaining_buffer = gcnew array < System::Byte >(1000);
		static int write_position = 0;
		static int expected_length = 0;
		static bool automode = false;
		static bool tejp_found = false;
		static unsigned char header = 0x00;
		static int current_distance = 0;
		static int added_distance = 0;
		static int count = 0;
		static bool finished = false;
		static int bufferindex = 0;
		static SByte added_distance_conveted = 0;

	private: static System::Windows::Forms::Button^  Reset;
	protected:
	private: static System::Windows::Forms::Label^  label3;
	private: static System::Windows::Forms::Label^  label4;
	private: static System::Windows::Forms::TextBox^  Kp_value;
	private: static System::Windows::Forms::Label^  label5;
	private: static System::Windows::Forms::TextBox^  Kd_value;
	private: static System::Windows::Forms::Button^  change_Kp;
	private: static System::Windows::Forms::Label^  label6;
	private: static System::Windows::Forms::TextBox^  totaldistance;
	private: static System::Windows::Forms::TextBox^  motor;
	private: static System::Windows::Forms::Label^  label7;
	private: static System::Windows::Forms::Button^  change_motor;
	private: static System::Windows::Forms::Button^  change_Kd;
	private: static System::Windows::Forms::Button^  calibration;
		 /// <summary>
		 /// Clean up any resources being used.
		 /// </summary>
		 ~PC_unit()
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
	private:
	public: static System::Windows::Forms::TextBox^  IRsensor_VB;
	public: static System::Windows::Forms::TextBox^  IRsensor_HF;
	public: static System::Windows::Forms::TextBox^  IRsensor_HB;
	public: static System::Windows::Forms::TextBox^  IRsensor_Front;
	private: static System::IO::Ports::SerialPort^  serialPort1;
	public:
	private: static System::Windows::Forms::Button^  open;
	private: static System::Windows::Forms::Button^  close;

	private: static System::Windows::Forms::TextBox^  Kommandon;
	private: static System::Windows::Forms::Label^  label1;
	private: static System::Windows::Forms::ComboBox^  comboBox1;
	private: static System::Windows::Forms::Label^  label2;
	private: static System::Windows::Forms::TextBox^  open_closed;
	private: static System::Windows::Forms::PictureBox^  pictureBox1;
	private: static System::ComponentModel::IContainer^  components;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(PC_unit::typeid));
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
			this->open = (gcnew System::Windows::Forms::Button());
			this->close = (gcnew System::Windows::Forms::Button());
			this->Kommandon = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->open_closed = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->Reset = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->Kp_value = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->Kd_value = (gcnew System::Windows::Forms::TextBox());
			this->change_Kp = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->totaldistance = (gcnew System::Windows::Forms::TextBox());
			this->calibration = (gcnew System::Windows::Forms::Button());
			this->motor = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->change_motor = (gcnew System::Windows::Forms::Button());
			this->change_Kd = (gcnew System::Windows::Forms::Button());
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
			this->Sensorvärden->Size = System::Drawing::Size(83, 13);
			this->Sensorvärden->TabIndex = 1;
			this->Sensorvärden->Text = L"Sensorvalues";
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
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &PC_unit::serialPort1_DataReceived_1);
			// 
			// open
			// 
			this->open->Location = System::Drawing::Point(42, 76);
			this->open->Name = L"open";
			this->open->Size = System::Drawing::Size(88, 28);
			this->open->TabIndex = 15;
			this->open->Text = L"Open";
			this->open->UseVisualStyleBackColor = true;
			this->open->Click += gcnew System::EventHandler(this, &PC_unit::open_Click);
			// 
			// close
			// 
			this->close->Location = System::Drawing::Point(136, 76);
			this->close->Name = L"close";
			this->close->Size = System::Drawing::Size(78, 28);
			this->close->TabIndex = 16;
			this->close->Text = L"Close";
			this->close->UseVisualStyleBackColor = true;
			this->close->Click += gcnew System::EventHandler(this, &PC_unit::close_Click);
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
			// Reset
			// 
			this->Reset->Location = System::Drawing::Point(969, 38);
			this->Reset->Name = L"Reset";
			this->Reset->Size = System::Drawing::Size(81, 38);
			this->Reset->TabIndex = 24;
			this->Reset->Text = L"Reset Map";
			this->Reset->UseVisualStyleBackColor = true;
			this->Reset->Click += gcnew System::EventHandler(this, &PC_unit::Reset_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(956, 100);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(95, 13);
			this->label3->TabIndex = 25;
			this->label3->Text = L"Control parameters";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(952, 129);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(20, 13);
			this->label4->TabIndex = 26;
			this->label4->Text = L"Kp";
			// 
			// Kp_value
			// 
			this->Kp_value->Location = System::Drawing::Point(978, 126);
			this->Kp_value->Name = L"Kp_value";
			this->Kp_value->Size = System::Drawing::Size(56, 20);
			this->Kp_value->TabIndex = 27;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(956, 176);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(20, 13);
			this->label5->TabIndex = 28;
			this->label5->Text = L"Kd";
			// 
			// Kd_value
			// 
			this->Kd_value->Location = System::Drawing::Point(978, 173);
			this->Kd_value->Name = L"Kd_value";
			this->Kd_value->Size = System::Drawing::Size(56, 20);
			this->Kd_value->TabIndex = 29;
			// 
			// change_Kp
			// 
			this->change_Kp->Location = System::Drawing::Point(1054, 121);
			this->change_Kp->Name = L"change_Kp";
			this->change_Kp->Size = System::Drawing::Size(81, 25);
			this->change_Kp->TabIndex = 30;
			this->change_Kp->Text = L"Change";
			this->change_Kp->UseVisualStyleBackColor = true;
			this->change_Kp->Click += gcnew System::EventHandler(this, &PC_unit::change_Kp_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(19, 427);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(70, 13);
			this->label6->TabIndex = 31;
			this->label6->Text = L"total distance";
			// 
			// totaldistance
			// 
			this->totaldistance->Enabled = false;
			this->totaldistance->Location = System::Drawing::Point(95, 424);
			this->totaldistance->Name = L"totaldistance";
			this->totaldistance->ReadOnly = true;
			this->totaldistance->Size = System::Drawing::Size(114, 20);
			this->totaldistance->TabIndex = 32;
			// 
			// calibration
			// 
			this->calibration->Location = System::Drawing::Point(955, 380);
			this->calibration->Name = L"calibration";
			this->calibration->Size = System::Drawing::Size(97, 40);
			this->calibration->TabIndex = 33;
			this->calibration->Text = L"Calibrate Tejpreference";
			this->calibration->UseVisualStyleBackColor = true;
			this->calibration->Click += gcnew System::EventHandler(this, &PC_unit::calibration_Click);
			// 
			// motor
			// 
			this->motor->Location = System::Drawing::Point(978, 263);
			this->motor->Name = L"motor";
			this->motor->Size = System::Drawing::Size(56, 20);
			this->motor->TabIndex = 34;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(975, 247);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(57, 13);
			this->label7->TabIndex = 35;
			this->label7->Text = L"Motor Trim";
			// 
			// change_motor
			// 
			this->change_motor->Location = System::Drawing::Point(974, 289);
			this->change_motor->Name = L"change_motor";
			this->change_motor->Size = System::Drawing::Size(66, 25);
			this->change_motor->TabIndex = 36;
			this->change_motor->Text = L"Change";
			this->change_motor->UseVisualStyleBackColor = true;
			this->change_motor->Click += gcnew System::EventHandler(this, &PC_unit::change_motor_Click);
			// 
			// change_Kd
			// 
			this->change_Kd->Location = System::Drawing::Point(1054, 169);
			this->change_Kd->Name = L"change_Kd";
			this->change_Kd->Size = System::Drawing::Size(81, 24);
			this->change_Kd->TabIndex = 37;
			this->change_Kd->Text = L"Change ";
			this->change_Kd->UseVisualStyleBackColor = true;
			this->change_Kd->Click += gcnew System::EventHandler(this, &PC_unit::change_Kd_Click);
			// 
			// PC_unit
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1147, 652);
			this->Controls->Add(this->change_Kd);
			this->Controls->Add(this->change_motor);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->motor);
			this->Controls->Add(this->calibration);
			this->Controls->Add(this->totaldistance);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->change_Kp);
			this->Controls->Add(this->Kd_value);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->Kp_value);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->Reset);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->open_closed);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Kommandon);
			this->Controls->Add(this->close);
			this->Controls->Add(this->open);
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
			this->Name = L"PC_unit";
			this->Text = L"GUI Ronny";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &PC_unit::PC_unit_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &PC_unit::PC_unit_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &PC_unit::PC_unit_KeyUp);
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

			 // find available ports
	private: void findPorts(void);

			 // Keypressevents & click
	private: System::Void PC_unit_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void PC_unit_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void PC_unit_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void open_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void close_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Reset_Click(System::Object^  sender, System::EventArgs^  e);
			 //Data recieved from serialport.
	private: delegate void recievedata_delegate(char status);
	private: delegate void hanldebyte(unsigned char byte);
	private: System::Void serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e);
	private: static void recievedata(char status);
	private: static void handleheader(unsigned char byte);
	private: static void handlebyte();

			 //using data from serialport
	private: delegate void SetTextDelegate(String^ text, TextBox^ textbox);
	private: static void SetText(String^ text, TextBox^ textbox);

			 //Map
	private: static void createarray(Bitmap^ image1);
	private: static void move_grid(unsigned int x_newrecieved, unsigned int y_newrecieved);
	private: static void update_map();
	private: static void fillkarta(Bitmap^ Karta, int x_ny, int y_ny, char status);
	private: static void Show_Map();

private: System::Void calibration_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void change_motor_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void change_Kp_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void change_Kd_Click(System::Object^  sender, System::EventArgs^  e);
};

}
