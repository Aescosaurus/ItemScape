#pragma once

#include "InventoryItem.h"
#include "Random.h"

#include "HealthCharge.h"
#include "FifthShotFlintlock.h"
#include "TwistyTie.h"
#include "ChargeGenerator.h"
#include "CableSplitter.h"
#include "TripleBow.h"
#include "DamagingStake.h"
#include "BulletRingCapacitor.h"
#include "GPSTracker.h"
#include "CactusBranch.h"
#include "SingleUseGatling.h"
#include "BeardOfInvincibility.h"
#include "UggsOfMobility.h"
#include "ElectroBoots.h"
#include "PinkBunny.h"
#include "GogglesOfDestiny.h"
#include "CreepySpecs.h"
#include "ChiliTomatoNoodle.h"
#include "KawaiiLemon.h"
#include "FlamiumOre.h"
#include "RefinedFlamium.h"

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
		options.emplace_back( new DamagingStake );
		options.emplace_back( new BulletRingCapacitor );
		options.emplace_back( new GPSTracker );
		options.emplace_back( new CactusBranch );
		options.emplace_back( new SingleUseGatling );
		options.emplace_back( new BeardOfInvincibility );
		options.emplace_back( new UggsOfMobility );
		options.emplace_back( new ElectroBoots );
		options.emplace_back( new PinkBunny );
		options.emplace_back( new GogglesOfDestiny );
		options.emplace_back( new CreepySpecs );
		options.emplace_back( new ChiliTomatoNoodle );
		options.emplace_back( new KawaiiLemon );
		options.emplace_back( new FlamiumOre );
		options.emplace_back( new RefinedFlamium );

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