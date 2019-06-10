#pragma once

#include "InventoryItem.h"

class ElectroBoots
	:
	public InventoryItem
{
public:
	ElectroBoots()
		:
		InventoryItem( "ItemDescriptions/ElectroBoots.txt",
			"Images/Items/ElectroBoots.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new ElectroBoots{ *this } );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		if( !activated )
		{
			activated = true;
			evtInfo.player.MultiplyMoveSpeedFactor( speedupFactor );
			evtInfo.player.SetSubColor( Colors::Green );
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( activated )
		{
			speedupDuration.Update( evtInfo.dt );

			if( speedupDuration.IsDone() )
			{
				evtInfo.player.MultiplyMoveSpeedFactor( 1.0f / speedupFactor );
				evtInfo.player.SetSubColor( Colors::Magenta );
				remove = true;
			}
		}
	}
private:
	static constexpr float speedupFactor = 1.5f;
	Timer speedupDuration = 10.0f;
	bool activated = false;
};