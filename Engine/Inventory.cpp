#include "Inventory.h"
#include "PickupManager.h"

Inventory::Inventory()
{
	// for( int i = 0; i < size.x * size.y; ++i )
	// {
	// 	AddItem( "Hi","test desc","Images/Wall2.bmp" );
	// }
}

void Inventory::Update( const Keyboard& kbd,const Mouse& mouse,
	InventoryEventInfo& invEvtInfo )
{
	// Handle open/closing inventory.
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

	// Update items when inventory is not open.
	if( !active )
	{
		for( auto& item : items )
		{
			if( !item->IsDeactivated() )
			{
				item->OnUpdate( invEvtInfo );
			}
		}
	}

	// Deal with activated items.
	if( mouse.LeftIsPressed() )
	{
		if( !items[0]->IsDeactivated() )
		{
			items[0]->OnGunFire( invEvtInfo );
		}
	}

	if( kbd.KeyIsPressed( VK_SHIFT ) )
	{
		if( !shiftPause && !items[1]->IsDeactivated() )
		{
			items[1]->OnActivate( invEvtInfo );
		}
		shiftPause = true;
	}
	else shiftPause = false;

	if( mouse.RightIsPressed() )
	{
		if( !rightClickPause && !items[2]->IsDeactivated() )
		{
			items[2]->OnActivate( invEvtInfo );
		}
		rightClickPause = true;
	}
	else rightClickPause = false;

	// Update item hovering status.
	if( active )
	{
		for( auto& item : items )
		{
			item->Update( mouse );
		}
	}
	else
	{
		for( auto& item : items )
		{
			item->UnHover();
		}
	}

	// Remove/organize items, deal with item swapping.
	for( auto it = items.begin(); it != items.end(); )
	{
		if( ( *it )->WillRemove() &&
			!( ( *it )->IsDeactivated() ) )
		{
			( *it )->OnRemove( invEvtInfo );
			( *it )->Deactivate();
			// items.erase( it );
			// ReorganizeInventory();
			// it = items.begin();
		}
		else
		{
			if( canSwapItems && active )
			{
				if( mouse.LeftIsPressed() )
				{
					if( ( *it )->GetRect().ContainsPoint(
						mouse.GetPos() ) && !holdingItem )
					{
						selectedItem = it;
						holdingItem = true;
					}
				}
				else if( ( ( *it )->GetRect().ContainsPoint(
					mouse.GetPos() ) ) && holdingItem )
				{
					std::iter_swap( it,selectedItem );
					holdingItem = false;
					ReorganizeInventory();
					break;
				}
			}

			++it;
		}
	}

	if( holdingItem && canSwapItems ) heldItemPos = mouse.GetPos();
}

void Inventory::Draw( Graphics& gfx ) const
{
	if( active )
	{
		const auto drawPos = invStart - itemPadding;

		gfx.DrawRect( drawPos.x,drawPos.y,
			invSize.x,invSize.y,
			Colors::Slate );

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

		gfx.DrawSprite( invStart.x,invStart.y -
			invInstructions.GetHeight(),
			invInstructions,
			SpriteEffect::Chroma{ Colors::Magenta } );

		if( holdingItem )
		{
			( *selectedItem )->Draw( heldItemPos -
				( *selectedItem )->size / 2,gfx );
		}
	}
	else
	{
		// Draw item overlay.
		gfx.DrawRect( overlayStart.x,overlayStart.y,
			itemSize.x * 3 + itemPadding.x * 3,
			itemSize.y + itemPadding.y * 2,
			Colors::Slate );

		gfx.DrawSprite( overlayStart.x,overlayStart.y + 32 + 5 * 3,
			invOverlayInstructions,
			SpriteEffect::Chroma{ Colors::Magenta } );

		if( items.size() > 0 ) items[0]->Draw( overlayStart +
			itemPadding,gfx );
		if( items.size() > 1 ) items[1]->Draw( overlayStart +
			itemPadding + itemPadding.X() + itemSize.X(),gfx );
		if( items.size() > 2 ) items[2]->Draw( overlayStart +
			itemPadding + itemPadding.X() * 2 + itemSize.X() * 2,gfx );
	}
}

