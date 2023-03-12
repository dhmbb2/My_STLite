#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include<climits>
#include<cstddef>
#include<cmath>

namespace sjtu {

template<typename T>
class vector {
private:
    T* arr;
    long long _nowsize, _maxsize;

    void _deleteSpace()
    {
        for (long long i = 0; i < _nowsize; ++i) 
            arr[i].~T();
        free(arr);
    }

    void _doubleSpace()
    {
        _maxsize *= 2;
        T* tmp = (T *)malloc(_maxsize * sizeof(T));
        for(long long i = 0; i < _nowsize; ++i) {
            new(tmp + i) T(arr[i]);
            arr[i].~T();
        }
        free(arr);
        arr = tmp;
    }
public:
    class const_iterator;
    class iterator {
        private:
            vector<T>* _ptr;
            int _idx;
        public:
            iterator() = default;
            iterator(const iterator &n) = default;
            iterator(vector<T>* p = nullptr, int idx = 0): _ptr(p), _idx(idx) {}

            iterator operator+(const int &n) const {
                return iterator(_ptr, _idx + n);
            }
            iterator operator-(const int &n) const {
                return iterator(_ptr, _idx - n);
            }

            int operator-(const iterator &rhs) const {
                if (_ptr != rhs._ptr) throw invalid_iterator();
                return abs(_idx - rhs._idx);
            }
            iterator& operator+=(const int &n) {
                _idx += n;
                return *this;
            }
            iterator& operator-=(const int &n) {
                if (_idx-n < 0) throw runtime_error();
                _idx -= n;
                return *this;
            }
            iterator operator++(int) {
                iterator tmp = *this;
                ++_idx;
                return tmp;
            }
            iterator& operator++() {
                ++_idx;
                return *this;
            }
            iterator operator--(int) {
                if (_idx == 0) throw runtime_error();
                iterator tmp = *this;
                --_idx;
                return tmp;
            }
            iterator& operator--() {
                if (_idx == 0) throw runtime_error();
                --_idx;
                return *this;
            }
            T& operator*() const{
                return _ptr->arr[_idx];
            }
            bool operator==(const iterator &rhs) const {
                return (_ptr == rhs._ptr && _idx == rhs._idx);
            }
            bool operator==(const const_iterator &rhs) const {
                return (_ptr == rhs._ptr && _idx == rhs._idx);
            }
            bool operator!=(const iterator &rhs) const {
                return (_ptr != rhs._ptr || _idx != rhs._idx);
            }
            bool operator!=(const const_iterator &rhs) const {
                return (_ptr != rhs._ptr || _idx != rhs._idx);
            }
    };
    

    class const_iterator {
        private:
            const vector<T>* _ptr;
            int _idx;
        public:
            const_iterator() = default;
            const_iterator(const const_iterator &n) = default;
            const_iterator(const vector<T>* p = nullptr, int idx = 0): _ptr(p), _idx(idx) {}

            const_iterator operator+(const int &n) const {
                return const_iterator(_ptr, _idx + n);
            }
            const_iterator operator-(const int &n) const {
                return const_iterator(_ptr, _idx - n);
            }

            int operator-(const iterator &rhs) const {
                if (_ptr != rhs._ptr) throw invalid_iterator();
                return abs(_idx - rhs._idx);
            }
            const_iterator& operator+=(const int &n) {
                _idx += n;
                return *this;
            }
            const_iterator& operator-=(const int &n) {
                if (_idx-n < 0) throw runtime_error();
                _idx -= n;
                return *this;
            }
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++_idx;
                return tmp;
            }
            const_iterator& operator++() {
                ++_idx;
                return *this;
            }
            const_iterator operator--(int) {
                if (_idx == 0) throw runtime_error();
                const_iterator tmp = *this;
                --_idx;
                return *this;
            }
            const_iterator& operator--() {
                if (_idx == 0) throw runtime_error();
                --_idx;
                return *this;
            }
            T& operator*() const{
                return _ptr->arr[_idx];
            }
            bool operator==(const iterator &rhs) const {
                return (_ptr == rhs._ptr && _idx == rhs._idx);
            }
            bool operator==(const const_iterator &rhs) const {
                return (_ptr == rhs._ptr && _idx == rhs._idx);
            }
            bool operator!=(const iterator &rhs) const {
                return (_ptr != rhs._ptr || _idx != rhs._idx);
            }
            bool operator!=(const const_iterator &rhs) const {
                return (_ptr != rhs._ptr || _idx != rhs._idx);
            }
    };

