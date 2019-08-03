// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderGL::_setProgram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "U" )
	
	self->SetProgram ( state.GetLuaObject < MOAIShaderProgramGL >( 2, true ));

	return 0;
}

//================================================================//
// MOAIShaderGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderGL::ApplyUniforms () {

	if ( this->mProgram ) {
		this->mProgram->ApplyUniforms ( this->mUniforms );
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::BindUniforms () {
	
	if ( this->mProgram ) {
		this->mProgram->BindUniforms ();
	}
}

//----------------------------------------------------------------//
bool MOAIShaderGL::HasDirtyUniforms () {

	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mUniforms );
}

//----------------------------------------------------------------//
MOAIShaderGL::MOAIShaderGL () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
		RTTI_EXTEND ( MOAIAbstractUniformBuffer )
		RTTI_EXTEND ( MOAIHasUniformComposer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderGL::~MOAIShaderGL () {

	this->SetProgram ( 0 );
}

//----------------------------------------------------------------//
void MOAIShaderGL::SetProgram ( MOAIShaderProgramGL* program ) {
	
	this->mProgram = program;
	
	if ( program ) {
		program->InitUniformBuffer ( this->mUniforms );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
const MOAIUniformSchema* MOAIShaderGL::MOAIAbstractUniformBuffer_GetSchema () const {

	return this->mProgram;
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIShader, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractUniformBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasUniformComposer, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIShader, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractUniformBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasUniformComposer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "setProgram",					_setProgram },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAIShader_ComposeUniforms () {

	if ( !this->mProgram ) return;
	MOAIUniformComposer* composer = this->GetComposer ();
	composer = composer ? composer : this->mProgram->GetComposer ();

	if ( composer ) {
		composer->ComposeUniforms ( *this->mProgram, *this );
	}
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAIShader_IsReady () const {

	return ( this->mProgram && this->mProgram->IsReady ());
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAIShader_SelectTextures () {

	if ( !this->mProgram ) return;
	MOAIUniformComposer* composer = this->GetComposer ();
	composer = composer ? composer : this->mProgram->GetComposer ();

	if ( composer ) {
		composer->SelectTextures ();
	}
}

