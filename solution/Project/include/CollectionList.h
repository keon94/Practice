#pragma once
#include "u_ptr.h"
#include "CollectionException.h"
class CollectionList {

private:

    struct Node {
        Node* _next;
        Node* _prev;
        util::u_ptr<void> _data;
    };

public:
    class Iterator {
    public:
		Iterator(Node* const node) : _node(node) {}

        Iterator& operator++ () {
            _node = _node->_next;
            return *this;
        }

        Iterator& operator-- () {
            _node = _node->_prev;
            return *this;
        }

        template <typename T>
        T get() const {
			if (!_node) {
				throw EmptyCollectionException("Error in calling CollectionList::Iterator::get(): The Collection is empty.");
			}
            return *static_cast<T*>(_node->_data.get());
        }
    private:
        Node* _node;
    };

private:

    Node* _front;
    Node* _back;
    size_t _size;

public:

    Iterator front() {
		return std::move(Iterator(_front));
    }

    Iterator back() {
		return std::move(Iterator(_back));
    }

    CollectionList() {
        _size = 0;
        _front = _back = nullptr;
    }

    ~CollectionList() {
        Node* iter;
        while(_front) {
            iter = _front->_prev;
            //guaranteed to be non-nullptr
            delete _front;
            _front = iter;
        }
    }

    void insert(const Iterator* iter) {

    }

    template <typename T>
    void emplace_front(const T& val) {
        Node* _new_front = new Node();
        if(_size == 0) {
            _front = _new_front;
            _front->_next = nullptr;
            _back = _front;
        } else {
            _new_front->_prev = _front;
            _front->_next = _new_front;
            _front = _new_front;
        }
        _front->_data = util::u_ptr<T>(val);
        _front->_next = nullptr;
        _size++;
    }

    template <typename T>
    void emplace_back(const T& val) {
    }

    template <typename T>
    inline void push_front(const T& val) {
        emplace_front(T(val));
    }

    template <typename T>
    inline void push_back(const T& val) {
        emplace_back(T(val));
    }

    template <typename T>
    T pop_front() {
        if(_size == 0) {
            throw EmptyCollectionException("Error in calling CollectionList::pop_front(): The Collection is empty.");
        }
        T val = *static_cast<T*>(_front->_data.get());
        if(_size == 1) {
            delete _front;
            _front = _back = nullptr;
        } else {
            Node* new_front = _front->_prev;
            new_front->_next = nullptr;
            delete _front;
            _front = new_front;
        }
        _size--;
        return val;
    }

    template <typename T>
    T pop_back() {


    }

    size_t size() const {
        return _size;
    }


};