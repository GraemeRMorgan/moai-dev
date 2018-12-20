// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPROP_H
#define	MOAIABSTRACTPROP_H

#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// MOAIAbstractProp
//================================================================//
// TODO: doxygen
class MOAIAbstractProp :
	public virtual MOAIPartitionHull,
	public virtual MOAITransform {
protected:

	u32					mFlags;
	ZLBox				mModelBoundsOverride;
	ZLVec3D				mModelBoundsPad;

	//----------------------------------------------------------------//
	static int			_getModelBounds				( lua_State* L );
	static int			_getModelBoundsSize			( lua_State* L );
	static int			_setExpandForSort			( lua_State* L );
	static int			_setFlag					( lua_State* L );
	static int			_setModelBounds				( lua_State* L );
	static int			_setModelBoundsPad			( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBounds	MOAIAbstractProp_GetModelBounds			() = 0; // get the prop bounds in model space

	//----------------------------------------------------------------//
	bool				MOAINode_ApplyAttrOp					( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	void				MOAINode_Update							();
	void				MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key );
	bool				MOAIPartitionHull_Inside				( ZLVec3D vec, float pad );

public:

	enum {
		FLAGS_OVERRIDE_BOUNDS		= 0x01,
		FLAGS_PAD_BOUNDS			= 0x02,
		FLAGS_EXPAND_FOR_SORT		= 0x04,
		FLAGS_PARTITION_GLOBAL		= 0x08,
		TOTAL_FLAGS,
	};

	//----------------------------------------------------------------//
	ZLBounds			GetModelBounds				(); // get the prop bounds in model space
	bool				InsideModelBounds			( const ZLVec3D& vec, float pad );
						MOAIAbstractProp			();
	virtual				~MOAIAbstractProp			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
