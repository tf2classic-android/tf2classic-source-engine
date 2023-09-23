#include "cbase.h"
#include "tf_weapon_sixshooter.h"

#if defined( CLIENT_DLL )
#include "c_tf_player.h"
#else
#include "tf_player.h"
#endif

CREATE_SIMPLE_WEAPON_TABLE( TFSixShooter, tf_weapon_sixshooter )

CTFSixShooter::CTFSixShooter()
{
}

CTFSixShooter::~CTFSixShooter()
{
}
