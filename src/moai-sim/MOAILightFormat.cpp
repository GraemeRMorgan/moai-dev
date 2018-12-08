// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILightFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILightFormat::_reserveTextures ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILightFormat, "U" )
	
	self->mTextures = state.GetValue < u32 >( 2, 0 );
	self->mDirty = true;
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILightFormat::_reserveUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILightFormat, "U" )
	
	self->mUniforms.Init ( state.GetValue < u32 >( 2, 0 ));
	self->mDirty = true;
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILightFormat::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILightFormat, "U" )
	
	ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	
	if ( idx < self->mUniforms.Size ()) {
	
		MOAILightFormatUniform& uniform = self->mUniforms [ idx ];
		uniform.mType	= state.GetValue < u32 >( 3, 0 );
		uniform.mWidth	= state.GetValue < u32 >( 4, 1 );
		self->mDirty = true;
	}
	return 0;
}

//================================================================//
// MOAILightFormat
//================================================================//

//----------------------------------------------------------------//
void MOAILightFormat::Bless () {

	if ( this->mDirty ) {
	
		ZLSize base = 0;
	
		ZLSize nUniforms = this->mUniforms.Size ();
		for ( ZLIndex i = ZLIndexOp::ZERO; i < nUniforms; ++i ) {
			
			MOAILightFormatUniform& uniform = this->mUniforms [ i ];
			uniform.mBase = base;
			base += uniform.GetSize ();
		}
		this->mBufferSize = base;
		this->mDirty = false;
	}
}

//----------------------------------------------------------------//
MOAILightFormatUniform* MOAILightFormat::GetUniform ( ZLIndex uniformID ) {

	return uniformID < this->mUniforms.Size () ? &this->mUniforms [ uniformID ] : 0;
}

//----------------------------------------------------------------//
MOAILightFormat::MOAILightFormat () :
	mTextures ( 0 ),
	mDirty ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILightFormat::~MOAILightFormat () {
}

//----------------------------------------------------------------//
void MOAILightFormat::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED(state);
}

//----------------------------------------------------------------//
void MOAILightFormat::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reserveTextures",			_reserveTextures },
		{ "reserveUniform",				_reserveUniform },
		{ "setUniform",					_setUniform },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
