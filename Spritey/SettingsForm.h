#pragma once
#pragma managed(push, off)
#include "SpriteyData.h"
#pragma managed(pop)

namespace Spritey {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace SpriteyData;

	/// <summary>
	/// Summary for SettingsForm
	/// </summary>
	public ref class SettingsForm : public System::Windows::Forms::Form
	{
			 
	public:
		SpriteySettings* currentSetLocCopy;//A local copy of our current settings.

		SettingsForm(SpriteySettings* currentSettings)
		{
			InitializeComponent();
			
			currentSetLocCopy = new SpriteySettings(*currentSettings); //take a copy of our current settings
			//initialise the elements on our form to the values stored in the SpriteySettings
			this->anchorDevCheckBox->Checked = currentSetLocCopy->isAnchorDevAllowed();
		}

		

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettingsForm()
		{
			if (components)
			{
				delete components;
				
			}
			if(currentSetLocCopy)
			{
				delete currentSetLocCopy;
			}
		}
	private: System::Windows::Forms::Button^  CancelButton;
	private: System::Windows::Forms::Button^  ApplyButton;
	private: System::Windows::Forms::GroupBox^  editorSettingsGroup;
	private: System::Windows::Forms::CheckBox^  anchorDevCheckBox;
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
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->ApplyButton = (gcnew System::Windows::Forms::Button());
			this->editorSettingsGroup = (gcnew System::Windows::Forms::GroupBox());
			this->anchorDevCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->editorSettingsGroup->SuspendLayout();
			this->SuspendLayout();
			// 
			// CancelButton
			// 
			this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->CancelButton->Location = System::Drawing::Point(259, 155);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(75, 23);
			this->CancelButton->TabIndex = 0;
			this->CancelButton->Text = L"Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			this->CancelButton->Click += gcnew System::EventHandler(this, &SettingsForm::CancelButton_Click);
			// 
			// ApplyButton
			// 
			this->ApplyButton->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->ApplyButton->Location = System::Drawing::Point(340, 155);
			this->ApplyButton->Name = L"ApplyButton";
			this->ApplyButton->Size = System::Drawing::Size(75, 23);
			this->ApplyButton->TabIndex = 1;
			this->ApplyButton->Text = L"Apply";
			this->ApplyButton->UseVisualStyleBackColor = true;
			this->ApplyButton->Click += gcnew System::EventHandler(this, &SettingsForm::ApplyButton_Click);
			// 
			// editorSettingsGroup
			// 
			this->editorSettingsGroup->Controls->Add(this->anchorDevCheckBox);
			this->editorSettingsGroup->Location = System::Drawing::Point(12, 12);
			this->editorSettingsGroup->Name = L"editorSettingsGroup";
			this->editorSettingsGroup->Size = System::Drawing::Size(403, 137);
			this->editorSettingsGroup->TabIndex = 2;
			this->editorSettingsGroup->TabStop = false;
			this->editorSettingsGroup->Text = L"Editor Settings";
			// 
			// anchorDevCheckBox
			// 
			this->anchorDevCheckBox->AutoSize = true;
			this->anchorDevCheckBox->Location = System::Drawing::Point(7, 16);
			this->anchorDevCheckBox->Name = L"anchorDevCheckBox";
			this->anchorDevCheckBox->Size = System::Drawing::Size(133, 17);
			this->anchorDevCheckBox->TabIndex = 1;
			this->anchorDevCheckBox->Text = L"Allow anchor deviation";
			this->anchorDevCheckBox->UseVisualStyleBackColor = true;
			this->anchorDevCheckBox->CheckedChanged += gcnew System::EventHandler(this, &SettingsForm::anchorDevCheckBox_CheckedChanged);
			// 
			// SettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(427, 185);
			this->Controls->Add(this->editorSettingsGroup);
			this->Controls->Add(this->ApplyButton);
			this->Controls->Add(this->CancelButton);
			this->Name = L"SettingsForm";
			this->Text = L"Settings";
			this->editorSettingsGroup->ResumeLayout(false);
			this->editorSettingsGroup->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	

private: System::Void anchorDevCheckBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //when there's a change in the checkbox, simply copy that bool value over to our curSetLocCopy
			 currentSetLocCopy->setAnchorDevAllowed(anchorDevCheckBox->Checked);
		 }
private: System::Void ApplyButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //The button already sets the dialogueresult to OK
			 //So we just hide the form
			 this->Hide();
		 }

private: System::Void CancelButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //The button already sets the dialogueresult to CANCEL
			 //So we just hide the form
			 this->Hide();
		 }
};
}
