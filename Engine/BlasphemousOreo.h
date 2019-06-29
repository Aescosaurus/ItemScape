#pragma once

#include "InventoryItem.h"
#include "GunBase.h"
#include "HealthCharge.h"

class BlasphemousOreo
	:
	public InventoryItem
{
public:
	BlasphemousOreo()
		:
		InventoryItem( "ItemDescriptions/BlasphemousOreo.txt",
			"Images/Items/BlasphemousOreo.bmp",2 )
	{}

	InventoryItem* Clone() override
	{
		return( new BlasphemousOreo{ *this } );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		if( !startedBuff )
		{
			startedBuff = true;

			evtInfo.player.SetInvulStatus( true );
			GunBase::AddDamageBoost( evtInfo,damageBoostAmount );

			for( int i = 0; i < healthChargeGainAmount; ++i )
			{
				evtInfo.pickups.emplace_back(
					new HealthCharge );
				evtInfo.pickups.back()->SetPos(
					evtInfo.player.GetPos() );
			}

			evtInfo.player.SetSubColor( Colors::Blue );
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( startedBuff )
		{
			buffDuration.Update( evtInfo.dt );
			if( buffDuration.IsDone() )
			{
				GunBase::RemoveDamageBoost( evtInfo,damageBoostAmount );

				evtInfo.player.SetInvulStatus( false );
				evtInfo.player.SetSubColor( Colors::Magenta );

				remove = true;
			}
		}
	}
private:
	Timer buffDuration = 15.0f;
	static constexpr int damageBoostAmount = 5;
	bool startedBuff = false;
	static constexpr int healthChargeGainAmount = 3;
};