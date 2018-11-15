#include "E_SpiderMole.h"
#include "Random.h"

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
}

void SpiderMole::Update( const EnemyUpdateInfo& info,float dt )
{
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
			action = State::Wander;
		}
		break;
	case State::Explode:
		break;
	}
}

void SpiderMole::Draw( Graphics& gfx ) const
{
	switch( action )
	{
	case State::Wander:
	case State::Charge:
		walking.Draw( Vei2( pos ),gfx,false );
		break;
	case State::Rise:
		// Flash blue here.
		rising.Draw( Vei2( pos ),gfx );
		break;
	case State::Sink:
		sinking.Draw( Vei2( pos ),gfx );
		break;
	case State::Explode:
		exploding.Draw( Vei2( pos ),gfx );
		break;
	}
}

void SpiderMole::Attack( int damage,const Vec2& loc )
{
	EnemyBase::Attack( damage,loc );

	// TODO: Explode when out of health.
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
