#pragma once

#include "InventoryItem.h"

class BulletRingCapacitor
	:
	public InventoryItem
{
public:
	BulletRingCapacitor()
		:
		InventoryItem( "ItemDescriptions/BulletRingCapacitor.txt",
			"Images/Items/BulletRingCapacitor.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new BulletRingCapacitor );
	}

	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		++curShot;
		if( curShot >= shotsToBuff )
		{
			const auto& player = invEvtInfo.player;
			for( int i = 0; i < nShots; ++i )
			{
				const auto vel = Vec2{ 0.0f,-1.0f }
					.Deviate( ( chili::pi * 2.0f ) *
					( float( i ) / float( nShots ) ) );

				invEvtInfo.items[0]->Shoot( invEvtInfo,
					Vec2( invEvtInfo.player.GetPos() + vel ) );
			}

			curShot = 0;
		}
	}
private:
	static constexpr int nShots = 8;
	static constexpr float bulletSpeed = 400.0f;
	static constexpr int damage = 1;
	static constexpr int shotsToBuff = 20;
	int curShot = 0;
};