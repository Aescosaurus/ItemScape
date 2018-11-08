#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>

class Anim
{
public:
	Anim( int x,int y,int width,int height,int count,const Surface& sheet,float holdTime,Color chroma = Colors::Magenta );
	Anim( const Anim& other );
	Anim& operator=( const Anim& other );

	void Update( float dt );
	template<typename Effect>
	void Draw( const Vei2& pos,Graphics& gfx,Effect e,bool flipped = false ) const
	{
		gfx.DrawSprite( int( pos.x ),int( pos.y ),
			frames[iCurFrame],Graphics::GetScreenRect(),
			sprite,e,flipped );
	}
	void Draw( const Vei2& pos,Graphics& gfx,bool flipped = false ) const;
	void Draw( const Vei2& pos,Graphics& gfx,const RectI& clip,bool flipped = false ) const;

	void SetFrame( int nextFrame );
	void ResetFinished();
	void Reset();

	bool IsFinished() const;
	int GetCurFrame() const;
	float GetPercent() const;
private:
	void Advance();
private:
	const Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	const float holdTime;
	float curFrameTime = 0.0f;
	bool finished = false;
};