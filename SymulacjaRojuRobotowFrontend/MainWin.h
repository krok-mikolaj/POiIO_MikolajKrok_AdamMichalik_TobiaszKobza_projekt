#pragma once
#include <vector>
#include "Robot.h"

namespace SymulacjaRojuRobotowFrontend {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Podsumowanie informacji o MainWin
	/// </summary>
	public ref class MainWin : public System::Windows::Forms::Form
	{
	private:
		Generic::List<PictureBox^>^ robots = gcnew Generic::List<PictureBox^>();
		
	private: System::Windows::Forms::ToolStripMenuItem^ stopToolStripMenuItem;


		//timer
		double time = 0;

	public:
		MainWin(void)
		{
			InitializeComponent();
			robots_pnt = new std::vector<Robot*>();
			//
			//TODO: W tym miejscu dodaj kod konstruktora
			//
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MainWin()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ symulacjaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ startToolStripMenuItem;

	private: System::Windows::Forms::ImageList^ imageList1;
	private: System::Windows::Forms::ToolStripMenuItem^ edycjaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ robotToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ dodajRobotaToolStripMenuItem;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWin::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->symulacjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->edycjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->robotToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dodajRobotaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->symulacjaToolStripMenuItem,
					this->edycjaToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(490, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// symulacjaToolStripMenuItem
			// 
			this->symulacjaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->startToolStripMenuItem,
					this->stopToolStripMenuItem
			});
			this->symulacjaToolStripMenuItem->Name = L"symulacjaToolStripMenuItem";
			this->symulacjaToolStripMenuItem->Size = System::Drawing::Size(73, 20);
			this->symulacjaToolStripMenuItem->Text = L"Symulacja";
			// 
			// startToolStripMenuItem
			// 
			this->startToolStripMenuItem->Name = L"startToolStripMenuItem";
			this->startToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->startToolStripMenuItem->Text = L"Start";
			this->startToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::startToolStripMenuItem_Click);
			// 
			// edycjaToolStripMenuItem
			// 
			this->edycjaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->robotToolStripMenuItem });
			this->edycjaToolStripMenuItem->Name = L"edycjaToolStripMenuItem";
			this->edycjaToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->edycjaToolStripMenuItem->Text = L"Edycja";
			// 
			// robotToolStripMenuItem
			// 
			this->robotToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->dodajRobotaToolStripMenuItem });
			this->robotToolStripMenuItem->Name = L"robotToolStripMenuItem";
			this->robotToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->robotToolStripMenuItem->Text = L"Robot";
			// 
			// dodajRobotaToolStripMenuItem
			// 
			this->dodajRobotaToolStripMenuItem->Name = L"dodajRobotaToolStripMenuItem";
			this->dodajRobotaToolStripMenuItem->Size = System::Drawing::Size(143, 22);
			this->dodajRobotaToolStripMenuItem->Text = L"Dodaj robota";
			this->dodajRobotaToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::dodajRobotaToolStripMenuItem_Click);
			// 
			// imageList1
			// 
			this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"imageList1.ImageStream")));
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList1->Images->SetKeyName(0, L"robot.png");
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MainWin::timer1_Tick);
			// 
			// stopToolStripMenuItem
			// 
			this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
			this->stopToolStripMenuItem->Size = System::Drawing::Size(180, 22);
			this->stopToolStripMenuItem->Text = L"Stop";
			this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::stopToolStripMenuItem_Click);
			// 
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(490, 379);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainWin";
			this->Text = L"MainWin";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void startToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		time = 0;
		timer1->Interval = 10;
		timer1->Enabled = true;
	}
	private: System::Void stopToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		timer1->Enabled = false;
	}
	private: Void addRobotImg()
	{
		PictureBox^ pb = gcnew PictureBox(); 
		pb->Size = Drawing::Size(50, 50); 
		pb->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize; 
		pb->Image = imageList1->Images[0];

		//pb->Location = Point(50, 50); 
		pb->Location = Point(50 + (10 + 50) * robots->Count, 50);
		pb->Name = L"robot" + Convert::ToString(robots->Count); 

		this->Controls->Add(pb); 
		robots->Add(pb); 
	}
private: System::Void dodajRobotaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	addRobot();
	addRobotImg();
}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
	double dt = double(timer1->Interval) / 1000;
	time += dt;

	for (int i = 0; i < robots_pnt->size(); i++) 
	{
		(*robots_pnt)[i]->updatePos(dt);

		int x = (int)(*robots_pnt)[i]->getXPosition();
		int y = (int)(*robots_pnt)[i]->getYPosition();

		robots[i]->Location = Point(x, y);
	}
}
private: Void addRobot() {
	Robot* robot = new Robot(50+(10+50)*robots_pnt->size(), 50, 0, 10, 100);
	robots_pnt->push_back(robot);
}

};
}
