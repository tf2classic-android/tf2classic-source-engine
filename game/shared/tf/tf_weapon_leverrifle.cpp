#include "cbase.h"
#include "tf_weapon_leverrifle.h"

// Client specific.
#ifdef CLIENT_DLL
#include "c_tf_player.h"
#else
#include "tf_player.h"
#endif

CREATE_SIMPLE_WEAPON_TABLE( TFLeverRifle, tf_weapon_leverrifle )

CTFLeverRifle::CTFLeverRifle()
{
}

CTFLeverRifle::~CTFLeverRifle()
{
}

void CTFLeverRifle::MakeTracer( const Vector &vecTracerSrc, const trace_t &tr )
{
	if( (((tr.endpos.x - vecTracerSrc.x) * (tr.endpos.x - vecTracerSrc.x))
	+ ((tr.endpos.y - vecTracerSrc.y) * (tr.endpos.y - vecTracerSrc.y))
	+ ((tr.endpos.z - vecTracerSrc.z) * (tr.endpos.z - vecTracerSrc.z))) >= 2304.f )
		BaseClass::MakeTracer( vecTracerSrc, tr );
}
