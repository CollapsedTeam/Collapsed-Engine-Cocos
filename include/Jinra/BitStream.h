#pragma once

#include "AddressOrGUID.h"
#include "StringCompressor.h"

#include "NetDefines.h"

#include "uint24_t.h"

namespace Jinra
{
	
class JINRA_DLL_EXPORT BitStream
{

public:
	/// Default Constructor
	BitStream();

	/** 
	 * Create the bitstream, with some number of bytes to immediately allocate.
	 * There is no benefit to calling this, unless you know exactly how many bytes you 
	 * need and it is greater than BITSTREAM_STACK_ALLOCATION_SIZE.
	 * In that case all it does is save you one or more realloc calls.
	 * @param initialBytesToAllocate the number of bytes to pre-allocate.
	 */
	BitStream(const u32 initialBytesToAllocate);

	/**
	 * Initialize the BitStream, immediately setting the data it contains to a predefined 
	 * pointer. Set copyData to true if you want to make an internal copy of the data you are 
	 * passing. Set it to false to just save a pointer to the data. You shouldn't call write 
	 * functions with copyData as false, as this will write to unallocated memory 99% of the 
	 * time you will use this function to cast Packet::data to a bit stream for reading, in 
	 * which case you should write something as follows:
	 * BitStream bs(packet->data, packet->length, false);
	 * @param data An array of bytes.
	 * @param lengthInBytes Size of the data.
	 * @param copyData true or false to make a copy of data or not.
	 */
	BitStream(u8* packetData, const u32 lengthInBytes, bool copyData);

	// Destructor
	~BitStream();

	/** 
	 * Resets the bitstream for reuse.
	 * resetWritePointer() + resetReadPointer()
	 */
	void reset();

	/**
	 * Sets the write pointer back to the beginning of your data.
	 */
	void resetWritePointer();

	/** 
	 * Sets the read pointer back to the beginning of your data.
	 */
	void resetReadPointer();

	/// \brief Write any integral type to a bitstream.  
	/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] inTemplateVar The value to write
	template <class templateType>
		void write(const templateType &inTemplateVar);

