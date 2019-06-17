#pragma once

#include "InventoryItem.h"

class WatermelonOre
	:
	public InventoryItem
{
public:
	WatermelonOre()
		:
		InventoryItem( "ItemDescriptions/WatermelonOre.txt",
			"Images/Items/WatermelonOre.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new WatermelonOre{ *this } );
	}

	void OnEnemyHit( InventoryEventInfo& evtInfo ) override
	{
		if( Random::RangeF( 0.0f,100.0f ) < seekerSpawnChance )
		{
			EnemyBase* targetEnemy = nullptr;
			float dist = 99999999.0f;

			for( auto& e : evtInfo.enemies )
			{
				const auto curDist = ( e->GetRect().GetCenter() -
					evtInfo.player.GetCenter() ).GetLengthSq();
				if( !e->IsExpl() && curDist < dist )
				{
					dist = curDist;
					targetEnemy = e.get();
				}
			}

			TrackingBullet* replacement = new TrackingBullet{
				*evtInfo.playerBullets.back() };

			replacement->SetSubColor( Colors::Yellow );
			// replacement->SetTarget( targetEnemy->GetPos() );
			replacement->SetSpeed( trackingBulletSpeed );
			if( targetEnemy != nullptr )
			{
				replacement->SetOffset( targetEnemy
					->GetRect().GetSize() / 2.0f );
			}

			evtInfo.playerBullets.pop_back();
			evtInfo.playerBullets.emplace_back( replacement );
		}
	}
private:
	static constexpr float seekerSpawnChance = 5.0f;
	static constexpr float trackingBulletSpeed = 200.0f;
};