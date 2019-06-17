#pragma once

#include "InventoryItem.h"
#include "Utils.h"

class WatermelonPop
	:
	public InventoryItem
{
public:
	WatermelonPop()
		:
		InventoryItem( "ItemDescriptions/WatermelonPop.txt",
			"Images/Items/WatermelonPop.bmp",2 )
	{}

	InventoryItem* Clone() override
	{
		return( new WatermelonPop{ *this } );
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		shotTimer.Update( evtInfo.dt );
		
		if( shotTimer.IsDone() )
		{
			shotTimer.Reset( Random::RangeF( minTime,maxTime ) );

			const int nShots = Random::RangeI( minShots,maxShots );
			for( int i = 0; i < nShots; ++i )
			{
				const auto vel = Vec2{ 0.0f,-1.0f }
					.Deviate( ( chili::pi * 2.0f ) *
					( float( i ) / float( nShots ) ) );

				evtInfo.items[0]->Shoot( evtInfo,
					Vec2( evtInfo.player.GetPos() + vel ) );

				auto replacement = new TrackingBullet{
					*evtInfo.playerBullets.back() };
				replacement->SetSubColor( Colors::Yellow );
				replacement->SetSpeed( trackingBulletSpeed );
				evtInfo.playerBullets.pop_back();
				evtInfo.playerBullets.emplace_back( replacement );
			}
		}
	}
private:
	Timer shotTimer = 3.0f;
	static constexpr float minTime = 3.0f;
	static constexpr float maxTime = 5.0f;
	static constexpr int minShots = 3;
	static constexpr int maxShots = 5;
	static constexpr float trackingBulletSpeed = 250.0f;
};