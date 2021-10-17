#ifndef __MY_LIST_H__
#define __MY_LIST_H__

template <typename T>
class my_list {
public:
	typedef struct Node {
		T _data;
		struct Node *_Prev;
		struct Node *_Next;
	}Node;

	class iterator {
	public:

		Node *_node;
		iterator(Node *node = nullptr) { _node = node; }
		iterator &operator ++(int) {
			iterator temp = iterator(_node);
			_node = _node->_Next;
			return temp;
		}
		T& operator *() { if(_node != nullptr) return _node->_data; }
		bool operator !=(iterator b) {
			if (this->_node == b._node) return false;
			else return true;
		}
	};
private:
	int _size;
	Node *_head;
	Node *_tail;
public:

	my_list() {
		_size = 0; _head = nullptr; _tail = nullptr;
	}
	~my_list() {
		this->clear();
	}

	iterator begin() { return iterator(_head); }
	iterator end() { return nullptr; }

	void push_front(T data) {
		Node* pNode = new Node;
		pNode->_data = data;

		if (_head == nullptr) {
			_tail = _head = pNode;
			_head->_Next = nullptr;
			_head->_Prev = nullptr;
		}
		else {
			_head->_Prev = pNode;
			pNode->_Next = _head;
			_head = pNode;
			_head->_Prev = nullptr;
		}

		_size++;
	}
	void push_back(T data) {
		Node* pNode = new Node;
		pNode->_data = data;

		if (_tail == nullptr) {
			_tail = _head = pNode;
			_head->_Next = nullptr;
			_head->_Prev = nullptr;
		}
		else {
			_tail->_Next = pNode;
			pNode->_Prev = _tail;
			_tail = pNode;
			_tail->_Next = nullptr;
		}

		_size++;
	}
	void clear() {
		for (iterator it = begin(); it != end();) {
			it = erase(it);
		}
	}
	int size() { return _size; };

	iterator erase(iterator itor) {
		if (itor._node == nullptr) return itor;
		if (_size == 0) { 
			_head = nullptr; _tail = nullptr;
			return iterator(nullptr); 
		}
		Node* pNode = itor._node;
		Node* pNext = pNode->_Next;
		if (pNode->_Prev) pNode->_Prev->_Next = pNode->_Next;
		if (pNode->_Next) pNode->_Next->_Prev = pNode->_Prev;
		delete pNode;
		pNode = nullptr;
		_size--;
		return iterator(pNext);
	}

};

#endif //__MY_LIST_H__