	/// \brief Write the dereferenced pointer to any integral type to a bitstream.  
	/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] inTemplateVar The value to write
	template <class templateType>
		void writePtr(templateType *inTemplateVar);

	/// \brief Write any integral type to a bitstream.  
	/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the bits of the type
	/// \param[in] inTemplateVar The value to write
	template <class templateType>
		void writeCompressed(const templateType &inTemplateVar);

	/// \brief Read any integral type from a bitstream.  
	/// \details Define __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] outTemplateVar The value to read
	/// \return true on success, false on failure.
	template <class templateType>
		bool read(templateType &outTemplateVar);

	/// \brief Read any integral type from a bitstream.  
	/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the bits of the type
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// \param[in] outTemplateVar The value to read
	/// \return true on success, false on failure.
	template <class templateType>
		bool readCompressed(templateType &outTemplateVar);

	/// \brief Read one bitstream to another.
	/// \param[in] numberOfBits bits to read
	/// \param bitStream the bitstream to read into from
	/// \return true on success, false on failure.
	bool read(BitStream *bitStream, BitSize_t numberOfBits);

	bool read(BitStream *bitStream);

	bool read(BitStream &bitStream, BitSize_t numberOfBits);

	bool read(BitStream &bitStream);

	/// \brief Write an array or casted stream or raw data.  This does NOT do endian swapping.
	/// \param[in] inputByteArray a byte buffer
	/// \param[in] numberOfBytes the size of \a input in bytes
	void write(cchar* inputByteArray, const u32 numberOfBytes);

	/// \brief Write one bitstream to another.
	/// \param[in] numberOfBits bits to write
	/// \param bitStream the bitstream to copy from
	void write(BitStream *bitStream, BitSize_t numberOfBits);
	void write(BitStream *bitStream);
	void write(BitStream &bitStream, BitSize_t numberOfBits);
	void write(BitStream &bitStream);

	/// \brief Read an array or casted stream of byte.
	/// \details The array is raw data. There is no automatic endian conversion with this function
	/// \param[in] output The result byte array. It should be larger than @em numberOfBytes.
	/// \param[in] numberOfBytes The number of byte to read
	/// \return true on success false if there is some missing bytes.
	bool read( char* output, const u32 numberOfBytes );

	/// Read one type serialized to another (smaller) type, to save bandwidth
	/// serializationType should be uint8_t, uint16_t, uint24_t, or u32
	/// Example: int num; ReadCasted<uint8_t>(num); would read 1 bytefrom the stream, and put the value in an integer
	/// \param[in] value The value to write
	template <class serializationType, class sourceType >
	bool ReadCasted( sourceType &value );

	/** 
	 * This is good to call when you are done with the stream to make
	 * sure you didn't leave any data left over void.
	 */
	void assertStreamEmpty();

	/// \brief JINRA_DEBUG_PRINTF the bits in the stream.  Great for debugging.
	void printBits(char *out) const;
	
	void printBits() const;

	void printHex(char *out) const;
	
	void printHex() const;

	/// \brief Ignore data we don't intend to read
	/// \param[in] numberOfBits The number of bits to ignore
	void ignoreBits(const BitSize_t numberOfBits);

	/// \brief Ignore data we don't intend to read
	/// \param[in] numberOfBits The number of bytes to ignore
	void ignoreBytes(const u32 numberOfBytes);

	/// \brief Move the write pointer to a position on the array.
	/// \param[in] offset the offset from the start of the array.
	/// \attention
	/// \details Dangerous if you don't know what you are doing!
	/// For efficiency reasons you can only write mid-stream if your data is byte aligned.
	inline void setWriteOffset(const BitSize_t offset) {
		_numberOfBitsUsed = offset;
	}

	/// \brief Returns the length in bits of the stream
	inline BitSize_t getNumberOfBitsUsed() const {
		return getWriteOffset();
	}

	inline BitSize_t getWriteOffset() const {
		return _numberOfBitsUsed;
	}

	/// \brief Returns the length in bytes of the stream
	inline BitSize_t getNumberOfBytesUsed() const {
		return BITS_TO_BYTES(_numberOfBitsUsed);
	}

	/// \brief Returns the number of bits into the stream that we have read
	inline BitSize_t getReadOffset() const {
		return _readOffset;
	}

	/// \brief Sets the read bit index
	inline void setReadOffset(const BitSize_t newReadOffset) {
		_readOffset = newReadOffset;
	}

	/// \brief Returns the number of bits left in the stream that haven't been read
	inline BitSize_t getNumberOfUnreadBits() const {
		return _numberOfBitsUsed - _readOffset;
	}

	/// \brief Makes a copy of the internal data for you \a _data will point to
	/// the stream. Partial bytes are left aligned.
	/// \param[out] _data The allocated copy of GetData()
	/// \return The length in bits of the stream.
	BitSize_t copyData(u8** _data) const;

	/// \internal
	/// Set the stream to some initial data.
	void setData(u8* inByteArray);

	/// Gets the data that BitStream is writing to / reading from.
	/// Partial bytes are left aligned.
	/// \return A pointer to the internal state
	inline u8* getData() const {
		return _data;
	}

	/// \brief Write numberToWrite bits from the input source.
	/// \details Right aligned data means in the case of a partial byte, the bits are aligned
	/// from the right (bit 0) rather than the left (as in the normal
	/// internal representation) You would set this to true when
	/// writing user data, and false when copying bitstream data, such
	/// as writing one bitstream to another.
	/// \param[in] inByteArray The data
	/// \param[in] numberOfBitsToWrite The number of bits to write
	/// \param[in] rightAlignedBits if true data will be right aligned
	void writeBits( const u8* inByteArray, BitSize_t numberOfBitsToWrite, const bool rightAlignedBits = true );

	/// \brief Align the bitstream to the byte boundary and then write the
	/// specified number of bits.  
	/// \details This is faster than WriteBits but
	/// wastes the bits to do the alignment and requires you to call
	/// ReadAlignedBits at the corresponding read position.
	/// \param[in] inByteArray The data
	/// \param[in] numberOfBytesToWrite The size of input.
	void writeAlignedBytes( const u8 *inByteArray, const u32 numberOfBytesToWrite );

	// Endian swap bytes already in the bitstream
	void endianSwapBytes( int byteOffset, int length );

	/// \brief Aligns the bitstream, writes inputLength, and writes input. Won't write beyond maxBytesToWrite
	/// \param[in] inByteArray The data
	/// \param[in] inputLength The size of input.
	/// \param[in] maxBytesToWrite Max bytes to write
	void writeAlignedBytesSafe( cchar *inByteArray, const u32 inputLength, const u32 maxBytesToWrite );

	/// \brief Read bits, starting at the next aligned bits. 
	/// \details Note that the modulus 8 starting offset of the sequence must be the same as
	/// was used with WriteBits. This will be a problem with packet
	/// coalescence unless you byte align the coalesced packets.
	/// \param[in] inOutByteArray The byte array larger than @em numberOfBytesToRead
	/// \param[in] numberOfBytesToRead The number of byte to read from the internal state
	/// \return true if there is enough byte.
	bool readAlignedBytes( u8 *inOutByteArray, const u32 numberOfBytesToRead );

	/// \brief Reads what was written by WriteAlignedBytesSafe.
	/// \param[in] inOutByteArray The data
	/// \param[in] maxBytesToRead Maximum number of bytes to read
	/// \return true on success, false on failure.
	bool readAlignedBytesSafe( char *inOutByteArray, int &inputLength, const int maxBytesToRead );
	bool readAlignedBytesSafe( char *inOutByteArray, u32 &inputLength, const u32 maxBytesToRead );

	/// \brief Same as ReadAlignedBytesSafe() but allocates the memory for you using new, rather than assuming it is safe to write to
	/// \param[in] outByteArray outByteArray will be deleted if it is not a pointer to 0
	/// \return true on success, false on failure.
	bool readAlignedBytesSafeAlloc( char **outByteArray, int &inputLength, const u32 maxBytesToRead );
	bool readAlignedBytesSafeAlloc( char **outByteArray, u32 &inputLength, const u32 maxBytesToRead );

	/// \brief Align the next write and/or read to a byte boundary.  
	/// \details This can be used to 'waste' bits to byte align for efficiency reasons It
	/// can also be used to force coalesced bitstreams to start on byte
	/// boundaries so so WriteAlignedBits and ReadAlignedBits both
	/// calculate the same offset when aligning.
	inline void alignWriteToByteBoundary() {_numberOfBitsUsed += 8 - ( (( _numberOfBitsUsed - 1 ) & 7) + 1 );}

	/// \brief Align the next write and/or read to a byte boundary.  
	/// \details This can be used to 'waste' bits to byte align for efficiency reasons It
	/// can also be used to force coalesced bitstreams to start on byte
	/// boundaries so so WriteAlignedBits and ReadAlignedBits both
	/// calculate the same offset when aligning.
	inline void AlignReadToByteBoundary() {_readOffset += 8 - ( (( _readOffset - 1 ) & 7 ) + 1 );}

	/// \brief Read \a numberOfBitsToRead bits to the output source.
	/// \details alignBitsToRight should be set to true to convert internal
	/// bitstream data to userdata. It should be false if you used
	/// WriteBits with rightAlignedBits false
	/// \param[in] inOutByteArray The resulting bits array
	/// \param[in] numberOfBitsToRead The number of bits to read
	/// \param[in] alignBitsToRight if true bits will be right aligned.
	/// \return true if there is enough bits to read
	bool ReadBits( u8 *inOutByteArray, BitSize_t numberOfBitsToRead, const bool alignBitsToRight = true );

	/// \brief Write a 0
	void write0();

	/// \brief Write a 1
	void write1();

	/// \brief Reads 1 bit and returns true if that bit is 1 and false if it is 0.
	bool readBit();

	/// \brief If we used the constructor version with copy data off, this
	/// *makes sure it is set to on and the data pointed to is copied.
	void AssertCopyData();

	/// \brief Use this if you pass a pointer copy to the constructor
	/// *(_copyData==false) and want to overallocate to prevent
	/// reallocation.
	void setNumberOfBitsAllocated(const BitSize_t lengthInBits);

	/// \brief Reallocates (if necessary) in preparation of writing numberOfBitsToWrite
	void addBitsAndReallocate( const BitSize_t numberOfBitsToWrite );

	/// \internal
	/// \return How many bits have been allocated internally
	inline BitSize_t getNumberOfBitsAllocated() const {
		return _numberOfBitsAllocated;
	}

	/// \brief Read strings, non reference.
	bool read(char *varString);
	bool read(u8 *varString);

	/// Write zeros until the bitstream is filled up to \a bytes
	void PadWithZeroToByteLength( u32 bytes );

	/// Get the number of leading zeros for a number
	/// \param[in] x Number to test
	static int numberOfLeadingZeroes( uint8_t x );
	static int numberOfLeadingZeroes( uint16_t x );
	static int numberOfLeadingZeroes( u32 x );
	static int numberOfLeadingZeroes( uint64_t x );

	/// \internal Unrolled inner loop, for when performance is critical
	void WriteAlignedVar8(cchar *inByteArray);
	/// \internal Unrolled inner loop, for when performance is critical
	bool ReadAlignedVar8(char *inOutByteArray);
	/// \internal Unrolled inner loop, for when performance is critical
	void WriteAlignedVar16(cchar *inByteArray);
	/// \internal Unrolled inner loop, for when performance is critical
	bool ReadAlignedVar16(char *inOutByteArray);
	/// \internal Unrolled inner loop, for when performance is critical
	void WriteAlignedVar32(cchar *inByteArray);
	/// \internal Unrolled inner loop, for when performance is critical
	bool ReadAlignedVar32(char *inOutByteArray);

	inline void write(const SString& inStringVar)
	{
		auto length = static_cast<u16>(inStringVar.length());
		write(length);
		writeAlignedBytes((const u8*)inStringVar.c_str(), (const u32)length);
	}

	inline void write(cchar* const inStringVar)
	{
		auto length = static_cast<u16>(strlen(inStringVar));
		write(length);
		writeAlignedBytes((const u8*)inStringVar, (const u32)length);
	}

	inline void write(const wchar_t* const inStringVar)
	{
#if 0
		char *multiByteBuffer;
		size_t allocated = wcslen(inStringVar)*MAX_BYTES_PER_UNICODE_CHAR;
		multiByteBuffer = (char*)malloc(allocated, _FILE_AND_LINE_);
		size_t used = wcstombs(multiByteBuffer, inStringVar, allocated);
		write(static_cast<u16>(used));
		this->WriteAlignedBytes((const u8*)multiByteBuffer, (const u32)used);
		free(multiByteBuffer, _FILE_AND_LINE_);
#else
		size_t mbByteLength = wcslen(inStringVar);
		write(static_cast<u16>(mbByteLength));
		for (u32 i = 0u; i < mbByteLength; ++i) {
			uint16_t t = static_cast<uint16_t>(inStringVar[i]);
			// Force endian swapping, and write to 16 bits
			this->write(t);
		}
#endif
	}
	
	inline void write(const u8* const inTemplateVar) {
		write((cchar*)inTemplateVar);
	}

	inline void write(char* const inTemplateVar) {
		write((cchar*)inTemplateVar);
	}

	inline void write(u8* const inTemplateVar) {
		write((cchar*)inTemplateVar);
	}

	inline void writeCompressed(cchar* const inStringVar) {
		StringCompressor::getInstance()->encodeString(inStringVar, 0xFFFF, this, 0);
	}

	inline void writeCompressed(const wchar_t* const inStringVar) {
		write(inStringVar);
	}

	inline void writeCompressed(const u8* const inTemplateVar) {
		writeCompressed((cchar*) inTemplateVar);
	}

	inline void writeCompressed(char* const inTemplateVar) {
		writeCompressed((cchar*) inTemplateVar);
	}

	inline void writeCompressed(u8* const inTemplateVar) {
		writeCompressed((cchar*) inTemplateVar);
	}

	inline static bool doEndianSwap() {
#ifndef __BITSTREAM_NATIVE_END
		return !isNetworkOrder();
#else
		return false;
#endif
	}

	inline static bool isBigEndian() {
		return isNetworkOrder();
	}

	static bool isNetworkOrder();

	static void reverseBytes(u8* inByteArray, u8* inOutByteArray, const u32 length);
	
	static void reverseBytesInPlace(u8* inOutData, const u32 length);

