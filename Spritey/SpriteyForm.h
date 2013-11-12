#pragma once
#include "FViewer.h"
#include "SSViewer.h"
//#include <GL/glut.h> //<--- Causes errors. Why is this? Why do cli and glut hate eachother?
#include "dStructs.h"
#include <msclr/marshal_cppstd.h>
#include "SettingsForm.h"
#pragma managed(push, off)
#include<string>
#include "SpriteyData.h"
#include "dStructs.h"
#include <stdio.h>

#pragma managed(pop)

using namespace dStructs;
using namespace System::IO;

//default capture box sizes
const int defaultCBX = 100;
const int defaultCBY = 100;
const int SSVSizeX = 1024;
const int SSVSizeY = 700;
const int FVSizeX = 120;
const int FVSizeY = 120;
const int FVPosX = 1049;
const int FVPosY = 607;


namespace Spritey {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;
	using namespace SpriteyData;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class SpriteyForm : public System::Windows::Forms::Form
	{

#pragma region Declarations
//Just for keeping things neat

	enum class ANIM_STATE{AS_IN, AS_OUT ,IDLE};


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newAnimationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openAnimationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  importSpriteSheetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAnimationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutSpriteyToolStripMenuItem;
	private: System::Windows::Forms::Panel^  sSheetPlaceholderPanel;
	private: System::Windows::Forms::Panel^  fViewerPlaceholderPanel;




	private: System::Windows::Forms::Button^  addNewFrameButton;
	private: System::Windows::Forms::Button^  eraseFrameButton;
	private: System::Windows::Forms::Button^  prevFramButton;
	private: System::Windows::Forms::Button^  nextFrameButton;


	private: System::Windows::Forms::Button^  resetFrameButton;
	private: System::Windows::Forms::Button^  cloneFrameButton;

	
	
	private: System::Windows::Forms::TextBox^  cbXSizeBox;
	private: System::Windows::Forms::TextBox^  cbYSizeBox;
	private: System::Windows::Forms::Label^  xLabel;
	private: System::Windows::Forms::Label^  yLabel;

		
		
		
	private: System::Windows::Forms::GroupBox^  cbGroupBox;
	private: System::Windows::Forms::Panel^  coordPanel;

	private: System::Windows::Forms::Label^  SSMY;
	private: System::Windows::Forms::Label^  SSMX;
	private: System::Windows::Forms::Label^  ssMouseCoordYLabel;
	private: System::Windows::Forms::Label^  ssMouseCoordXLabel;
	private: System::Windows::Forms::Panel^  controlPanel;
	private: System::Windows::Forms::PictureBox^  LogoImageBox;

	private: System::Windows::Forms::RadioButton^  cursorButton;
	private: System::Windows::Forms::RadioButton^  moveAnchorButton;

	private: System::Windows::Forms::Panel^  FVControlPanel;
	private: System::Windows::Forms::Panel^  FrameLabelPanel;
	private: System::Windows::Forms::Label^  frameNumberLabel;
	private: System::Windows::Forms::Label^  SSY;
	private: System::Windows::Forms::Label^  SSX;
	private: System::Windows::Forms::Label^  SSYLabel;
	private: System::Windows::Forms::Label^  SSXLabel;
	private: System::Windows::Forms::Label^  APY;
	private: System::Windows::Forms::Label^  APX;
	private: System::Windows::Forms::Label^  APYLabel;
	private: System::Windows::Forms::Label^  APXLabel;


	public: 
		FViewer^ frameViewer;
		SSViewer^ sSheetViewer;
		
		Animation* currentAnimation;//the instance of animation that we work with
		int* currentFrame;//the pointer that represents the current frame we are working on
		SpriteySettings* currentSettings;
		point* capBoxSize;
		
		int defRadius;
		int defaultHitboxStep;

		ANIM_STATE cbAnimState;
		ANIM_STATE hbAnimState;
		ANIM_STATE hubAnimState;
		int defaultHBXOut;
		int defaultHBXIn;
		int defaultHuBXOut;
		int defaultHuBXIn;
		
		int defaultCBGroupXIn;
		int defaultCBGroupXOut;
		

	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	public: 
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::RadioButton^  addHurtBoxButton;
	private: System::Windows::Forms::RadioButton^  addHitBoxButton;
	private: System::Windows::Forms::ToolTip^  SpriteyFormToolTip;
	private: System::Windows::Forms::Timer^  CBOutTimer;
			 
	private: System::Windows::Forms::Timer^  CBInTimer;
	private: System::Windows::Forms::Button^  editCBButton;
	private: System::Windows::Forms::Timer^  HBInTimer;
	private: System::Windows::Forms::Timer^  HBOutTimer;
	private: System::Windows::Forms::Timer^  HuBDelayTimer;

	private: System::Windows::Forms::Timer^  HuBInTimer;
	private: System::Windows::Forms::Timer^  HuBOutTimer;
	private: System::Windows::Forms::Timer^  HBOutDelay;

			 
			 


#pragma endregion
	
	public:
		
		SpriteyForm(void)
		{
			//////////////////////////////////
			//My own initialisations in here
			//////////////////////////////////
			InitializeComponent();
			sSheetViewer = gcnew SSViewer(this,SSVSizeX,SSVSizeY);
			frameViewer = gcnew FViewer(this, FVPosX,FVPosY,FVSizeX,FVSizeY);
			//maximizeWindow();
			capBoxSize = new point(defaultCBX,defaultCBY);

			this->sSheetPlaceholderPanel->SendToBack();
			
			HWND hndleSSPanel = static_cast<HWND> (this->sSheetPlaceholderPanel->Handle.ToPointer());
			SetWindowPos(hndleSSPanel, HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

			//set the initial values of the capBox textbox's
			this->cbXSizeBox->Text = System::Convert::ToString(capBoxSize->x);
			this->cbYSizeBox->Text = System::Convert::ToString(capBoxSize->y);

			//on startup we want our animation and currFrame instances to be nullptrs
			currentAnimation = nullptr;
			currentFrame = nullptr;
			currentSettings = nullptr; 

			defRadius = 10;
			defaultHitboxStep = 1;

			defaultCBGroupXIn = 68;
			defaultCBGroupXOut = 149;
			defaultHBXIn = 81;
			defaultHuBXIn = 81;
			defaultHBXOut = 149;
			defaultHuBXOut = 149;
			cbAnimState = ANIM_STATE::IDLE;
			hbAnimState = ANIM_STATE::IDLE;
			hubAnimState = ANIM_STATE::IDLE;

		}

	protected:

		
		~SpriteyForm()
		{
			if(components)
			{
				delete components;
			}
			delete capBoxSize;
			delete currentAnimation;
			delete currentSettings;
			delete currentFrame;
		}

private: System::ComponentModel::IContainer^  components;

protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(SpriteyForm::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newAnimationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openAnimationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importSpriteSheetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAnimationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutSpriteyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sSheetPlaceholderPanel = (gcnew System::Windows::Forms::Panel());
			this->addNewFrameButton = (gcnew System::Windows::Forms::Button());
			this->eraseFrameButton = (gcnew System::Windows::Forms::Button());
			this->fViewerPlaceholderPanel = (gcnew System::Windows::Forms::Panel());
			this->prevFramButton = (gcnew System::Windows::Forms::Button());
			this->nextFrameButton = (gcnew System::Windows::Forms::Button());
			this->resetFrameButton = (gcnew System::Windows::Forms::Button());
			this->cloneFrameButton = (gcnew System::Windows::Forms::Button());
			this->cbXSizeBox = (gcnew System::Windows::Forms::TextBox());
			this->cbYSizeBox = (gcnew System::Windows::Forms::TextBox());
			this->xLabel = (gcnew System::Windows::Forms::Label());
			this->yLabel = (gcnew System::Windows::Forms::Label());
			this->cbGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->coordPanel = (gcnew System::Windows::Forms::Panel());
			this->APY = (gcnew System::Windows::Forms::Label());
			this->APX = (gcnew System::Windows::Forms::Label());
			this->APYLabel = (gcnew System::Windows::Forms::Label());
			this->APXLabel = (gcnew System::Windows::Forms::Label());
			this->SSY = (gcnew System::Windows::Forms::Label());
			this->SSMY = (gcnew System::Windows::Forms::Label());
			this->SSX = (gcnew System::Windows::Forms::Label());
			this->SSMX = (gcnew System::Windows::Forms::Label());
			this->SSYLabel = (gcnew System::Windows::Forms::Label());
			this->ssMouseCoordYLabel = (gcnew System::Windows::Forms::Label());
			this->SSXLabel = (gcnew System::Windows::Forms::Label());
			this->ssMouseCoordXLabel = (gcnew System::Windows::Forms::Label());
			this->controlPanel = (gcnew System::Windows::Forms::Panel());
			this->addHurtBoxButton = (gcnew System::Windows::Forms::RadioButton());
			this->addHitBoxButton = (gcnew System::Windows::Forms::RadioButton());
			this->moveAnchorButton = (gcnew System::Windows::Forms::RadioButton());
			this->cursorButton = (gcnew System::Windows::Forms::RadioButton());
			this->editCBButton = (gcnew System::Windows::Forms::Button());
			this->LogoImageBox = (gcnew System::Windows::Forms::PictureBox());
			this->FVControlPanel = (gcnew System::Windows::Forms::Panel());
			this->FrameLabelPanel = (gcnew System::Windows::Forms::Panel());
			this->frameNumberLabel = (gcnew System::Windows::Forms::Label());
			this->SpriteyFormToolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->CBOutTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->CBInTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HBInTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HBOutTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HuBDelayTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HuBInTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HuBOutTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->HBOutDelay = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->cbGroupBox->SuspendLayout();
			this->coordPanel->SuspendLayout();
			this->controlPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LogoImageBox))->BeginInit();
			this->FVControlPanel->SuspendLayout();
			this->FrameLabelPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1184, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->newAnimationToolStripMenuItem, 
				this->openAnimationToolStripMenuItem, this->importSpriteSheetToolStripMenuItem, this->saveAnimationToolStripMenuItem, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// newAnimationToolStripMenuItem
			// 
			this->newAnimationToolStripMenuItem->Name = L"newAnimationToolStripMenuItem";
			this->newAnimationToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->newAnimationToolStripMenuItem->Text = L"New Animation";
			// 
			// openAnimationToolStripMenuItem
			// 
			this->openAnimationToolStripMenuItem->Name = L"openAnimationToolStripMenuItem";
			this->openAnimationToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->openAnimationToolStripMenuItem->Text = L"Open Animation";
			this->openAnimationToolStripMenuItem->Click += gcnew System::EventHandler(this, &SpriteyForm::openAnimationToolStripMenuItem_Click);
			// 
			// importSpriteSheetToolStripMenuItem
			// 
			this->importSpriteSheetToolStripMenuItem->Name = L"importSpriteSheetToolStripMenuItem";
			this->importSpriteSheetToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->importSpriteSheetToolStripMenuItem->Text = L"Import SpriteSheet";
			this->importSpriteSheetToolStripMenuItem->Click += gcnew System::EventHandler(this, &SpriteyForm::importSpriteSheetToolStripMenuItem_Click);
			// 
			// saveAnimationToolStripMenuItem
			// 
			this->saveAnimationToolStripMenuItem->Name = L"saveAnimationToolStripMenuItem";
			this->saveAnimationToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->saveAnimationToolStripMenuItem->Text = L"Save Animation";
			this->saveAnimationToolStripMenuItem->Click += gcnew System::EventHandler(this, &SpriteyForm::saveAnimationToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->settingsToolStripMenuItem});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->Enabled = false;
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->settingsToolStripMenuItem->Text = L"Settings";
			this->settingsToolStripMenuItem->Click += gcnew System::EventHandler(this, &SpriteyForm::settingsToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutSpriteyToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutSpriteyToolStripMenuItem
			// 
			this->aboutSpriteyToolStripMenuItem->Name = L"aboutSpriteyToolStripMenuItem";
			this->aboutSpriteyToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->aboutSpriteyToolStripMenuItem->Text = L"About Spritey";
			// 
			// sSheetPlaceholderPanel
			// 
			this->sSheetPlaceholderPanel->BackColor = System::Drawing::Color::Transparent;
			this->sSheetPlaceholderPanel->Location = System::Drawing::Point(12, 27);
			this->sSheetPlaceholderPanel->Margin = System::Windows::Forms::Padding(0);
			this->sSheetPlaceholderPanel->Name = L"sSheetPlaceholderPanel";
			this->sSheetPlaceholderPanel->Size = System::Drawing::Size(1024, 700);
			this->sSheetPlaceholderPanel->TabIndex = 1;
			this->sSheetPlaceholderPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &SpriteyForm::ssViewer_MouseDown);
			this->sSheetPlaceholderPanel->MouseLeave += gcnew System::EventHandler(this, &SpriteyForm::ssViewer_MouseExit);
			this->sSheetPlaceholderPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &SpriteyForm::ssViewer_MouseMotion);
			this->sSheetPlaceholderPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &SpriteyForm::ssViewer_MouseUp);
			// 
			// addNewFrameButton
			// 
			this->addNewFrameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"addNewFrameButton.Image")));
			this->addNewFrameButton->Location = System::Drawing::Point(11, 196);
			this->addNewFrameButton->Name = L"addNewFrameButton";
			this->addNewFrameButton->Size = System::Drawing::Size(50, 51);
			this->addNewFrameButton->TabIndex = 5;
			this->addNewFrameButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->addNewFrameButton, L"Create New Frame");
			this->addNewFrameButton->UseVisualStyleBackColor = true;
			this->addNewFrameButton->Click += gcnew System::EventHandler(this, &SpriteyForm::addNewFrameButton_Click);
			// 
			// eraseFrameButton
			// 
			this->eraseFrameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"eraseFrameButton.Image")));
			this->eraseFrameButton->Location = System::Drawing::Point(11, 254);
			this->eraseFrameButton->Name = L"eraseFrameButton";
			this->eraseFrameButton->Size = System::Drawing::Size(50, 51);
			this->eraseFrameButton->TabIndex = 6;
			this->eraseFrameButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->eraseFrameButton, L"Delete Frame");
			this->eraseFrameButton->UseVisualStyleBackColor = true;
			this->eraseFrameButton->Click += gcnew System::EventHandler(this, &SpriteyForm::eraseFrameButton_Click);
			// 
			// fViewerPlaceholderPanel
			// 
			this->fViewerPlaceholderPanel->BackColor = System::Drawing::Color::Transparent;
			this->fViewerPlaceholderPanel->Location = System::Drawing::Point(10, 29);
			this->fViewerPlaceholderPanel->Name = L"fViewerPlaceholderPanel";
			this->fViewerPlaceholderPanel->Size = System::Drawing::Size(120, 120);
			this->fViewerPlaceholderPanel->TabIndex = 7;
			this->fViewerPlaceholderPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &SpriteyForm::fViewer_MouseDown);
			this->fViewerPlaceholderPanel->MouseEnter += gcnew System::EventHandler(this, &SpriteyForm::fViewerPlaceholderPanel_MouseEnter);
			this->fViewerPlaceholderPanel->MouseLeave += gcnew System::EventHandler(this, &SpriteyForm::fViewerPlaceholderPanel_MouseLeave);
			this->fViewerPlaceholderPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &SpriteyForm::fViewer_MouseMotion);
			// 
			// prevFramButton
			// 
			this->prevFramButton->BackColor = System::Drawing::SystemColors::ControlDark;
			this->prevFramButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"prevFramButton.Image")));
			this->prevFramButton->Location = System::Drawing::Point(10, 152);
			this->prevFramButton->Name = L"prevFramButton";
			this->prevFramButton->Size = System::Drawing::Size(50, 30);
			this->prevFramButton->TabIndex = 8;
			this->prevFramButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->prevFramButton, L"Previous Frame");
			this->prevFramButton->UseVisualStyleBackColor = true;
			this->prevFramButton->Click += gcnew System::EventHandler(this, &SpriteyForm::prevFramButton_Click);
			// 
			// nextFrameButton
			// 
			this->nextFrameButton->BackColor = System::Drawing::SystemColors::ControlDark;
			this->nextFrameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"nextFrameButton.Image")));
			this->nextFrameButton->Location = System::Drawing::Point(82, 152);
			this->nextFrameButton->Name = L"nextFrameButton";
			this->nextFrameButton->Size = System::Drawing::Size(50, 30);
			this->nextFrameButton->TabIndex = 9;
			this->nextFrameButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->nextFrameButton, L"Next Frame");
			this->nextFrameButton->UseVisualStyleBackColor = true;
			this->nextFrameButton->Click += gcnew System::EventHandler(this, &SpriteyForm::nextFrameButton_Click);
			// 
			// resetFrameButton
			// 
			this->resetFrameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"resetFrameButton.Image")));
			this->resetFrameButton->Location = System::Drawing::Point(12, 729);
			this->resetFrameButton->Name = L"resetFrameButton";
			this->resetFrameButton->Size = System::Drawing::Size(36, 36);
			this->resetFrameButton->TabIndex = 12;
			this->resetFrameButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->resetFrameButton, L"Reset Sprite Sheet Position");
			this->resetFrameButton->UseVisualStyleBackColor = true;
			this->resetFrameButton->Click += gcnew System::EventHandler(this, &SpriteyForm::resetFrameButton_Click);
			// 
			// cloneFrameButton
			// 
			this->cloneFrameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"cloneFrameButton.Image")));
			this->cloneFrameButton->Location = System::Drawing::Point(11, 312);
			this->cloneFrameButton->Name = L"cloneFrameButton";
			this->cloneFrameButton->Size = System::Drawing::Size(50, 51);
			this->cloneFrameButton->TabIndex = 13;
			this->cloneFrameButton->TabStop = false;
			this->SpriteyFormToolTip->SetToolTip(this->cloneFrameButton, L"Clone Frame");
			this->cloneFrameButton->UseVisualStyleBackColor = true;
			this->cloneFrameButton->Click += gcnew System::EventHandler(this, &SpriteyForm::cloneFrameButton_Click);
			// 
			// cbXSizeBox
			// 
			this->cbXSizeBox->Location = System::Drawing::Point(15, 17);
			this->cbXSizeBox->MaxLength = 3;
			this->cbXSizeBox->Name = L"cbXSizeBox";
			this->cbXSizeBox->Size = System::Drawing::Size(49, 20);
			this->cbXSizeBox->TabIndex = 15;
			this->cbXSizeBox->TextChanged += gcnew System::EventHandler(this, &SpriteyForm::cbTBoxXChanged);
			this->cbXSizeBox->Enter += gcnew System::EventHandler(this, &SpriteyForm::cbSizeBox_EnterFocus);
			// 
			// cbYSizeBox
			// 
			this->cbYSizeBox->Location = System::Drawing::Point(15, 37);
			this->cbYSizeBox->MaxLength = 3;
			this->cbYSizeBox->Name = L"cbYSizeBox";
			this->cbYSizeBox->Size = System::Drawing::Size(49, 20);
			this->cbYSizeBox->TabIndex = 16;
			this->cbYSizeBox->TextChanged += gcnew System::EventHandler(this, &SpriteyForm::cbTBoxYChanged);
			this->cbYSizeBox->Enter += gcnew System::EventHandler(this, &SpriteyForm::cbSizeBox_EnterFocus);
			// 
			// xLabel
			// 
			this->xLabel->AutoSize = true;
			this->xLabel->BackColor = System::Drawing::Color::Transparent;
			this->xLabel->Location = System::Drawing::Point(1, 20);
			this->xLabel->Name = L"xLabel";
			this->xLabel->Size = System::Drawing::Size(15, 13);
			this->xLabel->TabIndex = 17;
			this->xLabel->Text = L"x:";
			// 
			// yLabel
			// 
			this->yLabel->AutoSize = true;
			this->yLabel->BackColor = System::Drawing::Color::Transparent;
			this->yLabel->Location = System::Drawing::Point(1, 40);
			this->yLabel->Name = L"yLabel";
			this->yLabel->Size = System::Drawing::Size(15, 13);
			this->yLabel->TabIndex = 17;
			this->yLabel->Text = L"y:";
			// 
			// cbGroupBox
			// 
			this->cbGroupBox->Controls->Add(this->cbXSizeBox);
			this->cbGroupBox->Controls->Add(this->cbYSizeBox);
			this->cbGroupBox->Controls->Add(this->xLabel);
			this->cbGroupBox->Controls->Add(this->yLabel);
			this->cbGroupBox->Location = System::Drawing::Point(68, 128);
			this->cbGroupBox->Name = L"cbGroupBox";
			this->cbGroupBox->Size = System::Drawing::Size(73, 61);
			this->cbGroupBox->TabIndex = 18;
			this->cbGroupBox->TabStop = false;
			this->cbGroupBox->Text = L"Cap Box";
			this->SpriteyFormToolTip->SetToolTip(this->cbGroupBox, L"Capture Box Dimensions");
			// 
			// coordPanel
			// 
			this->coordPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->coordPanel->Controls->Add(this->APY);
			this->coordPanel->Controls->Add(this->APX);
			this->coordPanel->Controls->Add(this->APYLabel);
			this->coordPanel->Controls->Add(this->APXLabel);
			this->coordPanel->Controls->Add(this->SSY);
			this->coordPanel->Controls->Add(this->SSMY);
			this->coordPanel->Controls->Add(this->SSX);
			this->coordPanel->Controls->Add(this->SSMX);
			this->coordPanel->Controls->Add(this->SSYLabel);
			this->coordPanel->Controls->Add(this->ssMouseCoordYLabel);
			this->coordPanel->Controls->Add(this->SSXLabel);
			this->coordPanel->Controls->Add(this->ssMouseCoordXLabel);
			this->coordPanel->Location = System::Drawing::Point(764, 728);
			this->coordPanel->Name = L"coordPanel";
			this->coordPanel->Size = System::Drawing::Size(273, 38);
			this->coordPanel->TabIndex = 19;
			// 
			// APY
			// 
			this->APY->AutoSize = true;
			this->APY->Location = System::Drawing::Point(88, 17);
			this->APY->Name = L"APY";
			this->APY->Size = System::Drawing::Size(17, 13);
			this->APY->TabIndex = 3;
			this->APY->Text = L"Y:";
			// 
			// APX
			// 
			this->APX->AutoSize = true;
			this->APX->BackColor = System::Drawing::Color::Transparent;
			this->APX->Location = System::Drawing::Point(28, 17);
			this->APX->Name = L"APX";
			this->APX->Size = System::Drawing::Size(17, 13);
			this->APX->TabIndex = 4;
			this->APX->Text = L"X:";
			// 
			// APYLabel
			// 
			this->APYLabel->AutoSize = true;
			this->APYLabel->Location = System::Drawing::Point(112, 17);
			this->APYLabel->Name = L"APYLabel";
			this->APYLabel->Size = System::Drawing::Size(19, 13);
			this->APYLabel->TabIndex = 1;
			this->APYLabel->Text = L"----";
			this->APYLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// APXLabel
			// 
			this->APXLabel->AutoSize = true;
			this->APXLabel->Location = System::Drawing::Point(51, 17);
			this->APXLabel->Name = L"APXLabel";
			this->APXLabel->Size = System::Drawing::Size(19, 13);
			this->APXLabel->TabIndex = 2;
			this->APXLabel->Text = L"----";
			this->APXLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// SSY
			// 
			this->SSY->AutoSize = true;
			this->SSY->Location = System::Drawing::Point(220, 17);
			this->SSY->Name = L"SSY";
			this->SSY->Size = System::Drawing::Size(17, 13);
			this->SSY->TabIndex = 0;
			this->SSY->Text = L"Y:";
			// 
			// SSMY
			// 
			this->SSMY->AutoSize = true;
			this->SSMY->Location = System::Drawing::Point(220, 3);
			this->SSMY->Name = L"SSMY";
			this->SSMY->Size = System::Drawing::Size(17, 13);
			this->SSMY->TabIndex = 0;
			this->SSMY->Text = L"Y:";
			// 
			// SSX
			// 
			this->SSX->AutoSize = true;
			this->SSX->BackColor = System::Drawing::Color::Transparent;
			this->SSX->Location = System::Drawing::Point(160, 17);
			this->SSX->Name = L"SSX";
			this->SSX->Size = System::Drawing::Size(17, 13);
			this->SSX->TabIndex = 0;
			this->SSX->Text = L"X:";
			// 
			// SSMX
			// 
			this->SSMX->AutoSize = true;
			this->SSMX->BackColor = System::Drawing::Color::Transparent;
			this->SSMX->Location = System::Drawing::Point(160, 3);
			this->SSMX->Name = L"SSMX";
			this->SSMX->Size = System::Drawing::Size(17, 13);
			this->SSMX->TabIndex = 0;
			this->SSMX->Text = L"X:";
			// 
			// SSYLabel
			// 
			this->SSYLabel->AutoSize = true;
			this->SSYLabel->Location = System::Drawing::Point(243, 17);
			this->SSYLabel->Name = L"SSYLabel";
			this->SSYLabel->Size = System::Drawing::Size(19, 13);
			this->SSYLabel->TabIndex = 0;
			this->SSYLabel->Text = L"----";
			this->SSYLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// ssMouseCoordYLabel
			// 
			this->ssMouseCoordYLabel->AutoSize = true;
			this->ssMouseCoordYLabel->Location = System::Drawing::Point(243, 3);
			this->ssMouseCoordYLabel->Name = L"ssMouseCoordYLabel";
			this->ssMouseCoordYLabel->Size = System::Drawing::Size(19, 13);
			this->ssMouseCoordYLabel->TabIndex = 0;
			this->ssMouseCoordYLabel->Text = L"----";
			this->ssMouseCoordYLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// SSXLabel
			// 
			this->SSXLabel->AutoSize = true;
			this->SSXLabel->Location = System::Drawing::Point(183, 17);
			this->SSXLabel->Name = L"SSXLabel";
			this->SSXLabel->Size = System::Drawing::Size(19, 13);
			this->SSXLabel->TabIndex = 0;
			this->SSXLabel->Text = L"----";
			this->SSXLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// ssMouseCoordXLabel
			// 
			this->ssMouseCoordXLabel->AutoSize = true;
			this->ssMouseCoordXLabel->Location = System::Drawing::Point(183, 3);
			this->ssMouseCoordXLabel->Name = L"ssMouseCoordXLabel";
			this->ssMouseCoordXLabel->Size = System::Drawing::Size(19, 13);
			this->ssMouseCoordXLabel->TabIndex = 0;
			this->ssMouseCoordXLabel->Text = L"----";
			this->ssMouseCoordXLabel->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// controlPanel
			// 
			this->controlPanel->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->controlPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->controlPanel->Controls->Add(this->addHurtBoxButton);
			this->controlPanel->Controls->Add(this->addHitBoxButton);
			this->controlPanel->Controls->Add(this->moveAnchorButton);
			this->controlPanel->Controls->Add(this->cursorButton);
			this->controlPanel->Controls->Add(this->cbGroupBox);
			this->controlPanel->Controls->Add(this->editCBButton);
			this->controlPanel->Controls->Add(this->addNewFrameButton);
			this->controlPanel->Controls->Add(this->cloneFrameButton);
			this->controlPanel->Controls->Add(this->eraseFrameButton);
			this->controlPanel->Location = System::Drawing::Point(1036, 136);
			this->controlPanel->Margin = System::Windows::Forms::Padding(0);
			this->controlPanel->Name = L"controlPanel";
			this->controlPanel->Size = System::Drawing::Size(148, 439);
			this->controlPanel->TabIndex = 20;
			// 
			// addHurtBoxButton
			// 
			this->addHurtBoxButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->addHurtBoxButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"addHurtBoxButton.Image")));
			this->addHurtBoxButton->Location = System::Drawing::Point(81, 370);
			this->addHurtBoxButton->Name = L"addHurtBoxButton";
			this->addHurtBoxButton->Size = System::Drawing::Size(50, 51);
			this->addHurtBoxButton->TabIndex = 22;
			this->SpriteyFormToolTip->SetToolTip(this->addHurtBoxButton, L"Add Hurtbox (remove by selecting and using the delete key)");
			this->addHurtBoxButton->UseVisualStyleBackColor = true;
			this->addHurtBoxButton->CheckedChanged += gcnew System::EventHandler(this, &SpriteyForm::addHurtBoxButton_CheckedChanged);
			// 
			// addHitBoxButton
			// 
			this->addHitBoxButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->addHitBoxButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"addHitBoxButton.Image")));
			this->addHitBoxButton->Location = System::Drawing::Point(81, 312);
			this->addHitBoxButton->Name = L"addHitBoxButton";
			this->addHitBoxButton->Size = System::Drawing::Size(50, 51);
			this->addHitBoxButton->TabIndex = 21;
			this->SpriteyFormToolTip->SetToolTip(this->addHitBoxButton, L"Add Hitbox (remove by selecting and using the delete key)");
			this->addHitBoxButton->UseVisualStyleBackColor = true;
			this->addHitBoxButton->CheckedChanged += gcnew System::EventHandler(this, &SpriteyForm::addHitBoxButton_CheckedChanged);
			// 
			// moveAnchorButton
			// 
			this->moveAnchorButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->moveAnchorButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"moveAnchorButton.Image")));
			this->moveAnchorButton->Location = System::Drawing::Point(11, 370);
			this->moveAnchorButton->Name = L"moveAnchorButton";
			this->moveAnchorButton->Size = System::Drawing::Size(50, 51);
			this->moveAnchorButton->TabIndex = 20;
			this->SpriteyFormToolTip->SetToolTip(this->moveAnchorButton, L"Edit Anchor: Move the anchor-point using the arrow keys");
			this->moveAnchorButton->UseVisualStyleBackColor = true;
			this->moveAnchorButton->CheckedChanged += gcnew System::EventHandler(this, &SpriteyForm::moveAnchorButton_CheckedChanged);
			// 
			// cursorButton
			// 
			this->cursorButton->Appearance = System::Windows::Forms::Appearance::Button;
			this->cursorButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"cursorButton.Image")));
			this->cursorButton->Location = System::Drawing::Point(11, 81);
			this->cursorButton->Name = L"cursorButton";
			this->cursorButton->Size = System::Drawing::Size(50, 51);
			this->cursorButton->TabIndex = 20;
			this->SpriteyFormToolTip->SetToolTip(this->cursorButton, L"Drag: Use to drag the Sprite Sheet or Hitboxes/Hurtboxes");
			this->cursorButton->UseVisualStyleBackColor = true;
			this->cursorButton->CheckedChanged += gcnew System::EventHandler(this, &SpriteyForm::cursorButton_CheckedChanged);
			// 
			// editCBButton
			// 
			this->editCBButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editCBButton.Image")));
			this->editCBButton->Location = System::Drawing::Point(11, 138);
			this->editCBButton->Name = L"editCBButton";
			this->editCBButton->Size = System::Drawing::Size(50, 51);
			this->editCBButton->TabIndex = 5;
			this->editCBButton->TabStop = false;
			this->editCBButton->UseVisualStyleBackColor = true;
			this->editCBButton->Click += gcnew System::EventHandler(this, &SpriteyForm::editCBButton_CheckedChanged);
			// 
			// LogoImageBox
			// 
			this->LogoImageBox->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->LogoImageBox->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"LogoImageBox.Image")));
			this->LogoImageBox->Location = System::Drawing::Point(1036, 28);
			this->LogoImageBox->Name = L"LogoImageBox";
			this->LogoImageBox->Size = System::Drawing::Size(148, 112);
			this->LogoImageBox->TabIndex = 21;
			this->LogoImageBox->TabStop = false;
			// 
			// FVControlPanel
			// 
			this->FVControlPanel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->FVControlPanel->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->FVControlPanel->Controls->Add(this->FrameLabelPanel);
			this->FVControlPanel->Controls->Add(this->nextFrameButton);
			this->FVControlPanel->Controls->Add(this->fViewerPlaceholderPanel);
			this->FVControlPanel->Controls->Add(this->prevFramButton);
			this->FVControlPanel->Location = System::Drawing::Point(1037, 576);
			this->FVControlPanel->Name = L"FVControlPanel";
			this->FVControlPanel->Size = System::Drawing::Size(147, 191);
			this->FVControlPanel->TabIndex = 22;
			// 
			// FrameLabelPanel
			// 
			this->FrameLabelPanel->BackColor = System::Drawing::SystemColors::ControlLight;
			this->FrameLabelPanel->Controls->Add(this->frameNumberLabel);
			this->FrameLabelPanel->Location = System::Drawing::Point(10, 7);
			this->FrameLabelPanel->Name = L"FrameLabelPanel";
			this->FrameLabelPanel->Size = System::Drawing::Size(120, 22);
			this->FrameLabelPanel->TabIndex = 10;
			this->SpriteyFormToolTip->SetToolTip(this->FrameLabelPanel, L"Current Frame");
			// 
			// frameNumberLabel
			// 
			this->frameNumberLabel->AutoSize = true;
			this->frameNumberLabel->Location = System::Drawing::Point(26, 4);
			this->frameNumberLabel->Name = L"frameNumberLabel";
			this->frameNumberLabel->Size = System::Drawing::Size(62, 13);
			this->frameNumberLabel->TabIndex = 0;
			this->frameNumberLabel->Text = L"Frame: N/A";
			this->frameNumberLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// CBOutTimer
			// 
			this->CBOutTimer->Interval = 15;
			this->CBOutTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::CBOutTimer_Tick);
			// 
			// CBInTimer
			// 
			this->CBInTimer->Interval = 15;
			this->CBInTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::CBInTimer_Tick);
			// 
			// HBInTimer
			// 
			this->HBInTimer->Interval = 15;
			this->HBInTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::HBInTimer_Tick);
			// 
			// HBOutTimer
			// 
			this->HBOutTimer->Interval = 15;
			this->HBOutTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::HBOutTimer_Tick);
			// 
			// HuBDelayTimer
			// 
			this->HuBDelayTimer->Interval = 300;
			this->HuBDelayTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::HuBDelayTimer_Tick);
			// 
			// HuBInTimer
			// 
			this->HuBInTimer->Interval = 15;
			this->HuBInTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::HuBInTimer_Tick);
			// 
			// HuBOutTimer
			// 
			this->HuBOutTimer->Interval = 15;
			this->HuBOutTimer->Tick += gcnew System::EventHandler(this, &SpriteyForm::HuBOutTimer_Tick);
			// 
			// HBOutDelay
			// 
			this->HBOutDelay->Enabled = true;
			this->HBOutDelay->Interval = 2000;
			this->HBOutDelay->Tick += gcnew System::EventHandler(this, &SpriteyForm::HBOutDelay_Tick);
			// 
			// SpriteyForm
			// 
			this->ClientSize = System::Drawing::Size(1184, 767);
			this->Controls->Add(this->FVControlPanel);
			this->Controls->Add(this->resetFrameButton);
			this->Controls->Add(this->sSheetPlaceholderPanel);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->controlPanel);
			this->Controls->Add(this->coordPanel);
			this->Controls->Add(this->LogoImageBox);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->KeyPreview = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"SpriteyForm";
			this->Text = L"Spritey: The Sprite Sheet & Frame Data Editor (BETA)";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &SpriteyForm::SpriteyForm_Paint);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &SpriteyForm::SpriteyForm_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->cbGroupBox->ResumeLayout(false);
			this->cbGroupBox->PerformLayout();
			this->coordPanel->ResumeLayout(false);
			this->coordPanel->PerformLayout();
			this->controlPanel->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->LogoImageBox))->EndInit();
			this->FVControlPanel->ResumeLayout(false);
			this->FrameLabelPanel->ResumeLayout(false);
			this->FrameLabelPanel->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void SpriteyForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		{
			UNREFERENCED_PARAMETER(sender);
			UNREFERENCED_PARAMETER(e);
			redrawViewers();
			

		}

