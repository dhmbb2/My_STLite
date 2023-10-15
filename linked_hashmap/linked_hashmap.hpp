/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */
    
template<typename T, class node_type>
class list {
protected:

    using node = node_type;

    node *head, *tail;
    size_t len;

    void freespace() {
        node* p = head->nex, *q;
        while(p != tail) {
            q = p;
            p = p->nex;
            delete q;
        }
    }
    node *privateinsert(node *pos, node *cur) {
        if (pos == head)
            throw runtime_error();
        cur->pre = pos->pre;
        pos->pre->nex = cur;
        cur->nex = pos;
        pos->pre = cur;
        ++ len;
        return cur;
    }
    node *privateerase(node *pos) {
        if(pos == head || pos == tail)
            throw runtime_error();
       pos->pre->nex = pos->nex;
       pos->nex->pre = pos->pre;
       //pos->nex = pos->pre = nullptr;
       --len;
       return pos; 
    }

public:
    class const_iterator;
    class iterator {
    private:
        node* pos;
        const list<T, node>* id;
    public:
        explicit iterator(node* _pos = nullptr, const list<T, node>* _id = nullptr): pos(_pos), id(_id) {}
        explicit iterator(const const_iterator& other): pos(other.getpos()), id(other.getid()) {}
        iterator(const iterator& other) = default;
        ~iterator() = default;

        node* getpos() const {
            return pos;
        }
        const list<T, node>* getid() const {
            return id;
        }

		iterator& operator=(const iterator& other)
        {
            pos = other.getpos();
            id = other.getid();
			return *this;
        }

        iterator operator++(int) {
            if (pos->nex == nullptr) 
                throw invalid_iterator();
            iterator tmp = *this;
            pos = pos->nex;
            return tmp;
        }
        iterator & operator++() {
            if (pos->nex == nullptr) 
                throw invalid_iterator();
            pos = pos->nex;
            return *this;
        }
        iterator operator--(int) {
            if (pos->pre->pre == nullptr)
                throw invalid_iterator();
            iterator tmp = *this;
            pos = pos->pre;
            return tmp;
        }
        iterator & operator--() {
            if (pos->pre->pre == nullptr) 
                throw invalid_iterator();
            pos = pos->pre;
            return *this;
        }
        T & operator *() const {
            if (pos == id->head || pos == id->tail || pos == nullptr)
                throw invalid_iterator();
            return pos->data;
        }
        T * operator ->() const {
            if (pos == id->head || pos == id->tail || pos == nullptr)
                throw invalid_iterator();
            return &(pos->data);
        }

        bool operator==(const iterator &rhs) const {return (id == rhs.getid() && pos == rhs.getpos());}
        bool operator==(const const_iterator &rhs) const {return (id == rhs.getid() && pos == rhs.getpos());}
        bool operator!=(const iterator &rhs) const {return (id != rhs.getid() || pos != rhs.getpos());}
        bool operator!=(const const_iterator &rhs) const {return (id != rhs.getid() || pos != rhs.getpos());}
    };
    class const_iterator {
    private:
        const node* pos;
        const list<T, node>* id;
    public:
        explicit const_iterator(const node* _pos = nullptr, const list<T, node>* _id = nullptr): pos(_pos), id(_id) {}
        explicit const_iterator(const iterator& other): pos(other.getpos()), id(other.getid()) {} 
        const_iterator(const const_iterator& other) = default;
        ~const_iterator() = default;

        const node* getpos() const {
            return pos;
        }
        const list<T, node>* getid() const {
            return id;
        }

		//why the fuck is this !!!!!!!!
		const_iterator& operator=(const iterator& other)
        {
            pos = other.getpos();
            id = other.getid();
			return *this;
        }

