#pragma once

#include "InventoryItem.h"

class PoisonBottle
	:
	public InventoryItem
{
public:
	PoisonBottle()
		:
		InventoryItem( "ItemDescriptions/PoisonBottle.txt",
			"Images/Items/PoisonBottle.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new PoisonBottle{ *this } );
	}

	void OnEnemyExplode( InventoryEventInfo& evtInfo ) override
	{
		const auto damage = evtInfo.hitEnemy->GetMaxHP() / 4;

		for( auto& e : evtInfo.enemies )
		{
			if( !e->IsExpl() )
			{
				evtInfo.visualEffects.emplace_back( VisualEffect{
					e->GetRect().GetCenter(),
					VisualEffect::Type::Poison } );

				e->Attack( damage,evtInfo.player.GetPos() );

				break;
			}
		}
	}
};