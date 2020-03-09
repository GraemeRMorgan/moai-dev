// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAITileFlags.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	The total number of indices to remap. Index remaps will be
			initialized from 1 to N.
	
	@in		MOAIDeckRemapper self
	@opt	number size		Default value is 0.
	@out	nil
*/
int MOAIDeckRemapper::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )

	ZLSize size = state.GetValue < u32 >( 2, 0 );
	self->mRemap.Init ( size );
	
	for ( ZLIndex i = 0; i < size; ++i ) {
		self->mRemap [ i ] = i;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBase
	@text	Set the base offset for the range of indices to remap.
			Used when remapping only a portion of the indices in
			the original deck.
	
	@in		MOAIDeckRemapper self
	@opt	number base		Default value is 0.
	@out	nil
*/
int MOAIDeckRemapper::_setBase ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )
	
	self->mBase = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRemap
	@text	Remap a single index to a new value.
	
	@in		MOAIDeckRemapper self
	@in		number index		Index to remap.
	@opt	number remap		New value for index. Default value is index (i.e. remove the remap).
	@out	nil
*/
int MOAIDeckRemapper::_setRemap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "UN" )

	ZLIndex idx		= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex remap	= state.GetValue < MOAILuaIndex >( 3, 0 );
	
	ZLIndex code = idx - self->mBase;
	
	if ( code < self->mRemap.Size ()) {
		self->mRemap [ code ] = remap;
	}
	return 0;
}

//================================================================//
// MOAIDeckRemapper
//================================================================//

//----------------------------------------------------------------//
MOAIDeckRemapper::MOAIDeckRemapper () :
	mBase ( 0 ) {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIDeckRemapper )
		RTTI_EXTEND ( MOAINode )
		RTTI_EXTEND ( MOAIDeckProxy )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckRemapper::~MOAIDeckRemapper () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLIndex MOAIDeckRemapper::MOAIDeckProxy_Remap ( ZLIndex idx ) {

	ZLIndex code = idx - this->mBase;
	
	return ( code < this->mRemap.Size () ? this->mRemap [ code ] : idx );
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckProxy, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckProxy, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setBase",			_setBase },
		{ "setRemap",			_setRemap },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIDeckRemapper::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	ZLIndex code = ( ZLSize )attrID.Unpack () - this->mBase - 1; // TODO: verify

	// TODO: verify
	if ( code < this->mRemap.Size ()) {
		ZLSize remap = this->mRemap [ code ];
		ZLIndex idx = ZLFloat::ToIndex ( attr.Apply (( float )remap, op, ZLAttribute::ATTR_READ_WRITE )) - 1;
		this->mRemap [ code ] = idx;
		return true;
	}
	return false;
}
