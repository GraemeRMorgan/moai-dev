// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERGL_H
#define	MOAISHADERGL_H

class MOAIShaderProgramGL;

//================================================================//
// MOAIShaderGL
//================================================================//
/**	@lua	MOAIShaderGL
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShaderGL :
	public virtual MOAIAbstractShader,
	public virtual MOAINode {
protected:

	friend class MOAIGfxStateGPUCacheGL;
	friend class MOAIShaderProgramGL;

	ZLStrongPtr < MOAIShaderProgramGL >	mProgram;
	ZLLeanArray < u8 >					mPendingUniformBuffer;

	//----------------------------------------------------------------//
	static int				_getAttributeID				( lua_State* L );
	static int				_setProgram					( lua_State* L );
	static int				_setUniform					( lua_State* L );
	static int				_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
	bool					IsDirty						();

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	GET ( MOAIShaderProgramGL*, Program, mProgram )

	DECL_LUA_FACTORY ( MOAIShaderGL )

	//----------------------------------------------------------------//
	void					ApplyUniforms				();
	void					BindUniforms				();
	void					Bless						();
	bool					HasDirtyUniforms			();
							MOAIShaderGL				();
							~MOAIShaderGL				();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					ResizeUniformArray			( u32 uniformID, u32 count );
	void					ScheduleTextures			();
	void					SetProgram					( MOAIShaderProgramGL* program );
	void					UpdateUniforms				();
};

#endif