        const_iterator operator++(int) {
            if (pos->nex == nullptr) 
                throw invalid_iterator();
            const_iterator tmp = *this;
            pos = pos->nex;
            return tmp;
        }
        const_iterator & operator++() {
            if (pos->nex == nullptr) 
                throw invalid_iterator();
            pos = pos->nex;
            return *this;
        }
        const_iterator operator--(int) {
            if (pos->pre->pre == nullptr)
                throw invalid_iterator();
            const_iterator tmp = *this;
            pos = pos->pre;
            return tmp;
        }
        const_iterator & operator--() {
            if (pos->pre->pre == nullptr) 
                throw invalid_iterator();
            pos = pos->pre;
            return *this;
        }
        const T & operator *() const {
            if (pos == id->head || pos == id->tail || pos == nullptr)
                throw invalid_iterator();
            return pos->data;
        }
        const T * operator ->() const {
            if (pos == id->head || pos == id->tail || pos == nullptr)
                throw invalid_iterator();
            return &(pos->data);
        }
        
        bool operator==(const iterator &rhs) const {return (id == rhs.getid() && pos == rhs.getpos());}
        bool operator==(const const_iterator &rhs) const {return (id == rhs.getid() && pos == rhs.getpos());}
        bool operator!=(const iterator &rhs) const {return (id != rhs.getid() || pos != rhs.getpos());}
        bool operator!=(const const_iterator &rhs) const {return (id != rhs.getid() || pos != rhs.getpos());}
    };

    list() {
        len = 0;
        head = (node *) malloc(sizeof(node));
        tail = (node *) malloc(sizeof(node));
        head->nex = tail;
        head->pre = nullptr;
        tail->pre = head;
        tail->nex = nullptr;
    }
    list(const list &other) {
        len = other.len;
        head = (node *) malloc(sizeof(node));
        tail = (node *) malloc(sizeof(node));
        node* p = other.head->nex, *q = head;
        while(p != other.tail) {
            q->nex = new node(p->data, q, tail);
            q = q->nex;
            p = p->nex;
        }
		if (q == head) head->nex = tail;
        tail->pre = q;
        head->pre = nullptr;
        tail->nex = nullptr;
    }
    virtual ~list() {
        freespace();
        head->nex = tail->pre = nullptr;
        free(head);
        free(tail);
    }
    list &operator=(const list &other) {
        if (&other == this) return *this;
        freespace();
        len = other.len;
        node* p = other.head->nex, *q = head;
        while(p != other.tail) {
            q->nex = new node(p->data, q, tail);
            q = q->nex;
            p = p->nex;
        }
        tail->pre = q;
        head->pre = nullptr;
        tail->nex = nullptr;
        return *this;
    }
    const T & front() const {
        if (len == 0) 
            throw container_is_empty();
        return head->nex->data;
    }
    const T & back() const {
        if (len == 0) 
            throw container_is_empty();
        return tail->pre->data;
    }
    iterator begin() {
        return iterator(head->nex, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head->nex, this);
    }
    iterator end() {
        return iterator(tail, this);
    }
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    virtual bool empty() const {return !len;}
    virtual size_t size() const {return len;}

