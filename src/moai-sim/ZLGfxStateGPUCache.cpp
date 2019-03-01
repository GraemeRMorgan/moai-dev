// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/ZLFrameBuffer.h>
#include <moai-sim/ZLGfxStateCPUCache.h>
#include <moai-sim/ZLGfxStateGPUCache.h>
#include <moai-sim/ZLGfxStateVertexCache.h>
#include <moai-sim/ZLIndexBuffer.h>
#include <moai-sim/ZLTexture.h>
#include <moai-sim/ZLVertexArray.h>
#include <moai-sim/ZLVertexBuffer.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// ZLVertexBufferWithFormat
//================================================================//

//----------------------------------------------------------------//
ZLVertexBufferWithFormat::ZLVertexBufferWithFormat () :
	mBoundVtxBuffer ( 0 ),
	mIsBound ( false ) {
}

//----------------------------------------------------------------//
ZLVertexBufferWithFormat::~ZLVertexBufferWithFormat () {
}

//----------------------------------------------------------------//
void ZLVertexBufferWithFormat::SetBufferAndFormat ( MOAIVertexBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer = buffer;
	this->mFormat = format;
}

//================================================================//
// ZLGfxStateGPUCacheFrame
//================================================================//

//----------------------------------------------------------------//
ZLGfxStateGPUCacheFrame::ZLGfxStateGPUCacheFrame () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mScissorEnabled ( false ),
	mScissorRect ( ZLRect::EMPTY ),
	mViewRect ( ZLRect::EMPTY ) {
}

//----------------------------------------------------------------//
ZLGfxStateGPUCacheFrame::~ZLGfxStateGPUCacheFrame () {
}

