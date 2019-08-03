// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMGL_H
#define	MOAISHADERPROGRAMGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>
#include <moai-gfx-gl/MOAIShaderUniformBindingGL.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class MOAIShaderGL;
class MOAITextureGL;

//================================================================//
// MOAIShaderProgramGlobalGL
//================================================================//
class MOAIShaderProgramGlobalGL {
private:

	friend class MOAIShaderGL;
	friend class MOAIShaderGlobals;
	friend class MOAIShaderProgramGL;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramGlobalGL		();
};

//================================================================//
// MOAIShaderProgramTextureGL
//================================================================//
class MOAIShaderProgramTextureGL {
private:

	friend class MOAIShaderProgramGL;

	u32								mName;
	ZLIndex							mUnit;
	ZLStrongPtr < MOAITextureGL >	mTexture;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramTextureGL		();
};

//================================================================//
// MOAIShaderProgramGL
//================================================================//
/**	@lua	MOAIShaderProgramGL
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShaderGL instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class MOAIShaderProgramGL :
	public virtual MOAIGfxResourceGL,
	public virtual MOAIShaderUniformSchema {
protected:

	friend class MOAIShaderGL;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle		mProgram;
	ZLGfxHandle		mVertexShader;
	ZLGfxHandle		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniformDescriptor >	mUniformDescriptors;
	ZLLeanArray < MOAIShaderUniformBindingGL >	mUniformBindings;
	ZLLeanArray < MOAIShaderProgramGlobalGL >	mGlobals;
	ZLLeanArray < MOAIShaderProgramTextureGL >	mTextures;

	size_t										mUniformBufferSize;
	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
	static int					_declareUniform				( lua_State* L );
	static int					_load						( lua_State* L );
	static int					_reserveGlobals				( lua_State* L );
	static int					_reserveTextures			( lua_State* L );
	static int					_reserveUniforms			( lua_State* L );
	static int					_setGlobal					( lua_State* L );
	static int					_setTexture					( lua_State* L );
	static int					_setVertexAttribute			( lua_State* L );

	//----------------------------------------------------------------//
	void						AffirmUniforms				();
	void						ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void						BindUniforms				();
	ZLGfxHandle					CompileShader				( u32 type, cc8* source );
	void						InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
	int							ReserveGlobals 				( lua_State* L, int idx );
	void						SelectTextures				();
	void						UpdateUniforms				( ZLLeanArray < u8 >& buffer );
	
	//----------------------------------------------------------------//
	bool						MOAIGfxResource_OnCPUCreate							();
	void						MOAIGfxResource_OnCPUPurgeRecoverable				();
	void						MOAIGfxResourceGL_OnGPUBind							();
	bool						MOAIGfxResourceGL_OnGPUCreate						();
	void						MOAIGfxResourceGL_OnGPUDeleteOrDiscard				( bool shouldDelete );
	void						MOAIGfxResourceGL_OnGPUUnbind						();
	bool						MOAIGfxResourceGL_OnGPUUpdate						();
	void						MOAILuaObject_RegisterLuaClass						( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs						( MOAIComposer& composer, MOAILuaState& state );
	MOAIShaderUniformHandle		MOAIAbstractShaderUniformSchema_GetUniformHandle			( void* buffer, ZLIndex uniformID ) const;
	void						ZLGfxListener_OnUniformLocation						( u32 addr, void* userdata );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramGL )

	//----------------------------------------------------------------//
	void				Clear						();
	void				DeleteShaders				();
	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	void				Load						( cc8* vshSource, cc8* fshSource );
	void				ReserveAttributes			( ZLSize nAttributes );
	void				ReserveGlobals				( ZLSize nGlobals );
	void				ReserveTextures				( ZLSize nTextures );
	void				ReserveUniforms				( ZLSize nUniforms );
	void				SetGlobal					( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureGL* fallback );
	void				SetTexture					( ZLIndex idx, MOAITextureGL* texture, ZLIndex unit );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						MOAIShaderProgramGL			();
						~MOAIShaderProgramGL		();
};

#endif
