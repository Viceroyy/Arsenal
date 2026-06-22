#include "NoSpread.h"
#include "../CFG.h"
#include <regex>
#include "../../Utils/Timer/Timer.h"
#include "../Notification/Notification.h"
/*
void CNoSpread::Reset()
{
	m_bWaitingForPlayerPerf = false;
	m_flServerTime = 0.f;
	m_vTimeDeltas.clear();
	m_dTimeDelta = 0.0;

	m_iSeed = 0;
	m_flMantissaStep = 0.f;

	m_bSynced = false;
}

bool CNoSpread::ShouldRun(C_CSPlayer* pLocal, C_WeaponCSBase* pWeapon, CUserCmd* cmd)
{
	if (!pLocal || !pWeapon || pLocal->deadflag() || !(cmd->buttons & IN_ATTACK))
		return false;

	if (!pWeapon->HasAnyAmmo())
		return false;

	switch (pWeapon->GetWeaponID())
	{
	case WEAPON_P228:
	case WEAPON_GLOCK:
	case WEAPON_SCOUT:
	case WEAPON_MAC10:
	case WEAPON_AUG:
	case WEAPON_FIVESEVEN:
	case WEAPON_UMP45:
	case WEAPON_SG550:
	case WEAPON_GALIL:
	case WEAPON_FAMAS:
	case WEAPON_USP:
	case WEAPON_AWP:
	case WEAPON_MP5NAVY:
	case WEAPON_M249:
	case WEAPON_M3:
	case WEAPON_M4A1:
	case WEAPON_TMP:
	case WEAPON_G3SG1:
	case WEAPON_DEAGLE:
	case WEAPON_SG552:
	case WEAPON_AK47:
	case WEAPON_P90:
		return true;
	default: break;
	}

	return false;
}


int CNoSpread::GetSeed(CUserCmd* pCmd)
{
	double dFloatTime = Util::PlatFloatTime() + m_dTimeDelta;
	float flTime = float(dFloatTime * 1000.0);
	return std::bit_cast<int32_t>(flTime) & 255;
}

float CNoSpread::CalcMantissaStep(float flV)
{
	// calculate the delta to the next representable value
	float flNextValue = std::nextafter(flV, std::numeric_limits<float>::infinity());
	float flMantissaStep = (flNextValue - flV) * 1000;

	// get the closest mantissa (next power of 2)
	return powf(2, ceilf(logf(flMantissaStep) / logf(2)));
}

std::string CNoSpread::GetFormat(int iServerTime)
{
	int iDays = iServerTime / 86400;
	int iHours = iServerTime / 3600 % 24;
	int iMinutes = iServerTime / 60 % 60;
	int iSeconds = iServerTime % 60;

	if (iDays)
		return std::format("{}d {}h", iDays, iHours);
	else if (iHours)
		return std::format("{}h {}m", iHours, iMinutes);
	else
		return std::format("{}m {}s", iMinutes, iSeconds);
}

void CNoSpread::AskForPlayerPerf()
{
	if (!CFG::Misc_NoSpread || !I::EngineClient->IsInGame())
		return Reset();

	static Timer tTimer = {};
	if (!m_bWaitingForPlayerPerf ? tTimer.Run(0.1f) : tTimer.Run(2.f))
	{
		I::ClientState->SendStringCmd("playerperf");
		m_bWaitingForPlayerPerf = true;
		m_dRequestTime = Util::PlatFloatTime();
	}
}

bool CNoSpread::ParsePlayerPerf(std::string sMsg)
{
	if (!CFG::Misc_NoSpread)
		return false;

	std::smatch match; std::regex_match(sMsg, match, std::regex(R"((\d+.\d+)\s\d+\s\d+\s\d+.\d+\s\d+.\d+\svel\s\d+.\d+)"));

	if (match.size() == 2)
	{
		m_bWaitingForPlayerPerf = false;

		// credits to kgb for idea
		float flNewServerTime = std::stof(match[1].str());
		if (flNewServerTime < m_flServerTime)
			return true;

		bool bLoopback = Util::IsLoopback();
		m_flServerTime = flNewServerTime;

		if (bLoopback)
			m_dTimeDelta = 0.f;
		else
		{
			m_vTimeDeltas.push_back(m_flServerTime - m_dRequestTime + TICKS_TO_TIME(1));
			while (!m_vTimeDeltas.empty() && m_vTimeDeltas.size() > 5)
				m_vTimeDeltas.pop_front();
			m_dTimeDelta = std::reduce(m_vTimeDeltas.begin(), m_vTimeDeltas.end()) / m_vTimeDeltas.size();
		}
		m_dTimeDelta += TICKS_TO_TIME(0.f);

		float flMantissaStep = CalcMantissaStep(m_flServerTime);
		m_bSynced = flMantissaStep >= 1.f || bLoopback;

		if (flMantissaStep > m_flMantissaStep && (m_bSynced || !m_flMantissaStep))
		{
			F::Notifications.Add(std::format("[Seed Prediction] {}", m_bSynced ? std::format("Synced ({})", m_dTimeDelta) : "Not synced, step too low"), CFG::Menu_Accent_Primary);
			F::Notifications.Add(std::format("[Seed Prediction] {}", std::format("Age {}; Step {}", GetFormat(m_flServerTime), flMantissaStep)), CFG::Menu_Accent_Primary);
		}
		m_flMantissaStep = flMantissaStep;

		return true;
	}

	return std::regex_match(sMsg, std::regex(R"(\d+.\d+\s\d+\s\d+)"));
}

void CNoSpread::Run(C_CSPlayer* pLocal, C_WeaponCSBase* pWeapon, CUserCmd* pCmd)
{
	if (!ShouldRun(pLocal, pWeapon))
		return;

	m_iSeed = GetSeed(pCmd);
	if (!m_bSynced)
		return;

	// credits to cathook for average spread stuff
	float flSpread = pWeapon->GetSpread();
	int iBulletsPerShot = 1;
	float flFireRate = std::ceilf(pWeapon->GetFireRate() / TICK_INTERVAL) * TICK_INTERVAL;

	std::vector<Vector> vBulletCorrections = {};
	Vector vAverageSpread = {};
	for (int iBullet = 0; iBullet < iBulletsPerShot; iBullet++)
	{
		Util::RandomSeed(m_iSeed + iBullet);

		if (!iBullet) // check if we'll get a guaranteed perfect shot
		{
			// if we are doubletapping and firerate is fast enough, prioritize later bullets
			int iTicks = F::Ticks.GetTicks();
			if (!iTicks || iTicks < TIME_TO_TICKS(flFireRate) * 2)
			{
				float flTimeSinceLastShot = I::GlobalVars->curtime - pWeapon->m_flLastFireTime();
				if (flTimeSinceLastShot > (iBulletsPerShot > 1 ? 0.25f : 1.25f))
					return;
			}
		}

		const float x = Util::RandomFloat(-0.5f, 0.5f) + Util::RandomFloat(-0.5f, 0.5f);
		const float y = Util::RandomFloat(-0.5f, 0.5f) + Util::RandomFloat(-0.5f, 0.5f);

		Vector vForward, vRight, vUp; U::Math.AngleVectors(pCmd->viewangles, &vForward, &vRight, &vUp);
		Vector vFixedSpread = vForward + (vRight * x * flSpread) + (vUp * y * flSpread);
		vFixedSpread.Normalize();
		vAverageSpread += vFixedSpread;

		vBulletCorrections.push_back(vFixedSpread);
	}
	vAverageSpread /= static_cast<float>(iBulletsPerShot);

	const auto cFixedSpread = std::ranges::min_element(vBulletCorrections,
		[&](const Vector& lhs, const Vector& rhs)
		{
			return lhs.DistTo(vAverageSpread) < rhs.DistTo(vAverageSpread);
		});

	if (cFixedSpread == vBulletCorrections.end())
		return;

	Vector vFixedAngles = U::Math.VectorAngles(*cFixedSpread);

	pCmd->viewangles += pCmd->viewangles - vFixedAngles;
	U::Math.ClampAngles(pCmd->viewangles);

	G::SilentAngles = true;
}

void CNoSpread::Draw(C_CSPlayer* pLocal)
{
	if (!(Vars::Menu::Indicators.Value & Vars::Menu::IndicatorsEnum::SeedPrediction) || !CFG::Misc_NoSpread || !pLocal->IsAlive())
		return;

	auto pWeapon = H::Entities.GetWeapon();
	if (!pWeapon || !ShouldRun(pLocal, pWeapon))
		return;

	int x = Vars::Menu::SeedPredictionDisplay.Value.x;
	int y = Vars::Menu::SeedPredictionDisplay.Value.y + 8;
	const auto& fFont = H::Fonts.GetFont(FONT_INDICATORS);
	const int nTall = fFont.m_nTall + H::Draw.Scale(1);

	EAlign align = ALIGN_TOP;
	if (x <= 100 + H::Draw.Scale(50, Scale_Round))
	{
		x -= H::Draw.Scale(42, Scale_Round);
		align = ALIGN_TOPLEFT;
	}
	else if (x >= H::Draw.m_nScreenW - 100 - H::Draw.Scale(50, Scale_Round))
	{
		x += H::Draw.Scale(42, Scale_Round);
		align = ALIGN_TOPRIGHT;
	}

	const auto& cColor = m_bSynced ? Vars::Menu::Theme::Active.Value : Vars::Menu::Theme::Inactive.Value;

	H::Draw.StringOutlined(fFont, x, y, cColor, Vars::Menu::Theme::Background.Value, align, std::format("Uptime {}", GetFormat(m_flServerTime)).c_str());
	H::Draw.StringOutlined(fFont, x, y += nTall, cColor, Vars::Menu::Theme::Background.Value, align, std::format("Mantissa step {}", m_flMantissaStep).c_str());
	if (Vars::Debug::Info.Value)
		H::Draw.StringOutlined(fFont, x, y += nTall, cColor, Vars::Menu::Theme::Background.Value, align, std::format("Delta {:.6f}", m_dTimeDelta).c_str());
}*/