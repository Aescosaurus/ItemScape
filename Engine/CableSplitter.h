#pragma once

#include "InventoryItem.h"
#include "Random.h"

class CableSplitter
	:
	public InventoryItem
{
public:
	CableSplitter()
		:
		InventoryItem( "ItemDescriptions/CableSplitter.txt",
			"Images/Items/CableSplitter.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new CableSplitter );
	}
	
	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		if( Random::RangeF( 0.0f,100.0f ) <= chance )
		{
			auto& plBulls = invEvtInfo.playerBullets;
			plBulls.emplace_back( std
				::make_unique<Bullet>( *plBulls.back() ) );

			const int last = int( plBulls.size() ) - 1;
			Vec2 vel1 = plBulls[last]->GetVel();
			Vec2 vel2 = plBulls[last - 1]->GetVel();
			vel1 = Vec2{ vel1.y,-vel1.x };
			vel2 = Vec2{ vel2.y,-vel2.x };

			Vec2 pos1 = plBulls[last]->GetPos() + vel1 / 25.0f;
			Vec2 pos2 = plBulls[last - 1]->GetPos() - vel2 / 25.0f;

			plBulls[last]->GetPos() = pos1;
			plBulls[last - 1]->GetPos() = pos2;
		}
	}
private:
	static constexpr float chance = 20.0f;
	static constexpr float deviationAngle = 0.05f;
};