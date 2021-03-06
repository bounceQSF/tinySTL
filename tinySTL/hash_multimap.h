﻿#pragma once
#include "hashtable.h"
#include "hash_function.h"
#include "alloc.h"

namespace tinySTL
{
	template<typename Key, typename T, typename HashFunc = hash<Key>,
		typename EqualKey = equal_to<Key>, typename Alloc = alloc>
	class hash_map
	{
		friend bool operator==(const hash_map&, const hash_map&);
	private:
		typedef hashtable<pair<const Key, T>, Key, HashFunc, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
		ht _rep;

	public:
		typedef T                                        data_type;
		typedef T                                        mapped_type;
		typedef typename ht::key_type                    key_type;
		typedef typename ht::value_type                  value_type;
		typedef typename ht::hasher                      hasher;
		typedef typename ht::key_equal                   key_equal;

		typedef typename ht::pointer                     pointer;
		typedef typename ht::const_pointer               const_pointer;
		typedef typename ht::reference                   reference;
		typedef typename ht::const_reference             const_reference;

		typedef typename ht::iterator                    iterator;
		typedef typename ht::const_iterator              const_iterator;
		typedef typename ht::difference_type             difference_type;
		typedef typename ht::size_type                   size_type;

		hasher hash_func()const { return _rep.hash_func(); }
		hasher key_eq()const { return _rep.key_eq(); }

	public:
		hash_map() :_rep(100, hasher(), key_equal()) {}
		explicit hash_map(size_type n) :_rep(n, hasher(), key_equal()) {}
		hash_map(size_type n, const hasher& hf) :_rep(n, hf, key_equal()) {}
		hash_map(size_type n, const hasher& hf, const key_equal& eq) :_rep(n, hf, eq) {}

		template<typename _InputIterator>
		hash_map(_InputIterator first, _InputIterator last)
			: _rep(100, hasher(), key_equal())
		{
			_rep.insert_equal(first, last);
		}

		template<typename _InputIterator>
		hash_map(_InputIterator first, _InputIterator last, size_type n)
			: _rep(n, hasher(), key_equal())
		{
			_rep.insert_equal(first, last);
		}

		template<typename _InputIterator>
		hash_map(_InputIterator first, _InputIterator last, size_type n, const hasher& hf)
			: _rep(n, hf, key_equal())
		{
			_rep.insert_equal(first, last);
		}

		template<typename _InputIterator>
		hash_map(_InputIterator first, _InputIterator last, size_type n, const hasher& hf,
			const key_equal& eql)
			: _rep(n, hf, eql)
		{
			_rep.insert_equal(first, last);
		}

	public:
		size_type size()const { return _rep.size(); }
		bool empty()const { return _rep.empty(); }
		void swap(const hash_map& rhs) { _rep.swap(rhs._rep); }
		iterator begin()const { return _rep.begin(); }
		iterator end()const { return _rep.end(); }

	public:
		pair<iterator, bool> insert(const value_type& obj) { return _rep.insert_equal(obj); }
		template<typename _InputIterator>
		void insert(_InputIterator first, _InputIterator last) { _rep.insert_equal(first, last); }
		pair<iterator, bool> insert_noresize(const value_type& obj) { return _rep.insert_equal_aux(obj); }

		iterator find(const key_type& key)const { return _rep.find(key); }
		size_type count(const key_type& key)const { return _rep.count(key); }
		pair<iterator, iterator> equal_range() { return _rep.equal_range(); }
		pair<const_iterator, const_iterator> equal_range()const { return _rep.equal_range(); }


		size_type erase(const key_type& key) { return _rep.erase(key); }
		void erase(const iterator& it) { _rep.erase(it); }
		void erase(iterator first, iterator last) { _rep.erase(first, last); }
		void clear() { _rep.clear(); }

	public:
		void resize(size_type n) { _rep.resize(n); }
		size_type bucket_count()const { return _rep.bucket_count(); }
		size_type elements_in_bucket(size_type n) { return _rep.elements_in_bucket(n); }

	};

	template<typename Key, typename HashFunc, typename EqualKey, typename Alloc>
	bool operator==(const hash_map<Key, HashFunc, EqualKey, Alloc>& lhs,
		const hash_map<Key, HashFunc, EqualKey, Alloc>& rhs)
	{
		return lhs._rep == rhs._rep;
	}
}
