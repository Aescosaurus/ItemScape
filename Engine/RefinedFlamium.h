#pragma once

#include "InventoryItem.h"

class RefinedFlamium
	:
	public InventoryItem
{
public:
	RefinedFlamium()
		:
		InventoryItem( "ItemDescriptions/RefinedFlamium.txt",
			"Images/Items/RefinedFlamium.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new RefinedFlamium{ *this } );
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		damageTimer.Update( evtInfo.dt );

		if( damageTimer.IsDone() )
		{
			damageTimer.Reset();

			auto& enemy = evtInfo.enemies[Random::RangeI( 0,
				int( evtInfo.enemies.size() ) - 1 )];

			enemy->Attack( damage,evtInfo.player.GetPos() );

			evtInfo.visualEffects.emplace_back( VisualEffect{
				Vei2( enemy->GetRect().GetCenter() ),
				VisualEffect::Type::Fire } );
		}
	}
private:
	Timer damageTimer = 5.0f;
	static constexpr int damage = 5;
};