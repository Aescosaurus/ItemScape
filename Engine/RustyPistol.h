#pragma once

#include "GunBase.h"

class RustyPistol
	:
	public GunBase
{
public:
	RustyPistol()
		:
		GunBase( "ItemDescriptions/Guns/RustyPistol.txt",
			"Images/Items/Guns/RustyPistol.bmp",
			0.23f,324.2f,1 )
	{}

	InventoryItem* Clone() override
	{
		return( new RustyPistol );
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