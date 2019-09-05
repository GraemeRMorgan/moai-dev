// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIONETRIVK_H
#define MOAIONETRIVK_H

class MOAIGfxBufferVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITexture2DVK;

//================================================================//
// MOAIOneTriVK
//================================================================//
class MOAIOneTriVK :
	public virtual ZLRefCountedObject {
private:

	struct Vertex {
		ZLVec3D32	mPosition;
		u32			mRGBA32;
		ZLVec2D32 	mUV;
		
		Vertex ( float x, float y, float z, float r, float g, float b, float u, float v ) {
			this->mPosition.Init ( x, y, z );
			this->mRGBA32 = ZLColor::PackRGBA ( r, g, b, 1.0 );
			this->mUV.Init ( u, v );
		}
	};

	ZLStrongPtr < MOAIVertexFormatVK >		mVertexFormat;

	ZLStrongPtr < MOAIVertexBufferVK >		mVertices;
	ZLStrongPtr < MOAIIndexBufferVK >		mIndices;
	ZLStrongPtr < MOAIUniformBufferVK >		mUniforms;
	
	ZLStrongPtr < MOAIShaderProgramVK >		mShaderProgram;
	
	VkImage					mTextureImage;
	VkDeviceMemory			mTextureImageMemory;
	VkImageView				mTextureImageView;
	VkSampler				mTextureSampler;
	VkDescriptorImageInfo	mTextureDescriptor;
	
	uint32_t				mTotalIndices;
	VkDescriptorBufferInfo	mUniformsDescriptor;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	VkPipelineLayout		mPipelineLayout;
	VkPipeline				mPipeline;
	VkDescriptorPool		mDescriptorPool;
	VkDescriptorSetLayout	mDescriptorSetLayout;
	VkDescriptorSet			mDescriptorSet;

	//----------------------------------------------------------------//
	void			PreparePipeline				();
	void			PrepareTexture				();
	void			PrepareUniformBuffers		();
	void			PrepareVertices				( bool useStagingBuffers = true );
	void			SetupDescriptorPool			();
	void			SetupDescriptorSet			();
	void			SetupDescriptorSetLayout	();
	void			UpdateUniformBuffers		( u32 width, u32 height );

public:

	//----------------------------------------------------------------//
	void			Draw						( VkCommandBuffer& commandBuffer, u32 width, u32 height );
					MOAIOneTriVK				();
					~MOAIOneTriVK				();
};

#endif
