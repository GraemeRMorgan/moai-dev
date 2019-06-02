// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-gfx/MOAIGfxResource.h>

//================================================================//
// MOAIGfxBuffer
//================================================================//
class MOAIGfxBuffer :
	public virtual MOAIGfxResource,
	public virtual MOAIStream,
	public virtual ZLCopyOnWrite {
protected:

	//----------------------------------------------------------------//
	static int			_copyFromStream				( lua_State* L );
	static int			_reserve					( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAIGfxResource_OnCPUDestroy	();
	
public:

	//----------------------------------------------------------------//
	void				CopyFromStream				( ZLStream& stream, size_t size );
						MOAIGfxBuffer				();
						~MOAIGfxBuffer				();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void				Reserve						( ZLSize size );
	void				MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
