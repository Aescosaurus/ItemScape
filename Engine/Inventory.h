#pragma once

#include "InventoryItem.h"
#include <memory>
#include <vector>
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Font.h"

class Inventory
{
public:
	Inventory();

	void Update( const Keyboard& kbd,const Mouse& mouse,
		InventoryEventInfo& invEvtInfo );
	void Draw( Graphics& gfx ) const;

	// void AddItem( const std::string& name,
	// 	const std::string& desc,const std::string& icon );
	void AddItem( InventoryItem* itemToAdd );
	void ConsumeItem( const std::string& name );
	/*Event methods*/
	void OnPlayerHit( InventoryEventInfo& evtInfo );
	void OnPlayerShoot( InventoryEventInfo& evtInfo );
	void OnEnemyExplode( InventoryEventInfo& evtInfo );
	/**/

	InventoryItem* FindItem( const std::string& name );
	std::vector<std::unique_ptr<InventoryItem>>& GetItemVec();
	bool IsOpen() const;
private:
	void DrawInvGrid( Graphics& gfx ) const;
	
	void ShiftItems( std::vector<std::unique_ptr<
		InventoryItem>>::iterator spot );
	void ReorganizeInventory();
private:
	std::vector<std::unique_ptr<InventoryItem>> items;

	bool active = false;
	bool canToggle = false;

	std::vector<std::unique_ptr<InventoryItem>>::iterator selectedItem;
	bool holdingItem = false;
	Vei2 heldItemPos;

	const Font luckyPixel = "Fonts/LuckyPixel6x9.bmp";

	static constexpr Vei2 size = { 15,5 };

	static constexpr Vei2 invStart = { 250,150 };
	static constexpr Vei2 itemSize = InventoryItem::size;
	static constexpr Vei2 itemPadding = { 5,5 };

	static constexpr Vei2 invSize = { itemSize.x * size.x +
		itemPadding.x * ( size.x + 1 ),
		itemSize.y * size.y +
		itemPadding.y * ( size.y + 1 ) };

	static constexpr Vei2 nameStart = invStart +
		itemSize.Y() * size.y + itemPadding.Y() * ( size.y + 1 );
	static constexpr Vei2 descStart = nameStart + Vei2{ 0,32 + 16 };

	const Surface invInstructions = { Surface{ "Images/InventoryInstructions.bmp" },Vei2{ 4,4 } };
};