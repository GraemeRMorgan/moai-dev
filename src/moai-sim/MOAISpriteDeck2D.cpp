// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractChildTransform.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISpriteDeck2D.h>

//================================================================//
// MOAISpriteDeck2DCallable
//================================================================//

//----------------------------------------------------------------//
void MOAISpriteDeck2DCallable::MOAIAbstractGfxComposerCallable_Call () {

	this->mBrush.Draw ();
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getQuad
	@text	Get model space quad given a deck index. 
			Vertex order is clockwiese from upper left.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number x0
	@out	number y0
	@out	number x1
	@out	number y1
	@out	number x2
	@out	number y2
	@out	number x3
	@out	number y3
*/
int MOAISpriteDeck2D::_getQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )
	
	ZLIndex quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	
	return quadID < self->mQuads.Size () ? state.Push ( self->mQuads [ quadID ]) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAISpriteDeck2D::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )
	
	ZLIndex rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		rectID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	
	return rectID < self->mQuads.Size () ? state.Push ( self->mQuads [ rectID ].GetFrame ()) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVQuad
	@text	Get UV model space quad given a valid deck index.
			Vertex order is clockwise from upper left.

	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number u0
	@out	number v0
	@out	number u1
	@out	number v1
	@out	number u2
	@out	number v2
	@out	number u3
	@out	number v3
*/
int MOAISpriteDeck2D::_getUVQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )
	
	ZLIndex quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	
	return quadID < self->mQuads.Size () ? state.Push ( self->mUVQuads [ quadID ]) : 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVRect
	@text	Get UV model space quad given a valid deck index.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAISpriteDeck2D::_getUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )
	
	ZLIndex rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		rectID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	
	return rectID < self->mUVQuads.Size () ? state.Push ( self->mUVQuads [ rectID ].GetFrame ()) : 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveQuads
	@text	Reserve quads.
	
	@in		MOAISpriteDeck2D self
	@in		number nQuads
	@out	nil
*/
int MOAISpriteDeck2D::_reserveQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveSprites
	@text	Reserve sprites.
	
	@in		MOAISpriteDeck2D self
	@in		number nPairs
	@out	nil
*/
int MOAISpriteDeck2D::_reserveSprites ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReservePairs ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveSpriteLists
	@text	Reserve quad lists.
	
	@in		MOAISpriteDeck2D self
	@in		number nLists
	@out	nil
*/
int MOAISpriteDeck2D::_reserveSpriteLists ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveLists ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUVQuads
	@text	Reserve UV quads.
	
	@in		MOAISpriteDeck2D self
	@in		number nUVQuads
	@out	nil
*/
int MOAISpriteDeck2D::_reserveUVQuads ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveUVQuads ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setQuad
	@text	Set model space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAISpriteDeck2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	ZLIndex quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	ZLQuad quad = state.GetQuad ( idx );

	self->SetQuad ( quadID, quad );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAISpriteDeck2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	ZLIndex rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 6 ) {
		rectID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	ZLRect rect = state.GetRect < float >( idx );

	self->SetRect ( rectID, rect );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSprite
	@text	Associates a quad with its UV coordinates.
	
	@in		MOAISpriteDeck2D self
	@in		number idx
	@in		number uvQuadID
	@in		number quadID
	@opt	number materialID
	@out	nil
*/
int MOAISpriteDeck2D::_setSprite ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UNNN" )

	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex uvQuadID	= state.GetValue < MOAILuaIndex >( 3, 0 );
	ZLIndex quadID		= state.GetValue < MOAILuaIndex >( 4, 0 );
	ZLIndex materialID	= state.GetValue < MOAILuaIndex >( 5, 0 );
	
	self->SetPair ( idx, uvQuadID, quadID, materialID );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSpriteList
	@text	Initializes quad pair list at index. A list starts at the index
			of a pair and then continues sequentially for n pairs after. So
			a list with base 3 and a run of 4 would display pair 3, 4, 5,
			and 6.
	
	@in		MOAISpriteDeck2D self
	@in		number idx
	@in		number basePairID	The base pair of the list.
	@in		number totalPairs	The run of the list - total pairs to display (including base).
	@out	nil
*/
int MOAISpriteDeck2D::_setSpriteList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UNNN" )

	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex basePairID	= state.GetValue < MOAILuaIndex >( 3, 0 );
	ZLSize totalPairs	= state.GetValue < MOAILuaSize >( 4, 0 );

	self->SetList ( idx, basePairID, totalPairs );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVQuad
	@text	Set UV space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAISpriteDeck2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	ZLIndex quadID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 10 ) {
		quadID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	ZLQuad quad = state.GetQuad ( idx );

	self->SetUVQuad ( quadID, quad );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVRect
	@text	Set UV space quad given a valid deck index and a rect.
	
	@in		MOAISpriteDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAISpriteDeck2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "U" )

	ZLIndex rectID = 0;
	u32 idx = 2;
	
	if ( state.GetTop () == 6 ) {
		rectID = state.GetValue < MOAILuaIndex >( idx++, 0 );
	}
	ZLRect rect = state.GetRect < float >( idx );

	self->SetUVRect ( rectID, rect );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAISpriteDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAISpriteDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UU" )
	
	MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->Transform ( transform->GetLocalToWorldMtx ());
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transformUV
	@text	Apply the given MOAITransform to all the uv coordinates in the deck.
	
	@in		MOAISpriteDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAISpriteDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISpriteDeck2D, "UU" )
	
	MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAISpriteDeck2D
