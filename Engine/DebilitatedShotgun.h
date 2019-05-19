#pragma once

#include "GunBase.h"
#include "Utils.h"

class DebilitatedShotgun
	:
	public GunBase
{
public:
	DebilitatedShotgun()
		:
		GunBase( "ItemDescriptions/Guns/DebilitatedShotgun.txt",
			"Images/Items/Guns/DebilitatedShotgun.bmp",
			0.23f * 3.0f,200.0f,1 )
	{}

	InventoryItem* Clone() override
	{
		return( new DebilitatedShotgun );
	}

	void Shoot( InventoryEventInfo& invEvtInfo,const Vec2& target ) override
	{
		auto& player = invEvtInfo.player;
		auto& plBulls = invEvtInfo.playerBullets;

		plBulls.emplace_back( std::make_unique<Bullet>(
			player.GetPos(),target,invEvtInfo.map,
			Bullet::Team::Player1,bulletSpeed,
			Bullet::Size::Small,damage ) );

		for( int i = 0; i < nPellets - 1; ++i )
		{
			plBulls.emplace_back( plBulls.back()->Clone() );

			invEvtInfo.playerBullets.back()->SetSubColor( bulletColor );
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
	static constexpr int nPellets = 3;
	static constexpr float devRange = chili::pi / 3.5f;
};