// void Inventory::AddItem( const std::string& name,
// 	const std::string& desc,const std::string& icon )
// {
// 	AddItem( new InventoryItem( name,desc,icon ) );
// }

void Inventory::AddItem( InventoryItem* itemToAdd,
	InventoryEventInfo& evtInfo )
{
	AddItem( itemToAdd );

	items.back()->OnReceive( evtInfo );

	const auto& indexes = itemToAdd->GetRemovalIndexes();
	if( indexes.size() > 0 )
	{
		assert( itemToAdd->GetTier() == 2 );
		for( auto index : indexes )
		{
			RemoveItem( index );
		}

		ReorganizeInventory();
	}
	// itemToAdd->ClearRemovalIndexes();
}

void Inventory::ConsumeItem( const std::string& name )
{
	assert( false ); // TODO: Fix this if you ever want to use it.

	for( auto it = items.begin(); it != items.end(); ++it )
	{
		if( ( *it )->GetName() == name )
		{
			items.erase( it );
			break;
		}
	}

	// ShiftItems();
}

void Inventory::LoadSaveInfo( const std::string& info )
{
	std::vector<std::string> lines;

	lines.emplace_back( "" );

	for( char c : info )
	{
		if( c == '\n' ) lines.emplace_back( "" );
		else lines.back() += c;
	}
	lines.pop_back(); // 

	for( auto& line : lines )
	{
		const bool itemIsUsed = ( line.substr( 0,4 ) == "Used" );
		if( itemIsUsed ) line = line.substr( 4 + 1,std::string::npos );
		InventoryItem* temp = PickupManager::CreateItem( line );

		if( temp != nullptr )
		{
			if( itemIsUsed ) temp->Deactivate();
			AddItem( temp );
		}
	}
}

void Inventory::ToggleItemSwapping( bool swappable )
{
	// canSwapItems = swappable;
}

void Inventory::OnPlayerHit( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnPlayerHit( evtInfo );
	}
}

void Inventory::OnPlayerShoot( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnPlayerShoot( evtInfo );
	}
}

void Inventory::OnEnemyExplode( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnEnemyExplode( evtInfo );
	}
}

void Inventory::OnEnemyHit( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnEnemyHit( evtInfo );
	}
}

void Inventory::OnDraw( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnDraw( evtInfo );
	}
}

void Inventory::OnRoomStart( InventoryEventInfo& evtInfo )
{
	for( auto& item : items )
	{
		if( !item->IsDeactivated() ) item->OnRoomStart( evtInfo );
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

const InventoryItem* Inventory::CFindItem( const std::string& name ) const
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

const InventoryItem* Inventory::GetItem( int index ) const
{
	return( items[index].get() );
}

std::vector<std::unique_ptr<InventoryItem>>& Inventory::GetItemVec()
{
	return( items );
}

bool Inventory::IsOpen() const
{
	return( active );
}

std::string Inventory::GenerateSaveInfo() const
{
	std::string temp = "";

	for( const auto& item : items )
	{
		// if( item->IsDeactivated() ) temp += "Used ";
		temp += item->GetName() + '\n';
	}

	return( temp );
}

const Vei2& Inventory::GetNameStart() const
{
	return( nameStart );
}

const Vei2& Inventory::GetDescStart() const
{
	return( descStart );
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

void Inventory::ShiftItems( std::vector<std::unique_ptr<
	InventoryItem>>::iterator spot )
{
	// Re-order items after removal.
	// for( int i = int( items.size() ) - 1; i > 0; --i )
	// {
	// 	items[i]->SetPos( items[i - 1]->GetPos() );
	// }
	for( auto it = items.end() - 1; it != spot; --it )
	{
		( *it )->SetPos( ( *( it - 1 ) )->GetPos() );
	}
}

void Inventory::ReorganizeInventory()
{
	std::vector<std::unique_ptr<InventoryItem>> oldItems;
	for( auto& item : items )
	{
		oldItems.emplace_back( item->Clone() );
	}

	items.clear();

	for( auto& item : oldItems )
	{
		AddItem( item->Clone() );
	}
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

void Inventory::RemoveItem( const std::string& index )
{
	for( auto it = items.begin(); it != items.end(); ++it )
	{
		if( ( *it )->GetName() == index )
		{
			items.erase( it );
			break;
		}
	}
}
