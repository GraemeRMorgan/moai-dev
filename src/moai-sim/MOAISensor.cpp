// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAISensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISensor::_getTimestamp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISensor, "U" )
	
	lua_pushnumber ( state, self->mTimestamp );
	
	return 1;
}

//================================================================//
// MOAISensor
//================================================================//

//----------------------------------------------------------------//
MOAISensor::MOAISensor () :
	mType ( 0 ),
	mTimestamp ( 0 ) {

	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAISensor, MOAILuaObject )
}

//----------------------------------------------------------------//
MOAISensor::~MOAISensor () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISensor::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "new",					MOAILuaObject::_alertNewIsUnsupported },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISensor::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getTimestamp",			_getTimestamp },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
