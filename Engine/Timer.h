#pragma once

class Timer
{
public:
	Timer() = delete;
	Timer( float time )
		:
		maxTime( time ),
		curTime( 0.0f )
	{}
	void Update( float dt )
	{
		if( curTime <= maxTime ) curTime += dt;
	}
	void Reset()
	{
		curTime = 0.0f;
	}
	void Reset( float newMaxTime )
	{
		Reset();
		maxTime = newMaxTime;
	}
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
private:
	float curTime;
	float maxTime;
};