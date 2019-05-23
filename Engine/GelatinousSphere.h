#pragma once

#include "InventoryItem.h"

class GelatinousSphere
	:
	public InventoryItem
{
public:
	GelatinousSphere()
		:
		InventoryItem( "ItemDescriptions/GelatinousSphere.txt",
			"Images/Items/GelatinousSphere.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new GelatinousSphere );
	}

	void OnEnemyExplode( InventoryEventInfo& evtInfo ) override
	{
		if( Random::RangeF( 0.0f,100.0f ) < ringSpawnChance &&
			evtInfo.hitEnemy != nullptr )
		{
			spawnRingNextFrame = true;
			ringSpawnPos = evtInfo.hitEnemy->GetPos() +
				evtInfo.hitEnemy->GetRect().GetSize() / 2.0f;
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( spawnRingNextFrame )
		{
			const auto oldPlayerPos = evtInfo.player.GetPos();

			evtInfo.player.GetPos() = ringSpawnPos;

			for( int i = 0; i < nShots; ++i )
			{
				const auto vel = Vec2{ 0.0f,-1.0f }
					.Deviate( ( chili::pi * 2.0f ) *
					( float( i ) / float( nShots ) ) );

				evtInfo.items[0]->Shoot( evtInfo,
					Vec2( evtInfo.player.GetPos() + vel ) );
			}

			evtInfo.player.GetPos() = oldPlayerPos;

			spawnRingNextFrame = false;
		}
	}
private:
	static constexpr float ringSpawnChance = 50.0f;
	static constexpr int nShots = 8;
	bool spawnRingNextFrame = false;
	Vec2 ringSpawnPos;
};