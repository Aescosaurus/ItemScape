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
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
private:
	float curTime;
	const float maxTime;
};