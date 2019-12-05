// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXDEVICE_H
#define ZLGFXDEVICE_H

#include <zl-gfx/ZLGfxEnum.h>

//================================================================//
// ZLGfxDevice
//================================================================//
class ZLGfxDevice {
public:

	//----------------------------------------------------------------//
	static u32				GetCap					( ZLGfxEnum::Type cap );
	static ZLGfxEnum::Type			GetError				();
	static cc8*				GetErrorString			( ZLGfxEnum::Type error );
	static cc8*				GetString				( ZLGfxEnum::Type stringID );
	static void				Initialize				();
};

#endif