    void clear() {
        freespace();
        len = 0;
        head->nex = tail;
        tail->pre = head;
    }
    iterator insert(iterator pos, const T &value) {
        if (pos.getpos() == head || pos.getid() != this)
            throw invalid_iterator();
        node* npos = pos.getpos();
        node* cur = new node(value);
        npos = privateinsert(npos, cur);
        iterator ret(npos, this);
        return ret;
    }
    iterator erase(iterator pos) {
        if (pos.getpos() == tail || pos.getpos() == head || pos.getid() != this)
            throw invalid_iterator();
        if (len == 0)
            throw container_is_empty();
        node* npos = pos.getpos();
        iterator ret(npos->nex, this);
        npos = privateerase(npos);
        delete npos;
        return ret;
    }
    void push_back(const T &value) {
        node* cur = new node(value);
        node* tmp = privateinsert(tail, cur);
    }
    void pop_back() {
        if (len == 0) 
            throw container_is_empty();
        node* tmp = privateerase(tail->pre);
        delete tmp;
    }
    void push_front(const T &value) {
        node* cur = new node(value);
        node* tmp = privateinsert(head->nex, cur);
    }
    void pop_front() {
        if (len == 0) 
            throw container_is_empty();
        node* tmp = privateerase(head->nex);
        delete tmp;
    }
	/*
    void sort() {
        if (len == 0) return;
        T* tmp = (T*) malloc(len * sizeof(T));
        node* p = head->nex;
        for(int i = 0; i < len; ++i) {
            new(tmp + i) T(p->data);
            p = p->nex;
        }
        sjtu::sort<T>(tmp, tmp+len, [](const T& a, const T& b){return a < b;});
        p = head->nex;
        for (int i = 0; i < len; ++i) {
            p->data = tmp[i];
            tmp[i].~T();
            p = p->nex;
        }
        free(tmp);
    }
    void merge(list &other) {
        if (other.len == 0) return;
        node* p = head->nex, *q = other.head->nex, *tmp;
        while(q != other.tail) {
            if(p == tail || q->data < p->data) {
                tmp = other.privateerase(q);
                q = tmp->nex;
                privateinsert(p, tmp);
            }
            else p = p->nex;
        }
    }
    void reverse() {
        if (len == 0) return;
        node* p = head, *tmp;
        while(p != nullptr) {
            tmp = p->nex;
            p->nex = p->pre;
            p->pre = tmp;
            p = p->pre;
        }
        tmp = tail;
        tail = head;
        head = tmp;
    }
    void unique() {
        if (len == 0) return;
        node* p = head->nex, *tmp;
        while(p != tail) {
            while(p->nex != tail && p->nex->data == p->data){
                tmp = privateerase(p->nex);
                delete tmp;
            }
            p = p->nex;
        }
    }*/
};

template<typename T>
class hash_node_t {
	public:
	T data;
	hash_node_t *pre, *nex, *hash_nex;

	hash_node_t(const T &value, hash_node_t *_pre = nullptr, hash_node_t *_nex = nullptr, hash_node_t *_hash_nex = nullptr): 
		data(value), pre(_pre), nex(_nex), hash_nex(_hash_nex) {}

	hash_node_t(T &&value):
		data(value), pre(nullptr), nex(nullptr), hash_nex(nullptr) {}
};

template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>> 
class linked_hashmap : public list<pair<const Key, T>, hash_node_t<pair<const Key, T>>> {
	public:
		using value_type = pair<const Key, T>;
		using hash_node = hash_node_t<value_type>;
		using list_hash = list<value_type, hash_node>;

	private: 
		static const size_t LOAD = 75, INITIAL_CAPACITY = 131;
		hash_node** hashmap;
		size_t capacity;
		Hash hash;
		Equal equal;

		//new a hashmap, base on current capacity
		void _new_hashmap()
		{
			hashmap	= new hash_node * [capacity];
			std::fill(hashmap, hashmap + capacity, nullptr);
		}

		// rehash a hashmap, provided the original hashmap is already deleted
		void _rehash()
		{
			_new_hashmap();
			for (hash_node *p = this->head->nex; p != nullptr && p != this->tail; p = p->nex) {
				size_t idx = hash(p->data.first) % capacity;
				p->hash_nex = hashmap[idx];
				hashmap[idx] = p;
			}
		}

		// find a value base on a key, return a pointer to the node
		hash_node *_find(const Key& key) const
		{
			size_t idx = hash(key) % capacity;
            hash_node* p = hashmap[idx];
			while (p!= nullptr && !equal(p->data.first, key))
				p = p->hash_nex;
			return p;
		}

		// double size the hashmap
		void _double_size()
		{
			delete [] hashmap;
			capacity = capacity * 2 + 3;
			_rehash();
		}

		// insert a node with a given key, provided we know that the key do not exist!
		hash_node *_insert(const value_type& value)
		{
			if (this->len * 100 > capacity * LOAD)
				_double_size();
			size_t idx = hash(value.first) % capacity;
			hash_node *p = new hash_node(value);
			this->privateinsert(this->tail, p);
			p->hash_nex = hashmap[idx], hashmap[idx] = p;
			return p;
		}

	public:

		using iterator = typename list_hash::iterator;
		using const_iterator = typename list_hash::const_iterator;
		
