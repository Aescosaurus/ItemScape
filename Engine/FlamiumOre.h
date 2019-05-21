#pragma once

#include "InventoryItem.h"

class FlamiumOre
	:
	public InventoryItem
{
public:
	FlamiumOre()
		:
		InventoryItem( "ItemDescriptions/FlamiumOre.txt",
			"Images/Items/FlamiumOre.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new FlamiumOre );
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		damageTimer.Update( evtInfo.dt );

		if( damageTimer.IsDone() )
		{
			damageTimer.Reset();

			for( auto& enemy : evtInfo.enemies )
			{
				enemy->Attack( damage,evtInfo.player.GetPos() );

				evtInfo.visualEffects.emplace_back( VisualEffect{
					Vei2( enemy->GetRect().GetCenter() ),
					VisualEffect::Type::Fire } );
			}
		}
	}
private:
	Timer damageTimer = 10.0f;
	static constexpr int damage = 1;
};