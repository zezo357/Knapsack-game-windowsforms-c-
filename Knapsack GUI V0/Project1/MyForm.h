#pragma once
#include "About.h"
#include "scores.h"
knapsack Game(10);
vector<Level> lev;
string Items_file = "resources/Items.txt";
string Levels_file = "resources/Levels.txt";
namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		About^ AboutForm = gcnew About(this);
		//these containers was made to avoid loading the items pictures every level (to avoid memory leak and make a better momery mangment)
		List<PictureBox^> pictureboxes;
		List<Button^> buttons;
		List<Image^> imagelist;
		//index for level
		int level = 1;
		//base score
		int scoren = 1000;
		//score multiplier
		int M = 10;
		//wrong clicks in the level
		int clicks = 0;
		//bool for the bot to solve all levels
		bool solver = 0;
		
		//check if he won or not and check if there is more levels
		//update price and score text 
		void check_game_state() {
			//updating text(weight,score)
			label4->Text = Game.RemainingWeight().ToString();
			label6->Text = scoren.ToString();
			//check if the weight below the max (true)
			if (Game.PriceOfItemsInBag() != -1) {
				//updating text(price in bag)
				score->Text = Game.PriceOfItemsInBag().ToString();
				//if the price is equal to the max price (won)
				if (Game.PriceOfItemsInBag() == Game.knapSackMaxValue()) {
					MessageBox::Show("You Won :) Good Job");
					//if there is a new defined level
					if (level < lev.size()) {
						lev[level - 1].score = scoren;
						level++;
						Game.clear();
						Game.SetKnapsackMaxWeight(lev[level - 1].KnapSackMax);
						textBox1->Text = Game.GetKnapsackMaxWeight().ToString();
						button1->Text = "Tip Can be Used ONCE";
						scoren = 1000;
						clicks = 0;
						updateform();
					}
					//if the user finished all the levels
					else {
						lev[level - 1].score = scoren;
						scoren = 1000;
						clicks = 0;
						MessageBox::Show("You Finsihed all the levels yaaaay");
						All_Enabled_Value(false);
						scores^ ScoreForm = gcnew scores(lev);
						ScoreForm->ShowDialog();
						textBox1->Enabled = true;
						button1->Text = "Tip Can be Used ONCE";
						button2->Enabled = true;
					}
				}
			}
			//check if the weight below the max (false)
			else { score->Text = "too much weight"; }
		}
		//will enable or disable all buttons and picturboxes
		void All_Enabled_Value(bool tmp) {
			for (int i=0; i < buttons.Count; i++) {
				buttons[i]->Enabled=tmp;
				pictureboxes[i]->Enabled = tmp;
			}
		}
		//if the button or picture is clicked the item will get inserted in the bag 
		//if the item is already in bag it will return it back
		void click(int index) {
			//check if the item in bag
			//item is not in bag (will insert the item)
			if (buttons[index]->Text != "return it back") {
				Game.InsertItemToBag(Game.GetAllItems()[index]);
				//updating (picture box to be empty,button to return it back)
				pictureboxes[index]->Image = nullptr;
				buttons[index]->Text = "return it back";
			}
			//item is in bag (will remove the item)
			else {
				Game.RemoveItemFromBag(Game.GetAllItems()[index]);
				//will bring back the weight and prive and picture of the item
				pictureboxes[index]->Image = imagelist[Game.Getrelative_position()[index]];
				buttons[index]->Text = "weight=" + (Game.GetAllItems()[index].weight).ToString() + " price=" + (Game.GetAllItems()[index].price).ToString();
				//increase the click counter at a wrong move has occured
				clicks++;
				scoren = ((scoren)-((clicks * M)));
			}
			check_game_state();
		}

		//works on the first run only 
		//reads files to get the levels and items
		//and initialize buttons and pictureboxes and images lists
		void initialize_list() {

			fstream FileLevel{ Levels_file, ios::in };
			// exit program if fstream cannot open file
			if (!FileLevel) {
				MessageBox::Show(gcnew String(Levels_file.c_str())+" File could not be found.");
				exit(EXIT_FAILURE);
			}
			int numberofitems, maxweight;
			while (FileLevel >> numberofitems >> maxweight) {
				lev.push_back(Level(numberofitems, maxweight));
			}
			FileLevel.close();

			fstream File{ Items_file, ios::in };
			// exit program if fstream cannot open file
			if (!File) {
				MessageBox::Show(gcnew String(Items_file.c_str()) + " File could not be found.");
				exit(EXIT_FAILURE);
			}
			int price, weight;
			string name, pic;
			while (File >> price >> weight >> name >> pic) {
				imagelist.Add(Image::FromFile("resources/" + gcnew String(pic.c_str())));
			}
			File.close();

			pictureboxes.Add(pictureBox1);
			pictureboxes.Add(pictureBox2);
			pictureboxes.Add(pictureBox3);
			pictureboxes.Add(pictureBox4);
			pictureboxes.Add(pictureBox5);
			pictureboxes.Add(pictureBox6);
			pictureboxes.Add(pictureBox7);
			pictureboxes.Add(pictureBox8);
			pictureboxes.Add(pictureBox9);
			pictureboxes.Add(pictureBox10);
			pictureboxes.Add(pictureBox11);
			pictureboxes.Add(pictureBox12);
			buttons.Add(item1);
			buttons.Add(item2);
			buttons.Add(item3);
			buttons.Add(item4);
			buttons.Add(item5);
			buttons.Add(item6);
			buttons.Add(item7);
			buttons.Add(item8);
			buttons.Add(item9);
			buttons.Add(item10);
			buttons.Add(item11);
			buttons.Add(item12);
		}
		//update form depending on the level
		void updateform() {
			Game.InsertRandomItemsFromFile(Items_file, lev[level - 1].NumberOfitems);
			//Game.InsertItemsFromFile("File.txt");
			check_game_state();
			int i;
			//will show and update text for the items that the game have
			for (i = 0; i < Game.GetAllItems().size(); i++) {

				buttons[i]->Text = "weight=" + (Game.GetAllItems()[i].weight).ToString() + " price=" + (Game.GetAllItems()[i].price).ToString();
				buttons[i]->Show();
				pictureboxes[i]->Image = imagelist[Game.Getrelative_position()[i]];
				pictureboxes[i]->Show();
			}
			//hide the rest of buttons and picture boxes
			for (i; i < buttons.Count; i++) {
				buttons[i]->Hide();
				pictureboxes[i]->Hide();
			}
			//if the bot solve all levels is on this will work
			if (solver) { Solve_This_Level(); }
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Bot(developers)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//select one item from the ones that will lead to a win
		void Select_Next_Winning_Item() {
			//loop on the winning items
			for (int i = 0; i < Game.GetWinningItems().size(); i++) {
				//check if the item is bag or not
				if ((Game.InBag(Game.GetWinningItems()[i])).name != "") {
					//if not in bag will insert it
					click(Game.Index_In_All(Game.GetWinningItems()[i]));
					//break to only insert one item
					break;
				}
			}
		}
		//select all items that will lead to a win in this level
		void Solve_This_Level() {
			
			int winning_items = Game.GetWinningItems().size();
			//loop on the winning items
			for (int i = 0; i < winning_items; i++) {
				//check if the item is bag or not
				if ((Game.InBag(Game.GetWinningItems()[i])).name != "") {
					//if not in bag will insert it
					click(Game.Index_In_All(Game.GetWinningItems()[i]));
				}
			}
		}
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}


	protected:
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Panel^ panel2;


	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::PictureBox^ pictureBox3;
	private: System::Windows::Forms::PictureBox^ pictureBox4;
	private: System::Windows::Forms::PictureBox^ pictureBox5;
	private: System::Windows::Forms::PictureBox^ pictureBox6;
	private: System::Windows::Forms::PictureBox^ pictureBox7;
	private: System::Windows::Forms::PictureBox^ pictureBox8;
	private: System::Windows::Forms::PictureBox^ pictureBox9;	
	private: System::Windows::Forms::PictureBox^ pictureBox10;
	private: System::Windows::Forms::PictureBox^ pictureBox11;
	private: System::Windows::Forms::PictureBox^ pictureBox12;
	private: System::Windows::Forms::PictureBox^ pictureBox13;
	
	
	private: System::Windows::Forms::Button^ item1;
	private: System::Windows::Forms::Button^ item2;
	private: System::Windows::Forms::Button^ item3;
	private: System::Windows::Forms::Button^ item4;
	private: System::Windows::Forms::Button^ item5;
	private: System::Windows::Forms::Button^ item6;
	private: System::Windows::Forms::Button^ item7;
	private: System::Windows::Forms::Button^ item8;
	private: System::Windows::Forms::Button^ item9;
	private: System::Windows::Forms::Button^ item10;
	private: System::Windows::Forms::Button^ item11;
	private: System::Windows::Forms::Button^ item12;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ Solve_All_Levels_Button;
	private: System::Windows::Forms::Button^ Solve_This_Level_Button;
	private: System::Windows::Forms::Button^ Select_Next_Winning_Item_Button;

	
	private: System::Windows::Forms::Label^ score;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;


	private: System::Windows::Forms::TextBox^ textBox1;


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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->Select_Next_Winning_Item_Button = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->Solve_All_Levels_Button = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->Solve_This_Level_Button = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->score = (gcnew System::Windows::Forms::Label());
			this->pictureBox13 = (gcnew System::Windows::Forms::PictureBox());
			this->item11 = (gcnew System::Windows::Forms::Button());
			this->item10 = (gcnew System::Windows::Forms::Button());
			this->item9 = (gcnew System::Windows::Forms::Button());
			this->item12 = (gcnew System::Windows::Forms::Button());
			this->item5 = (gcnew System::Windows::Forms::Button());
			this->item6 = (gcnew System::Windows::Forms::Button());
			this->item8 = (gcnew System::Windows::Forms::Button());
			this->item7 = (gcnew System::Windows::Forms::Button());
			this->item4 = (gcnew System::Windows::Forms::Button());
			this->item3 = (gcnew System::Windows::Forms::Button());
			this->item2 = (gcnew System::Windows::Forms::Button());
			this->pictureBox11 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox12 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox10 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox9 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->item1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox13))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox11))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox12))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->Select_Next_Winning_Item_Button);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->Solve_All_Levels_Button);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Controls->Add(this->Solve_This_Level_Button);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->label1);
			resources->ApplyResources(this->panel1, L"panel1");
			this->panel1->Name = L"panel1";
			// 
			// Select_Next_Winning_Item_Button
			// 
			resources->ApplyResources(this->Select_Next_Winning_Item_Button, L"Select_Next_Winning_Item_Button");
			this->Select_Next_Winning_Item_Button->Name = L"Select_Next_Winning_Item_Button";
			this->Select_Next_Winning_Item_Button->UseVisualStyleBackColor = true;
			this->Select_Next_Winning_Item_Button->Click += gcnew System::EventHandler(this, &MyForm::Select_Next_Winning_Item_Button_Click);
			// 
			// label7
			// 
			resources->ApplyResources(this->label7, L"label7");
			this->label7->Name = L"label7";
			// 
			// Solve_All_Levels_Button
			// 
			resources->ApplyResources(this->Solve_All_Levels_Button, L"Solve_All_Levels_Button");
			this->Solve_All_Levels_Button->Name = L"Solve_All_Levels_Button";
			this->Solve_All_Levels_Button->UseVisualStyleBackColor = true;
			this->Solve_All_Levels_Button->Click += gcnew System::EventHandler(this, &MyForm::Solve_All_Levels_Button_Click);
			// 
			// label6
			// 
			resources->ApplyResources(this->label6, L"label6");
			this->label6->Name = L"label6";
			// 
			// label5
			// 
			resources->ApplyResources(this->label5, L"label5");
			this->label5->Name = L"label5";
			this->label5->Click += gcnew System::EventHandler(this, &MyForm::label5_Click);
			// 
			// button4
			// 
			resources->ApplyResources(this->button4, L"button4");
			this->button4->Name = L"button4";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// Solve_This_Level_Button
			// 
			resources->ApplyResources(this->Solve_This_Level_Button, L"Solve_This_Level_Button");
			this->Solve_This_Level_Button->Name = L"Solve_This_Level_Button";
			this->Solve_This_Level_Button->UseVisualStyleBackColor = true;
			this->Solve_This_Level_Button->Click += gcnew System::EventHandler(this, &MyForm::Solve_This_Level_Button_Click);
			// 
			// button3
			// 
			resources->ApplyResources(this->button3, L"button3");
			this->button3->Name = L"button3";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// label4
			// 
			resources->ApplyResources(this->label4, L"label4");
			this->label4->Name = L"label4";
			// 
			// label3
			// 
			resources->ApplyResources(this->label3, L"label3");
			this->label3->Name = L"label3";
			// 
			// button2
			// 
			resources->ApplyResources(this->button2, L"button2");
			this->button2->Name = L"button2";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// label2
			// 
			resources->ApplyResources(this->label2, L"label2");
			this->label2->Name = L"label2";
			// 
			// textBox1
			// 
			resources->ApplyResources(this->textBox1, L"textBox1");
			this->textBox1->Name = L"textBox1";
			// 
			// button1
			// 
			resources->ApplyResources(this->button1, L"button1");
			this->button1->Name = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			resources->ApplyResources(this->label1, L"label1");
			this->label1->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->label1->Name = L"label1";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->score);
			this->panel2->Controls->Add(this->pictureBox13);
			this->panel2->Controls->Add(this->item11);
			this->panel2->Controls->Add(this->item10);
			this->panel2->Controls->Add(this->item9);
			this->panel2->Controls->Add(this->item12);
			this->panel2->Controls->Add(this->item5);
			this->panel2->Controls->Add(this->item6);
			this->panel2->Controls->Add(this->item8);
			this->panel2->Controls->Add(this->item7);
			this->panel2->Controls->Add(this->item4);
			this->panel2->Controls->Add(this->item3);
			this->panel2->Controls->Add(this->item2);
			this->panel2->Controls->Add(this->pictureBox11);
			this->panel2->Controls->Add(this->pictureBox12);
			this->panel2->Controls->Add(this->pictureBox10);
			this->panel2->Controls->Add(this->pictureBox5);
			this->panel2->Controls->Add(this->pictureBox7);
			this->panel2->Controls->Add(this->pictureBox8);
			this->panel2->Controls->Add(this->pictureBox9);
			this->panel2->Controls->Add(this->pictureBox6);
			this->panel2->Controls->Add(this->pictureBox4);
			this->panel2->Controls->Add(this->pictureBox3);
			this->panel2->Controls->Add(this->pictureBox2);
			this->panel2->Controls->Add(this->item1);
			this->panel2->Controls->Add(this->pictureBox1);
			resources->ApplyResources(this->panel2, L"panel2");
			this->panel2->Name = L"panel2";
			// 
			// score
			// 
			this->score->BackColor = System::Drawing::Color::White;
			resources->ApplyResources(this->score, L"score");
			this->score->Name = L"score";
			// 
			// pictureBox13
			// 
			this->pictureBox13->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox13, L"pictureBox13");
			this->pictureBox13->Name = L"pictureBox13";
			this->pictureBox13->TabStop = false;
			// 
			// item11
			// 
			this->item11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item11, L"item11");
			this->item11->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item11->Name = L"item11";
			this->item11->UseVisualStyleBackColor = false;
			this->item11->Click += gcnew System::EventHandler(this, &MyForm::item11_Click);
			// 
			// item10
			// 
			this->item10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item10, L"item10");
			this->item10->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item10->Name = L"item10";
			this->item10->UseVisualStyleBackColor = false;
			this->item10->Click += gcnew System::EventHandler(this, &MyForm::item10_Click);
			// 
			// item9
			// 
			this->item9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item9, L"item9");
			this->item9->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item9->Name = L"item9";
			this->item9->UseVisualStyleBackColor = false;
			this->item9->Click += gcnew System::EventHandler(this, &MyForm::item9_Click);
			// 
			// item12
			// 
			this->item12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item12, L"item12");
			this->item12->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item12->Name = L"item12";
			this->item12->UseVisualStyleBackColor = false;
			this->item12->Click += gcnew System::EventHandler(this, &MyForm::item12_Click);
			// 
			// item5
			// 
			this->item5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item5, L"item5");
			this->item5->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item5->Name = L"item5";
			this->item5->UseVisualStyleBackColor = false;
			this->item5->Click += gcnew System::EventHandler(this, &MyForm::item5_Click);
			// 
			// item6
			// 
			this->item6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item6, L"item6");
			this->item6->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item6->Name = L"item6";
			this->item6->UseVisualStyleBackColor = false;
			this->item6->Click += gcnew System::EventHandler(this, &MyForm::item6_Click);
			// 
			// item8
			// 
			this->item8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item8, L"item8");
			this->item8->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item8->Name = L"item8";
			this->item8->UseVisualStyleBackColor = false;
			this->item8->Click += gcnew System::EventHandler(this, &MyForm::item8_Click);
			// 
			// item7
			// 
			this->item7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item7, L"item7");
			this->item7->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item7->Name = L"item7";
			this->item7->UseVisualStyleBackColor = false;
			this->item7->Click += gcnew System::EventHandler(this, &MyForm::item7_Click);
			// 
			// item4
			// 
			this->item4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item4, L"item4");
			this->item4->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item4->Name = L"item4";
			this->item4->UseVisualStyleBackColor = false;
			this->item4->Click += gcnew System::EventHandler(this, &MyForm::item4_Click);
			// 
			// item3
			// 
			this->item3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item3, L"item3");
			this->item3->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item3->Name = L"item3";
			this->item3->UseVisualStyleBackColor = false;
			this->item3->Click += gcnew System::EventHandler(this, &MyForm::item3_Click);
			// 
			// item2
			// 
			this->item2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item2, L"item2");
			this->item2->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item2->Name = L"item2";
			this->item2->UseVisualStyleBackColor = false;
			this->item2->Click += gcnew System::EventHandler(this, &MyForm::item2_Click);
			// 
			// pictureBox11
			// 
			this->pictureBox11->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox11, L"pictureBox11");
			this->pictureBox11->Name = L"pictureBox11";
			this->pictureBox11->TabStop = false;
			this->pictureBox11->Click += gcnew System::EventHandler(this, &MyForm::pictureBox11_Click);
			// 
			// pictureBox12
			// 
			this->pictureBox12->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox12, L"pictureBox12");
			this->pictureBox12->Name = L"pictureBox12";
			this->pictureBox12->TabStop = false;
			this->pictureBox12->Click += gcnew System::EventHandler(this, &MyForm::pictureBox12_Click);
			// 
			// pictureBox10
			// 
			this->pictureBox10->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox10, L"pictureBox10");
			this->pictureBox10->Name = L"pictureBox10";
			this->pictureBox10->TabStop = false;
			this->pictureBox10->Click += gcnew System::EventHandler(this, &MyForm::pictureBox10_Click);
			// 
			// pictureBox5
			// 
			this->pictureBox5->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox5, L"pictureBox5");
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->TabStop = false;
			this->pictureBox5->Click += gcnew System::EventHandler(this, &MyForm::pictureBox5_Click);
			// 
			// pictureBox7
			// 
			this->pictureBox7->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox7, L"pictureBox7");
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->TabStop = false;
			this->pictureBox7->Click += gcnew System::EventHandler(this, &MyForm::pictureBox7_Click);
			// 
			// pictureBox8
			// 
			this->pictureBox8->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox8, L"pictureBox8");
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->TabStop = false;
			this->pictureBox8->Click += gcnew System::EventHandler(this, &MyForm::pictureBox8_Click);
			// 
			// pictureBox9
			// 
			this->pictureBox9->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox9, L"pictureBox9");
			this->pictureBox9->Name = L"pictureBox9";
			this->pictureBox9->TabStop = false;
			this->pictureBox9->Click += gcnew System::EventHandler(this, &MyForm::pictureBox9_Click);
			// 
			// pictureBox6
			// 
			this->pictureBox6->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox6, L"pictureBox6");
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->TabStop = false;
			this->pictureBox6->Click += gcnew System::EventHandler(this, &MyForm::pictureBox6_Click);
			// 
			// pictureBox4
			// 
			this->pictureBox4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox4, L"pictureBox4");
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->TabStop = false;
			this->pictureBox4->Click += gcnew System::EventHandler(this, &MyForm::pictureBox4_Click);
			// 
			// pictureBox3
			// 
			this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox3, L"pictureBox3");
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->TabStop = false;
			this->pictureBox3->Click += gcnew System::EventHandler(this, &MyForm::pictureBox3_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox2, L"pictureBox2");
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Click += gcnew System::EventHandler(this, &MyForm::pictureBox2_Click);
			// 
			// item1
			// 
			this->item1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(108)),
				static_cast<System::Int32>(static_cast<System::Byte>(186)));
			resources->ApplyResources(this->item1, L"item1");
			this->item1->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->item1->Name = L"item1";
			this->item1->UseVisualStyleBackColor = false;
			this->item1->Click += gcnew System::EventHandler(this, &MyForm::item1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			resources->ApplyResources(this->pictureBox1, L"pictureBox1");
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &MyForm::pictureBox1_Click);
			// 
			// MyForm
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox13))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox11))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox12))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox10))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {	
	initialize_list();
	updateform();
}

