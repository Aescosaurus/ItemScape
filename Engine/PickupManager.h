#pragma once

#include "InventoryItem.h"
#include "Random.h"
#include "HealthCharge.h"
#include "FifthShotFlintlock.h"
#include "TwistyTie.h"

class PickupManager
{
public:
	static InventoryItem* RandPickup()
	{
		std::vector<InventoryItem*> options;

		options.emplace_back( new HealthCharge );
		options.emplace_back( new FifthShotFlintlock );
		options.emplace_back( new TwistyTie );
		// TODO: Add more options here.

		std::random_device rd;
		std::mt19937 rng{ rd() };
		std::shuffle( options.begin(),options.end(),rng );

		while( options.size() > 1 )
		{
			delete options.back();
			// options.erase( options.end() );
			options.pop_back();
		}

		return( options.front() );
	}
};