#pragma once

#include "InventoryItem.h"
#include "GunBase.h"

class ChiliTomatoNoodle
	:
	public InventoryItem
{
public:
	ChiliTomatoNoodle()
		:
		InventoryItem( "ItemDescriptions/ChiliTomatoNoodle.txt",
			"Images/Items/ChiliTomatoNoodle.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new ChiliTomatoNoodle{ *this } );
	}

	void OnEnemyExplode( InventoryEventInfo& evtInfo ) override
	{
		if( int( boostTimers.size() ) < maxBoosts )
		{
			GunBase::AddFireRateBoost( evtInfo,fireRateBoost );
			boostTimers.emplace_back( Timer{ boostDuration } );
		}

		for( auto& timer : boostTimers )
		{
			timer.Reset();
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		for( auto it = boostTimers.begin();
			it != boostTimers.end(); ++it )
		{
			it->Update( evtInfo.dt );

			if( it->IsDone() )
			{
				GunBase::RemoveFireRateBoost( evtInfo,fireRateBoost );
				it = boostTimers.erase( it );
				if( it == boostTimers.end() ) break;
			}
		}
	}
private:
	static constexpr float fireRateBoost = 1.05f;
	static constexpr float boostDuration = 10.0f;
	std::vector<Timer> boostTimers;
	static constexpr int maxBoosts = 10;
};