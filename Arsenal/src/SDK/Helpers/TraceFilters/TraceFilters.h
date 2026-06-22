#pragma once
#include "../../Utils/Macros/Macros.h"

struct CGameTrace;
class CTraceFilter;
class IHandleEntity;
class CBaseEntity;
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter {};

class CTraceFilterWorldAndPropsOnly : public CTraceFilter
{
public:
	CBaseEntity* pSkip = nullptr;
};

class CTraceFilterCollideable : public CTraceFilter
{
public:
	CBaseEntity* pSkip = nullptr;
	int iType = 0;
};

enum { SKIP_CHECK = 0 };
