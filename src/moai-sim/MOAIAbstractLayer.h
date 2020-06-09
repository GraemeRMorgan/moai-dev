// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTLAYER_H
#define	MOAIABSTRACTLAYER_H

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;
class MOAIFrameBuffer;

//================================================================//
// MOAIAbstractLayer
//================================================================//
class MOAIAbstractLayer :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractDrawable,
	public virtual MOAISurfaceClearColor {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int		_draw						( lua_State* L );
	static int		_pushRenderPass				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIDrawable_DrawDebug		( int subPrimID );

public:
	
	//----------------------------------------------------------------//
					MOAIAbstractLayer			();
					~MOAIAbstractLayer			();
};

#endif