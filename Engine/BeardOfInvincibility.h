#pragma once

#include "InventoryItem.h"

class BeardOfInvincibility
	:
	public InventoryItem
{
public:
	BeardOfInvincibility()
		:
		InventoryItem( "ItemDescriptions/BeardOfInvincibility.txt",
			"Images/Items/BeardOfInvincibility.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new BeardOfInvincibility );
	}

	void OnActivate( InventoryEventInfo& evtInfo ) override
	{
		if( !startedInvul )
		{
			startedInvul = true;
			evtInfo.player.SetInvulStatus( true );
			evtInfo.player.SetSubColor( Colors::Blue );
		}
	}

	void OnUpdate( InventoryEventInfo& evtInfo ) override
	{
		if( startedInvul )
		{
			invulTime.Update( evtInfo.dt );

			if( invulTime.IsDone() )
			{
				evtInfo.player.SetInvulStatus( false );
				evtInfo.player.SetSubColor( Colors::Magenta );

				remove = true;
			}
		}
	}
private:
	Timer invulTime = 10.0f;
	bool startedInvul = false;
};