private: System::Void maximizeWindow()
		{
			HWND hWnd = static_cast<HWND> (this->Handle.ToPointer());
			SetWindowPos(hWnd,NULL,0,0,GetSystemMetrics(SM_CXMAXIMIZED),GetSystemMetrics(SM_CYMAXIMIZED),SWP_NOZORDER);
		}
	
#pragma region STARTUP FUNCTIONS

private: System::Void importSpriteSheetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			Stream^ myStream; //create a new stream to buffer to the chars returned by the OFD
			OpenFileDialog^ myOpenFileDialog = gcnew OpenFileDialog; //create a new dialogue box (does not display it)
			myOpenFileDialog->Title = "Open Sprite Sheet";//OFD title
			myOpenFileDialog->Filter = "Accepted image formats (*.PNG)|*.PNG|" + "All files (*.*)|*.*";//setup the filter
			myOpenFileDialog->FilterIndex = 1;//default filter to .spr files

			if(myOpenFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) //shows the dialog and reads in the return
			{
				if((myStream = myOpenFileDialog->OpenFile()) != nullptr)
				{
					String^ strFileName = myOpenFileDialog->InitialDirectory+ myOpenFileDialog->FileName; //store path of selected file as a string.
					strFileName = strFileName->Replace("\\","/"); //replace all of the backslashes with forward slashes, as lodepng only seems to respect these
					
					//////////////////////////////////////
					///Testing
					//MessageBox::Show("Path of selected file: "+strFileName->Replace("\\","/"));
					//
					//myStream->Close();//close the stream
					//////////////////////////////////////

					//Marshal the String^ to an std string
					msclr::interop::marshal_context context;
					std::string stdstrFileName = context.marshal_as<std::string>(strFileName);

					sSheetViewer->changeSpriteSheet(stdstrFileName);
					frameViewer->loadSpriteSheet(stdstrFileName);

					//When this is done correctly, we need to create a new instance of Animation
					createNewAnimation();
					
					
					if(currentSettings != nullptr)//if we already have a currentSettings
					{
						currentSettings->setSSFileName(stdstrFileName);//just change the name of the associated spritesheet 
					}else //if we don't
					{
						currentSettings = new SpriteySettings();//create a new instance of spriteySettings
						currentSettings->setSSFileName(stdstrFileName);//the change the name of the associated spritesheet
						this->settingsToolStripMenuItem->Enabled = true;
					}
					
					updateFramePos();//update the currentAnimation.currentframes.f_loc var and start-off the framepos labels at the bottom of the screen.
					//TODO:: ensure that we delete/save any old instance of animation we were previously working on,
					//as well as free all of the memory that those assets may have been using

				}

			}
		 }

