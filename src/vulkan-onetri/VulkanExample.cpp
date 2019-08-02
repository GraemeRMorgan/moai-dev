/*
* Vulkan Example base class
*
* Copyright (C) 2016-2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "VulkanExample.h"

#include "TriangleFragSPIRV.h"
#include "TriangleVertSPIRV.h"

#define SHADER(str) #str

//================================================================//
// VulkanExample
//================================================================//

//----------------------------------------------------------------//
// Build separate command buffers for every framebuffer image
// Unlike in OpenGL all rendering commands are recorded once into command buffers that are then resubmitted to the queue
// This allows to generate work upfront and from multiple threads, one of the biggest advantages of Vulkan
void VulkanExample::buildCommandBuffers () {
	
	VkCommandBufferBeginInfo cmdBufInfo = VkStruct::commandBufferBeginInfo ();

	// Set clear values for all framebuffer attachments with loadOp set to clear
	// We use two attachments (color and depth) that are cleared at the start of the subpass and as such we need to set clear values for both
	VkClearValue clearValues [] = {
		VkStruct::clearValue ( VkStruct::clearColorValueF ( 0.0, 0.0, 0.2, 1.0 ), VkStruct::clearDepthStencilValue ( 0.0, 0 )),
		VkStruct::clearValue ( VkStruct::clearColorValueF ( 0.0, 0.0, 0.0, 0.0 ), VkStruct::clearDepthStencilValue ( 1.0, 0 )),
	};

	VkRenderPassBeginInfo renderPassBeginInfo = VkStruct::renderPassBeginInfo ( mRenderPass, NULL, VkStruct::rect2D ( mWidth, mHeight ), clearValues, 2 );

	for ( size_t i = 0; i < mDrawCmdBuffers.size (); ++i ) {
		VkCommandBuffer commandBuffer = mDrawCmdBuffers [ i ];

		vkBeginCommandBuffer ( commandBuffer, &cmdBufInfo );
		
		renderPassBeginInfo.framebuffer = mFrameBuffers [ i ];
		vkCmdBeginRenderPass ( commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );
		
		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptorSet, 0, nullptr );
		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline );

		VkDeviceSize offsets [] = { 0 };
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &mVertices.mBuffer, offsets );
		vkCmdBindIndexBuffer ( commandBuffer, mIndices.mBuffer, 0, VK_INDEX_TYPE_UINT32 );
		vkCmdDrawIndexed ( commandBuffer, this->mTotalIndices, 1, 0, 0, 1 );
		vkCmdEndRenderPass ( commandBuffer );

		// Update dynamic state
		VkViewport viewport 	= VkStruct::viewport (( float )mWidth, ( float )mHeight, 0.0, 1.0 );
		VkRect2D scissor 		= VkStruct::rect2D ( mWidth, mHeight );
		
		vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
		vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

		// Ending the render pass will add an implicit barrier transitioning the frame buffer color attachment to
		// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR for presenting it to the windowing system

		VK_CHECK_RESULT ( vkEndCommandBuffer ( commandBuffer ));
	}
}

//----------------------------------------------------------------//
void VulkanExample::preparePipelines () {
	
	// Create the graphics pipeline used in this example
	// Vulkan uses the concept of rendering pipelines to encapsulate fixed states, replacing OpenGL's complex state machine
	// A pipeline is then stored and hashed on the GPU making pipeline changes very fast
	// Note: There are still a few dynamic states that are not directly part of the pipeline (but the info that they are used is)

	VkDynamicState dynamicStateEnables [] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkVertexInputAttributeDescription vertexInputAttributes [] = {
		VkStruct::vertexInputAttributeDescription ( 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof ( Vertex, position )),
		VkStruct::vertexInputAttributeDescription ( 0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof ( Vertex, color )),
	};

	VkPipelineShaderStageCreateInfo shaderStages [] = {
		VkStruct::pipelineShaderStageCreateInfo (
			VK_SHADER_STAGE_VERTEX_BIT,
			vks::tools::loadShaderSPIRV ( triangleVertSPIRV, sizeof ( triangleVertSPIRV ), this->mDevice ),
			"main"
		),
		VkStruct::pipelineShaderStageCreateInfo (
			VK_SHADER_STAGE_FRAGMENT_BIT,
			vks::tools::loadShaderSPIRV ( triangleFragSPIRV, sizeof ( triangleFragSPIRV ), this->mDevice ),
			"main"
		),
	};
	
	assert ( shaderStages [ 0 ].module != VK_NULL_HANDLE );
	assert ( shaderStages [ 1 ].module != VK_NULL_HANDLE );

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= VkStruct::pipelineInputAssemblyStateCreateInfo ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE, 0 );
	VkPipelineRasterizationStateCreateInfo rasterizationState 	= VkStruct::pipelineRasterizationStateCreateInfo ();
	VkPipelineColorBlendAttachmentState blendAttachmentState 	= VkStruct::pipelineColorBlendAttachmentState ();
	VkPipelineColorBlendStateCreateInfo colorBlendState 		= VkStruct::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used
	VkPipelineViewportStateCreateInfo viewportState 			= VkStruct::pipelineViewportStateCreateInfo ( NULL, 1, NULL, 1 ); // overridden by dynamic state
	VkPipelineDepthStencilStateCreateInfo depthStencilState 	= VkStruct::pipelineDepthStencilStateCreateInfo ();
	VkPipelineMultisampleStateCreateInfo multisampleState 		= VkStruct::pipelineMultisampleStateCreateInfo ();
	VkVertexInputBindingDescription vertexInputBinding 			= VkStruct::vertexInputBindingDescription ( 0, sizeof ( Vertex ), VK_VERTEX_INPUT_RATE_VERTEX );
	VkPipelineDynamicStateCreateInfo dynamicState				= VkStruct::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );
	VkPipelineVertexInputStateCreateInfo vertexInputState 		= VkStruct::pipelineVertexInputStateCreateInfo ( &vertexInputBinding, 1, vertexInputAttributes, 2 );

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = VkStruct::graphicsPipelineCreateInfo (
		shaderStages,
		2,
		&vertexInputState,
		&inputAssemblyState,
		NULL,
		&viewportState,
		&rasterizationState,
		&multisampleState,
		&depthStencilState,
		&colorBlendState,
		&dynamicState,
		this->mPipelineLayout,
		this->mRenderPass
	);

	// Create rendering pipeline using the specified states
	VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( this->mDevice, this->mPipelineCache, 1, &pipelineCreateInfo, nullptr, &mPipeline ));

	// Shader modules are no longer needed once the graphics pipeline has been created
	vkDestroyShaderModule ( this->mDevice, shaderStages [ 0 ].module, nullptr );
	vkDestroyShaderModule ( this->mDevice, shaderStages [ 1 ].module, nullptr );
}

//----------------------------------------------------------------//
void VulkanExample::prepareUniformBuffers () {
	
	this->mUniforms.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, sizeof ( mMatrixUniforms ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT );
	this->mUniforms.bind ( this->mDevice );
	
	// Store information in the uniform's descriptor that is used by the descriptor set
	this->mUniformsDescriptor.buffer = this->mUniforms;
	this->mUniformsDescriptor.offset = 0;
	this->mUniformsDescriptor.range = sizeof ( mMatrixUniforms );

	updateUniformBuffers ();
}

//----------------------------------------------------------------//
// Prepare vertex and index buffers for an indexed triangle
// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
void VulkanExample::prepareVertices ( bool useStagingBuffers ) {
	
	// Setup vertices
	std::vector < Vertex > vertexBuffer;
	vertexBuffer.push_back ( Vertex (  1.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f ));
	vertexBuffer.push_back ( Vertex ( -1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f ));
	vertexBuffer.push_back ( Vertex (  0.0f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f ));
	uint32_t vertexBufferSize = static_cast < uint32_t >( vertexBuffer.size ()) * sizeof ( Vertex );

	// Setup indices
	std::vector < uint32_t > indexBuffer;
	indexBuffer.push_back ( 0 );
	indexBuffer.push_back ( 1 );
	indexBuffer.push_back ( 2 );
	this->mTotalIndices = static_cast < uint32_t >( indexBuffer.size ());
	uint32_t indexBufferSize = this->mTotalIndices * sizeof ( uint32_t );

	if ( useStagingBuffers ) {
		
		// Static data like vertex and index buffer should be stored on the device memory
		// for optimal (and fastest) access by the GPU

		BufferVK stageVerts;
		stageVerts.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		stageVerts.mapAndCopy ( this->mDevice, vertexBuffer.data (), vertexBufferSize );
		stageVerts.bind ( this->mDevice );

		this->mVertices.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		this->mVertices.bind ( this->mDevice );
		
		BufferVK stageIndices;
		stageIndices.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		stageIndices.mapAndCopy ( this->mDevice, indexBuffer.data (), indexBufferSize );
		stageIndices.bind ( this->mDevice );

		this->mIndices.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		this->mIndices.bind ( this->mDevice );

		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		VkCommandBuffer copyCmd = vks::tools::createCommandBuffer ( this->mDevice, this->mSwapChainQueueCommandPool );

		// Put buffer region copies into command buffer
		VkBufferCopy copyRegion = {};

		// Vertex buffer
		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer ( copyCmd, stageVerts, this->mVertices, 1, &copyRegion );
		
		// Index buffer
		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer ( copyCmd, stageIndices, this->mIndices, 1, &copyRegion );

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		vks::tools::flushAndFreeCommandBuffer ( this->mDevice, this->mQueue, copyCmd, this->mSwapChainQueueCommandPool );

		// Destroy staging buffers
		// Note: Staging buffer must not be deleted before the copies have been submitted and executed
		stageVerts.cleanup ( this->mDevice );
		stageIndices.cleanup ( this->mDevice );
	}
	else {
		
		// Don't use staging
		// Create host-visible buffers only and use these for rendering. This is not advised and will usually result in lower rendering performance

		// Vertex buffer
		this->mVertices.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		this->mVertices.mapAndCopy ( this->mDevice, vertexBuffer.data (), vertexBufferSize );
		this->mVertices.bind ( this->mDevice );
		
		this->mIndices.init ( this->mDevice, this->mPhysicalDeviceMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		this->mIndices.mapAndCopy ( this->mDevice, indexBuffer.data (), indexBufferSize );
		this->mIndices.bind ( this->mDevice );
	}
}

//----------------------------------------------------------------//
void VulkanExample::setupDescriptorPool () {

	// We need to tell the API the number of max. requested descriptors per type
	VkDescriptorPoolSize typeCounts [ 1 ];
	// This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts [ 0 ].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	typeCounts [ 0 ].descriptorCount = 1;
	// For additional types you need to add new entries in the type count list
	// E.g. for two combined image samplers :
	// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// typeCounts[1].descriptorCount = 2;

	// Create the global descriptor pool
	// All descriptors used in this example are allocated from this pool
	VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.pNext = nullptr;
	descriptorPoolInfo.poolSizeCount = 1;
	descriptorPoolInfo.pPoolSizes = typeCounts;
	// Set the max. number of descriptor sets that can be requested from this pool (requesting beyond this limit will result in an error)
	descriptorPoolInfo.maxSets = 1;

	VK_CHECK_RESULT(vkCreateDescriptorPool(this->mDevice, &descriptorPoolInfo, nullptr, &mDescriptorPool));
}

//----------------------------------------------------------------//
void VulkanExample::setupDescriptorSet () {

	// Allocate a new descriptor set from the global descriptor pool
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = mDescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &mDescriptorSetLayout;

	VK_CHECK_RESULT(vkAllocateDescriptorSets(this->mDevice, &allocInfo, &mDescriptorSet));

	// Update the descriptor set determining the shader binding points
	// For every binding point used in a shader there needs to be one
	// descriptor set matching that binding point

	VkWriteDescriptorSet writeDescriptorSet = {};

	// Binding 0 : Uniform buffer
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = mDescriptorSet;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pBufferInfo = &this->mUniformsDescriptor;
	// Binds this uniform buffer to binding point 0
	writeDescriptorSet.dstBinding = 0;

	vkUpdateDescriptorSets(this->mDevice, 1, &writeDescriptorSet, 0, nullptr);
}

//----------------------------------------------------------------//
void VulkanExample::setupDescriptorSetLayout () {

	// Setup layout of descriptors used in this example
	// Basically connects the different shader stages to descriptors for binding uniform buffers, image samplers, etc.
	// So every shader binding should map to one descriptor set layout binding

	// Binding 0: Uniform buffer (Vertex shader)
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
	descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorLayout.pNext = nullptr;
	descriptorLayout.bindingCount = 1;
	descriptorLayout.pBindings = &layoutBinding;

	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(this->mDevice, &descriptorLayout, nullptr, &mDescriptorSetLayout));

	// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
	// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
	VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
	pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pPipelineLayoutCreateInfo.pNext = nullptr;
	pPipelineLayoutCreateInfo.setLayoutCount = 1;
	pPipelineLayoutCreateInfo.pSetLayouts = &mDescriptorSetLayout;

	VK_CHECK_RESULT(vkCreatePipelineLayout(this->mDevice, &pPipelineLayoutCreateInfo, nullptr, &mPipelineLayout));
}

//----------------------------------------------------------------//
void VulkanExample::updateUniformBuffers () {

	float aspect		= ( float )mWidth / ( float )mHeight;
	float fovy			= 60.0 * 0.01745329251994329576923690768489; // D2R
	float tanHalfFovy	= tan ( fovy / 2 );

	float xs = 1 / ( aspect * tanHalfFovy );
	float ys = 1 / tanHalfFovy;
	float zn = 0.1;
	float zf = 256;

	float projectionMatrix [] = {
		xs, 0, 0, 0,
		0, ys, 0, 0,
		0, 0, zf / ( zn - zf ), -1,
		0, 0, -( zf * zn ) / ( zf - zn ), 0,
	};

	float viewMatrix [] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	float modelMatrix [] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -2.5, 1,
	};

	memcpy ( mMatrixUniforms.projectionMatrix, projectionMatrix, sizeof ( projectionMatrix ));
	memcpy ( mMatrixUniforms.viewMatrix, viewMatrix, sizeof ( viewMatrix ));
	memcpy ( mMatrixUniforms.modelMatrix, modelMatrix, sizeof ( modelMatrix ));
	
	this->mUniforms.mapAndCopy ( this->mDevice, &mMatrixUniforms, sizeof ( mMatrixUniforms ));
}

//----------------------------------------------------------------//
VulkanExample::VulkanExample ( VulkanHost& host, bool enableValidation, bool useVsync, uint32_t apiVersion ) :
	VulkanAbstractExample ( host,  "Vulkan Onetri", enableValidation, useVsync, apiVersion ) {

	prepareVertices ( USE_STAGING );
	prepareUniformBuffers ();
	setupDescriptorSetLayout ();
	preparePipelines ();
	setupDescriptorPool ();
	setupDescriptorSet ();
	buildCommandBuffers ();
}

//----------------------------------------------------------------//
VulkanExample::~VulkanExample () {
	
	vkDeviceWaitIdle ( mDevice );
	
	// Clean up used Vulkan resources
	// Note: Inherited destructor cleans up resources stored in base class
	vkDestroyPipeline ( this->mDevice, mPipeline, nullptr );

	vkDestroyPipelineLayout ( this->mDevice, mPipelineLayout, nullptr );
	vkDestroyDescriptorSetLayout ( this->mDevice, mDescriptorSetLayout, nullptr );
	vkDestroyDescriptorPool ( this->mDevice, mDescriptorPool, nullptr );

	this->mVertices.cleanup ( this->mDevice );
	this->mIndices.cleanup ( this->mDevice );
	this->mUniforms.cleanup ( this->mDevice );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void VulkanExample::VulkanAbstractExample_render () {
	
	// Get next image in the swap chain (back/front buffer)
	uint32_t currentBuffer = this->prepareFrame ();
	//VK_CHECK_RESULT ( mSwapChain.acquireNextImage ( semaphores.presentComplete, &currentBuffer ));

	// render goes here (if dynamic)

	this->submitFrame ( currentBuffer );

//	// Submit to the graphics queue passing a wait fence
//	VK_CHECK_RESULT ( vkQueueSubmit ( mQueue, 1, &submitInfo, mWaitFences [ currentBuffer ]));
//
//	// Present the current buffer to the swap chain
//	// Pass the semaphore signaled by the command buffer submission from the submit info as the wait semaphore for swap chain presentation
//	// This ensures that the image is not presented to the windowing system until all commands have been submitted
//	VK_CHECK_RESULT ( mSwapChain.queuePresent ( mQueue, currentBuffer, semaphores.renderComplete ));
}

//----------------------------------------------------------------//
void VulkanExample::VulkanAbstractExample_viewChanged ()
{
	this->buildCommandBuffers ();

	// This function is called by the base example class each time the view is changed by user input
	updateUniformBuffers ();
}
