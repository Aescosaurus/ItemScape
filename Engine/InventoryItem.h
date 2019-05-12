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
#include "EnemyBase.h"

class InventoryItem;

class InventoryEventInfo
{
public:
	Player& player;
	std::vector<std::unique_ptr<Bullet>>& enemyBullets;
	std::vector<VisualEffect>& visualEffects;
	std::vector<std::unique_ptr<Bullet>>& playerBullets;
	std::vector<std::unique_ptr<EnemyBase>>& enemies;
	std::vector<InventoryItem*>& pickups;
	float dt;
	Mouse& mouse;
	TileMap& map;
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
	void Draw( const Vei2& pos,Graphics& gfx ) const;

	void SetPos( const Vei2& pos );
	virtual InventoryItem* Clone() = 0;
	/*Event methods*/
	virtual void OnPlayerHit( InventoryEventInfo& evtInfo ) {}
	virtual void OnPlayerShoot( InventoryEventInfo& evtInfo ) {}
	virtual void OnEnemyExplode( InventoryEventInfo& evtInfo ) {}
	virtual void OnGunFire( InventoryEventInfo& evtInfo ) {}
	virtual void OnActivate( InventoryEventInfo& evtInfo ) {}
	virtual void OnUpdate( InventoryEventInfo& evtInfo ) {}
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
	static const Surface itemBG;
};