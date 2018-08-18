// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONCELL_H
#define	MOAIPARTITIONCELL_H

class MOAIPartition;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIPartitionHull;

//================================================================//
// MOAIPartitionCell
//================================================================//
class MOAIPartitionCell {
private:
	
	friend class MOAIPartition;
	friend class MOAIPartitionLevel;
	friend class MOAIPartitionHull;
	
	typedef ZLLeanList < MOAIPartitionHull* >::Iterator HullIt;
	ZLLeanList < MOAIPartitionHull* > mHulls;

	//----------------------------------------------------------------//
	void			Clear					();
	void			ExtractProps			( MOAIPartitionCell& cell, MOAIPartitionLevel* level );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, u32 interfaceMask, u32 queryMask );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLVec3D& point, u32 interfaceMask, u32 queryMask );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLRect& rect, u32 interfaceMask, u32 queryMask );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLBox& box, u32 interfaceMask, u32 queryMask );
	void			GatherHulls				( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLFrustum& frustum, u32 interfaceMask, u32 queryMask );
	void			InsertHull				( MOAIPartitionHull& hull );
	void			RemoveHull				( MOAIPartitionHull& hull );
	void			ScheduleProps			(); // schedule all props in cell for update
					
public:

	//----------------------------------------------------------------//
					MOAIPartitionCell		();
					~MOAIPartitionCell		();
};

#endif
