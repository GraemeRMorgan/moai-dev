// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionPrim.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIMoveConstraint2D.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIOverlapResolver.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionShape::Bless () {

	ZLBox aabb;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mShapes.Size (); ++i ) {
		aabb.Grow ( this->mShapes [ i ]->mAABB, i > ZLIndexOp::ZERO );
	}
	this->mBounds.Init ( aabb );
}

//----------------------------------------------------------------//
void MOAICollisionShape::Clear () {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mShapes.Size (); ++i ) {
		if ( this->mShapes [ i ]) {
			delete ( this->mShapes [ i ]);
		}
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Draw ( const ZLAffine3D& localToWorldMtx ) {

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.SetUVTransform ( gfxState.GetMtx ( MOAIGfxState::UV_TO_MODEL_MTX ));

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mShapes.Size (); ++i ) {
	
		MOAICollisionPrim* shape = this->mShapes [ i ];
	
		switch ( shape->mType ) {
			
			case MOAICollisionConsts::BOX: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::WORLD_TO_CLIP_MTX );
			
				ZLBox box = *( ZLBox* )shape->mPtr;
				box.Transform ( localToWorldMtx );
				draw.DrawBoxOutline ( box );
				break;
			}
			case MOAICollisionConsts::CIRCLE: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::MODEL_TO_CLIP_MTX );
			
				ZLCircle circle = *( ZLCircle* )shape->mPtr;
				draw.DrawEllipseOutline ( circle.mCenter.mX, circle.mCenter.mY, circle.mRadius, circle.mRadius, 32 );
				break;
			}
			case MOAICollisionConsts::FRUSTUM:
				break;
			
			case MOAICollisionConsts::POLYGON:
				break;
			
			case MOAICollisionConsts::PRISM:
				break;
			
			case MOAICollisionConsts::QUAD: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::MODEL_TO_CLIP_MTX );
				draw.DrawQuadOutline ( *( ZLQuad* )shape->mPtr );
				break;
			}
			case MOAICollisionConsts::RECT: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::WORLD_TO_CLIP_MTX );
			
				ZLRect rect = *( ZLRect* )shape->mPtr;
				localToWorldMtx.Transform ( rect );
				draw.DrawRectOutline ( rect );
				break;
			}
			case MOAICollisionConsts::SPHERE:
				break;
			
			default:
				break;
		}
	}
}

//----------------------------------------------------------------//
MOAICollisionShape::MOAICollisionShape () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLBox& box ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapBox* overlap = new MOAIOverlapBox ();
	overlap->mShape = box;
	overlap->mAABB.Init ( box );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLQuad& quad ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapQuad* overlap = new MOAIOverlapQuad ();
	overlap->mShape = quad;
	overlap->mAABB.Init ( quad.GetFrame ());
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLRect& rect ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapRect* overlap = new MOAIOverlapRect ();
	overlap->mShape = rect;
	overlap->mAABB.Init ( rect );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}
