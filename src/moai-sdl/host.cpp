// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sdl/host.h>
#include <moai-sdl/headers.h>

//================================================================//
// aku-sdl
//================================================================//

//----------------------------------------------------------------//
void AKUSdlAppFinalize () {
}

//----------------------------------------------------------------//
void AKUSdlAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSdlContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIDialogSDL )
}

//----------------------------------------------------------------//
void AKUSdlSetWindow ( SDL_Window* window ) {

	MOAIDialogSDL::Get ().SetWindow ( window ); // TODO: this is hacky; move to MOAIAppSDL or something
}