private: System::Void createNewAnimation()//Called when a new spritesheet is loaded
		 {
			if(currentAnimation == nullptr)
			{
				int tempCurrFrame;//apparently it's okay to create an unmanaged int here. 
				currentAnimation = new Animation(tempCurrFrame);//pass it in by reference.
				if(currentFrame!=nullptr)
				{
					delete currentFrame;
					currentFrame = nullptr;
				}
				currentFrame = new int(tempCurrFrame);
				//////////////////////////////////////
				///we need to set some default values for the first frame. As it's a new animation.
				point* initialLoc = new point((SSVSizeX/2-(capBoxSize->x/2)),(SSVSizeY/2-(capBoxSize->y/2)));//we want this to be bottom left, but because the loc is stored as a frames relative pos to the CapBox, it's not going to be 0,0
				//inverse the basic translation that makes the spritesheet drawn in the center of the screen in order to get it to start off drawn at the BL
				point* initialDim = capBoxSize;// the dims of the current frame must = the capBoxSize at all times
				point* initialAnchor = new point(capBoxSize->x/2,capBoxSize->y*0.2f);//soo we will change this to a monitored anchor variable, but for now 0,0 will do.
				currentAnimation->getFrame(*currentFrame)->f_loc = initialLoc;
				currentAnimation->getFrame(*currentFrame)->f_dim = initialDim;
				currentAnimation->getFrame(*currentFrame)->f_anchor = initialAnchor;
				///////////////////End initialization of first frame
				updateStateOnFrameChange();
			}
		 
		 }


