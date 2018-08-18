// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeckProxy::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckProxy, "U" )
	
	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	return 0;
}

//================================================================//
// MOAIDeckProxy
//================================================================//

//----------------------------------------------------------------//
MOAIDeckProxy::MOAIDeckProxy () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckProxy::~MOAIDeckProxy () {

	this->mDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_ComputeMaxBounds () {

	return this->mDeck ? this->mDeck->MOAIDeck_ComputeMaxBounds () : ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::MOAIDeck_Draw ( u32 idx ) {

	if ( this->mDeck ) {
		this->mDeck->MOAIDeck_Draw ( this->MOAIDeckProxy_Remap ( idx ));
	}
}

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_GetBounds ( u32 idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetBounds ( this->MOAIDeckProxy_Remap ( idx )) : ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeckProxy::MOAIDeck_GetCollisionShape ( u32 idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetCollisionShape ( this->MOAIDeckProxy_Remap ( idx )) : 0;
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::MOAIDeck_Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	return this->mDeck ? this->mDeck->MOAIDeck_Overlap ( this->MOAIDeckProxy_Remap ( idx ), vec, granularity, result ) : false;
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::MOAIDeck_Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	return this->mDeck ? this->mDeck->MOAIDeck_Overlap ( this->MOAIDeckProxy_Remap ( idx ), vec, granularity, result ) : false;
}

//----------------------------------------------------------------//
u32 MOAIDeckProxy::MOAIDeckProxy_Remap ( u32 idx ) {

	return idx;
}
