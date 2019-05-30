// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREGL_H
#define	MOAITEXTUREGL_H

#include <moai-gfx-gl/MOAIAbstractGfxResourceGL.h>

class ZLImage;

//================================================================//
// MOAITextureGL
//================================================================//
/**	@lua	MOAITextureGL
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureGL :
	public virtual MOAITexture,
	public virtual MOAIAbstractGfxResourceGL {
protected:

	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class ZLTextureFormat;

	// debug name for memory use reporting
	STLString			mDebugName;

	// GL texture
	ZLGfxHandle			mGLTexture;
	
	// size of the original texture
	u32					mWidth;
	u32					mHeight;

	// GL_LINEAR
	// GL_LINEAR_MIPMAP_LINEAR
	// GL_LINEAR_MIPMAP_NEAREST
	// GL_NEAREST
	// GL_NEAREST_MIPMAP_LINEAR
	// GL_NEAREST_MIPMAP_NEAREST
	int					mMinFilter;
	int					mMagFilter;
	
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	int					mWrap;
	
	int					mGLInternalFormat;
	int					mGLPixelType;
	
	size_t				mTextureSize;

	//----------------------------------------------------------------//
	static int			_getSize										( lua_State* L );
	static int			_release										( lua_State* L );
	static int			_setDebugName									( lua_State* L );
	static int			_setFilter										( lua_State* L );
	static int			_setWrap										( lua_State* L );

	//----------------------------------------------------------------//
	void				CleanupOnError									();
	bool				CreateTextureFromImage							( ZLImage& srcImage );
	void				SetGLTexture									( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );
	bool				ShouldGenerateMipmaps							();
	bool				UpdateTextureFromImage							( ZLImage& image, ZLIntRect rect );
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				();
	void				ZLAbstractGfxResource_OnCPUDestroy				();
	void				ZLAbstractGfxResource_OnGPUBind					();
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				ZLAbstractGfxResource_OnGPUUnbind				();
	bool				ZLAbstractGfxResource_OnGPUUpdate				();
	u32					MOAITexture_GetHeight							() const;
	u32					MOAITexture_GetWidth							() const;
	void				ZLGfxListener_OnGfxEvent						( u32 event, void* userdata );
	
public:
	
	GET_SET ( cc8*, DebugName, mDebugName );
	GET_CONST ( ZLGfxHandle&, GLTexture, mGLTexture );

	//----------------------------------------------------------------//
	static void			CheckFilterModes			( int min, int mag );
						MOAITextureGL				();
						~MOAITextureGL				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SetFilter					( int filter );
	void				SetFilter					( int min, int mag );
	void				SetWrap						( int wrap );
};

#endif
