// TF2C 2017
// SanyaSho (2024)

#if !defined( FUNC_RESTOCK_ZONE_H )
#define FUNC_RESTOCK_ZONE_H

class CRestockZone : public CRegenerateZone
{
	DECLARE_CLASS( CRestockZone, CRegenerateZone );
	DECLARE_DATADESC();
public:
	CRestockZone();

	void Regenerate( CTFPlayer *pPlayer );

private:
	bool m_bRestoreHealth;
	bool m_bRestoreAmmo;
};

#endif
