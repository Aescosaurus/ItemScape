#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "MainWindow.h"

#include "TileMap.h"
#include "Player.h"
#include <vector>
#include "Bullet.h"
#include <memory>
#include "FloorLevel.h"
#include "Door.h"
#include "EnemyBase.h"
#include "Inventory.h"
#include "VisualEffect.h"

class Campaign
{
public:
	Campaign( MainWindow& wnd,Graphics& gfx );
	~Campaign();

	void Update();
	void Draw();

	FloorLevel& GetFloor();
	Inventory& GetInv();
	void LoadNextLevel();
	void ClearEnemies();
	void SetUpLevelStart();
	bool IsLevelOver() const;
	InventoryEventInfo GenerateInvEvtInfo( float dt = 0.0f,
		EnemyBase* hitEnemy = nullptr,Bullet* curBullet = nullptr );
	BulletUpdateInfo GenerateBulletEvtInfo( float dt = 0.0f );
	void GotoNextFloor();
private:
	Keyboard& kbd;
	Mouse& mouse;
	Graphics& gfx;

	TileMap map;
	Player guy;
	std::vector<std::unique_ptr<Bullet>> playerBullets;
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	std::vector<std::unique_ptr<EnemyBase>> enemies;
	FloorLevel floor;
	std::vector<Door> doors;
	Inventory playerInv;
	std::vector<VisualEffect> visualEffects;
	std::vector<InventoryItem*> pickups;
	Vei2 pickupPos = Graphics::GetScreenRect().GetCenter();
	bool spawnedEndOfLevelItem = false;
	static constexpr float freezeFrameDuration = 0.015f;
	float frozenTime = 0.0f;
	bool shopping = false;
};