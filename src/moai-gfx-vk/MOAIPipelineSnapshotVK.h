// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINESNAPSHOTVK_H
#define MOAIPIPELINESNAPSHOTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIPipelineSnapshotVK
//================================================================//
class MOAIPipelineSnapshotVK :
	public MOAIAbstractSnapshotVK,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
protected:

	VkPipeline		mPipeline;

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		() {}

public:

	IMPLEMENT_FINALIZABLE ( MOAIPipelineSnapshotVK )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mPipeline != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkPipeline* () {
		return &this->mPipeline;
	}
	
	//----------------------------------------------------------------//
	operator VkPipeline& () {
		return this->mPipeline;
	}

	//----------------------------------------------------------------//
	void Initialize ( MOAILogicalDeviceVK& logicalDevice, VkRenderPass& renderPass, MOAIVertexFormatVK* vertexFormat, MOAIShaderVK* shader ) {
	
		this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );

		VkDynamicState dynamicStateEnables [] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
		};

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= MOAIGfxStructVK::pipelineInputAssemblyStateCreateInfo ();
		VkPipelineRasterizationStateCreateInfo rasterizationState 	= MOAIGfxStructVK::pipelineRasterizationStateCreateInfo ();
		VkPipelineColorBlendAttachmentState blendAttachmentState 	= MOAIGfxStructVK::pipelineColorBlendAttachmentState ();
		VkPipelineColorBlendStateCreateInfo colorBlendState 		= MOAIGfxStructVK::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used)
		VkPipelineViewportStateCreateInfo viewportState 			= MOAIGfxStructVK::pipelineViewportStateCreateInfo (); // overridden by dynamic state
		VkPipelineDepthStencilStateCreateInfo depthStencilState 	= MOAIGfxStructVK::pipelineDepthStencilStateCreateInfo ();
		VkPipelineMultisampleStateCreateInfo multisampleState 		= MOAIGfxStructVK::pipelineMultisampleStateCreateInfo ();
		VkPipelineDynamicStateCreateInfo dynamicState				= MOAIGfxStructVK::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = MOAIGfxStructVK::graphicsPipelineCreateInfo (
			NULL,
			0,
			NULL,
			&inputAssemblyState,
			NULL,
			&viewportState,
			&rasterizationState,
			&multisampleState,
			&depthStencilState,
			&colorBlendState,
			&dynamicState,
			NULL,
			renderPass
		);

		vertexFormat->UpdatePipelineCreateInfo ( pipelineCreateInfo );
		shader->GetProgram ()->UpdatePipelineCreateInfo ( pipelineCreateInfo );

		// Create rendering pipeline using the specified states
		VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->mPipeline ));
	}

	//----------------------------------------------------------------//
	MOAIPipelineSnapshotVK () :
		mPipeline ( VK_NULL_HANDLE ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIPipelineSnapshotVK () {
	
		this->FinalizeDependencies ();

		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyPipeline ( logicalDevice, this->mPipeline, NULL );
	}
};

#endif
