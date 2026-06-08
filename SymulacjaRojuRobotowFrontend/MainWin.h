#pragma once
#include <vector>
#include "WrapperSwarm.h"

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
		Panel^ renderPanel;
		Bitmap^ originalRobotImg;
		//Generic::List<PictureBox^>^ robotsPB = gcnew Generic::List<PictureBox^>();
		WrapperSwarm^ wSwarm;
	private: System::Windows::Forms::ToolStripMenuItem^ symulacjaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ startToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ stopToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ edycjaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ robotToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ dodajRobotaToolStripMenuItem;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;




		//timer
		double time = 0;

	public:
		MainWin(void)
		{
			InitializeComponent();
			wSwarm = gcnew WrapperSwarm(490, 379, 0);

			//
			//TODO: W tym miejscu dodaj kod konstruktora
			//
			originalRobotImg = gcnew Bitmap(imageList1->Images[3]);
			

			this->renderPanel = gcnew Panel();
			this->renderPanel->Location = Point(0, 24);  // pod menu
			this->renderPanel->Size = Drawing::Size(490, 355);
			this->renderPanel->BackColor = Color::White;  // lub dowolne t³o symulacji
			this->renderPanel->Paint += gcnew PaintEventHandler(this, &MainWin::renderPanel_Paint);
			this->renderPanel->GetType()->GetProperty("DoubleBuffered",
				System::Reflection::BindingFlags::Instance |
				System::Reflection::BindingFlags::NonPublic)
				->SetValue(this->renderPanel, true, nullptr);
			
			this->Controls->Add(this->renderPanel);

			this->Resize += gcnew EventHandler(this, &MainWin::MainWin_Resize);
			this->KeyPreview = true;

			this->renderPanel->MouseDown += gcnew MouseEventHandler(this, &MainWin::renderPanel_MouseDown);
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




	private: System::Windows::Forms::ImageList^ imageList1;



	private: System::Windows::Forms::Timer^ timer1;
	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		
		//private: System::Void renderPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWin::typeid));
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->symulacjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->stopToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->edycjaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->robotToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dodajRobotaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// imageList1
			// 
			this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"imageList1.ImageStream")));
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList1->Images->SetKeyName(0, L"robot.png");
			this->imageList1->Images->SetKeyName(1, L"dot.png");
			this->imageList1->Images->SetKeyName(2, L"bigDot.png");
			this->imageList1->Images->SetKeyName(3, L"robot_arrow.png");
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MainWin::timer1_Tick);
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
			this->startToolStripMenuItem->Size = System::Drawing::Size(98, 22);
			this->startToolStripMenuItem->Text = L"Start";
			this->startToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::startToolStripMenuItem_Click);
			// 
			// stopToolStripMenuItem
			// 
			this->stopToolStripMenuItem->Name = L"stopToolStripMenuItem";
			this->stopToolStripMenuItem->Size = System::Drawing::Size(98, 22);
			this->stopToolStripMenuItem->Text = L"Stop";
			this->stopToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWin::stopToolStripMenuItem_Click);
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
			// MainWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(490, 379);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainWin";
			this->Text = L"MainWin";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainWin::MainWin_KeyDown);
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
	/*private: Void addRobotImg()
	{
		PictureBox^ pb = gcnew PictureBox(); 
		pb->Size = Drawing::Size(24, 24); 
		pb->SizeMode = PictureBoxSizeMode::Zoom;
		pb->BackColor = Color::Transparent;
		pb->Image = originalRobotImg;

		//pb->Location = Point(50, 50); 
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
	}*/

