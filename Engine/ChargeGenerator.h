#pragma once

#include "InventoryItem.h"
#include "HealthCharge.h"

class ChargeGenerator
	:
	public InventoryItem
{
public:
	ChargeGenerator()
		:
		InventoryItem( "ItemDescriptions/ChargeGenerator.txt",
			"Images/Items/ChargeGenerator.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new ChargeGenerator );
	}

	void OnEnemyExplode( InventoryEventInfo& invEvtInfo ) override
	{
		if( Random::RangeF( 0.0f,100.0f ) <= chance )
		{
			const auto itemSpawnPos = invEvtInfo.player
				.GetRect().GetCenter() + Vec2::Up() * 64.0f;
			invEvtInfo.pickups.emplace_back(
				new HealthCharge );

			invEvtInfo.pickups.back()->SetPos( itemSpawnPos );

			invEvtInfo.visualEffects.emplace_back( VisualEffect{
				itemSpawnPos,VisualEffect::Type::LightningDissipate } );
		}
	}
private:
	static constexpr float chance = 10.0f;
};