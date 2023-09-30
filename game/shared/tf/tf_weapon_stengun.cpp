#include "cbase.h"
#include "tf_weapon_stengun.h"

#if defined( CLIENT_DLL )
#include "c_tf_player.h"
#else
#include "tf_player.h"
#endif

CREATE_SIMPLE_WEAPON_TABLE( TFStenGun, tf_weapon_stengun )

CTFStenGun::CTFStenGun()
{
}

CTFStenGun::~CTFStenGun()
{
}
