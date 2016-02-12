#pragma once

#include "DS_OrderedList.h"
#include "BitStream.h"

namespace Jinra
{

namespace DataStructures
{
template <class range_type>
struct RangeNode
{
	RangeNode() {}
	~RangeNode() {}
	RangeNode(range_type min, range_type max) { minIndex = min; maxIndex = max; }
	range_type minIndex;
	range_type maxIndex;
};


template <class range_type>
int RangeNodeComp(const range_type &a, const RangeNode<range_type> &b)
{
	if (a < b.minIndex)
		return -1;
	if (a == b.minIndex)
		return 0;
	return 1;
}

template <class range_type>
class JINRA_DLL_EXPORT RangeList
{
public:
	RangeList();
	~RangeList();
	void insert(range_type index);
	void clear(void);
	unsigned size(void) const;
	unsigned RangeSum(void) const;
	Jinra::BitSize_t Serialize(Jinra::BitStream *in, Jinra::BitSize_t maxBits, bool clearSerialized);
	bool Deserialize(Jinra::BitStream *out);

	DataStructures::OrderedList<range_type, RangeNode<range_type>, RangeNodeComp<range_type> > ranges;
};

template <class range_type>
Jinra::BitSize_t RangeList<range_type>::Serialize(Jinra::BitStream *in, Jinra::BitSize_t maxBits, bool clearSerialized)
{
	ASSERT(ranges.size() < (u16)-1);
	Jinra::BitStream tempBS;
	Jinra::BitSize_t bitsWritten;
	u16 countWritten;
	unsigned i;
	countWritten = 0;
	bitsWritten = 0;
	for (i = 0; i < ranges.size(); ++i) {
		if ((int)sizeof(u16) * 8 + bitsWritten + (int)sizeof(range_type) * 8 * 2 + 1>maxBits)
			break;
		u8 minEqualsMax;
		if (ranges[i].minIndex == ranges[i].maxIndex)
			minEqualsMax = 1;
		else
			minEqualsMax = 0;
		tempBS.write(minEqualsMax); // Use one byte, intead of one bit, for speed, as this is done a lot
		tempBS.write(ranges[i].minIndex);
		bitsWritten += sizeof(range_type) * 8 + 8;
		if (ranges[i].minIndex != ranges[i].maxIndex) {
			tempBS.write(ranges[i].maxIndex);
			bitsWritten += sizeof(range_type) * 8;
		}
		++countWritten;
	}

	in->alignWriteToByteBoundary();
	BitSize_t before = in->getWriteOffset();
	in->write(countWritten);
	bitsWritten += in->getWriteOffset() - before;
	in->write(&tempBS, tempBS.getNumberOfBitsUsed());

	if (clearSerialized && countWritten) {
		unsigned rangeSize = ranges.size();
		for (i = 0; i < rangeSize - countWritten; ++i) {
			ranges[i] = ranges[i + countWritten];
		}
		ranges.RemoveFromEnd(countWritten);
	}

	return bitsWritten;
}

template <class range_type>
bool RangeList<range_type>::Deserialize(Jinra::BitStream *out)
{
	ranges.clear();
	u16 count;
	out->AlignReadToByteBoundary();
	out->read(count);
	u16 i;
	range_type min, max;
	u8 maxEqualToMin = 0;

	for (i = 0; i < count; ++i) {
		out->read(maxEqualToMin);
		if (out->read(min) == false)
			return false;
		if (maxEqualToMin == false) {
			if (out->read(max) == false)
				return false;
			if (max < min)
				return false;
		} else
			max = min;


		ranges.InsertAtEnd(RangeNode<range_type>(min, max));
	}
	return true;
}

template <class range_type>
RangeList<range_type>::RangeList()
{
	RangeNodeComp<range_type>(0, RangeNode<range_type>());
}

template <class range_type>
RangeList<range_type>::~RangeList()
{
	clear();
}

template <class range_type>
void RangeList<range_type>::insert(range_type index)
{
	if (ranges.size() == 0) {
		ranges.Insert(index, RangeNode<range_type>(index, index), true);
		return;
	}

	bool objectExists;
	unsigned insertionIndex = ranges.GetIndexFromKey(index, &objectExists);
	if (insertionIndex == ranges.size()) {
		if (index == ranges[insertionIndex - 1].maxIndex + (range_type)1)
			ranges[insertionIndex - 1].maxIndex++;
		else if (index > ranges[insertionIndex - 1].maxIndex + (range_type)1) {
			// Insert at end
			ranges.Insert(index, RangeNode<range_type>(index, index), true);
		}

		return;
	}

	if (index < ranges[insertionIndex].minIndex - (range_type)1) {
		// Insert here
		ranges.InsertAtIndex(RangeNode<range_type>(index, index), insertionIndex);

		return;
	} else if (index == ranges[insertionIndex].minIndex - (range_type)1) {
		// Decrease minIndex and join left
		ranges[insertionIndex].minIndex--;
		if (insertionIndex > 0 && ranges[insertionIndex - 1].maxIndex + (range_type)1 == ranges[insertionIndex].minIndex) {
			ranges[insertionIndex - 1].maxIndex = ranges[insertionIndex].maxIndex;
			ranges.RemoveAtIndex(insertionIndex);
		}

		return;
	} else if (index >= ranges[insertionIndex].minIndex && index <= ranges[insertionIndex].maxIndex) {
		// Already exists
		return;
	} else if (index == ranges[insertionIndex].maxIndex + (range_type)1) {
		// Increase maxIndex and join right
		ranges[insertionIndex].maxIndex++;
		if (insertionIndex < ranges.size() - 1 && ranges[insertionIndex + (range_type)1].minIndex == ranges[insertionIndex].maxIndex + (range_type)1) {
			ranges[insertionIndex + 1].minIndex = ranges[insertionIndex].minIndex;
			ranges.RemoveAtIndex(insertionIndex);
		}

		return;
	}
}

template <class range_type>
void RangeList<range_type>::clear(void)
{
	ranges.clear();
}

template <class range_type>
unsigned RangeList<range_type>::size(void) const
{
	return ranges.size();
}

template <class range_type>
unsigned RangeList<range_type>::RangeSum(void) const
{
	unsigned sum = 0, i;
	for (i = 0; i < ranges.size(); ++i)
		sum += ranges[i].maxIndex - ranges[i].minIndex + 1;
	return sum;
}

}

}