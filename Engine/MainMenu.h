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

	// Button create1 = 

	Button save1 = Button{ Vei2{ 150,300 },"Load 1" };
	Button save2 = Button{ Vei2{ 150,400 },"Load 2" };
	Button save3 = Button{ Vei2{ 150,500 },"Load 3" };

	Button create1 = Button{ Vei2{ 150,300 },"Create 1" };
	Button create2 = Button{ Vei2{ 150,400 },"Create 2" };
	Button create3 = Button{ Vei2{ 150,500 },"Create 3" };
};