private:
	BitStream(const BitStream &invalid) = delete;

	BitStream& operator=(const BitStream& invalid) = delete;

	/// \brief Assume the input source points to a native type, compress and write it.
	void writeCompressed( const u8* inByteArray, const u32 size, const bool unsignedData );

	/// \brief Assume the input source points to a compressed native type. Decompress and read it.
	bool readCompressed( u8* inOutByteArray,	const u32 size, const bool unsignedData );

private:
	u8* _data;
	BitSize_t _numberOfBitsUsed;
	BitSize_t _numberOfBitsAllocated;
	BitSize_t _readOffset;

	/// true if the internal buffer is copy of the data passed to the constructor
	bool _copyData;

	/// BitStreams that use less than BITSTREAM_STACK_ALLOCATION_SIZE use the stack, rather than the heap to store data.  It switches over if BITSTREAM_STACK_ALLOCATION_SIZE is exceeded
	u8 _stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
};

template <class templateType>
	inline void BitStream::write(const templateType &inTemplateVar)
{
	if (sizeof(inTemplateVar) == 1) {
		writeBits((u8*)&inTemplateVar, sizeof(templateType) * 8, true);
	} else {
#ifndef __BITSTREAM_NATIVE_END
		if (doEndianSwap())
		{
			u8 output[sizeof(templateType)];
			reverseBytes((u8*)&inTemplateVar, output, sizeof(templateType));
			writeBits( ( u8* ) output, sizeof(templateType) * 8, true );
		}
		else
#endif
			writeBits( ( u8* ) & inTemplateVar, sizeof(templateType) * 8, true );
	}
}

