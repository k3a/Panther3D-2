/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#pragma once

namespace P3D
{

/// Multi-purpose array template with fixed length
/** The behavior is similar to our String. When array is initialized it has fixed capacity. 
	Calling Add will add items to array but won't care about overflow cases. 
	This makes this template as fast as ordinary C++ arrays.
	
	But it is also possible to use this as dynamic array. Simply use AddEx() methods.
	These methods will check array capacity and expand it if needed. */
template <class T> class tArray {

public:

	// TtArray::Iterator
	class Iterator {

	protected:

		friend class tArray;

		Iterator(T *ptr) : ptr(ptr) { }

	public:

		Iterator() : ptr(0) { }
		Iterator(const Iterator &it) : ptr(it.ptr) { }
		virtual ~Iterator() { }

		Iterator &operator=(const Iterator &it) {
			ptr = it.ptr;
			return *this;
		}

		int operator==(const Iterator &it) const { return ptr == it.ptr; }
		int operator!=(const Iterator &it) const { return ptr != it.ptr; }

		T &operator*() { return *ptr; }
		const T &operator*() const { return *ptr; }

		T *operator->() { return ptr; }
		const T *operator->() const { return ptr; }

		Iterator &operator++() {
			ptr++;
			return *this;
		}
		Iterator operator++(int) {
			Iterator tmp = *this;
			ptr++;
			return tmp;
		}

		Iterator operator+(unsigned int n) const { return Iterator(ptr + n); }
		Iterator operator-(unsigned int n) const { return Iterator(ptr - n); }
		Iterator &operator+=(unsigned int n) { ptr += n; return *this; }
		Iterator &operator-=(unsigned int n) { ptr -= n; return *this; }

		int operator-(const Iterator &it) const { return (int)(ptr - it.ptr); }

	protected:

		T *ptr;
	};

	// tArray
	tArray() : length(0), capacity(0), data(0) {

	}
	tArray(const tArray &v) : length(v.length), capacity(v.length), data(0) {
		data = new T[capacity];
		for(register unsigned int i = 0; i < length; i++) {
			data[i] = v.data[i];
		}
	}
	tArray(unsigned int size) : length(0), capacity(size), data(0) {
		data = new T[capacity];
	}
	tArray(T *v,unsigned int size) : length(size), capacity(size), data(0) {
		data = new T[capacity];
		for(register int i = 0; i < length; i++) {
			data[i] = v[i];
		}
	}
	virtual ~tArray() {
		delete [] data;
	}

	tArray &operator=(const tArray &v) {
		if(v.length > capacity) {
			capacity = v.length;
			T *new_data = new T[capacity];
			delete [] data;
			data = new_data;
		}
		length = v.length;
		for(unsigned int i = 0; i < length; i++) {
			data[i] = v.data[i];
		}
		return *this;
	}

	inline T &operator[](unsigned int index) { assertd(index<length, "Trying to get item outside array range!"); return data[index]; }
	inline const T &operator[](unsigned int index) const { assertd(index<length, "Trying to get item outside array range!"); return data[index]; }

	/// Iterator at beginning of array
	inline Iterator Begin() const { return Iterator(data); }
	/// Iterator at end of array
	inline Iterator End() const { return Iterator(data + length); }

	/// Get current array length (number of array items)
	inline unsigned int Size() const { return length; }

	/// Get current array capacity (allocated memory space, could be > Size())
	inline unsigned int Capacity() const { return capacity; }

	/// (Re)allocate memory for size items and set length to this size
	inline void Resize(unsigned int size) {
		if(size <= capacity) {
			length = size;
			return;
		}
		capacity = size;
		T *new_data = new T[capacity];
		for(register unsigned int i = 0; i < length; i++) {
			new_data[i] = data[i];
		}
		delete [] data;
		data = new_data;
		length = size;
	}

	/// Reserve memory space for size items but not change array length
	inline void Reserve(unsigned int size) {
		if(size <= capacity) {
			return;
		}
		capacity = size;
		T *new_data = new T[capacity];
		for(unsigned int i = 0; i < length; i++) {
			new_data[i] = data[i];
		}
		delete [] data;
		data = new_data;
	}

	/// Clear array (but retain memory for buffer)
	inline void Clear() {
		length = 0;
	}

	/// Clear array with memory buffer
	/** \see Clear */
	inline void Delete() {
		capacity = 0;
		length = 0;
		delete[] data;
	}

	/// Find item in array and return iterator at this point (end if not found)
	inline Iterator Find(const T &t) {
		for(register unsigned int i = 0; i < length; i++) {
			if(data[i] == t) return Iterator(data + i);
		}
		return Iterator(data + length);
	}

	/// Find item in array and return index (-1 if not found)
	inline int FindIndex(const T &t) {
		for(register unsigned int i = 0; i < length; i++) {
			if(data[i] == t) return i;
		}
		return -1;
	}

	/// Appends other tArray to the end of this one
	inline void AppendEx(tArray<T>& A)
	{
		if (length+A.Size()<=capacity)
		{
			memcpy(&data[length], &A[0], A.Size() * sizeof(T));
		}
		else
		{
			// expand buffer
			int old_length = length;
			Resize(length + A.Size() + 1);
			length = old_length + A.Size();

			memcpy(&data[old_length], &A[0], A.Size() * sizeof(T));
		}
	}

	/// Add new array item at the end
	inline const T & Add(const T &t) {
		assertd(length < capacity, "Capacity of array exceeded. If you don't know array size in advance, you can use AddEx instead which will expand capacity where necessary.");
		data[length++] = t;

		return t;
	}
	/// Add new array item at the end. Expand buffer if needed.
	inline const T & AddEx(const T &t) {
		if(length < capacity) {
			data[length++] = t;
			return t;
		}
		// expand buffer
		int old_length = length;
		Resize(length * 2 + 1);
		length = old_length + 1;
		data[old_length] = t;

		return t;
	}

	/// Add new array item at the end, expand buffer if needed and return reference to this new item.
	inline T & AddEx() {
		if(length < capacity) {
			return data[length++];
		}
		// expand buffer
		int old_length = length;
		Resize(length * 2 + 1);
		length = old_length + 1;
		return data[old_length];
	}

	/// Add new array item at specified position
	inline const T & Add(unsigned int position,const T &t) {
		assertd(position<=length, "Trying to add item at the position greater then end of array.");
		assertd(length < capacity, "Capacity of array exceeded. If you don't know array size in advance, you can use AddEx instead which will expand capacity where necessary.");
		// copy [end, position] items to make shift, must be done from last to pos
		for(register unsigned int i = length - 1; i >= position; i--) {
			data[i + 1] = data[i];
		}
		// set new item at desired position
		data[position] = t;
		length++;

		return t;
	}
	/// Add new array item at specified position
	inline const T & AddEx(unsigned int position,const T &t) {
		assertd(position<=length, "Trying to add item at the position greater then end of array.");
		// in case buffer is long enough
		if(length < capacity) {
			// copy [end, position] items to make shift, must be done from last to pos
			for(register unsigned int i = length - 1; i >= position; i--) {
				data[i + 1] = data[i];
			}
			// set new item at desired position
			data[position] = t;
			length++;
			return;
		}
		// expand buffer
		capacity = length * 2 + 1;
		T *new_data = new T[capacity];
		// copy items until position (without position item)
		for(register unsigned int i = 0; i < position; i++) {
			new_data[i] = data[i];
		}
		// set item at position
		new_data[position] = t;
		// copy [end, position] items to new array, must be done from last to pos
		for(register unsigned int i = position; i < length; i++) {
			new_data[i + 1] = data[i];
		}
		delete [] data;
		data = new_data;
		length++;

		return t;
	}
	/// Add item at position of iterator
	inline const T & Add(const Iterator &it,const T &t) {
		return Add(it - Begin(),t);
	}
	/// Add item at position of iterator and expand buffer if needed
	inline const T & AddEx(const Iterator &it,const T &t) {
		return AddEx(it - Begin(),t);
	}
	/// Remove last item from array and return it as temporary pointer (until this position is replaced with new data)
	inline T* Remove() {
		if (length<=0) return NULL;
		length--;
		return &data[length];
	}
	/// Remove item at position
	inline void Remove(unsigned int position) {
		assertd(position<length, "Trying to remove item outside array range!");
		//assertd(index<length, "Trying to remove item outside array range!");
		for(register unsigned int i = position; i < length - 1; i++) {
			data[i] = data[i + 1];
		}
		length--;
	}
	/// Remove item at position of iterator
	inline void Remove(const Iterator &it) {
		Remove(it - Begin());
	}
	/// Exchanges the content of the vector by the content of vec, which is another vector of the same type. Sizes may differ.
	inline void Swap(tArray<T>& A)
	{
		//backup information
		unsigned int tmpLength = A.length;
		unsigned int tmpCapacity = A.capacity;
		T* tmpData = A.data;

		A.length = length;
		A.capacity = capacity;
		A.data = data;

		length = tmpLength;
		capacity = tmpCapacity;
		data = tmpData;
	}

private:

	unsigned int length;
	unsigned int capacity;
	T* data;
};

}; // ns