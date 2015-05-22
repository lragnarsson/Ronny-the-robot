#include "MyForm.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace GUIronny;
[STAThread]

void main(array<String^>^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GUIronny::MyForm form;
	Application::Run(%form);

}

void MyForm::findPorts(void)
{
	// get port names
	array<Object^>^ objectArray = SerialPort::GetPortNames();
	// add string array to combobox
	this->comboBox1->Items->AddRange(objectArray);
}

//Keypressevents and clicks

System::Void MyForm::MyForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	/*Function to make the keys "light up" when pressed*/
	
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
System::Void MyForm::MyForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	/*Function to make the keys "light up" when pressed*/
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
System::Void MyForm::MyForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	/*Controles the steering, sending the correct headers to the robot*/
	
	auto dataleft = gcnew array < System::Byte > { 0xC3 };
	auto databack = gcnew array < System::Byte > { 0xC4 };
	auto dataforward = gcnew array < System::Byte > { 0xC1 };
	auto dataright = gcnew array < System::Byte > { 0xC2 };
	auto datarforwardright = gcnew array < System::Byte > { 0xC5 };
	auto dataforwardleft = gcnew array < System::Byte > { 0xC6 };

	if (!automode) //Steering is only possible if in manualmode!
	{

		switch (e->KeyChar){

		case 'a':
			this->Kommandon->Text = "turn left";
			this->serialPort1->Write(dataleft, 0, dataleft->Length);
			break;

		case 's':
			this->Kommandon->Text = "backwards";
			this->serialPort1->Write(databack, 0, databack->Length);
			break;

		case 'w':
			this->Kommandon->Text = "forward";
			this->serialPort1->Write(dataforward, 0, dataforward->Length);
			break;

		case 'd':
			this->Kommandon->Text = "turn forward";
			this->serialPort1->Write(dataright, 0, dataright->Length);
			break;

		case 'q':
			this->Kommandon->Text = "turn left-forward";
			this->serialPort1->Write(dataforward, 0, dataforwardleft->Length);
			break;

		case 'e':
			this->Kommandon->Text = "turn right-forward";
			this->serialPort1->Write(datarforwardright, 0, datarforwardright->Length);

			break;
		default:
			this->Kommandon->Text = "wrong command";
			break;
		}
	}
	else
	{
		this->Kommandon->Text = "Autonomous mode";
	}
}
System::Void MyForm::button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
	/*Opening the serialport when button pressed */
	auto data = gcnew array < System::Byte > { 12 };
	this->serialPort1->PortName = this->comboBox1->Text;
	this->open_closed->Text = "port openeing, waiting";
	this->serialPort1->Open();
	this->open_closed->Text = "port open";
}
System::Void MyForm::button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
	/* Closing connection if open when button pressed */
	if (this->serialPort1->IsOpen){
		this->serialPort1->Close();
		this->open_closed->Text = "connection closed";
	}
}
System::Void MyForm::Reset_Click(System::Object^  sender, System::EventArgs^  e) {
	/*reset the map and matrix with mapinfo*/
	createarray(image1);
	x_start = 0;
	y_start = 8;
}
System::Void MyForm::change_control_Click(System::Object^  sender, System::EventArgs^  e){
	/* sending given controlvalues to robot */
	if (!Kp_value->Text->Empty && !Ki_value->Text->Empty && !automode)
	{
	auto kpvalue = gcnew array < System::Byte > {};
	auto kivalue = gcnew array < System::Byte > {};
	serialPort1->Write(kpvalue, 0,  kpvalue->Length);
	serialPort1->Write(kivalue, 0, kivalue->Length);
	}
}
System::Void MyForm::calibration_Click(System::Object^  sender, System::EventArgs^  e){
	/*sends request to robot to calibrate the reflexsensor*/
	auto tejpref = gcnew array < System::Byte > { CALIBRATE_SENSORS };
	this->serialPort1->Write(tejpref, 0, 1);
}
System::Void MyForm::Reset_Map(){
	/*reset map*/
	createarray(image1);
}
System::Void MyForm::change_motor_Click(System::Object^  sender, System::EventArgs^  e){
	/*function to trim the motor*/
	if (motor->Text->Length == 0){
		motor->Text = "set motor-trim value";
	}
	else
	{
		auto motor_trim_value = gcnew array < System::Byte > { 0xC9,(System::Byte)Convert::ToSByte(motor->Text) };
		this->serialPort1->Write(motor_trim_value, 0, 2);
	}
}


