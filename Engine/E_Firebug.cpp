#include "E_Firebug.h"

Firebug::Firebug( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bulletVec )
	:
	EnemyBase( pos,size,myHP,map ),
	bulletVec( bulletVec ),
	walking( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	charging( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	attacking( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{
	moveRetarget.Update( 0.4f );
}

void Firebug::Update( const EnemyUpdateInfo& info,float dt )
{
	switch( curAction )
	{
	case State::Wander:
		walking.Update( dt );
		moveStop.Update( dt );

		moveRetarget.Update( dt );
		if( moveRetarget.IsDone() )
		{
			moveRetarget.Reset();
			vel = ( ( info.playerPos + info.playerVel * 2.0f ) -
				GetCenter() ).GetNormalized() * speed;
		}

		{
			const auto testMove = vel * dt;
			const auto validMove = coll.GetValidMove( pos,testMove );
			pos += validMove;
			coll.MoveTo( pos );
		}

		if( moveStop.IsDone() && walking.IsFinished() )
		{
			moveStop.Reset();
			walking.Reset();
			curAction = State::Charge;
		}
		break;
	case State::Charge:
		charging.Update( dt );
		if( charging.IsFinished() )
		{
			charging.Reset();
			curAction = State::AttackLeft;
		}
		break;
	case State::AttackLeft:
	{
		attacking.Update( dt );

		// Only fire a bullet if enough time has passed.
		shotTimer.Update( dt );
		if( !shotTimer.IsDone() ) break;
		shotTimer.Reset();

		// Find the next angle at which to fire a bullet.
		const float startAngle = Vec2{ 0.0f,-1.0f }.GetAngle();
		static constexpr float moveAmount = 3.141592f / float( nShotsASide );
		const float curAngle = startAngle - moveAmount * float( curShot );
		ShootBullet( curAngle );

		// If we've shot the last bullet switch phases.
		++curShot;
		if( curShot > nShotsASide )
		{
			curShot = 0;
			curAction = State::AttackRight;
		}
		break;
	}
	case State::AttackRight: // Look at comments above.
	{
		attacking.Update( dt );

		shotTimer.Update( dt );
		if( !shotTimer.IsDone() ) break;
		shotTimer.Reset();

		const float startAngle = Vec2{ 0.0f,-1.0f }.GetAngle();
		static constexpr float moveAmount = 3.141592f / float( nShotsASide );
		const float curAngle = startAngle + moveAmount * float( curShot );
		ShootBullet( curAngle );

		++curShot;
		if( curShot > nShotsASide )
		{
			curShot = 0;
			curAction = State::Wander;
		}
		break;
	}
	case State::Explode:

		break;
	}
}

void Firebug::Draw( Graphics& gfx ) const
{
	switch( curAction )
	{
	case State::Wander:
		walking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Charge:
		charging.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::AttackLeft:
		attacking.Draw( Vei2( pos ),gfx,true );
		break;
	case State::AttackRight:
		attacking.Draw( Vei2( pos ),gfx,false );
		break;
	case State::Explode:

		break;
	}
}

void Firebug::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );


}

void Firebug::ShootBullet( float angle )
{
	const Vec2 shotDir = Vec2::FromAngle( angle );

	const auto shotPos = GetCenter();
	const auto shotTarget = shotPos + shotDir;
	bulletVec.emplace_back( std::make_unique<Bullet>(
		shotPos,shotTarget,*map,Bullet::Team::Firebug,
		bulletSpeed,Bullet::Size::Small ) );
}

Vec2 Firebug::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}