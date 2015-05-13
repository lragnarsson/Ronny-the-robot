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

			this->Kommandon->Text = "sv�ng v�nster 1100 0011";
			this->serialPort1->Write(dataleft, 0, dataleft->Length);
			d = d - 1;
			//Console::WriteLine("d=" + d);
			break;

		case 's':

			this->Kommandon->Text = "bak�t 1100 0100";
			this->serialPort1->Write(databack, 0, databack->Length);
			w = w - 1;
			//Console::WriteLine("w=" + w);
			break;

		case 'w':

			this->Kommandon->Text = "fram�t 1100 0001";
			this->serialPort1->Write(dataforward, 0, dataforward->Length);
			w = w + 1;
			//Console::WriteLine("w =" + w);
			break;

		case 'd':

			this->Kommandon->Text = "sv�ng h�ger1100 0010";
			this->serialPort1->Write(dataright, 0, dataright->Length);
			d = d + 1;
			//Console::WriteLine("d = " + d);

			break;

		case 'q':
			this->Kommandon->Text = "sv�ng v�nster-fram 1100 0110 ";
			this->serialPort1->Write(dataforward, 0, dataforwardleft->Length);
			break;

		case 'e':
			this->Kommandon->Text = "sv�ng h�ger-fram 1100 0101";
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
		this->Kommandon->Text = "Autonomt l�ge";
	}
}
System::Void MyForm::button3_Click_1(System::Object^  sender, System::EventArgs^  e) {
	auto data = gcnew array < System::Byte > { 12 };
	//if (!this->serialPort1->IsOpen){

	/*CSerial serial;
		int comport = Convert::ToInt32(comboBox1->Text);
		if (serial.Open(comport, 115200))
		open_closed->Text = "Port opened successfully";
		else
		open_closed->Text = "Failed to open port!";*/

	this->serialPort1->PortName = this->comboBox1->Text;
	this->open_closed->Text = "port openeing, waiting";
	this->serialPort1->Open();
	this->open_closed->Text = "port open";
}
//}
System::Void MyForm::button4_Click_1(System::Object^  sender, System::EventArgs^  e) {
	if (this->serialPort1->IsOpen){
		this->serialPort1->Close();
		this->open_closed->Text = "connection closed";
	}
}
System::Void MyForm::Sensordata_Click_1(System::Object^  sender, System::EventArgs^  e) {
	showing_sensor_window = true;
	sensorwindow->Show();
	sensorvalues(10, 10, 10, 10, 10);
}
System::Void MyForm::Reset_Click(System::Object^  sender, System::EventArgs^  e) {
	createarray(image1);
	x_start = 0;
	y_start = 8;
}
System::Void MyForm::change_control_Click(System::Object^  sender, System::EventArgs^  e){
	/*if (!Kp_value->Text->Empty && !Ki_value->Text->Empty && !automode)
	{
	auto kpvalue = gcnew array < System::Byte > {"some hex value" };
	auto kivalue = gcnew array < System::Byte > { "some hex value"};
	serialPort1->Write(kpvalue, 0,  kpvalue->Length);
	serialPort1->Write(kivalue, 0, kivalue->Length);
	}*/
}

System::Void MyForm::Reset_Map(){
	createarray(image1);
}



