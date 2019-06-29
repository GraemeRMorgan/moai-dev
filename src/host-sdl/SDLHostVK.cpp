// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-core/host.h>
#include <moai-sdl/host.h>
#include <host-modules/aku_modules.h>

#include <SDL_vulkan.h>
#include "SDLHostVK.h"

//================================================================//
// aku callbacks
//================================================================//

//----------------------------------------------------------------//
void SDLHostVK::_AKUGfxVKFunc_CreateSurface ( VkInstance instance, VkSurfaceKHR& surface, void* userdata ) {
	SDLHostVK* self = ( SDLHostVK* )userdata;
	assert ( self );

	surface = NULL;
	SDL_Vulkan_CreateSurface ( self->mWindow, instance, &surface );
}

//================================================================//
// SDLHostVK
//================================================================//

//----------------------------------------------------------------//
SDLHostVK::SDLHostVK () {
}

//----------------------------------------------------------------//
SDLHostVK::~SDLHostVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void SDLHostVK::SDLAbstractHost_MainLoopDidFinish () {
}

//----------------------------------------------------------------//
void SDLHostVK::SDLAbstractHost_MainLoopDidStart () {
}

//----------------------------------------------------------------//
void SDLHostVK::SDLAbstractHost_OpenWindow ( const char* title, int width, int height ) {

	if ( !this->mWindow ) {
		
		this->GetDeviceToPixelScale ( this->mDeviceScaleX, this->mDeviceScaleY );
	
		this->mWindow = SDL_CreateWindow (
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			( int )( width / this->mDeviceScaleX ),
			( int )( height / this->mDeviceScaleY ),
			SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
		);

		AKUSetViewSize ( width, height );
		AKUSdlSetWindow ( this->mWindow );

		AKUGfxVKSetFunc_CreateSurface ( _AKUGfxVKFunc_CreateSurface, this );
		AKUDetectGfxContext ( width, height, false );

		// Enable keyboard text input.
		// According to the SDL documentation, this will open an on-screen keyboard on some platforms.
		// Currently we're using the SDL host for desktop platforms only, so this should not be a problem.
		SDL_StartTextInput ();
	}
	else {
		SDL_SetWindowSize ( this->mWindow, width, height );
	}
}

//----------------------------------------------------------------//
void SDLHostVK::SDLAbstractHost_Render () {

	AKURender ();
}
