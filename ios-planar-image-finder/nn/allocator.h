#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <exception>

namespace nn {

	/**
	 * Pooled storage allocator
	 *
	 * The following routines allow for the efficient allocation of storage in
	 * small chunks from a specified pool.  Rather than allowing each structure
	 * to be freed individually, an entire pool of storage is freed at once.
	 * This method has two advantages over just using malloc() and free().  First,
	 * it is far more efficient for allocating small objects, as there is
	 * no overhead for remembering all the information needed to free each
	 * object or consolidating fragmented memory.  Second, the decision about
	 * how long to keep an object is made at the time of allocation, and there
	 * is no need to track down all the objects to free them.
	 *
	 */
	class PooledAllocator {
	private:
		PooledAllocator(const PooledAllocator&);
		PooledAllocator& operator=(const PooledAllocator&);
	private:
		int 	remaining;  /* Number of bytes left in current block of storage. */
		void*	base;     /* Pointer to base of current block of storage. */
		void*	loc;      /* Current location in block to next allocate memory. */

		enum {
			WORDSIZE = 16,
			BLOCKSIZE = 8192
		};

	public:

		PooledAllocator()
			: remaining(0)
			, base(0)
			, loc(0)
		{ }

		/**
		 * Destructor. Frees all the memory allocated in this pool.
		 */
		~PooledAllocator() {
			while (base) {
				void* prev = *((void**) base);   /* Get pointer to prev block. */
				delete[] *((char**)base);
				base = prev;
			}
		}

		/**
		 * Returns a pointer to a piece of new memory of the given size in bytes
		 * allocated from the pool.
		 */
		void* malloc(int size) {
			/* Round size up to a multiple of wordsize.  The following expression
				only works for WORDSIZE that is a power of 2, by masking last bits of
				incremented size to zero.
			*/
			size = (size + (WORDSIZE - 1)) & ~(WORDSIZE - 1);

			/* Check whether a new block must be allocated.  Note that the first word
				of a block is reserved for a pointer to the previous block.
			*/
			if (size > remaining) {
				/* Allocate new storage. */
				int blocksize = (size + sizeof(void*) + (WORDSIZE - 1) > BLOCKSIZE) ?
				                size + sizeof(void*) + (WORDSIZE - 1) : BLOCKSIZE;
				// use the standard C malloc to allocate memory
				void* m = new char[blocksize];
				/* Fill first word of new block with pointer to previous block. */
				((void**) m)[0] = base;
				base = m;
				int shift = 0;
				//int shift = (WORDSIZE - ( (((size_t)m) + sizeof(void*)) & (WORDSIZE-1))) & (WORDSIZE-1);
				remaining = blocksize - sizeof(void*) - shift;
				loc = ((char*)m + sizeof(void*) + shift);
			}

			void* rloc = loc;
			loc = (char*)loc + size;
			remaining -= size;
			return rloc;
		}

		/**
		 * Allocates (using this pool) a generic type T.
		 *
		 * Params:
		 *     count = number of instances to allocate.
		 * Returns: pointer (of type T*) to memory buffer
		 */
		template <typename T>
		T* allocate(size_t count = 1) {
			T* mem = (T*) this->malloc(sizeof(T) * count);
			return mem;
		}

	};

}

#endif //ALLOCATOR_H
