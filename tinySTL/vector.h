#pragma once

#include "algorithm.h"
#include "typetraits.h"
#include "alloc.h"
#include "reverseIterator.h"
#include "uninitializeFunctions.h"
#include "construct.h"

/***********vector************/
//reverse to be written
namespace tinySTL {
	template<typename T, typename Alloc = alloc>

	class vector {
	private:
		T* _first;
		T* _last;
		T* _free;
		typedef simple_alloc<value_type, Alloc> data_allocator;

	public:
		typedef T                                  value_type;
		typedef T*                                 iterator;
		typedef const T*                           const_iterator;
		typedef reverse_iterator_t<T*>             iterator;
		typedef reverse_iterator_t<const T*>       const_iterator;
		typedef iterator                           pointer;
		typedef T&                                 reference;
		typedef const T&                           const_reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;

	public:
		vector() : _first(nullptr), _last(nullptr), _free(nullptr) {}
		explicit vector(const size_type n)
		{
			allocateAndFillN(n, value_type());
		}

		vector(const size_type n, const value_type& value)
		{
			allocateAndFillN(n, value);
		}

		template<typename _InputIterator>
		vector(_InputIterator first, _InputIterator last) 
		{
			vector_aux(first, last, typename std::is_integral< _InputIterator>::type());
		}

		vector(const vector& v)
		{
			allocateAndCopy(v._first, v._last);
		}

		vector(vector&& v) :_first(v._first), _last(v._last), _free(v._free)
		{
			v._first = v._last = v._free = nullptr;
		}

		//remember to return *this, wtf
		vector& operator=(const vector& v)
		{
			if (this != &v) {
				allocateAndCopy(v._first, v._last);
			}
			return *this;
		}

		vector& operator=(vector&& v) 
		{
			if (this != &v) {
				destroyAndDeallocateAll();
				_first = v._first;
				_last = v._last;
				_free = v._free;
				v._first = v._last = v._free = nullptr;
			}
			return *this;
		}

		~vector()
		{
			destroyAndDeallocateAll();
		}

		//operator
		bool operator == (const vector& v)const
		{
			if (size() != v.size())
				return false;
			else {
				auto ptr1 = _first, ptr2 = _last;
				for (; ptr1 != ptr2; ++ptr1, ++ptr2) {
					if (*ptr1 != *ptr2)
						return false;
				}
				return true;
			}
		}

		bool operator != (const vector& v)const
		{
			return !(*this != v);
		}

		//iterator
		iterator begin() { return _first; }
		const_iterator begin() { return (_first); }
		const_iterator cbegin() { return (_first); }
		iterator end() { return (last_); }
		const_iterator end()const { return (last_); }
		const_iterator cend()const { return (last_); }
		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		const_reverse_iterator crbegin()const { return const_reverse_iterator(finish_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator crend()const { return const_reverse_iterator(start_); }

		//size
		difference_type size()const { return _last - _first; }
		difference_type capacity()const { return _free - _first; }
		bool empty()const { return start_ == finish_; }
		void resize(size_type n, const value_type& value)
		{
			if (n < size()) {
				dataAllocator::destroy(_first + n, _last);
				_last = first + n;
			}
			if (n > size()) {
				if (n <= capacity()) {
					auto diff = n - size();
					_last = _uninitialized_fill_n(_first, n, value);
				}
				else {
					auto diff = n - size();
					auto len = getNewCapacity(diff);
					auto newfirst = dataAllocator::allocate(len);
					auto res = _uninitialized_copy(_first, _last, newstart);
					res = _uninitialized_fill_n(res, n, value);
					destroyAndDeallocateAll();
					_first = newfirst;
					_last = res;
					_free = _first + len;
				}
			}

		}

		void reserve(size_type n)
		{
			if (n <= capacity()) return;
			auto diff = n - size();
			auto len = getNewCapacity(diff);
			auto newfirst = dataAllocator::Allocate(len);
			auto res = _uninitialized_copy(_first, _last, newstart);
			destroyAndDeallocateAll();
			_first = newfirst;
			_last = res;
			_free = _first + len;
		}

		void shrink_to_fit()
		{
		}

		//item
		reference operator[](const difference_type i) { return *(begin() + i); }
		const_reference operator[](const difference_type i)const { return *(cbegin() + i); }
		reference front() { return *(begin()); }
		reference back() { return *(end() - 1); }
		pointer data() { return begin(); }

		//modify
		void clear()
		{
			dataAllocator::destroy(_first, _last);
			_last = _first;
		}

		void swap(vector& v)
		{
			if (this != &v) {
				tinySTL::swap(_first, v._first);
				tinySTL::swap(_last, v._last);
				tinySTL::swap(_finish, v._finish);
			}
		}
		
		void push_back(const value_type& value)
		{
			if (!(_last < _free)){
				reserve(capacity() + 1);
			}
			construct(_last, value);
			++_last;
		}

		void push_back(value_type&& value)
		{
			if (!(_last < _free)) {
				reserve(capacity() + 1);
			}
			construct(_last, std::forward<value_type>(value));
			++_last;
		}

		void pop_back()
		{
			destroy(--_last);
		}

		iterator insert(const_iterator position, const value_type& val)
		{
			const auto index = position - _first;
			isnert(postion, 1, val);
			return begin() + index;
		}

		iterator insert(const_iterator position, size_type n, const value_type& val)
		{	
			insert_aux(position, n, val, typename std::is_integral<size_type>::type());
		}

		template <typename _InputIterator>
		iterator insert(const_iterator position, _InputIterator first, _InputIterator last)
		{
			insert_aux(position, _first, _last, typename std::is_integral<_InputIterator>::type());
		}

		iterator erase(const_iterator position)
		{

		}

		iterator erase(const_iterator first, iterator last)
		{

		}

		//alloc
		Alloc get_allocator() { return dataAllocator; }
	private:
		void destroyAndDeallocateAll()
		{
			dataAllocator::destroy(_first, _last);
			dataAllocator::deallocate(_first, _free);
		}

		void allocateAndFillN(const size_type& n, const value_type& value)
		{
			_first = dataAllocator::allocate(n);
			_last = _free =_uninitialized_fill_n(_first, n, value);
		}

		template<typename InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last)
		{
			_first = dataAllocator::allocate(n);
			_last = _free = _uninitialized_copy(first, last, _first);
		}


		template<typename InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type)
		{
			allocateAndCopy(first, last);
		}

