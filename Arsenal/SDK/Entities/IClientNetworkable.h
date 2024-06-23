//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef ICLIENTNETWORKABLE_H
#define ICLIENTNETWORKABLE_H
#ifdef _WIN32
#pragma once
#endif

#include "iclientunknown.h"

class IClientEntity;
class ClientClass;


enum ShouldTransmitState_t
{
	SHOULDTRANSMIT_START = 0,
	SHOULDTRANSMIT_END
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED
};

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;
	virtual void			NotifyShouldTransmit(ShouldTransmitState_t state) = 0;
	virtual void			OnPreDataChanged(DataUpdateType_t updateType) = 0;
	virtual void			OnDataChanged(DataUpdateType_t updateType) = 0;
	virtual void			PreDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void			PostDataUpdate(DataUpdateType_t updateType) = 0;
	virtual bool			IsDormant(void) = 0;
	virtual int				entindex(void) const = 0;
	virtual void			ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
	virtual void			OnDataUnchangedInPVS() = 0;

	ECSClientClass GetClassID()
	{
		if (auto pClientClass = GetClientClass())
			return static_cast<ECSClientClass>(pClientClass->m_ClassID);

		return static_cast<ECSClientClass>(0);
	}
};

#endif // ICLIENTNETWORKABLE_H