#pragma once

#include "InventoryItem.h"

class DamagingStake
	:
	public InventoryItem
{
public:
	DamagingStake()
		:
		InventoryItem( "ItemDescriptions/DamagingStake.txt",
			"Images/Items/DamagingStake.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new DamagingStake{ *this } );
	}

	void OnActivate( InventoryEventInfo& invEvtInfo ) override
	{
		const auto& playerPos = invEvtInfo.player.GetPos();
		EnemyBase* closest = nullptr;
		float closestDist = 9999.0f;

		for( auto& e : invEvtInfo.enemies )
		{
			const auto curDist = ( e->GetPos() - playerPos )
				.GetLengthSq();
			if( curDist < closestDist )
			{
				closestDist = curDist;
				closest = e.get();
			}
		}

		if( closest != nullptr )
		{
			invEvtInfo.visualEffects.emplace_back(
				VisualEffect{ closest->GetRect().GetCenter(),
				VisualEffect::Type::Lightning } );

			closest->Attack( damage,playerPos );

			remove = true;
		}
	}
private:
	static constexpr int damage = 20;
};