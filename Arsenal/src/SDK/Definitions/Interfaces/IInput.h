#ifndef CINPUT_H
#define CINPUT_H
#ifdef _WIN32
#pragma once
#endif
#include "IEngineTrace.h"

class C_BaseCombatWeapon;

struct CameraThirdData_t
{
	float	m_flPitch;
	float	m_flYaw;
	float	m_flDist;
	float	m_flLag;
	Vector	m_vecHullMin;
	Vector	m_vecHullMax;
};

class bf_write;
class bf_read;
typedef unsigned int CRC32_t;

#define MULTIPLAYER_BACKUP 90

struct kbutton_t
{
	// key nums holding it down
	int		down[2];
	// low bit is down state
	int		state;
};

class CKeyboardKey
{
public:
	// Name for key
	char				name[32];
	// Pointer to the underlying structure
	kbutton_t* pkey;
	// Next key in key list.
	CKeyboardKey* next;
};

typedef struct
{
	unsigned int AxisFlags;
	unsigned int AxisMap;
	unsigned int ControlMap;
} joy_axis_t;

enum
{
	GAME_AXIS_NONE = 0,
	GAME_AXIS_FORWARD,
	GAME_AXIS_PITCH,
	GAME_AXIS_SIDE,
	GAME_AXIS_YAW,
	MAX_GAME_AXES
};

enum
{
	CAM_COMMAND_NONE = 0,
	CAM_COMMAND_TOTHIRDPERSON = 1,
	CAM_COMMAND_TOFIRSTPERSON = 2
};

enum
{
	MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
	MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
	MOUSE_SPEED_FACTOR,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

	NUM_MOUSE_PARAMS,
};

enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_R,
	JOY_AXIS_U,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

class CThirdPersonManager
{
public:
	CThirdPersonManager() {}
	virtual void	SetCameraOffsetAngles(const Vector& vecOffset) = 0;
	virtual const Vector& GetCameraOffsetAngles(void) const = 0;
	virtual void	SetDesiredCameraOffset(const Vector& vecOffset) = 0;
	virtual const Vector& GetDesiredCameraOffset(void) const = 0;
	virtual Vector	GetFinalCameraOffset(void) = 0;
	virtual void	SetCameraOrigin(const Vector& vecOffset) = 0;
	virtual const Vector& GetCameraOrigin(void) const = 0;
	virtual void	Update(void) = 0;
	virtual void	PositionCamera(C_BaseEntity* pPlayer, const Vector& angles) = 0;
	virtual void	UseCameraOffsets(bool bUse) = 0;
	virtual bool	UsingCameraOffsets(void) = 0;
	virtual const Vector& GetCameraViewAngles(void) const = 0;
	virtual Vector	GetDistanceFraction(void) = 0;
	virtual bool	WantToUseGameThirdPerson(void) = 0;
	virtual void	SetOverridingThirdPerson(bool bOverride) = 0;
	virtual bool	IsOverridingThirdPerson(void) = 0;
	virtual void	Init(void) = 0;
	virtual void	SetForcedThirdPerson(bool bForced) = 0;
	virtual bool	GetForcedThirdPerson() const = 0;
public:
	Vector		m_vecCameraOffset;
	Vector		m_vecDesiredCameraOffset;
	Vector m_vecCameraOrigin;
	bool	m_bUseCameraOffsets;
	Vector	m_ViewAngles;
	float	m_flFraction;
	float	m_flUpFraction;
	float	m_flTargetFraction;
	float	m_flTargetUpFraction;
	bool	m_bOverrideThirdPerson;
	bool	m_bForced;
	float	m_flUpOffset;
	float	m_flLerpTime;
	float	m_flUpLerpTime;
};

enum GameActionSetFlags_t
{
	GAME_ACTION_SET_FLAGS_NONE = 0,
	GAME_ACTION_SET_FLAGS_TAUNTING = (1 << 0)
};