//================================================================//

//----------------------------------------------------------------//
MOAIDeck* MOAISpriteDeck2D::AffirmDeck ( MOAILuaState& state, int idx ) {

	MOAIDeck* deck = state.GetLuaObject < MOAIDeck >( idx, false );
	if ( deck ) return deck;
	
	MOAITexture* texture = MOAIGfxMgr::Get ().AffirmTexture ( state, idx );
		
	if ( texture ) {
		
		MOAISpriteDeck2D* spriteDeck = new MOAISpriteDeck2D ();
		assert ( spriteDeck );

		spriteDeck->PushTextureCmd (
			MOAIGfxComposerAddrModeEnum::FROM_OBJECT,
			MOAIGfxComposerAddrModeEnum::TO_GFX_STATE,
			texture,
			0
		);
		
		spriteDeck->PushShaderCmd (
			MOAIGfxComposerAddrModeEnum::FROM_OBJECT,
			MOAIGfxComposerAddrModeEnum::TO_GFX_STATE,
			MOAIGfxMgr::Get ().GetShaderPreset ( MOAIShaderPresetEnum::DECK2D_SHADER )
		);
		
		spriteDeck->PushCallCmd ();
		
		int hWidth = ( int )( texture->GetWidth () / 2 );
		int hHeight = ( int )( texture->GetHeight () / 2 );
		
		ZLRect rect;
		rect.Init (( float )-hWidth, ( float )-hHeight, ( float )hWidth, ( float )hHeight );
		spriteDeck->SetRect ( 0, rect );
		
		return spriteDeck;
	}
	return 0;
}

//----------------------------------------------------------------//
//bool MOAISpriteDeck2D::Contains ( u32 idx, const ZLVec2D& vec ) {
//	
//	size_t size = this->mSpriteLists.Size ();
//	if ( size ) {
//		
//		idx = ( idx - 1 ) % size;
//		MOAISpriteList& brush = this->mSpriteLists [ idx ];
//		
//		for ( size_t i = 0; i < brush.mTotalSprites; ++i ) {
//			MOAISprite& prim = this->mSprites [ brush.mBaseSprite + i ];
//			if ( this->mQuads [ prim.mQuadID ].Contains ( vec.mX, vec.mY )) {
//				return true;
//			} 
//		}
//	}
//	return false;
//}

//----------------------------------------------------------------//
//bool MOAISpriteDeck2D::Inside ( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad ) {
//	UNUSED ( pad );
//
//	u32 size = ( u32 )this->mSpriteLists.Size (); // TODO: cast
//	if ( size ) {
//	
//		idx = ( idx - 1 ) % size;
//		
//		MOAISpriteList& sprite = this->mSpriteLists [ idx ];
//		
//		for ( u32 i	 = 0; i < sprite.mTotalSprites; ++i ) {
//			MOAISprite& prim = this->mSprites [ sprite.mBaseSprite + i ];
//			if ( this->TestHit ( materials, prim.mMaterialID, idx, granularity, this->mQuads [ prim.mQuadID ], this->mUVQuads [ prim.mUVQuadID ], vec.mX, vec.mY )) return true;
//		}
//	}
//	return false;
//}

