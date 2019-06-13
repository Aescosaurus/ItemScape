#pragma once

#include "InventoryItem.h"
#include "GunBase.h"

class MithrilLetterOpener
	:
	public InventoryItem
{
public:
	MithrilLetterOpener()
		:
		InventoryItem( "ItemDescriptions/MithrilLetterOpener.txt",
			"Images/Items/MithrilLetterOpener.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new MithrilLetterOpener{ *this } );
	}

	void OnRoomStart( InventoryEventInfo& evtInfo ) override
	{
		GunBase::AddDamageBoost( evtInfo,damageBoost );
	}

	void OnPlayerShoot( InventoryEventInfo& evtInfo ) override
	{
		GunBase::RemoveDamageBoost( evtInfo,damageBoost );
	}
private:
	static constexpr int damageBoost = 5;
};