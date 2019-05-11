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

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		++curShot;

		if( curShot >= shotsToBuff )
		{
			evtInfo.playerBullets.back()->GetDamage() *= 2;
			evtInfo.playerBullets.back()->SetSubColor( Colors::Red );

			curShot = 0;
		}
	}
private:
	static constexpr int shotsToBuff = 5;
	int curShot = 0;
};