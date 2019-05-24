#pragma once

#include "InventoryItem.h"

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
			fireRateBuffFactor *= buff;
		}

		shotTimer.Update( evtInfo.dt * fireRateBuffFactor );

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
		}
	}

	static void AddDamageBoost( InventoryEventInfo& evtInfo,
		int amountAdded )
	{
		damageBuffs.emplace_back( amountAdded );
	}

	static void AddFireRateBoost( InventoryEventInfo& evtInfo,
		float amount )
	{
		fireRateBuffs.emplace_back( amount );
	}

	static void RemoveDamageBoost( InventoryEventInfo& evtInfo,
		int itemToRemove )
	{
		for( auto it = damageBuffs.begin();
			it != damageBuffs.end(); ++it )
		{
			if( *it == itemToRemove )
			{
				damageBuffs.erase( it );
				break;
			}
		}
	}

	static void RemoveFireRateBoost( InventoryEventInfo& evtInfo,
		float itemToRemove )
	{
		for( auto it = fireRateBuffs.begin();
			it != fireRateBuffs.end(); ++it )
		{
			if( *it == itemToRemove )
			{
				fireRateBuffs.erase( it );
				break;
			}
		}
	}

	bool IsGun() const override
	{
		return( true );
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

	int GetDamage()
	{
		int curDamage = damage;
		for( auto& buff : damageBuffs )
		{
			curDamage += buff;
		}

		return( curDamage );
	}
protected:
	Timer shotTimer;
	const float bulletSpeed;
	const int damage;

	// std::vector<std::tuple<int,int,Timer>> damageBuffs;
	// std::vector<std::tuple<float,int,Timer>> fireRateBuffs;
	static std::vector<int> damageBuffs;
	static std::vector<float> fireRateBuffs;

	Color bulletColor = Colors::Magenta;
};