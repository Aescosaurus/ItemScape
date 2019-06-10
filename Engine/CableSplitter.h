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
		return( new CableSplitter{ *this } );
	}
	
	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		if( Random::RangeF( 0.0f,100.0f ) <= chance )
		{
			const auto oldPlayerPos = invEvtInfo.player.GetPos();

			const auto diff = ( Vec2( invEvtInfo.mouse
				.GetPos() ) - invEvtInfo.player.GetPos() )
				.GetNormalized();

			invEvtInfo.player.GetPos() += Vec2{ diff.y,-diff.x } * 30.0f;

			invEvtInfo.items[0]->Shoot( invEvtInfo,
				Vec2( invEvtInfo.mouse.GetPos() ) );

			invEvtInfo.player.GetPos() = oldPlayerPos;
		}
	}
private:
	static constexpr float chance = 20.0f;
	static constexpr float deviationAngle = 0.05f;
};