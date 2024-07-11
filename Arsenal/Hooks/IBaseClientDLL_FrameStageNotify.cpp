#include "../SDK/SDK.h"
#include "../../SDK/Input/Input.h"

MAKE_HOOK(IBaseClientDLL_FrameStageNotify, U::VFunc.Get<void*>(I::BaseClientDLL, 35u), void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CALL_ORIGINAL(ecx, edx, curStage);

	switch (curStage)
	{
		case ClientFrameStage_t::FRAME_NET_UPDATE_START:
		{
			H::EntityCache.Clear();
			break;
		}
		case ClientFrameStage_t::FRAME_NET_UPDATE_END:
		{
			H::EntityCache.Fill();
			break;
		}
		case ClientFrameStage_t::FRAME_RENDER_START:
		{
			H::Input.Update();
			break;
		}
	}
}