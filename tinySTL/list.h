#pragma once
#include "algorithm.h"
#include "iterator.h"
#include "reverseIterator.h"
#include "uninitializeFunctions.h"'
#include "typetraits.h"
#include "alloc.h"

namespace tinySTL {
	template <typename T>
	struct list_node {
		T data;
		list_node *next;
		list_node *prev;
		list_node(const T& d, list_node* p, list_node* n) : data(d), prev(p), next(n) {}
		bool operator==(const list_node& n)
		{
			return next == n.next && prev == n.prev && data == n.data;
		}
	};

	template<typename T, typename Ref, typename Ptr>
	struct list_iterator {
		typedef list_iterator<T, T&, T*>           iterator;
		typedef list_iterator<T, Ref, Ptr>         self;

		typedef bidirectional_iterator_tag		   iterator_category;
		typedef T                                  value_type;
		typedef Ptr                                pointer;
		typedef Ref                                reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;
		typedef list_node<T>*                      link_type;
		
		link_type _node;

		list_iterator(){}
		list_iterator(link_type n):_node(n){}
		list_iterator(const iterator& i):_node(i._node) {}//why not self

		bool operator==(const self& x)const { return _node == x._node; }
		bool operator!=(const self& x)const { return !(_node == x._node); }
		
		reference operator*()const { return _node->data; }
		pointer operator->()const { return &(operator*()); }

		self& operator++()
		{
			_node = _node->next;
			return *this;
		}

		self& operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--()
		{
			_node = _node->prev;
			return *this;
		}

		self& operator--(int)
		{
			auto tmp = *this;
			--*this;
			return *this;
		}
	};

	template<typename T, typename Alloc = alloc>
	class list {
	protected:
		typedef simple_alloc<list_node, Alloc>  list_node_allocator;

	protected:
		typedef list_node<T>                    link_node;
		typedef size_t                          size_type;
		typedef T&                              reference;
		typedef list_iterator::iterator         iterator;

	public:
		typedef list_node*                      link_type;

	protected:
		link_type get_node() { return list_node_allocator::allocate(); }
		void put_node(link_type p) { return list_node_allocator::deallocate(p); }
		link_type create_node(const T& x)
		{
			auto p = get_node();
			construct(&p->data, x);
			return p;
		}

		void destroy_node(link_type p)
		{
			destroy(&p->data);
			put_node();
		}

	public:
		list() { empty_initialize(); }

	protected:
		void empty_initialize()
		{
			_node = get_node();
			_node->next = _node;
			_node->prev = _node;
		}

	public:
		void push_back(const T& x) { insert(end(), x); }
		void push_front(const T& x) { insert(begin(), x); }
		void insert(iterator pos, const T& x)
		{
			auto tmp = create_node(x);
			tmp->_next = pos._node;
			tmp->_prev = pos._node->prev;
			pos._node->prev->next = tmp;
			pos._node->prev = tmp;
			return tmp;
		}

		iterator erase(iterator pos)
		{
			auto prev_node = link_type(pos._node->prev);
			auto next_node = link_type(pos._node->next);
			prev_node->next = next_node;
			prev_node->next = prev_node;
			destroy_node(pos._node);
			return iterator(next_node);
		}

		void pop_front() { erase(begin()); }
		void pop_back() { auto iter = end(); erase--iter; }
		void clear()
		{
			auto cur = begin();
			while (cur != _node) {
				auto tmp = cur;
				cur = cur->next;
				destroy_node(tmp);
			}
			_node->next = _node;
			_node->prev = _node;
		}

		void remove(cosnt T& val)
		{
			auto first = begin();
			auto last = end();
			while (first != last) {
				auto next = first;
				++next;
				if (*first == val) {
					erase(first);
				}
				first = next;
			}
		}

		void unique(cosnt T& val)
		{
			auto first = begin();
			auto last = end();
			if (first == last) return;
			auto next = first;
			while (++next != last) {
				if (*first == *next)
					erase(next);
				else
					first = next;
				next = first;
			}
		}

	public:
		iterator begin() { return (link_type)(_node->next); }
		iterator end() { return _node; }
		bool empty() { return _node->next == _node; }
		size_type size()const
		{
			size_type ret = 0;
			return distance(begin(), end());
		}
		reference front() { return *begin(); }
		reference back() { return *end(); }

	protected:
		void transfer(iterator pos, iterator first, iterator last)
		{
			if (position != last){
				last._node->prev->next = pos._node;
				first._node->prev->next = last._node;
				pos._node->prev->next = first._node;
				auto tmp = pos._node->prev;
				pos._node->prev = last._node->prev;
				last._node->prev = first._node->prev;
				first._node->prev = tmp;
			}
		}
	
	public:
		void splice(iterator pos, list &x) 
		{
			if (!x.empty())
				transfer(pos, x.begin(), x.end());
		}

		void splice(iterator pos, list &x, iterator i)
		{
			auto j = i++;
			if (j == pos || i == pos) return;
			transfer(pos, i, j);
		}
 
		void splice(iterator pos, list&, iterator first, iterator last)
		{
			if (first != last)
				transfer(pos, first, last);
		}

		void merge(list<T, Alloc>& x)
		{
			auto first1 = begin(), last1 = end();
			auto first2 = x.begin(), last2 = x.end();
			while (first1 != last1 && first2 != last2) {
				if (*first2 < *first1) {
					auto next = first2;
					transfer(first, first2, ++next);
					first2 = next;
				}
				else
					++first1;
			}
			if (first2 != last2)
				transfer(last1, first2, last2);
		}

		void reverse()
		{
			if (_node->next == _node || _node->next->next == _node) return;
			auto first = begin();
			++first;
			while (first != last) {
				auto tmp = first;
				++first;
				transfer(begin, old, first)
			}
		}
	};
}