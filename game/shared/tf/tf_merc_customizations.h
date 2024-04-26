#if !defined( TF_MERC_CUSTOMIZATIONS_H )
#define TF_MERC_CUSTOMIZATIONS_H

#if defined( _WIN32 )
#pragma once
#endif

#include "igamesystem.h"

struct respawn_particle_t
{
	int id;
	char szEffect[128];
	char szName[128];
};

#if defined( CLIENT_DLL )
struct win_anim_t
{
	int id;
	char szSequence[128];
	char szName[128];
};
#endif

#define MERC_CUSTOM_FILE "scripts/deathmatch/mercenary_custom.txt"

class CTFMercCustomizations : public CAutoGameSystem
{
public:
	CTFMercCustomizations( const char *pszName );

	virtual bool Init();
	virtual void LevelInitPreEntity();

#if defined( CLIENT_DLL )
	respawn_particle_t *GetParticle( int index );
	win_anim_t *GetAnimationById( int id );
	win_anim_t *GetAnimation( int index );
	int GetWinAnimCount() { return m_WinAnimations.Size(); }
#else
	respawn_particle_t *GetParticleById( int id );
	respawn_particle_t *GetRandomParticle();
#endif
	int GetParticlesCount() { return m_Particles.Size(); } // shared code addition
private:
	CUtlVector< respawn_particle_t >	m_Particles;
#if defined( CLIENT_DLL )
	CUtlVector< win_anim_t >		m_WinAnimations;
#endif
};

extern CTFMercCustomizations g_TFMercCustomizations;

#endif
