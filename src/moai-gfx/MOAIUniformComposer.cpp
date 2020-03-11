// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIUniformComposer.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIUniformComposerGlobalRef
//================================================================//

//----------------------------------------------------------------//
MOAIUniformComposerGlobalRef::MOAIUniformComposerGlobalRef () :
	mGlobalID ( ZLIndexOp::INVALID ),
	mUniformID ( ZLIndexOp::INVALID ),
	mIndex ( 0 ) {
}

//================================================================//
// MOAIUniformComposerTextureRef
//================================================================//

//----------------------------------------------------------------//
MOAIUniformComposerTextureRef::MOAIUniformComposerTextureRef () :
	mName ( MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL ),
	mUnit ( 0 ) {
}

//================================================================//
// MOAIUniformComposer
//================================================================//

//----------------------------------------------------------------//
void MOAIUniformComposer::ComposeUniforms ( const MOAIUniformSchema& schema, MOAIAbstractUniformBuffer& buffer ) const {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	ZLRect viewRect = gfxMgr.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	u32 nGlobals = this->mGlobals.Size ();

	for ( ZLIndex i = 0; i < nGlobals; ++i ) {
	
		const MOAIUniformComposerGlobalRef& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == ZLIndexOp::INVALID ) continue;
		
		MOAIUniformHandle uniform = schema.GetUniformHandle ( buffer.mUniforms.GetBuffer (), global.mUniformID, global.mIndex );
		if ( !uniform.IsValid ()) continue;
		
		if ( global.mGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {
		
			uniform.SetValue ( gfxMgr.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch (( ZLSize )global.mGlobalID ) {
				
				case MOAIGfxMgr::PEN_COLOR:
				
					uniform.SetValue ( gfxMgr.GetFinalColor ());
					break;
				
				case MOAIGfxMgr::VIEW_HALF_HEIGHT:
				
					uniform.SetValue ( viewRect.Height () * 0.5f );
					break;
					
				case MOAIGfxMgr::VIEW_HALF_WIDTH: {
				
					uniform.SetValue ( viewRect.Width () * 0.5f );
					break;
				}
				case MOAIGfxMgr::VIEW_HEIGHT:
				
					uniform.SetValue ( viewRect.Height ());
					break;
					
				case MOAIGfxMgr::VIEW_WIDTH:
				
					uniform.SetValue ( viewRect.Width ());
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
MOAIUniformComposer::MOAIUniformComposer () {

	RTTI_BEGIN ( MOAIUniformComposer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIUniformComposer >)
		RTTI_EXTEND ( MOAIUniformComposerInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIUniformComposer::~MOAIUniformComposer () {
}

//----------------------------------------------------------------//
void MOAIUniformComposer::SelectTextures () {

	// TODO: this needs to go away. texture (in GL) are always.
	// No reason (right now) to remap material manager texture names to
	// different texture units. Should be part of the material,
	// if it exists at all. Removing this, shaders cleanly break
	// with material manager.

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIMaterialMgr& materialMgr = MOAIMaterialMgr::Get ();

	size_t nTextures = this->mTextures.Size ();
	for ( ZLIndex i = 0; i < nTextures; ++i ) {

		MOAIUniformComposerTextureRef& textureRef = this->mTextures [ i ];

		MOAITexture* texture = textureRef.mTexture;

		// load texture by name
		if ( !texture ) {
			texture = materialMgr.GetTexture ( textureRef.mName );
		}
		gfxMgr.SetTexture ( texture, textureRef.mUnit );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIUniformComposer& MOAIUniformComposer::MOAIUniformComposerInterface_AffirmComposer () {

	return *this;
}

//----------------------------------------------------------------//
MOAIUniformComposer* MOAIUniformComposer::MOAIUniformComposerInterface_GetComposer () {

	return this;
}
