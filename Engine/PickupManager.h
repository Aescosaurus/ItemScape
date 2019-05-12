#pragma once

#include "InventoryItem.h"
#include "Random.h"
#include "HealthCharge.h"
#include "FifthShotFlintlock.h"
#include "TwistyTie.h"
#include "ChargeGenerator.h"
#include "CableSplitter.h"
#include "TripleBow.h"
#include "RustyPistol.h"
#include "OldRifle.h"
#include "DebilitatedShotgun.h"

class PickupManager
{
public:
	static InventoryItem* RandT1Pickup()
	{
		std::vector<InventoryItem*> options;

		options.emplace_back( new HealthCharge );
		options.emplace_back( new FifthShotFlintlock );
		options.emplace_back( new TwistyTie );
		options.emplace_back( new ChargeGenerator );
		options.emplace_back( new CableSplitter );
		options.emplace_back( new TripleBow );

		options.emplace_back( new RustyPistol );
		options.emplace_back( new OldRifle );
		options.emplace_back( new DebilitatedShotgun );

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
	static InventoryItem* RandT2Pickup()
	{
		std::vector<InventoryItem*> options;

		// TODO: Add items here.

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