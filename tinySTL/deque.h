#pragma once
#include "alloc.h"
#include "uninitializeFunctions.h"

namespace tinySTL
{
	inline size_t deque_buf_size(size_t sz)
	{
		return sz < 512 ? size_t(512 / sz) : size_t(1);
	}
	template<typename T, typename Ref, typename Ptr>
	struct deque_iterator
	{
		typedef deque_iterator                                         self;
		typedef deque_iterator<T, T&, T*>                              iterator;
		typedef deque_iterator<T, const T&, const T*>                  const_iterator;
		typedef random_access_iterator_tag                             iterator_category;
		typedef T                                                      value_type;
		typedef Ptr                                                    pointer;
		typedef Ref                                                    reference;
		typedef size_t                                                 size_type;
		typedef ptrdiff_t                                              difference_type;
		typedef T**                                                    i_map_pointer;
		static size_type buffer_size() { return deque_buf_size(sizeof(T)); }
		

		T* _cur;
		T* _first;
		T* _last;
		i_map_pointer _node;

		deque_iterator(T* x, i_map_pointer y)
			:_cur(x), _first(*y), _last(*y + buffer_size()), _node(y){}
		deque_iterator()
			:_cur(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}
		deque_iterator(const deque_iterator& x)
			:_cur(x._cur), _first(x._first), _last(x._last), _node(x._node) {}
		reference operator*()const { return *_cur; }
		pointer operator->()const { return _cur; }
		difference_type operator- (const self& x)const
		{
			return (_cur - _first) + (x._last - x._cur) +
				difference_type(buffer_size())*(_node - x._node - 1);
		}

		self& operator++()
		{
			++_cur;
			if(_cur == _last)
			{
				set_node(++_node);
				_cur = _first;
			}
			return _cur;
		}

		self operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--()
		{
			
			if (_cur == _first)
			{
				set_node(--_node);
				_cur = _last;
			}
			--_cur;
			return _cur;
		}

		self operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			auto diff = n + (_cur - _first);
			if (diff < difference_type(buffer_size()) && diff >= 0)
				_cur += n;
			
			else
			{
				auto node_diff = diff > 0 ? node_diff / difference_type(buffer_size()) :
					-difference_type(-node_diff - 1) / difference_type(buffer_size()) - 1;
				set_node(_node + node_diff);
				_cur = _first + (diff - node_diff * difference_type(buffer_size()));
			}
				
			return *this;
		}
		self operator+(difference_type n)
		{
			auto tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) { return *this += (-n); }
		self operator-(difference_type n)
		{
			auto tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n)const { return *(*this + n); }

		bool operator==(const self& rhs)const { return _cur == rhs._cur; }
		bool operator!=(const self& rhs)const { return !(*this == rhs); }
		bool operator<(const self& rhs)const 
		{ 
			return (_node == rhs._node) ? (_cur < rhs._cur) : (_node < rhs._node);
		}
		bool operator>(const self& rhs)const { return rhs < *this; }
		bool operator<=(const self& rhs)const { return !(rhs < *this); }
		bool operator>=(const self& rhs)const { return !(*this < rhs); }

		void set_node(i_map_pointer new_node)
		{
			_node = new_node;
			_first = *new_node;
			_last = _first + difference_type(buffer_size());
		}
	};

	template<typename T, typename Ref, typename Ptr>
	random_access_iterator_tag iterator_category(const deque_iterator<T, Ref, Ptr>&)
	{
		return random_access_iterator_tag();
	}

	template <typename T, typename Alloc = alloc, size_t Bufsize = 0>
	class deque
	{
		//friend class deque_iterator<T, T&, T*>;
	public:
		typedef T                                              value_type;
		typedef deque_iterator<T, T&, T*>                      iterator;
		typedef deque_iterator<T, const T&, const T*>          const_iterator;
		//typedef reverse_iterator_t<T*>             iterator;
		//typedef reverse_iterator_t<const T*>       const_iterator;
		typedef iterator                                     pointer;
		typedef T&                                           reference;
		typedef const T&                                     const_reference;
		typedef size_t                                       size_type;
		typedef ptrdiff_t                                    difference_type;
		typedef pointer*                                     map_pointer;
		//typedef typename alloc_traits<T, Alloc>::allocator_type allocator_type;
		//allocator_type get_allocator_type()const { return }

	protected:
		//typedef typename alloc_traits<value_type, Alloc>::_alloc_type     node_alloc_type;
		//typedef typename alloc_traits<value_type*, Alloc>::_alloc_type    map_alloc_type;
		typedef simple_alloc<value_type, Alloc>               node_allocator;
		typedef simple_alloc<pointer, Alloc>                  map_allocator;

	protected:
		//node_alloc_type _node_allocator;
		//map_alloc_type _map_allocator;
		iterator _start;
		iterator _finish;
		map_pointer _map;
		size_type _map_size;
		enum {INITIALIZE_MAP_SIZE = 8};

	public:
		deque(int n, const value_type& value)
			:_start(), _finish(), _map(nullptr), _map_size(0)
		{
			fill_initialize(n, value);
		}

		//no need to construct
		T* allocate_node()
		{
			return node_allocator::allocate(deque_buf_size(sizeof(T)));
		}
		void deallocate_node(T* p)
		{
			node_allocator::deallocate(p);
		}

		map_pointer allocate_map(size_type n)
		{
			return map_allocator::allocate(n);
		}

		void deallocate_map(map_pointer p,size_type n)
		{
			map_allocator::deallocate(p, n);
		}
		void create_nodes(map_pointer nstart, map_pointer nfinish)
		{
			map_pointer cur;
			try {
				for (cur = nstart; cur != nfinish; ++cur)
					*cur = allocate_node();
			}
			catch (...)
			{
				destroy_nodes(nstart, cur);
			}
		}

		void destroy_nodes(map_pointer nstart, map_pointer nfinish)
		{
			for (auto cur = nstart; cur != nfinish; ++cur)
				deallocate_node(*cur);
		}

		void fill_initialize(size_type n, const value_type& value)
		{
			create_nodes(n);
			map_pointer cur;
			try
			{
				for (cur = _start._node; cur != _finish._node; ++cur)
					_uninitialized_fill(*cur, *cur + buffer_size(), value);
				_uninitialized_fill(_finish._first, _finish._cur, value);
			}
			catch(...)
			{
				destroy(_start, iterator(*cur, cur));
			}
		}

		void create_map_and_node(size_type num_elements)
		{
			auto num_nodes = num_elements / buffer_size();
			_map_size = max(size_type(INITIALIZE_MAP_SIZE), num_elements + 2);

			auto nstart = _map + (_map_size - num_nodes) / 2;
			auto nfinish = nstart + num_nodes - 1;
			create_nodes(nstart, nfinish);	
			_start.set_node(nstart);
			_finish.set_node(nfinish);
			_start._cur = _start._first;
			_finish._cur = _finish._first + num_elements % buffer_size();
		}



	public:
		iterator begin() { return _start; } //can not
		iterator end() { return _finish; }
		iterator cbegin()const { return _start; } //can not
		iterator cend()const { return _finish; }

		reference operator[](size_type n) { return _start[difference_type(n)]; }
		reference front() { return *_start; }
		reference back()
		{
			auto tmp = _finish;
			return *--tmp;
		}
		
		size_type size()const { return _finish - _start; }
		bool empty()const { return _start == _finish; }
		size_type buffer_size()const { return Bufsize; }
	};
}