//Serialport
System::Void MyForm::serialPort1_DataReceived_1(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {

	/*--------------------****serialport function*****--------------------------
	|	If there are any bytes to read, find how many and read all of them.		|
	|	call the myrecievedata function to handle the bytes and allow new		|
	|	events to be raised! (Invoke is a pointer to a function)				|
	---------------------------------------------------------------------------*/

	if (serialPort1->BytesToRead > 0){
		count = serialPort1->BytesToRead;
		serialPort1->Read(data_recieved_buffer, 0, count);
		myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&MyForm::myrecievedata);
		this->Invoke(d, gcnew array < Object^ > { 'h' });
	}
}


System::Void MyForm::myrecievedata(char status){
	/*------------------****my recieved data function*****----------------------
	|	The first byte is always a header. Call handle headerfunction to 		|
	|	see what header it is and how many bytes tp expect. Then if buffer		|
	|	consists of enough bytes to read all of them handlebyte and	count 		|
	|	up buffer index and buffer length. If not enough bytes mask them out	|
	---------------------------------------------------------------------------*/

	int bufferlength = count; //buffer length is the ammount of bytes collected
	while (!finished)
	{
		if (bufferlength > 0){ 
			handleheader(data_recieved_buffer[bufferindex]);
			if ((bufferlength >= expected_length) && !finished) //makes sure that we have enough bytes and that we
																//didn't recieve a header without any following bytes
			{
				handlebyte();
				bufferindex = bufferindex + expected_length;  //count up the index if not all bytes are handled
				bufferlength = bufferlength - expected_length; //take away the bytes just hanled.

				write_position = 0;  //reset writeposition and expected length.
				expected_length = 0;
			}
			else if (bufferlength < expected_length && bufferlength > 0 && !finished) //if not enough bytes in buffer
			{
				int index = 1; //help index for adding masked bytes into the original buffer without counting up the used buffer index
				while (bufferlength < expected_length)
				{
					if (serialPort1->BytesToRead > 0){ //mask out bytes if there are bytes to read
						serialPort1->Read(remaining_buffer, 0, 1);
						data_recieved_buffer[bufferindex + bufferlength] = remaining_buffer[0]; //put the masked byte into the buffer
						bufferlength = bufferlength + 1; //count up the bufferlength
						index++;
					}

				}
				handlebyte(); //handle the message
				bufferlength = bufferlength - expected_length;
				write_position = 0;
				expected_length = 0;
				bufferindex = 0;
				finished = true;
			}
			else 
			{
				write_position = 0;
				expected_length = 0;
				finished = true;
				bufferindex = 0;
			}
		}
		else //if buffer is 0 then finished
		{
			write_position = 0;
			expected_length = 0;
			finished = true;
			bufferindex = 0;
		}
	}
	finished = false;
}


