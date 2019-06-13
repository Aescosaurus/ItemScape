#pragma once

#include "InventoryItem.h"

class MithrilHammer
	:
	public InventoryItem
{
public:
	MithrilHammer()
		:
		InventoryItem( "ItemDescriptions/MithrilHammer.txt",
			"Images/Items/MithrilHammer.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new MithrilHammer{ *this } );
	}

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		if( Random::RangeI( 0,100 ) < bulletExplodeChance )
		{
			auto replacement = new ExplodingBullet{
				*evtInfo.playerBullets.back() };
			replacement->SetExplBulletCount( nBullets );
			replacement->SetExplTime( bulletSpawnTime );
			evtInfo.playerBullets.pop_back();
			evtInfo.playerBullets.emplace_back( replacement );
		}
	}
private:
	static constexpr int bulletExplodeChance = 30;
	static constexpr int nBullets = 3;
	static constexpr float bulletSpawnTime = 1.0f;
};