private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	check_game_state();
	if (button1->Text == "Tip Can be Used ONCE" ) {
		//loop on winning items
		for (int i = 0;i < Game.GetWinningItems().size();i++) {
			//if the item is not in bag 
			if ((Game.InBag(Game.GetWinningItems()[i])).name!="") {
				//will make the text the name of the item
				button1->Text = gcnew String(Game.InBag(Game.GetWinningItems()[i]).name.c_str());
			}
		}
	}
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		solver = 0;
		Game.clear();
		lev.push_back(Level(12, Convert::ToInt32(textBox1->Text)));
		Game.SetKnapsackMaxWeight(Convert::ToInt32(textBox1->Text));
		level++;
		button2->Enabled = false;
		All_Enabled_Value(true);
		updateform();
		check_game_state();
	}
	catch (...) {
	
		 textBox1->Text = "Please enter only number"; 
	}
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	AboutForm->ShowDialog();
}
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	MessageBox::Show("You have to get the highest price in the limited weight To win the game");
}
private: System::Void item1_Click(System::Object^ sender, System::EventArgs^ e) {
	click(0);
}
private: System::Void item2_Click(System::Object^ sender, System::EventArgs^ e) {
	click(1);
}
private: System::Void item3_Click(System::Object^ sender, System::EventArgs^ e) {
	click(2);
}
private: System::Void item4_Click(System::Object^ sender, System::EventArgs^ e) {
	click(3);
}
private: System::Void item5_Click(System::Object^ sender, System::EventArgs^ e) {
	click(4);
}
private: System::Void item6_Click(System::Object^ sender, System::EventArgs^ e) {
	click(5);
}
private: System::Void item7_Click(System::Object^ sender, System::EventArgs^ e) {
	click(6);
}
private: System::Void item8_Click(System::Object^ sender, System::EventArgs^ e) {
	click(7);
}
private: System::Void item9_Click(System::Object^ sender, System::EventArgs^ e) {
	click(8);
}
private: System::Void item10_Click(System::Object^ sender, System::EventArgs^ e) {
	click(9);
}
private: System::Void item11_Click(System::Object^ sender, System::EventArgs^ e) {
	click(10);
}
private: System::Void item12_Click(System::Object^ sender, System::EventArgs^ e) {
	click(11);
}

