// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-jpg/host.h>
#include <moai-image-jpg/MOAIImageFormatJpg.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImageJpgContextInitialize () {

	ZLImageFormatMgr::Get ().RegisterFormat < MOAIImageFormatJpg >( "jpg" );
}
