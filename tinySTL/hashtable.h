#pragma once
#include "alloc.h"
#include "algorithm.h"
#include "vector.h"
//#include "typetraits.h"

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
	public:
		typedef hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			hashtable;
		typedef hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			iterator;
		typedef hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			const_iterator;
		typedef hashtable_node<Val> node;

		typedef forward_iterator_tag iterator_category;
		typedef Val value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Val& reference;
		typedef Val* pointer;
		
	private:
		node *_cur;
		hashtable *_ht;
		hashtable_iterator(node* n, hashtable* h)
			:_cur(n), _ht(h){}
		hashtable_iterator(){}
		reference operator*() { return _cur->_value; }
		pointer operator->() { return &(operator*()); }

		iterator& operator++();
		iterator operator++(int);
	

		bool operator==(const iterator& rhs)const
		{
			return _cur == rhs._cur;
		}
		bool operator!=(const iterator& rhs)const
		{
			return _cur != rhs->_cur;
		}
	};

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
	class hashtable_const_iterator
	{
	public:
		typedef hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			hashtable;
		typedef hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			iterator;
		typedef hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
			const_iterator;
		typedef hashtable_node<Val> node;

		typedef forward_iterator_tag iterator_category;
		typedef Val value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Val& reference;
		typedef Val* pointer;

	private:
		node *_cur;
		hashtable *_ht;
		hashtable_const_iterator(node* n, hashtable* h)
			:_cur(n), _ht(h) {}
		hashtable_const_iterator() {}
		reference operator*() { return _cur->_value; }
		pointer operator->() { return &(operator*()); }

		const_iterator& operator++();
		const_iterator operator++(int);


		bool operator==(const iterator& rhs)const
		{
			return _cur == rhs._cur;
		}
		bool operator!=(const iterator& rhs)const
		{
			return _cur != rhs->_cur;
		}
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
		friend class hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
		friend class hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>;

		typedef typename alloc_traits<Val, Alloc>::allocator_type allocator_type;

	public:
		typedef HashFunc                     hasher;
		typedef EqualKey                     key_equal;
		typedef size_t                       size_type;
		typedef Val                          value_type;
		typedef Key                          key_type;

	private:
		hasher _hash;
		key_equal _equals;
		ExtractKey _get_key;
		typedef hashtable_node<Val>           node;
		vector<node*, Alloc> _buckets;
		size_type _num_elements;

	public:
		typedef typename alloc_traits<Val, Alloc>::allocator_type allocator_type;
		allocator_type get_allocator() { return allocator_type(); }
	private:
		typename alloc_traits<node, Alloc>::allocator_type node_allocator;
		node* get_node() { return node_allocator.allocate(1); }
		node* new_node(const value_type& val)
		{
			auto n = get_node(); 
			construct(n, val); 
			return n;
		}
		void put_node(node* p) {return node_allocator.allocate(p, 1);}
		void destroy_node(node* p)
		{
			destroy(&p->_value);
			node_allocator.deallocate(p);
		}

	public:
		typedef hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>            iterator;
		typedef hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>      const_iterator;

		hasher hash_func() { return _hash; }
		key_equal key_eq() { return _equals; }

	public:
		hashtable(size_type n, 
				  const HashFunc& hf, 
				  const EqualKey& eql,
				  const allocator_type& a = allocator_type())//
			:_hash(hf), _equals(eql), _get_key(ExtractKey()), _num_elements(0)
		{
			initialize_buckets(n);
		}

		hashtable(size_type n,
			const HashFunc& hf,
			const EqualKey& eql,
			const ExtractKey& exk,
			const allocator_type& a = allocator_type())//
			:_hash(hf), _equals(eql), _get_key(exk), _num_elements(0)
		{
			initialize_buckets(n);
		}

		hashtable(const hashtable& ht)
			:_hash(ht._hash), _equals(ht._equals), _get_key(ht._get_key), _num_elements(0)
		{
			copy_from(ht);
		}

		hashtable& operator=(const hashtable& ht)
			: _hash(ht._hash), _equals(ht._equals), _get_key(ht._get_key), _num_elements(0)
		{
			if(this != &ht){
				clear();
				_hash = ht._hash;
				_equals = ht._equals;
				_get_key = ht._equals;
				copy_from(ht);
			}
			return *this;
		}

		size_type size() const { return _num_elements; }
		bool empty() const { return !_num_elements; }
		void swap(const hashtable& ht)
		{
			swap(_hash, ht._hash);
			swap(_equals, ht._equals);
			swap(_get_key, ht._equals);
			_buckets.swap(ht._buckets);
			swap(_num_elements, ht._num_elements);
		}

		iterator begin()
		{
			for (size_type i = 0; i != _buckets.size(); ++i)
				if (_buckets[i])
					return iterator(_buckets[i], this);
			return end();
		}
		iterator end() { return iterator(nullptr, this); }
		size_type bucket_count()const { return _buckets.size(); }
		size_type elements_in_bucket(size_type n)const
		{
			auto cur = _buckets[n];
			size_type cnt = 0;
			while(cur)
			{
				cur = cur->next;
				++cnt;
			}
			return cnt;
		}

		void initialize_buckets(size_type n)
		{
			const size_type n_buckets = _next_prime(n);
			_buckets.reserve(n_buckets);
			_buckets.insert(_buckets.end(), n_buckets, static_cast<node*>(nullptr));
			_num_elements = 0;
		}

		//tired to optimize
		pair<iterator, bool> insert_unique(const value_type& obj)
		{
			resize(_num_elements + 1);
			return insert_unique_aux(obj);
		}

		template<typename _InputIterator>
		void insert_unique(_InputIterator first, _InputIterator last)
		{
			//no random_access
			while(first != last)
				insert_unique(*first++);
		}

		iterator insert_equal(const value_type& obj)
		{
			resize(_num_elements + 1);
			return insert_equal_aux(obj);
		}

		template<typename _InputIterator>
		void insert_equal(_InputIterator first, _InputIterator last)
		{
			while (first != last)
				insert_equal(*first++);
		}

		pair<iterator, bool> insert_unique_aux(const value_type& obj)
		{
			auto n = bkt_num(obj);
			auto first = _buckets[n];
			for (auto pos = first; pos; pos = pos->next)
				if (equals(get_key(pos->_value), get_key(obj)))
					return{ iterator(pos, this), false };
			auto tmp = new node(obj);
			tmp->next = first;
			_buckets[n] = tmp->next;
			++_num_elements;
			return{ iterator(tmp, this), true };
		}

		iterator insert_equal_aux(const value_type& obj)
		{
			auto n = bkt_num(obj);
			auto first = _buckets[n];
			for (auto pos = first; pos; pos = pos->next)
				if (equals(get_key(pos->_value), get_key(obj)))
				{
					auto tmp = new_node(obj);
					tmp->next = first;
					_buckets[n] = tmp->next;
					++_num_elements;
					return iterator(tmp, this);
				}
					
			auto tmp = new node(obj);
			tmp->next = first;
			_buckets[n] = tmp->next;
			++_num_elements;
			return iterator(tmp, this);
		}

		void resize(size_type num_elements_hint)
		{
			const size_type old_sz = _buckets.size();
			if(num_elements_hint > old_sz)
			{
				auto new_sz = _next_prime(num_elements_hint);
				if(new_sz >old_sz)
				{
					vector<node*, Alloc> tmp(new_sz, nullptr);
					for (size_type bucket = 0; bucket < old_sz; ++bucket)
					{
						auto first = _buckets[bucket];
						while(first)
						{
							auto new_bucket = bkt_num(first->val, new_sz);
							_buckets[bucket] = first->next;
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = _buckets[bucket];
						}
					}
					_buckets.swap(tmp);
				}
			}
		}

		/*void find(const key_type)
		{}*/


		size_type bkt_num(const value_type& obj, size_type n)const
		{
			return bkt_num_key(get_key(obj), n);
		}

		size_type bkt_num(const value_type& obj)const
		{
			return bkt_num_key(get_key(obj));
		}

		size_type btk_num_key(const key_type& key)const
		{
			return bkt_num_key(key, _buckets.size());
		}
		
		size_type btk_num_key(const key_type& key, size_type n)const
		{
			return hash(key) % n;
		}

		void clear()
		{
			for (size_type i = 0; i < _buckets.size(); ++i)
			{
				auto cur = _buckets[i];
				while(cur)
				{
					auto tmp = cur;
					cur = cur->next;
					destroy_node(tmp);
				}
				_buckets[i] = nullptr;
			}
			_num_elements = 0;
		}

		void copy_from(const hashtable& ht)
		{
			//why?
			clear();
			for (size_type i = 0; i < _buckets.size(); ++i)
			{
				if(const auto cur = _buckets[i])
				{
					auto tmp = new_node(cur->_value);
					_buckets[i] = tmp;
					for (auto _next = cur->next; _next; cur = _next, _next = _next->next) {
						tmp->next = new_node(cur->_value);
						tmp = tmp->next;
					}
				}
			}
			_num_elements = ht._num_elements;
		}
		
	};

	//iterator
	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>& 
		hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++()
	{
		const auto old = _cur;
		_cur = _cur->next;
		if (!_cur)
		{
			auto bucket = _ht->bkt_num(old->_value);
			while (!_cur && ++bucket < _ht->buckets.size())
				_cur = _ht->buckets[bucket];
		}
		return *this;
	}

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
		hashtable_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++(int)
	{
		auto tmp = _cur;
		++_cur;
		return tmp;
	}

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>&
		hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++()
	{
		const auto old = _cur;
		_cur = _cur->next;
		if (!_cur)
		{
			auto bucket = _ht->bkt_num(old->_value);
			while (!_cur && ++bucket < _ht->buckets.size())
				_cur = _ht->buckets[bucket];
		}
		return *this;
	}

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>
		hashtable_const_iterator<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++(int)
	{
		auto tmp = _cur;
		++_cur;
		return tmp;
	}

	template<typename Val, typename Key, typename HashFunc, typename ExtractKey,
		typename EqualKey, typename Alloc>
		bool operator==(const hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>& a,
			const hashtable<Val, Key, HashFunc, ExtractKey, EqualKey, Alloc>& b)
	{
		return a.buckets == b.buckets;
	}
}