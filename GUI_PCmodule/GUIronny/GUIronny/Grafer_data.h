#pragma once

namespace GUIronny {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Grafer_data
	/// </summary>
	public ref class Grafer_data : public System::Windows::Forms::Form
	{
	public:
		Grafer_data(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Grafer_data()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// Grafer_data
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Name = L"Grafer_data";
			this->Text = L"Grafer_data";
			this->Load += gcnew System::EventHandler(this, &Grafer_data::Grafer_data_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Grafer_data_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	};
}
