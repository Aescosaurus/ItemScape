#include "E_BeetleBig.h"

BeetleBig::BeetleBig( const Vec2& pos,const TileMap& map,
	std::vector<Bullet>& bulletVec )
	:
	EnemyBase( pos,size,myHP,map ),
	pBulletVec( &bulletVec ),
	walking( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	jumping( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	landing( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f )
{}

void BeetleBig::Update( const Vec2& playerPos,float dt )
{
	switch( curState )
	{
	case State::Moving:
		pos += vel * dt;
		walking.Update( dt );
		retarget.Update( dt );
		if( retarget.IsDone() )
		{
			retarget.Reset();
			Retarget( playerPos );
		}
		moveReset.Update( dt );
		if( moveReset.IsDone() && walking.IsFinished() )
		{
			walking.Reset();
			moveReset.Reset();
			curState = State::WindingUp;
		}
		break;
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
			// TODO: Spawn bullets going in every direction.
			Retarget( playerPos );
			curState = State::Moving;
		}
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
	}
}

void BeetleBig::Retarget( const Vec2& theTarget )
{
	vel = ( theTarget - pos ).GetNormalized() * speed;
}
