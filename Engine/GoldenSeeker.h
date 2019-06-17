#pragma once

#include "InventoryItem.h"

class GoldenSeeker
	:
	public InventoryItem
{
public:
	GoldenSeeker()
		:
		InventoryItem( "ItemDescriptions/GoldenSeeker.txt",
			"Images/Items/GoldenSeeker.bmp",2 )
	{}

	InventoryItem* Clone() override
	{
		return( new GoldenSeeker{ *this } );
	}

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		if( evtInfo.enemies.size() > 0 )
		{
			auto replacement = new TrackingBullet{
				*evtInfo.playerBullets.back() };
			replacement->SetSubColor( Colors::Yellow );
			replacement->SetSpeed( trackingBulletSpeed );

			evtInfo.playerBullets.pop_back();
			evtInfo.playerBullets.emplace_back( replacement );
		}
	}
private:
	static constexpr float trackingBulletSpeed = 250.0f;
};