#include "MyForm.h"
//#include <string>
using namespace System;
using namespace System::Windows::Forms;

//[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PC_module::MyForm form;
	Application::Run(%form);
}

