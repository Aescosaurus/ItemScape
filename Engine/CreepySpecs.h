#pragma once

#include "InventoryItem.h"

class CreepySpecs
	:
	public InventoryItem
{
private:
	class HitNumber
	{
	public:
		HitNumber( const Vec2& pos,int number )
			:
			pos( pos ),
			number( number ),
			xMove( Random::RangeF( -xAcc,xAcc ) )
		{}
		void Update( float dt )
		{
			vel.x += xMove * dt;
			vel.y += gravAcc * dt;

			pos += vel * dt;

			despawnTime.Update( dt );
		}
		void Draw( const Font& font,Graphics& gfx ) const
		{
			font.DrawText( std::to_string( number ),
				Vei2( pos ),Colors::White,gfx );
		}
		bool IsDone() const
		{
			return( despawnTime.IsDone() );
		}
	public:
		Vec2 pos;
		int number;
		static constexpr float gravAcc = 50.0f * 50.0f;
		Vec2 vel = { 0.0f,-gravAcc / 5.0f };
		static constexpr float xAcc = 10.0f * 60.0f;
		float xMove = 0.0f;
		Timer despawnTime = 0.5f;
	};
public:
	CreepySpecs()
		:
		InventoryItem( "ItemDescriptions/CreepySpecs.txt",
			"Images/Items/CreepySpecs.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new CreepySpecs{ *this } );
	}

	void OnEnemyHit( InventoryEventInfo& evtInfo ) override
	{
		damageNumbers.emplace_back( HitNumber{ evtInfo.hitEnemy
			->GetPos(),evtInfo.curBullet->GetDamage() } );
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		for( auto it = damageNumbers.begin();
			it != damageNumbers.end(); ++it )
		{
			it->Update( evtInfo.dt );

			if( it->IsDone() )
			{
				it = damageNumbers.erase( it );

				if( it == damageNumbers.end() ) break;
			}
		}
	}

	void OnDraw( InventoryEventInfo& evtInfo ) override
	{
		for( const auto& dmgNum : damageNumbers )
		{
			dmgNum.Draw( luckyPixel,evtInfo.gfx );
		}
	}
private:
	std::vector<HitNumber> damageNumbers;
	const Font luckyPixel = "Fonts/LuckyPixel6x9.bmp";
};