template <class templateType>
inline void BitStream::writePtr(templateType *inTemplateVar)
{
	if (sizeof(templateType)==1)
		writeBits( ( u8* ) inTemplateVar, sizeof( templateType ) * 8, true );
	else
	{
#ifndef __BITSTREAM_NATIVE_END
		if (doEndianSwap())
		{
			u8 output[sizeof(templateType)];
			reverseBytes((u8*) inTemplateVar, output, sizeof(templateType));
			writeBits( ( u8* ) output, sizeof(templateType) * 8, true );
		}
		else
#endif
			writeBits( ( u8* ) inTemplateVar, sizeof(templateType) * 8, true );
	}
}

/// \brief Write a bool to a bitstream.
/// \param[in] inTemplateVar The value to write
template <>
inline void BitStream::write(const bool &inTemplateVar) {
	(inTemplateVar) ? write1() : write0();
}


/// \brief Write a systemAddress to a bitstream.
/// \param[in] inTemplateVar The value to write
template <>
	inline void BitStream::write(const SystemAddress &inTemplateVar)
{
	write(inTemplateVar.getIPVersion());
	if (inTemplateVar.getIPVersion() == 4)
	{
		// Hide the address so routers don't modify it
		SystemAddress var2=inTemplateVar;
		u32 binaryAddress=~inTemplateVar.address.addr4.sin_addr.s_addr;
		// Don't endian swap the address or port
		writeBits((u8*)&binaryAddress, sizeof(binaryAddress)*8, true);
		u16 p = var2.getPortNetworkOrder();
		writeBits((u8*)&p, sizeof(u16)*8, true);
	} else {
#if defined(JINRA_SUPPORT_IPV6)
		// Don't endian swap
		writeBits((const u8*) &inTemplateVar.address.addr6, sizeof(inTemplateVar.address.addr6)*8, true);
#endif
	}
}