#pragma endregion
		
#pragma region BOXES & UPDATES

private: System::Void redrawViewers()
		 {
			 if (sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation!=nullptr)//if we have a valid animation in place
			 {
				 sSheetViewer->Render(capBoxSize, currentAnimation->getFrame(*currentFrame)); //we render with the anchorpoint, as that requires we pass a pointer to the current frame
				 sSheetViewer->SwapOpenGLBuffers();
			 }else{//if not
			 sSheetViewer->Render(capBoxSize);//we render without
			 sSheetViewer->SwapOpenGLBuffers();
			 }

			 if(currentAnimation!=nullptr && frameViewer->loadedMode==FViewer::FV_LOADED_MODE::SHEET_LOADED)
			 {
				 frameViewer->Render(currentAnimation->getFrame(*currentFrame));
				 frameViewer->SwapOpenGLBuffers();
			 }else
			 {
				 frameViewer->Render();
				 frameViewer->SwapOpenGLBuffers();
			 }

		 }

private: System::Void updateStateOnFrameChange()
		 {
			 updateSpriteSheetLocOnFrameChange();//TRUE:: This needs to happen first, otherwise, when changing frames, the previous frames locaction is loaded into the frame we are moving to, and somehow spritesheet.pos isn't properly update if this doesn't happen first
			 updateCapBoxesOnFrameChange();//FALSE::important that this happens before updateFramePos, as the currentFrames capbox dims are used to calculate the dimensions
			 //update any labels that need updating
			 updateFrameLabel();
			 updateFramePosLabels();
			 updateAnchorPosLabels();
			 if(frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED)
			 {
				 frameViewer->hovered = FViewer::FV_SELECTED::NONE;
				 frameViewer->unselectAllHB();
			 }
			 
			 //Redraw the viewers
			 redrawViewers();
			 printf("\nViews redrawn");

			 
		 }

private: System::Void updateStateOnNewFrame()
		 {
			 updateSpriteSheetLocOnNewFrame();//TRUE::This needs to happen first, otherwise, when changing frames, the previous frames locaction is loaded into the frame we are moving to, and somehow spritesheet.pos isn't properly update if this doesn't happen first
			 updateCapBoxesOnFrameChange();////FALSE::important that this happens before updateFramePos, as the currentFrames capbox dims are used to calculate the dimensions
			 //update any labels that need updating
			 updateFrameLabel();
			 updateFramePosLabels();
			 updateAnchorPosLabels();
			 if(frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED)
			 {
				 frameViewer->hovered = FViewer::FV_SELECTED::NONE;
				 frameViewer->unselectAllHB();
			 }

			 //Redraw the viewers
			 redrawViewers();
			 printf("\nViews redrawn");


		 }

private: System::Void updateSpriteSheetLocOnNewFrame()
		 {
			 if(currentAnimation->getFrame(*currentFrame)!=nullptr)
			 {
				 point* screenSize = new point(SSVSizeX,SSVSizeY);//for debug purposes
				 sSheetViewer->spriteSheet->pos.x = (-1* currentAnimation->getFrame(*currentFrame)->f_loc.x) + (screenSize->x/2 - (currentAnimation->getFrame(*currentFrame)->f_dim.x/2));//we no longer get the dimensions of the frame from capboxsize
				 sSheetViewer->spriteSheet->pos.y = (-1* currentAnimation->getFrame(*currentFrame)->f_loc.y) + (screenSize->y/2 - (currentAnimation->getFrame(*currentFrame)->f_dim.y/2));//this causes complications. we instead get it directly from the frame in the animation object we're editing
				 //where it should be as described by the newly loaded (or re-loaded) frame
				 delete screenSize;
				 
			 }
		 }

private: System::Void updateSpriteSheetLocOnFrameChange()
		 {
			 if(currentAnimation->getFrame(*currentFrame)!=nullptr)
			 {
				 point* screenSize = new point(SSVSizeX,SSVSizeY);//for debug purposes
				 sSheetViewer->spriteSheet->pos.x = (-1* currentAnimation->getFrame(*currentFrame)->f_loc.x) + (screenSize->x/2 - (currentAnimation->getFrame(*currentFrame)->f_dim.x/2));//set the spritesheet location in the ssViewer to
				 sSheetViewer->spriteSheet->pos.y = (-1* currentAnimation->getFrame(*currentFrame)->f_loc.y) + (screenSize->y/2 - (currentAnimation->getFrame(*currentFrame)->f_dim.y/2));
				 //where it should be as described by the newly loaded (or re-loaded) frame
				 //ALSO: this needs to undergo the reverse of the translation that occurs in the function updateFramePos()
				 delete screenSize;
			 }
		 }

private: System::Void updateCapBoxesOnFrameChange()
		 {
			 if(currentAnimation->getFrame(*currentFrame)!=nullptr)
			 {
				 String^ newCBX = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_dim.x);
				 String^ newCBY = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_dim.y);

				 //ok we're updating the textboxes manually here
				 //HAPPY NOTE:) by doing this, we automatically trigger the textboxChanged events
				 //for the respective tb's. Which perform all the necessary safety checks for us. 
				 this->cbXSizeBox->Text = newCBX;
				 this->cbYSizeBox->Text = newCBY;
			 }
			 
		 }

private: System::Void loadIntoFrameDimsFromCB() //Remember ONLY use this function after a check for valid values in the cbX(&Y)SizeBox, otherwise bad things will happen 
		 {
			 if(currentAnimation!=nullptr)
			 {
				 currentAnimation->getFrame(*currentFrame)->f_dim = capBoxSize; //our frames dimensions = the capBox's size
				 

			 }
		 }

private: System::Void updateFrameLabel()
		 {
			 
			 String^ frameNumber = System::Convert::ToString(*currentFrame);
			 this->frameNumberLabel->Text = "Frame: " + frameNumber;
		 }

private: System::Void updateFramePosLabels()
		 {
			 this->SSXLabel->Text = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_loc.x);
			 this->SSYLabel->Text = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_loc.y);

		 }

private: System::Void updateAnchorPosLabels()
		 {
			 if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation!=nullptr)
			 {
				 this->APXLabel->Text = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_anchor.x);
				 this->APYLabel->Text = System::Convert::ToString(currentAnimation->getFrame(*currentFrame)->f_anchor.y);
			 }
		 }

