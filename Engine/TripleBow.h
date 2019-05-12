#pragma once

#include "InventoryItem.h"
#include "Utils.h"

class TripleBow
	:
	public InventoryItem
{
public:
	TripleBow()
		:
		InventoryItem( "ItemDescriptions/TripleBow.txt",
			"Images/Items/TripleBow.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new TripleBow );
	}

	void OnPlayerShoot( InventoryEventInfo& invEvtInfo ) override
	{
		++curShot;
		if( curShot >= shotsToBuff )
		{
			auto& plBulls = invEvtInfo.playerBullets;

			plBulls.emplace_back( std
				::make_unique<Bullet>( *plBulls.back() ) );
			plBulls.emplace_back( std
				::make_unique<Bullet>( *plBulls.back() ) );

			Bullet* bull1 = plBulls[plBulls.size() - 1].get();
			Bullet* bull2 = plBulls[plBulls.size() - 2].get();

			bull1->GetVel() = bull1->GetVel().GetDeviated(
				devAmount ) * bull1->GetVel().GetLength();
			bull2->GetVel() = bull2->GetVel().GetDeviated(
				-devAmount ) * bull2->GetVel().GetLength();

			curShot = 0;
		}
	}
private:
	static constexpr float devAmount = chili::pi / 12.0f;
	static constexpr int shotsToBuff = 10;
	int curShot = 0;
};