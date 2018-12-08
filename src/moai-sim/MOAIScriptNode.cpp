// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIScriptNode.h>

//================================================================//
// MOAIMemberTableAttr
//================================================================//
class MOAIMemberTableAttr {
protected:
	friend class MOAIScriptNode;
	MOAIScriptNode*		mSource;
	cc8*				mFieldName;
	
	//----------------------------------------------------------------//
	MOAIMemberTableAttr () :
		mSource ( 0 ),
		mFieldName ( 0 ) {
		}
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveAttrs
	@text	Reserve memory for custom attributes and initializes them to 0.
	
	@in		MOAIScriptNode self
	@in		number nAttributes
	@out	nil
*/
int MOAIScriptNode::_reserveAttrs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "UN" );

	ZLSize size = state.GetValue < MOAILuaState::SizeType >( 2, 0 );
	self->mAttributes.Init ( size );
	self->mAttributes.Fill ( 0.0f );
	
	self->mAttrNames.Init ( size );
	self->mAttrNames.Fill ( 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets a Lua function to be called whenever the node is updated.
	
	@in		MOAIScriptNode self
	@in		function onUpdate
	@out	nil
*/
int MOAIScriptNode::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "UF" );

	self->mOnUpdate.SetRef ( *self, state, 2 );
	return 0;
}


int MOAIScriptNode::_setAttrName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "U" );
	
	ZLIndex idx = state.GetValueAsIndex ( 2 );
	self->mAttrNames [ idx ] = state.GetValue < cc8* >( 3, 0 );
	
	return 0;
}

//================================================================//
// MOAIScriptNode
//================================================================//

//----------------------------------------------------------------//
MOAIScriptNode::MOAIScriptNode () {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIScriptNode::~MOAIScriptNode () {
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrAdd ( ZLIndex attrID, MOAIAttribute &attr ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	switch ( attr.GetTypeID ()) {
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			float value = attr.GetValue ( 0.0f );
			
			if ( value != 0.0f ) {
				
				MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
				this->PushMemberTable ( state );
				float cur = state.GetFieldValue < float >( -1, attrName, 0.0f );
				state.SetField ( -1, attrName, cur + value );
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			int value = ( int )attr.GetValue ( 0 );
			
			if ( value != 0 ) {
				MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
				this->PushMemberTable ( state );
				int cur = state.GetFieldValue < int >( -1, attrName, 0 );
				state.SetField ( -1, attrName, cur + value );
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrGet ( ZLIndex attrID, MOAIAttribute &attr ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushMemberTable ( state );
	state.PushField ( -1, attrName );
	
	if ( state.IsType ( -1, LUA_TNUMBER )) {
		
		float value = state.GetValue < float >( -1, 0.0f );
		attr.Apply ( value, MOAIAttribute::GET, MOAIAttribute::ATTR_WRITE );
	}
	else {
		
		MOAIMemberTableAttr value;
		value.mSource = this;
		value.mFieldName = attrName;
		attr.ApplyVariantNoAdd ( value, MOAIAttribute::GET, MOAIAttribute::ATTR_WRITE );
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrSet ( ZLIndex attrID, MOAIAttribute &attr ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	switch ( attr.GetTypeID ()) {
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->PushMemberTable ( state );
			state.SetField ( -1, attrName, value );
			
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			
			int value = attr.GetValue ( 0 );
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->PushMemberTable ( state );
			state.SetField ( -1, attrName, value );
			
			break;
		}
		case MOAIAttribute::ATTR_TYPE_VARIANT: {
			
			MOAIMemberTableAttr value;
			value = attr.GetVariant < MOAIMemberTableAttr >( value );
			MOAIScriptNode* source = value.mSource;
			cc8* sourceField = value.mFieldName;
			
			if ( source && sourceField ) {
			
				MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
				source->PushMemberTable ( state );
				this->PushMemberTable ( state );
				state.PushField ( -2, sourceField );
				
				lua_setfield ( state, -2, attrName );
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveAttrs",			_reserveAttrs },
		{ "setAttrName",			_setAttrName },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIScriptNode::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {
	
	ZLIndex attrIndex = ZLIndex ( UNPACK_ATTR ( attrID ) - 1, ZLIndex::LIMIT );
	
	if ( attrID >= this->mAttributes.Size()) {
		return false;
	}
	
	if ( this->mAttrNames [ attrIndex ] == 0 ) {
		this->mAttributes [ attrIndex ] = attr.Apply ( this->mAttributes [ attrIndex ], op, MOAIAttribute::ATTR_READ_WRITE );
		return true;
	}
	else {
		switch ( op ) {
			case MOAIAttribute::CHECK:
				attr.SetFlags ( MOAIAttribute::ATTR_READ_WRITE );
				break;
				
			case MOAIAttribute::ADD:
				this->NamedAttrAdd ( attrIndex, attr );
				break;
				
			case MOAIAttribute::SET:
				this->NamedAttrSet ( attrIndex, attr );
				break;
				
			case MOAIAttribute::GET:
				this->NamedAttrGet ( attrIndex, attr );
				break;
		}
		return true;
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::MOAINode_Update () {

	if ( this->mOnUpdate ) {
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnUpdate.PushRef ( state )) {
			this->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
	}
}
