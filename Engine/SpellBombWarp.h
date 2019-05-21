#pragma once

#include "InventoryItem.h"

class SpellBombWarp
	:
	public InventoryItem
{
public:
	SpellBombWarp()
		:
		InventoryItem( "ItemDescriptions/SpellBombWarp.txt",
			"Images/Items/SpellBombWarp.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new SpellBombWarp );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		evtInfo.player.GetPos() = Vec2( evtInfo.mouse.GetPos() );

		for( int i = 0; i < nShots; ++i )
		{
			const auto vel = Vec2{ 0.0f,-1.0f }
				.Deviate( ( chili::pi * 2.0f ) *
				( float( i ) / float( nShots ) ) );

			evtInfo.items[0]->Shoot( evtInfo,
				Vec2( evtInfo.player.GetPos() + vel ) );

		}

		remove = true;
	}
private:
	static constexpr int nShots = 8;
};