//Serialport
System::Void MyForm::serialPort1_DataReceived_1(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
	Console::WriteLine("EVENTRAISED");
	if (serialPort1->BytesToRead > 0){
		Console::WriteLine("--------------BEGIN ------------");
		Console::WriteLine("Initial buffer: " + serialPort1->BytesToRead);
		count = serialPort1->BytesToRead;
		int initial = count;
		int recievedcount = serialPort1->Read(data_recieved_buffer, 0, count);
			SetText("buffer: " + Convert::ToString(initial) + Convert::ToString(data_recieved_buffer[initial]), Kommandon);
		
		myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&MyForm::myrecievedata);
		this->Invoke(d, gcnew array < Object^ > { 'h' });
		Console::WriteLine("----------- END-----------");

		/*
		//handleheader(data_recieved_buffer[0]);
		Console::WriteLine("data that is recieved");
		serialPort1->Read(data_recieved_buffer, 0, 2);
		Console::WriteLine("X" + data_recieved_buffer[0] + "Y " + data_recieved_buffer[1]);
		Console::WriteLine("Remaining buffer: " + serialPort1->BytesToRead);
		write_position = 0;
		expected_length = 0;*/
	}
	/*if (write_position == 0){
		Console::WriteLine("Buffer header: " + serialPort1->BytesToRead);
		serialPort1->Read(data_recieved_buffer, 0, 1);
		header = data_recieved_buffer[0];
		if (this->InvokeRequired){
		myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&myrecievedata);
		this->Invoke(d, gcnew array < Object^ > {'h'});
		}
		else
		{
		myrecievedata('h');
		}
		}
		else if (this->serialPort1->BytesToRead > expected_length - 1)
		{
		//Console::WriteLine("Data " );
		Console::WriteLine("Buffer before DATA: " + serialPort1->BytesToRead);
		serialPort1->Read(data_recieved_buffer, 0, expected_length - 1);
		Console::WriteLine("Buffer after DATA: " + serialPort1->BytesToRead);
		//handlebyte();
		if (this->InvokeRequired){
		myrecievedata_delegate^ d = gcnew myrecievedata_delegate(&myrecievedata);
		this->Invoke(d, gcnew array < Object^ > {'b'});
		}
		else
		{
		myrecievedata('b');
		}
		}
		else{
		return;
		}
		}
		else
		return;*/
}


System::Void MyForm::myrecievedata(/*SerialPort^ sender,*/ char status){
	//if (status == 'h'){
	//serialPort1->BaseStream->ReadAsync(data_recieved_buffer, 0, 1); //
	//Console::WriteLine("header " + header);
	//Console::WriteLine("in my recievedata: " + status);
	/*switch (status)
	{
	case 'h':
	handleheader(header);
	break;
	case 'b':
	//handlebyte();
	write_position = 0;
	expected_length = 0;
	break;
	default:
	break;
	}*/
	//data_recieved_buffer[0];
	Console::WriteLine("invoke");
	int bufferlength = count;
	bool finished = false;
	//int bufferindex = 0;
	Console::WriteLine("----------- in while -> handling byte" + " " + bufferlength + "-----------");
	while (!finished)
	{
		if (bufferlength > 0){
			handleheader(data_recieved_buffer[bufferindex]);
			
			if ((bufferlength >= expected_length) && !finished)
			{
				handlebyte();
				bufferindex = bufferindex + expected_length;
				bufferlength =bufferlength - expected_length;
				
				write_position = 0;
				expected_length = 0;
			}
			else if (bufferlength < expected_length && bufferlength > 0 && !finished)
			{
				while (bufferlength < expected_length)
				{
					if (serialPort1->BytesToRead > 0){
						serialPort1->Read(remaining_buffer, 0, 1);
						bufferindex = bufferindex + 1;
						data_recieved_buffer[bufferindex] = remaining_buffer[0];
						bufferlength = bufferlength + 1;
						bufferindex = bufferindex + 1;
					}

				}


				//handlebyte();
				bufferlength = bufferlength - expected_length;

				write_position = 0;
				expected_length = 0;
			}
			else
			{
				int x = count - bufferlength;
				SetText("FEL : " + Convert::ToString(x), totaldistance);
				finished = true;
				bufferindex = 0;
			}
		}
		else
		{
			finished = true;
			bufferindex = 0;
		}
	}
	Console::WriteLine("----------- Finished While-----------");
}

//else if (status == 'b'){
//serialPort1->BaseStream->ReadAsync(data_recieved_buffer, 0, expected_length - 1);
//Console::WriteLine("buffer" + data_recieved_buffer[0] + data_recieved_buffer[1]);

