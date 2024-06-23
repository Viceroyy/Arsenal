#include "../Hooks.h"

#include "../../Features/ESP/ESP.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Notification/Notification.h"
#include "../../Features/PlayerArrows/PlayerArrows.h"
#include "../../Features/Visual/Visual.h"

DEFINE_HOOK(IEngineVGui_Paint, void, __fastcall, void* ecx, void* edx, int mode)
{
	Func.Original<FN>()(ecx, edx, mode);

	if (mode & PAINT_UIPANELS)
	{
		H::Draw.UpdateMatrix();

		I::MatSystemSurface->StartDrawing();
		{
			if (CFG::Visual_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
				return I::MatSystemSurface->FinishDrawing();

			F::Notifications.Draw();
			F::Visual.Run();
			F::ESP.Run();
			F::PlayerArrows.Run();
			F::Menu.Run();
		}
		I::MatSystemSurface->FinishDrawing();
	}
}