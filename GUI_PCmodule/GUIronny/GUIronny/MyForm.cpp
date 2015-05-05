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
System::Void MyForm::button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
	auto data = gcnew array < System::Byte > { 12 };
	if (!this->serialPort1->IsOpen){

		this->serialPort1->PortName = this->comboBox1->Text;
		this->open_closed->Text = "port openeing, waiting";
		this->serialPort1->Open();
		this->open_closed->Text = "port open";
	}
}
System::Void MyForm::button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
	if (this->serialPort1->IsOpen){
		this->serialPort1->Close();
		this->open_closed->Text = "connection closed";
	}
}
System::Void MyForm::Sensordata_Click_1(System::Object^  sender, System::EventArgs^  e) {
	showing_sensor_window = true;
	sensorwindow->Show();

}

//Serialport
System::Void MyForm::serialPort1_DataReceived_1(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
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
System::Void MyForm::myrecievedata(SerialPort^ sender, char status){
	if (status == 'h'){
		sender->BaseStream->ReadAsync(data_recieved_buffer, 0, 1);
		handleheader(data_recieved_buffer[0]);
	}
	else if (status == 'b'){
		sender->BaseStream->ReadAsync(data_recieved_buffer, 0, 1);
		handlebyte();
	}
}
System::Void MyForm::handleheader(unsigned char byte){

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
System::Void MyForm::handlebyte(){

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

	write_position = 0;
	expected_length = 0;

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
System::Void MyForm::change_coordinates(int unsigned newrecieved_x, unsigned int newrecieved_y){
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
System::Void MyForm::move_squares(int unsigned x_new, unsigned int y_new){
	if (y_new < 1){
		for (int i = 0; i != ++drivable_cell; i++){
			++driveblesquares[i, 1]; // vill räkna upp alla y-värden i arrayen vilket ligger på [0,1], [1,1] osv gör jag rätt?
		}
		for (int i = 0; i != ++wall_cell; i++){
			++walls[i, 1]; // samma som ovan
		}
		++y_new;
		++y_recieved_current;
	}
	else if (y_new > 15){
		for (int i = 0; i != ++drivable_cell; i++){
			--driveblesquares[i, 1]; // om vi hamnar utanför på höger sida vill vi flytta alla rutor åt vänster.
		}
		for (int i = 0; i != ++wall_cell; i++){
			--walls[i, 1];
		}
		--y_new;
		--y_recieved_current;
	}
	else if (x_new > 15){
		for (int i = 0; i != ++drivable_cell; i++){
			--driveblesquares[i, 0];
		}
		for (int i = 0; i != ++wall_cell; i++){
			--walls[i, 0];
		}
		--x_new;
		--x_recieved_current;
	}
	else{
		return;
	}
}
System::Void MyForm::fillkarta(Bitmap^ Karta, int x_ny, int y_ny, int status){

	switch (status)
	{
	case DRIVABLE_SQUARE:

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

//Grafer_data
System::Void MyForm::sensorvalues(int byte1, int byte2, int byte3, int byte4, int byte5){
	Grafer_data^ grafer = gcnew Grafer_data();
	grafer->dataGridView1->Rows[0]->Cells[2]->Value = byte1;
	grafer->dataGridView1->Rows[0]->Cells[3]->Value = byte2;
	grafer->dataGridView1->Rows[0]->Cells[4]->Value = byte3;
	grafer->dataGridView1->Rows[0]->Cells[5]->Value = byte4;
	grafer->dataGridView1->Rows[0]->Cells[6]->Value = byte5;
	grafer->dataGridView1->Rows->Add();
}