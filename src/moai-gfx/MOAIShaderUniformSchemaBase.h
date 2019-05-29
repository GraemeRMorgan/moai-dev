// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMSCHEMABASE_H
#define	MOAISHADERUNIFORMSCHEMABASE_H

#include <moai-gfx/MOAIAbstractShaderUniformSchema.h>

//================================================================//
// MOAIShaderUniformSchemaBase
//================================================================//
class MOAIShaderUniformSchemaBase :
	public virtual MOAIAbstractShaderUniformSchema {
public:
	
	//----------------------------------------------------------------//
	bool		ApplyAttrOp							( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const;
	void		SetUniform							( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const;
				MOAIShaderUniformSchemaBase			();
				~MOAIShaderUniformSchemaBase		();
};

#endif
