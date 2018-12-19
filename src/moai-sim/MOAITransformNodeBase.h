// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMNODEBASE_H
#define	MOAITRANSFORMNODEBASE_H

#include <moai-sim/MOAINode.h>

//================================================================//
// MOAITransformNodeBase
//================================================================//
/**	@lua	MOAITransformNodeBase
	@text	Base class for 2D affine transforms.
	
	@attr	ATTR_WORLD_X_LOC
	@attr	ATTR_WORLD_Y_LOC
	@attr	ATTR_WORLD_Z_LOC
	@attr	ATTR_WORLD_Z_ROT
	@attr	ATTR_WORLD_X_SCL
	@attr	ATTR_WORLD_Y_SCL
	@attr	ATTR_WORLD_Z_SCL
	@attr	TRANSFORM_TRAIT
*/
class MOAITransformNodeBase :
	public virtual MOAINode {
protected:
	
	ZLAffine3D		mLocalToWorldMtx;
	ZLAffine3D		mWorldToLocalMtx;

	//----------------------------------------------------------------//
	static int	_getWorldDir		( lua_State* L );
	static int	_getWorldLoc		( lua_State* L );
	static int	_getWorldRot		( lua_State* L );
	static int	_getWorldScl		( lua_State* L );
	static int	_getWorldXAxis		( lua_State* L );
	static int	_getWorldYAxis		( lua_State* L );
	static int	_getWorldZAxis		( lua_State* L );
	static int	_getWorldXNormal	( lua_State* L );
	static int	_getWorldYNormal	( lua_State* L );
	static int	_getWorldZNormal	( lua_State* L );
	static int	_modelToWorld		( lua_State* L );
	static int	_worldToModel		( lua_State* L );

	//----------------------------------------------------------------//
	bool				MOAINode_ApplyAttrOp							( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	void				MOAINode_Update									();
	virtual void		MOAITransformNodeBase_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx ) = 0;

public:
	
	DECL_ATTR_HELPER ( MOAITransformNodeBase )
	
	enum {
		ATTR_WORLD_X_LOC,
		ATTR_WORLD_Y_LOC,
		ATTR_WORLD_Z_LOC,
		
		ATTR_WORLD_Z_ROT,
		
		ATTR_WORLD_X_SCL,
		ATTR_WORLD_Y_SCL,
		ATTR_WORLD_Z_SCL,
		
		TRANSFORM_TRAIT,
		
		TOTAL_ATTR,
	};
	
	GET ( ZLVec3D, WorldLoc, mLocalToWorldMtx.GetTranslation ())
	
	//----------------------------------------------------------------//
	const ZLAffine3D&		GetLocalToWorldMtx			() const;
	const ZLAffine3D*		GetLocTrait					() const;
	const ZLAffine3D*		GetTransformTrait			() const;
	const ZLAffine3D&		GetWorldToLocalMtx			() const;
							MOAITransformNodeBase		();
							~MOAITransformNodeBase		();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
