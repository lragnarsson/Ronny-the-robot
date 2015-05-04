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




