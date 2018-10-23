#pragma once

#include "Vec2.h"

template<typename T,typename U>
class Vec3_
	:
	public Vec2_<T>
{
public:
	Vec3_( T x,T y,U z )
		:
		Vec2_<T>{ x,y },
		z( z )
	{}
	operator Vec2_()
	{
		return( Vec2_<T>{ x,y } );
	}
public:
	U z;
};

typedef Vec3_<float,bool> Vec3b;