private: System::Void updateFramePos()//gets the position of the frame via the spritesheet and capbox positions, and loads it into the currentFrames f_loc var
		 {//Also updates the labels at the bottom of the screen that display the frames position
			 point* framePos = new point();//the point that will hold the actual position of our frame. NOT the spritesheet. The frames pos
			 //is calculated by 
			 //-1*(translation of SS - (translation of CB - (CB.dimension/2)))

			 point* screenSize = new point(SSVSizeX,SSVSizeY);//for debug purposes
			 //don't forget that the capbox center is located exactly halfway across the viewer
			 framePos->x = (-1)*(sSheetViewer->spriteSheet->pos.x - ((screenSize->x/2)-(capBoxSize->x/2)));
			 framePos->y = (-1)*(sSheetViewer->spriteSheet->pos.y - ((screenSize->y/2)-(capBoxSize->y/2)));
			 //note: we couldve done this in a point constructor, but this formula is long so it's much more readable here

			 currentAnimation->getFrame(*currentFrame)->f_loc = framePos;

			 updateFramePosLabels();

			 delete framePos;//cleanup
			 delete screenSize;
		 }

private: System::Void updateFramePosOnDrag()//Does the above function, but checks for the dragstate first, meaning it won't update unles we're dragging
		 {
			 if(sSheetViewer->dragState == SSViewer::DRAG_STATE::DRAGGING && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)
			 {
				 updateFramePos();
			 }

		 }

private: System::Void cbTBoxXChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			
			printf("\nCapture Box X Text entered");
			try
			{
				point* prevCapBoxSize;
				int newX = System::Convert::ToUInt32(this->cbXSizeBox->Text);
				if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation != nullptr)//If we actually have a current animation
				{
				prevCapBoxSize = new point(currentAnimation->getFrame(*currentFrame)->f_dim);//save a copy of this, so that we can test the validity of the anchor 
				}else{//if not, we'll just use the basic capboxsize, which exists before the current animation anyway
					prevCapBoxSize = new point(*capBoxSize);
				}


				//against the new capbox size (ensuring our anchor point isnt outside the CB)
				entity* prevCapBoxEntity = new entity(point(SSVSizeX/2,SSVSizeY/2), point(prevCapBoxSize->x, prevCapBoxSize->y));//needed to calculate previous
				

				capBoxSize->x = newX; 
				loadIntoFrameDimsFromCB();
				
				entity* currentCapBoxEntity = new entity(point(SSVSizeX/2,SSVSizeY/2), point(capBoxSize->x, capBoxSize->y));//We should always be using currentAnimation(*getFrame)->f_anchor
				//when we can, but this may not even exist at this point in execution, so to be safe and easy we'll just go with the capboxsize value, which we know exists

				//Don't forget: We also need to update the current framePos (IF there is a spritesheet & animation currently loaded)
				//Also: We need to check that our Anchor is still within the bounds of the CapBox, so we'll check that in here as well
				if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation != nullptr)
				{
					updateFramePos();

					//ANCHOR CALCULATION
					if(!currentSettings->isAnchorDevAllowed())
					{
						//check if our anchor point is still inside the CB
						point* prevAnchorScreenPos = new point(currentAnimation->getFrame(*currentFrame)->f_anchor.x + prevCapBoxEntity->pos.x - (prevCapBoxEntity->size.x/2), currentAnimation->getFrame(*currentFrame)->f_anchor.y + prevCapBoxEntity->pos.y - (prevCapBoxEntity->size.y/2));
						//above is the screen position of the anchor position under the previous capbox

						if(!pointIsWithinCenteredBox(currentCapBoxEntity,prevAnchorScreenPos))//if it's not within the confines of the new CB
						{						
							if(prevAnchorScreenPos->x < currentCapBoxEntity->pos.x - (currentCapBoxEntity->size.x/2))//if it's less than the left border of the CB
							{
								currentAnimation->getFrame(*currentFrame)->f_anchor.x = 0; //As the anchor is measure from BL of the frame, left hand side is 0
							}
							if(prevAnchorScreenPos->x > currentCapBoxEntity->pos.x + (currentCapBoxEntity->size.x/2))//if it's further than the right border of the CB
							{
								currentAnimation->getFrame(*currentFrame)->f_anchor.x = currentCapBoxEntity->size.x; //As the anchor is measure from BL of the frame, right hand side is = to the size
							}
						}

						delete prevAnchorScreenPos;
					}
					

				}
				//clean up
				delete prevCapBoxEntity;
				delete currentCapBoxEntity;
				delete prevCapBoxSize;
				//redraw the viewers
				redrawViewers();
				printf("\nViews redrawn");
			}
			catch (FormatException^ e)
			{
				printf("\nNot a valid 3-digit number");
				capBoxSize->x = 2; //Do not reset the text, but discretely set the size to 2 
				loadIntoFrameDimsFromCB(); //The Same goes for the current frame f_dim value				
				//Textbox will still be empty, but capBox will not spaz out

				//Then redraw
				redrawViewers();
			}
			catch (OverflowException^ e)
			{
				this->cbXSizeBox->Text = System::Convert::ToString(capBoxSize->x);
				printf("\nNot a valid positive 3-digit number");
				MessageBox::Show("Only Positive Integers Allowed (no symbols)","Capture Box Error",MessageBoxButtons::OK);
			}

			 //Also lets make sure that this value cannot be zero or a negative number
			if(capBoxSize->x <= 0)
			{
				printf("\nThe Capture Box has a 2px minimum");
				capBoxSize->x = 2;
				loadIntoFrameDimsFromCB(); //The Same for the current frame f_dim value
				this->cbXSizeBox->Text = System::Convert::ToString(capBoxSize->x);
				//redraw the viewers
				redrawViewers();
				printf("\nViews redrawn");
			}

			
			

		 }


private: System::Void cbTBoxYChanged(System::Object^  sender, System::EventArgs^  e)
				  {
					  printf("\nCapture Box Y Text entered");
					  
					  try
					  {
						  point* prevCapBoxSize;//see above version of this function for more detailed annotations on this first section of code here, as they're mirrors of eachother. cba to type them out twice
						  int newY = System::Convert::ToUInt32(this->cbYSizeBox->Text);
						  if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation != nullptr)
						  {
							  prevCapBoxSize = new point(currentAnimation->getFrame(*currentFrame)->f_anchor);
						  }else{
							  prevCapBoxSize = new point(*capBoxSize);//save a copy of this, so that we can test the validity of the anchor 
						  }
						  //against the new capbox size (ensuring our anchor point isnt outside the CB)
						  entity* prevCapBoxEntity = new entity(point(SSVSizeX/2,SSVSizeY/2), point(prevCapBoxSize->x, prevCapBoxSize->y));//needed to calculate previous
						  
						  
						  capBoxSize->y = newY;
						  loadIntoFrameDimsFromCB();

						  entity* currentCapBoxEntity = new entity(point(SSVSizeX/2,SSVSizeY/2), point(capBoxSize->x, capBoxSize->y));

						  //Don't forget: We also need to update the current framePos (IF there is a spritesheet & animation currently loaded)
						  //Also: We need to check that our Anchor is still within the bounds of the CapBox, so we'll check that in here as well
						  if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation != nullptr)
						  {
							  updateFramePos();
							  
							  //ANCHOR CALCULATION
							  if(!currentSettings->isAnchorDevAllowed())
							  {
								  //check if our anchor point is still inside the CB
								  point* prevAnchorScreenPos = new point(currentAnimation->getFrame(*currentFrame)->f_anchor.x + prevCapBoxEntity->pos.x - (prevCapBoxEntity->size.x/2), currentAnimation->getFrame(*currentFrame)->f_anchor.y + prevCapBoxEntity->pos.y - (prevCapBoxEntity->size.y/2));
								  //above is the screen position of the anchor position under the previous capbox

								  if(!pointIsWithinCenteredBox(currentCapBoxEntity,prevAnchorScreenPos))//if it's not within the confines of the new CB
								  {
									  if(prevAnchorScreenPos->y < currentCapBoxEntity->pos.y - (currentCapBoxEntity->size.y/2))//if it's less than the left border of the CB
									  {
										  currentAnimation->getFrame(*currentFrame)->f_anchor.y = 0; //As the anchor is measure from BL of the frame, left hand side is 0
									  }
									  if(prevAnchorScreenPos->y > currentCapBoxEntity->pos.y + (currentCapBoxEntity->size.y/2))//if it's less than the left border of the CB
									  {
										  currentAnimation->getFrame(*currentFrame)->f_anchor.y = currentCapBoxEntity->size.y; //As the anchor is measure from BL of the frame, right hand side is = to the size
									  }
								  }

								  delete prevAnchorScreenPos;
							  }
							  

						  }
						  //clean up
						  delete prevCapBoxEntity;
						  delete currentCapBoxEntity;
						  delete prevCapBoxSize;
						  //redraw the viewers
						  redrawViewers();
						  printf("\nViews redrawn");
					  }
					  catch (FormatException^ e)
					  {
						  printf("\nNot a valid 3-digit number");
						  capBoxSize->y = 2; //Do not reset the text, but discretely set the size to 2 
						  loadIntoFrameDimsFromCB(); //The Same goes for the current frame f_dim value
						  //Textbox will still be empty, but capBox will not spaz-out

						  //Then redraw
						 redrawViewers();
					  }
					  catch (OverflowException^ e)
					  {
						  this->cbYSizeBox->Text = System::Convert::ToString(capBoxSize->y);
						  printf("\nNot a valid positive 3-digit number");
						  MessageBox::Show("Only Positive Integers Allowed (no symbols)","Capture Box Error",MessageBoxButtons::OK);
					  }

					  //Also lets make sure that this value cannot be zero or a negative number
					  if(capBoxSize->y <= 0)
					  {
						  printf("\nThe Capture Box has a 2px minimum");
						  capBoxSize->y = 2;
						  loadIntoFrameDimsFromCB(); //The Same goes for the current frame f_dim value
						  this->cbYSizeBox->Text = System::Convert::ToString(capBoxSize->y);
						  //redraw the viewers
						  redrawViewers();
						  printf("\nViews redrawn");
					  }


				  }




#pragma endregion

#pragma region SS VIEWER MOUSE FUNCTIONS

private: System::Void ssViewer_MouseMotion(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		 {
			 printf("\nSSV: Mouse Location: (%i, %i)",e->X,e->Y);
			 //update our mouse coord label
			 updateSSviewerCoordLabels(e);

			 //lets check our mouse coordinates against the spritesheet, and do stuff in the SSViewer if necessary
			 point* mousePos = new point(e->X,(SSVSizeY-e->Y));//DONT FORGET TO FLIP THE Y COORDS
			 
			 if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)
			 {
				sSheetViewer->mouseHoverCheck(mousePos);
			 }

			 sSheetViewer->drag(mousePos);//no need to check on the state of the viewer, drag does that for us
			 //if we're not in the 'dragging state/mode SSViewer::drag() simply does nothing #safeBlud
			 //same for updating the frame position
			 updateFramePosOnDrag();

			 redrawViewers();
			 delete mousePos;
		 }

private: System::Void updateSSviewerCoordLabels(System::Windows::Forms::MouseEventArgs^ e)
	   {
		   //take the int mouse coords and store them
		   String^ xVal = Convert::ToString(e->X);
		   
		   String^ yVal = Convert::ToString(SSVSizeY - e->Y);//FLIP the y coords. We draw from the bottom left, like normal people


		   //then simply put these into our labels for these
		   this->ssMouseCoordXLabel->Text = xVal;
		   this->ssMouseCoordYLabel->Text = yVal;
	   }

private: System::Void ssViewer_MouseExit(System::Object^ sender, System::EventArgs^ e)
		 {
			 //set them back to "----"
			 this->ssMouseCoordXLabel->Text = "----";
			 this->ssMouseCoordYLabel->Text = "----";

			 //also don't highlight the spritesheet anymore
			 if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)
			 {
				 sSheetViewer->DRAW_HB_FLAG = false;
				 sSheetViewer->MOUSE_HOVERED = false;

				 redrawViewers();
			 }

			 sSheetViewer->endDrag();//if we leave the context with the mouse, we ain't draggin nothin no more.
			 
		 }

