#pragma once

namespace Jinra
{


/// The namespace DataStructures was only added to avoid compiler errors for commonly named data structures
/// As these data structures are stand-alone, you can use them outside of Jinra for your own projects if you wish.
namespace DataStructures
{
/// \brief A queue implemented as an array with a read and write index.
template <class queue_type>
class JINRA_DLL_EXPORT Queue
{
public:
	Queue();
	~Queue();
	Queue(Queue& original_copy);
	bool operator= (const Queue& original_copy);
	void push(const queue_type& input);
	void PushAtHead(const queue_type& input, unsigned index);
	queue_type& operator[] (u32 position) const; // Not a normal thing you do with a queue but can be used for efficiency
	void RemoveAtIndex(u32 position); // Not a normal thing you do with a queue but can be used for efficiency
	inline queue_type peek() const;
	inline queue_type PeekTail() const;
	inline queue_type pop();
	inline queue_type PopTail();
	// Debug: Set pointer to 0, for memory leak detection
	inline queue_type PopDeref();
	inline u32 size() const;
	inline bool isEmpty() const;
	inline u32 AllocationSize() const;
	inline void Clear();
	void Compress();
	bool Find(const queue_type& q);
	void clearAndForceAllocation(int size); // Force a memory allocation to a certain larger size

private:
	queue_type* array;
	u32 head;  // Array index for the head of the queue
	u32 tail; // Array index for the tail of the queue
	u32 allocation_size;
};


template <class queue_type>
inline u32 Queue<queue_type>::size() const
{
	if (head <= tail)
		return tail - head;
	else
		return allocation_size - head + tail;
}

template <class queue_type>
inline bool Queue<queue_type>::isEmpty() const
{
	return head == tail;
}

template <class queue_type>
inline u32 Queue<queue_type>::AllocationSize() const
{
	return allocation_size;
}

template <class queue_type>
Queue<queue_type>::Queue()
{
	allocation_size = 0;
	array = 0;
	head = 0;
	tail = 0;
}

template <class queue_type>
Queue<queue_type>::~Queue()
{
	if (allocation_size > 0)
		delete[] array;
}

template <class queue_type>
inline queue_type Queue<queue_type>::pop()
{
#ifdef JINRA_DEBUG
	ASSERT(head != tail);
#endif
	//head=(head+1) % allocation_size;

	if (++head == allocation_size)
		head = 0;

	if (head == 0)
		return (queue_type)array[allocation_size - 1];

	return (queue_type)array[head - 1];
}

template <class queue_type>
inline queue_type Queue<queue_type>::PopTail()
{
#ifdef JINRA_DEBUG
	ASSERT(head != tail);
#endif
	if (tail != 0) {
		--tail;
		return (queue_type)array[tail];
	} else {
		tail = allocation_size - 1;
		return (queue_type)array[tail];
	}
}

template <class queue_type>
inline queue_type Queue<queue_type>::PopDeref()
{
	if (++head == allocation_size)
		head = 0;

	queue_type q;
	if (head == 0) {
		q = array[allocation_size - 1];
		array[allocation_size - 1] = 0;
		return q;
	}

	q = array[head - 1];
	array[head - 1] = 0;
	return q;
}

template <class queue_type>
void Queue<queue_type>::PushAtHead(const queue_type& input, unsigned index)
{
	ASSERT(index <= size());

	// Just force a reallocation, will be overwritten
	push(input);

	if (size() == 1)
		return;

	unsigned writeIndex, readIndex, trueWriteIndex, trueReadIndex;
	writeIndex = size() - 1;
	readIndex = writeIndex - 1;
	while (readIndex >= index) {
		if (head + writeIndex >= allocation_size)
			trueWriteIndex = head + writeIndex - allocation_size;
		else
			trueWriteIndex = head + writeIndex;

		if (head + readIndex >= allocation_size)
			trueReadIndex = head + readIndex - allocation_size;
		else
			trueReadIndex = head + readIndex;

		array[trueWriteIndex] = array[trueReadIndex];

		if (readIndex == 0)
			break;
		writeIndex--;
		readIndex--;
	}

	if (head + index >= allocation_size)
		trueWriteIndex = head + index - allocation_size;
	else
		trueWriteIndex = head + index;

	array[trueWriteIndex] = input;
}


template <class queue_type>
inline queue_type Queue<queue_type>::peek() const
{
#ifdef JINRA_DEBUG
	ASSERT(head != tail);
#endif

	return (queue_type)array[head];
}

template <class queue_type>
inline queue_type Queue<queue_type>::PeekTail() const
{
#ifdef JINRA_DEBUG
	ASSERT(head != tail);
#endif
	if (tail != 0)
		return (queue_type)array[tail - 1];
	else
		return (queue_type)array[allocation_size - 1];
}

template <class queue_type>
void Queue<queue_type>::push(const queue_type& input)
{
	if (allocation_size == 0) {
		array = new queue_type[16];
		head = 0;
		tail = 1;
		array[0] = input;
		allocation_size = 16;
		return;
	}

	array[tail++] = input;

	if (tail == allocation_size)
		tail = 0;

	if (tail == head) {
		//  u32 index=tail;

		// Need to allocate more memory.
		queue_type * new_array;
		new_array = new queue_type[(int)allocation_size * 2];
#ifdef JINRA_DEBUG
		ASSERT(new_array);
#endif
		if (new_array == 0)
			return;

		for (u32 counter = 0; counter < allocation_size; ++counter)
			new_array[counter] = array[(head + counter) % (allocation_size)];

		head = 0;

		tail = allocation_size;

		allocation_size *= 2;

		// Delete the old array and move the pointer to the new array
		delete[] array;

		array = new_array;
	}

}

template <class queue_type>
Queue<queue_type>::Queue(Queue& original_copy)
{
	// Allocate memory for copy

	if (original_copy.size() == 0) {
		allocation_size = 0;
	}

	else {
		array = new queue_type(original_copy.size() + 1);

		for (u32 counter = 0; counter < original_copy.size(); ++counter)
			array[counter] = original_copy.array[(original_copy.head + counter) % (original_copy.allocation_size)];

		head = 0;

		tail = original_copy.size();

		allocation_size = original_copy.size() + 1;
	}
}

template <class queue_type>
bool Queue<queue_type>::operator= (const Queue& original_copy)
{
	if ((&original_copy) == this)
		return false;

	Clear();

	// Allocate memory for copy
	if (original_copy.size() == 0) {
		allocation_size = 0;
	}

	else {
		array = new queue_type(original_copy.size() + 1);

		for (u32 counter = 0; counter < original_copy.size(); ++counter)
			array[counter] = original_copy.array[(original_copy.head + counter) % (original_copy.allocation_size)];

		head = 0;

		tail = original_copy.size();

		allocation_size = original_copy.size() + 1;
	}

	return true;
}

template <class queue_type>
inline void Queue<queue_type>::Clear()
{
	if (allocation_size == 0)
		return;

	if (allocation_size > 32) {
		delete[] array;
		allocation_size = 0;
	}

	head = 0;
	tail = 0;
}

template <class queue_type>
void Queue<queue_type>::Compress()
{
	queue_type* new_array;
	u32 newAllocationSize;
	if (allocation_size == 0)
		return;

	newAllocationSize = 1;
	while (newAllocationSize <= size())
		newAllocationSize <<= 1; // Must be a better way to do this but I'm too dumb to figure it out quickly :)

	new_array = new queue_type[newAllocationSize];

	for (u32 counter = 0; counter < size(); ++counter)
		new_array[counter] = array[(head + counter) % (allocation_size)];

	tail = size();
	allocation_size = newAllocationSize;
	head = 0;

	// Delete the old array and move the pointer to the new array
	delete[] array;
	array = new_array;
}

template <class queue_type>
bool Queue<queue_type>::Find(const queue_type &q)
{
	if (allocation_size == 0)
		return false;

	u32 counter = head;

	while (counter != tail) {
		if (array[counter] == q)
			return true;

		counter = (counter + 1) % allocation_size;
	}

	return false;
}

template <class queue_type>
void Queue<queue_type>::clearAndForceAllocation(int size)
{
	delete[] array;
	if (size > 0)
		array = new queue_type[size];
	else
		array = 0;
	allocation_size = size;
	head = 0;
	tail = 0;
}

template <class queue_type>
inline queue_type& Queue<queue_type>::operator[] (u32 position) const
{
#ifdef JINRA_DEBUG
	ASSERT(position < size());
#endif
	//return array[(head + position) % allocation_size];

	if (head + position >= allocation_size)
		return array[head + position - allocation_size];
	else
		return array[head + position];
}

template <class queue_type>
void Queue<queue_type>::RemoveAtIndex(u32 position)
{
#ifdef JINRA_DEBUG
	ASSERT(position < size());
	ASSERT(head != tail);
#endif

	if (head == tail || position >= size())
		return;

	u32 index;

	u32 next;

	//index  = (head + position) % allocation_size;
	if (head + position >= allocation_size)
		index = head + position - allocation_size;
	else
		index = head + position;

	//next = (index + 1) % allocation_size;
	next = index + 1;

	if (next == allocation_size)
		next = 0;

	while (next != tail) {
		// Overwrite the previous element
		array[index] = array[next];
		index = next;
		//next = (next + 1) % allocation_size;

		if (++next == allocation_size)
			next = 0;
	}

	// Move the tail back
	if (tail == 0)
		tail = allocation_size - 1;
	else
		--tail;
}
} // End namespace

}


