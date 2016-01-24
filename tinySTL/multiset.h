#pragma once
#include "rbtree.h"
#include "algorithm.h"
#include "alloc.h"
#include "function.h"
//no reverse iterator

namespace tinySTL {
	template<typename Key, typename Compare = less<Key>, typename Alloc = alloc>
	class multiset {
		friend bool operator==(const multiset&, const multiset&);
		friend bool operator<(const multiset&, const multiset&);
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
		multiset() :_tree(Compare()) {}
		explicit multiset(const key_compare& comp) :_tree(comp) {}
		template<typename _InputIterator>
		multiset(_InputIterator first, _InputIterator last)
			: _tree(Compare()) {
			_tree.insert_equal(first, last);
		}

		template<typename _InputIterator>
		multiset(_InputIterator first, _InputIterator last, const Compare& comp)
			: _tree(comp) {
			_tree.insert_equal(first, last);
		}

		multiset(const multiset<Key, Compare, Alloc>& s) : _tree(s._tree) {}
		multiset<Key, Compare, Alloc>& operator=(const multiset<Key, Compare, Alloc>& s)
		{
			_tree = s._tree;
			return *this;
		}

		key_compare key_comp()const { return _tree._key_cmp; }
		value_compare value_comp()const { return _tree._key_cmp; }
		iterator begin()const { return _tree.begin(); }
		iterator end()const { return _tree.end(); }
		size_type size()const { return _tree.size(); }
		bool emtpy()const { return _tree.empty(); }

		void swap(multiset<Key, Compare, Alloc>& s) { _tree.swap(s._tree); }

		pair<iterator, bool>insert(const value_type& x) { return _tree.insert_equal(x); }
		iterator insert(iterator pos, const value_type& x) { return _tree.insert_equal(x).second; }
		void erase(iterator pos) { _tree.erase(pos); }
		void erase(const value_type x) { _tree.erase(x); }
		void clear() { _tree.clear(); }

		iterator find(const value_type& x)const { return _tree.find(x); }
		size_type count(const value_type& x)const { return _tree.count(x); }
		iterator lower_bound(const value_type& x)const { return _tree.lower_bound(); }
		iterator upper_bound(const value_type& x)const { return _tree.upper_bound(); }
		pair<iterator, iterator> equal_range(const value_type& x)const { return _tree.equal_range(); }
	};

	template<typename Key, typename Compare, typename Alloc>
	bool operator==(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y)
	{
		return x._tree == y._tree;
	}

	template<typename Key, typename Compare, typename Alloc>
	bool operator<(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y)
	{
		return x._tree < y._tree;
	}
}
