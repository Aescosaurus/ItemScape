#pragma once

#include "InventoryItem.h"

class GunBase
	:
	public InventoryItem
{
public:
	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		shotTimer.Update( evtInfo.dt * fireRateBoostFactor );

		evtInfo.player.SetJustShot( false );
	}

	void OnGunFire( InventoryEventInfo& evtInfo ) override
	{
		if( shotTimer.IsDone() )
		{
			shotTimer.Reset();

			evtInfo.player.SetJustShot( true );

			Shoot( evtInfo,Vec2( evtInfo.mouse.GetPos() ) );

			--nRemainingDamageAddShots;
			--nRemainingFireRateBuffShots;
			if( nRemainingDamageAddShots <= 0 )
			{
				nRemainingDamageAddShots = 0;
				damageAdd = 0;
				bulletColor = Colors::Magenta;
			}
			if( nRemainingFireRateBuffShots <= 0 )
			{
				nRemainingFireRateBuffShots = 0;
				fireRateBoostFactor = 1.0f;
			}
		}
	}

	void BoostDamage( InventoryEventInfo& evtInfo,
		int amountAdded,int nBuffedShots ) override
	{
		damageAdd = amountAdded;
		nRemainingDamageAddShots = nBuffedShots;

		bulletColor = Colors::Red;
	}

	void BoostFireRate( InventoryEventInfo& evtInfo,
		float amount,int nBuffedShots ) override
	{
		fireRateBoostFactor = amount;
		nRemainingFireRateBuffShots = nBuffedShots;
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
protected:
	Timer shotTimer;
	const float bulletSpeed;
	const int damage;

	int damageAdd = 0;
	int nRemainingDamageAddShots = 0;
	float fireRateBoostFactor = 1.0f;
	int nRemainingFireRateBuffShots = 0;

	Color bulletColor = Colors::Magenta;
};