template <>
inline void BitStream::write(const uint24_t &inTemplateVar)
{
	alignWriteToByteBoundary();
	addBitsAndReallocate(3*8);

	if (!isBigEndian())
	{
		_data[( _numberOfBitsUsed >> 3 ) + 0] = ((u8 *)&inTemplateVar.val)[0];
		_data[( _numberOfBitsUsed >> 3 ) + 1] = ((u8 *)&inTemplateVar.val)[1];
		_data[( _numberOfBitsUsed >> 3 ) + 2] = ((u8 *)&inTemplateVar.val)[2];
	} else {
		_data[( _numberOfBitsUsed >> 3 ) + 0] = ((u8 *)&inTemplateVar.val)[3];
		_data[( _numberOfBitsUsed >> 3 ) + 1] = ((u8 *)&inTemplateVar.val)[2];
		_data[( _numberOfBitsUsed >> 3 ) + 2] = ((u8 *)&inTemplateVar.val)[1];
	}

	_numberOfBitsUsed+=3*8;
}

template <>
inline void BitStream::write(const NetGUID& inTemplateVar) {
	write(inTemplateVar.id);
}

template <>
inline void BitStream::write(const std::wstring& inTemplateVar) {
	write(inTemplateVar.c_str());
}

template <>
inline void BitStream::write(cchar * const &inStringVar)
{
	u16 l = (u16)strlen(inStringVar);
	write(l);
	writeAlignedBytes((const u8*)inStringVar, (const u32)l);
}