System::Void MyForm::backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e){
	Console::WriteLine("backgroundworker doing work");
	BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
	/*for (int i = 0; i <= data_recieved_buffer->Length; i++)
	{
	Worker_buffer->Add(data_recieved_buffer[i]);
	}*/
	int bufferlength = data_recieved_buffer->Length;
	bool finished = false;
	int bufferindex = 0;
	while (!finished)
	{
		if (bufferlength > 0){
			handleheader(data_recieved_buffer[bufferindex]);
			if (bufferlength > expected_length)
			{
				handlebyte();
				bufferlength = expected_length - 1;
				bufferindex = bufferindex + expected_length;
			}
			else
			{
				finished = true;
				bufferindex = 0;
			}
		}
	}
	Console::WriteLine("backgroundworker FINISHED");
}
/*System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e){
	Console::WriteLine("RunWorkerCompleted...");
	}*/

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
			break;
		case MANUALMODE: //Manual mode
			write_position = 0;
			automode = false;
			break;
		case DRIVABLE_SQUARE: //drivalbe square
			Console::WriteLine("Header: DRIVALBE_SQUARE ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case DISTRESSEDFOUND: //Distressed found
			expected_length = 3;
			header = byte;
			break;
		case SENSOR_VALUES: //Sensorvalues
			Console::WriteLine("Header: SENSORVALUES ");
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
		case WHEELENCODERS://Wheelencoderv�rden
			Console::WriteLine("Header: WHEELENCODERS ");
			expected_length = 3;
			header = byte;
			++write_position;
			break;
		case TEJP_FOUND: //Tejpbit funnen.
			Console::WriteLine("TEJP FOUND");
			finished = true;
			write_position = 0;
			tejp_found = true;
			break;
		case TEJP_REF: //Referensv�rde tejp
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
	case ABSOLUTEVALUE: //Absolutv�rde x,y (allts� position)
		current_xpos = data_recieved_buffer[0];
		current_ypos = data_recieved_buffer[1];
		Console::WriteLine("ABSOLUTEVALUE:  X " + current_xpos + "Y " + current_ypos);
		//fillkarta(image1, current_xpos, current_ypos, ABSOLUTEVALUE);
		break;

	case DRIVABLE_SQUARE: //K�rbar ruta x,y
		Console::WriteLine("DRIVALBE_SQUARE:  X " + data_recieved_buffer[bufferindex + 1] + "Y " + data_recieved_buffer[bufferindex + 2]);
		drivablesquare_xpos = data_recieved_buffer[bufferindex + 1];
		drivablesquare_ypos = data_recieved_buffer[bufferindex + 2];
		map_squares[drivablesquare_xpos, drivablesquare_ypos] = 'D';
		move_grid(drivablesquare_xpos, drivablesquare_ypos);
		update_map();
		Console::WriteLine("DRIVBLE_SQUARE: X-pos " + drivablesquare_xpos + "Y-pos " + drivablesquare_ypos);
		//change_coordinates(drivablesquare_xpos, drivablesquare_ypos, DRIVABLE_SQUARE);
		//drivablesquares[drivable_cell, 0] = x_GUIcurrent; // Vi hade ju t�nkt helt fel, det �r ju v�ra koord p� v�ran grid som ska sparas
		//drivablesquares[drivable_cell, 1] = y_GUIcurrent;
		/*if (!first_square) {
			move_squares(x_GUIcurrent, y_GUIcurrent);
			}
			else
			{
			first_square = false;
			}*/
		//Console::WriteLine("GUI")
		//fillkarta(image1, x_GUIcurrent, y_GUIcurrent, DRIVABLE_SQUARE); // eller skulle vi byta plats p� x o y n�r vi skickar in i fillkartan h�r eller fixade funktionen switchen?! s� att de ritas ut r�tt
		//Show_Map();
		break;
	case DISTRESSEDFOUND: //N�dst�lld funnen
		distressedfound_xpos = data_recieved_buffer[0];
		distressedfound_ypos = data_recieved_buffer[1];
		map_squares[distressedfound_xpos, distressedfound_ypos] = 'T';
		move_grid(distressedfound_xpos, distressedfound_ypos);
		update_map();
		//change_coordinates(distressedfound_xpos, distressedfound_ypos, DISTRESSEDFOUND);
		//distressed[0, 0] = x_GUIcurrent;
		//distressed[0, 1] = y_GUIcurrent; // antar att vi bara kan f� information om en n�dst�lld (ett m�l)
		//move_squares(x_GUIcurrent, y_GUIcurrent); // beh�ver inte g�ra ett case f�r distressed d� de ska funka lika som drivables ( kolla banspec)
		//fillkarta(image1, x_GUIcurrent, y_GUIcurrent, DISTRESSEDFOUND);
		break;

	case WALL:
		wall_xpos = data_recieved_buffer[bufferindex + 1];
		wall_ypos = data_recieved_buffer[bufferindex + 2];
		map_squares[wall_xpos, wall_ypos] = 'W';
		move_grid(wall_xpos, wall_ypos);
		update_map();
		//change_coordinates(wall_xpos, wall_ypos, WALL);
		//walls[wall_cell, 0] = xpos_wall;
		//walls[wall_cell, 1] = ypos_wall;
		//move_squares(x_GUIcurrent, y_GUIcurrent);
		//fillkarta(image1, xpos_wall, ypos_wall, WALL);
		Console::WriteLine("WALL: X-pos " + wall_xpos + "Y-pos " + wall_ypos);
	
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

		Console::WriteLine(front + " " + front_left + " " + front_right + " " + rear_right + " " + rear_left);
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
		added_distance = data_recieved_buffer[0];
		current_distance = current_distance + added_distance;
		SetText(Convert::ToString(current_distance), totaldistance);
		break;

	//case TEJP_FOUND: //Tejpbit funnen.
		//Console::WriteLine("TEJP FOUND:  X " + current_xpos + "Y " + current_ypos);
		//fillkarta(image1, current_xpos, current_ypos, DISTRESSEDFOUND);
		//break;
	case TEJP_REF: //Referensv�rde tejp
		//tejp_ref_value = data_recieved_buffer[0];
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
/*System::Void MyForm::change_coordinates(int unsigned newrecieved_x, unsigned int newrecieved_y, int status){
	switch (status)
	{
	case DRIVABLE_SQUARE:
	if (first_square){//if first square!
	return;
	}
	else if (newrecieved_x == x_recieved_current && newrecieved_y < y_recieved_current){
	//--y_recieved_current;
	y_recieved_current = newrecieved_y;
	--y_GUIcurrent;
	}
	else if (newrecieved_x == x_recieved_current && newrecieved_y > y_recieved_current) {
	//++y_recieved_current;
	y_recieved_current = newrecieved_y;
	++y_GUIcurrent;
	}
	else if (newrecieved_y == y_recieved_current && newrecieved_x < x_recieved_current){
	x_recieved_current = newrecieved_x;
	--x_GUIcurrent;
	}

	else if (newrecieved_y == y_recieved_current && newrecieved_x > x_recieved_current)
	{
	//++x_recieved_current;
	x_recieved_current = newrecieved_x;
	++x_GUIcurrent; // kan beh�va k�ra else if p� sista fallet f�r att utesluta/visa att det �r de enda fallen vi f�r, antar nu att koord vi f�r
	}
	else
	{
	Console::WriteLine("wrong value ");
	}
	break;

	case WALL:
	Console::WriteLine("ChangeCoordinates: Wall");
	if (newrecieved_x == x_recieved_current && newrecieved_y < y_recieved_current){

	ypos_wall = y_GUIcurrent - 1;
	xpos_wall = x_GUIcurrent;
	}
	else if (newrecieved_x == x_recieved_current && newrecieved_y > y_recieved_current) {

	ypos_wall = y_GUIcurrent + 1;
	xpos_wall = x_GUIcurrent;
	}
	else if (newrecieved_y == y_recieved_current && newrecieved_x < x_recieved_current){

	ypos_wall = y_GUIcurrent;
	xpos_wall = x_GUIcurrent - 1;
	}
	else {

	ypos_wall = y_GUIcurrent;
	xpos_wall = x_GUIcurrent + 1; // kan beh�va k�ra else if p� sista fallet f�r att utesluta/visa att det �r de enda fallen vi f�r, antar nu att koord vi f�r
	}
	break;
	case DISTRESSEDFOUND:
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
	++x_GUIcurrent; // kan beh�va k�ra else if p� sista fallet f�r att utesluta/visa att det �r de enda fallen vi f�r, antar nu att koord vi f�r
	}
	break;
	default:
	break;
	}
	Console::WriteLine("Change coordinates: Current GUI X, Y " + x_GUIcurrent + " " + y_GUIcurrent);
	}*/
/*System::Void MyForm::move_squares(int unsigned x_new, unsigned int y_new){
	if (y_new < 1){
		Reset_Map();
		for (int i = 0; i < drivable_cell + 1; i++){
			int y = drivablesquares[i, 1];
			drivablesquares[i, 1] = y + 1; // vill r�kna upp alla y-v�rden i arrayen vilket ligger p� [0,1], [1,1] osv g�r jag r�tt?
			fillkarta(image1, drivablesquares[i, 0], drivablesquares[i, 1], DRIVABLE_SQUARE);
		}
		for (int i = 0; i < wall_cell; i++){
			int y = walls[i, 1];
			walls[i, 1] = y + 1; // samma som ovan
			fillkarta(image1, y_GUIcurrent, drivablesquares[i, 1], WALL);
		}

		++distressed[0, 1];
		++y_GUIcurrent;
		y_recieved_current = drivablesquare_ypos;
		//Console::WriteLine("MoveSquares: " + "X_gui_prev " + x_new + " X_gui_curr " + x_GUIcurrent);
		//Console::WriteLine("MoveSquares: " + "Y_gui_prev " + y_new + " Y_gui_curr " + y_GUIcurrent);
	}
	else if (y_new > 15){
		Reset_Map();
		for (int i = 0; i < drivable_cell; i++){
			int y = drivablesquares[i, 1]; // om vi hamnar utanf�r p� h�ger sida vill vi flytta alla rutor �t v�nster.
			drivablesquares[i, 1] = y - 1;
			fillkarta(image1, drivablesquares[i, 0], drivablesquares[i, 1], DRIVABLE_SQUARE);
		}
		for (int i = 0; i < wall_cell; i++){
			int y = walls[i, 1];
			walls[i, 1] = y - 1;
			fillkarta(image1, walls[i, 0], walls[i, 1], WALL);
		}
		--distressed[0, 1]; //r�kna ned y-v�rdet f�r den n�dst�llde (vill ju flytta den ocks�)
		--y_GUIcurrent;
		//--y_recieved_current;
	}
	else if (x_new > 15){
	Reset_Map();
	for (int i = 0; i < ++drivable_cell; i++){
	int x = drivablesquares[i, 0];
	drivablesquares[i, 0] = x - 1;
	fillkarta(image1, drivablesquares[i, 0], drivablesquares[i, 1], DRIVABLE_SQUARE);
	}
	for (int i = 0; i < wall_cell + 1; i++){
	int x = walls[i, 0];
	walls[i, 0] = x - 1;
	fillkarta(image1, walls[i, 0], walls[i, 1], WALL);
	}
	--distressed[0, 0];
	--x_GUIcurrent;
	//--x_recieved_current;
	}
	else{
	fillkarta(image1, x_GUIcurrent, y_GUIcurrent, DRIVABLE_SQUARE); // eller skulle vi byta plats p� x o y n�r vi skickar in i fillkartan h�r eller fixade funktionen switchen?! s� att de ritas ut r�tt
	return;
	}

	Show_Map();
	}*/

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
		Console::Write("Impossible coordinates" + x_newrecieved); // dels kommer vi inte kunna f� koord mindre �n 0 (fr�n b�rjan) 
		//o har vi flyttat ned gridet kommer vi aldrig kunna komma utanf�r upp�t! 
	}
	else{
		return;
	}
}

System::Void MyForm::update_map(){
	for (int i = x_start; i < (x_start + 17); ++i){
		for (int j = y_start; j < (y_start + 17); ++j){
			fillkarta(image1, (i - x_start), (j - y_start), map_squares[i,j]);
			
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
