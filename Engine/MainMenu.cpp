#include "MainMenu.h"
#include <fstream>
#include "SaveLoader.h"

MainMenu::MainMenu( MainWindow& wnd,FloorLevel& floor,
	Inventory& inv,GameState& state,int& slot )
	:
	wnd( wnd ),
	floor( floor ),
	inv( inv ),
	gameState( state ),
	saveSlot( slot )
{
	save1Exists = std::ifstream{ "Saves/Save1.txt" }.good();
	save2Exists = std::ifstream{ "Saves/Save2.txt" }.good();
	save3Exists = std::ifstream{ "Saves/Save3.txt" }.good();
}

void MainMenu::Update( Mouse& mouse )
{
	switch( state )
	{
	case MenuState::TitleScreen:
		play.Update( mouse );
		quit.Update( mouse );

		if( play.IsDown() ) state = MenuState::SaveSlots;
		if( quit.IsDown() ) wnd.Kill();
		break;
	case MenuState::SaveSlots:
		if( save1Exists ) save1.Update( mouse );
		else create1.Update( mouse );
		if( save2Exists ) save2.Update( mouse );
		else create2.Update( mouse );
		if( save3Exists ) save3.Update( mouse );
		else create3.Update( mouse );

		if( save1.IsDown() ) SaveLoader::Load( SaveLoaderInfo{ floor,inv },1 );
		if( save2.IsDown() ) SaveLoader::Load( SaveLoaderInfo{ floor,inv },2 );
		if( save3.IsDown() ) SaveLoader::Load( SaveLoaderInfo{ floor,inv },3 );

		if( create1.IsDown() ) saveSlot = 1;
		if( create2.IsDown() ) saveSlot = 2;
		if( create3.IsDown() ) saveSlot = 3;
		
		if( save1.IsDown() || save2.IsDown() ||
			save3.IsDown() )
		{
			gameState = GameState::Gameplay;
		}

		if( create1.IsDown() || create2.IsDown() ||
			create3.IsDown() )
		{
			gameState = GameState::Gameplay;
		}
		break;
	}
}

void MainMenu::Draw( Graphics& gfx ) const
{
	switch( state )
	{
	case MenuState::TitleScreen:
		play.Draw( gfx );
		quit.Draw( gfx );
		break;
	case MenuState::SaveSlots:
		if( save1Exists ) save1.Draw( gfx );
		else create1.Draw( gfx );
		if( save2Exists ) save2.Draw( gfx );
		else create2.Draw( gfx );
		if( save3Exists ) save3.Draw( gfx );
		else create3.Draw( gfx );
		break;
	}
}
