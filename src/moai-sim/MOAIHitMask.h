// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHITMASK_H
#define	MOAIHITMASK_H

//================================================================//
// MOAIHitMask
//================================================================//
class MOAIHitMask :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLStrongPtr < ZLImage > mImage;

	u32				mHitColorScalar;
	u32				mHitColorThreshold;

	//----------------------------------------------------------------//
	static int		_setImage				( lua_State* L );
	static int		_setScalar				( lua_State* L );
	static int		_setThreshold			( lua_State* L );
	static int		_testHit				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIHitMask				();
					~MOAIHitMask			();
	void			SetImage				( ZLImage* image );
	void			SetScalar				( u32 scalar );
	void			SetThreshold			( u32 threshold );
	bool			TestHit					( float x, float y ); // in local (uv) space of the mask
	bool			TestHit					( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad
};

#endif
