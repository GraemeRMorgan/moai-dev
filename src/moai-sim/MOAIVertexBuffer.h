// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/ZLVertexBuffer.h>

class MOAIVertexFormat;

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@lua	MOAIIndexBuffer
	@text	Buffer for vertices.
*/
class MOAIVertexBuffer :
	public virtual MOAIGfxResource < ZLVertexBuffer >,
	public virtual MOAIGfxBuffer {
private:
	
	friend class MOAIGfxPipelineClerk;
	friend class ZLVertexBufferWithFormat;

	//----------------------------------------------------------------//
	static int		_computeBounds			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printVertices			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLGfxBuffer&		MOAIGfxBuffer_ZLGfxBuffer		();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	//----------------------------------------------------------------//
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			PrintVertices			( MOAIVertexFormat& vertexFormat );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
