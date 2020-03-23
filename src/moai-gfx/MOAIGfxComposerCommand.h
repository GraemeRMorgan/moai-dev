// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERSOMMAND_H
#define	MOAIGFXCOMPOSERSOMMAND_H

class MOAIAbstractGfxComposerCallable;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIGfxComposerCmdEnum
//================================================================//
struct MOAIGfxComposerCmdEnum {
	enum _ {
		NONE,
		
		DRAW_BOX,
		DRAW_ELLIPSE,
		DRAW_LINE,
		DRAW_POINT,
		DRAW_POLY,
		DRAW_QUAD,
		DRAW_RECT,
		DRAW_TRIANGLE,
		SET_PEN_COLOR,
		SET_PEN_WIDTH,
		SET_SHADER,
		SET_TEXTURE,
		SET_UNIFORM,
		
		// keep these last
		CALL, // keep this first of call commands
		CALL_FROM_SHADER,
	};
};

//================================================================//
// MOAIGfxComposerParam
//================================================================//
namespace MOAIGfxComposerParam {

	//----------------------------------------------------------------//
	struct DrawBox {
		ZLBox 				mBox;
		u32					mStyle;
	};

	//----------------------------------------------------------------//
	struct DrawEllipse {
		ZLVec3D 			mCenter;
		float 				mXRad;
		float 				mYRad;
		u32 				mSteps;
		u32 				mStyle;
	};

	//----------------------------------------------------------------//
	struct DrawLine {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
	};

	//----------------------------------------------------------------//
	struct DrawPoint {
		ZLVec3D 			mPoint;
	};

	//----------------------------------------------------------------//
	struct DrawQuad {
		ZLQuad 				mQuad;
		u32 				mStyle;
	};

	//----------------------------------------------------------------//
	struct DrawRect {
		ZLRect 				mRect;
		u32 				mStyle;
		u32 				mEdges;
	};

	//----------------------------------------------------------------//
	struct DrawRoundedRect {
		ZLRect 				mRect;
		u32 				mStyle;
		u32					mSteps;
		float 				mXRad;
		float 				mYRad;
		float 				mStroke;
		float				mOffset;
	};

	//----------------------------------------------------------------//
	struct DrawTriangle {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
		ZLVec3D 			mV2;
		u32 				mStyle;
	};

	//----------------------------------------------------------------//
	struct SetPenColor {
		u32 				mColor;
	};

	//----------------------------------------------------------------//
	struct SetPenWidth {
		float 				mWidth;
	};

	//----------------------------------------------------------------//
	struct SetShader {
		MOAIShader* 		mShader;
	};
	
	//----------------------------------------------------------------//
	struct SetTexture {
		MOAITexture* 		mTexture;
		ZLIndex 			mTextureUnit;
	};
	
	//----------------------------------------------------------------//
	struct SetUniform {
		ZLIndex				mPipelineGlobalID;
		ZLIndex				mTargetUniformID;
		ZLIndex				mTargetUniformIndex;
	};
};

//================================================================//
// MOAIGfxComposerCommand
//================================================================//
struct MOAIGfxComposerCommand {

	MOAIGfxComposerCmdEnum::_ 	mType;
	ZLSize 						mParamSize;
	
	//----------------------------------------------------------------//
	static void		Execute				( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ cmd, const void* rawParam );
	static void		ExecuteUniform		( const MOAIGfxComposerParam::SetUniform& param );
};

#endif
