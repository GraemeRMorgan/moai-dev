// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFERGL_H
#define	MOAIINDEXBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxBufferGL.h>

//================================================================//
// MOAIIndexBufferGL
//================================================================//
/**	@lua	MOAIIndexBufferGL
	@text	Buffer for vertex indices.
*/
class MOAIIndexBufferGL :
	public virtual MOAIIndexBuffer,
	public virtual MOAIGfxBufferGL {
protected:

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	
public:
	
	friend class MOAIGfxMgrGL_DisplayListClerkGL;
	
	IMPLEMENT_FINALIZABLE ( MOAIIndexBufferGL )
	DECL_LUA_FACTORY ( MOAIIndexBufferGL )
	
	//----------------------------------------------------------------//
					MOAIIndexBufferGL		();
					~MOAIIndexBufferGL		();
};

#endif
