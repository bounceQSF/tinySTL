#pragma once
#include "alloc.h"
#include "algorithm.h"
#include "vector.h"
#include "typetraits.h"

namespace tinySTL
{
	template<typename T>
	class hashtable_node
	{
		hashtable_node* next;
		T _value;
	};

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
			 typename EqualKey, typename Alloc>
	class hashtable;

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable_iterator;

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable_const_iterator;

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable_iterator
	{
		
	};

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable_const_iterator
	{
		
	};

	enum {_num_primes = 28 };

	static const unsigned long _prime_list[_num_primes] =
	{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};

	inline unsigned long _next_prime(unsigned long n)
	{
		const auto first = _prime_list;
		const auto last = _prime_list + static_cast<int>(_num_primes);
		const auto pos = lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc = alloc>
	class hashtable; //why?

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		bool operator==(const hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>&,
			const hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>&);

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable
	{
		typedef typename alloc_traits<Val, Alloc>::allocator_type allocator_type;


	public:
		typedef HashFunc                     hasher;
		typedef EqualKey                     key_equal;
		typedef size_t                       size_type;

	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;
		typedef hashtable_node<Val>           node;
		typedef simple_alloc<node, alloc>     node_allocator;

		vector<node*, Alloc> buckets;
		size_type num_elements;

	public:
		typedef hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>            iterator;
		typedef hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>      const_iterator;

	public:
		hashtable()


		size_type bucket_count()const { return buckets.size(); }

	};




}