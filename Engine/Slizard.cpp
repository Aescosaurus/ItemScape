#include "Slizard.h"

Slizard::Slizard( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	walking( 0,0,size.x,size.y,4,*surfSheet,0.2f ),
	charging( 0,size.y,size.x,size.y,4,*surfSheet,0.2f ),
	attacking( 0,size.y * 2,size.x,size.y,4,*surfSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*surfSheet,0.2f )
{}