		template<typename Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type)
		{
			allocateAndFillN(n, value);
		}

		template<typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type)
		{
			auto diffNow = _free - _last;
			auto diffIns = last - first;
			if (diffNow >= diffIns)
				if (_last - position > diffIns) {
					_uninitialized_copy(_last - diffIns, _last, _last);
					copy_backward(position, _last - diffIns, _last);
					copy(first, last, position);
				}
				else {
					auto tmp = _uninitialized_copy(first + (_last - position), last, _last);
					_uninitialized_copy(position, _last, tmp);
					copy(first, first + (_last - position), position);
				}
				_last += diffIns;
			else
				reallocateAndCopy(position, first, last);
		}

		template<typename Integer>
		void insert_aux(iterator position, Integer n, const value_type& value, std::true_type)
		{
			auto diffLeft = _last - _finish;
			if (n <= diffLeft) {
				for (auto tmp = _last - 1; tmp >= position; --tmp) {
					construct(tmp + n, *tmp);
				}
				_uninitialized_fill_n(position, n, value);
				_last += n;
			}

			else
				reallocateAndFillN(position, value);
		}

		template<typename InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last)
		{
			auto len = getNewCapacity(diffIns);
			auto newFirst = dataAllocator::allocate(len);
			auto newFree = newFirst + len;
			auto newLast = _uninitialized_copy(_first, position, newFirst);
			newLast = _uninitialized_copy(first, last, newLast);
			newLast = _uninitialized_copy(pos, _last, newLast);
			destroyAndDeallocateAll();
			_first = newLast;
			_last = newLast;
			_free = newFree;
		}

		void reallocateAndFillN(iterator position, const size_type& n, const value_type& val)
		{
			auto len = getNewCapacity(n);
			auto newFirst = dataAllocator::allocate(len);
			auto newFREE = newFirst + len;
			auto newLast = _uninitialized_copy(_first, position, newFirst);
			newLast = _uninitialized_fill_n(newLast, n, val);
			newLast = _uninitialized_copy(pos, _last, newLast);
			destroyAndDeallocateAll();
			_first = newLast;
			_last = newLast;
			_free = newFree;
		}

		size_type getNewCapacity(size_type len = capacity())const
		{
			auto cap = capacity();
			auto ret = max(cap, len);
			return cap ? (2 * ret) : (len ? len : 1);
		}

	public:
		template<typename T, typename Alloc>
		friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
		{
			if (v1.size() != v2.size())
				return false;
			for (auto iter1 = v1.begin(), iter2 = v2.begin(); iter1 != v2.end(); ++iter1, ++iter2)
				if (*iter1 != *iter2)
					return false;
			return true;
		}

		template<typename T, typename Alloc>
		friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
		{
			return !(v1 == v2);
		}
	};
}
