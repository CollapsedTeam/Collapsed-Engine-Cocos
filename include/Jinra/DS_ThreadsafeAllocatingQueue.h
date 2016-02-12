#pragma once

#include "DS_MemoryPool.h"

namespace Jinra
{

namespace DataStructures
{

template <class structureType>
class JINRA_DLL_EXPORT ThreadsafeAllocatingQueue
{
public:
	// Queue operations
	void Push(structureType *s);
	structureType *PopInaccurate(void);
	structureType *Pop(void);
	void SetPageSize(int size);
	bool IsEmpty(void);
	structureType * operator[] (u32 position);
	void RemoveAtIndex(u32 position);
	u32 Size(void);

	// Memory pool operations
	structureType *Allocate();
	void Deallocate(structureType *s);
	void Clear();
protected:

	mutable MemoryPool<structureType> memoryPool;
	std::mutex memoryPoolMutex;
	Queue<structureType*> queue;
	std::mutex queueMutex;
};

template <class structureType>
void ThreadsafeAllocatingQueue<structureType>::Push(structureType *s)
{
	queueMutex.lock();
	queue.push(s);
	queueMutex.unlock();
}

template <class structureType>
structureType *ThreadsafeAllocatingQueue<structureType>::PopInaccurate(void)
{
	structureType *s;
	if (queue.isEmpty())
		return 0;
	queueMutex.lock();
	if (queue.isEmpty() == false)
		s = queue.pop();
	else
		s = 0;
	queueMutex.unlock();
	return s;
}

template <class structureType>
structureType *ThreadsafeAllocatingQueue<structureType>::Pop(void)
{
	structureType *s;
	queueMutex.lock();
	if (queue.isEmpty()) {
		queueMutex.unlock();
		return 0;
	}
	s = queue.pop();
	queueMutex.unlock();
	return s;
}

template <class structureType>
structureType *ThreadsafeAllocatingQueue<structureType>::Allocate()
{
	structureType *s;
	memoryPoolMutex.lock();
	s = memoryPool.allocate();
	memoryPoolMutex.unlock();
	// Call new operator, memoryPool doesn't do this
	s = new ((void*)s) structureType;
	return s;
}
template <class structureType>
void ThreadsafeAllocatingQueue<structureType>::Deallocate(structureType *s)
{
	// Call delete operator, memory pool doesn't do this
	s->~structureType();
	memoryPoolMutex.lock();
	memoryPool.release(s);
	memoryPoolMutex.unlock();
}

template <class structureType>
void ThreadsafeAllocatingQueue<structureType>::Clear()
{
	memoryPoolMutex.lock();
	for (u32 i = 0; i < queue.size(); ++i) {
		queue[i]->~structureType();
		memoryPool.release(queue[i]);
	}
	queue.Clear();
	memoryPoolMutex.unlock();
	memoryPoolMutex.lock();
	memoryPool.clear();
	memoryPoolMutex.unlock();
}

template <class structureType>
void ThreadsafeAllocatingQueue<structureType>::SetPageSize(int size)
{
	memoryPool.SetPageSize(size);
}

template <class structureType>
bool ThreadsafeAllocatingQueue<structureType>::IsEmpty(void)
{
	bool isEmpty;
	queueMutex.lock();
	isEmpty = queue.isEmpty();
	queueMutex.unlock();
	return isEmpty;
}

template <class structureType>
structureType * ThreadsafeAllocatingQueue<structureType>::operator[] (u32 position)
{
	structureType *s;
	queueMutex.lock();
	s = queue[position];
	queueMutex.unlock();
	return s;
}

template <class structureType>
void ThreadsafeAllocatingQueue<structureType>::RemoveAtIndex(u32 position)
{
	queueMutex.lock();
	queue.RemoveAtIndex(position);
	queueMutex.unlock();
}

template <class structureType>
u32 ThreadsafeAllocatingQueue<structureType>::Size(void)
{
	u32 s;
	queueMutex.lock();
	s = queue.size();
	queueMutex.unlock();
	return s;
}

}

}