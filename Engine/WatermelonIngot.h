#pragma once

#include "InventoryItem.h"

class WatermelonIngot
	:
	public InventoryItem
{
public:
	WatermelonIngot()
		:
		InventoryItem( "ItemDescriptions/WatermelonIngot.txt",
			"Images/Items/WatermelonIngot.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new WatermelonIngot{ *this } );
	}

	void OnEnemyExplode( InventoryEventInfo& evtInfo ) override
	{
		const int nShots = Random::RangeI( minShots,maxShots );

		for( int i = 0; i < nShots; ++i )
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

			const auto vel = Vec2{ 0.0f,-1.0f }
				.Deviate( ( chili::pi * 2.0f ) *
				( float( i ) / float( nShots ) ) );

			evtInfo.items[0]->Shoot( evtInfo,
				Vec2( evtInfo.player.GetPos() + vel ) );

			TrackingBullet* replacement = new TrackingBullet{
				*evtInfo.playerBullets.back() };

			replacement->SetSubColor( Colors::Yellow );
			replacement->SetSpeed( trackingBulletSpeed );
			// replacement->SetTarget( targetEnemy->GetPos() );
			// replacement->SetOffset( targetEnemy->GetRect()
			// 	.GetSize() / 2.0f );

			evtInfo.playerBullets.pop_back();
			evtInfo.playerBullets.emplace_back( replacement );
		}
	}
private:
	static constexpr float trackingBulletSpeed = 200.0f;
	static constexpr int minShots = 3;
	static constexpr int maxShots = 5;
};