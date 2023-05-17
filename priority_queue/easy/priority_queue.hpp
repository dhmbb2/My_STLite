#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	T* arr;
	int nowsize, maxsize;
	void deleteSpace()
	{
        for (long long i = 1; i <= nowsize; ++i) 
            arr[i].~T();
        free(arr);
    }

    void doubleSpace()
    {
        maxsize *= 2;
        T* tmp = (T*)malloc((maxsize + 1) * sizeof(T));
        for(long long i = 1; i <= nowsize; ++i) {
            new(tmp + i) T(arr[i]);
            arr[i].~T();
        }
        free(arr);
        arr = tmp;
	}

	void perlocateDown(long long i)
	{
		long long hole = i;
		T tmp = arr[hole];

		long long child;
		for (; hole * 2 <= nowsize; hole = child) {
			child = hole * 2;
			if (child < nowsize && Compare() (arr[child], arr[child+1])) 
				child += 1;
			if (Compare() (tmp, arr[child]))
				arr[hole] = arr[child];
			else break; 
		}
		arr[hole] = tmp;
	}

public:
	/**
	 * TODO constructors
	 */
	priority_queue(): nowsize(0), maxsize(100) {arr = (T*) malloc(101 * sizeof(T));}
	priority_queue(const priority_queue &other): nowsize(other.nowsize), maxsize(other.maxsize) 
	{
		arr = (T*)malloc((maxsize + 1) * sizeof(T));
		for (long long i = 1; i <= nowsize; ++i) 
			new(arr+i) T(other.arr[i]);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {deleteSpace();}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) 
	{
		if(this == &other) return *this;
		deleteSpace();
		nowsize = other.nowsize;
		maxsize = other.maxsize;
		arr = (T*)malloc((maxsize + 1) * sizeof(T));
		for (long long i = 1; i <= nowsize; ++i) 
			new(arr+i) T(other.arr[i]);
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if (!nowsize) throw container_is_empty();
		return arr[1];
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		if (nowsize == maxsize) doubleSpace();
		
		new(arr + nowsize + 1) T(e);
		long long hole = ++nowsize;
		for(; hole > 1 && !Compare()(e, arr[hole/2]); hole /= 2)
			arr[hole] = arr[hole/2];
		arr[hole] = e;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if (!nowsize) throw container_is_empty();

		arr[1] = arr[nowsize];
		arr[nowsize].~T();
		--nowsize;
		perlocateDown(1);
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return nowsize;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return !nowsize;
	}
};

}

#endif