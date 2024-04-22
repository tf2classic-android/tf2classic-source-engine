#include "cbase.h"

#include <tier1/fmtstr.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/RichText.h>
#include <vgui_controls/Frame.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/SectionedListPanel.h>
#include <vgui_controls/ImageList.h>
#include <game/client/iviewport.h>
#include <KeyValues.h>
#include <filesystem.h>

#include "tf_fourteamscoreboard.h"

using namespace vgui;

// lmao
CTFFourTeamScoreBoardDialog::CTFFourTeamScoreBoardDialog( IViewPort *pViewPort, const char *pszName ) : CTFClientScoreBoardDialog( pViewPort, pszName, 6 )
{

}