template <>
inline void BitStream::write(const wchar_t* const &inStringVar) {
	write(inStringVar);
}

template <>
inline void BitStream::write(const u8* const &inTemplateVar) {
	write((cchar*)inTemplateVar);
}

template <>
inline void BitStream::write(char* const &inTemplateVar) {
	write((cchar*)inTemplateVar);
}

template <>
inline void BitStream::write(u8* const &inTemplateVar) {
	write((cchar*)inTemplateVar);
}

/// \brief Write any integral type to a bitstream.  
/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the bits of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] inTemplateVar The value to write
template <class templateType>
	inline void BitStream::writeCompressed(const templateType &inTemplateVar)
{
	if (sizeof(inTemplateVar)==1)
		writeCompressed( ( u8* ) & inTemplateVar, sizeof( templateType ) * 8, true );
	else
	{
#ifndef __BITSTREAM_NATIVE_END
		if (doEndianSwap())
		{
			u8 output[sizeof(templateType)];
			reverseBytes((u8*)&inTemplateVar, output, sizeof(templateType));
			writeCompressed( ( u8* ) output, sizeof(templateType) * 8, true );
		}
		else
#endif
			writeCompressed( ( u8* ) & inTemplateVar, sizeof(templateType) * 8, true );
	}
}

template <>
inline void BitStream::writeCompressed(const SystemAddress& inTemplateVar) {
	write(inTemplateVar);
}

template <>
inline void BitStream::writeCompressed(const NetGUID& inTemplateVar) {
	write(inTemplateVar);
}

template <>
inline void BitStream::writeCompressed(const uint24_t& var) {
	write(var);
}

template <>
inline void BitStream::writeCompressed(const bool &inTemplateVar) {
	write(inTemplateVar);
}

/// For values between -1 and 1
template <>
inline void BitStream::writeCompressed(const float &inTemplateVar)
{
	ASSERT(inTemplateVar > -1.01f && inTemplateVar < 1.01f);
	float varCopy = inTemplateVar;

	if (varCopy < -1.0f)
		varCopy=-1.0f;

	if (varCopy > 1.0f)
		varCopy=1.0f;

	write((u16)((varCopy+1.0f)*32767.5f));
}

/// For values between -1 and 1
template <>
inline void BitStream::writeCompressed(const double &inTemplateVar)
{
	ASSERT(inTemplateVar > -1.01 && inTemplateVar < 1.01);
	double varCopy=inTemplateVar;

	if (varCopy < -1.0f)
		varCopy=-1.0f;

	if (varCopy > 1.0f)
		varCopy=1.0f;

	write((u32)((varCopy+1.0)*2147483648.0));
}

template <>
inline void BitStream::writeCompressed(const std::wstring &inTemplateVar) {
	write(inTemplateVar.c_str());
}

template <>
inline void BitStream::writeCompressed(cchar* const& inStringVar) {
	StringCompressor::getInstance()->encodeString(inStringVar, 0xFFFF, this, 0);
}

template <>
inline void BitStream::writeCompressed(const wchar_t* const& inStringVar) {
	write(inStringVar);
}

template <>
inline void BitStream::writeCompressed(const u8* const& inTemplateVar) {
	writeCompressed((cchar*) inTemplateVar);
}

template <>
inline void BitStream::writeCompressed(char* const& inTemplateVar) {
	writeCompressed((cchar*) inTemplateVar);
}

template <>
inline void BitStream::writeCompressed(u8* const& inTemplateVar) {
	writeCompressed((cchar*) inTemplateVar);
}

