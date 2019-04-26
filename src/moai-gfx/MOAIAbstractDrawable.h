// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWABLE_H
#define	MOAIABSTRACTDRAWABLE_H

//================================================================//
// MOAIAbstractDrawable
//================================================================//
class MOAIAbstractDrawable :
	public virtual RTTIBase {
private:

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractDrawable_Draw					( int subPrimID ) = 0;
	virtual void	MOAIAbstractDrawable_DrawDebug				( int subPrimID ) = 0;

public:

	//----------------------------------------------------------------//
	static void		Draw				( MOAILuaMemberRef& ref, bool debug = false );
	static void		Draw				( MOAILuaState& state, int idx, bool debug = false );
	void			Draw				( int subPrimID = ( int )-1 ) { MOAIAbstractDrawable_Draw ( subPrimID ); }
	void			DrawDebug			( int subPrimID = ( int )-1 ) { MOAIAbstractDrawable_DrawDebug ( subPrimID ); }
};

#endif
