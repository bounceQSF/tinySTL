#pragma once
#include "algorithm.h"
#include "iterator.h"
#include "reverseIterator.h"
#include "uninitializeFunctions.h"'
#include "typetraits.h"

namespace tinySTL {
	template <typename T>
	struct list_node {
		T data;
		list_node *next;
		list_node *prev;
		list_node(const T& d, list_node* p, list_node* n) : data(d), prev(p), next(n) {}
		bool operator==(const node&n)
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
		typedef Ptr                                iterator;
		typedef Ref                                reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;
		typedef list_node<T>*                      link_type;
		
		link_type node;

		list_iterator(){}
		list_iterator(link_type n):node(n){}
		list_iterator(const iterator& i):node(i.node) {}//why not self

		bool operator==(const self& x)const
	};


}