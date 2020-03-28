// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIONETRIVK_H
#define MOAIONETRIVK_H

#include <moai-gfx-vk/MOAIGfxBufferVK.h>

class MOAICommandBufferVK;
class MOAIPipelineInputChunkSchemaVK;
class MOAIPipelineInputChunkVK;
class MOAIPipelineInputBodySchemaVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITexture2DVK;
class MOAIGfxBufferSnapshotVK;

//================================================================//
// MOAIOneTriVK
//================================================================//
class MOAIOneTriVK :
	public virtual MOAIAbstractDrawable {
private:

	struct Vertex {
		ZLVec4D32	mPosition;
		ZLVec2D32 	mUV;
		u32			mRGBA32;
		
		Vertex ( float x, float y, float z, float r, float g, float b, float u, float v ) {
			this->mPosition.Init ( x, y, z, 1.0 );
			this->mUV.Init ( u, v );
			this->mRGBA32 = ZLColor::PackRGBA ( r, g, b, 1.0 );
		}
	};

	ZLStrongPtr < MOAIGfxBufferSnapshotVK >		mVertices;
	ZLStrongPtr < MOAIGfxBufferSnapshotVK >		mIndices;
	ZLStrongPtr < MOAIUniformBufferVK >		mUniforms;
	
	VkImage					mTextureImage;
	VkDeviceMemory			mTextureImageMemory;
	VkImageView				mTextureImageView;
	VkSampler				mTextureSampler;
	VkDescriptorImageInfo	mTextureDescriptor;
	
	uint32_t				mTotalIndices;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	ZLStrongPtr < MOAIPipelineInputBodySchemaVK >	mPipelineLayout;
	ZLStrongPtr < MOAIPipelineInputChunkVK >		mDescriptorSet;
	VkPipeline										mPipeline;

	//----------------------------------------------------------------//
	void			PreparePipeline				();
	void			PrepareTexture				();
	void			PrepareVertices				( bool useStagingBuffers = true );
	void			UpdateMatrices				( u32 width, u32 height );

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw			( int subPrimID );
	void			MOAIDrawable_DrawDebug		( int subPrimID );

public:

	//----------------------------------------------------------------//
					MOAIOneTriVK				();
					~MOAIOneTriVK				();
};

#endif
