// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEVEC_H
#define	MOAIANIMCURVEVEC_H

#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveVec
//================================================================//
/**	@lua	MOAIAnimCurveVec
	@text	Implementation of animation curve for 3D vector values.
*/
class MOAIAnimCurveVec :
	public virtual MOAIAnimCurve {
private:

	ZLLeanArray < ZLVec3D > mSamples;
	ZLVec3D mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	ZLVec3D			GetCurveDelta		() const;
	ZLVec3D			GetValue			( const MOAIAnimKeySpan& span ) const;

	//----------------------------------------------------------------//
	void				MOAIAnimCurve_ApplyValueAttrOp		( MOAIAttribute& attr, u32 op );
	void				MOAIAnimCurve_GetDelta				( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	void				MOAIAnimCurve_GetValue				( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void				MOAIAnimCurve_GetZero				( MOAIAttribute& attr ) const;
	void				MOAIAnimCurve_ReserveSamples		( u32 total );
	void				MOAINode_Update							();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveVec )
	
	//----------------------------------------------------------------//
	ZLVec3D			GetValue				( float time ) const;
					MOAIAnimCurveVec		();
					~MOAIAnimCurveVec		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveSamples			( u32 total );
	void			SetSample				( ZLIndex idx, const ZLVec3D& value );
};

#endif
