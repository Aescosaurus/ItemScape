#pragma once

#include "InventoryItem.h"

class BugSpray
	:
	public InventoryItem
{
public:
	BugSpray()
		:
		InventoryItem( "ItemDescriptions/BugSpray.txt",
			"Images/Items/BugSpray.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new BugSpray{ *this } );
	}

	void OnEnemyHit( InventoryEventInfo& evtInfo ) override
	{
		if( evtInfo.hitEnemy->GetHPPercent() < hpThreshhold &&
			Random::RangeF( 0.0f,100.0f ) < damageChance )
		{
			evtInfo.visualEffects.emplace_back( VisualEffect{
				evtInfo.hitEnemy->GetRect().GetCenter(),
				VisualEffect::Type::Lightning } );

			evtInfo.hitEnemy->Attack( damage,
				evtInfo.player.GetPos() );
		}
	}
private:
	static constexpr float hpThreshhold = 0.5f;
	static constexpr float damageChance = 5.0f;
	static constexpr int damage = 10;
};