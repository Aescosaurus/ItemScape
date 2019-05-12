#pragma once

#include "InventoryItem.h"

class OldRifle
	:
	public InventoryItem
{
public:
	OldRifle()
		:
		InventoryItem( "ItemDescriptions/Guns/OldRifle.txt",
			"Images/Items/Guns/OldRifle.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new OldRifle );
	}

	void OnUpdate( InventoryEventInfo& invEvtInfo ) override
	{
		shotTimer.Update( invEvtInfo.dt );

		invEvtInfo.player.SetJustShot( false );
	}

	void OnGunFire( InventoryEventInfo& invEvtInfo ) override
	{
		if( shotTimer.IsDone() )
		{
			auto& player = invEvtInfo.player;
			shotTimer.Reset();

			player.SetJustShot( true );

			invEvtInfo.playerBullets.emplace_back(
				std::make_unique<Bullet>( player.GetPos(),
					Vec2( invEvtInfo.mouse.GetPos() ),
					invEvtInfo.map,Bullet::Team::Player1,
					bulletSpeed,Bullet::Size::Small,damage ) );
		}
	}
private:
	Timer shotTimer = 0.23f * 2.0f;
	static constexpr float bulletSpeed = 450.0f;
	static constexpr int damage = 2;
};