// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overridden by the prop's bounds if it's been set (using
			setBounds ())
	
	@in		MOAIPartitionHull self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIPartitionHull::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	ZLBounds bounds = self->GetModelBounds ();
	if ( bounds.mStatus != ZLBounds::ZL_BOUNDS_OK ) return 0;

	state.Push ( bounds.mMin.mX );
	state.Push ( bounds.mMin.mY );
	state.Push ( bounds.mMin.mZ );
	
	state.Push ( bounds.mMax.mX );
	state.Push ( bounds.mMax.mY );
	state.Push ( bounds.mMax.mZ );

	return 6;
}

//----------------------------------------------------------------//
/**	@lua	getDims
	@text	Return the prop's width and height or 'nil' if prop rect is global.
               
	@in		MOAIPartitionHull self
	@out	number width		X max - X min
	@out	number height		Y max - Y min
	@out	number depth		Z max - Z min
*/
int MOAIPartitionHull::_getDims ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	ZLBounds bounds = self->GetModelBounds ();
	if ( bounds.mStatus != ZLBounds::ZL_BOUNDS_OK ) return 0;
 
	state.Push ( bounds.mMax.mX - bounds.mMin.mX );
	state.Push ( bounds.mMax.mY - bounds.mMin.mY );
	state.Push ( bounds.mMax.mZ - bounds.mMin.mZ );
 
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getPartition
	@text	Returns the partition prop is currently held in.
	
	@in		MOAIPartitionHull self
	@out	MOAIPartition partition
