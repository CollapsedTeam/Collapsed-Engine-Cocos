#pragma once

namespace Jinra
{

/// The namespace DataStructures was only added to avoid compiler errors for commonly named data structures
/// As these data structures are stand-alone, you can use them outside of Jinra for your own projects if you wish.
namespace DataStructures
{
template <class key_type, class data_type>
int defaultOrderedListComparison(const key_type &a, const data_type &b)
{
	if (a < b) return -1; if (a == b) return 0; return 1;
}

/// \note IMPORTANT! If you use defaultOrderedListComparison then call IMPLEMENT_DEFAULT_COMPARISON or you will get an unresolved external linker error.
template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&) = defaultOrderedListComparison<key_type, data_type> >
class JINRA_DLL_EXPORT OrderedList
{
public:
	static void IMPLEMENT_DEFAULT_COMPARISON(void) { DataStructures::defaultOrderedListComparison<key_type, data_type>(key_type(), data_type()); }

	OrderedList();
	~OrderedList();
	OrderedList(const OrderedList& original_copy);
	OrderedList& operator= (const OrderedList& original_copy);

	/// comparisonFunction must take a key_type and a data_type and return <0, ==0, or >0
	/// If the data type has comparison operators already defined then you can just use defaultComparison
	bool HasData(const key_type &key, int(*cf)(const key_type&, const data_type&) = default_comparison_function) const;
	// GetIndexFromKey returns where the insert should go at the same time checks if it is there
	unsigned GetIndexFromKey(const key_type &key, bool *objectExists, int(*cf)(const key_type&, const data_type&) = default_comparison_function) const;
	data_type GetElementFromKey(const key_type &key, int(*cf)(const key_type&, const data_type&) = default_comparison_function) const;
	bool GetElementFromKey(const key_type &key, data_type &element, int(*cf)(const key_type&, const data_type&) = default_comparison_function) const;
	unsigned Insert(const key_type &key, const data_type &data, bool assertOnDuplicate, int(*cf)(const key_type&, const data_type&) = default_comparison_function);
	unsigned Remove(const key_type &key, int(*cf)(const key_type&, const data_type&) = default_comparison_function);
	unsigned RemoveIfExists(const key_type &key, int(*cf)(const key_type&, const data_type&) = default_comparison_function);
	data_type& operator[] (const u32 position);
	void RemoveAtIndex(const unsigned index);
	void InsertAtIndex(const data_type &data, const unsigned index);
	void InsertAtEnd(const data_type &data);
	void RemoveFromEnd(const unsigned num = 1);
	void clear();
	unsigned size(void) const;

protected:
	std::vector<data_type> orderedList;
};

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
OrderedList<key_type, data_type, default_comparison_function>::OrderedList()
{
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
OrderedList<key_type, data_type, default_comparison_function>::~OrderedList()
{
	clear();
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
OrderedList<key_type, data_type, default_comparison_function>::OrderedList(const OrderedList& original_copy)
{
	orderedList = original_copy.orderedList;
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
OrderedList<key_type, data_type, default_comparison_function>& OrderedList<key_type, data_type, default_comparison_function>::operator= (const OrderedList& original_copy)
{
	orderedList = original_copy.orderedList;
	return *this;
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
bool OrderedList<key_type, data_type, default_comparison_function>::HasData(const key_type &key, int(*cf)(const key_type&, const data_type&)) const
{
	bool objectExists;
	GetIndexFromKey(key, &objectExists, cf);
	return objectExists;
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
data_type OrderedList<key_type, data_type, default_comparison_function>::GetElementFromKey(const key_type &key, int(*cf)(const key_type&, const data_type&)) const
{
	bool objectExists;
	unsigned index;
	index = GetIndexFromKey(key, &objectExists, cf);
	ASSERT(objectExists);
	return orderedList[index];
}
template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
bool OrderedList<key_type, data_type, default_comparison_function>::GetElementFromKey(const key_type &key, data_type &element, int(*cf)(const key_type&, const data_type&)) const
{
	bool objectExists;
	unsigned index;
	index = GetIndexFromKey(key, &objectExists, cf);
	if (objectExists)
		element = orderedList[index];
	return objectExists;
}
template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
unsigned OrderedList<key_type, data_type, default_comparison_function>::GetIndexFromKey(const key_type &key, bool *objectExists, int(*cf)(const key_type&, const data_type&)) const
{
	int index, upperBound, lowerBound;
	int res;

	if (orderedList.size() == 0) {
		*objectExists = false;
		return 0;
	}

	upperBound = (int)orderedList.size() - 1;
	lowerBound = 0;
	index = (int)orderedList.size() / 2;

	while (1) {
		res = cf(key, orderedList[index]);
		if (res == 0) {
			*objectExists = true;
			return (unsigned)index;
		} else if (res < 0) {
			upperBound = index - 1;
		} else// if (res>0)
		{

			lowerBound = index + 1;
		}

		index = lowerBound + (upperBound - lowerBound) / 2;

		if (lowerBound > upperBound) {
			*objectExists = false;
			return (unsigned)lowerBound; // No match
		}

		if (index < 0 || index >= (int)orderedList.size()) {
			// This should never hit unless the comparison function was inconsistent
			ASSERT(index && 0);
			*objectExists = false;
			return 0;
		}
	}
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
unsigned OrderedList<key_type, data_type, default_comparison_function>::Insert(const key_type &key, const data_type &data, bool assertOnDuplicate, int(*cf)(const key_type&, const data_type&))
{
	(void)assertOnDuplicate;
	bool objectExists;
	u32 index = GetIndexFromKey(key, &objectExists, cf);

	// Don't allow duplicate insertion.
	if (objectExists) {
		// This is usually a bug!
		ASSERT(assertOnDuplicate == false);
		return (unsigned)-1;
	}

	if (index >= orderedList.size()) {
		orderedList.push_back(data);
		return orderedList.size() - 1;
	} else {
		orderedList.insert(orderedList.begin() + index, data);
		return index;
	}
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
unsigned OrderedList<key_type, data_type, default_comparison_function>::Remove(const key_type &key, int(*cf)(const key_type&, const data_type&))
{
	bool objectExists;
	unsigned index;
	index = GetIndexFromKey(key, &objectExists, cf);

	// Can't find the element to remove if this assert hits
	if (objectExists == false) {
		ASSERT(objectExists == true);
		return 0;
	}

	orderedList.erase(orderedList.begin() + index);
	return index;
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
unsigned OrderedList<key_type, data_type, default_comparison_function>::RemoveIfExists(const key_type &key, int(*cf)(const key_type&, const data_type&))
{
	bool objectExists;
	unsigned index;
	index = GetIndexFromKey(key, &objectExists, cf);

	// Can't find the element to remove if this assert hits
	if (objectExists == false)
		return 0;

	orderedList.erase(orderedList.begin() + index);
	return index;
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
void OrderedList<key_type, data_type, default_comparison_function>::RemoveAtIndex(const unsigned index)
{
	orderedList.erase(orderedList.begin() + index);
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
void OrderedList<key_type, data_type, default_comparison_function>::InsertAtIndex(const data_type &data, const unsigned index)
{
	orderedList.insert(orderedList.begin() + index, data);
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
void OrderedList<key_type, data_type, default_comparison_function>::InsertAtEnd(const data_type &data)
{
	orderedList.push_back(data);
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
void OrderedList<key_type, data_type, default_comparison_function>::RemoveFromEnd(const unsigned num)
{
	for (s32 i = 0; i < num; ++i)
		orderedList.pop_back();
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
void OrderedList<key_type, data_type, default_comparison_function>::clear()
{
	orderedList.clear();
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
data_type& OrderedList<key_type, data_type, default_comparison_function>::operator[](const u32 position)
{
	return orderedList[position];
}

template <class key_type, class data_type, int(*default_comparison_function)(const key_type&, const data_type&)>
unsigned OrderedList<key_type, data_type, default_comparison_function>::size() const
{
	return orderedList.size();
}
}

}