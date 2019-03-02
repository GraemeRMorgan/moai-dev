// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tesselator.h>

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshSparseQuadTree.h>
#include <moai-sim/MOAIMeshTernaryTree.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIMeshPrim
//================================================================//
	
//----------------------------------------------------------------//
ZLBox MOAIMeshPrimCoords::GetAABB () {

	ZLBox aabb;
	aabb.Init ( this->mCoords [ 0 ]);
	
	if ( this->mPrimSize > 1 ) {
	
		aabb.Grow ( this->mCoords [ 1 ]);
	
		if ( this->mPrimSize > 2 ) {
			aabb.Grow ( this->mCoords [ 2 ]);
		}
	}
	return aabb;
}

//================================================================//
// MOAIMeshPrimReader
//================================================================//

//----------------------------------------------------------------//
bool MOAIMeshPrimReader::GetPrimCoords ( u32 idx, MOAIMeshPrimCoords& prim ) const {

	// TODO: should return index instead of bool?
	
	assert ( this->mMesh && this->mVertexFormat && this->mVertexBuffer );
	
	prim.mIndex = idx;
	
	switch ( this->mMesh->mPrimType ) {
		
		case ZGL_PRIM_POINTS: {
		
			prim.mPrimSize = 1;
			prim.mCoords [ 0 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_LINES:
		
			idx = idx * 2;

		case ZGL_PRIM_LINE_LOOP:
		case ZGL_PRIM_LINE_STRIP: {
		
			prim.mPrimSize = 2;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLES: {
		
			prim.mPrimSize = 3;
		
			idx = idx * 3;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLE_FAN: {
		
			prim.mPrimSize = 3;
		
			prim.mCoords [ 0 ] = this->ReadCoord ( 0 );
		
			idx = idx + 1;
		
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLE_STRIP: {
		
			// 0   1   2   3   4   5   6
			// 012 213 234 435 456 657 678
		
			prim.mPrimSize = 3;
		
			if ( idx & 1 ) {
				
				// odd
				prim.mCoords [ 0 ] = this->ReadCoord ( idx + 1 );
				prim.mCoords [ 1 ] = this->ReadCoord ( idx );
				prim.mCoords [ 2 ] = this->ReadCoord ( idx + 2 );
			}
			else {
			
				// even
				prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
				prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
				prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			}
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool MOAIMeshPrimReader::Init ( MOAIMesh& mesh, ZLIndex vertexBufferIndex ) {

	MOAIVertexFormat* vertexFormat = mesh.GetVertexFormat ( vertexBufferIndex );
	ZLVertexBuffer* vertexBuffer = mesh.GetVertexBuffer ( vertexBufferIndex );

	if ( !( vertexFormat && vertexBuffer )) return false;

	if ( !vertexFormat->CountAttributesByUse ( MOAIVertexFormat::ATTRIBUTE_COORD )) return false;

	this->mMesh				= &mesh;
	this->mVertexFormat		= vertexFormat;
	
	this->mAttribute		= vertexFormat->GetAttributeByUse ( MOAIVertexFormat::ATTRIBUTE_COORD, ZLIndexOp::ZERO );
	this->mVertexBuffer		= vertexBuffer->ZLCopyOnWrite::GetBuffer ();
	this->mIndexBuffer		= mesh.mIndexBuffer;
	
	this->mTotalPrims = 0;
	
	switch ( this->mMesh->mPrimType ) {
		
		case ZGL_PRIM_POINTS:
		case ZGL_PRIM_LINE_LOOP:
			this->mTotalPrims = mesh.mTotalElements;
			break;
		
		case ZGL_PRIM_LINES:
			this->mTotalPrims = mesh.mTotalElements / 2;
			break;
		
		case ZGL_PRIM_LINE_STRIP:
			this->mTotalPrims = mesh.mTotalElements - 1;
			break;
		
		case ZGL_PRIM_TRIANGLES:
			this->mTotalPrims = mesh.mTotalElements / 3;
			break;
		
		case ZGL_PRIM_TRIANGLE_FAN:
		case ZGL_PRIM_TRIANGLE_STRIP:
			this->mTotalPrims = mesh.mTotalElements - 2;
			break;
	}
	
	return true;
}

//----------------------------------------------------------------//
ZLVec3D MOAIMeshPrimReader::ReadCoord ( u32 idx ) const {

	if ( !( this->mMesh && this->mVertexFormat && this->mAttribute && this->mVertexBuffer )) return ZLVec3D::ORIGIN; // TODO: report error

	idx %= this->mMesh->mTotalElements;

	if ( this->mIndexBuffer ) {
		idx = this->mIndexBuffer->GetIndex ( idx );
	}
	
	const void* packedCoord = this->mVertexFormat->GetAttributeAddress ( *this->mAttribute, this->mVertexBuffer, idx );
	ZLVec4D coord = this->mVertexFormat->UnpackCoord ( packedCoord, *this->mAttribute );
	
	return ZLVec3D ( coord.mX, coord.mY, coord.mZ );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildQuadTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	u32 targetPrimsPerNode			= state.GetValue < u32 >( 1, MOAIMeshSparseQuadTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	ZLIndex vertexBufferIndex		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );

	MOAIMeshPrimReader coordReader;
	
	if ( coordReader.Init ( *self, vertexBufferIndex )) {
	
		MOAIMeshSparseQuadTree* quadTree = new MOAIMeshSparseQuadTree ();
		quadTree->Init ( coordReader, targetPrimsPerNode );
		self->mPartition = quadTree;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildTernaryTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	u32 axisMask				= state.GetValue < u32 >( 2, MOAIMeshTernaryTree::AXIS_MASK_ALL );
	u32 targetPrimsPerNode		= state.GetValue < u32 >( 3, MOAIMeshTernaryTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	ZLIndex vertexBufferIndex	= state.GetValue < MOAILuaIndex >( 4, ZLIndexOp::ZERO );
	
	MOAIMeshPrimReader coordReader;
	
	if ( coordReader.Init ( *self, vertexBufferIndex )) {
	
		MOAIMeshTernaryTree* ternaryTree = new MOAIMeshTernaryTree ();
		ternaryTree->Init ( coordReader, targetPrimsPerNode, axisMask );
		self->mPartition = ternaryTree;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMesh::_getPrimsForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	// TODO: this is a naive first pass. need to use the partition if one has been created.
	// TODO: handle all prim types or bail if not triangles
	
	MOAIMeshPrimReader primReader;
	
	bool is3D = state.CheckParams ( 2, "NNN", false );
	
	ZLVec3D point = state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );

	u32 totalPrims = 0;

	ZLBox meshBounds = self->GetBounds ().mAABB;
	if ((( is3D ) && meshBounds.Contains ( point )) || meshBounds.Contains ( point, ZLBox::PLANE_XY )) {
		
		if ( primReader.Init ( *self, ZLIndexOp::ZERO )) {
			
			u32 basePrim = state.GetValue < u32 >( 5, 1 ) - 1;
			u32 nPrims = state.GetValue < u32 >( 6, primReader.GetTotalPrims ());
			
			for ( u32 i = basePrim; i < nPrims; ++i ) {
				
				MOAIMeshPrimCoords prim;
				if ( primReader.GetPrimCoords ( i, prim )) {
					
					if ((
						( is3D )
						&&
						ZLBarycentric::PointInTriangle (
							prim.mCoords [ 0 ],
							prim.mCoords [ 1 ],
							prim.mCoords [ 2 ],
							point ))
						||
						ZLBarycentric::PointInTriangle (
							prim.mCoords [ 0 ].Vec2D (),
							prim.mCoords [ 1 ].Vec2D (),
							prim.mCoords [ 2 ].Vec2D (),
							point.Vec2D ())
						) {
					
						state.Push ( i + 1 );
						totalPrims++;
					}
				}
			}
		}
	}
	return totalPrims;
}

//----------------------------------------------------------------//
int MOAIMesh::_getRegionForPrim ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	// TODO: non-triangle meshes? need to support or error out

	u32 basePrimID = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 nPrims = state.GetValue < u32 >( 3, 1 );

	SafeTesselator tesselator;

	MOAIMeshPrimReader primReader;
	
	if ( primReader.Init ( *self, ZLIndexOp::ZERO )) {
		
		for ( u32 i = 0; i < nPrims; ++i ) {
		
			MOAIMeshPrimCoords prim;
			if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
				
				assert ( prim.mPrimSize == 3 );
				
				ZLVec2D triangle [ 3 ];
				
				triangle [ 0 ] = prim.mCoords [ 0 ].Vec2D ();
				triangle [ 1 ] = prim.mCoords [ 1 ].Vec2D ();
				triangle [ 2 ] = prim.mCoords [ 2 ].Vec2D ();
				
				tesselator.AddContour2D ( triangle, 3 );
			}
		}
	}
	
	tesselator.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );

	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 4, false );
	region = region ? region : new MOAIRegion ();

	region->Copy ( tesselator );

	state.Push ( region );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_intersectRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	// TODO: this is a naive first pass. need to use the partition if one has been created.
	// TODO: handle all prim types or bail if not triangles
	
	MOAIMeshPrimReader primReader;
	
	ZLVec3D loc		= state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
	ZLVec3D vec		= state.GetValue < ZLVec3D >( 5, ZLVec3D::ORIGIN );
	
	bool hasHit = false;
	float bestTime = 0.0f;
	ZLVec3D bestHit;
	
	if ( primReader.Init ( *self, ZLIndexOp::ZERO )) {
	
		u32 totalMeshPrims = primReader.GetTotalPrims ();
		
		for ( u32 i = 0; i < totalMeshPrims; ++i ) {
		
			MOAIMeshPrimCoords prim;
			if ( primReader.GetPrimCoords ( i, prim )) {
				
				float time;
				ZLVec3D hit;
				
				if ( ZLSect::VecToTriangle ( loc, vec, prim.mCoords [ 0 ], prim.mCoords [ 1 ], prim.mCoords [ 2 ], time, hit ) == ZLSect::SECT_HIT ) {
				
					if (( !hasHit ) || ( time < bestTime )) {
						bestTime = time;
						bestHit = hit;
						hasHit = true;
					}
				}
			}
		}
	}
	
	if ( hasHit ) {
		state.Push ( bestTime );
		state.Push ( bestHit );
		return 4;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_printPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	if ( self->mPartition ) {
		self->mPartition->Print ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_readPrimCoords ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( stream ) {
	
		u32 basePrimID = state.GetValue < u32 >( 3, 1 ) - 1;
		u32 nPrims = state.GetValue < u32 >( 4, 1 );

		MOAIMeshPrimReader primReader;
		
		if ( primReader.Init ( *self, ZLIndexOp::ZERO )) {
			
			for ( u32 i = 0; i < nPrims; ++i ) {
			
				MOAIMeshPrimCoords prim;
				if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
					
					stream->Write < ZLVec3D >( prim.mCoords [ 0 ]);
					
					if ( prim.mPrimSize > 1 ) {
					
						stream->Write < ZLVec3D >( prim.mCoords [ 1 ]);
						
						if ( prim.mPrimSize > 2 ) {
						
							stream->Write < ZLVec3D >( prim.mCoords [ 2 ]);
						}
					}
				}
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->SetBounds ( state.GetValue < ZLBox >( 2, ZLBox::EMPTY ));
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBuffer
	@text	Set the index buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIGfxBuffer indexBuffer
	@out	nil
*/
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth
	@text	Sets the pen with for drawing prims in this vertex buffer.
			Only valid with prim types GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP.
	
	@in		MOAIMesh self
	@in		number penWidth
	@out	nil
*/
int MOAIMesh::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPenWidth = state.GetValue < float >( 2, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIMesh self
	@in		number primType		One of MOAIMesh GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIMesh::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->SetPrimType ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------///
// TODO: doxygen
int MOAIMesh::_setTotalElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->SetTotalElements ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::ClearBounds () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
u32 MOAIMesh::CountPrims () const {

	return 0;
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( ZLIndex idx, MOAIMeshSpan* span ) {

	MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();
	materialStack.Push ( this->GetMaterial ( idx ));
	materialStack.SetShader ( MOAIShaderMgr::MESH_SHADER );
	materialStack.LoadGfxState ();
	materialStack.Pop ();

	//if ( !this->LoadGfxState ( materials, idx, MOAIShaderMgr::MESH_SHADER )) return;

	// TODO: make use of offset and scale

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;
	if ( gfxState.SetVertexArray ( this )) {

		// I am super lazy, so set this up here instead of adding if's below
		MOAIMeshSpan defaultSpan;
		if ( !span ) {
			defaultSpan.mBase = ZLIndexOp::ZERO;
			defaultSpan.mTop = ZLIndexCast ( this->mTotalElements );
			defaultSpan.mNext = 0;
			span = &defaultSpan;
		}
		
		gfxState.SetPenWidth ( this->mPenWidth );
		
		if ( this->mIndexBuffer ) {
			
			// TODO: turns out we can bind this inside the VAO as well. so there.
			if ( gfxState.SetIndexBuffer ( this->mIndexBuffer )) {
			
				for ( ; span; span = span->mNext ) {
				
					if ( span->mBase == span->mTop ) continue;
					assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
				
					gfxState.DrawPrims ( this->mPrimType, span->mBase, ( u32 )( span->mTop - span->mBase ));
				}

				gfxState.SetIndexBuffer ();
			}
		}
		else {
		
			for ( ; span; span = span->mNext ) {
			
				if ( span->mBase == span->mTop ) continue;
				assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
			
				gfxState.DrawPrims ( this->mPrimType, span->mBase, ( u32 )( span->mTop - span->mBase ));
			}
		}
		gfxState.SetVertexArray ();
	}
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mTotalElements ( 0 ),
	mPrimType ( ZGL_PRIM_TRIANGLES ),
	mPenWidth ( 1.0f ),
	mPartition ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIMaterialBatchHolder )
		RTTI_EXTEND ( MOAIVertexArray )
	RTTI_END
	
	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {

	this->SetIndexBuffer ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatchHolder::RegisterLuaClass ( state );
	MOAIVertexArray::RegisterLuaClass ( state );
	
	state.SetField ( -1, "GL_POINTS",			( u32 )ZGL_PRIM_POINTS );
	state.SetField ( -1, "GL_LINES",			( u32 )ZGL_PRIM_LINES );
	state.SetField ( -1, "GL_TRIANGLES",		( u32 )ZGL_PRIM_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP",		( u32 )ZGL_PRIM_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP",		( u32 )ZGL_PRIM_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN",		( u32 )ZGL_PRIM_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP",	( u32 )ZGL_PRIM_TRIANGLE_STRIP );
	
	state.SetField ( -1, "X_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::X_AXIS_MASK );
	state.SetField ( -1, "Y_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Y_AXIS_MASK );
	state.SetField ( -1, "Z_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Z_AXIS_MASK );
	state.SetField ( -1, "AXIS_MASK_ALL",		( u32 )MOAIMeshTernaryTree::AXIS_MASK_ALL );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatchHolder::RegisterLuaFuncs ( state );
	MOAIVertexArray::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "buildQuadTree",				_buildQuadTree },
		{ "buildTernaryTree",			_buildTernaryTree },
		{ "getPrimsForPoint",			_getPrimsForPoint },
		{ "getRegionForPrim",			_getRegionForPrim },
		{ "intersectRay",				_intersectRay },
		{ "printPartition",				_printPartition },
		{ "readPrimCoords",				_readPrimCoords },
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVAOs ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		ZLGfxResourceClerk::DeleteOrDiscard ( this->mVAOs [ i ], false );
	}
	this->mVAOs.Init ( total );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVertexBuffers ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
	MOAIMaterialBatchHolder::SerializeIn ( state, serializer );
	MOAIVertexArray::SerializeIn ( state, serializer );

	this->SetIndexBuffer (
		serializer.MemberIDToObject < MOAIIndexBuffer >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 ))
	);
	
	this->mTotalElements = state.GetFieldValue < cc8*, u32 >( -1, "mTotalElements", 0 );
	
	if ( state.PushFieldWithType ( -1, "mBounds", LUA_TTABLE )) {
		
		ZLBox aabb;
		
		aabb.mMin.mX	= state.GetFieldValue < cc8*, float >( -1, "mMinX", 0 );
		aabb.mMin.mY	= state.GetFieldValue < cc8*, float >( -1, "mMinY", 0 );
		aabb.mMin.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMinZ", 0 );
		
		aabb.mMax.mX	= state.GetFieldValue < cc8*, float >( -1, "mMaxX", 0 );
		aabb.mMax.mY	= state.GetFieldValue < cc8*, float >( -1, "mMaxY", 0 );
		aabb.mMax.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMaxZ", 0 );
		
		this->mBounds.Init ( aabb );
		
		this->mBounds.UpdateStatus ();
		
		state.Pop ();
	}
	
	this->mPenWidth = state.GetFieldValue < cc8*, float >( -1, "mPenWidth", 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
	MOAIMaterialBatchHolder::SerializeOut ( state, serializer );
	MOAIVertexArray::SerializeOut ( state, serializer );

	state.SetField ( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));
	
	state.SetField < cc8*, u32 >( -1, "mTotalElements", this->mTotalElements );
	
	lua_newtable ( state );
	
		state.SetField < cc8*, float >( -1, "mMinX", this->mBounds.mAABB.mMin.mX );
		state.SetField < cc8*, float >( -1, "mMinY", this->mBounds.mAABB.mMin.mY );
		state.SetField < cc8*, float >( -1, "mMinZ", this->mBounds.mAABB.mMin.mZ );
	
		state.SetField < cc8*, float >( -1, "mMaxX", this->mBounds.mAABB.mMax.mX );
		state.SetField < cc8*, float >( -1, "mMaxY", this->mBounds.mAABB.mMax.mY );
		state.SetField < cc8*, float >( -1, "mMaxZ", this->mBounds.mAABB.mMax.mZ );
	
	lua_setfield ( state, -2, "mBounds" );
	
	state.SetField < cc8*, u32 >( -1, "mPenWidth", ( u32 )this->mPenWidth );
}

//----------------------------------------------------------------//
void MOAIMesh::SetBounds ( const ZLBox& aabb ) {

	this->mBounds.Init ( aabb );
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->mIndexBuffer.Set ( *this, indexBuffer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIMesh::MOAIDeck_ComputeMaxAABB () {

	return this->mBounds;
}

//----------------------------------------------------------------//
void MOAIMesh::MOAIDeck_Draw ( ZLIndex idx ) {

	this->DrawIndex ( idx, 0 );
}

//----------------------------------------------------------------//
ZLBounds MOAIMesh::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return this->mBounds;
}
