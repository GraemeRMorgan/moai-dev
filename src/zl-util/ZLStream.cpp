// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLDirectoryItr.h>
#include <zl-util/ZLMemStream.h>
#include <zl-util/ZLStream.h>
#include <stdio.h>

//----------------------------------------------------------------//
bool ZLStream::CheckCaps ( u32 flags ) {

	return ( this->GetCaps () & flags ) != 0;
}

//----------------------------------------------------------------//
void ZLStream::Flush () {
}

//----------------------------------------------------------------//
size_t ZLStream::GetLength () {
	return UNKNOWN_SIZE;
}

//----------------------------------------------------------------//
bool ZLStream::IsAtEnd () {

	return ( this->GetCursor () == this->GetLength ());
}

//----------------------------------------------------------------//
size_t ZLStream::PeekBytes ( void* buffer, size_t size  ) {

	if ( !( this->GetCaps () & ( CAN_READ | CAN_SEEK ))) return 0;

	size_t cursor = this->GetCursor ();
	size = this->ReadBytes ( buffer, size );
	this->Seek ( cursor, SEEK_SET );
	return size;
}

//----------------------------------------------------------------//
size_t ZLStream::Print ( cc8* format, ... ) {

	va_list args;
	va_start ( args, format );
	
	size_t size = this->Print ( format, args );
	
	va_end ( args );
	
	return size;
}

//----------------------------------------------------------------//
size_t ZLStream::Print ( cc8* format, va_list args ) {

	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	static const size_t BUFFER_SIZE = 1024;
	char stackBuffer [ BUFFER_SIZE ];
	char* buffer = stackBuffer;
	int buffSize = BUFFER_SIZE;
	
	int result;
	
	for ( ;; ) {
	
		va_list copy;
		va_copy ( copy, args );
		result = vsnprintf ( buffer, buffSize, format, copy );
		va_end ( copy );
		
		// thanks to http://perfec.to/vsnprintf/ for a discussion of vsnprintf portability issues
		if (( result == buffSize ) || ( result == -1 ) || ( result == buffSize - 1 ))  {
			buffSize = buffSize << 1;
		}
		else if ( result > buffSize ) {
			buffSize = ( size_t )result + 2;
		}
		else {
			break;
		}
		
		if ( buffer == stackBuffer ) {
			buffer = 0;
		}
		
		if ( buffer ) {
			buffer = ( char* )realloc ( buffer, buffSize );
		}
		else {
			buffer = ( char* )malloc ( buffSize );
		}
	}
	
	size_t size = 0;
	if ( result > 0 ) {
		size = this->WriteBytes ( buffer, ( size_t )result );
	}
	
	if ( buffer != stackBuffer ) {
		free ( buffer );
	}
	
	return size;
}

//----------------------------------------------------------------//
template <> bool ZLStream::Read < bool >( bool value ) {
	u8 result = this->Read < u8 >( value ? 1 : 0 );
    return ( result > 0 );
}

//----------------------------------------------------------------//
size_t ZLStream::ReadBytes ( void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	return 0;
}

//----------------------------------------------------------------//
STLString ZLStream::ReadString ( size_t size ) {

	STLString str;

	const unsigned int MAX_HEAP_ALLOC = 1024;
	if ( size ) {

		char* buffer;
		if ( size > MAX_HEAP_ALLOC ) {
			buffer = ( char* )malloc ( size + 1 );
		}
		else {
			buffer = ( char* )alloca ( size + 1 );
		}
		this->ReadBytes ( buffer, size );
		buffer [ size ] = 0;
		str = buffer;

		if ( size > MAX_HEAP_ALLOC ) {
			free ( buffer );
		}
	}
	return str;
}

//----------------------------------------------------------------//
STLString ZLStream::ReadToken ( cc8* delimiters ) {

	STLString str;
	if ( this->IsAtEnd ()) return str;

	char stackBuffer [ LOCAL_BUFFER ];
	
	ZLMemStream memStream;
	memStream.SetChunkSize ( LOCAL_BUFFER );
	memStream.SetGuestBuffer ( stackBuffer, LOCAL_BUFFER );
	
	char c = 0;
	size_t size = 0;
	
	do {
		
		c = this->Read < char >( 0 );
		
		if ( delimiters && c ) {
		
			bool isDelimiter = false;
		
			for ( size_t i = 0; delimiters [ i ]; ++i ) {
				if ( delimiters [ i ] == c ) {
					isDelimiter = true;
					break;
				}
			}
			
			if ( isDelimiter ) {
				if ( size ) {
					c = 0;
				}
				else {
					continue;
				}
			}
		}
		
		memStream.Write < char >( c );
		size++;
	}
	while ( c );
	
	if ( size ) {
		
		str.reserve ( size + 1 );
		memStream.Seek ( 0, SEEK_SET );
		
		while ( size > 0 ) {
			
			char buffer [ LOCAL_BUFFER ];
			
			size_t readSize = size;
			if ( LOCAL_BUFFER < readSize ) {
				readSize = LOCAL_BUFFER;
			}
			
			memStream.ReadBytes ( buffer, readSize );
			str.append ( buffer, readSize );
			size -= readSize;
		}
	}
	return str;
}

