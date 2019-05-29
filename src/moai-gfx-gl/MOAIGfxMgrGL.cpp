// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderMgrGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>
#include <moai-gfx-gl/MOAITextureBaseGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgrGL::_enablePipelineLogging ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	MOAIGfxMgrGL::Get ().EnablePipelineLogging ( state.GetValue < bool >( 1, false ));

	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBufferGL frameBuffer
*/
int MOAIGfxMgrGL::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().GetDefaultFrameBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxMgrGL::_getMaxTextureSize ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().mMaxTextureSize );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxMgrGL::_getMaxTextureUnits ( lua_State* L ) {

	lua_pushnumber ( L, ( double )MOAIGfxMgrGL::Get ().CountTextureUnits ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
 
	@out	number width
	@out	number height
*/
int MOAIGfxMgrGL::_getViewSize ( lua_State* L  ) {

	MOAIFrameBufferGL* frameBuffer = MOAICast < MOAIFrameBufferGL >( MOAIGfxMgrGL::Get ().GetCurrentFrameBuffer ());
	
	lua_pushnumber ( L, frameBuffer->GetBufferWidth ());
	lua_pushnumber ( L, frameBuffer->GetBufferHeight ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	purgeResources
	@text	Purges all resources older that a given age (in render cycles).
			If age is 0 then all resources are purged.
 
	@opt	number age		Default value is 0.
	@out	nil
*/
int MOAIGfxMgrGL::_purgeResources ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 age = state.GetValue < u32 >( 1, 0 );

	MOAIGfxMgrGL::Get ().PurgeResources ( age );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxMgrGL::_renewResources ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIGfxMgrGL::Get ().RenewResources ();
	
	return 0;
}

//================================================================//
// MOAIGfxMgrGL
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxMgrGL::Clear () {
//
//	this->MOAIGfxStateGPUCacheGL::Clear ();
//	this->MOAIAbstractVertexCache::Clear ();
//}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDevice::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	// renew resources in immediate mode
	this->SelectDrawingAPI ();
	
	MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
	
	// TODO: ZLGfx - does this need to be called after shader mgr setup>
	this->RenewResources ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::DetectFramebuffer () {
	
	MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
	this->SetFrameBuffer ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::FinishFrame () {

	this->FlushToGPU (); // TODO: need to do this here?
	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetNormToWndMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Wnd
	ZLMatrix4x4 normToWnd;
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Translate ( hWidth + wndRect.mXMin, hHeight + wndRect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
	worldToWnd.Append ( this->GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWndToNormMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Inv Wnd
	ZLMatrix4x4 wndToNorm;
	wndToNorm.Translate ( -hWidth - wndRect.mXMin, -hHeight - wndRect.mYMin, 0.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxMgrGL::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = this->GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgrGL::LogErrors () {

	// TODO: ZLGfx

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = ZLGfxDevice::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
//				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsInitialize () {

	this->AffirmBuffers ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::PopState () {

	assert ( this->mStateStackTop > 0 );
	
	this->FlushToGPU ();
	
	MOAIGfxStateFrameGL* frame = this->mStateStack [ --this->mStateStackTop ];
	
	this->RestoreCPUState ( *frame );
	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::PushState () {

	this->FlushToGPU ();

	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
	if ( !this->mStateStack [ this->mStateStackTop ]) {
		this->mStateStack [ this->mStateStackTop ] = new MOAIGfxStateFrameGL ();
	}
	
	MOAIGfxStateFrameGL* frame = this->mStateStack [ this->mStateStackTop++ ];
	
	this->StoreCPUState ( *frame );
	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
	
	state.SetField ( -1, "DRAWING_PIPELINE",	( u32 )MOAIGfxPipelineClerkGL::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",	( u32 )MOAIGfxPipelineClerkGL::LOADING_PIPELINE );

	luaL_Reg regTable [] = {
		{ "enablePipelineLogging",		_enablePipelineLogging },
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgrGL > },
		{ "getMaxTextureSize",			_getMaxTextureSize },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "purgeResources",				_purgeResources },
		{ "renewResources",				_renewResources },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgrGL > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ReportTextureAlloc ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage += size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ReportTextureFree ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage -= size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ResetDrawCount () {
	//this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::SetBufferScale ( float scale ) {

	MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ())->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::SetBufferSize ( u32 width, u32 height ) {

	MOAIFrameBufferGL* defaultFrameBuffer = MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ());
	assert ( defaultFrameBuffer );
	defaultFrameBuffer->SetBufferSize ( width, height );
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::MOAIGfxMgrGL () :
	mHasContext ( false ),
	mIsFramebufferSupported ( 0 ),
	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
		mIsOpenGLES ( true ),
	#else
		mIsOpenGLES ( false ),
	#endif
	mMajorVersion ( 0 ),
	mMinorVersion ( 0 ),
	mTextureMemoryUsage ( 0 ),
	mMaxTextureSize ( 0 ),
	mRenderCounter ( 0 ),
	mStateStackTop ( ZLIndexOp::ZERO ) {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIAbstractGfxMgr )
	RTTI_END
	
	this->SetDefaultFrameBuffer ( new MOAIFrameBufferGL ());
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::~MOAIGfxMgrGL () {

	this->Clear ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
		delete this->mStateStack [ i ];
	}
	
	this->SetDefaultFrameBuffer ( 0 );
	this->SetDefaultTexture ( 0 );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractShader* MOAIGfxMgrGL::MOAIAbstractGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

	MOAIShaderGL* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgrGL::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgrGL::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShaderGL >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIAbstractTexture* MOAIGfxMgrGL::MOAIAbstractGfxMgr_AffirmTexture ( MOAILuaState& state, int idx ) const {

	MOAITextureBaseGL* textureBase = 0;
	
	textureBase = state.GetLuaObject < MOAITextureBaseGL >( idx, false );
	if ( textureBase ) return textureBase;
	
	MOAITextureGL* texture = new MOAITextureGL ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL& MOAIGfxMgrGL::MOAIAbstractGfxMgrComponentsGL_GetGfxMgrGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxPipelineClerkGL& MOAIGfxMgrGL::MOAIAbstractGfxMgrComponentsGL_GetGfxPipelineClerkGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxResourceClerkGL& MOAIGfxMgrGL::MOAIAbstractGfxMgrComponentsGL_GetGfxResourceClerkGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxStateGPUCacheGL& MOAIGfxMgrGL::MOAIAbstractGfxMgrComponentsGL_GetGfxStateGPUCacheGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIVertexCacheGL& MOAIGfxMgrGL::MOAIAbstractGfxMgrComponentsGL_GetVertexCacheGL () {
	return *this;
}
