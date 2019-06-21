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
	std::vector<std::unique_ptr<InventoryItem>>& items;
	Graphics& gfx;
	EnemyBase* hitEnemy;
	Bullet* curBullet;
};

class InventoryItem
{
public:
	// InventoryItem( const std::string& name,
	// 	const std::string& desc,const std::string& icon );

	// Names/descriptions must be <23 chars in length.
	InventoryItem( const std::string& fileName,
		const std::string& icon,int tier = 1 );

	void Update( const Mouse& mouse );
	void Draw( Graphics& gfx,const class Inventory* inv = nullptr ) const;
	void Draw( const Vei2& pos,Graphics& gfx,const class Inventory* inv = nullptr ) const;
	void DrawInfo( Graphics& gfx,const class Inventory* inv ) const;

	void SetPos( const Vei2& pos );
	void AddRemoveIndex( const std::string& index );
	void Deactivate();
	void UnHover();
	// Make sure to return new obj{ *this }
	virtual InventoryItem* Clone() = 0;
	/* Event methods */
	virtual void OnPlayerHit( InventoryEventInfo& evtInfo ) {}
	virtual void OnPlayerShoot( InventoryEventInfo& evtInfo ) {}
	virtual void OnEnemyExplode( InventoryEventInfo& evtInfo ) {}
	// When player left clicks, only works on first item in inv.
	virtual void OnGunFire( InventoryEventInfo& evtInfo ) {}
	virtual void OnActivate( InventoryEventInfo& evtInfo ) {}
	virtual void OnUpdate( InventoryEventInfo& evtInfo ) {}
	virtual void OnReceive( InventoryEventInfo& evtInfo ) {}
	virtual void OnRemove( InventoryEventInfo& evtInfo ) {}
	virtual void OnDraw( InventoryEventInfo& evtInfo ) {}
	virtual void OnEnemyHit( InventoryEventInfo& evtInfo ) {}
	virtual void OnRoomStart( InventoryEventInfo& evtInfo ) {}
	/**/

	/* Gun methods */
	virtual void Shoot( InventoryEventInfo& invEvtInfo,
		const Vec2& target );
	/**/

	const Vei2& GetPos() const;
	bool IsSelected() const;
	const std::string& GetName() const;
	const std::string& GetDesc() const;
	bool WillRemove() const;
	RectI GetRect() const;
	virtual bool IsGun() const;
	int GetTier() const;
	const std::vector<std::string>& GetRemovalIndexes() const;
	bool IsDeactivated() const;
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
	int tier;
	std::vector<std::string> toRemove;
	bool deactivated = false;
};