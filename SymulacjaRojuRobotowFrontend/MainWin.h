#pragma once
#include <vector>
#include "WrapperSwarm.h"

// ============================================================
//  WYBÓR ALGORYTMU — zmieñ tê liniê ¿eby wybraæ tryb startowy:
//
//    WrapperSwarm::PATTERN_NONE     -> swobodny flocking (³awica)
//    WrapperSwarm::PATTERN_CIRCLE   -> formacja okr¹g
//    WrapperSwarm::PATTERN_SQUARE   -> formacja kwadrat
//    WrapperSwarm::PATTERN_HEX_GRID -> formacja siatka hex
// ============================================================
static const int INITIAL_PATTERN = WrapperSwarm::PATTERN_HEX_GRID;

namespace SymulacjaRojuRobotowFrontend {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainWin : public System::Windows::Forms::Form
	{
	private:
		Generic::List<PictureBox^>^ robotsPB = gcnew Generic::List<PictureBox^>();
		WrapperSwarm^ wSwarm;

	private: System::Windows::Forms::ToolStripMenuItem^ stopToolStripMenuItem;

		   double time = 0;

	public:
		MainWin(void)
		{
			InitializeComponent();
			wSwarm = gcnew WrapperSwarm(490, 379, 0);

			// Ustaw algorytm startowy — zmieñ INITIAL_PATTERN na górze pliku
			if (INITIAL_PATTERN != WrapperSwarm::PATTERN_NONE)
				wSwarm->setPattern(INITIAL_PATTERN);
		}

	protected:
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

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWin::typeid));
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->symulacjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->startToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->edycjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->robotToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->dodajRobotaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			   this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			   this->menuStrip1->SuspendLayout();
			   this->SuspendLayout();
			   // menuStrip1
			   this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			   this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->symulacjaToolStripMenuItem,
					   this->edycjaToolStripMenuItem
			   });
			   this->menuStrip1->Location = System::Drawing::Point(0, 0);
			   this->menuStrip1->Name = L"menuStrip1";
			   this->menuStrip1->Size = System::Drawing::Size(653, 28);
			   this->menuStrip1->TabIndex = 0;
			   this->menuStrip1->Text = L"menuStrip1";
			   // symulacjaToolStripMenuItem
			   this->symulacjaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->startToolStripMenuItem,
					   this->stopToolStripMenuItem
			   });
			   this->symulacjaToolStripMenuItem->Name = L"symulacjaToolStripMenuItem";
			   this->symulacjaToolStripMenuItem->Size = System::Drawing::Size(90, 24);
			   this->symulacjaToolStripMenuItem->Text = L"Symulacja";
			   // startToolStripMenuItem
			   this->startToolStripMenuItem->Name = L"startToolStripMenuItem";
			   this->startToolStripMenuItem->Size = System::Drawing::Size(123, 26);
			   this->startToolStripMenuItem->Text = L"Start";
			   this->startToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::startToolStripMenuItem_Click);
			   // stopToolStripMenuItem
			   this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
			   this->stopToolStripMenuItem->Size = System::Drawing::Size(123, 26);
			   this->stopToolStripMenuItem->Text = L"Stop";
			   this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::stopToolStripMenuItem_Click);
			   // edycjaToolStripMenuItem
			   this->edycjaToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->robotToolStripMenuItem });
			   this->edycjaToolStripMenuItem->Name = L"edycjaToolStripMenuItem";
			   this->edycjaToolStripMenuItem->Size = System::Drawing::Size(66, 24);
			   this->edycjaToolStripMenuItem->Text = L"Edycja";
			   // robotToolStripMenuItem
			   this->robotToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->dodajRobotaToolStripMenuItem });
			   this->robotToolStripMenuItem->Name = L"robotToolStripMenuItem";
			   this->robotToolStripMenuItem->Size = System::Drawing::Size(133, 26);
			   this->robotToolStripMenuItem->Text = L"Robot";
			   // dodajRobotaToolStripMenuItem
			   this->dodajRobotaToolStripMenuItem->Name = L"dodajRobotaToolStripMenuItem";
			   this->dodajRobotaToolStripMenuItem->Size = System::Drawing::Size(182, 26);
			   this->dodajRobotaToolStripMenuItem->Text = L"Dodaj robota";
			   this->dodajRobotaToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::dodajRobotaToolStripMenuItem_Click);
			   // imageList1
			   this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"imageList1.ImageStream")));
			   this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			   this->imageList1->Images->SetKeyName(0, L"robot.png");
			   this->imageList1->Images->SetKeyName(1, L"dot.png");
			   this->imageList1->Images->SetKeyName(2, L"bigDot.png");
			   // timer1
			   this->timer1->Tick += gcnew System::EventHandler(this, &MainWin::timer1_Tick);
			   // MainWin
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(653, 466);
			   this->Controls->Add(this->menuStrip1);
			   this->MainMenuStrip = this->menuStrip1;
			   this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			   this->Name = L"MainWin";
			   this->Text = L"MainWin";
			   this->Load += gcnew System::EventHandler(this, &MainWin::MainWin_Load);
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
		pb->Image = imageList1->Images[2];
		pb->Location = Point(50 + (10 + 50) * robotsPB->Count, 50);
		pb->Name = L"robot" + Convert::ToString(robotsPB->Count);
		this->Controls->Add(pb);
		robotsPB->Add(pb);
	}

	private: Void removeRobotImg()
	{
		if (robotsPB->Count == 0) return;
		PictureBox^ pb = robotsPB[robotsPB->Count - 1];
		this->Controls->Remove(pb);
		robotsPB->RemoveAt(robotsPB->Count - 1);
	}

	private: System::Void dodajRobotaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		wSwarm->addRobot();
		addRobotImg();
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		double dt = double(timer1->Interval) / 1000.0;
		time += dt;

		wSwarm->update(dt);
		List<Tuple<float, float>^>^ positions = wSwarm->getSwarmPositions();

		for (int i = 0; i < positions->Count && i < robotsPB->Count; i++)
		{
			robotsPB[i]->Location = Point((int)positions[i]->Item1, (int)positions[i]->Item2);
		}
	}

	private: System::Void MainWin_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
