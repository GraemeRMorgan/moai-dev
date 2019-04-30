// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-png/host.h>
#include <moai-image-png/MOAIImageFormatPng.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImagePngContextInitialize () {

	ZLImageFormatMgr::Get().RegisterFormat < MOAIImageFormatPng >( "png" );
}
