// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGRIDSPACE_H
#define MOAIGRIDSPACE_H

#include <moai-sim/MOAITileFlags.h>

//================================================================//
// MOAICellCoord
//================================================================//
class MOAICellCoord :
	public ZLIntVec2D {
public:
	
	//----------------------------------------------------------------//
	u32			GetID				();
	bool		IsEqual				( MOAICellCoord cellCoord );
				MOAICellCoord		();
				~MOAICellCoord		();
	
	//----------------------------------------------------------------//
	inline MOAICellCoord ( int x, int y ) :
		ZLIntVec2D ( x, y ) {
	}
	
	//----------------------------------------------------------------//
	inline MOAICellCoord MakeOffset ( int x, int y ) {
		
		MOAICellCoord result;
		result.mX = this->mX + x;
		result.mY = this->mY + y;
		
		return result;
	}
};

//================================================================//
// MOAIGridSpace
//================================================================//
/**	@lua	MOAIGridSpace
	@text	Represents spatial configuration of a grid. The grid is made
			up of cells. Inside of each cell is a tile. The tile can be
			larger or smaller than the cell and also offset from the cell.
			By default, tiles are the same size of their cells and are
			no offset.

	@flag	TILE_X_FLIP
	@flag	TILE_Y_FLIP
	@flag	TILE_XY_FLIP
	@flag	TILE_HIDE

	@const	TILE_BOTTOM_CENTER
	@const	TILE_CENTER
	@const	TILE_LEFT_BOTTOM
	@const	TILE_LEFT_CENTER
	@const	TILE_LEFT_TOP
	@const	TILE_RIGHT_BOTTOM
	@const	TILE_RIGHT_CENTER
	@const	TILE_RIGHT_TOP
	@const	TILE_TOP_CENTER
	
	@const	SQUARE_SHAPE
	@const	DIAMOND_SHAPE
	@const	OBLIQUE_SHAPE
	@const	HEX_SHAPE
	@const	AXIAL_HEX_SHAPE
*/
class MOAIGridSpace :
	public virtual MOAILuaObject {
protected:

	real		mXOff;
	real		mYOff;

	real		mCellWidth;
	real		mCellHeight;
	
	real		mTileWidth;
	real		mTileHeight;

	int			mWidth;
	int			mHeight;

	u32			mShape;
	u32			mRepeat;

	//----------------------------------------------------------------//
	static int		_cellAddrToCoord	( lua_State* L );
	static int		_getCellAddr		( lua_State* L );
	static int		_getCellSize		( lua_State* L );
	static int		_getOffset			( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_getTileLoc			( lua_State* L );
	static int		_getTileSize		( lua_State* L );
	static int		_initAxialHexGrid	( lua_State* L );
	static int		_initDiamondGrid	( lua_State* L );
	static int		_initHexGrid		( lua_State* L );
	static int		_initObliqueGrid	( lua_State* L );
	static int		_initRectGrid		( lua_State* L );
	static int		_locToCellAddr		( lua_State* L );
	static int		_locToCoord			( lua_State* L );
	static int		_setRepeat			( lua_State* L );
	static int		_setShape			( lua_State* L );
	static int		_setSize			( lua_State* L );
	static int		_wrapCoord			( lua_State* L );

	//----------------------------------------------------------------//
	MOAICellCoord	GetAxialHexCellCoord		( real x, real y ) const;
	MOAICellCoord	GetHexCellCoord			( real x, real y, real a, real b ) const;
	MOAICellCoord	GetObliqueCellCoord		( real x, real y ) const;
	ZLVec2D			GetRectPoint			( real x, real y, real width, real height, u32 position ) const;
	virtual void	OnResize				();

public:
	
	DECL_LUA_FACTORY ( MOAIGridSpace )
	
	enum {
		TILE_LEFT_TOP,
		TILE_RIGHT_TOP,
		TILE_LEFT_BOTTOM,
		TILE_RIGHT_BOTTOM,
		
		TILE_LEFT_CENTER,
		TILE_RIGHT_CENTER,
		TILE_TOP_CENTER,
		TILE_BOTTOM_CENTER,
		
		TILE_CENTER,
	};
	
	static const u32 REPEAT_X			= 0x00000001;
	static const u32 REPEAT_Y			= 0x00000002;
	
	static const u32 STAGGER_FLAG		= 0x80000000;
	static const u32 STAGGER_MASK		= 0x80000000;
	static const u32 SHAPE_MASK			= 0x7FFFFFFF;
	
	static const u32 RECT_SHAPE			= 0x00000000;
	static const u32 DIAMOND_SHAPE		= 0x00000001 | STAGGER_FLAG;
	static const u32 OBLIQUE_SHAPE		= 0x00000002;
	static const u32 HEX_SHAPE			= 0x00000003 | STAGGER_FLAG;
	static const u32 AXIAL_HEX_SHAPE	= 0x00000004;
	
	GET_SET ( real, XOff, mXOff )
	GET_SET ( real, YOff, mYOff )
	
	GET_SET ( real, CellWidth, mCellWidth )
	GET_SET ( real, CellHeight, mCellHeight )
	
	GET_SET ( real, TileWidth, mTileWidth )
	GET_SET ( real, TileHeight, mTileHeight )
	
	GET_SET ( int, Width, mWidth )
	GET_SET ( int, Height, mHeight )
	
	GET_SET ( u32, Shape, mShape )
	GET_SET ( u32, Repeat, mRepeat )
	
	//----------------------------------------------------------------//
	ZLVec2D				CellToWorld				( MOAICellCoord cellCoord, ZLVec2D loc ) const;
	
	MOAICellCoord		Clamp					( MOAICellCoord cellCoord ) const;
	MOAICellCoord		ClampX					( MOAICellCoord cellCoord ) const;
	MOAICellCoord		ClampY					( MOAICellCoord cellCoord ) const;
	
	ZLRect				GetBounds				() const;
	ZLRect				GetBounds				( MOAICellCoord c0, MOAICellCoord c1 ) const;
	void				GetBoundsInRect			( ZLRect rect, MOAICellCoord& c0, MOAICellCoord& c1, ZLRect maxSize ) const;
	
	int					GetCellAddr				( MOAICellCoord cellCoord ) const;
	int					GetCellAddr				( int xCell, int yCell ) const;
	MOAICellCoord		GetCellCoord			( int cellAddr ) const;
	MOAICellCoord		GetCellCoord			( ZLVec2D loc ) const;
	MOAICellCoord		GetCellCoord			( real x, real y ) const;
	MOAICellCoord		GetCellCoord			( int xCell, int yCell ) const;
	
	ZLVec2D				GetCellPoint			( MOAICellCoord cellCoord, u32 position ) const;
	ZLRect				GetCellRect				( MOAICellCoord cellCoord ) const;
	
	ZLVec2D				GetTilePoint			( MOAICellCoord cellCoord, u32 position ) const;
	ZLRect				GetTileRect				( MOAICellCoord cellCoord ) const;
	
	int					GetTotalCells			() const;
	
	ZLVec2D				GridToWorld				( ZLVec2D loc ) const;
	void				Init					( int width, int height, real tileWidth, real tileHeight );
	bool				IsValidCoord			( MOAICellCoord cellCoord ) const;
						MOAIGridSpace			();
						~MOAIGridSpace			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	MOAICellCoord		WrapCellCoord			( int xCell, int yCell ) const;
	ZLVec2D				WorldToCell				( MOAICellCoord cellCoord, ZLVec2D loc ) const;
	ZLVec2D				WorldToGrid				( ZLVec2D loc ) const;
};

#endif
