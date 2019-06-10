#pragma once

#include "InventoryItem.h"

class KawaiiLemon
	:
	public InventoryItem
{
public:
	KawaiiLemon()
		:
		InventoryItem( "ItemDescriptions/KawaiiLemon.txt",
			"Images/Items/KawaiiLemon.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new KawaiiLemon{ *this } );
	}

	void OnEnemyExplode( InventoryEventInfo& evtInfo ) override;
private:
	static constexpr float chance = 5.0f;
};