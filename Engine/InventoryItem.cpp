#include "InventoryItem.h"
#include "SpriteEffect.h"
#include <fstream>
#include "Inventory.h"

const Font InventoryItem::luckyPixel = "Fonts/LuckyPixel6x9.bmp";
const Surface InventoryItem::itemBG = "Images/InventoryItemBackground.bmp";
const std::string InventoryItem::usedItemDesc = "This item has been all\nused up and is no\nlonger useful.";

// InventoryItem::InventoryItem( const std::string& name,
// 	const std::string& desc,const std::string& icon )
// 	:
// 	name( name ),
// 	description( GetPruned( desc ) ),
// 	surf( icon )
// {}

InventoryItem::InventoryItem( const std::string& fileName,
	const std::string& icon,int tier )
	:
	surf( icon ),
	tier( tier )
{
	std::ifstream in{ fileName };
	assert( in.good() );

	std::getline( in,name );
	usedName = "Used " + name;

	description = "";
	for( char c = in.get(); in.good(); c = in.get() )
	{
		description += c;
	}

	// description = GetPruned( description );
}

void InventoryItem::Update( const Mouse& mouse )
{
	hovering = ( RectI{ pos,size.x,size.y }.ContainsPoint(
		mouse.GetPos() ) );
}

void InventoryItem::Draw( Graphics& gfx,const Inventory* inv ) const
{
	Draw( pos,gfx,inv );
}

void InventoryItem::Draw( const Vei2& pos,Graphics& gfx,const Inventory* inv ) const
{
	if( deactivated )
	{
		gfx.DrawSprite( pos.x,pos.y,surf,
			SpriteEffect::Monochrome{ Colors::Magenta } );
	}
	else if( hovering && inv == nullptr )
	{
		gfx.DrawSprite( pos.x,pos.y,surf,
			SpriteEffect::SubstituteFade{ Colors::Magenta,
			Colors::White,0.25f } );
	}
	else
	{
		gfx.DrawSprite( pos.x,pos.y,surf,
			SpriteEffect::Chroma{ Colors::Magenta } );

		if( inv != nullptr )
		{
			const Vei2 padding = { 5,5 };
			Vei2 start = pos + size.Y() + padding.Y() -
				size.X() * int( toRemove.size() / 2 ) -
				padding.X() * int( toRemove.size() / 2 );
			for( int i = 0; i < int( toRemove.size() ); ++i )
			{
				inv->CFindItem( toRemove[i] )->Draw( start,gfx );

				start += size.X() + padding.X();
			}
		}
	}
}

void InventoryItem::DrawInfo( Graphics& gfx,const Inventory* inv ) const
{
	if( hovering )
	{
		luckyPixel.DrawText( GetName(),
			inv->GetNameStart(),Colors::White,gfx );

		luckyPixel.DrawText( GetDesc(),
			inv->GetDescStart(),Colors::White,gfx );
	}
}

void InventoryItem::SetPos( const Vei2& pos )
{
	this->pos = pos;
}

void InventoryItem::AddRemoveIndex( const std::string& index )
{
	for( auto item : toRemove )
	{
		if( item == index ) return;
	}
	toRemove.emplace_back( index );
}

void InventoryItem::Deactivate()
{
	deactivated = true;
}

void InventoryItem::UnHover()
{
	hovering = false;
}

void InventoryItem::Shoot( InventoryEventInfo& invEvtInfo,const Vec2& target )
{
	assert( false );
}

const Vei2& InventoryItem::GetPos() const
{
	return( pos );
}

bool InventoryItem::IsSelected() const
{
	return( hovering );
}

const std::string& InventoryItem::GetName() const
{
	if( !deactivated ) return( name );
	else return( usedName );
}

const std::string& InventoryItem::GetDesc() const
{
	if( !deactivated ) return( description );
	else return( usedItemDesc );
}

bool InventoryItem::WillRemove() const
{
	return( remove );
}

RectI InventoryItem::GetRect() const
{
	return( RectI{ pos,size.x,size.y } );
}

bool InventoryItem::IsGun() const
{
	return( false );
}

int InventoryItem::GetTier() const
{
	return( tier );
}

const std::vector<std::string>& InventoryItem::GetRemovalIndexes() const
{
	return( toRemove );
}

bool InventoryItem::IsDeactivated() const
{
	return( deactivated );
}

std::string InventoryItem::GetPruned( const std::string& in ) const
{
	return( in );

	// std::string temp;
	// 
	// for( char c : in )
	// {
	// 	temp += c;
	// 	if( temp.length() % 23 == 0 ) temp += '\n';
	// }
	// 
	// return( temp );
}