//----------------------------------------------------------------//
int ZLStream::Seek ( long offset, int origin ) {

	if ( !( this->GetCaps () & CAN_SEEK )) return -1;

	size_t cursor = this->GetCursor ();
	size_t length = this->GetLength ();
	size_t absCursor = 0;

	switch ( origin ) {
		case SEEK_CUR: {
			absCursor = cursor + offset;
			break;
		}
		case SEEK_END: {
			if ( length == UNKNOWN_SIZE ) return -1;
			absCursor = length + offset;
			break;
		}
		case SEEK_SET: {
			absCursor = offset;
			break;
		}
	}
	
	if ( absCursor > cursor ) {
		if (( length == UNKNOWN_SIZE ) || ( absCursor > length )) return -1;
	}
	return this->SetCursor ( absCursor );
}

//----------------------------------------------------------------//
int ZLStream::SetCursor ( long offset ) {
	UNUSED ( offset );
	return -1;
}

//----------------------------------------------------------------//
template <> void ZLStream::Write < bool >( bool value ) {

    u8 boolByte = ( value ) ? 1 : 0;
    this->Write < u8 >( boolByte );
}

//----------------------------------------------------------------//
size_t ZLStream::WriteBytes ( const void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	return 0;
}

//----------------------------------------------------------------//
size_t ZLStream::WriteRealsAsDoubles ( const real* reals, size_t count ) {

	#if ZL_SIZEOF_REAL == 4
	
		size_t total;
		for ( size_t i = 0; i < count; ++i ) {
			double value = ( double )reals [ i ];
			size_t written = this->WriteBytes ( &value, 8 );
			if ( written < 8 ) {
				return total + written;
			}
			total += written;
		}
		return total;
	
	#elif ZL_SIZEOF_REAL == 8
	
		return this->WriteBytes ( reals, count * 8 );
	
	#else
	
		#error "ZL_SIZEOF_REAL must be 4 or 8"
		return 0;
	
	#endif
}

//----------------------------------------------------------------//
size_t ZLStream::WriteRealsAsFloats ( const real* reals, size_t count ) {

	#if ZL_SIZEOF_REAL == 4
		
		return this->WriteBytes ( reals, count * 4 );
	
	#elif ZL_SIZEOF_REAL == 8
	
		size_t total;
		for ( size_t i = 0; i < count; ++i ) {
			float value = ( float )reals [ i ];
			size_t written = this->WriteBytes ( &value, 4 );
			if ( written < 8 ) {
				return total + written;
			}
			total += written;
		}
		return total;
	
	#else
	
		#error "ZL_SIZEOF_REAL must be 4 or 8"
		return 0;
	
	#endif
}

//----------------------------------------------------------------//
size_t ZLStream::WriteStream ( ZLStream& source ) {

	if ( !( source.GetCaps () & CAN_READ )) return 0;
	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t writeSize = 0;
	size_t total = 0;
	
	do {
	
		readSize = source.ReadBytes ( buffer, LOCAL_BUFFER );
		if ( readSize ) {
			writeSize = this->WriteBytes ( buffer, readSize );
			total += writeSize;
			if ( writeSize != readSize ) break;
		}
		
	} while ( readSize == LOCAL_BUFFER );

	return total;
}

//----------------------------------------------------------------//
size_t ZLStream::WriteStream ( ZLStream& source, size_t size ) {

	if ( !( source.GetCaps () & CAN_READ )) return 0;
	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t total = 0;
	
	do {
	
		if (( total + LOCAL_BUFFER ) > size ) {
			readSize = source.ReadBytes ( buffer, size - total );
		}
		else {
			readSize = source.ReadBytes ( buffer, LOCAL_BUFFER );
		}
		
		if ( readSize ) {
			total += this->WriteBytes ( buffer, readSize );
		}
	
	} while ( readSize == LOCAL_BUFFER );
	
	return total;
}

//----------------------------------------------------------------//
ZLStream::ZLStream () {
}

//----------------------------------------------------------------//
ZLStream::~ZLStream () {
}
