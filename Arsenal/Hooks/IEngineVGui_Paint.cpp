#include "../SDK/SDK.h"

#include "../Features/ESP/ESP.h"
#include "../Features/Menu/Menu.h"
#include "../Features/Notification/Notification.h"
#include "../Features/PlayerArrows/PlayerArrows.h"
#include "../Features/Visuals/Visuals.h"
#include "../Features/SpectatorList/SpectatorList.h"

MAKE_HOOK(IEngineVGui_Paint, U::VFunc.Get<void*>(I::EngineVGui, 13u), void, __fastcall, 
	void* ecx, void* edx, int mode)
{
	CALL_ORIGINAL(ecx, edx, mode);

	if (mode & PAINT_UIPANELS)
	{
		H::Draw.UpdateMatrix();

		I::MatSystemSurface->StartDrawing();
		{
			if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
				return I::MatSystemSurface->FinishDrawing();

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