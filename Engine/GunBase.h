#pragma once

#include "InventoryItem.h"
#include <tuple>

class GunBase
	:
	public InventoryItem
{
public:
	// Call this from children!
	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		float fireRateBuffFactor = 1.0f;
		for( const auto& buff : fireRateBuffs )
		{
			fireRateBuffFactor *= std::get<0>( buff );
		}

		shotTimer.Update( evtInfo.dt * fireRateBuffFactor );

		for( auto it = damageBuffs.begin();
			it != damageBuffs.end(); ++it )
		{
			std::get<2>( *it ).Update( evtInfo.dt );

			if( std::get<1>( *it ) == 0 &&
				std::get<2>( *it ).IsDone() )
			{
				damageBuffs.erase( it );
				break;
			}
		}
		for( auto it = fireRateBuffs.begin();
			it != fireRateBuffs.end(); ++it )
		{
			std::get<2>( *it ).Update( evtInfo.dt );

			if( std::get<1>( *it ) == 0 &&
				std::get<2>( *it ).IsDone() )
			{
				fireRateBuffs.erase( it );
				break;
			}
		}

		bulletColor = ( ( damageBuffs.size() != 0 )
			? Colors::Red : Colors::Magenta );

		evtInfo.player.SetJustShot( false );
	}

	void OnGunFire( InventoryEventInfo& evtInfo ) override
	{
		if( shotTimer.IsDone() )
		{
			shotTimer.Reset();

			evtInfo.player.SetJustShot( true );

			Shoot( evtInfo,Vec2( evtInfo.mouse.GetPos() ) );

			for( auto& buff : damageBuffs )
			{
				if( std::get<1>( buff ) > 0 )
				{
					--std::get<1>( buff );
				}
			}
			for( auto& buff : fireRateBuffs )
			{
				if( std::get<1>( buff ) > 0 )
				{
					--std::get<1>( buff );
				}
			}
		}
	}

	// -1 nBuffedShots for infinite
	void BoostDamage( InventoryEventInfo& evtInfo,
		int amountAdded,int nBuffedShots,float duration ) override
	{
		damageBuffs.emplace_back( std::make_tuple(
			amountAdded,nBuffedShots,Timer{ duration } ) );
	}

	// -1 nBuffedShots for infinite
	void BoostFireRate( InventoryEventInfo& evtInfo,
		float amount,int nBuffedShots,float duration ) override
	{
		fireRateBuffs.emplace_back( std::make_tuple(
			amount,nBuffedShots,Timer{ duration } ) );
	}
protected:
	GunBase( const std::string& fileName,const std::string& icon,
		float shotRefireTime,float bulletSpeed,int damage )
		:
		InventoryItem( fileName,icon ),
		shotTimer( shotRefireTime ),
		bulletSpeed( bulletSpeed ),
		damage( damage )
	{}

	int GetDamage() const
	{
		int damage = 1;
		for( auto& buff : damageBuffs )
		{
			damage += std::get<0>( buff );
		}

		return( damage );
	}
protected:
	Timer shotTimer;
	const float bulletSpeed;
	const int damage;

	std::vector<std::tuple<int,int,Timer>> damageBuffs;
	std::vector<std::tuple<float,int,Timer>> fireRateBuffs;

	Color bulletColor = Colors::Magenta;
};