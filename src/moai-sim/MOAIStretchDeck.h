// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHDECK_H
#define	MOAISTRETCHDECK_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAIStretchDeck
//================================================================//
// TODO: doxygen
class MOAIStretchDeck :
	public MOAIDeck {
protected:

	float			mStretchFactor;

	//----------------------------------------------------------------//
	static int		_setStretchFactor			( lua_State* L );
	
public:
		
	//----------------------------------------------------------------//
	ZLVec3D			BindStretchVertexTransform				() const;
					MOAIStretchDeck							();
					~MOAIStretchDeck						();
	void			RegisterLuaClass						( MOAILuaState& state );
	void			RegisterLuaFuncs						( MOAILuaState& state );
};

#endif
