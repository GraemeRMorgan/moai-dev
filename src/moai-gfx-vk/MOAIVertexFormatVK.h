// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATVK_H
#define	MOAIVERTEXFORMATVK_H

//================================================================//
// MOAIVertexFormatVK
//================================================================//
/**	@lua	MOAIVertexFormatVK
	@text	Vertex format class.
*/
class MOAIVertexFormatVK :
	public virtual MOAIVertexFormat {
protected:

	ZLLeanArray < VkVertexInputAttributeDescription >	mVertexInputAttributeDescriptionsVK;
	VkVertexInputBindingDescription						mVertexInputBindingDescriptionVK;
	VkPipelineVertexInputStateCreateInfo				mPipelineVertexInputStateCreateInfoVK;

	//----------------------------------------------------------------//
	static VkFormat		GuessFormat							( const MOAIVertexAttribute& attribute );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormatVK )
		
	//----------------------------------------------------------------//
						MOAIVertexFormatVK				();
						~MOAIVertexFormatVK				();
	void				UpdatePipelineCreateInfo		( VkGraphicsPipelineCreateInfo& info );
};

#endif
