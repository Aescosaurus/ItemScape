#pragma once

#include "InventoryItem.h"
#include "Utils.h"

class DebilitatedShotgun
	:
	public InventoryItem
{
public:
	DebilitatedShotgun()
		:
		InventoryItem( "ItemDescriptions/Guns/DebilitatedShotgun.txt",
			"Images/Items/Guns/DebilitatedShotgun.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new DebilitatedShotgun );
	}

	bool IsGun() const override
	{
		return( true );
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

			player.SetJustShot( true );

			Shoot( invEvtInfo,Vec2( invEvtInfo.mouse.GetPos() ) );

			shotTimer.Reset();
		}
	}

	void Shoot( InventoryEventInfo& invEvtInfo,const Vec2& target ) override
	{
		auto& player = invEvtInfo.player;
		auto& plBulls = invEvtInfo.playerBullets;

		plBulls.emplace_back( std::make_unique<Bullet>(
			player.GetPos(),target,
			invEvtInfo.map,Bullet::Team::Player1,bulletSpeed,
			Bullet::Size::Small,damage ) );
		for( int i = 0; i < nPellets - 1; ++i )
		{
			plBulls.emplace_back( plBulls.back()->Clone() );
		}

		for( int i = int( plBulls.size() ) - nPellets;
			i < int( plBulls.size() ); ++i )
		{
			const auto devAmount = Random::RangeF(
				-devRange,devRange );
			plBulls[i]->GetVel() = plBulls[i]->GetVel()
				.GetDeviated( devAmount ) * plBulls[i]
				->GetVel().GetLength();
		}
	}
private:
	Timer shotTimer = 0.23f * 3.0f;
	static constexpr float bulletSpeed = 200.0f;
	static constexpr int damage = 1;
	static constexpr int nPellets = 3;
	static constexpr float devRange = chili::pi / 3.5f;
};