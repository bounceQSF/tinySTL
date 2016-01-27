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
			create_map_and_node(n);
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
			auto num_nodes = num_elements / buffer_size() + 1;
			_map_size = max(size_type(INITIALIZE_MAP_SIZE), num_elements + 2);
			_map = map_allocator::allocate(_map_size);

			auto nstart = _map + (_map_size - num_nodes) / 2;
			auto nfinish = nstart + num_nodes - 1;
			create_nodes(nstart, nfinish);	
			_start.set_node(nstart);
			_finish.set_node(nfinish);
			_start._cur = _start._first;
			_finish._cur = _finish._first + num_elements % buffer_size();
		}

	public:
		void push_back(const value_type& value)
		{
			if (_finish._last - 1 != _finish._cur)
				construct(_finish._cur++, value);
			else
				push_back_aux(value);
		}

		void push_back_aux(const value_type& value)
		{
			reserve_map_at_back();
			*(_finish._node + 1) = allocate_node();
			try
			{
				construct(_finish._cur, value);
				_finish.set_node(++_finish._node);
				_finish._cur = _finish._first;
			}
			catch(...)
			{
				deallocate_node(*(_finish._node + 1));
			}
		}

		void push_front(const value_type& value)
		{
			if (_start._first != _start._cur)
				construct(_finish._cur--, value);
			else
				push_front_aux(value);
		}

		void push_front_aux(const value_type& value)
		{
			reserve_map_at_front();
			*(_start._node - 1) = allocate_node();
			try
			{
				_start.set_node(_start._node);
				_start._cur = _start._cur - 1;
				construct(_start._last, value);
			}
			catch (...)
			{
				_start.set_node(_start._node + 1);
				_start._cur = _start._first;
				deallocate_node(*--_start._node);
			}
		}


		void pop_back()
		{
			if (_finish._cur != _finish._first) {
				--_finish._cur;
				destroy(_finish._cur);
			}
			else
				pop_back_aux();
	
		}

		void pop_back_aux()
		{
			deallocate_node(_finish._node);
			_finish.set_node(--_finish._node);
			_finish._cur = _finish._last--;
			destroy(_finish._cur);
		}

		void pop_front()
		{
			if (_start._cur != _start._last - 1)
			{
				destroy(_start._cur);
				++_start._cur;
			}
			else
				pop_front_aux();
			
		}

		void pop_front_aux()
		{
			destroy(_start._cur);
			deallocate_node(_start._node);
			_start.set_node(++_start._node);
			_start._cur = _start._first;
		}

		void clear()
		{
			for (auto node = _start._node + 1; node != _finish._node; ++node) {
				destroy(*node, *node + buffer_size());
				node_allocator::deallocate(*node, buffer_size());
			}
			if (_start._node != _finish._node)
			{
				destroy(_start._cur, _start._last);
				destroy(_finish._first, _finish._cur);
				node_allocator::deallocate(_finish._first, buffer_size());//keep head buffer;
			}
			else
				destroy(_start._cur, _finish._cur);//keep the only buffer
			_finish = _start;
		}

		iterator erase(iterator pos)
		{
			iterator next = pos;
			++next;
			auto index = pos - _start;
			if(index < (size() >> 1))
			{
				copy_backward(_start, pos, next);
				pop_front();
			}
			else
			{
				copy(next, _finish, pos);
				pop_back();
			}
			return _start + index;
		}

		iterator erase(iterator first, iterator last)
		{
			if (first == _start && last == _finish) {
				clear();
				return _finish;
			}
			auto n = last - first;
			auto elem_before = first - _start;
			if (elem_before < (size() - n) / 2)
			{
				copy_backward(_start, first, last);
				auto new_start = _start + n;
				destroy(_start, new_start);
				for (auto cur = _start._node; cur != new_start._node; ++cur)
					node_allocator::deallocate(*cur, buffer_size());
				_start = new_start;
			}
			else
			{
				copy(last, _finish, first);
				auto new_finish = _finish - n;
				destroy(new_finish, _finish);
				for (auto cur = new_finish._node; cur != _finish._node; ++cur)
					node_allocator::deallocate(*cur, buffer_size());
				_finish = new_finish;
			}
			return _start + elem_before;
		}

		void reserve_map_at_back(size_type nodes_to_add = 1)
		{
			if (nodes_to_add + 1 > _map_size - (_finish._node - _map))
				reallocate_map(nodes_to_add, false);
		}

		void reserve_map_at_front(size_type nodes_to_add = 1)
		{
			if (nodes_to_add > _start._node - _map)
				reallocate_map(nodes_to_add, true);
		}

		void reallocate_map(size_type nodes_to_add, bool add_front)
		{
			auto old_num_nodes = _finish._node - _start._node + 1;
			auto new_num_nodes = old_num_nodes + nodes_to_add;
		
			map_pointer new_nstart;
			if(_map_size > 2 * new_num_nodes)
			{
				new_nstart = _map + (_map_size - new_num_nodes) / 2 + (add_front ? nodes_to_add : 0);
				if (new_nstart < _start._node)
					copy(_start._node, _finish._node + 1, new_nstart);
				else
					copy_backward(_start, _finish._node + 1, new_nstart + old_num_nodes);
			}
			else
			{
				auto new_map_size = _map_size + max(_map_size, nodes_to_add) + 2;
				auto new_map = map_allocator::allocate(new_map_size);
				new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_front ? nodes_to_add : 0);
				copy(_start._node, _finish._node + 1, new_nstart);
				map_allocator::deallocate(_map, _map_size);
				_map = new_map;
				_map_size = new_map_size;
			}
			_start.set_node(new_nstart);
			_finish.set_node(new_nstart + old_num_nodes - 1);
		}

		iterator insert(iterator pos, const value_type& value)
		{
			if (pos._cur == _start._cur) {
				push_front(value);
				return _start;
			}
			if (pos._cur == _finish._cur) {
				push_back(value);
				return _finish - 1;
			}
			return insert_aux(pos, value);
		}

		iterator insert_aux(iterator pos, const value_type& value)
		{
			auto index = pos - _start;
			if (index < size() / 2)
			{
				push_front(front());
				auto front1 = _start;
				++front1;
				auto front2 = front1;
				++front2;
				pos = _start + index;
				iterator pos1 = pos;
				++pos1;
				copy(front2, pos1, front1);
			}
			else
			{
				push_back(back());
				auto back1 = _finish;
				--back1;
				auto back2 = back1;
				--back2;
				pos = _start + index;
				iterator pos1 = pos;
				++pos1;
				copy(pos, back2, back1);
			}
			*pos = x;
			return pos;
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