private: System::Void ssViewer_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 point* mousePos = new point(e->X,(SSVSizeY-e->Y));//DONT FORGET TO FLIP THE Y COORDS
			 sSheetViewer->beginDrag(mousePos);
			 delete mousePos;

			 redrawViewers();
		 }

private: System::Void ssViewer_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 sSheetViewer->endDrag();
		 }


#pragma endregion
		 
#pragma region  F VIEWER MOUSE FUNCTIONS

		 private: System::Void fViewer_MouseMotion(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
				  {
					  printf("\nFV: Mouse Location: (%i, %i)",e->X,e->Y);
					  point* mousePos = new point(e->X,(FVSizeY-e->Y));//DONT FORGET TO FLIP THE Y COORDS
					  
					  
					  if (frameViewer->mode == FViewer::FV_MODE::EDIT_BOXES)//only if the mode is EDIT_BOXES
					  {
						  if(frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED && currentAnimation!= nullptr)
						  {
							  frameViewer->checkHovered(*mousePos, *currentAnimation->getFrame(*currentFrame));
						  }
					  }
					  
					  

					  redrawViewers();
					  delete mousePos;
				  }

		 private: System::Void fViewer_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
				  {
					  //create a point to rep the mousepos
					  point* mousePos = new point(e->X,(FVSizeY-e->Y));//DONT FORGET TO FLIP THE Y COORDS
					  printf("\nFV: Clicked Mouse Location: (%i, %i)",e->X,FVSizeY-e->Y);

					  if(frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED && currentAnimation!= nullptr)
					  {
						  switch (frameViewer->mode)
						  {
						  case FViewer::FV_MODE::ADD_HITBOX:
							  {
								  point* locOfNewHitBox = new point(mousePos->x - frameViewer->drawAnchorAt->x, mousePos->y - frameViewer->drawAnchorAt->y);//calculate the location of the new hitbox in relation to the anchor of the current frame
								  HitCircle* tempHitCircle = new HitCircle(*locOfNewHitBox, defRadius);//create a hitcircle to be copied into the hitboxlist
								  currentAnimation->getFrame(*currentFrame)->addHitBox(*tempHitCircle);
								  delete locOfNewHitBox;//cleanup
								  delete tempHitCircle;
								  break;
							  }
							 

						  case FViewer::FV_MODE::ADD_HURTBOX:
							  {
								  point* locOfNewHurtBox = new point(mousePos->x - frameViewer->drawAnchorAt->x, mousePos->y - frameViewer->drawAnchorAt->y);//calculate the location of the new hitbox in relation to the anchor of the current frame
								  HitCircle* tempHitCircle = new HitCircle(*locOfNewHurtBox, defRadius);//create a hitcircle to be copied into the hitboxlist
								  currentAnimation->getFrame(*currentFrame)->addHurtBox(*tempHitCircle);
								  delete locOfNewHurtBox;
								  delete tempHitCircle;
								  break;
							  }

						  case FViewer::FV_MODE::EDIT_BOXES:
							  {
								  //point* pointToCheck = new point(mousePos->x - frameViewer->drawAnchorAt->x, mousePos->y - frameViewer->drawAnchorAt->y);
								  /*bool shiftHeld;
								  if(Control.ModifierKeys == Keys.Control)
								  {
								  shiftHeld = true;
								  }
								  else{shiftHeld = false;}*/
								  frameViewer->checkIfSelecting(*mousePos,*currentAnimation->getFrame(*currentFrame), Control::ModifierKeys == Keys::Control);
								  //delete pointToCheck;
							  }
							 

						  case FViewer::FV_MODE::IDLE:
							  break;

						  default:
							  break;
						  }
					  }
					  delete mousePos;
				  }


#pragma endregion

#pragma region BUTTONS
private: System::Void cursorButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cursorButton->Checked)
			 {
				 sSheetViewer->mode = SSViewer::SSV_MODE::DRAG;//When the cursor button is pressed or changes state to 'down'
				 frameViewer->mode = FViewer::FV_MODE::EDIT_BOXES;//One button switches to generic arrow tool for both viewers
				 //we change the actual mode of the SSViewer
			 }
		 }

private: System::Void editCBButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 sSheetViewer->mode = SSViewer::SSV_MODE::CB_EDIT;//When the cbEdit button is pressed or changes state to 'down'
			 //we change the actual mode of the SSViewer

			 switch (cbAnimState)
			 {
			 case ANIM_STATE::AS_IN:
				 if (CBInTimer->Enabled) //if we're going in, reverse it
				 {
					 CBInTimer->Enabled = false;//stop the out animation
					 cbAnimState = ANIM_STATE::AS_OUT;//change to outward animation state
					 CBOutTimer->Enabled = true;//start animating
				 }
				 break;

			 case ANIM_STATE::AS_OUT:
				 if (CBOutTimer->Enabled)//if we're going out, reverese it
				 {
					 CBOutTimer->Enabled = false;//stop the outward animation
					 cbAnimState = ANIM_STATE::AS_IN;//change to inward animating
					 CBInTimer->Enabled = true;//start animating
				 }
				 break;

			 case ANIM_STATE::IDLE:
				 if(cbGroupBox->Location.X >= defaultCBGroupXOut)//if we're resting on the right hand position
				 {
					 cbAnimState = ANIM_STATE::AS_IN;//state = IN
					 CBInTimer->Enabled = true;
					 CBOutTimer->Enabled = false;
				 }
				 else
				 {
					 cbAnimState = ANIM_STATE::AS_OUT;//state = out
					 CBInTimer->Enabled = false;
					 CBOutTimer->Enabled = true;
				 }
			 }


		 }

private: System::Void moveAnchorButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(moveAnchorButton->Checked)
			 {
				 sSheetViewer->mode = SSViewer::SSV_MODE::MOVE_ANCHOR;//When the moveAnchor button is pressed or changes state to 'down'
				 //we change the actual mode of the SSViewer
			 }
		 }

private: System::Void addNewFrameButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation!=nullptr)//if we're in business
			 {
				point* screenSize = new point(SSVSizeX,SSVSizeY);//for debug purposes
				point* tempNewFrameBLloc = new point((screenSize->x/2-(capBoxSize->x/2)),(screenSize->y/2-(capBoxSize->y/2)));
				currentAnimation->newFrame(*currentFrame, tempNewFrameBLloc);//insert a new frame after the current frame, and change our current frame to that
				updateStateOnNewFrame();//perform all the necessary updates that will actually change the view to this new frame.

				this->cursorButton->Checked = true;//make the cursor button the currently checked button of all of the '1 at a time' checkable buttons
				//also switches the state to DRAG

				//cleanup
				delete tempNewFrameBLloc;
				delete screenSize;
			 }
		 }

private: System::Void eraseFrameButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(currentAnimation != nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)//if we're in business
			 {
				 currentAnimation->eraseFrame(*currentFrame);//erase the current frame, performing the necessary actions that take place if we erase the last remaining
				 //frame, or the end frame

				 if(currentAnimation->size() == 1)//if the erase func has left us with an empty list and created a default frame
				 {
					 point* screenSize = new point(SSVSizeX,SSVSizeY);//for debug purposes
					 point* tempNewFrameBLloc = new point((screenSize->x/2-(currentAnimation->getFrame(1)->f_dim.x/2)),(screenSize->y/2-(currentAnimation->getFrame(1)->f_dim.y/2)));
					 currentAnimation->getFrame(1)->f_loc = *tempNewFrameBLloc; //make the first frames pos = to the default frame pos we have created
					 updateStateOnNewFrame();
					 delete screenSize;
					 delete tempNewFrameBLloc;
				 }else{//if not just update the 'new frame' way
					 updateStateOnFrameChange();
				 }
				 
			 }
		 }

private: System::Void cloneFrameButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(currentAnimation != nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)//if we're in business
			 {
				 currentAnimation->cloneFrame(*currentFrame);//clone the current frame, and increment the currentFrame variable
				 updateStateOnFrameChange();
			 }
		 }



private: System::Void prevFramButton_Click(System::Object^  sender, System::EventArgs^  e) 
				  {
					  if(currentAnimation != nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)//if were in business
					  {
						  if(*currentFrame>1 && currentAnimation->size()>1)//if the currentFrame is beyond '1' (first frame) & there is more than one 1 frame in the animation
						  {
							  (*currentFrame)--;
							  updateStateOnFrameChange();

						  }
					  }

				  }

private: System::Void nextFrameButton_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(currentAnimation != nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED)//if were in business
			 {
				 if(*currentFrame<currentAnimation->size() && currentAnimation->size()>1)//if the current frame is less than the total of the Animations framelist
				 {//i.e. there are still frames left to skip to
					 (*currentFrame)++;
					 updateStateOnFrameChange();
				 }
			 }
		 }

private: System::Void saveAnimationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(currentAnimation != nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentSettings != nullptr)//if we're in business
			 {
				 //Create a save file dialog // for more details on how this works see the OpenFileDialog below
				 SaveFileDialog^ mySaveFileDialog = gcnew SaveFileDialog();//create a new saveFileDialogue
				 mySaveFileDialog->Title = "Save Animation As...";
				 mySaveFileDialog->Filter = "Spritey Files(.spr)|.spr|"+"All Files(*.*)|*.*";
				 mySaveFileDialog->FilterIndex = 1;
				 mySaveFileDialog->RestoreDirectory = true;
				 mySaveFileDialog->DefaultExt = "spr";
				 mySaveFileDialog->AddExtension = true;
				 mySaveFileDialog->OverwritePrompt = true;
				 
				 if(mySaveFileDialog->ShowDialog()==System::Windows::Forms::DialogResult::OK)
				 {
					 if (Path::GetExtension(mySaveFileDialog->FileName)!=nullptr)//if we have a valid file extension
					 {
						 //Marshal the String^ to an std string
						 msclr::interop::marshal_context context;
						 std::string stdstrFileName = context.marshal_as<std::string>(mySaveFileDialog->FileName);
						 SaveFile* testSave = new SaveFile(*currentAnimation, *currentSettings, stdstrFileName);//give it the object currently pointed-to by currentAnimation.
						 delete testSave;
					 }
					 
				 }
				 
				 


			 }

		 }

private: System::Void settingsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SettingsForm^ mySettingsForm = gcnew SettingsForm(currentSettings);//pass in our current settings by value
			 mySettingsForm->ShowDialog();//show our dialogue
			 switch (mySettingsForm->DialogResult)
			 {
			 case System::Windows::Forms::DialogResult::OK://if the APPLY button was clicked
				 delete currentSettings;//get rid of what's currently in our currentSettings.
				 currentSettings = new SpriteySettings(*mySettingsForm->currentSetLocCopy);//and reinitialise them with what our settings window currently had.
				 break;

			 case System::Windows::Forms::DialogResult::Cancel://if cancel was clicked 
				 break;//do absolutely nothing

			 default:
				 break;
			 }
			 
			 mySettingsForm->~SettingsForm();//mark the form for deletion

		 }

