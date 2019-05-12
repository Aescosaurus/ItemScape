#pragma once

#include "InventoryItem.h"
#include "Timer.h"

class RustyPistol
	:
	public InventoryItem
{
public:
	RustyPistol()
		:
		InventoryItem( "ItemDescriptions/Guns/RustyPistol.txt",
			"Images/Wall2.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new RustyPistol );
	}

	void OnUpdate( InventoryEventInfo& invEvtInfo ) override
	{
		shotTimer.Update( invEvtInfo.dt );

		invEvtInfo.player.SetJustShot( false );
	}

	void OnActivate( InventoryEventInfo& invEvtInfo ) override
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
				bulletSpeed,Bullet::Size::Small ) );
		}
	}
private:
	Timer shotTimer = 0.23f;
	static constexpr float bulletSpeed = 324.2f;
};