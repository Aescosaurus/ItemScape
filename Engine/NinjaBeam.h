#pragma once

#include "GunBase.h"

class NinjaBeam
	:
	public GunBase
{
public:
	NinjaBeam()
		:
		GunBase( "ItemDescriptions/Guns/NinjaBeam.txt",
			"Images/Items/Guns/NinjaBeam.bmp",1.0f,350.0f,1,2 )
	{}

	InventoryItem* Clone() override
	{
		return( new NinjaBeam{ *this } );
	}

	void OnGunFire( InventoryEventInfo& evtInfo ) override
	{
		if( shotTimer.IsDone() )
		{
			evtInfo.player.SetJustShot( true );

			shotRefire.Update( evtInfo.dt );
			if( shotRefire.IsDone() )
			{
				shotRefire.Reset();
				Shoot( evtInfo,Vec2( evtInfo.mouse.GetPos() ) );

				++curShot;
				if( curShot > nShotsPerVolley )
				{
					curShot = 0;
					shotTimer.Reset();
				}
			}
		}
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
private:
	static constexpr int nShotsPerVolley = 7;
	int curShot = 0;
	Timer shotRefire = 0.036f;
};