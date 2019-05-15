#pragma once

#include "InventoryItem.h"

class GPSTracker
	:
	public InventoryItem
{
public:
	GPSTracker()
		:
		InventoryItem( "ItemDescriptions/GPSTracker.txt",
			"Images/Items/GPSTracker.bmp" )
	{}
	
	InventoryItem* Clone() override
	{
		return( new GPSTracker );
	}

	void OnActivate( InventoryEventInfo& invEvtInfo ) override
	{
		if( !activated )
		{
			activated = true;

			invEvtInfo.visualEffects.emplace_back(
				VisualEffect{ invEvtInfo.player.GetPos(),
				VisualEffect::Type::LightningDissipate } );
		}
	}

	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		if( activated )
		{
			const auto& playerPos = invEvtInfo.player.GetPos();

			

			if( invEvtInfo.enemies.size() > 0 )
			{
				auto* targetEnemy = invEvtInfo.enemies[Random::RangeI(
					0,int( invEvtInfo.enemies.size() ) - 1 )].get();

				TrackingBullet* replacement = new TrackingBullet{
					*invEvtInfo.playerBullets.back() };

				replacement->SetSubColor( Colors::Yellow );
				replacement->SetTarget( targetEnemy->GetPos() );
				replacement->SetSpeed( trackingBulletSpeed );
				replacement->SetOffset( targetEnemy->GetRect()
					.GetSize() / 2.0f );

				invEvtInfo.playerBullets.pop_back();
				invEvtInfo.playerBullets.emplace_back( replacement );

				++soughtBullets;

				if( soughtBullets >= seekBulletCount )
				{
					remove = true;
				}
			}
		}
	}
private:
	static constexpr int seekBulletCount = 30;
	int soughtBullets = 0;
	bool activated = false;
	static constexpr float trackingBulletSpeed = 200.0f;
};