    vector():_nowsize(0), _maxsize(1000) {arr = (T*)malloc(_maxsize * sizeof(T));}
    vector(const vector &other): _nowsize(other._nowsize), _maxsize(other._maxsize) {
        arr = (T*)malloc(_maxsize * sizeof(T));
        for(long long i = 0; i < _nowsize; ++i)
            new(arr + i) T(other.arr[i]);
    }
    
    ~vector() {_deleteSpace();}

    vector &operator=(const vector &other) {
        if(this == &other) return *this;
        _maxsize = other._maxsize;
        for(long long i = 0; i < _nowsize; ++ i) 
            arr[i].~T();
        free(arr);
        _nowsize = other._nowsize;
        arr = (T*)malloc(_maxsize * sizeof(T));
        for(long long i = 0; i < _nowsize; ++i)
            new(arr + i) T(other.arr[i]);
        return *this;
    }

    T & at(const size_t &pos) {
        if (pos < 0 || pos >= _nowsize) 
            throw index_out_of_bound();
        return arr[pos];
    }
    const T & at(const size_t &pos) const {
        if (pos < 0 || pos >= _nowsize) 
            throw index_out_of_bound();
        return arr[pos];
    }

    T & operator[](const size_t &pos) {
        return at(pos);
    }
    const T & operator[](const size_t &pos) const {
        return at(pos);
    }
    
    const T & front() const {
        if(_nowsize == 0) 
            throw container_is_empty();
        return arr[0];
    }
    const T & back() const {
        if(_nowsize == 0) 
            throw container_is_empty();
        return arr[_nowsize - 1];
    }
    
    iterator begin() {
        return iterator(this, 0);
    }
    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }
    
    iterator end() {
        return iterator(this, _nowsize);
    }
    const_iterator cend() const {
        return const_iterator(this, _nowsize);
    }
    
    bool empty() const {
        return _nowsize == 0;
    }

    size_t size() const {
        return _nowsize;
    }
    
    void clear() {
        for (long long i = 0; i < _nowsize; ++i) 
            arr[i].~T();
        _nowsize = 0;
    }
    
    iterator insert(iterator pos, const T &value) {
        if (_nowsize == _maxsize - 1)
            _doubleSpace();
        new(arr + _nowsize) T(arr[_nowsize - 1]);
        for (long long i = _nowsize - 1; i > pos - begin(); --i)  // note that it can't be pos._idx
            arr[i] = arr[i - 1];
        arr[pos - begin()] = value;
        ++_nowsize;
        return pos;
    }
    
    iterator insert(const size_t &ind, const T &value) {
        if(ind > _nowsize)
            throw index_out_of_bound();
        if (_nowsize == _maxsize - 1)
            _doubleSpace();
        new(arr + _nowsize) T(arr[_nowsize - 1]);
        for (long long i = _nowsize - 1; i > ind; --i)
            arr[i] = arr[i - 1];
        arr[ind] = value;
        ++_nowsize;
        return iterator(this, ind);

    }
    
    iterator erase(iterator pos) {
        /*if (pos - begin() == _nowsize - 1){
            arr[_nowsize - 1].~T();
            --_nowsize;
            return end();
        } */
        for (long long i = pos - begin(); i < _nowsize-1; ++i)
            arr[i] = arr[i + 1];
        arr[_nowsize - 1].~T();
        --_nowsize;
        return iterator(this, pos - begin());
        
    }

    iterator erase(const size_t &ind) {
        if (ind >= size) 
            throw index_out_of_bound();
        /*if (ind == _nowsize - 1){
            arr[_nowsize - 1].~T();
            --_nowsize;
            return end();
        }*/
        for (long long i = ind; i < _nowsize-1; ++i)
            arr[i] = arr[i + 1];
        arr[_nowsize - 1].~T();
        --_nowsize;
        return iterator(this, ind);
    }
    
    void push_back(const T &value) {
        if (_nowsize == _maxsize - 1)
            _doubleSpace();
        new(arr + _nowsize) T(value);
        ++_nowsize;
    }
    
    void pop_back() {
        if (_nowsize == 0) 
            throw container_is_empty();
        arr[_nowsize - 1].~T();
        --_nowsize;
    }
};


}

#endif