		linked_hashmap(): list_hash::list(), capacity(INITIAL_CAPACITY) 
		{
			_new_hashmap();
		}

		linked_hashmap(const linked_hashmap &other): list_hash::list(other), capacity(other.capacity) {
			_rehash();
		}
	
		// assignment operator using the list's assignment operator
		linked_hashmap & operator=(const linked_hashmap &other) 
		{
			if (&other == this) return *this;
			list_hash::operator=(other);
			delete [] hashmap;
			capacity = other.capacity;
			_rehash();
			return *this;
		}
	
		/**
		 * TODO Destructors
		 */
		~linked_hashmap() 
		{
			delete [] hashmap;
		}
	
		/**
		 * TODO
		 * access specified element with bounds checking
		 * Returns a reference to the mapped value of the element with key equivalent to key.
		 * If no such element exists, an exception of type `index_out_of_bound'
		 */
		T & at(const Key &key) 
		{
			hash_node* target = _find(key);
			if (!target)
				throw sjtu::index_out_of_bound();
			return target->data.second;
		}
		const T & at(const Key &key) const 
		{
			hash_node* target = _find(key);
			if (!target)
				throw index_out_of_bound();
			return target->data.second;
		}
	
		/**
		 * TODO
		 * access specified element 
		 * Returns a reference to the value that is mapped to a key equivalent to key,
		 *   performing an insertion if such key does not already exist.
		 */
		T & operator[](const Key &key) 
		{
			hash_node *target = _find(key);
			if (target)
				return target->data.second;
			return _insert(pair<Key, T>(key, T()))->data.second;
		}
	
		/**
		 * behave like at() throw index_out_of_bound if such key does not exist.
		 */
		const T & operator[](const Key &key) const 
		{
			return at(key);
		}
	
		void clear()
		{
			list_hash::clear();
			std::fill(hashmap, hashmap + capacity, nullptr);
		}
	
		/**
		 * insert an element.
		 * return a pair, the first of the pair is
		 *   the iterator to the new element (or the element that prevented the insertion), 
		 *   the second one is true if insert successfully, or false.
		 */
		pair<iterator, bool> insert(const value_type &value)
		{
			hash_node* p = _find(value.first);
			if (p) return pair<iterator, bool> (iterator(p, this), 0);
			p = _insert(value);
			return pair<iterator, bool> (iterator(p, this), 1);
		}
	
		/**
		 * erase the element at pos.
		 *
		 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
		 */
		void erase(iterator pos)
		{
			if (pos.getpos() == this->head || pos.getid() != this  || pos == this->end())
            	throw invalid_iterator();
			hash_node* p = _find(pos.getpos()->data.first);
			size_t idx = hash(p->data.first) % capacity;
			if (p == nullptr || hashmap[idx] == nullptr)
				throw index_out_of_bound();
			if (hashmap[idx] == p) {
				hashmap[idx] = p->hash_nex;
				list_hash::erase(iterator(p, this));
			}
			else {
				hash_node *q = hashmap[idx];
				for (; !equal(q->hash_nex->data.first, p->data.first); q = q->hash_nex);
				q->hash_nex = p->hash_nex;
				list_hash::erase(iterator(p, this));
			}
		}
	
		/**
		 * Returns the number of elements with key 
		 *   that compares equivalent to the specified argument,
		 *   which is either 1 or 0 
		 *     since this container does not allow duplicates.
		 */
		size_t count(const Key &key) const 
		{
			hash_node *p = _find(key);
			if (!p) return 0;
			else return 1;
		}
	
		/**
		 * Finds an element with key equivalent to key.
		 * key value of the element to search for.
		 * Iterator to an element with key equivalent to key.
		 *   If no such element is found, past-the-end (see end()) iterator is returned.
		 */
		iterator find(const Key &key) 
		{
			hash_node *p = _find(key);
			if (!p) return this->end();
			return iterator(p, this);
		}
		const_iterator find(const Key &key) const 
		{
			hash_node *p = _find(key);
			if (!p) return this->cend();
			return const_iterator(p, this);
		}
};

}

#endif
