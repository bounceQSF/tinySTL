#pragma once
#include "rbtree.h"
#include "alloc.h"
#include  "function.h"

namespace tinySTL
{
	template<typename Key, typename T, typename Compare = less<Key>, typename Alloc = alloc>
	class map
	{
		friend bool operator==(const map&, const map&);
		friend bool operator<(const map&, const map&);
	public:
		typedef Key                                     key_type;
		typedef T                                       data_type;
		typedef T                                       mapped_type;
		typedef pair<const Key, T>                      value_type;
		typedef Compare                                 key_compare;
	
		class value_compare:
			public binary_function<value_type, value_type, bool>
		{
			friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			explicit value_compare(Compare c) : comp(c){}

		public:
			bool operator()(const value_type& x, const value_type& y)const
			{
				return comp(x.first, y.first);
			}
		};

	private:
		typedef rb_tree<key_type, value_type,
			select1st<value_type>, key_compare, Alloc>  rep_type;
		rep_type _tree;

	public:
		typedef typename rep_type::pointer              pointer;
		typedef typename rep_type::const_pointer        const_pointer;
		typedef typename rep_type::reference            reference;
		typedef typename rep_type::const_reference      const_reference;
		typedef typename rep_type::iterator             iterator;
		typedef typename rep_type::const_iterator       const_iterator;
		//typedef typename rep_type::const_iterator     iterator;
		//typedef typename rep_type::const_iterator     const_iterator;
		typedef typename rep_type::difference_type      difference_type;
		typedef typename rep_type::size_type            size_type;

	public:
		//allocation/deallcation
		map():_tree(Compare()){}
		explicit map(const Compare& cmp):_tree(cmp){}
		
		template<typename _InputIterator>
		map(_InputIterator first, _InputIterator last) { _tree.insert_unique(first, last); }
		
		template<typename _InputIterator>
		map(_InputIterator first, _InputIterator last, Compare cmp)
		:_tree(cmp){ _tree.insert_unique(first, last); }

		map(const map<Key, T, Compare, Alloc>& s):_tree(s._tree) {}
		map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& s)
		{
			_tree = s._tree;
			return *this;
		}

		key_compare key_comp()const { return _tree.key_comp(); }
		value_type value_comp() const { return value_compare(_tree.key_comp()); }
		iterator begin() { return _tree.begin(); }
		iterator end() { return _tree.end(); }
		const_iterator begin()const { return _tree.begin(); }
		const_iterator end() const { return _tree.end(); }
		bool empty()const { return _tree.empty(); }
		size_type size()const { return _tree.size(); }
		T &operator[](const Key& x) { return (*((_tree.insert(value_type(x, T()))).first)).second; }
		void swap(map<Key, T, Compare, Alloc>& m) { _tree.swap(m._tree); }

		pair<iterator, bool> insert(const value_type& x) { return _tree.insert_unique(x); }
		iterator insert(iterator pos, const value_type& x) { return _tree.insert_unique(pos, x); }
		template<typename _InputIterator>
		void insert(_InputIterator first, _InputIterator last) { _tree.insert_unique(first, last); }

		void erase(iterator pos) { _tree.erase(pos); }
		size_type erase(const key_type& x) { return _tree.erase(x); }
		template<typename _InputIterator>
		void erase(iterator first, iterator last) { _tree.erase(first, last); }
		void clear() { _tree.clear(); }

		iterator find(){ return _tree.find();}
		const_iterator find()const { return _tree.find(); }
		size_type count(const value_type& x)const { return _tree.count(x); }
		iterator lower_bound(const value_type& x) { return _tree.lower_bound(x); }
		const_iterator lower_bound(const value_type& x)const { return _tree.lower_bound(x); }
		iterator upper_bound(const value_type& x) { return _tree.upper_bound(x); }
		const_iterator upper_bound(const value_type& x)const { return _tree.upper_bound(x); }
		iterator equal_range(const value_type& x){ return _tree.equal_range(x); }
		const_iterator equal_range(const value_type& x)const { return _tree.equal_range(x); }
	};

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator==(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{
		return x._tree == y._tree;
	}

	template<typename Key, typename T, typename Compare, typename Alloc>
	bool operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{
		return x._tree < y._tree;
	}
}
