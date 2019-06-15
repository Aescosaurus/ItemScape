#pragma once

#include "GunBase.h"

class StoneSword
	:
	public GunBase
{
public:
	StoneSword()
		:
		GunBase( "ItemDescriptions/Guns/StoneSword.txt",
			"Images/Items/Guns/StoneSword.bmp",
			0.41f,250.0f,6,2 )
	{}

	InventoryItem* Clone() override
	{
		return( new StoneSword{ *this } );
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
		auto* replacement = new ExplodingBullet{
			*invEvtInfo.playerBullets.back() };
		replacement->SetExplBulletCount( 0 );
		replacement->SetExplTime( 0.6f );

		invEvtInfo.playerBullets.pop_back();
		invEvtInfo.playerBullets.emplace_back( replacement );
	}
};