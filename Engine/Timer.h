#pragma once

#include "Random.h"

class Timer
{
public:
	Timer() = delete;
	Timer( float time,bool randomd = false )
		:
		maxTime( time ),
		curTime( randomd
			? Random::RangeF( -time,0.0f )
			: 0.0f )
	{}
	void Update( float dt )
	{
		if( curTime <= maxTime ) curTime += dt;
	}
	void Reset()
	{
		curTime = 0.0f;
	}
	void ResetRng()
	{
		curTime = Random::RangeF( -maxTime,0.0f );
	}
	void Reset( float newMaxTime )
	{
		Reset();
		maxTime = newMaxTime;
	}
	void SetMaxTime( float time )
	{
		maxTime = time;
	}
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
	float GetDuration() const
	{
		return( maxTime );
	}
private:
	float curTime;
	float maxTime;
};