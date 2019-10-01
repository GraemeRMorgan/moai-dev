// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotSubjectVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public ZLRefCountedObject,
	public MOAIDescriptorSetLayoutClientVK,
	public MOAIAbstractSnapshotSubjectVK < MOAIDescriptorSetSnapshotVK, MOAIDescriptorSetSignatureVK >,
	public MOAIDescriptorSetSignatureVK {
private:

	friend class MOAIDescriptorSetLayoutVK;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*		GetWriteDescriptorSet 		( ZLIndex binding, ZLIndex arrayElement );

	//----------------------------------------------------------------//
	void											MOAIAbstractLifecycleClientVK_Finalize				();
	SnapshotCache*									MOAIAbstractSnapshotSubjectVK_GetCache				();
	const MOAIDescriptorSetSignatureVK*				MOAIAbstractSnapshotSubjectVK_GetSignature			() const;
	MOAIDescriptorSetSnapshotVK*					MOAIAbstractSnapshotSubjectVK_MakeSnapshot			();
	
public:
	
	//----------------------------------------------------------------//
	void			Initialize						( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
					MOAIDescriptorSetVK				();
					~MOAIDescriptorSetVK			();
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo );
};

#endif