class CInput
{
public:
	virtual	void Init_All(void) = 0;
	virtual void Shutdown_All(void) = 0;
	virtual int GetButtonBits(int) = 0;
	virtual void CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void ExtraMouseSample(float frametime, bool active) = 0;
	virtual bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void EncodeUserCmdToBuffer(bf_write& buf, int slot) = 0;
	virtual void DecodeUserCmdFromBuffer(bf_read& buf, int slot) = 0;
	virtual CUserCmd* GetUserCmd(int sequence_number) = 0;
	virtual void MakeWeaponSelection(void* weapon) = 0;
	virtual float KeyState(kbutton_t* key) = 0;
	virtual int KeyEvent(int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual kbutton_t* FindKey(const char* name) = 0;
	virtual void ControllerCommands(void) = 0;
	virtual void Joystick_Advanced(void) = 0;
	virtual void Joystick_SetSampleTime(float frametime) = 0;
	virtual void IN_SetSampleTime(float frametime) = 0;
	virtual void AccumulateMouse(void) = 0;
	virtual void ActivateMouse(void) = 0;
	virtual void DeactivateMouse(void) = 0;
	virtual void ClearStates(void) = 0;
	virtual float GetLookSpring(void) = 0;
	virtual void GetFullscreenMousePos(int* mx, int* my, int* unclampedx = 0, int* unclampedy = 0) = 0;
	virtual void SetFullscreenMousePos(int mx, int my) = 0;
	virtual void ResetMouse(void) = 0;
	virtual	float GetLastForwardMove(void) = 0;
	virtual	float Joystick_GetForward(void) = 0;
	virtual	float Joystick_GetSide(void) = 0;
	virtual	float Joystick_GetPitch(void) = 0;
	virtual	float Joystick_GetYaw(void) = 0;
	virtual void CAM_Think(void) = 0;
	virtual int CAM_IsThirdPerson(void) = 0;
	virtual void CAM_ToThirdPerson(void) = 0;
	virtual void CAM_ToFirstPerson(void) = 0;
	virtual void CAM_StartMouseMove(void) = 0;
	virtual void CAM_EndMouseMove(void) = 0;
	virtual void CAM_StartDistance(void) = 0;
	virtual void CAM_EndDistance(void) = 0;
	virtual int CAM_InterceptingMouse(void) = 0;
	virtual void CAM_ToOrthographic() = 0;
	virtual	bool CAM_IsOrthographic() const = 0;
	virtual	void CAM_OrthographicSize(float& w, float& h) const = 0;
	virtual void SetPreferredGameActionSet(GameActionSet_t action_set) = 0;
	virtual GameActionSet_t GetPreferredGameActionSet() = 0;
	virtual void SetGameActionSetFlags(GameActionSetFlags_t action_set_flags) = 0;
	virtual void LevelInit(void) = 0;
	virtual void ClearInputButton(int bits) = 0;
	virtual	void CAM_SetCameraThirdData(CameraThirdData_t* pCameraData, const Vector& vecCameraOffset) = 0;
	virtual void CAM_CameraThirdThink(void) = 0;
	virtual	bool EnableJoystickMode() = 0;
	virtual bool IsSteamControllerActive() = 0;

public:
	bool		m_fMouseInitialized;
	bool		m_fMouseActive;
	bool		m_fJoystickAdvancedInit;
	bool		m_fHadJoysticks;
	float		m_flAccumulatedMouseXMovement;
	float		m_flAccumulatedMouseYMovement;
	float		m_flPreviousMouseXPosition;
	float		m_flPreviousMouseYPosition;
	float		m_flRemainingJoystickSampleTime;
	float		m_flKeyboardSampleTime;
	bool		m_fRestoreSPI;
	int			m_rgOrigMouseParms[NUM_MOUSE_PARAMS];
	int			m_rgNewMouseParms[NUM_MOUSE_PARAMS];
	bool		m_rgCheckMouseParam[NUM_MOUSE_PARAMS];
	bool		m_fMouseParmsValid;
	joy_axis_t m_rgAxes[MAX_JOYSTICK_AXES];
	CKeyboardKey* m_pKeys;
	bool		m_fCameraInterceptingMouse;
	bool		m_fCameraInThirdPerson;
	bool		m_fCameraMovingWithMouse;
	bool		m_fCameraDistanceMove;
	int			m_nCameraOldX;
	int			m_nCameraOldY;
	int			m_nCameraX;
	int			m_nCameraY;
	bool		m_CameraIsOrthographic;
	Vector		m_angPreviousViewAngles;
	float		m_flLastForwardMove;
	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;
	CUserCmd* m_pCommands;
	CVerifiedUserCmd* m_pVerifiedCommands;
	CameraThirdData_t* m_pCameraThirdData;
	CHandle<C_BaseCombatWeapon>* m_hSelectedWeapon;
};

namespace I { inline CInput* Input = nullptr; }
#endif