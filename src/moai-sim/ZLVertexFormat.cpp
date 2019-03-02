// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// ZLVertexFormat
//================================================================//

//----------------------------------------------------------------//
ZLVertexFormat* ZLVertexFormat::AffirmVertexFormat ( MOAILuaState& state, int idx ) {

	ZLVertexFormat* format = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		format = MOAIVertexFormatMgr::Get ().GetFormat ( state.GetValue < u32 >( idx, MOAIVertexFormatMgr::UNKNOWN_FORMAT ));
	}
	else {
		format = state.GetLuaObject < ZLVertexFormat >( 2, true );
	}
	return format;
}

//----------------------------------------------------------------//
void ZLVertexFormat::Bind ( ZLSharedConstBuffer* buffer ) const {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
		
		const ZLVertexAttribute& attr = this->mAttributes [ i ];
		
		gfx.EnableVertexAttribArray ( attr.mIndex );
		gfx.VertexAttribPointer ( attr.mIndex, attr.mSize, attr.mType, attr.mNormalized, this->mVertexSize, buffer, attr.mOffset );
	}
}

//----------------------------------------------------------------//
void ZLVertexFormat::Clear () {

	for ( u32 i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		this->mAttributeIDsByUse [ i ].Clear ();
		this->mTotalAttributesByUse [ i ] = 0;
	}
	this->mAttributes.Clear ();
	this->mTotalAttributes = 0;
	this->mVertexSize = 0;
}

