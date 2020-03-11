// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROP_H
#define	MOAIGRAPHICSPROP_H

#include <moai-sim/MOAIHasDeckAndIndex.h>
#include <moai-sim/MOAIGraphicsPropBase.h>

//================================================================//
// MOAIGraphicsProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsProp :
	public virtual MOAIHasDeckAndIndex,
	public virtual MOAIGraphicsPropBase {
protected:

	//----------------------------------------------------------------//
	ZLBounds				MOAIAbstractProp_GetModelBounds			(); // get the prop bounds in model space
	void					MOAIDrawable_Draw						( int subPrimID );
	bool					MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update							();
	bool					MOAIPartitionHull_Inside				( ZLVec3D vec, float pad );

public:

	DECL_LUA_FACTORY ( MOAIGraphicsProp )

	//----------------------------------------------------------------//
							MOAIGraphicsProp			();
	virtual					~MOAIGraphicsProp			();
};

#endif
