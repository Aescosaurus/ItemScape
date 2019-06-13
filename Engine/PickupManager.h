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
#include "SpellBombWarp.h"
#include "BugSpray.h"
#include "GelatinousSphere.h"
#include "WatermelonOre.h"
#include "WatermelonIngot.h"
#include "PoisonBottle.h"
#include "Skateboard.h"
#include "MithrilHammer.h"
#include "MithrilLetterOpener.h"

#include "RustyPistol.h"
#include "OldRifle.h"
#include "DebilitatedShotgun.h"

class PickupManager
{
public:
	static void Initialize()
	{
		int id = 0;

		t1Pickups.emplace_back( new HealthCharge,id++ );
		t1Pickups.emplace_back( new FifthShotFlintlock,id++ );
		t1Pickups.emplace_back( new TwistyTie,id++ );
		t1Pickups.emplace_back( new ChargeGenerator,id++ );
		t1Pickups.emplace_back( new CableSplitter,id++ );
		t1Pickups.emplace_back( new TripleBow,id++ );
		t1Pickups.emplace_back( new DamagingStake,id++ );
		t1Pickups.emplace_back( new BulletRingCapacitor,id++ );
		t1Pickups.emplace_back( new GPSTracker,id++ );
		t1Pickups.emplace_back( new CactusBranch,id++ );
		t1Pickups.emplace_back( new SingleUseGatling,id++ );
		t1Pickups.emplace_back( new BeardOfInvincibility,id++ );
		t1Pickups.emplace_back( new UggsOfMobility,id++ );
		t1Pickups.emplace_back( new ElectroBoots,id++ );
		t1Pickups.emplace_back( new PinkBunny,id++ );
		t1Pickups.emplace_back( new GogglesOfDestiny,id++ );
		t1Pickups.emplace_back( new CreepySpecs,id++ );
		t1Pickups.emplace_back( new ChiliTomatoNoodle,id++ );
		t1Pickups.emplace_back( new KawaiiLemon,id++ );
		t1Pickups.emplace_back( new FlamiumOre,id++ );
		t1Pickups.emplace_back( new RefinedFlamium,id++ );
		t1Pickups.emplace_back( new SpellBombWarp,id++ );
		t1Pickups.emplace_back( new BugSpray,id++ );
		t1Pickups.emplace_back( new GelatinousSphere,id++ );
		t1Pickups.emplace_back( new WatermelonOre,id++ );
		t1Pickups.emplace_back( new WatermelonIngot,id++ );
		t1Pickups.emplace_back( new PoisonBottle,id++ );
		t1Pickups.emplace_back( new Skateboard,id++ );
		t1Pickups.emplace_back( new MithrilHammer,id++ );
		t1Pickups.emplace_back( new MithrilLetterOpener,id++ );

		t1Pickups.emplace_back( new RustyPistol,id++ );
		t1Pickups.emplace_back( new OldRifle,id++ );
		t1Pickups.emplace_back( new DebilitatedShotgun,id++ );
	}
	static void Free()
	{
		for( auto& item : t1Pickups )
		{
			delete item.first;
		}
		for( auto& item : t2Pickups )
		{
			delete item.first;
		}
	}
	static InventoryItem* RandT1Pickup()
	{
		return( t1Pickups[Random::RangeI( 0,
			int( t1Pickups.size() - 1 ) )].first->Clone() );
	}
	static InventoryItem* RandT2Pickup()
	{
		return( t2Pickups[Random::RangeI( 0,
			int( t2Pickups.size() - 1 ) )].first->Clone() );
	}
	static InventoryItem* CreateItem( const std::string& name )
	{
		for( auto& pickup : t1Pickups )
		{
			if( pickup.first->GetName() == name )
			{
				return( pickup.first->Clone() );
			}
		}
		return( nullptr );
	}
	// Returns copy, so don't drop it.
	static InventoryItem* FindItem( int id )
	{
		for( auto& item : t1Pickups )
		{
			if( item.second == id ) return( item.first->Clone() );
		}
		for( auto& item : t2Pickups )
		{
			if( item.second == id ) return( item.first->Clone() );
		}
		assert( false );
		return( nullptr );
	}
private:
	static std::vector<std::pair<InventoryItem*,int>> t1Pickups;
	static std::vector<std::pair<InventoryItem*,int>> t2Pickups;
};