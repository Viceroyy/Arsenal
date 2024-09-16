#include "PlayerArrows.h"

void CFeatures_PlayerArrows::DrawArrowTo(const Vector& vecFromPos, const Vector& vecToPos, Color_t color)
{
	const auto cx = static_cast<float>(H::Draw.m_nScreenW / 2);
	const auto cy = static_cast<float>(H::Draw.m_nScreenH / 2);

	bool onScreen; Vector vScreenPos = {};
	{
		const matrix3x4_t& worldToScreen = H::Draw.m_WorldToProjection.As3x4();
		float w = worldToScreen[3][0] * vecToPos[0] + worldToScreen[3][1] * vecToPos[1] + worldToScreen[3][2] * vecToPos[2] + worldToScreen[3][3];
		vScreenPos.z = 0;

		onScreen = w > 0.f;
		float fl1DBw = 1 / abs(w);
		vScreenPos.x = cx + (0.5 * ((worldToScreen[0][0] * vecToPos[0] + worldToScreen[0][1] * vecToPos[1] + worldToScreen[0][2] * vecToPos[2] + worldToScreen[0][3]) * fl1DBw) * cx + 0.5);
		vScreenPos.y = cy - (0.5 * ((worldToScreen[1][0] * vecToPos[0] + worldToScreen[1][1] * vecToPos[1] + worldToScreen[1][2] * vecToPos[2] + worldToScreen[1][3]) * fl1DBw) * cy + 0.5);
	}

	if (onScreen)
	{
		float minc = std::min(cx, cy); float maxc = std::max(cx, cy);
		float dist = sqrt(powf(vScreenPos.x - cx, 2) + powf(vScreenPos.y - cy, 2)) * 2;
		float transparency;
		if (minc != maxc)
			transparency = 1 - std::clamp((dist - minc) / (maxc - minc), 0.f, 1.f);
		else
			transparency = 1 - std::clamp(dist - minc, 0.f, 1.f);
		color.a = static_cast<byte>(std::max(static_cast<float>(color.a) - transparency * 255.f, 0.f));
	}

	if (color.a == 0)
		return;

	Vector2D
		p1 = { static_cast<float>(-CFG::Visuals_PlayerArrows_Offset), 12.5f },
		p2 = { static_cast<float>(-CFG::Visuals_PlayerArrows_Offset), -12.5f },
		p3 = { static_cast<float>(-CFG::Visuals_PlayerArrows_Offset) - 25.f * sqrt(3.f) / 2.f, 0.f };

	auto angle = Vector();
	U::Math.VectorAngles(Vector(cx - vScreenPos.x, cy - vScreenPos.y, 0), angle);
	const float deg = DEG2RAD(angle.y);
	const float _cos = cos(deg);
	const float _sin = sin(deg);

	Vertex_t t1, t2, t3;
	t1.Init({ cx + p1.x * _cos - p1.y * _sin, cy + p1.y * _cos + p1.x * _sin });
	t2.Init({ cx + p2.x * _cos - p2.y * _sin, cy + p2.y * _cos + p2.x * _sin });
	t3.Init({ cx + p3.x * _cos - p3.y * _sin, cy + p3.y * _cos + p3.x * _sin });
	std::array<Vertex_t, 3> verts{ t1, t2, t3 };
	H::Draw.Polygon(3, verts.data(), color);
}

void CFeatures_PlayerArrows::Run()
{
	if (!CFG::Visuals_PlayerArrows_Enabled || I::EngineVGui->IsGameUIVisible())
		return;

	auto pLocal = H::EntityCache.GetLocal();
	if (!pLocal || pLocal->deadflag())
		return;

	const Vector vLocalPos = pLocal->EyePosition();

	for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pPlayer->deadflag())
			continue;

		Vector vEnemyPos = pPlayer->GetCenter();

		Color_t color;
		if (!CFG::Colors_Relative)
		{
			if (pLocal->m_iTeamNum() == 2)
				color = CFG::Colors_TeamT;
			else
				color = CFG::Colors_TeamCT;
		}
		else
			color = CFG::Colors_Enemy;

		auto MapFloat = [&](float x, float in_min, float in_max, float out_min, float out_max) -> float
			{
				return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
			};
		const float fMap = std::clamp(MapFloat(vLocalPos.DistTo(vEnemyPos), CFG::Visuals_PlayerArrows_MaxDist, CFG::Visuals_PlayerArrows_MaxDist * 0.9f, 0.0f, 1.0f), 0.0f, 1.0f);
		color.a = static_cast<byte>(fMap * 255.f);

		DrawArrowTo(vLocalPos, vEnemyPos, color);
	}
}
