// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVERTEXARRAY_H
#define	ZLVERTEXARRAY_H

#include <zl-gfx/ZLAbstractGfxResource.h>
#include <zl-gfx/ZLGfxHandle.h>

class ZLVertexBuffer;
class ZLVertexFormat;

//================================================================//
// ZLVertexBufferWithFormat
//================================================================//
class ZLVertexBufferWithFormat {
public:

	ZLStrongPtr < ZLVertexBuffer >				mBuffer;
	ZLStrongPtr < ZLVertexFormat >				mFormat;

	//----------------------------------------------------------------//
				ZLVertexBufferWithFormat		();
				~ZLVertexBufferWithFormat		();
	void		SetBufferAndFormat				( ZLVertexBuffer* buffer, ZLVertexFormat* format );
	
};

//================================================================//
// MOAIVertexArray
//================================================================//
// TODO: doxygen
class ZLVertexArray :
	public ZLAbstractGfxResource {
protected:

	ZLLeanArray < ZLGfxHandle >						mVAOs; // vertex array objects to bind all the vertex and buffer state
	ZLIndex											mCurrentVAO;

	ZLLeanArray < ZLVertexBufferWithFormat >		mVertexBuffers;

	bool				mUseVAOs;

	//----------------------------------------------------------------//
	bool				AffirmVertexBuffers			( u32 idx );
	void				BindVertexArrayItems		();
	void				UnbindVertexArrayItems		();
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				(); // load or initialize any CPU-side resources required to create the GPU-side resource
	void				ZLAbstractGfxResource_OnCPUDestroy				(); // clear any CPU-side memory used by class
	void				ZLAbstractGfxResource_OnGPUBind					(); // select GPU-side resource on device for use
	bool				ZLAbstractGfxResource_OnGPUCreate				(); // create GPU-side resource
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete ); // delete or discard GPU resource
	void				ZLAbstractGfxResource_OnGPUUnbind				(); // unbind GPU-side resource
	bool				ZLAbstractGfxResource_OnGPUUpdate				();

public:
		
	IS ( UsingVAOs, mUseVAOs, true )
	
	//----------------------------------------------------------------//
	ZLVertexBuffer*		GetVertexBuffer				( ZLIndex idx );
	ZLVertexFormat*		GetVertexFormat				( ZLIndex idx );
	void				ReserveVAOs					( u32 total );
	void				ReserveVertexBuffers		( u32 total );
	void				SetVertexBuffer				( ZLIndex idx, ZLVertexBuffer* vtxBuffer, ZLVertexFormat* vtxFormat );
						ZLVertexArray				();
						~ZLVertexArray				();
};

#endif
