#pragma once

#include "Mouse.h"
#include "Graphics.h"
#include "Button.h"
#include "MainWindow.h"
#include "FloorLevel.h"
#include "Inventory.h"
#include "GameState.h"

class MainMenu
{
private:
	enum class MenuState
	{
		TitleScreen,
		SaveSlots
	};
public:
	MainMenu( MainWindow& wnd,FloorLevel& floor,
		Inventory& inv,GameState& state,int& slot );

	bool Update( InventoryEventInfo& evtInfo,Mouse& mouse );
	void Draw( Graphics& gfx ) const;
private:
	MainWindow& wnd;
	FloorLevel& floor;
	Inventory& inv;
	GameState& gameState;
	int& saveSlot;

	MenuState state = MenuState::TitleScreen;

	Button play = Button{ Vei2{ 150,300 },"Play" };
	Button quit = Button{ Vei2{ 150,400 },"Quit" };

	bool save1Exists = false;
	bool save2Exists = false;
	bool save3Exists = false;

	Surface* save1Img = nullptr;
	Surface* save2Img = nullptr;
	Surface* save3Img = nullptr;

	// Button create1 = 

	Button save1 = Button{ Vei2{ 150,300 },"Load Game" };
	Button save2 = Button{ Vei2{ 150,400 },"Load Game" };
	Button save3 = Button{ Vei2{ 150,500 },"Load Game" };

	Button create1 = Button{ Vei2{ 150,300 },"New Game" };
	Button create2 = Button{ Vei2{ 150,400 },"New Game" };
	Button create3 = Button{ Vei2{ 150,500 },"New Game" };

	Button delete1 = Button{ Vei2{ 100,300 },"X" };
	Button delete2 = Button{ Vei2{ 100,400 },"X" };
	Button delete3 = Button{ Vei2{ 100,500 },"X" };
};