private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
	click(0);
}
private: System::Void pictureBox2_Click(System::Object^ sender, System::EventArgs^ e) {
	click(1);
}
private: System::Void pictureBox3_Click(System::Object^ sender, System::EventArgs^ e) {
	click(2);
}
private: System::Void pictureBox4_Click(System::Object^ sender, System::EventArgs^ e) {
	click(3);
}
private: System::Void pictureBox5_Click(System::Object^ sender, System::EventArgs^ e) {
	click(4);
}
private: System::Void pictureBox6_Click(System::Object^ sender, System::EventArgs^ e) {
	click(5);
}
private: System::Void pictureBox7_Click(System::Object^ sender, System::EventArgs^ e) {
	click(6);
}
private: System::Void pictureBox8_Click(System::Object^ sender, System::EventArgs^ e) {
	click(7);
}
private: System::Void pictureBox9_Click(System::Object^ sender, System::EventArgs^ e) {
	click(8);
}
private: System::Void pictureBox10_Click(System::Object^ sender, System::EventArgs^ e) {
	click(9);
}
private: System::Void pictureBox11_Click(System::Object^ sender, System::EventArgs^ e) {
	click(10);
}
private: System::Void pictureBox12_Click(System::Object^ sender, System::EventArgs^ e) {
	click(11);
}
private: System::Void Select_Next_Winning_Item_Button_Click(System::Object^ sender, System::EventArgs^ e) {
	Select_Next_Winning_Item();
}
private: System::Void Solve_This_Level_Button_Click(System::Object^ sender, System::EventArgs^ e) {
	Solve_This_Level();
}
private: System::Void Solve_All_Levels_Button_Click(System::Object^ sender, System::EventArgs^ e) {
	solver = 1;
	Solve_This_Level();
}
private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
	if (AboutForm->Developer) {
		label7->Visible = true;
		Solve_All_Levels_Button->Visible = true;
		Solve_All_Levels_Button->Enabled = true;
		Solve_This_Level_Button->Visible = true;
		Solve_This_Level_Button->Enabled = true;
		Select_Next_Winning_Item_Button->Visible = true;
		Select_Next_Winning_Item_Button->Enabled = true;
		label1->Visible = false;
	}
}
};
}
