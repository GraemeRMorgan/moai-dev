// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>

class MOAIColor;
class MOAITransformBase;

//================================================================//
// MOAIShaderUniformBase
//================================================================//
class MOAIShaderUniformBase {
protected:

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix
	static const size_t ELEMENT_SIZE = 4;

public:

	enum {
		UNIFORM_TYPE_FLOAT,
		UNIFORM_TYPE_INT,
	};

	enum {
		UNIFORM_WIDTH_VEC_2			= 2,
		UNIFORM_WIDTH_VEC_3			= 3,
		UNIFORM_WIDTH_VEC_4			= 4,
		UNIFORM_WIDTH_MATRIX_3X3	= 9,
		UNIFORM_WIDTH_MATRIX_4X4	= 16,
	};

	u32				mType;				// type of buffer
	u32				mWidth;				// number of elements (ELEMENT_SIZE bytes each)

	//----------------------------------------------------------------//
	size_t			GetSize							() const;
	bool			Init							( u32 type, u32 width = 1 );
					MOAIShaderUniformBase			();
					~MOAIShaderUniformBase			();
};

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform :
	public MOAIShaderUniformBase {
protected:

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	STLString		mName;
	u32				mCount;				// (default) size of array
	u32				mGPUBase;			// this is resolved when linking the shader
	size_t			mCPUOffset;			// offset in CPU buffer

public:

	//----------------------------------------------------------------//
	void			Bind					( const void* buffer ) const;
	size_t			GetSize					() const;
	void			Init					( u32 type, u32 width = 1, u32 count = 1 );
					MOAIShaderUniform		();
					~MOAIShaderUniform		();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
