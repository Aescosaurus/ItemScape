#include "E_BeetleBig.h"

BeetleBig::BeetleBig( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bulletVec,
	std::vector<std::unique_ptr<EnemyBase>>& enemies )
	:
	EnemyBase( pos,size,myHP,map ),
	pBulletVec( &bulletVec ),
	pEnemyVec( &enemies ),
	walking( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	jumping( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	landing( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	explode( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{}

void BeetleBig::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( curState )
	{
	case State::Moving:
	{
		const auto testMove = vel * dt;
		const auto validMove = coll.GetValidMove( pos,testMove );
		pos += validMove;
		coll.MoveTo( pos );

		walking.Update( dt );
		retarget.Update( dt );
		if( retarget.IsDone() )
		{
			retarget.Reset();
			Retarget( info.playerPos );
		}
		moveReset.Update( dt );
		if( moveReset.IsDone() && walking.IsFinished() )
		{
			walking.Reset();
			moveReset.Reset();
			curState = State::WindingUp;
		}
		break;
	}
	case State::WindingUp:
		jumping.Update( dt );
		if( jumping.IsFinished() )
		{
			jumping.Reset();
			curState = State::WindingDown;
		}
		break;
	case State::WindingDown:
		landing.Update( dt );
		if( landing.IsFinished() )
		{
			landing.Reset();

			SpawnBulletCircle();

			Retarget( info.playerPos );
			curState = State::Moving;
		}
		break;
	case State::Exploding:
		if( !explode.IsFinished() ) explode.Update( dt );
		if( explode.IsFinished() )
		{
			explode.SetFrame( 3 );
			if( !deadDead )
			{
				deadDead = true;
				SpawnBulletCircle();
				// Spawn minis.
				static constexpr int nMinis = 7;
				for( int i = 0; i < nMinis; ++i )
				{
					pEnemyVec->emplace_back( std
						::make_unique<Beetle>( pos,
						*map,*pBulletVec ) );
				}
			}
		}
		break;
	}
}

void BeetleBig::Draw( Graphics& gfx ) const
{
	switch( curState )
	{
	case State::Moving:
		walking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::WindingUp:
		jumping.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::WindingDown:
		landing.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Exploding:
		explode.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}
	// gfx.DrawHitbox( coll.GetRect() );
}

void BeetleBig::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	if( IsDead() )
	{
		curState = State::Exploding;
		coll.MoveTo( Vec2{ -9999.0f,-9999.0f } );
	}
}

void BeetleBig::Retarget( const Vec2& theTarget )
{
	vel = ( theTarget - pos ).GetNormalized() * speed;
}

void BeetleBig::SpawnBulletCircle()
{
	// Spawn a ring of bullets going out every dir.
	const auto sPos = GetCenter();
	static constexpr auto bTeam = Bullet::Team::BeetleBig;
	static constexpr auto bSpd = bullSpeed;
	static constexpr auto bSize = Bullet::Size::Medium;

	// Don't need to normalize since it is telling target.
	// up,up-right,right,right-down,down,down-left,
	//  left,left-up
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ 0,-1 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ 1,-1 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ 1,0 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ 1,1 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ 0,1 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ -1,1 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ -1,0 },*map,bTeam,bSpd,bSize ) );
	pBulletVec->emplace_back( std::make_unique<Bullet>( sPos,
		sPos + Vec2{ -1,-1 },*map,bTeam,bSpd,bSize ) );
	// Whew that was gross let's hope I find a
	//  better way to do this soon.
}

Vec2 BeetleBig::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
