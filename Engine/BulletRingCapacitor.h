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
				invEvtInfo.playerBullets.emplace_back(
					std::make_unique<Bullet>( player.GetPos(),
						Vec2( invEvtInfo.mouse.GetPos() ),
						invEvtInfo.map,Bullet::Team::Player1,
						bulletSpeed,Bullet::Size::Small,damage ) );

				auto& vel = invEvtInfo.playerBullets.back()->GetVel();
				vel = vel.Deviate( ( chili::pi * 2.0f ) *
					( float( i ) / float( nShots ) ) ) *
					vel.GetLength();
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