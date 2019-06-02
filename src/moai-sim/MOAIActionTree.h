// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONTREE_H
#define	MOAIACTIONTREE_H

#include <moai-sim/MOAIAction.h>

// TODO: finish out for general purpose use

//================================================================//
// MOAIActionTree
//================================================================//
/**	@lua MOAIActionTree
	@text	Tree of MOAIAction objects. Formerly a singleton; not yet
			ready for general purpose use.
*/
class MOAIActionTree :
	public virtual MOAIAction {
private:

	static const u32 RESET_PASS	= 0xffffffff;

	bool mProfilingEnabled;
	bool mThreadInfoEnabled;

	MOAIAction* mRoot;

	//----------------------------------------------------------------//
	MOAIAction*			AffirmRoot					();
	void				SetRoot						( MOAIAction* root );

	//----------------------------------------------------------------//
	void				MOAIAction_DidLoseChild			( MOAIAction* child );
	MOAIAction*			MOAIAction_GetDefaultParent		();
	bool				MOAIAction_IsDone				();
	void				MOAIAction_Update				( double step );

protected:

	//----------------------------------------------------------------//
	static int			_getRoot				( lua_State* L );
	static int			_setProfilingEnabled	( lua_State* L );
	static int			_setRoot				( lua_State* L );
	static int			_setThreadInfoEnabled	( lua_State* L );

public:

	friend class MOAIAction;

	DECL_LUA_FACTORY ( MOAIActionTree )
	
	GET_SET ( bool, ProfilingEnabled, mProfilingEnabled )
	GET_SET ( bool, ThreadInfoEnabled, mThreadInfoEnabled )

	//----------------------------------------------------------------//
						MOAIActionTree			();
						~MOAIActionTree			();
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				SetDefaultParent		();
	void				SetDefaultParent		( MOAIAction* defaultParent );
	void				Update					( double step );
};

#endif
