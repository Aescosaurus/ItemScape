#pragma once

#include "Graphics.h"
#include "Vec2.h"
#include <string>
#include "Surface.h"
#include "Mouse.h"
#include "Rect.h"
#include "Font.h"
#include "Player.h"
#include "Bullet.h"
#include "VisualEffect.h"

class InventoryEventInfo
{
public:
	InventoryEventInfo( Player& player,
		std::vector<std::unique_ptr<Bullet>>& enemyBullets,
		std::vector<VisualEffect>& visualEffects,
		std::vector<std::unique_ptr<Bullet>>& playerBullets )
		:
		player( player ),
		enemyBullets( enemyBullets ),
		visualEffects( visualEffects ),
		playerBullets( playerBullets )
	{}
public:
	Player& player;
	std::vector<std::unique_ptr<Bullet>>& enemyBullets;
	std::vector<VisualEffect>& visualEffects;
	std::vector<std::unique_ptr<Bullet>>& playerBullets;
};

class InventoryItem
{
public:
	InventoryItem( const std::string& name,
		const std::string& desc,const std::string& icon );
	// Names/descriptions must be <23 chars in length.
	InventoryItem( const std::string& fileName,
		const std::string& icon );

	void Update( const Mouse& mouse );
	void Draw( Graphics& gfx ) const;
	
	void SetPos( const Vei2& pos );
	/*Event methods*/
	virtual void OnPlayerHit( InventoryEventInfo& evtInfo ) {}
	virtual void OnPlayerShoot( InventoryEventInfo& evtInfo ) {}
	/**/

	const Vei2& GetPos() const;
	bool IsSelected() const;
	const std::string& GetName() const;
	const std::string& GetDesc() const;
	bool WillRemove() const;
	RectI GetRect() const;
private:
	// Returns string that will fit within inventory.
	std::string GetPruned( const std::string& in ) const;
public:
	static constexpr Vei2 size = { 32,32 };
protected:
	std::string name;
	std::string description;
	const Surface surf;
	Vei2 pos;
	bool hovering = false;
	static const Font luckyPixel;
	bool remove = false;
};