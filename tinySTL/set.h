#pragma once
#include "rbtree.h"
#include "algorithm.h"
#include "alloc.h"
#include "function.h"
//no reverse iterator

namespace tinySTL {
	template<typename Key, typename Compare = less<Key>, typename Alloc = alloc>
	class set {
	public:
		typedef Key                                     key_type;
		typedef Key                                     value_type;
		typedef Compare                                 key_compare;
		typedef Compare                                 value_compare;

	private:
		typedef rb_tree<key_type, value_type, 
			identity<value_type>, Alloc>                rep_type;
		rep_type _tree;
	
	public:
		typedef typename rep_type::pointer              pointer;
		typedef typename rep_type::const_pointer        const_pointer;
		typedef typename rep_type::reference            reference;
		typedef typename rep_type::const_reference      const_reference;

		typedef typename rep_type::iterator             iterator;
		typedef typename rep_type::const_iterator       const_iterator;
		//typedef typename rep_type::const_iterator       iterator;
		//typedef typename rep_type::const_iterator       const_iterator;
		typedef typename rep_type::difference_type      difference_type;
		typedef typename rep_type::size_type            size_type;

	public:
		set():_tree(Compare()){}
		explicit set(const key_compare& comp) :_tree(comp) {}
		template<typename _InputIterator>
		set(_InputIterator first, _InputIterator last)
			:_tree(Compare()) {_tree.insert_unique(first, last);}

		template<typename _InputIterator>
		set(_InputIterator first, _InputIterator last, const Compare& comp)
			: _tree(cmp) {
			_tree.insert_unique(first, last);
		}

		set(const set<Key, Compare, Alloc>& s): _tree(s._tree){}
		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& s)
		{
			_tree = x._tree;
			return *this;
		}

		key_compare key_comp()const { return _tree._key_cmp; }
		value_compare value_comp()const { return _tree._key_cmp; }
		iterator begin()const { return _tree.begin(); }
		iterator end()const { return _tree.end(); }
		size_type size()const { return _tree.size(); }
		bool emtpy()const { return _tree.empty(); }
		size_type size()const { return _tree.size(); }
		void swap(set<Key, Compare, Alloc>& s) { _tree.swap(s._tree); }
		
		pair<iterator, bool>insert(const value_type& x) { return _tree.insert_unique(x); }
		iterator insert(const value_type& x) { return _tree.insert_unique(x).sec; }

	};

}
