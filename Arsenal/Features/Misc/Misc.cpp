#include "Misc.h"
#include "../CFG.h"

void CFeatures_Misc::Run(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal || pLocal->deadflag() || pLocal->m_MoveType() != MOVETYPE_WALK || pLocal->IsSwimming())
		return;

	Bunnyhop(pLocal, cmd);
	AutoStrafe(pLocal, cmd);
}

void CFeatures_Misc::Bunnyhop(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!CFG::Misc_Bunnyhop)
		return;

	static bool bStaticJump = false, bStaticGrounded = false, bLastAttempted = false;
	const bool bLastJump = bStaticJump, bLastGrounded = bStaticGrounded;
	const bool bCurJump = bStaticJump = cmd->buttons & IN_JUMP, bCurGrounded = bStaticGrounded = pLocal->m_hGroundEntity();

	if (bCurJump && bLastJump && (bCurGrounded ? !pLocal->IsDucking() : true))
	{
		if (!(bCurGrounded && !bLastGrounded))
			cmd->buttons &= ~IN_JUMP;

		if (!(cmd->buttons & IN_JUMP) && bCurGrounded && !bLastAttempted)
			cmd->buttons |= IN_JUMP;
	}

	if (CFG::Misc_AntiCheatCompatibility)
	{	// prevent more than 9 bhops occurring. if a server has this under that threshold they're retarded anyways
		static int iJumps = 0;
		if (bCurGrounded)
		{
			if (!bLastGrounded && cmd->buttons & IN_JUMP)
				iJumps++;
			else
				iJumps = 0;

			if (iJumps > 9)
				cmd->buttons &= ~IN_JUMP;
		}
	}
	bLastAttempted = cmd->buttons & IN_JUMP;
}

void CFeatures_Misc::AutoStrafe(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!CFG::Misc_AutoStrafe || pLocal->m_hGroundEntity() || !(pLocal->m_afButtonLast() & IN_JUMP) && (cmd->buttons & IN_JUMP))
		return;

	switch (CFG::Misc_AutoStrafe)
	{
	case 1: //Legit
	{
		static auto cl_sidespeed = U::ConVars.FindVar("cl_sidespeed");
		const float flSideSpeed = cl_sidespeed->GetFloat();

		if (cmd->mousedx)
		{
			cmd->forwardmove = 0.f;
			cmd->sidemove = cmd->mousedx > 0 ? flSideSpeed : -flSideSpeed;
		}
		break;
	}
	case 2: //Directional
	{
		// credits: KGB
		if (!(cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
			break;

		float flForward = cmd->forwardmove, flSide = cmd->sidemove;

		Vector vForward, vRight; U::Math.AngleVectors(cmd->viewangles, &vForward, &vRight, nullptr);
		vForward.Normalize2D(), vRight.Normalize2D();

		Vector vWishDir = U::Math.VectorAngles({ vForward.x * flForward + vRight.x * flSide, vForward.y * flForward + vRight.y * flSide, 0.f });
		Vector vCurDir = U::Math.VectorAngles(pLocal->m_vecVelocity());
		float flDirDelta = U::Math.NormalizeAngle(vWishDir.y - vCurDir.y);
		if (fabsf(flDirDelta) > CFG::Misc_AutoStrafeMaxDelta)
			break;

		float flTurnScale = U::Math.RemapValClamped(CFG::Misc_AutoStrafeTurnScale, 0.f, 1.f, 0.9f, 1.f);
		float flRotation = DEG2RAD((flDirDelta > 0.f ? -90.f : 90.f) + flDirDelta * flTurnScale);
		float flCosRot = cosf(flRotation), flSinRot = sinf(flRotation);

		cmd->forwardmove = flCosRot * flForward - flSinRot * flSide;
		cmd->sidemove = flSinRot * flForward + flCosRot * flSide;
	}
	}
}