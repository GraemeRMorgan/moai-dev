// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_RenderTreeGL.h>

//================================================================//
// MOAIGfxMgrGL_RenderTreeGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrGL_RenderTreeGL::MOAIGfxMgrGL_RenderTreeGL () {
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_RenderTreeGL::~MOAIGfxMgrGL_RenderTreeGL () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_RenderTreeGL::MOAIGfxMgr_RenderTree_Render () {

	MOAIGfxMgrGL& gfxMgr = MOAIGfxMgrGL::Get ();

	gfxMgr.ResetDrawingAPIs ();
	gfxMgr.Update ();

	ZLGfx* gfx = gfxMgr.SelectDrawingAPI ( MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE );
	if ( !gfx ) return;

	ZGL_COMMENT ( *gfx, "RENDER MGR RENDER" );

//	MOAIFrameBufferGL* frameBuffer = MOAICast < MOAIFrameBufferGL >( gfxMgr.GetDefaultFrameBuffer ());
//	assert ( frameBuffer );
//	frameBuffer->NeedsClear ( true );

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	this->mRenderBatch->Render ();
		
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	gfxMgr.FinishFrame ();
}