//----------------------------------------------------------------//
int ZLVertexFormat::Compare ( const void* v0, const void* v1, float componentEpsilon, float normEpsilon ) const {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {

		ZLVertexAttribute& attribute = this->mAttributes [ i ];
		
		float tolerance = attribute.mType == ZGL_TYPE_FLOAT ? componentEpsilon : 0.0f;
		
		const void* a0 = ( const void* )(( size_t )v0 + attribute.mOffset );
		const void* a1 = ( const void* )(( size_t )v1 + attribute.mOffset );
		
		ZLVec4D coord0 = ZLVertexFormat::UnpackCoord ( a0, attribute );
		ZLVec4D coord1 = ZLVertexFormat::UnpackCoord ( a1, attribute );
		
		if ( attribute.mUse == ATTRIBUTE_NORMAL ) {
		
			if ( coord0.ZLVec3D::Dot ( coord1 ) < ( 1.0f - normEpsilon )) {
			
				if ( coord0.mX != coord1.mX ) {
					return ( coord0.mX < coord1.mX ) ? -1 : 1;
				}
				
				if ( coord0.mY != coord1.mY ) {
					return ( coord0.mY < coord1.mY ) ? -1 : 1;
				}
				
				if ( coord0.mZ != coord1.mZ ) {
					return ( coord0.mZ < coord1.mZ ) ? -1 : 1;
				}
			}
			
			if ( ABS ( coord0.mW - coord1.mW ) > tolerance ) {
				return ( coord0.mW < coord1.mW ) ? -1 : 1;
			}
		}
		else {
		
			if ( ABS ( coord0.mX - coord1.mX ) > tolerance ) {
				return ( coord0.mX < coord1.mX ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mY - coord1.mY ) > tolerance ) {
				return ( coord0.mY < coord1.mY ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mZ - coord1.mZ ) > tolerance ) {
				return ( coord0.mZ < coord1.mZ ) ? -1 : 1;
			}
			
			if ( ABS ( coord0.mW - coord1.mW ) > tolerance ) {
				return ( coord0.mW < coord1.mW ) ? -1 : 1;
			}
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
bool ZLVertexFormat::ComputeAABB ( ZLBox& aabb, const void* buffer, size_t size ) const {

	ZLByteStream stream;
	stream.SetBuffer ( buffer, size, size );
	return this->ComputeAABB ( aabb, stream, size );
}

//----------------------------------------------------------------//
bool ZLVertexFormat::ComputeAABB ( ZLBox& aabb, ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( u32 )( size / this->mVertexSize ) : 0; // TODO: cast
	if ( !total ) return false;
	
	u32 nCoordsPerVert = this->mTotalAttributesByUse [ ATTRIBUTE_COORD ];
	if ( !nCoordsPerVert ) return false;
	
	size_t base = stream.GetCursor ();
	
	ZLVec4D coord = this->ReadCoord ( stream, ZLIndexCast ( 0 ));
	
	aabb.Init ( coord );
	aabb.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	if ( nCoordsPerVert > 0 ) {
	
		for ( ZLIndex i = ZLIndexOp::ZERO; i < total; ++i ) {
		
			this->SeekVertex ( stream, base, i );
		
			for ( ZLIndex j = ZLIndexOp::ZERO; j < nCoordsPerVert; ++j ) {
				coord = this->ReadCoord ( stream, j );
				aabb.Grow ( coord );
			}
		}
	}
	else {
	
		for ( ZLIndex i = ZLIndexOp::ZERO; i < total; ++i ) {
			this->SeekVertex ( stream, base, i );
			coord = this->ReadCoord ( stream, ZLIndexCast ( 0 ));
			aabb.Grow ( coord );
		}
	}
	this->SeekVertex ( stream, base, total );
	return true;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::CountAttributesByUse ( u32 useID ) const {

	return useID < TOTAL_ATTRIBUTE_TYPES ? this->mTotalAttributesByUse [ useID ] : 0;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::CountAttributeComponents ( ZLIndex attrIdx ) const {

	return attrIdx < this->mAttributes.Size () ? this->mAttributes [ attrIdx ].mSize : 0;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::CountAttributeComponents ( u32 useID, ZLIndex idx ) const {

	if (( useID < TOTAL_ATTRIBUTE_TYPES ) && ( idx < this->mAttributeIDsByUse [ useID ].Size ())) {
		return this->CountAttributeComponents( this->mAttributeIDsByUse [ useID ][ idx ]);
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::CountBones () const {

	u32 nIndices = this->CountComponentsByUse ( ATTRIBUTE_BONE_INDICES );
	u32 nWeights = this->CountComponentsByUse ( ATTRIBUTE_BONE_WEIGHTS );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::CountComponentsByUse ( u32 useID ) const {

	u32 nComponents = 0;
	u32 nAttributes = this->mTotalAttributesByUse [ useID ];
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nAttributes; ++i ) {
	
		ZLIndex attrIdx = this->mAttributeIDsByUse [ useID ][ i ];
		const ZLVertexAttribute& attr = this->mAttributes [ attrIdx ];
		nComponents += attr.mSize;
	}
	return nComponents;
}

//----------------------------------------------------------------//
void ZLVertexFormat::DeclareAttribute ( ZLIndex index, u32 type, u32 size, u32 use, bool normalized ) {

	ZLIndex attrIdx = ZLIndexCast ( this->mTotalAttributes++ );
	this->mAttributes.Grow ( this->mTotalAttributes );
	ZLVertexAttribute& attr = this->mAttributes [ attrIdx ];
	
	u32 offset = this->mVertexSize;
	
	attr.mIndex			= index == ZLIndexOp::INVALID ? attrIdx : index;
	attr.mSize			= size;
	attr.mType			= type;
	attr.mUse			= use;
	attr.mNormalized	= normalized;
	attr.mOffset		= offset;
	attr.mSizeInBytes	= ZLVertexFormat::GetComponentSize ( size, type );
	
	this->mVertexSize	+= attr.mSizeInBytes;
	
	if ( use < TOTAL_ATTRIBUTE_TYPES ) {
		
		ZLIndex idx = ZLIndexCast ( this->mTotalAttributesByUse [ use ]++ );
		
		this->mAttributeIDsByUse [ use ].Grow (( ZLSize )idx + 1 );
		this->mAttributeIDsByUse [ use ][ idx ] = attrIdx;
	}
}

//----------------------------------------------------------------//
const ZLVertexAttribute& ZLVertexFormat::GetAttribute ( ZLIndex attrIdx ) {

	assert ( attrIdx < this->mAttributes.Size ());
	return this->mAttributes [ attrIdx ];
}

//----------------------------------------------------------------//
const ZLVertexAttribute* ZLVertexFormat::GetAttributeByUse ( u32 useID, ZLIndex attrIndex ) const {

	if ( attrIndex < this->mTotalAttributesByUse [ useID ]) {
		return &this->mAttributes [ this->mAttributeIDsByUse [ useID ][ attrIndex ]];
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::GetComponentSize ( u32 size, u32 type ) {

	u32 bytes;
	switch ( type ) {
	
		case ZGL_TYPE_BYTE:
		case ZGL_TYPE_UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case ZGL_TYPE_SHORT:
		case ZGL_TYPE_UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case ZGL_FIXED:
		case ZGL_TYPE_FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return size * bytes;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::GetLuaIndexForUseID ( u32 useID ) {

	switch ( useID ) {
		case ATTRIBUTE_BONE_INDICES:	return 'i';
		case ATTRIBUTE_BONE_WEIGHTS:	return 'w';
		case ATTRIBUTE_BONE_COUNT:		return 'k';
		case ATTRIBUTE_COLOR:			return 'c';
		case ATTRIBUTE_COORD:			return 'x';
		case ATTRIBUTE_NORMAL:			return 'n';
		case ATTRIBUTE_TEX_COORD:		return 't';
		case ATTRIBUTE_USER:			return 'u';
	}
	
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::GetUseIDForLuaIndex ( u32 idx ) {

	switch ( idx ) {
		case 'i':						return ATTRIBUTE_BONE_INDICES;
		case 'w':						return ATTRIBUTE_BONE_WEIGHTS;
		case 'k':						return ATTRIBUTE_BONE_COUNT;
		case 'c':						return ATTRIBUTE_COLOR;
		case 'x':						return ATTRIBUTE_COORD;
		case 'n':						return ATTRIBUTE_NORMAL;
		case 't':						return ATTRIBUTE_TEX_COORD;
		case 'u':						return ATTRIBUTE_USER;
	}
	
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::PackAttribute ( void* buffer, const ZLVec4D& coord, const ZLVertexAttribute& attribute ) {

	u32 components = attribute.mSize;
	bool normalized = attribute.mNormalized;

	switch ( attribute.mType ) {
	
		case ZGL_TYPE_BYTE:
			return ZLVertexFormat::PackAttribute < s8 >( buffer, coord, components, normalized ? 127.0f : 1.0f );
		
		case ZGL_TYPE_FLOAT:
			return ZLVertexFormat::PackAttribute < float >( buffer, coord, components, 1.0f );
		
		case ZGL_TYPE_SHORT:
			return ZLVertexFormat::PackAttribute < s16 >( buffer, coord, components, normalized ? 32767.0f : 1.0f );
		
		case ZGL_TYPE_UNSIGNED_BYTE:
			return ZLVertexFormat::PackAttribute < u8 >( buffer, coord, components, normalized ? 255.0f : 1.0f );
		
		case ZGL_TYPE_UNSIGNED_SHORT:
			return ZLVertexFormat::PackAttribute < u16 >( buffer, coord, components, normalized ? 65535.0f : 1.0f );
	}
	
	return 0;
}

//----------------------------------------------------------------//
void ZLVertexFormat::PrintVertices ( ZLStream& stream ) const {

	this->PrintVertices ( stream, stream.GetLength () - stream.GetCursor ());
}

//----------------------------------------------------------------//
void ZLVertexFormat::PrintVertices ( ZLStream& stream, size_t size ) const {

	u32 total = this->mVertexSize ? ( u32 )( size / this->mVertexSize ) : 0; // TODO: cast
	if ( !total ) return;

	for ( ZLIndex i = ZLIndexOp::ZERO; i < total; ++i ) {
	
		printf ( "vertex %d:\n", ( int )ZLSize ( i ));
		
		for ( ZLIndex j = ZLIndexOp::ZERO; j < this->mTotalAttributes; ++j ) {
			ZLVertexAttribute& attribute = this->mAttributes [ j ];
			
			printf ( "  %d: ", ( int )ZLSize ( j ));
			
			for ( u32 k = 0; k < attribute.mSize; ++k ) {
			
				if ( k ) {
					printf ( ", " );
				}
			
				switch ( attribute.mType  ) {
				
					case ZGL_TYPE_BYTE:
						printf ( "%d", ( int )stream.Read < s8 >( 0 ));
						break;
					
					case ZGL_TYPE_UNSIGNED_BYTE:
						printf ( "%d", ( int )stream.Read < u8 >( 0 ));
						break;
					
					case ZGL_TYPE_SHORT:
						printf ( "%d", ( int )stream.Read < s16 >( 0 ));
						break;
					
					case ZGL_TYPE_UNSIGNED_SHORT:
						printf ( "%d", ( int )stream.Read < u16 >( 0 ));
						break;
					
					case ZGL_TYPE_FLOAT:
						printf ( "%f", ( float )stream.Read < float >( 0 ));
						break;
				}
			}
			printf ( "\n" );
		}
	}
}

//----------------------------------------------------------------//
void ZLVertexFormat::PrintVertices ( const void* buffer, size_t size ) const {

	ZLByteStream stream;
	stream.SetBuffer ( buffer, size, size );
	this->PrintVertices ( stream, size );
}

//----------------------------------------------------------------//
ZLVec4D ZLVertexFormat::ReadAttribute ( ZLStream& stream, ZLIndex attrIdx, float yFallback, float zFallback, float wFallback ) const {

	if ( attrIdx < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const ZLVertexAttribute& attribute = this->mAttributes [ attrIdx ];
		
		size_t base = stream.GetCursor ();
		stream.SetCursor ( base + attribute.mOffset );
		stream.ReadBytes ( buffer, attribute.mSizeInBytes );
		stream.SetCursor ( base );
		
		return this->UnpackAttribute ( buffer, attribute, yFallback, zFallback, wFallback );
	}
	return ZLVec4D ( 0.0f, yFallback, zFallback, wFallback );
}

//----------------------------------------------------------------//
ZLVec4D ZLVertexFormat::ReadAttribute ( ZLStream& stream, u32 useID, ZLIndex idx, float yFallback, float zFallback, float wFallback ) const {

	if ( idx < this->mTotalAttributesByUse [ useID ]) {
		return this->ReadAttribute ( stream, this->mAttributeIDsByUse [ useID ][ idx ], yFallback, zFallback, wFallback);
	}
	return ZLVec4D ( 0.0f, yFallback, zFallback, wFallback );
}

//----------------------------------------------------------------//
u32 ZLVertexFormat::ReadBoneCount ( ZLStream& stream, ZLIndex idx ) const {

	ZLVec4D boneCount = this->ReadAttribute ( stream, ATTRIBUTE_BONE_COUNT, idx, 0.0f, 0.0f, 0.0f );
	return ( u32 )boneCount.mX;
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::ReadBones ( ZLStream& stream, float* indices, float* weights, size_t size ) const {
	
	size_t nIndices = this->ReadComponents ( stream, ATTRIBUTE_BONE_INDICES, indices, size );
	size_t nWeights = this->ReadComponents ( stream, ATTRIBUTE_BONE_WEIGHTS, weights, size );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
ZLColorVec ZLVertexFormat::ReadColor ( ZLStream& stream, ZLIndex idx ) const {

	ZLVec4D color = this->ReadAttribute ( stream, ATTRIBUTE_COLOR, idx, 0.0f, 0.0f, 1.0f );
	return ZLColorVec ( color.mX, color.mY, color.mZ, color.mW );
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::ReadComponents ( ZLStream& stream, u32 useID, float* components, size_t size ) const {

	size_t remaining = size;
	size_t n = this->mTotalAttributesByUse [ useID ];
	
	for ( ZLIndex i = ZLIndexOp::ZERO; ( i < n ) && remaining; ++i ) {
	
		const ZLVertexAttribute& attribute = this->mAttributes [ this->mAttributeIDsByUse [ useID ][ i ]];
		
		size_t attrSize = attribute.mSize;
		
		ZLVec4D vector = this->ReadAttribute ( stream, useID, i, 0.0f, 0.0f, 0.0f );
		
		memcpy ( components, &vector, attrSize * sizeof ( float ));
		
		remaining = remaining - attrSize;
		components = &components [ attrSize ];
	}
	
	return size - remaining;
}

//----------------------------------------------------------------//
ZLVec4D ZLVertexFormat::ReadCoord ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_COORD, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D ZLVertexFormat::ReadNormal ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_NORMAL, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
ZLVec3D ZLVertexFormat::ReadUV ( ZLStream& stream, ZLIndex idx ) const {

	return this->ReadAttribute ( stream, ATTRIBUTE_TEX_COORD, idx, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::SeekVertex ( ZLStream& stream, size_t base, size_t vertex ) const {

	stream.SetCursor ( base + ( vertex * this->mVertexSize ));
	return stream.GetCursor ();
}

//----------------------------------------------------------------//
void ZLVertexFormat::Unbind () const {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
		
		ZLVertexAttribute& attr = this->mAttributes [ i ];
		MOAIGfxMgr::GetDrawingAPI ().DisableVertexAttribArray ( attr.mIndex );
	}
}

//----------------------------------------------------------------//
ZLVec4D ZLVertexFormat::UnpackAttribute ( const void* buffer, const ZLVertexAttribute& attribute, float yFallback, float zFallback, float wFallback ) {

	u32 components = attribute.mSize;
	bool normalized = attribute.mNormalized;

	switch ( attribute.mType ) {
	
		case ZGL_TYPE_BYTE:
			return ZLVertexFormat::UnpackAttribute < s8 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 127.0f : 1.0f );
		
		case ZGL_TYPE_FLOAT:
			return ZLVertexFormat::UnpackAttribute < float >( buffer, components, yFallback, zFallback, wFallback, 1.0f );
		
		case ZGL_TYPE_SHORT:
			return ZLVertexFormat::UnpackAttribute < s16 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 32767.0f : 1.0f );
		
		case ZGL_TYPE_UNSIGNED_BYTE:
			return ZLVertexFormat::UnpackAttribute < u8 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 255.0f : 1.0f );
		
		case ZGL_TYPE_UNSIGNED_SHORT:
			return ZLVertexFormat::UnpackAttribute < u16 >( buffer, components, yFallback, zFallback, wFallback, normalized ? 65535.0f : 1.0f );
	}

	return ZLVec4D ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
ZLVec4D ZLVertexFormat::UnpackCoord ( const void* buffer, const ZLVertexAttribute& attribute ) {
	
	return ZLVertexFormat::UnpackAttribute ( buffer, attribute, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteAhead ( ZLStream& stream ) const {

	size_t base = stream.GetCursor ();

	void* buffer = alloca ( this->mVertexSize );
	memset ( buffer, 0, this->mVertexSize );
	
	stream.WriteBytes ( buffer, this->mVertexSize );
	stream.SetCursor ( base );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteAttribute ( ZLStream& stream, ZLIndex attrIdx, float x, float y, float z, float w ) const {

	if ( attrIdx < this->mTotalAttributes ) {
	
		u8 buffer [ MAX_ATTR_BYTES ];
		
		const ZLVertexAttribute& attribute = this->mAttributes [ attrIdx ];
		
		size_t size = this->PackAttribute ( buffer, ZLVec4D ( x, y, z, w ), attribute );
		
		if ( size ) {
			size_t base = stream.GetCursor ();
			stream.SetCursor ( base + attribute.mOffset );
			stream.WriteBytes ( buffer, size );
			stream.SetCursor ( base );
		}
	}
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteAttribute ( ZLStream& stream, u32 useID, ZLIndex idx, float x, float y, float z, float w ) const {

	if ( idx < this->mTotalAttributesByUse [ useID ]) {
		this->WriteAttribute ( stream, this->mAttributeIDsByUse [ useID ][ idx ], x, y, z, w );
	}
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteBoneCount ( ZLStream& stream, ZLIndex idx, u32 count ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_BONE_COUNT, idx, ( float )count, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::WriteBones ( ZLStream& stream, const float* indices, const float* weights, size_t size ) const {

	size_t nIndices = this->WriteComponents ( stream, ATTRIBUTE_BONE_INDICES, indices, size );
	size_t nWeights = this->WriteComponents ( stream, ATTRIBUTE_BONE_WEIGHTS, weights, size );
	
	return MIN ( nIndices, nWeights );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteColor ( ZLStream& stream, ZLIndex idx, u32 color ) const {

	ZLColorVec colorVec ( color );
	this->WriteAttribute ( stream, ATTRIBUTE_COLOR, idx, colorVec.mR, colorVec.mG, colorVec.mB, colorVec.mA );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteColor ( ZLStream& stream, ZLIndex idx, float r, float g, float b, float a ) const {
	
	this->WriteAttribute ( stream, ATTRIBUTE_COLOR, idx, r, g, b, a );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteCoord ( ZLStream& stream, ZLIndex idx, float x, float y, float z, float w ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_COORD, idx, x, y, z, w );
}

//----------------------------------------------------------------//
size_t ZLVertexFormat::WriteComponents ( ZLStream& stream, u32 useID, const float* components, size_t size ) const {

	size_t remaining = size;
	size_t n = this->mTotalAttributesByUse [ useID ];
	
	for ( ZLIndex i = ZLIndexOp::ZERO; ( i < n ) && remaining; ++i ) {
	
		const ZLVertexAttribute& attribute = this->mAttributes [ this->mAttributeIDsByUse [ useID ][ i ]];
		
		size_t attrSize = attribute.mSize;
		
		ZLVec4D vector ( 0.0f, 0.0f, 0.0f, 0.0f );
		memcpy ( &vector, components, attrSize * sizeof ( float ));
		
		this->WriteAttribute ( stream, useID, i, vector.mX, vector.mY, vector.mZ, vector.mW );
		
		remaining = remaining - attrSize;
		components = &components [ attrSize ];
	}
	
	return size - remaining;
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteNormal ( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_NORMAL, idx, x, y, z, 0.0f );
}

//----------------------------------------------------------------//
void ZLVertexFormat::WriteUV ( ZLStream& stream, ZLIndex idx, float x, float y, float z ) const {

	this->WriteAttribute ( stream, ATTRIBUTE_TEX_COORD, idx, x, y, z, 0.0f );
}

//----------------------------------------------------------------//
ZLVertexFormat::ZLVertexFormat () :
	mTotalAttributes ( 0 ),
	mVertexSize ( 0 ) {
	
	RTTI_SINGLE ( RTTIBase )
	
	for ( u32 i = 0; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		this->mTotalAttributesByUse [ i ] = 0;
	}
}

//----------------------------------------------------------------//
ZLVertexFormat::~ZLVertexFormat () {
}
