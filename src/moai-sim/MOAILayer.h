// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILAYER_H
#define	MOAILAYER_H

#include <moai-sim/MOAIDrawable.h>

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;

//================================================================//
// MOAILayer
//================================================================//
class MOAILayer :
	public virtual MOAILuaObject,
	public virtual MOAIDrawable {
private:

	u32				mClearFlags;
	u32				mClearColor;
	u32				mClearMode;

	MOAILuaSharedPtr < MOAIColor >			mClearColorNode;
	MOAILuaSharedPtr < MOAIFrameBuffer >	mFrameBuffer;

	//----------------------------------------------------------------//
	static int		_draw					( lua_State* L );
	static int		_getClearMode			( lua_State* L );
	static int		_getFrameBuffer			( lua_State* L );
	static int		_pushRenderPass			( lua_State* L );
	static int		_setClearColor			( lua_State* L );
	static int		_setClearDepth			( lua_State* L );
	static int		_setClearMode			( lua_State* L );
	static int		_setFrameBuffer			( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			ClearSurface			();

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw		( int subPrimID );

public:

	GET_SET ( u32, ClearFlags, mClearFlags );
	GET_SET ( u32, ClearMode, mClearMode );

	enum {
		CLEAR_NEVER,
		CLEAR_ALWAYS,
		CLEAR_ON_BUFFER_FLAG,
	};
	
	DECL_LUA_FACTORY ( MOAILayer )

	//----------------------------------------------------------------//
	MOAIFrameBuffer*	GetFrameBuffer			();
						MOAILayer				();
						~MOAILayer				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SetClearColor			( MOAIColor* color );
	void				SetFrameBuffer			( MOAIFrameBuffer* frameBuffer = 0 );
};

#endif
