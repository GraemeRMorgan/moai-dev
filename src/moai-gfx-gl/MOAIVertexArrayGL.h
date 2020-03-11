// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXARRAYGL_H
#define	MOAIVERTEXARRAYGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class MOAIVertexBufferGL;
class MOAIVertexFormatGL;

//================================================================//
// MOAIVertexBufferWithFormatGL
//================================================================//
class MOAIVertexBufferWithFormatGL {
public:

	ZLStrongPtr < MOAIVertexBufferGL >				mBuffer;
	ZLStrongPtr < MOAIVertexFormatGL >				mFormat;

	//----------------------------------------------------------------//
				MOAIVertexBufferWithFormatGL		();
				~MOAIVertexBufferWithFormatGL		();
	void		SetBufferAndFormat				( MOAIVertexBufferGL* buffer, MOAIVertexFormatGL* format );
	
};

//================================================================//
// MOAIVertexArrayGL
//================================================================//
// TODO: doxygen
class MOAIVertexArrayGL :
	public virtual MOAIVertexArray,
	public virtual MOAIGfxResourceGL {
protected:

	ZLLeanArray < ZLGfxHandle >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	ZLIndex											mCurrentVAO;

	ZLLeanArray < MOAIVertexBufferWithFormatGL >	mVertexBuffers;

	bool					mUseVAOs;

	//----------------------------------------------------------------//
	static int				_reserveVAOs				( lua_State* L );
	static int				_reserveVertexBuffers		( lua_State* L );
	static int				_setVertexBuffer			( lua_State* L );

	//----------------------------------------------------------------//
	bool					AffirmVertexBuffers			( u32 idx );
	void					BindVertexArrayItems		();
	void					UnbindVertexArrayItems		();
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_SerializeIn								( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void					_SerializeOut								( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void					MOAIGfxResourceGL_OnGPUBind					(); // select GPU-side resource on device for use
	bool					MOAIGfxResourceGL_OnGPUCreate				(); // create GPU-side resource
	void					MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void					MOAIGfxResourceGL_OnGPUUnbind				(); // unbind GPU-side resource
	bool					MOAIGfxResourceGL_OnGPUUpdate				();
	MOAIVertexBuffer*		MOAIVertexArray_GetVertexBuffer				( ZLIndex idx );
	MOAIVertexFormat*		MOAIVertexArray_GetVertexFormat				( ZLIndex idx );
	void					MOAIVertexArray_SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );

public:
	
	IS ( UsingVAOs, mUseVAOs, true )
	
	DECL_LUA_FACTORY ( MOAIVertexArrayGL )
	
	//----------------------------------------------------------------//
							MOAIVertexArrayGL			();
							~MOAIVertexArrayGL			();
	void					ReserveVAOs					( u32 total );
	void					ReserveVertexBuffers		( u32 total );
};

#endif