private: System::Void dodajRobotaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	wSwarm->addRobot();
	renderPanel->Invalidate();
	//addRobotImg();
}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
	double dt = double(timer1->Interval) / 1000.0;
	time += dt;

	wSwarm->update(dt);

	renderPanel->Invalidate();/*
	List<Tuple<float, float>^>^ positions = wSwarm->getSwarmPositions();
	List<float>^ rotations = wSwarm->getSwarmRotations();

	for (int i = 0; i < positions->Count && i < robotsPB->Count; i++) 
	{
		float angle = rotations[i];

		// Usuñ poprzedni obrócony obraz (¿eby nie wyciekaæ pamiêci)
		if (robotsPB[i]->Image != nullptr && robotsPB[i]->Image != originalRobotImg)
			delete robotsPB[i]->Image;

		Bitmap^ rotated = RotateImage(originalRobotImg, angle);
		robotsPB[i]->Image = rotated;

		// Wyœrodkuj PictureBox wzglêdem pozycji robota
		int cx = (int)positions[i]->Item1 - rotated->Width / 2;
		int cy = (int)positions[i]->Item2 - rotated->Height / 2;
		robotsPB[i]->Size = Drawing::Size(rotated->Width, rotated->Height);
		robotsPB[i]->Location = Point(cx, cy);


		//robotsPB[i]->Location = Point((int)positions[i]->Item1, (int)positions[i]->Item2);

	}*/
}

private: Bitmap^ RotateImage(Bitmap^ original, float angleDeg)
{
	double rad = angleDeg * Math::PI / 180.0;
	double cosA = Math::Abs(Math::Cos(rad));
	double sinA = Math::Abs(Math::Sin(rad));

	int newW = (int)(original->Width * cosA + original->Height * sinA);
	int newH = (int)(original->Width * sinA + original->Height * cosA);

	Bitmap^ result = gcnew Bitmap(newW, newH, Imaging::PixelFormat::Format32bppArgb);
	Graphics^ g = Graphics::FromImage(result);

	g->InterpolationMode = Drawing2D::InterpolationMode::HighQualityBicubic;
	g->SmoothingMode = Drawing2D::SmoothingMode::AntiAlias;
	g->Clear(Color::Transparent);

	g->TranslateTransform(newW / 2.0f, newH / 2.0f);
	g->RotateTransform(angleDeg);
	g->TranslateTransform(-original->Width / 2.0f, -original->Height / 2.0f);
	g->DrawImage(original, 0, 0);

	delete g;
	return result;
}

private: System::Void renderPanel_Paint(System::Object^ sender, PaintEventArgs^ e)
{
	Graphics^ g = e->Graphics;
	g->InterpolationMode = Drawing2D::InterpolationMode::HighQualityBicubic;
	g->SmoothingMode = Drawing2D::SmoothingMode::AntiAlias;

	List<Tuple<float, float>^>^ positions = wSwarm->getSwarmPositions();
	List<float>^ rotations = wSwarm->getSwarmRotations();

	int hw = originalRobotImg->Width / 2;
	int hh = originalRobotImg->Height / 2;

	for (int i = 0; i < positions->Count; i++)
	{
		float px = positions[i]->Item1;
		float py = positions[i]->Item2;
		float angle = rotations[i];

		System::Drawing::Drawing2D::GraphicsState^ state = g->Save();
		g->TranslateTransform(px, py);
		g->RotateTransform(angle);
		g->DrawImage(originalRobotImg, -hw, -hh);  // rysuj wyœrodkowany
		g->Restore(state);
	}
}

private: System::Void MainWin_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
	if (e->KeyCode == Keys::A) {
		wSwarm->addRobot();
		renderPanel->Invalidate();
	}
	else if (e->KeyCode == Keys::X) {
		wSwarm->removeRobot();
		renderPanel->Invalidate();
	}
}

private: System::Void MainWin_Resize(System::Object^ sender, System::EventArgs^ e)
{
	int newW = this->ClientSize.Width;
	int newH = this->ClientSize.Height - menuStrip1->Height;

	// Aktualizuj rozmiar panelu
	renderPanel->Size = Drawing::Size(newW, newH);

	// Aktualizuj rozmiar mapy w Swarm
	wSwarm->setMapSize((float)newW, (float)newH);

	renderPanel->Invalidate();
}

private: System::Void renderPanel_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		wSwarm->addRobotAt((float)e->X, (float)e->Y);
	}
	else if (e->Button == System::Windows::Forms::MouseButtons::Right)
	{
		wSwarm->removeNearestRobot((float)e->X, (float)e->Y);
	}

	renderPanel->Invalidate();
}
};
}