*/
int	MOAIPartitionHull::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	if ( self->mPartition ) {
		self->mPartition->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIPartitionHull self
	@out	number priority		The node's priority or nil.
*/
int MOAIPartitionHull::_getPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	if ( self->mPriority != UNKNOWN_PRIORITY ) {
		lua_pushnumber ( state, self->mPriority );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getWorldBounds
	@text	Return the prop's world bounds or 'nil' if prop bounds is
			global or missing.
	
	@in		MOAIPartitionHull self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIPartitionHull::_getWorldBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	ZLBox bounds = self->mWorldBounds;

	state.Push ( bounds.mMin.mX );
	state.Push ( bounds.mMin.mY );
	state.Push ( bounds.mMin.mZ );
	
	state.Push ( bounds.mMax.mX );
	state.Push ( bounds.mMax.mY );
	state.Push ( bounds.mMax.mZ );

	return 6;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_getWorldBoundsCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	ZLBox bounds = self->mWorldBounds;
	
	ZLVec3D center;
	bounds.GetCenter ( center );

	state.Push ( center.mX );
	state.Push ( center.mY );
	state.Push ( center.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	insid
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIPartitionHull self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIPartitionHull::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	ZLVec3D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );
	vec.mZ	= state.GetValue < float >( 4, 0.0f );

	float pad = state.GetValue < float >( 5, 0.0f );

	bool result = self->Inside ( vec, pad );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setBounds
	@text	Sets or clears the partition bounds override.
	
	@overload	Clear the bounds override.
	
		@in		MOAIPartitionHull self
		@out	nil
	
	@overload	Set the bounds override.
	
		@in		MOAIPartitionHull self
		@in		number xMin
		@in		number yMin
		@in		number zMin
		@in		number xMax
		@in		number yMax
		@in		number zMax
		@out	nil
*/
int MOAIPartitionHull::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	if ( state.CheckParams ( 2, "NNNNNN", false )) {

		self->mBoundsOverride = state.GetBox ( 2 );
		self->mFlags |= FLAGS_OVERRIDE_BOUNDS;
	}
	else {
		self->mFlags &= ~FLAGS_OVERRIDE_BOUNDS;
	}
	
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_setBoundsPad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	self->mBoundsPad.mX	= state.GetValue < float >( 2, 0.0f );
	self->mBoundsPad.mY	= state.GetValue < float >( 3, 0.0f );
	self->mBoundsPad.mZ	= state.GetValue < float >( 4, 0.0f );

	bool pad = (( self->mBoundsPad.mX != 0.0f ) || ( self->mBoundsPad.mY != 0.0f ) || ( self->mBoundsPad.mZ != 0.0f ));
	self->mFlags = pad ? ( self->mFlags | FLAGS_PAD_BOUNDS ) : ( self->mFlags & ~FLAGS_PAD_BOUNDS );

	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setExpandForSort
	@text	Used when drawing with a layout scheme (i.e. MOAIGrid).
			Expanding for sort causes the prop to emit a sub-prim
			for each component of the layout. For example, when
			attaching a MOAIGrid to a prop, each cell of the grid
			will be added to the render queue for sorting against
			all other props and sub-prims. This is obviously less
			efficient, but still more efficient then using an
			separate prop for each cell or object.
	
	@in		MOAIPartitionHull self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIPartitionHull::_setExpandForSort ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	bool expandForSort = state.GetValue < bool >( 2, false );

	if ( expandForSort ) {
		self->mFlags |= FLAGS_EXPAND_FOR_SORT;
	}
	else {
		self->mFlags &= ~FLAGS_EXPAND_FOR_SORT;
	}

	return 0;
}

//----------------------------------------------------------------//
// TODO: macro
int MOAIPartitionHull::_setFlag ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	u32 flag		= state.GetValue < u32 >( 2, 0 );
	bool set		= state.GetValue < bool >( 3, true );

	self->mFlags = set ? self->mFlags |= flag : self->mFlags &= flag;

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitGranularity
	@text	Specify the granularity to use when performing a hit test. This is a hint to the implementation
			as to how much processing to allocate to a given test. The default value is MOAIPartitionHull.HIT_TEST_COARSE,
			which will cause only the deck or prop bounds to be used. A setting of MOAIPartitionHull.HIT_TEST_MEDIUM or
			MOAIPartitionHull.HIT_TEST_FINE is implementation dependent on the deck, but 'medium' usually means to test
			against the geometry of the deck and 'fine' means to test against both the geometry and the pixels
			of the hit mask (if any).
	
	@in		MOAIPartitionHull self
	@opt	int granularity		One of MOAIPartitionHull.HIT_TEST_COARSE, MOAIPartitionHull.HIT_TEST_MEDIUM, MOAIPartitionHull.HIT_TEST_FINE. Default is MOAIPartitionHull.HIT_TEST_COARSE.
	@out	nil
*/
int MOAIPartitionHull::_setHitGranularity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	self->mHitGranularity = state.GetValue < u32 >( 2, HIT_TEST_COARSE );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	MOAIPartitionHolder* holder = state.GetLuaObject < MOAIPartitionHolder >( 2, false );
	MOAIPartition* partition = holder ? holder->GetPartition () : state.GetLuaObject < MOAIPartition >( 2, true );
	
	self->SetPartition ( partition );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIPartitionHull self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIPartitionHull::_setPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->mPriority = ( s32 )state.GetValue < int >( 2, 0 );
	}
	else {
		self->mPriority = UNKNOWN_PRIORITY;
		if ( self->mPartition ) {
			self->mPartition->AffirmPriority ( *self );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_setQueryMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	self->mQueryMask = state.GetValue < u32 >( 2, 0 );
	return 0;
}

//================================================================//
// MOAIPartitionHull
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionHull::AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	this->MOAIPartitionHull_AddToSortBuffer ( buffer, key );
}

