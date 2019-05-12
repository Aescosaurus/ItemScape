#include "InventoryItem.h"
#include "SpriteEffect.h"
#include <fstream>

const Font InventoryItem::luckyPixel = "Fonts/LuckyPixel6x9.bmp";

InventoryItem::InventoryItem( const std::string& name,
	const std::string& desc,const std::string& icon )
	:
	name( name ),
	description( GetPruned( desc ) ),
	surf( Surface{ icon },Vei2{ 4,4 } )
{}

InventoryItem::InventoryItem( const std::string& fileName,
	const std::string& icon )
	:
	surf( Surface{ icon },Vei2{ 4,4 } )
{
	std::ifstream in{ fileName };
	assert( in.good() );

	std::getline( in,name );

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

void InventoryItem::Draw( Graphics& gfx ) const
{
	// gfx.DrawRect( pos.x,pos.y,size.x,size.y,Colors::Magenta );

	gfx.DrawSprite( pos.x,pos.y,surf,
		SpriteEffect::Chroma{ Colors::Magenta } );

	// if( hovering )
	// {
	// 	gfx.DrawRect( pos.x,pos.y,size.x,size.y,Colors::White );
	// }
}

void InventoryItem::Draw( const Vei2& pos,Graphics& gfx ) const
{
	gfx.DrawSprite( pos.x,pos.y,surf,
		SpriteEffect::Chroma{ Colors::Magenta } );
}

void InventoryItem::SetPos( const Vei2& pos )
{
	this->pos = pos;
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
	return( name );
}

const std::string& InventoryItem::GetDesc() const
{
	return( description );
}

bool InventoryItem::WillRemove() const
{
	return( remove );
}

RectI InventoryItem::GetRect() const
{
	return( RectI{ pos,size.x,size.y } );
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
