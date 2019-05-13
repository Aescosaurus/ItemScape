#pragma once

#include "InventoryItem.h"
#include "Utils.h"

class TripleBow
	:
	public InventoryItem
{
public:
	TripleBow()
		:
		InventoryItem( "ItemDescriptions/TripleBow.txt",
			"Images/Items/TripleBow.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new TripleBow );
	}

	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		++curShot;
		if( curShot >= shotsToBuff )
		{
			const auto diff = ( Vec2( invEvtInfo.mouse
				.GetPos() ) - invEvtInfo.player.GetPos() )
				.GetNormalized();
			const auto angle1 = diff.GetDeviated( devAmount );
			const auto angle2 = diff.GetDeviated( -devAmount );

			invEvtInfo.items[0]->Shoot( invEvtInfo,
				invEvtInfo.player.GetPos() + angle1 );
			invEvtInfo.items[0]->Shoot( invEvtInfo,
				invEvtInfo.player.GetPos() + angle2 );

			curShot = 0;
		}
	}
private:
	static constexpr float devAmount = chili::pi / 12.0f;
	static constexpr int shotsToBuff = 10;
	int curShot = 0;
};