/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "TileMap.h"
#include "Player.h"
#include <vector>
#include "Bullet.h"
#include <memory>
#include "FloorLevel.h"
#include "Door.h"
#include "EnemyBase.h"
#include "GameState.h"
#include "CursorHandler.h"
#include "Inventory.h"
#include "VisualEffect.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
	~Game();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	void LoadNextLevel();
	bool IsLevelOver() const;
	InventoryEventInfo GenerateInvEvtInfo( float dt = 0.0f,
		EnemyBase* hitEnemy = nullptr,Bullet* curBullet = nullptr );
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	TileMap map;
	Player guy;
	std::vector<std::unique_ptr<Bullet>> playerBullets;
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	std::vector<std::unique_ptr<EnemyBase>> enemies;
	FloorLevel floor;
	std::vector<Door> doors;
	CursorHandler cursorHand;
	GameState state = GameState::Gameplay;
	Inventory playerInv;
	std::vector<VisualEffect> visualEffects;
	std::vector<InventoryItem*> pickups;
	Vei2 pickupPos = Graphics::GetScreenRect().GetCenter();
	bool spawnedEndOfLevelItem = false;
	/********************************/
};