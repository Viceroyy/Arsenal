#include "../SDK/SDK.h"

#include "../Features/ESP/ESP.h"
#include "../Features/Menu/Menu.h"
#include "../Features/Notification/Notification.h"
#include "../Features/PlayerArrows/PlayerArrows.h"
#include "../Features/Visuals/Visuals.h"
#include "../Features/SpectatorList/SpectatorList.h"

MAKE_HOOK(IEngineVGui_Paint, U::VFunc.Get<void*>(I::EngineVGui, 14u), void,
	void* rcx, int mode)
{
	/*if (G.Unload)
		return CALL_ORIGINAL(rcx, mode);*/

	CALL_ORIGINAL(rcx, mode);

	if (mode & PAINT_UIPANELS && (!CFG::Visuals_ClearScreenshots || !I::EngineClient->IsTakingScreenshot()))
	{
		H::Draw.UpdateW2SMatrix();

		I::MatSystemSurface->StartDrawing();
		{
			F::Notifications.Draw();
			F::Visuals.Run();
			F::ESP.Run();
			F::PlayerArrows.Run();
			F::SpectatorList.Run();
			F::Menu.Run();
		}
		I::MatSystemSurface->FinishDrawing();
	}
}