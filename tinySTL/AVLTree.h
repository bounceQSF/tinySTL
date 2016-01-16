#pragma once

#include "alloc.h"
#include "stack.h"
#include "algorithm.h"

#include <iostream>
using std::ostream;
//iterator to be finished & erase operation;

template <typename T>
class deque;

namespace tinySTL {
	template<typename T>
	class avl_tree {
	private:
		struct node
		{
			node* _left;
			node* _right;
			T _data;
			size_t _height;
			explicit node(T d = T(), node *l =nullptr, node *r = nullptr)
				:_data(d), _left(l), _right(r){}
		};
		
		typedef allocator<node>                       data_allocator;

	public:
		typedef T                                  value_type;
		typedef T&                                 reference;
		typedef const T&                           const_reference;
		typedef size_t                             size_type;
	
	private:
		node* _root;
		size_t _size;

	public:
		avl_tree() :_root(nullptr), _size(0) {}
		avl_tree(const avl_tree&) = delete;
		avl_tree& operator=(const avl_tree&) = delete;
		~avl_tree() { destroyAndDeallocate(_root); }

		void insert(const T& val) { insert(val, _root); }
		template<typename _InputIterator>
		void insert(_InputIterator first, _InputIterator last);
		void erase(const T& val) { erase(val, _root); }

		size_t height() { return getHeight(_root); }
		size_t size() { return _size; }
		bool empty() { return !_root; }

	private:
		node* singleLeftLeftRotate(node*);
		node* doubleLeftRightRotate(node*);
		node* doubleRightLeftRotate(node*);
		node* singleRightRightRotate(node*);

		void preOrderTraverse(ostream &os = std::cout) { preOrderTraverse(_root, os); os << endl; }
		void midOrderTraverse(ostream &os = std::cout) { midOrderTraverse(_root, os); os << endl; }
		void postOrderTraverse(ostream &os = std::cout) { postOrderTraverse(_root, os); os << endl; }
		void levelOrderTraverse(ostream &os = std::cout) { levelOrderTraverse(_root, os); os << endl; }
		
	private:
		void insert(const T& val, node*& p);
		void erase(const T& val, node*& p);
		void destroyAndDeallocate(node *p);
		size_t getHeight(const node *p)const { 
			return p ? p->_height : 0; }

		void preOrderTraverse(const node* ptr, ostream &os = std::cout);
		void midOrderTraverse(const node* ptr, ostream &os = std::cout);
		void postOrderTraverse(const node* ptr, ostream &os = std::cout);
		void levelOrderTraverse(const node* ptr, ostream &os = std::cout);
	};

	template<typename T>
	inline typename avl_tree<T>::node* avl_tree<T>::singleLeftLeftRotate(node* k2)
	{
		auto k1 = k2->_left;
		k2->_left = k1->_right;
		k1->_right = k2;
		k2->_height = max(getHeight(k2->_left), getHeight(k2->_right)) + 1;
		k1->_height = max(getHeight(k1->_left), getHeight(k1->_right)) + 1;
		return k1;
	}

	template<typename T>
	inline typename avl_tree<T>::node* avl_tree<T>::doubleLeftRightRotate(node* k3)
	{

		k3->_left = singleRightRightRotate(k3->_left);
		auto k2 = singleLeftLeftRotate(k3);
		return k2;
	}

	template<typename T>
	inline typename avl_tree<T>::node* avl_tree<T>::doubleRightLeftRotate(node* k3)
	{
		k3->_right = singleLeftLeftRotate(k3->_right);
		auto k2 = singleRightRightRotate(k3);
		return k2;
	}

	template<typename T>
	inline typename avl_tree<T>::node* avl_tree<T>::singleRightRightRotate(node* k2)
	{
		auto k1 = k2->_right;
		k2->_right = k1->_left;
		k1->_left = k2;
		k2->_height = max(getHeight(k2->_left), getHeight(k2->_right)) + 1;
		k1->_height = max(getHeight(k1->_left), getHeight(k1->_right)) + 1;
		return k1;
	}


	template<typename T>
	template<typename _InputIterator>
	inline void avl_tree<T>::insert(_InputIterator first, _InputIterator last)
	{
		for (; first != last; ++first)
			insert(*first);
	}

	template<typename T>
	inline void avl_tree<T>::preOrderTraverse(const node* ptr, ostream &os = std::cout)
	{
		if (!ptr) return;
		os << ptr->data << ' ';
		preOrderTraverse(ptr->_left, os);
		preOrderTraverse(ptr->_right, os);
	}

	template<typename T>
	inline void avl_tree<T>::midOrderTraverse(const node* ptr, ostream &os = std::cout)
	{
		if (!ptr) return;
		midOrderTraverse(ptr->_left, os);
		os << ptr->data << ' ';
		midOrderTraverse(ptr->_right, os);
	}

	template<typename T>
	inline void avl_tree<T>::postOrderTraverse(const node* ptr, ostream &os = std::cout)
	{
		if (!ptr) return;
		postOrderTraverse(ptr->_left, os);
		postOrderTraverse(ptr->_right, os);
		os << ptr->data << ' ';
	}

	template<typename T>
	inline void avl_tree<T>::levelOrderTraverse(const node* ptr, ostream &os = std::cout)
	{
		auto tmp = ptr;
		if (tmp) {
			deque<node*> q;
			q.push_back(tmp);
			while (!q.empty()) {
				tmp = q.front();
				q.pop_front();
				os << tmp->_data;
				if (tmp->_left) q.push_back(tmp->_left);
				if (tmp->_right) q.push_back(tmp->_right);
			}
		}
	}

	template<typename T>
	inline void avl_tree<T>::insert(const T& val, node*& p)
	{
		if (!p) {
			//p = new node();
			p = data_allocator::allocate();
			data_allocator::construct(p);
			p->_data = val;
			p->_left = p->_right = nullptr;
			p->_height = 1;
			++_size;
		}
		else if (p->_data < val) {
			insert(val, p->_right);
			if (getHeight(p->_right) == getHeight(p->_left) + 2) {
				if (p->_right->_data < val) 
					p = singleRightRightRotate(p);
				else if (val < p->_right->_data)
					p = doubleRightLeftRotate(p);
			}
		}
		else if (val < p->_data) {
			insert(val, p->_left);
			if (getHeight(p->_right) == getHeight(p->_left) - 2) {
				if (val < p->_left->_data)
					p = singleLeftLeftRotate(p);
				else if (p->_left->_data < val)
					p = doubleLeftRightRotate(p);
			}

		}
		p->_height = max(getHeight(p->_left), getHeight(p->_right)) + 1;
	}

	template<typename T>
	inline void avl_tree<T>::erase(const T& val, node*& p)
	{

	}

	template<typename T>
	inline void avl_tree<T>::destroyAndDeallocate(node *p)
	{
		if (p) {
			destroyAndDeallocate(p->_left);
			destroyAndDeallocate(p->_right);
			data_allocator::destroy(p);
			data_allocator::deallocate(p);
		}
	}


}