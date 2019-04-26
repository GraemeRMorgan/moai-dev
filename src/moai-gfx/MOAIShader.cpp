// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAIShaderProgram.h>
#include <moai-gfx/MOAIShaderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_getAttributeID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UN" )

	ZLIndex uniformID		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex index			= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );
	
	if ( self->mProgram ) {
		state.Push ( self->mProgram->GetAttributeID ( uniformID, index ));
	}
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_setProgram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )
	
	self->SetProgram ( state.GetLuaObject < MOAIShaderProgram >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
int MOAIShader::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );

	MOAIShaderProgram* program = MOAICast < MOAIShaderProgram >( self->mProgram );

	if ( program ) {
		program->SetUniform ( L, 3, self->mPendingUniformBuffer.GetBuffer (), uniformID, ZLIndexOp::ZERO );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIShader::_setUniformArrayItem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );

	MOAIShaderProgram* program = MOAICast < MOAIShaderProgram >( self->mProgram );

	if ( program ) {
		program->SetUniform ( L, 4, self->mPendingUniformBuffer.GetBuffer (), uniformID, index );
	}
	return 0;
}

//================================================================//
// MOAIShader
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIShader::AffirmShader ( MOAILuaState& state, int idx ) {

	MOAIShader* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgr::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgr::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShader >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIShader::MOAIShader () {

	RTTI_BEGIN
		RTTI_EXTEND ( ZLShader )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShader::~MOAIShader () {

	this->SetProgram ( 0 );
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getAttributeID",				_getAttributeID },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ "setUniformArrayItem",		_setUniformArrayItem },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}
