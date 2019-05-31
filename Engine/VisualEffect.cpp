#include "VisualEffect.h"

const Surface VisualEffect::sprSheet = "Images/VisualEffects.bmp";

VisualEffect::VisualEffect( const Vei2& pos,Type t )
	:
	pos( pos - size / 2 ),
	anim( 0,int( t ) * size.y,size.x,size.y,4,sprSheet,0.2f )
{}

void VisualEffect::Update( float dt )
{
	anim.Update( dt );

	if( anim.IsFinished() ) expl = true;
}

void VisualEffect::Draw( Graphics& gfx ) const
{
	anim.Draw( pos,gfx );
}

bool VisualEffect::IsExpl() const
{
	return( expl );
}
