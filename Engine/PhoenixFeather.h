#pragma once

#include "InventoryItem.h"
#include "HealthCharge.h"

class PhoenixFeather
	:
	public InventoryItem
{
public:
	PhoenixFeather()
		:
		InventoryItem( "ItemDescriptions/PhoenixFeather.txt",
			"Images/Items/PhoenixFeather.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new PhoenixFeather{ *this } );
	}

	void OnPlayerHit( InventoryEventInfo& evtInfo ) override
	{
		if( Random::RangeI( 0,100 ) < hpChargeGainChance )
		{
			const auto& pickupPos = evtInfo.player.GetPos();
			evtInfo.pickups.emplace_back( new HealthCharge );
			evtInfo.visualEffects.emplace_back( VisualEffect{
				pickupPos,VisualEffect::Type::Fire } );
			evtInfo.pickups.back()->SetPos( pickupPos );
		}
	}
private:
	static constexpr int hpChargeGainChance = 35;
};