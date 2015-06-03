/*
 * PC_unit.cpp
 * Ronny-the-robot/PC_unit
 * ------------------------------
 * This file contains definitions for the PC_unit's functions and windowapplications.
 * ------------------------------
 * Author: M. Östlund Visén, E. Söderström
 */

#include "PC_unit.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace PC_unit_namespace;
[STAThread]

void main(array<String^>^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PC_unit_namespace::PC_unit form;
	Application::Run(%form);

}

/* 
 * Finds existing COM-ports.
 */
void PC_unit::findPorts(void)
{
	array<Object^>^ objectArray = SerialPort::GetPortNames();
	this->comboBox1->Items->AddRange(objectArray);
}

/*
 * Function to show original arrowkeys on window after a keypress.
 */
System::Void PC_unit::PC_unit_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {

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

/*
 * Function to mark the arrowkeys while pressed.
 */
System::Void PC_unit::PC_unit_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
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

/*
 * Function to send steeringcommands to the robot.
 */
System::Void PC_unit::PC_unit_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	auto dataleft = gcnew array < System::Byte > { 0xC3 }; // Sending the correct headers to the robot.
	auto databack = gcnew array < System::Byte > { 0xC4 };
	auto dataforward = gcnew array < System::Byte > { 0xC1 };
	auto dataright = gcnew array < System::Byte > { 0xC2 };
	auto datarforwardright = gcnew array < System::Byte > { 0xC5 };
	auto dataforwardleft = gcnew array < System::Byte > { 0xC6 }; 

	if (!automode) // Steering is only possible in manualmode.
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

/*
 * Opens the serialport when button is clicked. 
 */
System::Void PC_unit::open_Click(System::Object^  sender, System::EventArgs^  e) {
	auto data = gcnew array < System::Byte > { 12 };
	this->serialPort1->PortName = this->comboBox1->Text;
	this->open_closed->Text = "port openeing, waiting";
	this->serialPort1->Open();
	this->open_closed->Text = "port open";
}

/*
 * Closes the serialport when button is clicked.
 */
System::Void PC_unit::close_Click(System::Object^  sender, System::EventArgs^  e) {
	if (this->serialPort1->IsOpen){
		this->serialPort1->Close();
		this->open_closed->Text = "connection closed";
	}
}

/*
 * Resets the map and the matrix with mapinfo.
 */
System::Void PC_unit::Reset_Click(System::Object^  sender, System::EventArgs^  e) {
	createarray(image1);
	x_start = 0;
	y_start = 8;
	for (int i = 0; i < 33; ++i){
		for (int j = 0; j < 33; ++j){
			map_squares[i, j] = 0;
		}
	}
}

/*
 * Sends a command to the robot to calibrate the reflexsensor.
 */
System::Void PC_unit::calibration_Click(System::Object^  sender, System::EventArgs^  e){
	auto tejpref = gcnew array < System::Byte > { CALIBRATE_SENSORS };
	this->serialPort1->Write(tejpref, 0, 1);
}

/*
 * Function to trim the motor.
 */
System::Void PC_unit::change_motor_Click(System::Object^  sender, System::EventArgs^  e){
	if (motor->Text->Length == 0){
		motor->Text = "set motor-trim value";
	}
	else
	{
		auto motor_trim_value = gcnew array < System::Byte > { 0xC9, (System::Byte)Convert::ToSByte(motor->Text) };
		this->serialPort1->Write(motor_trim_value, 0, 2);
	}
}

/*
 * Sends a command to the robot to change the Kp parameter.
 */
System::Void PC_unit::change_Kp_Click(System::Object^  sender, System::EventArgs^  e) {
	if (Kp_value->Text->Length != 0)
	{
		auto kpvalue = gcnew array < System::Byte > {P_PARAMETER, (Byte)Convert::ToSByte(Kp_value->Text)};
		serialPort1->Write(kpvalue, 0, kpvalue->Length);
	}

}

/*
 * Sends a command to the robot to change the Kd parameter.
 */
System::Void PC_unit::change_Kd_Click(System::Object^  sender, System::EventArgs^  e) {
	if (Kd_value->Text->Length != 0)
	{
		auto kdvalue = gcnew array < System::Byte > {D_PARAMETER, (Byte)Convert::ToSByte(Kd_value->Text)};
		serialPort1->Write(kdvalue, 0, kdvalue->Length);
	}
}

/*
 * SerialPort
 * If there are any bytes to read, find how many and read all of them.
 * call the receive_data function to handle the bytes and allow new
 * events to be raised! (Invoke is a pointer to a function).
 */
System::Void PC_unit::serialPort1_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
	if (serialPort1->BytesToRead > 0){
		count = serialPort1->BytesToRead;
		serialPort1->Read(data_recieved_buffer, 0, count);
		receive_data_delegate^ d = gcnew receive_data_delegate(&PC_unit::receive_data);
		this->Invoke(d, gcnew array < Object^ > { 'h' });
	}
}

