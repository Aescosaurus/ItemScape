#include "Inventory.h"

Inventory::Inventory()
{
	// for( int i = 0; i < size.x * size.y; ++i )
	// {
	// 	AddItem( "Hi","test desc","Images/Wall2.bmp" );
	// }
}

void Inventory::Update( const Keyboard& kbd,const Mouse& mouse )
{
	if( kbd.KeyIsPressed( 'I' ) || kbd.KeyIsPressed( VK_TAB ) )
	{
		if( active && canToggle )
		{
			active = false;
		}
		else if( !active && canToggle )
		{
			active = true;
		}

		canToggle = false;
	}
	else canToggle = true;

	for( auto& item : items )
	{
		item->Update( mouse );
	}

	for( auto it = items.begin(); it != items.end(); )
	{
		if( ( *it )->WillRemove() )
		{
			ShiftItems();
			items.erase( it );
			it = items.begin();
		}
		else
		{
			++it;
		}
	}
}

void Inventory::Draw( Graphics& gfx ) const
{
	if( active )
	{
		const auto drawPos = invStart - itemPadding;

		gfx.DrawRect( drawPos.x,drawPos.y,
			invSize.x,invSize.y,
			Colors::Gray );

		DrawInvGrid( gfx );

		for( const auto& item : items )
		{
			if( item->IsSelected() )
			{
				// gfx.DrawRect( nameStart.x - itemPadding.x,
				// 	nameStart.y - itemPadding.y,
				// 	invSize.x,invSize.y,Colors::Gray );

				luckyPixel.DrawText( item->GetName(),
					nameStart,Colors::White,gfx );

				luckyPixel.DrawText( item->GetDesc(),
					descStart,Colors::White,gfx );
			}
		}
	}
}

void Inventory::AddItem( const std::string& name,
	const std::string& desc,const std::string& icon )
{
	AddItem( new InventoryItem( name,desc,icon ) );
}

void Inventory::AddItem( InventoryItem* itemToAdd )
{
	items.emplace_back( itemToAdd );

	if( ( items.size() - 1 ) % size.x == 0 &&
		items.size() > 1 )
	{
		items.back()->SetPos( items[items.size() - 2]->GetPos().Y() +
			itemSize.Y() + itemPadding.Y() + invStart.X() );
	}
	else if( items.size() > 1 )
	{
		items.back()->SetPos( items[items.size() - 2]->GetPos() +
			itemSize.X() + itemPadding.X() );
	}
	else
	{
		items.back()->SetPos( invStart );
	}
}

void Inventory::ConsumeItem( const std::string& name )
{
	for( auto it = items.begin(); it != items.end(); ++it )
	{
		if( ( *it )->GetName() == name )
		{
			items.erase( it );
			break;
		}
	}

	ShiftItems();
}

void Inventory::OnPlayerHit( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		item->OnPlayerHit( evtInfo );
	}
}

InventoryItem* Inventory::FindItem( const std::string& name )
{
	for( auto& item : items )
	{
		if( item->GetName() == name )
		{
			return( item.get() );
		}
	}

	return( nullptr );
}

void Inventory::DrawInvGrid( Graphics& gfx ) const
{
	// auto curPos = invStart;
	for( int y = 0; y < size.y; ++y )
	{
		for( int x = 0; x < size.x; ++x )
		{
			if( y * size.x + x > int( items.size() ) - 1 ) return;
			items[y * size.x + x]->Draw( gfx );
			// curPos += itemSize.X();
			// curPos += itemPadding.X();
		}
		// curPos += itemSize.Y();
		// curPos += itemPadding.Y();
		// curPos.x = invStart.x;
	}
}

void Inventory::ShiftItems()
{
	// Re-order items after removal.
	for( int i = int( items.size() ) - 1; i > 0; --i )
	{
		items[i]->SetPos( items[i - 1]->GetPos() );
	}
}
