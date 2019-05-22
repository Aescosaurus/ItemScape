#pragma once

#include "InventoryItem.h"
#include "SpriteEffect.h"

class GogglesOfDestiny
	:
	public InventoryItem
{
public:
	GogglesOfDestiny()
		:
		InventoryItem( "ItemDescriptions/GogglesOfDestiny.txt",
			"Images/Items/GogglesOfDestiny.bmp" )
	{}

	InventoryItem* Clone() override
	{
		return( new GogglesOfDestiny );
	}

	void OnDraw( InventoryEventInfo& evtInfo ) override
	{
		for( const auto& e : evtInfo.enemies )
		{
			const int pos = int( e->GetPos().x + e->GetHPPercent() *
				hpBar.GetWidth() - 1 );

			evtInfo.gfx.DrawSprite( int( e->GetPos().x ),
				int( e->GetPos().y + e->GetRect().GetHeight() ) + 5,hpBar,
				SpriteEffect::SubstitutePos{ Colors::Magenta,
				Colors::Green,Colors::Red,pos } );
		}
	}
private:
	const Surface hpBar = { Surface{ "Images/HealthBar.bmp" },Vei2{ 4,4 } };
};