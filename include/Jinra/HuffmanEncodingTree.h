#pragma once

#include "BitStream.h"

#include "DS_LinkedList.h" 

namespace Jinra
{

struct HuffmanEncodingTreeNode
{
	u8 value;
	u32 weight;
	HuffmanEncodingTreeNode* left;
	HuffmanEncodingTreeNode* right;
	HuffmanEncodingTreeNode* parent;
};

// This generates special cases of the Huffman encoding tree using 8 bit keys with the 
// additional condition that unused combinations of 8 bits are treated as a frequency of 1.
class JINRA_DLL_EXPORT HuffmanEncodingTree
{

public:
	HuffmanEncodingTree();

	~HuffmanEncodingTree();

	/** 
	 * Pass an array of bytes to array and a preallocated BitStream to receive the output.
	 * @param input Array of bytes to encode
	 * @param sizeInBytes size of input
	 * @param output The bit stream to write to.
	 */
	void encodeArray(u8* input, size_t sizeInBytes, BitStream* output);

	/** 
	 * Decodes an array encoded by encodeArray().
	 */
	u32 decodeArray(BitStream* input, BitSize_t sizeInBits, size_t maxCharsToWrite, u8* output);
	
	void decodeArray(u8* input, BitSize_t sizeInBits, BitStream* output);

	/** 
	 * Given a frequency table of 256 elements, all with a frequency of 1 or more, generate 
	 * the tree.
	 */
	void generateFromFrequencyTable(u32 frequencyTable[256]);

	/** 
	 * Free the memory used by the tree.
	 */
	void freeMemory();

private:

	// The root node of the tree 
	HuffmanEncodingTreeNode* _root;

	// Used to hold bit encoding for one character
	struct CharacterEncoding
	{
		u8* encoding;
		u16 bitLength;
	};

	CharacterEncoding _encodingTable[256];

	void insertNodeIntoSortedList(HuffmanEncodingTreeNode* node, DataStructures::LinkedList<
								  HuffmanEncodingTreeNode*>* huffmanEncodingTreeNodeList) const;
};

} // namespace Jinra