//----------------------------------------------------------------//
u32 MOAIPartitionHull::AffirmInterfaceMask ( MOAIPartition& partition ) {

	return this->MOAIPartitionHull_AffirmInterfaceMask ( partition );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::BoundsDidChange () {

	this->MOAIPartitionHull_BoundsDidChange ();
}

//----------------------------------------------------------------//
//void MOAIPartitionHull::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( sampler );

	//if ( !this->mDeck ) return;
	//
	//sampler.SetSourcePrim ( this );
	//
	//if ( this->mGrid ) {
	//	
	//	ZLRect localRect = sampler.GetLocalRect ();
	//	
	//	MOAICellCoord c0;
	//	MOAICellCoord c1;
	//	
	//	ZLRect deckBounds = this->mDeck->GetBounds ().GetRect( ZLBox::PLANE_XY );

	//	this->mGrid->GetBoundsInRect ( localRect, c0, c1, deckBounds );
	//	//this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	//}
	//else {
	//	//this->mDeck->GatherSurfaces ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), sampler );
	//}
//}

//----------------------------------------------------------------//
bool MOAIPartitionHull::GetCellRect ( ZLRect* cellRect, ZLRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLevel ) {
	
		ZLVec3D center;
		this->mWorldBounds.GetCenter ( center );
		
		MOAICellCoord coord = this->mLevel->mGridSpace.GetCellCoord ( center.mX, center.mY );
		ZLRect rect = this->mLevel->mGridSpace.GetCellRect ( coord );
		
		if ( cellRect ) {
			*cellRect = rect;
		}
		
		if ( paddedRect ) {
			rect.Inflate ( this->mLevel->mCellSize * 0.5f );
			*paddedRect = rect;
		}
		return true;
	}
	
	if ( cellRect ) {
		cellRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	if ( paddedRect ) {
		paddedRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	return false;
}

//----------------------------------------------------------------//
ZLBounds MOAIPartitionHull::GetModelBounds () {

	if ( this->mFlags & FLAGS_PARTITION_GLOBAL ) {
		return ZLBounds::GLOBAL;
	}

	ZLBounds bounds = ZLBounds::EMPTY;

	if ( this->mFlags & FLAGS_OVERRIDE_BOUNDS ) {
		bounds.Init ( this->mBoundsOverride );
	}
	else {
		bounds = this->MOAIPartitionHull_GetModelBounds ();
	}
	
	if (( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) && ( this->mFlags & FLAGS_PAD_BOUNDS )) {
		bounds.Pad ( this->mBoundsPad.mX, this->mBoundsPad.mY, this->mBoundsPad.mZ );
	}

	return bounds;
}

//----------------------------------------------------------------//
MOAIPartition* MOAIPartitionHull::GetPartitionTrait () {

	return this->mPartition;
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::Inside ( ZLVec3D vec, float pad ) {
	
	return this->MOAIPartitionHull_Inside ( vec, pad );
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::InsideModelBounds ( const ZLVec3D& vec, float pad ) {

	ZLBounds bounds = this->GetModelBounds ();
	
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_EMPTY ) return false;
	
	bool passTrivial = false;
	
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_GLOBAL ) {
		passTrivial = true;
	}
	else {
		bounds.Bless ();
		bounds.Inflate ( pad );
		if ( pad != 0 ) bounds.Bless ();
		passTrivial = bounds.Contains ( vec );
	}
	return passTrivial;
}

//----------------------------------------------------------------//
MOAIPartitionHull::MOAIPartitionHull () :
	mPartition ( 0 ),
	mCell ( 0 ),
	mLevel ( 0 ),
	mNextResult ( 0 ),
	mInterfaceMask ( 0 ),
	mQueryMask ( 0xffffffff ),
	mPriority ( UNKNOWN_PRIORITY ),
	mFlags ( 0 ),
	mBoundsPad ( 0.0f, 0.0f, 0.0f ),
	mHitGranularity ( HIT_TEST_COARSE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mLinkInCell.Data ( this );
	this->mWorldBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIPartitionHull::~MOAIPartitionHull () {

	// MOAIPartition retains member props so it *should* be impossible
	// to destruct a prop without first removing it
	assert ( !this->mPartition );
	assert ( !this->mCell );
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::PrepareForInsertion ( const MOAIPartition& partition ) {

	return this->MOAIPartitionHull_PrepareForInsertion ( partition );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_PARTITION",				MOAIPartitionHullAttr::Pack ( ATTR_PARTITION ).ToRaw ());
	
	state.SetField ( -1, "FLAGS_EXPAND_FOR_SORT",		( u32 )FLAGS_EXPAND_FOR_SORT );
	state.SetField ( -1, "FLAGS_PARTITION_GLOBAL",		( u32 )FLAGS_PARTITION_GLOBAL );
	
	state.SetField ( -1, "HIT_TEST_COARSE",			( u32 )HIT_TEST_COARSE );
	state.SetField ( -1, "HIT_TEST_MEDIUM",			( u32 )HIT_TEST_MEDIUM );
	state.SetField ( -1, "HIT_TEST_FINE",			( u32 )HIT_TEST_FINE );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getBounds",				_getBounds },
		{ "getDims",				_getDims },
		{ "getPartition",			_getPartition },
		{ "getPriority",			_getPriority },
		{ "getWorldBounds",			_getWorldBounds },
		{ "getWorldBoundsCenter",	_getWorldBoundsCenter },
		{ "inside",					_inside },
		{ "setBounds",				_setBounds },
		{ "setBoundsPad",			_setBoundsPad },
		{ "setExpandForSort",		_setExpandForSort },
		{ "setFlag",				_setFlag },
		{ "setHitGranularity",		_setHitGranularity },
		{ "setPartition",			_setPartition },
		{ "setPriority",			_setPriority },
		{ "setQueryMask",			_setQueryMask },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAITransform::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAITransform::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::SetPartition ( MOAIPartition* partition ) {

	if ( partition != this->mPartition ) {
		if ( partition ) {
			partition->InsertHull ( *this );
		}
		else if ( this->mPartition ) {
			this->mPartition->RemoveHull ( *this );
		}
	}
}

//----------------------------------------------------------------//
//void MOAIPartitionHull::UpdateWorldBounds ( u32 status ) {
//
//	ZLBox bounds;
//	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
//
//	if ( status == ZLBounds::ZL_BOUNDS_OK ) {
//		status = ZLBounds::ZL_BOUNDS_EMPTY;
//	}
//	this->UpdateWorldBounds ( bounds, status );
//}

//----------------------------------------------------------------//
void MOAIPartitionHull::UpdateWorldBounds ( const ZLBounds& bounds ) {

	MOAIPartitionCell* prevCell = this->mCell;
	ZLBox prevBounds = this->mWorldBounds;

	this->mWorldBounds = bounds;

	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
		this->mWorldBounds.UpdateStatus ();
	}

	if ( this->mPartition ) {
		
		this->mPartition->UpdateHull ( *this );
		
		if (( prevCell != this->mCell ) || ( !prevBounds.IsSame ( this->mWorldBounds ))) {
			this->BoundsDidChange ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionHull::WasRemovedFromPartition () {

	this->MOAIPartitionHull_WasRemovedFromPartition ();
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIPartitionHullAttr::Check ( attrID )) {
		
		switch (  attrID.Unpack ()) {;
			case ATTR_PARTITION:
				this->SetPartition ( attr.ApplyVariantNoAdd < MOAIPartition* >( this->GetPartition (), op, MOAIAttribute::ATTR_READ_WRITE ));
				return true;
		}
	}
	
	return MOAITransform::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAINode_Update () {
	
	MOAITransform::MOAINode_Update ();
	
	ZLBounds bounds = this->GetModelBounds ();
	
	// update the prop location in the partition
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
		bounds.Transform ( this->mLocalToWorldMtx );
	}
	this->UpdateWorldBounds ( bounds );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAIPartitionHull_BoundsDidChange () {
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->InsideModelBounds ( vec, pad );
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAIPartitionHull_PrepareForInsertion ( const MOAIPartition& partition ) {
	UNUSED ( partition );
	return true;
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAIPartitionHull_WasRemovedFromPartition () {
}
