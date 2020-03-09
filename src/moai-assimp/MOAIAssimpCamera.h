// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPCAMERA_H
#define MOAIASSIMPCAMERA_H

#include <moai-assimp/MOAIAssimpSceneMember.h>
#include <moai-sim/MOAICamera.h>

//================================================================//
// MOAIAssimpCamera
//================================================================//
// TODO: doxygen
class MOAIAssimpCamera :
	public MOAIAssimpSceneMember {
private:

	aiCamera*		mAssimpCamera;
	MOAICamera*		mMoaiCamera;
	aiVector3D		mScaling;
	aiVector3D		mPosition;
	aiQuaternion	mRotation;
	
	//----------------------------------------------------------------//
	static int		_getAspect					( lua_State* L );
	static int		_getFarClipPlane			( lua_State* L );
	static int		_getHorizontalFOV			( lua_State* L );
	static int		_getLocationVector			( lua_State* L );
	static int		_getLookAtVector			( lua_State* L );
	static int		_getMoaiCamera				( lua_State* L );
	static int		_getNearClipPlane			( lua_State* L );
	static int		_getPitchAngle				( lua_State* L );
	static int		_getPositionVector			( lua_State* L );
	static int		_getRollAngle				( lua_State* L );
	static int		_getRotationQuaternion		( lua_State* L );
	static int		_getScalingVector			( lua_State* L );
	static int		_getUpVector				( lua_State* L );
	static int		_getYawAngle				( lua_State* L );
	
	//----------------------------------------------------------------//
	float			CalculatePitch				( aiQuaternion quaternion );
	float			CalculateRoll				( aiQuaternion quaternion );
	float			CalculateYaw				( aiQuaternion quaternion );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIAssimpCamera )

	//----------------------------------------------------------------//
					MOAIAssimpCamera			();
					~MOAIAssimpCamera			();
	void			SetCamera					( aiCamera* assimpCamera );
};

#endif