/*
 * Function to handle incomming data.
 * The first byte is always a header. Call handle headerfunction to
 * see what header it is and how many bytes tp expect. Then if buffer
 * consists of enough bytes to read all of them handle_byte and	count
 * up buffer index and buffer length. If not enough bytes mask them out
 */
System::Void PC_unit::receive_data(char status){
	int bufferlength = count; // buffer length is the ammount of bytes collected
	while (!finished)
	{
		if (bufferlength > 0){ 
			handle_header(data_recieved_buffer[bufferindex]);
			if ((bufferlength >= expected_length) && !finished) // makes sure that we have enough bytes and that we
																// didn't recieve a header without any following bytes
			{
				handle_byte();
				bufferindex = bufferindex + expected_length;  // count up the index if not all bytes are handled
				bufferlength = bufferlength - expected_length; // take away the bytes just hanled.

				write_position = 0;  // reset writeposition and expected length.
				expected_length = 0;
			}
			else if (bufferlength < expected_length && bufferlength > 0 && !finished) // if not enough bytes in buffer
			{
				int index = 1; // help index for adding masked bytes into the original buffer without counting up the used buffer index
				while (bufferlength < expected_length)
				{
					if (serialPort1->BytesToRead > 0){ // mask out bytes if there are bytes to read
						serialPort1->Read(remaining_buffer, 0, 1);
						data_recieved_buffer[bufferindex + bufferlength] = remaining_buffer[0]; // put the masked byte into the buffer
						bufferlength = bufferlength + 1; // count up the bufferlength
						index++;
					}

				}
				handle_byte(); // handle the message
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
		else // if buffer is 0 then finished
		{
			write_position = 0;
			expected_length = 0;
			finished = true;
			bufferindex = 0;
		}
	}
	finished = false;
}

/*
* Function to handle incomming data.
* The first byte is always a header. Call handle headerfunction to
* see what header it is and how many bytes tp expect. Then if buffer
* consists of enough bytes to read all of them handle_byte and	count
* up buffer index and buffer length. If not enough bytes mask them out
*/
System::Void PC_unit::handle_header(unsigned char byte){
	if (write_position == 0)  // If readposition = 0 we have a header. 
	{
		switch (byte)
		{
		case ABSOLUTEVALUE: 
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case AUTOMODE: 
			SetText("Auto mode", Kommandon);
			write_position = 0;
			automode = true; 
			finished = true; 
			break;
		case MANUALMODE: 
			SetText("Manual mode", Kommandon);
			write_position = 0;
			automode = false;
			finished = true; 
			break;
		case DRIVABLE_SQUARE: 
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case DISTRESSEDFOUND: 
			expected_length = 3;
			header = byte;
			break;
		case SENSOR_VALUES: 
			expected_length = 11;
			header = byte;
			++write_position;
			break;
		case WALL:
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case WHEELENCODERS:
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case TEJP_FOUND: 
			finished = true; // only one byte->finished.
			write_position = 0;
			tejp_found = true;
			break;
		case TEJP_REF: 
			expected_length = 2;
			header = byte;
			++write_position;
			break;
		default:
			finished = true;
			break;
		}
		return;
	}
}

/*
 * This function handle the incomming bytes and calls the mapping functions.
 */
 System::Void PC_unit::handle_byte(){
	switch (header)
	{
	case ABSOLUTEVALUE: 
		current_xpos = data_recieved_buffer[bufferindex + 1];
		current_ypos = data_recieved_buffer[bufferindex + 2];
		move_grid(current_xpos, current_ypos);
		update_map();
		current_robot = false;
		Console::WriteLine("absolutevalue: X" + current_xpos + " Y " + current_ypos);
		break;
	case DRIVABLE_SQUARE: 
		drivablesquare_xpos = data_recieved_buffer[bufferindex + 1];
		drivablesquare_ypos = data_recieved_buffer[bufferindex + 2];
		Console::WriteLine("drivablesquare: X" + drivablesquare_xpos + " Y " + drivablesquare_ypos);
		move_grid(drivablesquare_xpos, drivablesquare_ypos);
		update_map();
		break;
	case DISTRESSEDFOUND: 
		distressedfound_xpos = data_recieved_buffer[bufferindex + 1];
		distressedfound_ypos = data_recieved_buffer[bufferindex + 2];
		distressedfound = true;
		Console::WriteLine("distressedfound: X" + distressedfound_xpos + " Y " + distressedfound_ypos);
		map_squares[distressedfound_xpos, distressedfound_ypos] = 'T';
		move_grid(distressedfound_xpos, distressedfound_ypos);
		update_map();
		break;
	case WALL:
		wall_xpos = data_recieved_buffer[bufferindex + 1];
		wall_ypos = data_recieved_buffer[bufferindex + 2];
		Console::WriteLine("wall: X" + wall_xpos + " Y " + wall_ypos);
		map_squares[wall_xpos, wall_ypos] = 'W';
		move_grid(wall_xpos, wall_ypos);
		update_map();
		break;
	case SENSOR_VALUES:  
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

		SetText(Convert::ToString(front), IRsensor_Front);
		SetText(Convert::ToString(front_left), IRsensor_VF);
		SetText(Convert::ToString(front_right), IRsensor_HF);
		SetText(Convert::ToString(rear_left), IRsensor_VB);
		SetText(Convert::ToString(rear_right), IRsensor_HB);

	}
		break;
	case WHEELENCODERS:
		added_distance = data_recieved_buffer[bufferindex + 1];
		added_distance_converted = (SByte)added_distance;
		current_distance = current_distance + (int)added_distance_converted;
		SetText(Convert::ToString(current_distance), totaldistance);
		break;
	case TEJP_REF: 
		tejp_ref_value = data_recieved_buffer[bufferindex +1];
		break;
	default:
		break;
	}
}

/*
 * Function to change text in a textbox.
 */
System::Void PC_unit::SetText(String^ text, TextBox^ textbox){
	if (textbox->InvokeRequired){
		SetTextDelegate^ d = gcnew SetTextDelegate(&PC_unit::SetText);
		textbox->Invoke(d, gcnew array < Object^ > { text, textbox });
	}
	else {
		textbox->Text = text;
	}
}

/*
 * Makes a 17x17 grid inside the picturebox (gray squares).
 */
System::Void PC_unit::createarray(Bitmap^ image1){

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

/*
 * This function checks if the receiving square's position is out of range 
 * for the 17x17 grid and changes the starcoordinate for the drawing of the grid,
 * if necessary.
 */
System::Void PC_unit::move_grid(unsigned int x_newrecieved, unsigned int y_newrecieved){
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
		Console::Write("Impossible coordinates" + x_newrecieved);  
	}
	else{
		return;
	}
}

/*
 * This function updates the map (with the robots currentposition) and fills the grid 
 * by calling fill_map for every square.
 */
System::Void PC_unit::update_map(){
	if (!distressedfound){
		if (drivablesquare_xpos == current_xpos && drivablesquare_ypos == current_ypos){
			map_squares[current_xpos, current_ypos] = 'C';
		}
		else {
			map_squares[drivablesquare_xpos, drivablesquare_ypos] = 'D';
		}
	}
	else {
		if (distressedfound_xpos == current_xpos && distressedfound_ypos == current_ypos ||
			distressedfound_xpos == drivablesquare_xpos && distressedfound_ypos == drivablesquare_ypos){
			map_squares[distressedfound_xpos, distressedfound_ypos] = 'T';
		}
		else if (drivablesquare_xpos == current_xpos && drivablesquare_ypos == current_ypos && 
			distressedfound_xpos != current_xpos && distressedfound_ypos != current_ypos){
			map_squares[current_xpos, current_ypos] = 'C';
		}
		else {
			map_squares[drivablesquare_xpos, drivablesquare_ypos] = 'D';
		}
	}
	for (int i = x_start; i < (x_start + 17); ++i){
		for (int j = y_start; j < (y_start + 17); ++j){			
			fill_map(image1, (i - x_start), (j - y_start), map_squares[i, j]);
			Show_Map();
		}
	}
}

/*
 * This function fills the square with the associated colour for the incomming square's status.
 */
System::Void PC_unit::fill_map(Bitmap^ Karta, int x_ny, int y_ny, char status){

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
}

/*
 * Show Map.
 */
System::Void PC_unit::Show_Map(){
	pictureBox1->Image = image1;
}