System::Void MyForm::handleheader(unsigned char byte){

	if (write_position == 0)  //If readposition = 0 we have a header. 
	{
		switch (byte)
		{
		case ABSOLUTEVALUE: //Absolutevalue x,y (position)
			Console::WriteLine("Header: ABSOLUTEVALUE ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case AUTOMODE: //Auto mode
			write_position = 0;
			automode = true;
			finished = true;
			break;
		case MANUALMODE: //Manual mode
			write_position = 0;
			automode = false;
			finished = true;
			SetText("manualmode", Kommandon);
			break;
		case DRIVABLE_SQUARE: //drivalbe square
			Console::WriteLine("Header: DRIVALBE_SQUARE ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case DISTRESSEDFOUND: //Distressed found
			Console::WriteLine("Header: DISTRESSEDFOUND");
			expected_length = 3;
			header = byte;
			break;
		case SENSOR_VALUES: //Sensorvalues
			//Console::WriteLine("Header: SENSORVALUES ");
			expected_length = 11;
			header = byte;
			++write_position;
			break;
		case WALL:
			Console::WriteLine("Header: WALL ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case WHEELENCODERS://Wheelencodervärden
			Console::WriteLine("Header: WHEELENCODERS ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case TEJP_FOUND: //Tejpbit funnen.
			Console::WriteLine("TEJP FOUND");
			finished = true;
			/*map_squares[drivablesquare_xpos, drivablesquare_ypos] = '';
			move_grid(drivablesquare_xpos, drivablesquare_ypos);
			update_map();*/
			write_position = 0;
			tejp_found = true;
			break;
		case TEJP_REF: //Referensvärde tejp
			expected_length = 2;
			header = byte;
			++write_position;
			break;
		default:
			Console::WriteLine("Header: WIIIEEERRRDOOO HEADER!!!! ");
			finished = true;
			break;
		}
		return;
	}
}
System::Void MyForm::handlebyte(){

	switch (header)
	{
	case ABSOLUTEVALUE: //Absolutvärde x,y (alltså position)
		Console::WriteLine("ABSOLUTEVALUE PREV:  X " + current_xpos + "Y " + current_ypos);
		current_xpos = data_recieved_buffer[bufferindex + 1];
		current_ypos = data_recieved_buffer[bufferindex + 2];

		Console::WriteLine("ABSOLUTEVALUE:  X " + current_xpos + "Y " + current_ypos);
		//map_squares[current_xpos, current_ypos] = 'C';
		move_grid(current_xpos, current_ypos);
		update_map();
		current_robot = false;
		break;

	case DRIVABLE_SQUARE: //Körbar ruta x,y
		drivablesquare_xpos = data_recieved_buffer[bufferindex + 1];
		drivablesquare_ypos = data_recieved_buffer[bufferindex + 2];
		/*if (drivablesquare_xpos == current_xpos && drivablesquare_ypos == current_ypos){

		}*/
		//map_squares[drivablesquare_xpos, drivablesquare_ypos] = 'D';
		move_grid(drivablesquare_xpos, drivablesquare_ypos);
		update_map();
		Console::WriteLine("DRIVBLE_SQUARE: X-pos " + drivablesquare_xpos + "Y-pos " + drivablesquare_ypos);
		break;
	case DISTRESSEDFOUND: //Nödställd funnen
		distressedfound_xpos = data_recieved_buffer[bufferindex + 1];
		distressedfound_ypos = data_recieved_buffer[bufferindex + 2];
		map_squares[distressedfound_xpos, distressedfound_ypos] = 'T';
		move_grid(distressedfound_xpos, distressedfound_ypos);
		update_map();
		Console::WriteLine("DISTRESSEDFOUND: X-pos " + wall_xpos + "Y-pos " + wall_ypos);
		break;
	case WALL:
		wall_xpos = data_recieved_buffer[bufferindex + 1];
		wall_ypos = data_recieved_buffer[bufferindex + 2];
		map_squares[wall_xpos, wall_ypos] = 'W';
		move_grid(wall_xpos, wall_ypos);
		update_map();
		Console::WriteLine("WALL: X-pos " + wall_xpos + "Y-pos " + wall_ypos);
		break;

	case SENSOR_VALUES:  //Dealing with sensorvalues
	{
		rear_left = data_recieved_buffer[bufferindex + 1];
		rear_left = rear_left << 8;
		rear_left |= data_recieved_buffer[bufferindex + 2];

		front_left = data_recieved_buffer[bufferindex + 3] << 8;
		front_left |= data_recieved_buffer[bufferindex + 4];

		rear_right = data_recieved_buffer[bufferindex + 5] << 8;
		rear_right |= data_recieved_buffer[bufferindex + 6];

		front_right = data_recieved_buffer[bufferindex + 7] << 8;
		front_right |= data_recieved_buffer[bufferindex + 8];

		front = data_recieved_buffer[bufferindex + 9] << 8;
		front |= data_recieved_buffer[bufferindex + 10];

	//	Console::WriteLine(front + " " + front_left + " " + front_right + " " + rear_right + " " + rear_left);
		SetText(Convert::ToString(front), IRsensor_Front);
		SetText(Convert::ToString(front_left), IRsensor_VF);
		SetText(Convert::ToString(front_right), IRsensor_HF);
		SetText(Convert::ToString(rear_left), IRsensor_VB);
		SetText(Convert::ToString(rear_right), IRsensor_HB);

		if (showing_sensor_window)
		{
			sensorvalues(front, front_left, front_right, rear_right, rear_left);
		}

	}
		break;
	case WHEELENCODERS:
		added_distance = data_recieved_buffer[bufferindex + 1];
		current_distance = current_distance + added_distance;
		SetText(Convert::ToString(current_distance), totaldistance);
		break;

		//case TEJP_FOUND: //Tejpbit funnen.
		//Console::WriteLine("TEJP FOUND:  X " + current_xpos + "Y " + current_ypos);
		//break;
	case TEJP_REF: //Referensvärde tejp
		
		tejp_ref_value = data_recieved_buffer[bufferindex +1];
		SetText("tejpref =" + tejp_ref_value, Kommandon);
		break;

	default:
		break;
	}

	//write_position = 0;
	//expected_length = 0;

}

//using data from serialport
System::Void MyForm::changeIR(unsigned int front, unsigned int front_left, unsigned int front_right, unsigned int rear_left, unsigned int rear_right){

	String^ s_front = Convert::ToString(front);
	this->IRsensor_Front->Text = s_front;

	String^ s_front_left = Convert::ToString(front_left);
	this->IRsensor_VF->Text = s_front_left;

	String^ s_front_right = Convert::ToString(front_right);
	this->IRsensor_HF->Text = s_front_right;

	String^ s_rear_left = Convert::ToString(rear_left);
	this->IRsensor_VB->Text = s_rear_left;

	String^ s_rear_right = Convert::ToString(rear_left);
	this->IRsensor_HB->Text = s_rear_right;
}
System::Void MyForm::SetText(String^ text, TextBox^ textbox){
	if (textbox->InvokeRequired){
		SetTextDelegate^ d = gcnew SetTextDelegate(&MyForm::SetText);
		textbox->Invoke(d, gcnew array < Object^ > { text, textbox });
	}
	else {
		textbox->Text = text;
	}
}

//Map
System::Void MyForm::createarray(Bitmap^ image1){

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

System::Void MyForm::move_grid(unsigned int x_newrecieved, unsigned int y_newrecieved){
	if (x_newrecieved > x_start + 16){
		x_start = x_newrecieved - 16;
	}
	else if (y_newrecieved > y_start + 16){
		y_start = y_newrecieved - 16;
	}
	else if (y_newrecieved < y_start){
		y_start = y_newrecieved;
	}
	else if (x_newrecieved < x_start){
		Console::Write("Impossible coordinates" + x_newrecieved); // dels kommer vi inte kunna få koord mindre än 0 (från början) 
		//o har vi flyttat ned gridet kommer vi aldrig kunna komma utanför uppåt! 
	}
	else{
		return;
	}
}

System::Void MyForm::update_map(){
	if (drivablesquare_xpos == current_xpos && drivablesquare_ypos == current_ypos){
		map_squares[current_xpos, current_ypos] = 'C';
	}
	else {
		map_squares[drivablesquare_xpos, drivablesquare_ypos] = 'D';
	}
	for (int i = x_start; i < (x_start + 17); ++i){
		for (int j = y_start; j < (y_start + 17); ++j){			
			fillkarta(image1, (i - x_start), (j - y_start), map_squares[i, j]);

			pictureBox1->Image = image1;
		}
	}
}

System::Void MyForm::fillkarta(Bitmap^ Karta, int x_ny, int y_ny, char status){

	switch (status)
	{
	case 'D':
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

	case 'W':
		for (int x = squaresize * x_ny; x < squaresize * x_ny + squaresize; x++)
		{
			for (int y = squaresize * y_ny; y < squaresize * y_ny + squaresize; y++)
			{
				image1->SetPixel(y, x, Color::Black);
			}
		}
		break;

	case 'T':
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
					image1->SetPixel(y, x, Color::Red);
				}

			}
		}
		break;
	case 'C':
		for (int x = (squaresize)* x_ny; x < (squaresize)* x_ny + squaresize; x++)
		{
			for (int y = (squaresize)* y_ny; y < (squaresize)* y_ny + squaresize; y++)
			{
				current_robot = false;
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
					image1->SetPixel(y, x, Color::Turquoise);
				}

			}
		}

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
	Show_Map();
}
System::Void MyForm::Show_Map(){
	pictureBox1->Image = image1;
}

//Grafer_data
System::Void MyForm::sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){
	//Grafer_data^ grafer = gcnew Grafer_data();
	if (sensorwindow->dataGridView1->InvokeRequired){
		sensorvaluesDelegate^ d = gcnew sensorvaluesDelegate(&MyForm::sensorvalues);
		sensorwindow->dataGridView1->Invoke(d, gcnew array < Object^ > {byte1, byte2, byte3, byte4, byte5 });
	}
	else{
		sensorwindow->dataGridView1->Rows[0]->Cells[2]->Value = byte1;
		sensorwindow->dataGridView1->Rows[0]->Cells[3]->Value = byte2;
		sensorwindow->dataGridView1->Rows[0]->Cells[4]->Value = byte3;
		sensorwindow->dataGridView1->Rows[0]->Cells[5]->Value = byte4;
		sensorwindow->dataGridView1->Rows[0]->Cells[6]->Value = byte5;
		sensorwindow->dataGridView1->Rows->Add();
	}
}

