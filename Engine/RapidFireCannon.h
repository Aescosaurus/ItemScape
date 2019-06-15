#pragma once

#include "GunBase.h"

class RapidFireCannon
	:
	public GunBase
{
public:
	RapidFireCannon()
		:
		GunBase( "ItemDescriptions/Guns/RapidFireCannon.txt",
			"Images/Items/Guns/RapidFireCannon.bmp",
			0.19f,230.0f,2,2 )
	{}

	InventoryItem* Clone() override
	{
		return( new RapidFireCannon{ *this } );
	}

	void Shoot( InventoryEventInfo& invEvtInfo,const Vec2& target ) override
	{
		invEvtInfo.playerBullets.emplace_back(
			std::make_unique<Bullet>(
			invEvtInfo.player.GetPos(),target,
			invEvtInfo.map,Bullet::Team::Player1,
			bulletSpeed,Bullet::Size::Small,
			GetDamage() ) );

		invEvtInfo.playerBullets.back()->SetSubColor( bulletColor );
	}
};