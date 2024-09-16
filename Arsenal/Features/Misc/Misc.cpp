#include "Misc.h"
#include "../CFG.h"

void CFeatures_Misc::Run(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal || pLocal->deadflag() || pLocal->m_MoveType() != MOVETYPE_WALK || pLocal->IsSwimming()) // TODO: fix swimming
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
	const bool bCurJump = bStaticJump = cmd->buttons & IN_JUMP, bCurGrounded = bStaticGrounded = pLocal->OnSolid();

	if (bCurJump && bLastJump)
	{
		if (!(bCurGrounded && !bLastGrounded))
			cmd->buttons &= ~IN_JUMP;

		if (!(cmd->buttons & IN_JUMP) && bCurGrounded && !bLastAttempted)
			cmd->buttons |= IN_JUMP;
	}

	bLastAttempted = cmd->buttons & IN_JUMP;
}

void CFeatures_Misc::AutoStrafe(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!CFG::Misc_AutoStrafe || pLocal->OnSolid() || !(pLocal->m_afButtonLast() & IN_JUMP) && (cmd->buttons & IN_JUMP))
		return;

	if (!(cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
		return;

	float flForwardMove = cmd->forwardmove;
	float flSideMove = cmd->sidemove;

	Vector vForward = {}, vRight = {};
	U::Math.AngleVectors(cmd->viewangles, &vForward, &vRight, nullptr);

	vForward.z = vRight.z = 0.f;

	vForward.Normalize();
	vRight.Normalize();

	Vector vWishDir = {};
	U::Math.VectorAngles({ (vForward.x * flForwardMove) + (vRight.x * flSideMove), (vForward.y * flForwardMove) + (vRight.y * flSideMove), 0.f }, vWishDir);

	Vector vCurDir = {};
	U::Math.VectorAngles(pLocal->m_vecVelocity(), vCurDir);

	float flDirDelta = U::Math.NormalizeAngle(vWishDir.y - vCurDir.y);
	float flTurnScale = U::Math.RemapValClamped(0.7f, 0.f, 1.f, 0.9f, 1.f);
	float flRotation = DEG2RAD((flDirDelta > 0.f ? -90.f : 90.f) + (flDirDelta * flTurnScale));

	float flCosRot = cosf(flRotation);
	float flSinRot = sinf(flRotation);

	cmd->forwardmove = (flCosRot * flForwardMove) - (flSinRot * flSideMove);
	cmd->sidemove = (flSinRot * flForwardMove) + (flCosRot * flSideMove);
}