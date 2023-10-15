#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class node {
    public:
        T data;
        node* pre;
        node* nex;

        node(T _data, node* _pre = nullptr, node* _nex = nullptr): data(_data), pre(_pre), nex(_nex) {};
    };

template<typename T, class node_type = node<T>>
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
        iterator& operator=(const_iterator& other)
        {
            pos = other.getpos();
            id = other.getid();
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
        const_iterator operator++(int) {
            if (pos->nex == nullptr) 
                throw invalid_iterator();
            const_iterator tmp = *this;
            pos = pos->nex;
            return tmp;
        }
        const_iterator& operator=(iterator& other)
        {
            pos = other.getpos();
            id = other.getid();
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
    }
};

}

#endif //SJTU_LIST_HPP
