#pragma once

#include "GunBase.h"

class OldRifle
	:
	public GunBase
{
public:
	OldRifle()
		:
		GunBase( "ItemDescriptions/Guns/OldRifle.txt",
			"Images/Items/Guns/OldRifle.bmp",
			0.23f * 2.0f,450.0f,2 )
	{}

	InventoryItem* Clone() override
	{
		return( new OldRifle{ *this } );
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