private: System::Void openAnimationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)//loads a previously saved animation
		 {
			 Stream^ myStream; //create a new stream to buffer to the chars returned by the OFD
			 OpenFileDialog^ myOpenFileDialog = gcnew OpenFileDialog(); //create a new dialogue box (does not display it)
			 //Interesting note: works with or without () on constructor??? for some reason.
			 myOpenFileDialog->Title = "Open Animation File";//OFD title
			 myOpenFileDialog->Filter = "Spritey files (*.spr)|*.spr|" + "All files (*.*)|*.*";//setup the filter
			 myOpenFileDialog->FilterIndex = 1;//default filter to .spr files


			 if(myOpenFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) //shows the dialog and reads in the return
			 {
				 if((myStream = myOpenFileDialog->OpenFile()) != nullptr)
				 {
					 String^ strFileName = myOpenFileDialog->InitialDirectory+ myOpenFileDialog->FileName; //store path of selected file as a string.
					 strFileName = strFileName->Replace("\\","/"); //replace all of the backslashes with forward slashes, as lodepng only seems to respect these
					 ///Testing
					 //MessageBox::Show("Path of selected file: "+strFileName->Replace("\\","/"));

					 //Marshal the String^ to an std string
					 msclr::interop::marshal_context context;
					 std::string stdstrFileName = context.marshal_as<std::string>(strFileName);

					 //catch exceptions too
					 try
					 {
						 //create a new instance of LoadFile to load the animation file contents
						 LoadFile* myLoadFile = new LoadFile(stdstrFileName);

						 if(currentFrame!=nullptr && sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation->getFrame(*currentFrame)!=nullptr)//if we're in business
						 {
							 delete currentSettings; // the current settings get deleted
							 delete currentAnimation; //the current animation gets deleted
							 currentAnimation = new Animation(myLoadFile->loadedAnimation);//copy the loadedAnimation into the currentAnimation
							 currentSettings = new SpriteySettings(myLoadFile->loadedSettings); //copy the myLoadFile loadedSettings value

							 *currentFrame = 1;//set current frame to the start
							 updateStateOnNewFrame();
						 }

						 delete myLoadFile;
					 }

					 catch (SpriteyData::invalExtException& e)
					 {
						 printf("\nERROR: Not a valid file extension");
						 MessageBox::Show(gcnew String(e.what()),"Invalid File Type",MessageBoxButtons::OK);
					 }

					 catch (SpriteyData::corruptHeaderException& e)
					 {
						 printf("\nERROR: Header is corrupt");
						 MessageBox::Show(gcnew String(e.what()),"File Corrupt",MessageBoxButtons::OK);
					 }

					 catch (SpriteyData::corruptAnimDataException& e)
					 {
						 printf("\nERROR: Animation Data is corrupt");
						 MessageBox::Show(gcnew String(e.what()),"File Corrupt",MessageBoxButtons::OK);
					 }

					 catch (SpriteyData::emptyFileException& e)
					 {
						 printf("\nERROR: File is empty");
						 MessageBox::Show(gcnew String(e.what()),"File Corrupt",MessageBoxButtons::OK);
					 }

				 }

			 }
		 }

private: System::Void addHitBoxButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(addHitBoxButton->Checked)
			 {
				 frameViewer->mode = FViewer::FV_MODE::ADD_HITBOX;
			 }
		 }
private: System::Void addHurtBoxButton_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(addHurtBoxButton->Checked)
			 {
				 frameViewer->mode = FViewer::FV_MODE::ADD_HURTBOX;
			 }
		 }

private: System::Void resetFrameButton_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && currentAnimation!=nullptr)
			 {
				 currentAnimation->getFrame(*currentFrame)->f_loc = point(-1*((currentAnimation->getFrame(*currentFrame)->f_dim.x/2)-(SSVSizeX/2)),-1*((currentAnimation->getFrame(*currentFrame)->f_dim.y/2)-(SSVSizeY/2)));
				 updateStateOnFrameChange();
			 }
		 }


#pragma endregion

#pragma region KEYBOARD
protected: System::Void MoveAchorArrowFunc(Message% msg, Keys keyData)
		   {
			   //Functionality for WM_KEYDOWN
			   if(msg.Msg==WM_KEYDOWN && sSheetViewer->loadedMode==SSViewer::SS_LOADED_MODE::SHEET_LOADED && sSheetViewer->mode==SSViewer::SSV_MODE::MOVE_ANCHOR && currentAnimation!=nullptr && currentAnimation!=NULL)
			   {
				   entity* capBoxEntity = new entity(point(SSVSizeX/2,SSVSizeY/2), point(capBoxSize->x, capBoxSize->y));//create an entity to represent the capbox's values on screen (pos & dims)
				   point* moveAnchorTo = new point(currentAnimation->getFrame(*currentFrame)->f_anchor.x + (capBoxEntity->pos.x-(capBoxEntity->size.x/2)), currentAnimation->getFrame(*currentFrame)->f_anchor.y + (capBoxEntity->pos.y-(capBoxEntity->size.y/2)));//create a point NOT identical to the current anchor pos, but to the anchor pos on screen. 
				   //in order to do this, we need to ADD the location of the bottom left hand corner of the capBox. As the anchorPos is saved as the translation from the BL CB corner to the anchor
				   //VERY non-verbose i know, but i should be used to understanding code like this by now.  

				   //Which we will change depending on which key is pressed
				   switch(keyData)
				   {
				   case Keys::Up:
					   moveAnchorTo->y += 1;
					   break;

				   case Keys::Down:
					   moveAnchorTo->y -= 1;
					   break;

				   case Keys::Right:
					   moveAnchorTo->x += 1;
					   break;

				   case Keys::Left:
					   moveAnchorTo->x -= 1;
					   break;

				   default:
					   break;

				   }


				   if(!currentSettings->isAnchorDevAllowed())//if we're disabling Anchor Deviation
				   {	//we check to see if it's going to be in the box first. And THEN move it.
					   if (pointIsWithinCenteredBox(capBoxEntity,moveAnchorTo)) //if checking whether the possible pos is within the box returns true
					   {
						   //DONT FORGET TO REVERSE THE ADDITIONS MADE TO GET THE ANCHOR SCREEN POS FROM THE BL CORNER OF THE CAPBOX
						   moveAnchorTo->x = moveAnchorTo->x - (capBoxEntity->pos.x-(capBoxEntity->size.x/2));
						   moveAnchorTo->y = moveAnchorTo->y - (capBoxEntity->pos.y-(capBoxEntity->size.y/2));
						   currentAnimation->getFrame(*currentFrame)->f_anchor = moveAnchorTo; ///it now becomes the anchor position for the current frame. 


					   }else
					   {

						   *moveAnchorTo= currentAnimation->getFrame(*currentFrame)->f_anchor; //if not it reverts back to the previous anchorPos (just for neatness)
					   }
				   }else{//if Anchor deviation is enabled, we just move it.
					   moveAnchorTo->x = moveAnchorTo->x - (capBoxEntity->pos.x-(capBoxEntity->size.x/2));
					   moveAnchorTo->y = moveAnchorTo->y - (capBoxEntity->pos.y-(capBoxEntity->size.y/2));
					   currentAnimation->getFrame(*currentFrame)->f_anchor = moveAnchorTo; ///it now becomes the anchor position for the current frame. 
				   }
				   

				   //update the anchorPosLabels 
				   updateAnchorPosLabels();
				   
				   ////redrawing is now done in the umbrella function ProcessCMDKeys() below, to ensure that there is only ONE redraw per
				   ////per-key press
				   //redrawViewers();
				   //clean up the pointers
				   delete capBoxEntity;
				   delete moveAnchorTo;
			   }
		   }

protected: System::Void MoveHBoxesArrowFunc(Message% msg, Keys keyData)
		   {
			   //Functionality for WM_KEYDOWN
			   if(msg.Msg==WM_KEYDOWN && frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED && frameViewer->mode == FViewer::FV_MODE::EDIT_BOXES && currentAnimation!=nullptr && currentAnimation!=NULL)
			   {
				   
				   if (Control::ModifierKeys == Keys::ShiftKey || Control::ModifierKeys == Keys::Shift)
				   {

				   }

				   switch(keyData)
				   {
				   case Keys::Up | Keys::Shift:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, true, FViewer::DIRECTION::UP);
					   break;

				   case Keys::Up:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, false, FViewer::DIRECTION::UP);
					   break;

				   case Keys::Down | Keys::Shift:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, true, FViewer::DIRECTION::DOWN);
					   break;

				   case Keys::Down:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, false, FViewer::DIRECTION::DOWN);
					   break;

				   case Keys::Right | Keys::Shift:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, true, FViewer::DIRECTION::RIGHT);
					   break;

				   case Keys::Right:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, false, FViewer::DIRECTION::RIGHT);
					   break;

				   case Keys::Left | Keys::Shift:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, true, FViewer::DIRECTION::LEFT);
					   break;

				   case Keys::Left:
					   frameViewer->moveAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), defaultHitboxStep, false, FViewer::DIRECTION::LEFT);
					   break;

				   case Keys::Delete:
					   frameViewer->deleteAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame));
					   break;

				   case Keys::Oemplus:
					   frameViewer->resizeAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), 1);
					   break;

				   case Keys::OemMinus:
					   frameViewer->resizeAllSelectedBoxes(*currentAnimation->getFrame(*currentFrame), -1);
					   break;

				   default:
					   break;

				   }
			   }
		   }

		   

protected: virtual bool ProcessCmdKey(Message% msg, Keys keyData) override
		   {
			   //Junction the different states for which we wish to accommodate with a switch statement
			   if(sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED && moveAnchorButton->Checked)
			   {
				   switch(sSheetViewer->mode)
				   {
				   case SSViewer::SSV_MODE::MOVE_ANCHOR:
					   MoveAchorArrowFunc(msg, keyData);
						   break;
				   default:
					   break;
				   }

				   
			   }

			   if(frameViewer->loadedMode == FViewer::FV_LOADED_MODE::SHEET_LOADED && cursorButton->Checked)
			   {
				   switch(frameViewer->mode)
				   {
				   case FViewer::FV_MODE::EDIT_BOXES:
					   MoveHBoxesArrowFunc(msg, keyData);
					   break;
				   default:
					   break;
				   }
			   }

			   //redraw
			   redrawViewers();

			   //What we need to realize is that by overriding ProcessCmdKey, we abandon all the basic functions that this function has for ALL other keys
			   //in order to reinstate these, while retaining our added functionality, we call the Base class's version within our orverridden version (at the end).

			   //HOWEVER: as we want all the functionailty of the keys to cease when we're editing the anchor-point, and our focus is on that. 
			   //We may as well only call the Base func and get the other keys involved when our state is not MOVE_ANCHOR.
			   //UPDATE: added to this is now a check for FV_MODE::EDIT_BOXES. So now If either EDIT_BOXES or MOVE_ANCHOR are true, 
			   //we do not cascade through to the basic keyboard functionality, and it remains locked.
			   if(sSheetViewer->mode==SSViewer::SSV_MODE::MOVE_ANCHOR || frameViewer->mode==FViewer::FV_MODE::EDIT_BOXES)
			   {
					return true;// if it is MOVE_ANCHOR, we'll simply continue freezing out other keyboard functionality, and just return true
			   }else
			   {
				   return System::Windows::Forms::Form::ProcessCmdKey(msg, keyData);
				   
			   }

		   }

