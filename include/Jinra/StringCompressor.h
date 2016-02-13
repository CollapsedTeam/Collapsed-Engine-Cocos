#pragma once

namespace Jinra
{

class BitStream;
class HuffmanEncodingTree;

/** 
 * Writes and reads strings to and from bit streams.
 * Only works with ASCII strings. The default compression is for English.
 * You can call generateTreeFromStrings to compress and decompress other languages efficiently 
 * as well.
 */
class JINRA_DLL_EXPORT StringCompressor
{
public:
	/**
	 * Given an array of strings, such as a chat log, generate the optimal encoding tree for it.
	 * This function is optional and if it is not called a default tree will be used instead.
	 * @param input An array of bytes which should point to text.
	 * @param inputLength Length of \a input.
	 * @param languageID An identifier for the language / string table to generate the tree for. English is automatically created with ID 0 in the constructor.
	 */
	void generateTreeFromStrings(u8* input, u32 inputLength, u8 languageId);
	
 	/** 
	 * Writes input to output, compressed. Takes care of the null terminator for you.
	 * @param input Pointer to an ASCII string.
	 * @param maxCharsToWrite The max number of bytes to write of \a input. Use 0 to mean no limit.
	 * @param output The bit stream to write the compressed string to
	 * @param languageID Which language to use
	 */
	void encodeString(cchar* input, u32 maxCharsToWrite, BitStream* output, u8 languageId = 0);
	
	/** 
	 * Writes input to output, uncompressed. Takes care of the null terminator for you.
	 * @param output A block of bytes to receive the output
	 * @param maxCharsToWrite Size, in bytes, of \a output. A null terminator will always be appended to the output string. If the maxCharsToWrite is not large enough, the string will be truncated.
	 * @param input The bit stream containing the compressed string
	 * @param languageID Which language to use
	 */
	bool decodeString(char *output, u32 maxCharsToWrite, BitStream* input, u8 languageId = 0);

	void encodeString(const SString& input, u32 maxCharsToWrite, BitStream* output, 
					  u8 languageId = 0);
	
	bool decodeString(SString* output, u32 maxCharsToWrite, BitStream* input, 
					  u8 languageId = 0);

private:
	StringCompressor();

	/** 
	 * Destructor
	 */
	~StringCompressor();

public:
	/** 
	 * Used so I can allocate and deallocate this singleton at runtime.
	 */
	static void addReference();

	/** 
	 * Used so I can allocate and deallocate this singleton at runtime.
	 */
	static void removeReference();

	/**
	 * Static function because only static functions can access static members.
	 * The NetPeer constructor adds a reference to this class, so don't call this until an 
	 * instance of NetPeer exists, or unless you call addReference yourself.
	 * @return the unique instance of the StringCompressor 
	 */
	static StringCompressor* getInstance() {
		return _instance;
	}

private:
	static StringCompressor* _instance;
	std::map<s32, HuffmanEncodingTree*> _huffmanEncodingTrees;
};

} // namespace Jinra

