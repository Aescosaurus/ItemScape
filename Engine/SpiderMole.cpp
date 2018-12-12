#include "SpiderMole.h"
#include "Random.h"
#include "SpriteEffect.h"

SpiderMole::SpiderMole( const Vec2& pos,const TileMap& map,
	std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,health,map ),
	pBulletVec( &bullets ),
	walking( 0,0,size.x,size.y,4,*sprSheet,0.2f ),
	rising( 0,size.y,size.x,size.y,4,*sprSheet,0.2f ),
	sinking( 0,size.y * 2,size.x,size.y,4,*sprSheet,0.2f ),
	exploding( 0,size.y * 3,size.x,size.y,4,*sprSheet,0.2f )
{
	ResetTargeting();
	coll.MoveTo( { -9999.0f,-9999.0f } );
}

void SpiderMole::Update( const EnemyUpdateInfo& info,float dt )
{
	EnemyBase::Update( info,dt );

	switch( action )
	{
	case State::Wander:
		walking.Update( dt );
		walkEnd.Update( dt );

		pos += vel * dt;

		if( map->GetTilePos( target ) ==
			map->GetTilePos( pos ) )
		{
			ResetTargeting();
		}

		if( walkEnd.IsDone() && walking.IsFinished() )
		{
			walkEnd.ResetRng();
			walking.Reset();
			vel = ( info.playerPos - pos ).GetNormalized() * fastSpeed;
			target = info.playerPos;
			action = State::Charge;
		}
		break;
	case State::Charge:
		walking.Update( dt );

		pos += vel * dt;
		if( map->GetTilePos( target ) ==
			map->GetTilePos( pos ) )
		{
			walking.Reset();
			coll.MoveTo( pos );
			action = State::Rise;
		}
		break;
	case State::Rise:
		rising.Update( dt );
		if( rising.IsFinished() )
		{
			rising.Reset();

			// Fire a ring of bullets.
			SpawnBulletCircle();

			action = State::Sink;
		}
		break;
	case State::Sink:
		// Reset targeting when done.
		sinking.Update( dt );
		if( sinking.IsFinished() )
		{
			sinking.Reset();
			ResetTargeting();
			coll.MoveTo( { -9999.0f,-9999.0f } );
			action = State::Wander;
		}
		break;
	case State::Explode:
		if( !exploding.IsFinished() ) exploding.Update( dt );
		if( exploding.IsFinished() ) exploding.SetFrame( 3 );
		break;
	}
}

void SpiderMole::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
	case State::Charge:
		walking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	case State::Rise:
		// Flash blue here.
		if( int( rising.GetPercent() ) % 2 == 0 )
		{
			rising.Draw( Vei2( pos ),gfx,SpriteEffect
				::SubstituteFade{ Colors::Magenta,Colors::Blue,0.5f },
				vel.x < 0.0f );
		}
		else
		{
			if( !justTookDamage ) rising.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
			else rising.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		}
		break;
	case State::Sink:
		if( !justTookDamage ) sinking.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		else sinking.Draw( Vei2( pos ),gfx,FlashCol(),vel.x < 0.0f );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx,vel.x < 0.0f );
		break;
	}

	// if( coll.GetRect().IsContainedBy( Rect( Graphics
	// 	::GetScreenRect() ) ) )
	// {
	// 	gfx.DrawHitbox( coll.GetRect() );
	// }
}

void SpiderMole::Attack( int damage,const Vec2& loc )
{
	// Spider Mole cannot be hit by bullets when underground.
	//  This shouldn't matter since the hitbox is far away anyway.
	switch( action )
	{
	case State::Rise:
	case State::Sink:
		EnemyBase::Attack( damage,loc );

		if( IsExpl() )
		{
			coll.MoveTo( { -9999.0f,-9999.0f } );
			action = State::Explode;
		}
		break;
	}
}

void SpiderMole::ResetTargeting()
{
	target = FindTarget();
	lastTarget = target;
	vel = ( target - pos ).GetNormalized() * speed;
}

void SpiderMole::SpawnBulletCircle()
{

	// Spawn a ring of bullets going out every dir.
	const auto sPos = GetCenter();
	static constexpr auto bTeam = Bullet::Team::SpiderMole;
	static constexpr auto bSpd = bulletSpeed;
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

Vec2 SpiderMole::FindTarget() const
{
	auto test = Vec2( pos );
	const int nTries = 5;
	int curTries = 0;

	static constexpr float moveTolerance = 163.4f;
	do
	{
		test = Vec2( pos );
		test.x += Random::RangeF( -moveTolerance,moveTolerance );
		test.y += Random::RangeF( -moveTolerance,moveTolerance );

		++curTries;
		if( curTries >= nTries ) // No infinite loops here!
		{
			test = lastTarget;
			break;
		}
	} while( !Graphics::GetScreenRect()
		.ContainsPoint( Vei2( Vec2{ test.x,test.y } ) ) );

	return( test );
}

Vec2 SpiderMole::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