private: System::Void SpriteyForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		 {
			 ////////CURRENTLY UNUSED
			 if(sSheetViewer->loadedMode==SSViewer::SS_LOADED_MODE::SHEET_LOADED && sSheetViewer->mode==SSViewer::SSV_MODE::MOVE_ANCHOR && currentAnimation!=nullptr && currentAnimation!=NULL)
			 {


				 entity* capBoxEntity = new entity(point(SSVSizeX,SSVSizeY), point(capBoxSize->x, capBoxSize->y));//create an entity to represent the capbox's values on screen (pos & dims)
				 point* moveAnchorTo = new point(currentAnimation->getFrame(*currentFrame)->f_anchor);//create a point identical to the current anchor pos. 
				 //Which we will change depending on which key is pressed
				 switch(e->KeyCode)
				 {
				 case Keys::Up:
					 moveAnchorTo->y += 1;
					 break;

				 case Keys::Down:
					 moveAnchorTo->y -= 1;
					 break;

				 case Keys::Right:
					 moveAnchorTo->x += 1;
					 break;

				 case Keys::Left:
					 moveAnchorTo->x -= 1;
					 break;

				 default:
					 break;

				 }

				 if (pointIsWithinCenteredBox(capBoxEntity,moveAnchorTo)) //if checking whether the possible pos is within the box returns true
				 {
					 currentAnimation->getFrame(*currentFrame)->f_anchor = moveAnchorTo; //it now becomes the anchor position for the current frame

				 }else
				 {

					 *moveAnchorTo= currentAnimation->getFrame(*currentFrame)->f_anchor; //if not it reverts back to the previous anchorPos (just for neatness)
				 }
				 //redraw
				 redrawViewers();
				 //clean up the pointers
				 delete capBoxEntity;
				 delete moveAnchorTo;
			 }
		 }

		 private: System::Void cbSizeBox_EnterFocus(System::Object^  sender, System::EventArgs^  e) 
				  {
					  if((sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::SHEET_LOADED || sSheetViewer->loadedMode == SSViewer::SS_LOADED_MODE::HOLDER)  && (sSheetViewer->mode == SSViewer::SSV_MODE::MOVE_ANCHOR||frameViewer->mode == FViewer::FV_MODE::EDIT_BOXES))//if we enter the focus of this TB, and we're in MOVE_ANCHOR mode
					  {
						  sSheetViewer->mode = SSViewer::SSV_MODE::IDLE;//leave MOVE_ANCHOR mode. 
						  //This means we don't have to manually switch away from MOVE_ANCHOR mode every time we 
						  //wish to readjust the CB and MA mode is enabled

						  //The exact same for the Fviewer and EDIT_BOXES state.
						  frameViewer->mode = FViewer::FV_MODE::IDLE;

						  this->moveAnchorButton->Checked = false;//uncheck the button
					  }
				  }

#pragma endregion 

//////////ANIMATION FUNCTIONS//////////////
//DONT ASK QUESTIONS ABOUT WHAT FOLLOWS!!!!
//JUST ACCEPT THAT IT WORKS!!!!!!!!!!!
//In case of emergency, comment out this entire region
//And remove all events from all timers on the forms designer
//also remove the FViwer onMouseEnter & Leave functions, and everything will return to normal.
//This is the only part of the project that I'm not-so proud of. But due to time constraints it cannot be helped.
//Still looks pretty though :)
/////////////////////////////////////

#pragma region ANIMATION TIMERS

private: System::Void CBOutTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(CBInTimer->Enabled){CBInTimer->Enabled=false;} //disable opposite timer
			 //the function that happens to animate thee cbGroubBox out
			 if(cbGroupBox->Location.X<=defaultCBGroupXOut-2)//if were left of the target location (off-screen)
			 {
				 cbAnimState = ANIM_STATE::AS_OUT;
				 int diff = defaultCBGroupXOut - defaultCBGroupXIn;
				 double step = ceil(static_cast<long double>(diff/15));//we want this to take 1 second at 30 fps
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(cbGroupBox->Location.X+step, cbGroupBox->Location.Y);

				 cbGroupBox->Location = *newLoc;//ceiling to make sure we dont get 0
				 //this->Invalidate();
			 }
			 if(cbGroupBox->Location.X>defaultCBGroupXOut-2)//if we're right of the target location 
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultCBGroupXOut, cbGroupBox->Location.Y);
				 cbGroupBox->Location = *newLoc;//ceiling to make sure we dont get 0
				 CBOutTimer->Enabled = false;
				 cbAnimState = ANIM_STATE::IDLE;
			 }
		 }

private: System::Void CBInTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(CBOutTimer->Enabled){CBOutTimer->Enabled=false;} //disable opposite timer
			 if(cbGroupBox->Location.X>=defaultCBGroupXIn+2)//if were left of the target location (off-screen)
			 {
				 cbAnimState = ANIM_STATE::AS_IN;
				 int diff = defaultCBGroupXOut - defaultCBGroupXIn;
				 double step = ceil(static_cast<long double>(diff/10));//we want this to take 1 second at 30 fps
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(cbGroupBox->Location.X-step, cbGroupBox->Location.Y);

				 cbGroupBox->Location = *newLoc;//btw: ceiling to make sure we dont get 0
				 //this->Invalidate();
			 }
			 if(cbGroupBox->Location.X<defaultCBGroupXIn+2)//if we're right of the target location 
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultCBGroupXIn, cbGroupBox->Location.Y);
				 cbGroupBox->Location = *newLoc;//ceiling to make sure we dont get 0
				 CBInTimer->Enabled = false;
				 cbAnimState = ANIM_STATE::IDLE;
			 }
		 }

private: System::Void HBInTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(HBOutTimer->Enabled){HBOutTimer->Enabled = false;}//disable the opposite
			 if(addHitBoxButton->Location.X>=defaultHBXIn+2)
			 {
				 hbAnimState = ANIM_STATE::AS_IN;//making sure our animState is correct
				 int diff = defaultHBXOut-defaultHBXIn;//calculate the gap between the start and dest
				 double step = ceil(static_cast<long double>(diff/10));//how far should we move each frame
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(addHitBoxButton->Location.X - step, addHitBoxButton->Location.Y);

				 addHitBoxButton->Location = *newLoc;

				 if (addHurtBoxButton->Location.X == defaultHuBXOut)//THIS ANCHORS THEM TOGETHER
				 {
					 HuBInTimer->Enabled = true;//THIS ENSURES THAT THE FIRST BOX NEVER GOES WITHOUT THE 2ND
				 }
			 }
			 if (addHitBoxButton->Location.X<defaultHBXIn+2)//if we've reached our destination
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultHBXIn, addHitBoxButton->Location.Y);
				 addHitBoxButton->Location = *newLoc;
				 HBInTimer->Enabled = false;//this timer ceases
				 hbAnimState = ANIM_STATE::IDLE;
			 }
		 }

private: System::Void HBOutTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(HBInTimer->Enabled){HBInTimer->Enabled = false;}//disable the opposite
			 if(addHitBoxButton->Location.X<=defaultHBXOut-2)
			 {
				 hbAnimState = ANIM_STATE::AS_OUT;//making sure our animState is correct
				 int diff = defaultHBXOut-defaultHBXIn;//calculate the gap between the start and dest
				 double step = ceil(static_cast<long double>(diff/10));//how far should we move each frame
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(addHitBoxButton->Location.X + step, addHitBoxButton->Location.Y);

				 addHitBoxButton->Location = *newLoc;

				 if (!HuBDelayTimer->Enabled && addHurtBoxButton->Location.X == defaultHuBXIn)//THIS ANCHORS THEM TOGETHER
				 {
					 HuBOutTimer->Enabled = true;//THIS ENSURES THAT THE FIRST BOX NEVER GOES WITHOUT THE 2ND
				 }
			 }
			 if (addHitBoxButton->Location.X>defaultHBXOut-2)//if we've reached our destination
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultHBXOut, addHitBoxButton->Location.Y);
				 addHitBoxButton->Location = *newLoc;
				 HBInTimer->Enabled = false;//this timer ceases
				 hbAnimState = ANIM_STATE::IDLE;
			 }
		 }

private: System::Void HuBInTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(HuBOutTimer->Enabled){HuBOutTimer->Enabled = false;}//disable the opposite
			 if(addHurtBoxButton->Location.X>=defaultHuBXIn+2)
			 {
				 hubAnimState = ANIM_STATE::AS_IN;//making sure our animState is correct
				 int diff = defaultHuBXOut-defaultHuBXIn;//calculate the gap between the start and dest
				 double step = ceil(static_cast<long double>(diff/10));//how far should we move each frame
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(addHurtBoxButton->Location.X - step, addHurtBoxButton->Location.Y);

				 addHurtBoxButton->Location = *newLoc;
			 }
			 if (addHurtBoxButton->Location.X<defaultHBXIn+2)//if we've reached our destination
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultHuBXIn, addHurtBoxButton->Location.Y);
				 addHurtBoxButton->Location = *newLoc;
				 HuBInTimer->Enabled = false;//this timer ceases
				 hubAnimState = ANIM_STATE::IDLE;
			 }
			 if(addHitBoxButton->Location.X == defaultHBXIn && addHurtBoxButton->Location.X == defaultHuBXIn)
			 {
				 //if theyre both were theyre meant to be, stop all timers until we next leave the fviewr
				 //because this is executed last in this func, this should also avoid stopping the timer flow before it starts
				 stopAllAnims();
			 }
		 }

private: System::Void HuBOutTimer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(HuBInTimer->Enabled){HuBInTimer->Enabled = false;}//disable the opposite
			 if(addHurtBoxButton->Location.X<=defaultHuBXOut-2)
			 {
				 hubAnimState = ANIM_STATE::AS_OUT;//making sure our animState is correct
				 int diff = defaultHuBXOut-defaultHuBXIn;//calculate the gap between the start and dest
				 double step = ceil(static_cast<long double>(diff/10));//how far should we move each frame
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(addHurtBoxButton->Location.X + step, addHurtBoxButton->Location.Y);

				 addHurtBoxButton->Location = *newLoc;
			 }
			 if (addHurtBoxButton->Location.X>defaultHuBXOut-2)//if we've reached our destination
			 {
				 //end timer
				 System::Drawing::Point^ newLoc = gcnew System::Drawing::Point(defaultHuBXOut, addHurtBoxButton->Location.Y);
				 addHurtBoxButton->Location = *newLoc;
				 HuBOutTimer->Enabled = false;//this timer ceases
				 hubAnimState = ANIM_STATE::IDLE;
			 }
			 
		 }
	
private: System::Void HuBDelayTimer_Tick(System::Object^  sender, System::EventArgs^  e)//delayed function is triggered when the addHitBoxButton first starts moving
		 {//causes a delay in the way in which the two buttons animate on/off screen

			 switch (hbAnimState)//test the anim_state
			 {
			 case ANIM_STATE::AS_IN://if it's animating in 
				 hubAnimState = ANIM_STATE::AS_IN;
				 HuBInTimer->Enabled = true;
				 break;

			 case ANIM_STATE::AS_OUT://if its going out
				 hubAnimState = ANIM_STATE::AS_OUT;
				 HuBOutTimer->Enabled = true;
				 break;

			 default: 
				 break;
			 }
		 }

private: System::Void HBOutDelay_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //a timer that starts when the mouse leaves the confines of the FVPanel, and when it ticks it starts off the AddHBButton animating out
			 //if(addHitBoxButton->Location.X < defaultHBXIn+2)//also if this condition is not met, this timer will continue to run
			 //{
			 if(!HBOutTimer->Enabled)
			 {
				 HBOutTimer->Enabled = true;
				 hbAnimState = ANIM_STATE::AS_OUT;
			 }//start the hb animating
			 //HuBDelayTimer->Enabled = true;//start the hurtboxbutton out delay timer
			 //HBOutDelay->Enabled = true;//only if this condition is met does this timer ever stop
			 //}
		 }


private: System::Void fViewerPlaceholderPanel_MouseEnter(System::Object^  sender, System::EventArgs^  e) 
		 {
			 stopAllAnims();
			 //as the mouse enters this control, I want the two HB buttons to animate onto screen (if they aren't both already there)
			 if (addHitBoxButton->Location.X != defaultHBXIn)
			 {
				 HBInTimer->Enabled = true;//start the HB animating in
				 hbAnimState = ANIM_STATE::AS_IN;
				 HuBDelayTimer->Enabled = true;//the delayed entry of the AHUBUTT is determined
			 }

		 }
private: System::Void fViewerPlaceholderPanel_MouseLeave(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //when the mouse leaves the control we want the timer for the delayed exit of the buttons to begin
			 HBOutDelay->Enabled = true;
		 }

private: System::Void stopAllAnims()
		 {
			 HuBInTimer->Enabled = false;
			 HBInTimer->Enabled = false;
			 HuBOutTimer->Enabled = false;
			 HBOutTimer->Enabled = false;
			 HuBDelayTimer->Enabled = false;
			 HBOutDelay->Enabled = false;
		 }

#pragma endregion


};
}

