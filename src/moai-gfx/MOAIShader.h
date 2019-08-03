// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

// uniform buffer - attach statically (like a texture)
// uniform buffer - assemble dynamically (from lights)
// attach to shader at binding point
// light - fragment of uniform buffer (?)
// light - complete uniform buffer (?)
// light - trigger multiple redraws (?)
// bind in render pass (for object)

// shader has an implicit light (which is a collection of uniforms)

//================================================================//
// MOAIShader
//================================================================//
class MOAIShader :
	public virtual MOAILuaObject {
	
	//----------------------------------------------------------------//
	virtual bool	MOAIShader_IsReady				() const = 0;
	virtual void	MOAIShader_SelectTextures		() = 0;
	virtual void	MOAIShader_UpdateUniforms		() = 0;
	
public:

	//----------------------------------------------------------------//
	bool			HasProgram				() const;
	bool			IsReady					() const;
					MOAIShader				();
					~MOAIShader				();
	void			SelectTextures			();
	void			UpdateUniforms			();
};

#endif
