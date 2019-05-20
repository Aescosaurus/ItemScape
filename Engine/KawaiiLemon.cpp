#include "KawaiiLemon.h"
#include "PickupManager.h"

void KawaiiLemon::OnEnemyExplode( InventoryEventInfo& evtInfo )
{
	if( Random::RangeF( 0.0f,100.0f ) < chance )
	{
		evtInfo.pickups.emplace_back( PickupManager
			::RandT1Pickup() );

		const auto pickupPos = Vei2( evtInfo.hitEnemy->GetPos() );

		evtInfo.pickups.back()->SetPos( pickupPos );
		evtInfo.visualEffects.emplace_back( VisualEffect{
			pickupPos,VisualEffect::Type
			::LightningDissipate } );
	}
}