//----------------------------------------------------------------//
MOAISpriteDeck2D::MOAISpriteDeck2D () {
	
	RTTI_BEGIN ( MOAISpriteDeck2D )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAISpriteDeck2D >)
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIHasGfxComposer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpriteDeck2D::~MOAISpriteDeck2D () {
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveLists ( ZLSize total ) {

	this->mSpriteLists.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReservePairs ( ZLSize total ) {

	this->mSprites.Init ( total );
	
	MOAISprite zero;
	zero.mQuadID		= 0;
	zero.mUVQuadID		= 0;
	
	this->mSprites.Fill ( zero );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveQuads ( ZLSize total ) {

	this->mQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::ReserveUVQuads ( ZLSize total ) {

	this->mUVQuads.Init ( total );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetList ( ZLIndex idx, ZLIndex basePairID, ZLSize totalPairs ) {
	
	this->mSpriteLists.Grow ( idx + ( ZLSize )1 );
	MOAISpriteList& sprite = this->mSpriteLists [ idx ];
	
	sprite.mBaseSprite = basePairID;
	sprite.mTotalSprites = totalPairs;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetPair ( ZLIndex idx, ZLIndex uvQuadID, ZLIndex quadID, ZLIndex materialID ) {
	
	this->mSprites.Grow ( idx + ( ZLSize )1 );
	MOAISprite& spritePair = this->mSprites [ idx ];
	
	spritePair.mUVQuadID = uvQuadID;
	spritePair.mQuadID = quadID;
	spritePair.mMaterialID = materialID;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetQuad ( ZLIndex idx, ZLQuad& quad ) {

	this->mQuads.Grow ( idx + ( ZLSize )1 );
	this->mQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetRect ( ZLIndex idx, ZLRect& rect ) {

	this->mQuads.Grow ( idx + ( ZLSize )1 );
	this->mQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetUVQuad ( ZLIndex idx, ZLQuad& quad ) {

	this->mUVQuads.Grow ( idx + ( ZLSize )1 );
	this->mUVQuads [ idx ] = quad;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::SetUVRect ( ZLIndex idx, ZLRect& rect ) {

	this->mUVQuads.Grow ( idx + ( ZLSize )1 );
	this->mUVQuads [ idx ].Init ( rect );
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::Transform ( const ZLAffine3D& mtx ) {

	ZLSize total = this->mQuads.Size ();
	for ( ZLIndex i = 0; i < total; ++i ) {
		this->mQuads [ i ].Transform ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::TransformUV ( const ZLAffine3D& mtx ) {

	ZLSize total = this->mQuads.Size ();
	for ( ZLIndex i = 0; i < total; ++i ) {
		this->mUVQuads [ i ].Transform ( mtx );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISpriteDeck2D::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getQuad",				_getQuad },
		{ "getRect",				_getRect },
		{ "getUVQuad",				_getUVQuad },
		{ "getUVRect",				_getUVRect },
		{ "reserveQuads",			_reserveQuads },
		{ "reserveSprites",			_reserveSprites },
		{ "reserveSpriteLists",		_reserveSpriteLists },
		{ "reserveUVQuads",			_reserveUVQuads },
		{ "setQuad",				_setQuad },
		{ "setRect",				_setRect },
		{ "setSprite",				_setSprite },
		{ "setSpriteList",			_setSpriteList },
		{ "setUVQuad",				_setUVQuad },
		{ "setUVRect",				_setUVRect },
		{ "transform",				_transform },
		{ "transformUV",			_transformUV },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAISpriteDeck2D::MOAIDeck_ComputeMaxAABB () {

	ZLSize size = this->mQuads.Size ();
	
	if ( size ) {
	
		ZLRect frame;
		for ( ZLIndex i = 0; i < size; ++i ) {
			frame.Grow ( this->mQuads [ i ].GetFrame (), i > 0 );
		}
		
		ZLBounds bounds;
		bounds.Init ( frame );
		bounds.mStatus = ZLBounds::ZL_BOUNDS_OK;
		return bounds;
	}
	return ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAISpriteDeck2D::MOAIDeck_Draw ( ZLIndex idx ) {

	MOAIMaterialMgr& materialMgr = MOAIMaterialMgr::Get ();

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIQuadBrush::BindVertexFormat ();
	
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );
	gfxMgr.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );

	ZLSize totalSprites			= this->mSprites.Size ();
	ZLSize totalSpriteLists		= this->mSpriteLists.Size ();
	ZLSize totalQuads			= this->mQuads.Size ();
	
	if ( totalSprites ) {

		ZLIndex base;
		ZLIndex top;
		
		if ( totalSpriteLists ) {
		
			MOAISpriteList& spriteList = this->mSpriteLists [  ZLIndexOp::Wrap ( idx, totalSpriteLists )];
			base = spriteList.mBaseSprite;
			top = base + spriteList.mTotalSprites;
		}
		else {
		
			base =  ZLIndexOp::Wrap ( idx, totalSprites );
			top = base + ( ZLSize )1;
		}
		
		ZLIndex materialID = ZLIndexOp::INVALID;
//		materialMgr.Push (); // push an empty stack frame to facilitate "swap" below
		materialMgr.LoadGfxState ();
		
		for ( ZLIndex i = base; i < top; ++i ) {
			
			MOAISprite spritePair = this->mSprites [  ZLIndexOp::Wrap ( i, totalSprites )];
			
			MOAIGfxComposer* composer = this->GetComposer ();
			if ( !composer ) continue;
			
			MOAISpriteDeck2DCallable callable;
			callable.mBrush.mUVQuad = this->mUVQuads [ spritePair.mUVQuadID ];
			callable.mBrush.mModelQuad = this->mQuads [ spritePair.mQuadID ];
			
			composer->Execute ( callable );
		}
		
//		materialMgr.Pop ();
	}
	else {
		
		MOAIGfxComposer* composer = this->GetComposer ();
		if ( !composer ) return;
		
		MOAISpriteDeck2DCallable callable;
		MOAIMaterial* material = 0;
		
		if ( totalQuads ) {
		
			ZLIndex itemIdx =  ZLIndexOp::Wrap ( idx, totalQuads );
//			material = this->GetMaterial ( itemIdx );

			callable.mBrush.mModelQuad = this->mQuads [ itemIdx ];

			if ( itemIdx < this->mUVQuads.Size ()) {
				callable.mBrush.mUVQuad = this->mUVQuads [ itemIdx ];
			}
			else {
				callable.mBrush.mUVQuad.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
			}
		}
		else {
				
			callable.mBrush.mUVQuad.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
			callable.mBrush.mModelQuad.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
		}
		
//		materialMgr.Push ();
		materialMgr.LoadGfxState ();
		composer->Execute ( callable );
//		materialMgr.Pop ();
	}
}

//----------------------------------------------------------------//
ZLBounds MOAISpriteDeck2D::MOAIDeck_GetBounds ( ZLIndex idx ) {

	ZLBounds bounds = ZLBounds::EMPTY;

	ZLSize totalSprites			= this->mSprites.Size ();
	ZLSize totalSpriteLists		= this->mSpriteLists.Size ();
	ZLSize totalQuads			= this->mQuads.Size ();
	
	if ( totalQuads ) {
	
		if ( totalSprites ) {

			ZLIndex base;
			ZLIndex top;
			
			if ( totalSpriteLists ) {
			
				MOAISpriteList& spriteList = this->mSpriteLists [  ZLIndexOp::Wrap ( idx, totalSpriteLists )];
				base = spriteList.mBaseSprite;
				top = base + spriteList.mTotalSprites;
			}
			else {
			
				base =  ZLIndexOp::Wrap ( idx, totalSprites );
				top = base + ( ZLSize )1;
			}

			ZLRect rect;

			for ( ZLIndex i = base ; i < top; ++i ) {
				
				MOAISprite sprite = this->mSprites [  ZLIndexOp::Wrap ( i, totalSprites )];
				rect.Grow ( this->mQuads [ sprite.mQuadID ].GetFrame (), i > base );
			}
			bounds.Init ( rect );
		}
		else {
		
			ZLRect rect = this->mQuads [  ZLIndexOp::Wrap ( idx, totalQuads )].GetFrame ();
			bounds.Init ( rect );
		}
	}
	else {
	
		bounds.Init ( -0.5f, 0.5f, 0.5f, -0.5f );
	}
	
	return bounds;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAISpriteDeck2D::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {
	UNUSED(idx);
	return 0;
}

//----------------------------------------------------------------//
bool MOAISpriteDeck2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	//return this->GetMaterialBatch ()->TestHit ( this, idx, granularity, this->mQuad.mModelQuad, this->mQuad.mUVQuad, vec.mX, vec.mY );
	UNUSED(result);
	UNUSED(granularity);
	UNUSED(vec);
	UNUSED(idx);
	return true;
}

//----------------------------------------------------------------//
bool MOAISpriteDeck2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	return (( vec.mZ == 0.0f ) && this->MOAIDeck_Overlap ( idx, vec, granularity, result ));
}
