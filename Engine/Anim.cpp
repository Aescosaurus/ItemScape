#include "Anim.h"
#include "SpriteEffect.h"

Anim::Anim( int x,int y,int width,int height,int count,
	const Surface& sheet,float holdTime,Color chroma )
	:
	sprite( sheet ),
	holdTime( holdTime ),
	chroma( chroma )
{
	for( int i = 0; i < count; ++i )
	{
		frames.emplace_back( x + i * width,
			x + ( i + 1 ) * width,y,y + height );
	}
}

Anim::Anim( const Anim& other )
	:
	chroma( other.chroma ),
	sprite( other.sprite ),
	holdTime( other.holdTime )
{
	*this = other;
}

Anim& Anim::operator=( const Anim& other )
{
	iCurFrame = other.iCurFrame;
	curFrameTime = other.curFrameTime;
	frames = other.frames;
	return( *this );
}

void Anim::Update( float dt )
{
	curFrameTime += dt;
	while( curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Anim::Draw( const Vei2& pos,Graphics& gfx,bool flipped ) const
{
	Draw( pos,gfx,Graphics::GetScreenRect(),flipped );
}

void Anim::Draw( const Vei2& pos,Graphics& gfx,const RectI& clip,bool flipped ) const
{
	gfx.DrawSprite( int( pos.x ),int( pos.y ),
		frames[iCurFrame],clip,sprite,
		SpriteEffect::Chroma{ chroma },flipped );
}

void Anim::SetFrame( int nextFrame )
{
	iCurFrame = nextFrame;
}

void Anim::ResetFinished()
{
	finished = false;
}

void Anim::Reset()
{
	ResetFinished();
	SetFrame( 0 );
}

bool Anim::IsFinished() const
{
	return( finished );
}

int Anim::GetCurFrame() const
{
	return( iCurFrame );
}

float Anim::GetPercent() const
{
	return( ( float( iCurFrame ) / float( frames.size() ) ) * 100.0f );
}

void Anim::Advance()
{
	++iCurFrame;
	if( iCurFrame >= int( frames.size() ) )
	{
		iCurFrame = 0;
		finished = true;
	}
}