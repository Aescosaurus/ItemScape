#pragma once

#include "Graphics.h"
#include "Vec2.h"
#include <string>
#include "Surface.h"
#include "Mouse.h"
#include "Rect.h"
#include "Font.h"

class InventoryItem
{
public:
	InventoryItem( const std::string& name,
		const std::string& desc,const std::string& icon );
	// Names/descriptions must be <23 chars in length.
	InventoryItem( const std::string& fileName,
		const std::string& icon );

	void Update( const Mouse& mouse );
	void Draw( Graphics& gfx );
	
	void SetPos( const Vei2& pos );

	const Vei2& GetPos() const;
	bool IsSelected() const;
	const std::string& GetName() const;
	const std::string& GetDesc() const;
private:
	// Returns string that will fit within inventory.
	std::string GetPruned( const std::string& in ) const;
public:
	static constexpr Vei2 size = { 32,32 };
private:
	std::string name;
	std::string description;
	const Surface surf;
	Vei2 pos;
	bool hovering = false;
	int count = 1;
	static const Font luckyPixel;
};