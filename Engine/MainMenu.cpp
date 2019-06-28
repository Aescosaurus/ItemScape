#include "MainMenu.h"
#include <fstream>
#include "SaveLoader.h"
#include "PickupManager.h"
#include <cstdio>

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

	if( save1Exists ) save1Img = new Surface{ "Saves/Save1Img.bmp" };
	if( save2Exists ) save2Img = new Surface{ "Saves/Save2Img.bmp" };
	if( save3Exists ) save3Img = new Surface{ "Saves/Save3Img.bmp" };
}

bool MainMenu::Update( InventoryEventInfo& evtInfo,Mouse& mouse )
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

		delete1.Update( mouse );
		delete2.Update( mouse );
		delete3.Update( mouse );

		if( save1.IsDown() )
		{
			SaveLoader::Load( SaveLoaderInfo{ floor,inv,
				evtInfo.gfx,evtInfo.map },1 );
			saveSlot = 1;
		}
		if( save2.IsDown() )
		{
			SaveLoader::Load( SaveLoaderInfo{ floor,inv,
				evtInfo.gfx,evtInfo.map },2 );
			saveSlot = 2;
		}
		if( save3.IsDown() )
		{
			SaveLoader::Load( SaveLoaderInfo{ floor,inv,
				evtInfo.gfx,evtInfo.map },3 );
			saveSlot = 3;
		}

		if( create1.IsDown() ) saveSlot = 1;
		if( create2.IsDown() ) saveSlot = 2;
		if( create3.IsDown() ) saveSlot = 3;

		if( save1.IsDown() || save2.IsDown() ||
			save3.IsDown() )
		{
			return( true );
		}

		if( create1.IsDown() || create2.IsDown() ||
			create3.IsDown() )
		{
			inv.AddItem( new RustyPistol,evtInfo );
			inv.AddItem( new HealthCharge,evtInfo );
			inv.AddItem( new HealthCharge,evtInfo );
			inv.AddItem( new HealthCharge,evtInfo );
			inv.AddItem( new MithrilHoe,evtInfo );

			return( true );
		}
		break;
	}

	if( delete1.IsDown() )
	{
		save1Exists = false;
		delete save1Img;
		save1Img = nullptr;
		std::remove( "Saves/Save1.txt" );
		std::remove( "Saves/Save1Img.bmp" );
	}
	if( delete2.IsDown() )
	{
		save2Exists = false;
		delete save1Img;
		save1Img = nullptr;
		std::remove( "Saves/Save2.txt" );
		std::remove( "Saves/Save2Img.bmp" );
	}
	if( delete3.IsDown() )
	{
		save3Exists = false;
		delete save1Img;
		save1Img = nullptr;
		std::remove( "Saves/Save3.txt" );
		std::remove( "Saves/Save3Img.bmp" );
	}

	return( false );
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
		if( save1.IsHovering() )
		{
			assert( save1Img != nullptr );
			gfx.DrawSprite( 0,0,*save1Img,SpriteEffect::Copy{} );
		}
		if( save2.IsHovering() )
		{
			assert( save2Img != nullptr );
			gfx.DrawSprite( 0,0,*save2Img,SpriteEffect::Copy{} );
		}
		if( save3.IsHovering() )
		{
			assert( save3Img != nullptr );
			gfx.DrawSprite( 0,0,*save3Img,SpriteEffect::Copy{} );
		}

		if( save1Exists )
		{
			save1.Draw( gfx );
			delete1.Draw( gfx );
		}
		else create1.Draw( gfx );
		if( save2Exists )
		{
			save2.Draw( gfx );
			delete2.Draw( gfx );
		}
		else create2.Draw( gfx );
		if( save3Exists )
		{
			save3.Draw( gfx );
			delete3.Draw( gfx );
		}
		else create3.Draw( gfx );
		break;
	}
}
