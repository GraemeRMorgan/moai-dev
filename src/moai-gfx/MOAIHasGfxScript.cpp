// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxScriptRetained.h>
#include <moai-gfx/MOAIHasGfxScript.h>

//================================================================//
// MOAIHasGfxScript
//================================================================//

//----------------------------------------------------------------//
MOAIHasGfxScript::MOAIHasGfxScript () {

	RTTI_BEGIN ( MOAIHasGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScript >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScript::~MOAIHasGfxScript () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIHasGfxScript::MOAIAbstractHasGfxScript_AffirmGfxScript () {

	MOAIGfxScriptRetained* gfxScript = this->mGfxScript ? MOAICast < MOAIGfxScriptRetained >( this->mGfxScript ) : NULL;
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScriptRetained ();
		this->mGfxScript = gfxScript;
	}
	return *gfxScript;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIHasGfxScript::MOAIAbstractHasGfxScript_GetGfxScript () {

	return this->mGfxScript;
}

//----------------------------------------------------------------//
void MOAIHasGfxScript::MOAIAbstractHasGfxScript_SetGfxScript ( MOAIAbstractGfxScript* gfxScript ) {

	this->mGfxScript = gfxScript;
}

