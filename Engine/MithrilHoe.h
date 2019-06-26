#pragma once

#include "InventoryItem.h"

class MithrilHoe
	:
	public InventoryItem
{
public:
	MithrilHoe()
		:
		InventoryItem( "ItemDescriptions/MithrilHoe.txt",
			"Images/Items/MithrilHoe.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new MithrilHoe{ *this } );
	}

	void OnEnemyHit( InventoryEventInfo& evtInfo ) override
	{
		if( Random::RangeI( 0,100 ) < rootChance )
		{
			evtInfo.hitEnemy->ApplyEffect(
				EnemyBase::Effect::Rooted,rootTime );
		}
	}
private:
	static constexpr int rootChance = 15;
	static constexpr float rootTime = 5.0f;
};