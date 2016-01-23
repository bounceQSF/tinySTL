#pragma once


#include "iterator.h"
#include "function.h"
#include "utility.h"
#include "typetraits.h"

#include <assert.h>
#include <memory>

namespace tinySTL {
	//fill
	template<typename _ForwardIterator, typename T>
	void fill(_ForwardIterator first, _ForwardIterator last, const T& value)
	{
		for (; first != last; ++first) {
			*first = value;
		}
	}
	
	inline void fill(char* first, char* last, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), last - first);
	}

	inline void fill(wchar_t* first, wchar_t* last, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), (last - first) * sizeof(wchar_t));
	}

	//fill_n
	template<typename output_iterator, typename size, typename T>//why template size rather size_t?
	output_iterator fill_n(output_iterator first, size n, const T& value)
	{
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;//has return value
	}

	template<typename size>
	char* fill_n(char* first, size n, const char& value)
	{
		for (; n > 0; --n, ++first) {

		}
		return first;
	}

	template<typename size>
	wchar_t* fill_n(wchar_t* first, size n, const wchar_t& value)
	{
		for (; n > 0; --n, ++first) {

		}
		return first;
	}

	//by "<"
	//max
	template<typename T>
	const T& max(const T& l, const T& r)
	{
		return (l < r) ? r : l;
	}

	template<typename T, typename Cmp>
	const T& max(const T& l, const T& r, Cmp c)
	{
		return c(l , r) ? r : l;
	}
	//min
	template<typename T>
	const T& min(const T& l, const T& r)
	{
		return !(r < l) ? l : r;
	}

	template<typename T, typename Compare>
	const T& min(const T& l, const T& r, Compare c)
	{
		return !c(r, l) ? r : l;
	}
	
	//all_of
	template<typename _InputIterator, typename T>
	bool all_of(_InputIterator first, _InputIterator last, const T& value)
	{
		for (; first != last; ++first)
			if (!pred(*first)) return false;
		return true;
	}

	//any_of
	//pred
	template<typename _InputIterator, typename UnaryPredicate>
	bool all_of(_InputIterator first, _InputIterator last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
			if (pred(*first)) return true;
		return false;
	}

	//none_of
	template<typename _InputIterator, typename UnaryPredicate>
	bool none_of(_InputIterator first, _InputIterator last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
			if (pred(*first)) return false;
		return true;
	}

	//for_each
	template<typename _InputIterator, typename Function>
	bool for_each(_InputIterator first, _InputIterator last, Function func)
	{
		for (; first != last; ++first)
			func(*first);
		return true;
	}

	//find
	template<typename _InputIterator, typename T>
	_InputIterator find(_InputIterator first, _InputIterator last, const T& value)
	{
		typedef typename iterator_traits<_InputIterator>::category category;
		return find(first, last, value, category());
	}

	template<typename _InputIterator, typename T>
	_InputIterator find(_InputIterator first, _InputIterator last, const T& value, input_iterator_tag)
	{
		while (first != last && !(*first == value))
			++first;
		return first;
	}

	template<typename _RandomIterator, typename T>
	_RandomIterator find(_RandomIterator first, _RandomIterator last, const T& value, random_access_iterator_tag)
	{
		auto count = last - first;
		while (count && *(last - count) != value)
			--count;
		return last - count;
	}

	//find_if
	template<typename _InputIterator, typename UnaryPredicate>
	_InputIterator find_if(_InputIterator first, _InputIterator last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
			if (pred(*first)) break;
		return first;
	}

	//find_if_not
	template<typename _InputIterator, typename UnaryPredicate>
	_InputIterator find_if_not(_InputIterator first, _InputIterator last, UnaryPredicate pred)
	{
		for (; first != last; ++first)
			if (!pred(*first)) break;
		return first;
	}

	//find_end
	template<typename _ForwardIterator1, typename _ForwardIterator2>
	_ForwardIterator1 find_end(_ForwardIterator1 first1, _ForwardIterator1 last1,
		_ForwardIterator2 first2, _ForwardIterator2 last2)
	{
		if (first2 == last2) return first1;
		_ForwardIterator1 res = last1;
		while(first1 != last1)
		{
			auto it1 = first1;
			auto it2 = first2;
			while (*it1 == *it2) {
				++it1, ++it2;
				if (it2 == last2) {
					res == first1;
					break;
				}
				if (it1 == last1) return res;
			}
			++first1;
		}
		return res;
	}

	template<typename _ForwardIterator1, typename _ForwardIterator2, typename UnaryPredicate>
	_ForwardIterator1 find_end(_ForwardIterator1 first1, _ForwardIterator1 last1,
			_ForwardIterator2 first2, _ForwardIterator2 last2, UnaryPredicate pred)
	{
		if (first2 == last2) return first1;
		_ForwardIterator1 res = last1;
		while (first1 != last1)
		{
			auto it1 = first1;
			auto it2 = first2;
			while (pred(*it1, *it2)) {
				++it1, ++it2;
				if (it2 == last2) {
					res == first1;
					break;
				}
				if (it1 == last1) return res;
			}
			++first1;
		}
		return res;
	}

	//find_first_of
	//the first iterator can just be inputIterator
	template<typename _InputIterator, typename _ForwardIterator>
	_InputIterator find_first_of(_InputIterator first1, _InputIterator last1,
		_ForwardIterator first2, _ForwardIterator last2)
	{
		for (; first1 != last1; ++first1) {
			for (auto it = first2; it != last2; ++it) {
				if (*first1 == *it) return first1;
			}
		}
		return last1;
	}

	template<typename _InputIterator, typename _ForwardIterator, typename UnaryPredicate>
	_InputIterator find_first_of(_InputIterator first1, _InputIterator last1,
		_ForwardIterator first2, _ForwardIterator last2, UnaryPredicate pred)
	{
		for (; first1 != last1; ++first1) {
			for (auto it = first2; it != last2; ++iter) {
				if (pred(*first1, *it)) return first1;
			}
		}
		return last1;
	}

	//adjacent_find
	template<typename _ForwardIterator, typename T>
	_ForwardIterator adjacent_find(_ForwardIterator first, _ForwardIterator last)
	{
		if (first == last) return last;
		auto next = first;
		++next;
		for (; next != last; ++next, ++first) {
			if (*first == *next) return first;
		}
		return last;
	}

	template<typename _ForwardIterator, typename UnaryPredicate>
	_ForwardIterator adjacent_find(_ForwardIterator first, _ForwardIterator last, UnaryPredicate pred)
	{
		if (first == last) return last;
		auto next = first;
		++next;
		for (; next != last; ++next, ++first) {
			if (pred(*first,*next)) return first;
		}
		return last;
	}

	//count
	//why use difference_type?
	template<typename _InputIterator, typename T>
	auto count(_InputIterator first, _InputIterator last, const T& value)//c++11?
	{
		typename iterator_traits<_InputIterator>::difference_type ret = 0;
		for (; first != last; ++first) {
			if (*first == value) ++ret;
		}
		return ret;
	}

	//count_if
	template<typename _InputIterator,typename UnaryPredicate>
	decltype(auto) count_if(_InputIterator first, _InputIterator last, UnaryPredicate pred)//c++14?
	{
		typename iterator_traits<_InputIterator>::difference_type ret = 0;
		for (; first != last; ++first) {
			if (pred(*first)) ++ret;
		}
		return ret;
	}

	//mismatch
	template<typename _InputIterator1, typename _InputIterator2>
	pair<_InputIterator1, _InputIterator2> mismatch(_InputIterator1 first1, _InputIterator1 last1, _InputIterator2 first2)
	{
		for (; last1 != first1; ++first1, ++first2) {
			if (*first1 != *first2)
				break;
		}
		return make_pair(first1, first2);
	}

	template<typename _InputIterator1, typename _InputIterator2, typename BinaryPredicate>
	pair<_InputIterator1, _InputIterator2> mismatch(_InputIterator1 first1, _InputIterator1 last1, _InputIterator2 first2, BinaryPredicate pred)
	{
		for (; last1 != first1; ++first1, ++first2) {
			if (!pred(*last1, *first2))
				break;
		}
		return make_pair(first1, first2);
	}
	//equal
	template<typename _InputIterator1, typename _InputIterator2>
	bool equal(_InputIterator1 first1, _InputIterator2 last1, _InputIterator2 first2)
	{
		return equal(first1, last1, first2, equal_to<typename iterator_traits<_InputIterator1>::value_type>());
	}

	template<typename _InputIterator1, typename _InputIterator2, typename BinaryPredicate>
	bool equal(_InputIterator1 first1, _InputIterator1 last1, _InputIterator2 first2, BinaryPredicate pred)
	{
		for (; last1 != first1; ++first1, ++first2) {
			if (!pred(*last1, *first2))
				return false;
		}
		return true;
	}

	//is_permutation
	template <typename _ForwardIterator1, typename _ForwardIterator2>
	bool is_permutation(_ForwardIterator1 first1, _ForwardIterator1 last1, _ForwardIterator2 first2)
	{
		auto res = mismatch(first1, last1, first2);
		first1 = res.first, first2 = res.second;
		if (first1 == last1) return true;
		auto last2 = first2;
		advance(last2, last1 - first1);
		for (auto iter = first1; iter != last1; ++iter) {
			if (find(first1, last1, *iter) = iter) {
				auto n = count(first2, last2, *iter);
				if (n == 0 || count(iter, last1, *iter) == n)
					return false;
			}
		}
		return true;

	}

	template <typename _ForwardIterator1, typename _ForwardIterator2, typename BinaryPredicate>
	bool is_permutation(_ForwardIterator1 first1, _ForwardIterator1 last1, _ForwardIterator2 first2, BinaryPredicate pred)
	{
		auto res = mismatch(first1, last1, first2, pred);
		first1 = res.first, first2 = res.second;
		if (first1 == last1) return true;
		auto last2 = first2;
		advance(last2, last1 - first1);
		for (auto iter = first1; iter != last1; ++iter)
		{
			auto cmp = [&](decltype(*first1) val) {return pred(val, *iter)};//decltype generates reference type
			if (find_if(first1, iter, cmp) == iter) {
				auto n = count_if(first2, last2, cmp);
				if (n == 0 || n == count_if(iter, last1, cmp)) return false;
			}
		}
		return true;
	}
	
	//search
	template <typename _ForwardIterator1, typename _ForwardIterator2>
	_ForwardIterator1 search(_ForwardIterator1 first1, _ForwardIterator1 last1,
		_ForwardIterator2 first2, _ForwardIterator2 last2)
	{
		while (first1 != last1)
		{
			auto iter1 = first1, iter2 = first2;
			while (iter1 != last1 && iter2 != last2) {
				if (iter1 == iter2) ++iter1, ++iter2;
				else break;
			}
			if (iter1 == last1) return last1;
			if (iter2 == last2) return last2;
			++first1;
		}
		return last1;
	}

	template <typename _ForwardIterator1, typename _ForwardIterator2, typename BinaryPredicate>
	_ForwardIterator1 search(_ForwardIterator1 first1, _ForwardIterator1 last1,
		_ForwardIterator2 first2, _ForwardIterator2 last2, BinaryPredicate pred)
	{
		while (first1 != last1)
		{
			auto iter1 = first1, iter2 = first2;
			while (iter1 != last1 && iter != last2) {
				if (pred(*iter1, *iter2)) ++iter1, ++iter2;
				else break;
			}
			if (iter1 == last1) return last1;
			if (iter2 == last2) return last2;
			++first1;
		}
		return last1;
	}
	
	//advance
	namespace{
		template<typename _InputIterator, typename Distance>
		void _advance(_InputIterator& it, Distance n, input_iterator_tag)
		{
			assert(n >= 0);
			while (n--)
				++it;
		}

		template<typename _BidirectIterator, typename Distance>
		void _advance(_BidirectIterator& it, Distance n, bidirectional_iterator_tag)
		{
			if (n < 0)
				while (n++)
					--it;
			else
				while (n--)
					++it;
		}

		template<typename _RandomIterator, typename Distance>
		void _advance(_RandomIterator& it, Distance n, random_access_iterator_tag)
		{
			if (n < 0)
				it -= (-n);
			else
				it += n;
		}

	}
	template<typename _InputIterator, typename Distance>
	void advance(_InputIterator &it, Distance n)
	{
		typedef iterator_traits<_InputIterator>::iterator_category iteraotr_category;
		_advance(it, n, iteraotr_category());
	}

	/******************sort*********************/
	const int stl_threhold = 16;

	template<typename _RandonIterator>
	void sort(_RandonIterator first, _RandonIterator last)
	{
		if (first != last) {
			introsort_loop(first, last, value_type(first), lg(last - first) * 2);
			final_insertion_sort(first, last);
		}
	}
	//intro_sort
	template<typename size>
	size lg(size n)
	{
		size k = 0;
		while (n > 1) {
			++k;
			n >>= 1;
		}
		return k;
	}

	template<typename _RandonIterator, typename T, typename size>
	void introsort_loop(_RandonIterator first, _RandonIterator last, T*, size dep_limit)
	{
		while (last - first > stl_threhold) {
			if (dep_limit == 0) {
				partial_sort(first, last, last);
				return;
			}
			--dep_limit;
			auto cut = unguarded_partition(first, last, T(median(*first, *((first + last) / 2), *(last - 1))));
			introsort_loop(cut, last, value(first), dep_limit);
			introsort_loop(first, cut, value(first), dep_limit);
		}
	}

	template<typename T>
	const T& median(const T&a, const T&b, const T&c)
	{
		if (a < b)
			if (b < c)
				return b;
			else if (c < a)
				return a;
			else
				return c;
		else if (a < c)
			return a;
		else if (b < c)
			return c;
		else
			return b;
	}

	template<typename _RandonIterator, typename T>
	_RandonIterator unguarded_partition(_RandonIterator first, _RandonIterator last, T pivot)
	{
		while (true) {
			while (*first < pivot) ++first;
			--last;
			while (pivot < *last) --last;
			if (!(first < last)) return first;
			iter_swap(first, last);
			++first;
		}
	}

	//insertion_sort
	template<typename _RandonIterator>
	void final_insertion_sort(_RandonIterator first, _RandonIterator last)
	{
		if (last - first > stl_threhold) {
			insertion_sort(first, first + stl_threhold);
			unguarded_insertion_sort(first + stl_threhold, last);
		}
		else
			insertion_sort(first, last);
	}

	template<typename _RandonIterator>
	void unguarded_insertion_sort(_RandonIterator first, _RandonIterator last)
	{
		unguarded_insertion_sort_aux(first, last, value_type(first));
	}

	template<typename _RandonIterator, typename T>
	void unguarded_insertion_sort_aux(_RandonIterator first, _RandonIterator last, T*)
	{
		for (auto iter = first; iter != last; ++iter)
			unguarded_linear_insert(iter, T(*iter));
	}

	template<typename _RandonIterator, typename T>
	void unguarded_linear_insert(_RandonIterator last, T value)
	{
		auto iter = last;
		--iter;
		while (value < *iter) {
			*last = *iter;
			last = iter;
			--next;
		}
		*last = value;
	}


	template<typename _RandonIterator>
	void insertion_sort(_RandonIterator first, _RandonIterator last)
	{
		if (first == last) return;
		for (auto iter = first + 1; iter != last; ++iter)
			linear_insert(first, iter, value_type(first));
	}

	template<typename _RandonIterator, typename T>
	void linear_insert(_RandonIterator first, _RandonIterator last, T*)
	{
		T value = *last;
		if (value < *first) {
			copy_backward(first, last, last + 1);
			*first = value;
		}
		else
			unguarded_linear_insert(last, value);
	}

	//partial_sort
	template<typename _RandomIterator>
	void partial_sort(_RandomIterator first, _RandomIterator middle, _RandomIterator last)
	{
		__partial_sort(first, middle, last, value_type(first));
	}

	template<typename _RandomIterator, typename T>
	void partial_sort(_RandomIterator first, _RandomIterator middle, _RandomIterator last, T*)
	{
		for (auto iter = middle; iter != last; ++iter) {
			if(*iter < *first)
				//pop_heap()
		}
	}

	//generate

	//generate_n

	//distance
	template<typename _InputIterator>
	typename iterator_traits<_InputIterator>::difference_type distance(_InputIterator first, _InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<_InputIterator>::difference_type n = 0;
		while (first != last) {
			++first, ++n;
		}
		return n;
	}

	template<typename _RandomIterator>
	typename iterator_traits<_RandomIterator>::difference_type distance(_RandomIterator first, _RandomIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template<typename _InputIterator>
	typename iterator_traits<_InputIterator>::difference_type distance(_InputIterator first, _InputIterator last)
	{
		typedef typename iterator_traits<_InputIterator>::iterator_category category;
		return distance(first, last, category());
	}


	//copy
	template<typename _BidirectIterator1, typename _BidirectIterator2>
	_BidirectIterator2 copy_backward(_BidirectIterator1 first, _BidirectIterator1 last, _BidirectIterator2 d_last)
	{
		for (; last != first; last, d_last) {
			--*d_last = --*last;
		}
		return d_last;
	}

	template<typename _InputIterator, typename _OutputIterator>
	_OutputIterator copy(_InputIterator first, _InputIterator last, _OutputIterator d_first)
	{
		return __copy_dispatch<_InputIterator, _OutputIterator>(first, last, d_first);
	}

	char* copy(const char* first, const char* last, char* d_first)
	{
		memmove(d_first, first, last - first);
		return d_first + (last - first);
	}
	wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* d_first)
	{
		memmove(d_first, first, sizeof(wchar_t) *(last - first));
		return d_first + (last - first);
	}

	template<typename _InputIterator, typename _OutputIterator>
	struct __copy_dispatch
	{
		_OutputIterator operator()(_InputIterator first, _InputIterator last, _OutputIterator d_first)
		{
			return __copy(first, last, d_first, iterator_category(first));
		}
	};

	template<typename T>
	struct __copy_dispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* d_first)
		{
			using t = typename _type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, d_first, t());
		}
	};

	template<typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* d_first)
		{
			using t = typename _type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, d_first, t());
		}
	};

	template<typename _InputIterator, typename _OutputIterator>
	_OutputIterator __copy(_InputIterator first, _InputIterator last, _OutputIterator d_first, input_iterator_tag)
	{
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}

	template<typename _RandomIterator, typename _OutputIterator>
	_OutputIterator __copy(_RandomIterator first, _RandomIterator last, _OutputIterator d_first, random_access_iterator_tag)
	{
		return __copy_d(first, last, d_first);
	}

	template<typename _InputIterator, typename _OutputIterator, typename UnaryPredicate>
	_OutputIterator copy_if(_InputIterator first, _InputIterator last,
		_OutputIterator d_first,
		UnaryPredicate pred)
	{
		for (; first != last; ++first) {
			if (UnaryPredicate(*first))
				*d_first++ = *first;
		}
		return d_first;
	}

	template<typename _RandomIterator, typename _OutputIterator>
	_OutputIterator __copy_d(_RandomIterator first, _RandomIterator last, _OutputIterator d_first)
	{
		for (auto n = last - first; n > 0; --n, ++result, ++first)
			*result = *first;
		return result;
	}

	template<typename T>
	T* __copy_t(const T* first, const T* last, T* d_first, _true_type)
	{
		memmove(d_first, first, (last - first)*sizeof(T));
		return __copy(first, last, d_first, t());
	}

	template<typename T>
	T* __copy_t(const T* first, const T* last, T* d_first, _false_type)
	{
		return __copy_d(first, lats, d_first, (ptrdiff_t*)0);
	}

	//copy_backward
	template<typename _BidirectIterator1, typename _BidirectIterator2>
	_BidirectIterator2 copy_backward(_BidirectIterator1 first, _BidirectIterator1 last,
		_BidirectIterator1 d_last)
	{
		return __copy_backward_dispatch(first, last, d_last);
	}

	template<typename _BidirectIterator1, typename _BidirectIterator2>
	_BidirectIterator2 __copy_backward_dispatch(_BidirectIterator1 first, _BidirectIterator1 last,
		_BidirectIterator1 d_last)
	{
		using t = typename iterator_traits<_BidirectIterator1>::iterator_category;
		return __copy_backward(first, last, d_last, t());
	}

	template<typename T>
	T* copy_backward_dispatch(T* first, T* last, T* d_last)
	{
		return __copy_backward_dispatch(first, last, d_last);
	}

	template<typename T>
	T* copy_backward_dispatch(const T* first, const T* last, T* d_last)
	{
		using t = typename _type_traits<T>::has_trivial_assignment_operator;
		return __copy_backward_t(first, last, d_last, t());
	}

	template<typename T>
	T* __copy_backward_t(const T* first, const T* last, T* d_last, _true_type)
	{
		auto diff = last - first;
		memmove(d_last - diff, first, (last - first)*sizeof(T));
		return d_last - diff;
	}

	template<typename T>
	T* __copy_backward_t(const T* first, const T* last, T* d_last, _false_type)
	{
		return __copy_backward_dispatch(first, last, d_last);
	}

	template<typename _BidirectIterator1, typename _BidirectIterator2>
	_BidirectIterator2 copy_backward_dispatch(_BidirectIterator1 first, _BidirectIterator1 last,
		_BidirectIterator1 d_last)
	{
		return __copy_backward_dispatch(first, last, d_last);
	}

	template<typename _BidirectIterator1, typename _BidirectIterator2>
	_BidirectIterator2 __copy_backward(_BidirectIterator1 first, _BidirectIterator1 last,
		_BidirectIterator1 d_last, bidirectional_iterator_tag)
	{
		for (; first != last; )
			*--d_last = *--last;
		return d_last;
	}

	template<typename _RandomIterator, typename _BidirectIterator>
	_RandomIterator __copy_backward(_RandomIterator first, _RandomIterator last,
		_BidirectIterator d_last, random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--d_last = *--last;
		return d_last;
	}

	//compare
	template<typename _InputIterator1, typename _InputIterator2>
	bool lexicographical_compare(_InputIterator1 first1, _InputIterator1 last1,
									 _InputIterator1 first2, _InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return first1 == last1 && first2 != last2;
	}

	template<typename _InputIterator1, typename _InputIterator2, typename Compare>
	bool lexicographical_compare(_InputIterator1 first1, _InputIterator1 last1,
		_InputIterator1 first2, _InputIterator2 last2, Compare cmp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (cmp(*first1, *first2)) return true;
			if (cmp(*first2, *first1)) return false;
		}
		return first1 == last1 && first2 != last2;
	}


	//swap
	template<typename _ForwardIterator1, typename _ForwardIterator2, typename T>
	void iter_swap(_ForwardIterator1 it1, _ForwardIterator2 it2, T*)
	{
		auto tmp = *it1;
		*it1 = *it2;
		*it2 = *it1;
	}

}