// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIAbstractDescriptorElementVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetStateVK;

//================================================================//
// MOAIDescriptorSetKeyVK
//================================================================//
class MOAIDescriptorSetKeyVK {
public:

	const ZLLeanArray < MOAIAbstractDescriptorElementVK* >&		mSignature;

	//----------------------------------------------------------------//
	bool operator < ( const MOAIDescriptorSetKeyVK& other ) const {
	
		ZLSize sigSize = this->mSignature.BufferSize ();
		ZLSize otherSigSize = other.mSignature.BufferSize ();
		if ( sigSize == otherSigSize ) {
			return ( memcmp ( this->mSignature.GetBuffer (), other.mSignature.GetBuffer (), sigSize ) < 0 );
		}
		return ( sigSize < otherSigSize );
	}

	//----------------------------------------------------------------//
	MOAIDescriptorSetKeyVK ( const ZLLeanArray < MOAIAbstractDescriptorElementVK* >& signature ) :
		mSignature ( signature ) {
	}
};

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public MOAIAbstractSnapshotVK,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK > {
private:

	friend class MOAIAbstractDescriptorElementVK;
	friend class MOAIDescriptorSetKeyVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetStateVK;

	MOAIDescriptorSetKeyVK								mKey;
	ZLLeanArray < MOAIAbstractDescriptorElementVK* >	mSignature;
	VkDescriptorSet										mDescriptorSet;
	bool												mIsValid;

	//----------------------------------------------------------------//
	void			Invalidate							();

	//----------------------------------------------------------------//
	void			MOAIAbstractSnapshotVK_OnPin		( MOAICommandBufferVK& commandBuffer );
	void			MOAIAbstractSnapshotVK_OnUnpin		();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetVK )

	IS ( Valid, mIsValid, true )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mDescriptorSet != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSet* () {
		return &this->mDescriptorSet;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSet& () {
		return this->mDescriptorSet;
	}
	
	//----------------------------------------------------------------//
	operator MOAIDescriptorSetKeyVK& () {
		return this->mKey;
	}
	
	//----------------------------------------------------------------//
					MOAIDescriptorSetVK					();
					~MOAIDescriptorSetVK				();
};

#endif