/// \brief Read any integral type from a bitstream.  Define __BITSTREAM_NATIVE_END if you need endian swapping.
/// \param[in] outTemplateVar The value to read
template <class templateType>
inline bool BitStream::read(templateType &outTemplateVar)
{
	if (sizeof(outTemplateVar) == 1)
		return ReadBits( ( u8* ) &outTemplateVar, sizeof(templateType) * 8, true );
	else
	{
#ifndef __BITSTREAM_NATIVE_END
		if (doEndianSwap())
		{
			u8 output[sizeof(templateType)];
			if (ReadBits( ( u8* ) output, sizeof(templateType) * 8, true ))
			{
				reverseBytes(output, (u8*)&outTemplateVar, sizeof(templateType));
				return true;
			}
			return false;
		}
		else
#endif
			return ReadBits( ( u8* ) & outTemplateVar, sizeof(templateType) * 8, true );
	}
}

/// \brief Read a bool from a bitstream.
/// \param[in] outTemplateVar The value to read
template <>
inline bool BitStream::read(bool &outTemplateVar)
{
	if ( _readOffset + 1 > _numberOfBitsUsed )
		return false;

	if ( _data[ _readOffset >> 3 ] & ( 0x80 >> ( _readOffset & 7 ) ) )   // Is it faster to just write it out here?
		outTemplateVar = true;
	else
		outTemplateVar = false;

	// Has to be on a different line for Mac
	++_readOffset;

	return true;
}

/// \brief Read a systemAddress from a bitstream.
/// \param[in] outTemplateVar The value to read
template <>
inline bool BitStream::read(SystemAddress &outTemplateVar)
{
	u8 ipVersion;
	read(ipVersion);
	if (ipVersion==4)
	{
		outTemplateVar.address.addr4.sin_family=AF_INET;
		// Read(var.binaryAddress);
		// Don't endian swap the address or port
		u32 binaryAddress;
		ReadBits( ( u8* ) & binaryAddress, sizeof(binaryAddress) * 8, true );
		// Unhide the IP address, done to prevent routers from changing it
		outTemplateVar.address.addr4.sin_addr.s_addr=~binaryAddress;
		bool b = ReadBits(( u8* ) & outTemplateVar.address.addr4.sin_port, sizeof(outTemplateVar.address.addr4.sin_port) * 8, true);
		outTemplateVar.debugPort=ntohs(outTemplateVar.address.addr4.sin_port);
		return b;
	}
	else
	{
#if defined(JINRA_SUPPORT_IPV6)
		bool b = ReadBits((u8*) &outTemplateVar.address.addr6, sizeof(outTemplateVar.address.addr6)*8, true);
		outTemplateVar.debugPort=ntohs(outTemplateVar.address.addr6.sin6_port);
		return b;
#else
		return false;
#endif
	}	
}

template <>
inline bool BitStream::read(uint24_t &outTemplateVar)
{
	AlignReadToByteBoundary();
	if ( _readOffset + 3*8 > _numberOfBitsUsed )
		return false;

	if (isBigEndian()==false)
	{
		((u8 *)&outTemplateVar.val)[0]=_data[ (_readOffset >> 3) + 0];
		((u8 *)&outTemplateVar.val)[1]=_data[ (_readOffset >> 3) + 1];
		((u8 *)&outTemplateVar.val)[2]=_data[ (_readOffset >> 3) + 2];
		((u8 *)&outTemplateVar.val)[3]=0;
	}
	else
	{

		((u8 *)&outTemplateVar.val)[3]=_data[ (_readOffset >> 3) + 0];
		((u8 *)&outTemplateVar.val)[2]=_data[ (_readOffset >> 3) + 1];
		((u8 *)&outTemplateVar.val)[1]=_data[ (_readOffset >> 3) + 2];
		((u8 *)&outTemplateVar.val)[0]=0;
	}

	_readOffset+=3*8;
	return true;
}

template <>
inline bool BitStream::read(SString& varString)
{
	varString.clear();

	u16 l;
	bool b = read(l);
	if (l > 0) {
		varString.resize(l);
		b = readAlignedBytes((u8*)&varString[0], l);
		if (!b)
			varString.clear();
	} else
		AlignReadToByteBoundary();
	return b;
}

template <>
inline bool BitStream::read(NetGUID &outTemplateVar) {
	return read(outTemplateVar.id);
}

