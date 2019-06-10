#pragma once

#include "InventoryItem.h"

class TwistyTie
	:
	public InventoryItem
{
public:
	TwistyTie()
		:
		InventoryItem( "ItemDescriptions/TwistyTie.txt",
			"Images/Items/TwistyTie.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new TwistyTie{ *this } );
	}
	
	void OnPlayerShoot( InventoryEventInfo& invEvtInfo )
	{
		// Turn player's shots into twisty shots.
		const Bullet* lastBullet = invEvtInfo.playerBullets.back().get();

		invEvtInfo.playerBullets.emplace_back(
			std::make_unique<WavyBullet>( *lastBullet ) );
		invEvtInfo.playerBullets.erase( invEvtInfo
			.playerBullets.end() - 2 );
	}
};