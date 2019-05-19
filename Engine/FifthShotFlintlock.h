#pragma once

#include "InventoryItem.h"

class FifthShotFlintlock
	:
	public InventoryItem
{
public:
	FifthShotFlintlock()
		:
		InventoryItem( "ItemDescriptions/FifthShotFlintlock.txt",
			"Images/Items/FifthShotFlintlock.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new FifthShotFlintlock );
	}

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		++curShot;

		if( curShot >= shotsToBuff )
		{
			// evtInfo.playerBullets.back()->GetDamage() *= 2;
			// evtInfo.playerBullets.back()->SetSubColor( Colors::Red );

			evtInfo.items[0]->BoostDamage( evtInfo,1 );

			curShot = 0;
		}
		else
		{
			evtInfo.items[0]->RemoveDamageBoost( evtInfo,1 );
		}
	}
private:
	static constexpr int shotsToBuff = 5;
	int curShot = 0;
};