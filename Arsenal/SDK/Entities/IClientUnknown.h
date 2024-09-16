//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef ICLIENTUNKNOWN_H
#define ICLIENTUNKNOWN_H
#ifdef _WIN32
#pragma once
#endif

#include "NetVarManager/NetVarManager.h"
#include "../Globals/Globals.h"

class IClientNetworkable;
class C_BaseEntity;
class IClientRenderable;
class IClientEntity;
class IClientThinkable;

class ICollideable
{
public:
	virtual void* GetEntityHandle() = 0;
	virtual const Vector& OBBMinsPreScaled() const = 0;
	virtual const Vector& OBBMaxsPreScaled() const = 0;
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
	virtual void			WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const = 0;
	virtual bool			TestCollision(const Ray_t& ray, unsigned int fContentsMask, CGameTrace& tr) = 0;
	virtual bool			TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, CGameTrace& tr) = 0;
	virtual int				GetCollisionModelIndex() = 0;
	virtual const model_t* GetCollisionModel() = 0;
	virtual const Vector& GetCollisionOrigin() const = 0;
	virtual const Vector& GetCollisionAngles() const = 0;
	virtual const matrix3x4_t& CollisionToWorldTransform() const = 0;
	virtual SolidType_t		GetSolid() const = 0;
	virtual int				GetSolidFlags() const = 0;
	virtual void* GetIClientUnknown() = 0;
	virtual int				GetCollisionGroup() const = 0;
	virtual void			WorldSpaceSurroundingBounds(Vector* pVecMins, Vector* pVecMaxs) = 0;
	virtual bool			ShouldTouchTrigger(int triggerSolidFlags) const = 0;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable * GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};


#endif // ICLIENTUNKNOWN_H