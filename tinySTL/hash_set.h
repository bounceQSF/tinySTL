#pragma once
#include "hashtable.h"
#include "hash_function.h"
//#include "function.h"
#include "alloc.h"

namespace tinySTL
{
	template<typename Val, typename HashFunc = hash<Val>, 
		typename EqualKey = equal_to<Val>, typename Alloc = alloc>
	class hash_set
	{
		friend bool operator==(const hash_set&, const hash_set&);
	private:
		typedef hashtable<Val, Val, HashFunc, identity<Val>, EqualKey, Alloc> ht;
		ht _rep;
		
	public:
		typedef ht::value_type                           value_type;
		typedef ht::key_type                             key_type;
		typedef ht::hasher                               hasher;
		typedef ht::key_equal                            key_equal;

		typedef typename ht::pointer                     pointer;
		typedef typename ht::const_pointer               const_pointer;
		typedef typename ht::reference                   reference;
		typedef typename ht::const_reference             const_reference;

		typedef typename ht::iterator                    iterator;
		typedef typename ht::const_iterator              const_iterator;
		//typedef typename ht::const_iterator              iterator;
		//typedef typename ht::const_iterator              const_iterator;
		typedef typename ht::difference_type             difference_type;
		typedef typename ht::size_type                   size_type;

		hasher hash_func()const { return rep.hash_func(); }
		hasher key_eq()const { return rep.key_eq(); }

	public:
		hash_set():_rep(100, hasher(),key_equal()) {}
		explicit hash_set(size_type n):_rep(n, hasher(), key_equal()) {}
		hash_set(size_type n, const hasher& hf):_rep(n, hf, key_equal()) {}
		hash_set(size_type n, const hasher& hf, const key_equal& eq) :_rep(n, hf, eq) {}

		template<typename _InputIterator>
		hash_set(_InputIterator first, _InputIterator last)
			: _rep(100, hasher(), key_equal())
		{
			_rep.insert_unique(first, last);
		}

		template<typename _InputIterator>
		hash_set(_InputIterator first, _InputIterator last, size_type n)
			: _rep(n, hasher(), key_equal())
		{
			_rep.insert_unique(first, last);
		}

		template<typename _InputIterator>
		hash_set(_InputIterator first, _InputIterator last, size_type n ,const hasher& hf)
			: _rep(n, hf, key_equal())
		{
			_rep.insert_unique(first, last);
		}

		template<typename _InputIterator>
		hash_set(_InputIterator first, _InputIterator last, size_type n, const hasher& hf,
			const key_equal& eql)
			: _rep(n, hf, eql)
		{
			_rep.insert_unique(first, last);
		}

	public:
		size_type size()const { return _rep.size(); }
		bool empty()const { return _rep.empty(); }
		void swap(const hash_set& rhs) { _rep.swap(rhs._rep); }
		iterator begin() { return _rep.begin(); }
		iterator end() { return _rep.end(); }


	public:
		void resize(size_type n) { _rep.resize(n); }
		size_type bucket_count()const { return _rep.bucket_count(); }
		size_type elements_in_bucket(size_type n) { return _rep.elements_in_bucket(n); }

	};
	
	template<typename Val, typename HashFunc, typename EqualKey, typename Alloc>
	bool operator==(const hash_set<Val, HashFunc, EqualKey, Alloc>& lhs, 
		const hash_set<Val, HashFunc, EqualKey, Alloc>& rhs)
	{
		return lhs._rep == rhs._rep;
	}
}