//================================================================//
// ZLGfxStateGPUCache
//================================================================//

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::ApplyStateChange ( u32 stateID ) {
	
	switch ( stateID ) {

		case BLEND_MODE:
			
			this->FlushBlendMode ( this->mPendingState.mBlendEnabled, this->mPendingState.mBlendMode );
			break;
		
		case CULL_FUNC:
			
			this->FlushCullFunc ( this->mPendingState.mCullFunc );
			break;
		
		case DEPTH_MODE:
			
			this->FlushDepthFunc ( this->mPendingState.mDepthFunc );
			this->FlushDepthMask ( this->mPendingState.mDepthMask );
			break;

		case FRAME_BUFFER:
			
			this->FlushFrameBuffer ( this->mPendingState.mFrameBuffer );
			break;
		
		case INDEX_BUFFER:
		
			this->FlushIndexBuffer ( this->mPendingState.mIdxBuffer );
			break;
		
		case PEN_WIDTH:
		
			this->FlushPenWidth ( this->mPendingState.mPenWidth );
			break;
		
		case SCISSOR_RECT:
	
			this->FlushScissorRect ( this->mPendingState.mScissorEnabled, this->mPendingState.mScissorRect );
			break;
		
		case SHADER:
		
			this->FlushShader ( this->mPendingState.mShader );
			break;
		
		case VERTEX_ARRAY:
		
			this->FlushVertexArray ( this->mPendingState.mVtxArray );
			break;
		
		case VERTEX_BUFFER:
			
			this->mActiveState.mVtxFormat = this->mPendingState.mVtxFormat;
			this->FlushVertexBuffer ( this->mPendingState.mVtxBuffer );
			break;
		
		case VERTEX_FORMAT:
			
			this->FlushVertexFormat ( this->mPendingState.mVtxFormat );
			break;

		case VIEW_RECT:
		
			this->FlushViewRect ( this->mPendingState.mViewRect );
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::ApplyStateChanges () {

	if ( !this->mApplyingStateChanges ) {
	
		if ( this->mPendingState.mShader ) {
			this->mPendingState.mShader->ScheduleTextures ();
		}

		if ( this->mDirtyFlags || this->mTextureDirtyFlags ) {
			
			this->SuspendChanges ();
			this->mCurrentState = &this->mActiveState;
			
			u32 dirtyFlags = this->mDirtyFlags;
			this->mDirtyFlags = 0;
			
			DEBUG_LOG ( "APPLY STATE CHANGES\n" );

			for ( u32 i = 0; dirtyFlags; ++i ) {
				u32 mask = 1 << i;
				if ( dirtyFlags & mask ) {
					this->ApplyStateChange ( mask );
					dirtyFlags &= ~mask;
				}
			}
			
			u32 textureDirtyFlags = this->mTextureDirtyFlags;
			this->mTextureDirtyFlags = 0;
			
			for ( ZLIndex i = ZLIndexOp::ZERO; textureDirtyFlags; ++i ) {
				u32 mask = 1 << i;
				if ( textureDirtyFlags & mask ) {
					this->FlushTexture ( i, this->mPendingState.mTextureUnits [ i ]);
					textureDirtyFlags &= ~mask;
				}
			}
			
			this->mCurrentState = &this->mPendingState;
			this->ResumeChanges ();
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::BindVertexBufferWithFormat ( ZLVertexBufferWithFormat& bufferWithFormat, bool useVAOs ) {
	UNUSED ( useVAOs );

	ZLVertexBuffer* buffer = bufferWithFormat.mBuffer;
	MOAIVertexFormat* format = bufferWithFormat.mFormat;
	
	if ( buffer && format ) {
	
		assert ( !bufferWithFormat.mIsBound );
		assert (( useVAOs && buffer->IsUsingVBOs ()) || ( !useVAOs )); // buffer objects must use VBOs to work with VAOs
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( bufferForBind );
		buffer->Unbind ();
		
		bufferWithFormat.mBoundVtxBuffer = bufferForBind;
		bufferWithFormat.mIsBound = true;
	}
	else {
		assert ( false );
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::ClearSurface () {
	
	ZLGfxStateCPUCache& cpuCache = this->ZLAbstractGfxStateCache_GetGfxStateCacheCPU ();
	
	u32 clearFlags = cpuCache.GetClearFlags ();

	if ( clearFlags ) {
	
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
		// discard this if all buffers are to be cleared?
		// (may still need to draw if depth or color only)
		this->ZLAbstractGfxStateCache_GetGfxVertexCache ().FlushVertexCache ();
		
		this->ApplyStateChanges ();
	
		if ( clearFlags & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
		
			const ZLColorVec& clearColor = cpuCache.GetClearColor ();
		
			gfx.ClearColor (
				clearColor.mR,
				clearColor.mG,
				clearColor.mB,
				clearColor.mA
			);
		}
	
		if (( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) && !this->GetDepthMask ()) {
			gfx.DepthMask ( true );
			gfx.Clear ( clearFlags );
			gfx.DepthMask ( false );
		}
		else {
			gfx.Clear ( clearFlags );
		}
	}
}

//----------------------------------------------------------------//
size_t ZLGfxStateGPUCache::CountTextureUnits () {

	return this->mActiveState.mTextureUnits.Size ();
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::DrawPrims ( u32 primType, u32 base, u32 count ) {

	DEBUG_LOG ( "DRAW PRIMS: %d %d %d\n", primType, base, count );
	
	this->ApplyStateChanges ();

	MOAIShader* shader = this->mActiveState.mShader;

	if ( shader && ( this->mActiveState.mVtxBuffer || this->mActiveState.mVtxArray )) {
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLIndexBuffer* idxBuffer = this->mActiveState.mIdxBuffer;
		
		if ( idxBuffer ) {
		
			DEBUG_LOG ( "drawing prims with index and vertex buffer\n" );
			
			size_t indexSize = idxBuffer->GetIndexSize ();
			u32 indexType = indexSize == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT;
			gfx.DrawElements ( primType, count, indexType, this->mBoundIdxBuffer, base * indexSize );
		}
		else {
			DEBUG_LOG ( "drawing prims with vertex buffer\n" );
			gfx.DrawArrays ( primType, base, count );
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushBlendMode ( bool blendEnabled, const MOAIBlendMode& blendMode ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( blendEnabled ) {
	
		bool updateBlend = false;
	
		if ( !active.mBlendEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling blend\n" );
			gfx.Enable ( ZGL_PIPELINE_BLEND );
			active.mBlendEnabled = true;
			updateBlend = true;
		}
		
		if ( updateBlend || !active.mBlendMode.IsSame ( blendMode )) {
		
			this->GfxStateWillChange ();
			
			DEBUG_LOG ( "  updating blend mode\n" );
			active.mBlendMode = blendMode;
			gfx.BlendMode ( active.mBlendMode.mEquation );
			gfx.BlendFunc ( active.mBlendMode.mSourceFactor, active.mBlendMode.mDestFactor );
		}
	}
	else {
		
		if ( active.mBlendEnabled ) {
		
			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling blend\n" );
			MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_BLEND );
			active.mBlendEnabled = false;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushCullFunc ( int cullFunc ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( active.mCullFunc != cullFunc ) {
	
		this->GfxStateWillChange ();
	
		active.mCullFunc = cullFunc;
	
		if ( active.mCullFunc ) {
			DEBUG_LOG ( "  enabling/setting cull func\n" );
			gfx.Enable ( ZGL_PIPELINE_CULL );
			gfx.CullFace ( active.mCullFunc );
		}
		else {
			DEBUG_LOG ( "  disabling cull func\n" );
			gfx.Disable ( ZGL_PIPELINE_CULL );
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushDepthFunc ( int depthFunc ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( active.mDepthFunc != depthFunc ) {
	
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  setting depth func: %d\n", pending.mDepthFunc );
		
		active.mDepthFunc = depthFunc;
		
		if ( active.mDepthFunc ) {
			gfx.Enable ( ZGL_PIPELINE_DEPTH );
			gfx.DepthFunc ( active.mDepthFunc );
		}
		else {
			gfx.Disable ( ZGL_PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushDepthMask ( bool depthMask ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( active.mDepthMask != depthMask ) {
		
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  setting depth mask: %s\n", depthMask ? "true" : "false" );
		active.mDepthMask = depthMask;
		gfx.DepthMask ( active.mDepthMask );
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushFrameBuffer ( ZLFrameBuffer* frameBuffer ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
	if ( active.mFrameBuffer != frameBuffer ) {
		
		DEBUG_LOG ( "  binding frame buffer: %p\n", frameBuffer );
	
		this->GfxStateWillChange ();
	
		if ( frameBuffer ) {
			frameBuffer->AffirmBuffers ();
		}
	
		gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBuffer );
		active.mFrameBuffer = frameBuffer;
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushIndexBuffer ( ZLIndexBuffer* buffer ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
	
	if (( active.mIdxBuffer != buffer ) || ( this->mBoundIdxBuffer != bufferForBind )) {
	
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  binding index buffer: %p\n", buffer );
		
		if ( active.mIdxBuffer ) {
			active.mIdxBuffer->Unbind ();
		}
		
		active.mIdxBuffer = buffer;
		this->mBoundIdxBuffer = 0;
		
		if ( buffer ) {
			buffer->Bind ();
			this->mBoundIdxBuffer = bufferForBind;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushPenWidth ( float penWidth ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( active.mPenWidth != penWidth ) {
		
		DEBUG_LOG ( "  setting pen width: %f\n", penWidth );
	
		this->GfxStateWillChange ();
	
		active.mPenWidth = penWidth;
		gfx.LineWidth ( active.mPenWidth );
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushScissorRect ( bool scissorEnabled, ZLRect rect ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( scissorEnabled ) {
	
		bool updateRect = false;
		
		if ( !active.mScissorEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling scissor rect\n" );
			gfx.Enable ( ZGL_PIPELINE_SCISSOR );
			active.mScissorEnabled = true;
			updateRect = true;
		}
		
		if ( updateRect || !active.mScissorRect.IsEqual ( rect )) {
		
			this->GfxStateWillChange ();
			
			active.mScissorRect = rect;
		
			ZLRect rect = active.mFrameBuffer->WndRectToDevice ( rect );
		
			s32 x = ( s32 )active.mScissorRect.mXMin;
			s32 y = ( s32 )active.mScissorRect.mYMin;
			
			u32 w = ( u32 )( active.mScissorRect.Width () + 0.5f );
			u32 h = ( u32 )( active.mScissorRect.Height () + 0.5f );

			w = h == 0 ? 0 : w;
			h = w == 0 ? 0 : h;
			
			DEBUG_LOG ( "  update scissor rect: %d %d %d %d\n", x, y, w, h );
			
			gfx.Scissor ( x, y, w, h );
		}
	}
	else {
	
		if ( active.mScissorEnabled == true ) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling scissor rect\n" );
			MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_SCISSOR );
			active.mScissorEnabled = false;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushShader ( MOAIShader* shader ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	MOAIShaderProgram* program = shader ? shader->GetProgram () : 0;
	shader = program ? shader : 0;
	
	if ( shader ) {
		shader->UpdateUniforms ();
	}
	
	bool applyUniforms	= ( shader && shader->HasDirtyUniforms ());
	bool changeShader	= ( shader != this->mActiveState.mShader );

	applyUniforms = true;
	changeShader = true;

	if ( applyUniforms || changeShader ) {
	
		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding shader program: %p\n", program );
		
		if ( changeShader ) {
			
			if ( active.mShader ) {
				active.mShader->GetProgram ()->Unbind ();
			}
			if ( shader ) {
				program->Bind ();
			}
			active.mShader = shader;
		}
		
		if ( applyUniforms ) {
			shader->ApplyUniforms ();
			shader->BindUniforms ();
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushTexture ( ZLIndex textureUnit, ZLTexture* texture ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	texture = texture && texture->Affirm () ? texture : ( ZLTexture* )this->mDefaultTexture;
	ZLTexture* prevTexture = active.mTextureUnits [ textureUnit ];

	if ( prevTexture != texture ) {

		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding texture set: %p\n", textureSet );

		gfx.ActiveTexture ( textureUnit );

		if ( prevTexture ) {
			prevTexture->Unbind ();
		}
		
		active.mTextureUnits [ textureUnit ] = texture;
		
		if ( texture ) {
		
			DEBUG_LOG ( "    binding texture: %d %p\n", i, bindTexture );
			texture->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushVertexArray ( ZLVertexArray* vtxArray ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( active.mVtxArray != vtxArray ) {

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding vertex array: %p\n", vtxArray );

		if ( active.mVtxArray ) {
			active.mVtxArray->Unbind ();
		}
		
		active.mVtxArray = vtxArray;
		
		if ( vtxArray ) {
			
			this->FlushVertexBuffer ( 0 ); // force the unbind in case it hasn't happened yet
			vtxArray->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushVertexBuffer ( ZLVertexBuffer* buffer ) {

	assert ( this->mApplyingStateChanges );

	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;

	MOAIVertexFormat* format = active.mVtxFormat;

	if (( active.mVtxBuffer != buffer ) || ( this->mBoundVtxBuffer != bufferForBind )) {

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding vertex buffer: (%p)\n", buffer );

		if ( active.mVtxBuffer ) {
			assert ( active.mVtxFormat );
			active.mVtxFormat->Unbind ();
		}
		
		active.mVtxBuffer = 0;
		this->mBoundVtxBuffer = 0;
		
		if ( format && buffer ) {
			
			this->FlushVertexArray ( 0 ); // force the unbind in case it hasn't happened yet
			
			buffer->Bind ();
			format->Bind ( bufferForBind );
			buffer->Unbind ();
			
			active.mVtxBuffer = buffer;
			this->mBoundVtxBuffer = bufferForBind;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushVertexFormat ( MOAIVertexFormat* vtxFormat ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( active.mVtxFormat != vtxFormat ) {
		
		DEBUG_LOG ( "  binding vertex format: (%p)\n", vtxFormat );
	
		this->GfxStateWillChange ();
		
		if ( active.mVtxFormat && active.mVtxBuffer ) {
			active.mVtxFormat->Unbind ();
		}
		
		active.mVtxFormat = vtxFormat;
		active.mVtxBuffer = 0; // must be set in a later step
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::FlushViewRect ( ZLRect rect ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	if ( !active.mViewRect.IsEqual ( rect )) {
		
		this->GfxStateWillChange ();
	
		active.mViewRect = rect;

		ZLRect rect = this->mActiveState.mFrameBuffer->WndRectToDevice ( rect );

		s32 x = ( s32 )active.mViewRect.mXMin;
		s32 y = ( s32 )active.mViewRect.mYMin;
	
		u32 w = ( u32 )( active.mViewRect.Width () + 0.5f );
		u32 h = ( u32 )( active.mViewRect.Height () + 0.5f );
	
		DEBUG_LOG ( "  update view rect: %d %d %d %d\n", x, y, w, h );
	
		gfx.Viewport ( x, y, w, h );
	}
}

//----------------------------------------------------------------//
u32 ZLGfxStateGPUCache::GetBufferHeight () const {

	assert ( this->mCurrentState );
	const ZLFrameBuffer* frameBuffer = this->mCurrentState->mFrameBuffer;
	return frameBuffer ? frameBuffer->mBufferHeight : 0;
}

//----------------------------------------------------------------//
u32 ZLGfxStateGPUCache::GetBufferWidth () const {

	assert ( this->mCurrentState );
	const ZLFrameBuffer* frameBuffer = this->mCurrentState->mFrameBuffer;
	return frameBuffer ? frameBuffer->mBufferWidth : 0;
}

//----------------------------------------------------------------//
//float ZLGfxStateGPUCache::GetDeviceScale () {
//
//	assert ( this->mFrameBuffer );
//	return this->mFrameBuffer->mBufferScale;
//}

//----------------------------------------------------------------//
float ZLGfxStateGPUCache::GetViewHeight () const {

	assert ( this->mCurrentState );
	return this->mCurrentState->mViewRect.Height ();
}

//----------------------------------------------------------------//
//ZLQuad ZLGfxStateGPUCache::GetViewQuad () const {
//
//	ZLQuad quad;
//
//	ZLMatrix4x4 invMtx;
//	invMtx.Inverse ( this->GetViewProjMtx ());
//	
//	quad.mV [ 0 ].Init ( -1.0f, 1.0f );
//	quad.mV [ 1 ].Init ( 1.0f, 1.0f );
//	quad.mV [ 2 ].Init ( 1.0f, -1.0f );
//	quad.mV [ 3 ].Init ( -1.0f, -1.0f );
//	
//	invMtx.TransformQuad ( quad.mV );
//	return quad;
//}

//----------------------------------------------------------------//
//ZLRect ZLGfxStateGPUCache::GetViewRect () const {
//
//	return this->mViewRect;
//}

//----------------------------------------------------------------//
float ZLGfxStateGPUCache::GetViewWidth () const {

	assert ( this->mCurrentState );
	return this->mCurrentState->mViewRect.Width ();
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::GfxStateWillChange () {
		
	this->GetGfxVertexCache ().FlushVertexCache ();
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::InitTextureUnits ( size_t nTextureUnits ) {

	if ( MAX_TEXTURE_UNITS < nTextureUnits ) {
		ZLLog_Warning ( "Hardware textures units (%d) exceed Moai maximum supported texture units (%d)\n", nTextureUnits, MAX_TEXTURE_UNITS );
		nTextureUnits = MAX_TEXTURE_UNITS;
	}

	this->mPendingState.mTextureUnits.Grow ( nTextureUnits, 0 );
	this->mActiveState.mTextureUnits.Grow ( nTextureUnits, 0 );
	
	this->mMaxTextureUnits = nTextureUnits;
}

//----------------------------------------------------------------//
ZLGfxStateGPUCache::ZLGfxStateGPUCache () :
	mCurrentState ( 0 ),
	mDirtyFlags ( 0 ),
	mTextureDirtyFlags ( 0 ),
	mMaxTextureUnits ( 0 ),
	mApplyingStateChanges ( 0 ),
	mBoundIdxBuffer ( 0 ),
	mBoundVtxBuffer ( 0 ) {
	
	this->mCurrentState = &this->mPendingState;
}

//----------------------------------------------------------------//
ZLGfxStateGPUCache::~ZLGfxStateGPUCache () {
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::RecalculateDirtyFlags () {

	if ( this->mPendingState.mBlendEnabled ) {
		this->SetBlendMode ( this->mPendingState.mBlendMode );
	}
	else {
		this->SetBlendMode ();
	}
	
	this->SetCullFunc ( this->mPendingState.mCullFunc );
	this->SetDepthFunc ( this->mPendingState.mDepthFunc );
	this->SetDepthMask ( this->mPendingState.mDepthMask );
	this->SetFrameBuffer ( this->mPendingState.mFrameBuffer );
	this->SetIndexBuffer ( this->mPendingState.mIdxBuffer );
	this->SetPenWidth ( this->mPendingState.mPenWidth );
	
	if ( this->mPendingState.mScissorEnabled ) {
		this->SetScissorRect ( this->mPendingState.mScissorRect );
	}
	else {
		this->SetScissorRect ();
	}
	
	this->SetShader ( this->mPendingState.mShader );
	this->SetVertexArray ( this->mPendingState.mVtxArray );
	this->SetViewRect ( this->mPendingState.mViewRect );
	this->SetVertexBuffer ( this->mPendingState.mVtxBuffer );
	this->SetVertexFormat ( this->mPendingState.mVtxFormat );
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < MAX_TEXTURE_UNITS; ++i ) {
		this->SetTexture ( this->mPendingState.mTextureUnits [ i ], i );
	}
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::ResetState () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	ZLGfxStateGPUCacheFrame& pending = this->mPendingState;
	ZLGfxStateGPUCacheFrame& active = this->mActiveState;

	ZGL_COMMENT ( gfx, "GFX RESET STATE" );

	// reset the shader
	gfx.UseProgram ( ZLGfxResource::UNBIND );
	
	// turn off blending
	gfx.Disable ( ZGL_PIPELINE_BLEND );
	pending.mBlendEnabled = false;
	active.mBlendEnabled = false;
	
	// disable backface culling
	gfx.Disable ( ZGL_PIPELINE_CULL );
	pending.mCullFunc = 0;
	active.mCullFunc = 0;
	
	// disable depth test
	gfx.Disable ( ZGL_PIPELINE_DEPTH );
	pending.mDepthFunc = 0;
	active.mDepthFunc = 0;
	
	// disable depth write
	gfx.DepthMask ( false );
	pending.mDepthMask = false;
	active.mDepthMask = 0;
	
	// reset the pen width
	pending.mPenWidth = 1.0f;
	active.mPenWidth = 1.0f;
	gfx.LineWidth ( 1.0f );
	
	// reset the scissor rect
	pending.mScissorEnabled = false;
	active.mScissorEnabled = false;
	gfx.Disable ( ZGL_PIPELINE_SCISSOR );
	
	// TODO: seems like overkill
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mMaxTextureUnits; ++i ){
		gfx.ActiveTexture (( u32 )i );
		gfx.BindTexture ( ZLGfxResource::UNBIND );
		pending.mTextureUnits [ i ] = 0;
		active.mTextureUnits [ i ] = 0;
	}
	gfx.ActiveTexture ( 0 );
	
	if ( active.mVtxArray ) {
		active.mVtxArray->Unbind ();
	}
	
	pending.mShader			= 0;
	pending.mIdxBuffer		= 0;
	pending.mVtxArray		= 0;
	pending.mVtxBuffer		= 0;
	pending.mVtxFormat		= 0;
	
	active.mShader			= 0;
	active.mIdxBuffer		= 0;
	active.mVtxArray		= 0;
	active.mVtxBuffer		= 0;
	active.mVtxFormat		= 0;
	
	pending.mFrameBuffer = this->mDefaultFrameBuffer;
	active.mFrameBuffer = this->mDefaultFrameBuffer;
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, pending.mFrameBuffer->mGLFrameBuffer );
	
	this->mDirtyFlags = 0;
	this->mTextureDirtyFlags = 0;
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::RestoreGPUState ( const ZLGfxStateGPUCacheFrame& frame ) {

	this->mPendingState = frame;
	this->RecalculateDirtyFlags ();
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::ResumeChanges () {

	this->mApplyingStateChanges--;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetBlendMode () {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mBlendEnabled = false;
	this->mDirtyFlags = ( !this->mActiveState.mBlendEnabled ) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mBlendEnabled = true;
	this->mPendingState.mBlendMode = blendMode;
	this->mDirtyFlags = ( this->mActiveState.mBlendEnabled && ( this->mActiveState.mBlendMode.IsSame ( blendMode ))) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetCullFunc () {

	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetCullFunc ( int cullFunc ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mCullFunc = cullFunc;
	this->mDirtyFlags = ( this->mActiveState.mCullFunc == cullFunc ) ? ( this->mDirtyFlags & ~CULL_FUNC ) : ( this->mDirtyFlags | CULL_FUNC );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetDefaultFrameBuffer ( ZLFrameBuffer* frameBuffer ) {

	this->mDefaultFrameBuffer = frameBuffer;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetDefaultTexture ( ZLTexture* texture ) {

	this->mDefaultTexture = texture;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetDepthFunc () {

	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetDepthFunc ( int depthFunc ) {

	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mDepthFunc = depthFunc;
	this->mDirtyFlags = (( this->mActiveState.mDepthFunc == depthFunc ) && ( this->mActiveState.mDepthMask == this->mPendingState.mDepthMask )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetDepthMask ( bool depthMask ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState.mDepthMask = depthMask;
	this->mDirtyFlags = (( this->mActiveState.mDepthMask == depthMask ) && ( this->mActiveState.mDepthFunc == this->mPendingState.mDepthFunc )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetFrameBuffer ( ZLFrameBuffer* frameBuffer ) {

	assert ( !this->mApplyingStateChanges );

	frameBuffer = frameBuffer ? frameBuffer : ( ZLFrameBuffer* )this->mDefaultFrameBuffer;
	this->mPendingState.mFrameBuffer = frameBuffer;
	this->mDirtyFlags = ( this->mActiveState.mFrameBuffer == frameBuffer ) ? ( this->mDirtyFlags & ~FRAME_BUFFER ) : ( this->mDirtyFlags | FRAME_BUFFER );
	
	return true;
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetIndexBuffer ( ZLIndexBuffer* buffer ) {
	
	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mIdxBuffer = buffer;
	this->mDirtyFlags = ( this->mActiveState.mIdxBuffer == buffer ) ? ( this->mDirtyFlags & ~INDEX_BUFFER ) : ( this->mDirtyFlags | INDEX_BUFFER );

	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetPenWidth ( float penWidth ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState.mPenWidth = penWidth;
	this->mDirtyFlags = ( this->mActiveState.mPenWidth == penWidth ) ? ( this->mDirtyFlags & ~PEN_WIDTH ) : ( this->mDirtyFlags | PEN_WIDTH );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetScissorRect () {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState.mScissorEnabled = false;
	this->mDirtyFlags = ( !this->mActiveState.mScissorEnabled ) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mScissorEnabled = true;
	this->mPendingState.mScissorRect = rect;
	this->mDirtyFlags = ( this->mActiveState.mScissorEnabled && ( this->mActiveState.mScissorRect.IsEqual ( rect ))) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetShader ( MOAIShaderMgr::Preset preset ) {

	return this->SetShader ( MOAIShaderMgr::Get ().GetShader ( preset ));
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetShader ( MOAIShader* shader ) {

	assert ( !this->mApplyingStateChanges );
	
	MOAIShaderProgram* program = shader ? shader->GetProgram () : 0;
	shader = program ? shader : 0;
	
	this->mPendingState.mShader = shader;
	
	// shader dirty flag only ceared if both are null
	this->mDirtyFlags = ( !( shader || this->mActiveState.mShader )) ? ( this->mDirtyFlags & ~SHADER ) : ( this->mDirtyFlags | SHADER );
	
	return program ? program->IsReady () : true;
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetTexture ( ZLTexture* texture, ZLIndex textureUnit ) {

	assert ( !this->mApplyingStateChanges );

	u32 mask = 1 << textureUnit;
	this->mPendingState.mTextureUnits [ textureUnit ] = texture;
	if ( this->mActiveState.mTextureUnits [ textureUnit ] == texture ) {
		this->mTextureDirtyFlags = this->mTextureDirtyFlags & ~mask;
	}
	else {
		this->mTextureDirtyFlags = this->mTextureDirtyFlags | mask;
	}
	
	return true;
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetVertexArray ( ZLVertexArray* vtxArray ) {

	assert ( !this->mApplyingStateChanges );

	if ( vtxArray ) {
		this->mPendingState.mVtxBuffer = 0;
		this->mPendingState.mVtxFormat = 0;
		this->mDirtyFlags &= ~VERTEX_BUFFER;
	}

	this->mPendingState.mVtxArray = vtxArray;
	this->mDirtyFlags = ( this->mActiveState.mVtxArray == vtxArray ) ? ( this->mDirtyFlags & ~VERTEX_ARRAY ) : ( this->mDirtyFlags | VERTEX_ARRAY );

	return vtxArray ? vtxArray->IsReady () : true;
}

//----------------------------------------------------------------//
bool ZLGfxStateGPUCache::SetVertexBuffer ( ZLVertexBuffer* buffer ) {

	assert ( !this->mApplyingStateChanges );
	
	if ( buffer ) {
		this->mPendingState.mVtxArray = 0;
		this->mDirtyFlags &= ~VERTEX_ARRAY;
	}

	this->mPendingState.mVtxBuffer = buffer;
	this->mDirtyFlags = ( this->mActiveState.mVtxBuffer == buffer ) ? ( this->mDirtyFlags & ~VERTEX_BUFFER ) : ( this->mDirtyFlags | VERTEX_BUFFER );
	
	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetVertexFormat ( MOAIVertexFormatMgr::Preset preset ) {

	this->SetVertexFormat ( MOAIVertexFormatMgr::Get ().GetFormat ( preset ));
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetVertexFormat ( MOAIVertexFormat* format ) {

	assert ( !this->mApplyingStateChanges );

	this->mPendingState.mVtxFormat = format;
	this->mDirtyFlags = ( this->mActiveState.mVtxFormat == format ) ? ( this->mDirtyFlags & ~VERTEX_FORMAT ) : ( this->mDirtyFlags | VERTEX_FORMAT );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetViewRect () {

	float width = ( float )this->GetBufferWidth ();
	float height = ( float )this->GetBufferHeight ();

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SetViewRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	assert ( !this->mApplyingStateChanges );
	
	this->mPendingState.mViewRect = rect;
	this->mDirtyFlags = ( this->mActiveState.mViewRect.IsEqual ( rect )) ? ( this->mDirtyFlags & ~VIEW_RECT ) : ( this->mDirtyFlags | VIEW_RECT );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::StoreGPUState ( ZLGfxStateGPUCacheFrame& frame ) const {

	if ( frame.mTextureUnits.Size () < this->mMaxTextureUnits ) {
		frame.mTextureUnits.Grow ( this->mMaxTextureUnits, 0 );
	}
	frame = this->mPendingState;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::SuspendChanges () {

	this->mApplyingStateChanges++;
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::UnbindAll () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	ZGL_COMMENT ( gfx, "GFX UNBIND ALL" );

	this->SetFrameBuffer ();
	this->SetIndexBuffer ();
	this->SetShader ();
	this->SetTexture ();
	this->SetVertexArray ();
	this->SetVertexBuffer ();
	this->SetVertexFormat ();
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void ZLGfxStateGPUCache::UnbindVertexBufferWithFormat ( ZLVertexBufferWithFormat& bufferWithFormat ) {

	assert ( bufferWithFormat.mIsBound );
	assert ( bufferWithFormat.mBuffer && bufferWithFormat.mFormat );
		
	bufferWithFormat.mFormat->Unbind ();
	bufferWithFormat.mBoundVtxBuffer = 0;
		
	bufferWithFormat.mIsBound = false;
}
