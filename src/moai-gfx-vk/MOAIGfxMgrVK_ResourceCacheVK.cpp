// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_ResourceCacheVK.h>

//================================================================//
// MOAIGfxMgrVK_ResourceCacheVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrVK_ResourceCacheVK::MOAIGfxMgrVK_ResourceCacheVK () {
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_ResourceCacheVK::~MOAIGfxMgrVK_ResourceCacheVK () {
}

//----------------------------------------------------------------//
ZLStrongPtr < MOAIDescriptorSetLayoutImplVK > MOAIGfxMgrVK_ResourceCacheVK::ProcureDescriptorSetLayoutImpl ( MOAIDescriptorSetLayoutKeyVK key ) {

	if ( this->mDescriptorLayoutImplCache.contains ( key )) {
		return this->mDescriptorLayoutImplCache [ key ];
	}
	
	DescriptorSetLayoutImpl* descriptorLayoutImpl = new DescriptorSetLayoutImpl ( this->mDescriptorLayoutImplCache );
	descriptorLayoutImpl->Initialize (
		MOAIGfxMgrVK::Get ().GetLogicalDevice (),
		key.mSignature
	);
	
	this->mDescriptorLayoutImplCache [ *descriptorLayoutImpl ] = descriptorLayoutImpl;
	return descriptorLayoutImpl;
}