template <>
inline bool BitStream::read(wchar_t *&varString)
{
	size_t mbByteLength;
	this->ReadCasted<u16>(mbByteLength);
	if (mbByteLength > 0) {
#if 0
		char *multiByteBuffer;
		multiByteBuffer = (char*)malloc(mbByteLength + 1, _FILE_AND_LINE_);
		bool result = bs->ReadAlignedBytes((u8*)multiByteBuffer, (const u32)mbByteLength);
		if (result == false) {
			free(multiByteBuffer, _FILE_AND_LINE_);
			return false;
		}
		multiByteBuffer[mbByteLength] = 0;
		size_t c_strCharLength = mbstowcs(str, multiByteBuffer, mbByteLength);
		free(multiByteBuffer, _FILE_AND_LINE_);
		str[c_strCharLength] = 0;
#else
		for (u32 i = 0; i < mbByteLength; ++i) {
			uint16_t t;
			// Force endian swapping, and read 16 bits
			this->read(t);
			varString[i] = t;
		}
		varString[mbByteLength] = 0;
#endif
		return true;
	} else {
		wcscpy(varString, L"");
	}
	return true;
}
template <>
	inline bool BitStream::read(u8 *&varString)
{
	bool b;
	u16 l;
	b = read(l);
	if (b && l > 0)
		b = readAlignedBytes((u8*)varString, l);

	if (b == false)
		varString[0] = 0;

	varString[l] = 0;
	return b;
}

/// \brief Read any integral type from a bitstream.  
/// \details Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the bits of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] outTemplateVar The value to read
template <class templateType>
inline bool BitStream::readCompressed(templateType& outTemplateVar)
{
	if (sizeof(outTemplateVar) == 1)
		return readCompressed((u8*)&outTemplateVar, sizeof(templateType) * 8, true);

#ifndef __BITSTREAM_NATIVE_END
	if (doEndianSwap()) {
		u8 output[sizeof(templateType)];
		if (readCompressed( ( u8* ) output, sizeof(templateType) * 8, true )) {
			reverseBytes(output, (u8*)&outTemplateVar, sizeof(templateType));
			return true;
		}
		return false;
	}
#endif
	return readCompressed( ( u8* ) & outTemplateVar, sizeof(templateType) * 8, true );
}

template <>
inline bool BitStream::readCompressed(SystemAddress &outTemplateVar) {
	return read(outTemplateVar);
}

template <>
inline bool BitStream::readCompressed(uint24_t &outTemplateVar) {
	return read(outTemplateVar);
}

template <>
inline bool BitStream::readCompressed(NetGUID &outTemplateVar) {
	return read(outTemplateVar);
}

template <>
inline bool BitStream::readCompressed(bool &outTemplateVar) {
	return read(outTemplateVar);
}

/// For values between -1 and 1
template <>
inline bool BitStream::readCompressed(float &outTemplateVar)
{
	u16 compressedFloat;
	if (read(compressedFloat)) {
		outTemplateVar = (static_cast<float>(compressedFloat) / 32767.5f - 1.0f);
		return true;
	}
	return false;
}

/// For values between -1 and 1
template <>
inline bool BitStream::readCompressed(double &outTemplateVar)
{
	u32 compressedDouble;
	if (read(compressedDouble)) {
		outTemplateVar = (static_cast<double>(compressedDouble) / 2147483648.0 - 1.0);
		return true;
	}
	return false;
}

template <>
inline bool BitStream::readCompressed(std::wstring &outTemplateVar) {
	return read(outTemplateVar);
}

template <>
inline bool BitStream::readCompressed(char* &outTemplateVar) {
	return StringCompressor::getInstance()->decodeString(outTemplateVar, 0xFFFF, this, 0);
}

template <>
inline bool BitStream::readCompressed(u8* &outTemplateVar) {
	return StringCompressor::getInstance()->decodeString((char*)outTemplateVar, 0xFFFF, this, 0);
}

template <class serializationType, class sourceType >
bool BitStream::ReadCasted( sourceType &value )
{
	serializationType val;
	bool success = read(val);
	value = (sourceType)val;
	return success;
}

template <class templateType>
BitStream& operator<<(BitStream& out, templateType& c)
{
	out.write(c);
	return out;
}

template <class templateType>
BitStream& operator>>(BitStream& in, templateType& c)
{
	bool success = in.read(c);
	ASSERT(success);
	return in;
}

}
