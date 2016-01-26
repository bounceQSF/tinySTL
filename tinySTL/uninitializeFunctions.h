#pragma once
#include "algorithm.h"
#include "construct.h"
#include "iterator.h"
#include "typetraits.h"

namespace tinySTL {
	/*****************copy******************/
	template<typename _InputIterator, typename _ForwardIterator>
	_ForwardIterator _uninitialized_copy_aux(_InputIterator first, _InputIterator last,
		_ForwardIterator result, _true_type);

	template<typename _InputIterator, typename _ForwardIterator>
	_ForwardIterator _uninitialized_copy_aux(_InputIterator first, _InputIterator last,
		_ForwardIterator result, _false_type);

	template<typename _InputIterator, typename _ForwardIterator>
	_ForwardIterator _uninitialized_copy(_InputIterator first, _InputIterator last,
		_ForwardIterator result)
	{
		using isPODtype = typename _type_traits<iterator_traits<_InputIterator>::value_type>::is_POD_type;
		_uninitialized_copy_aux(first, last, isPODtype());
	}

	template<typename _InputIterator, typename _ForwardIterator>
	_ForwardIterator _uninitialized_copy_aux(_InputIterator first, _InputIterator last,
		_ForwardIterator result, _true_type)
	{
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	template<typename _InputIterator, typename _ForwardIterator>
	_ForwardIterator _uninitialized_copy_aux(_InputIterator first, _InputIterator last,
		_ForwardIterator result, _false_type)
	{
		auto iter = result;//in case copy failed
		for (; first != last; ++last, ++iter) {
			construct(&*iter, *first); 
		}
	}

	/*****************fill******************/
	template<typename _ForwardIterator, typename T>
	void _uninitialized_fill_aux(_ForwardIterator first, _ForwardIterator last,
		const T& value, _true_type);

	template<typename _ForwardIterator, typename T>
	void _uninitialized_fill_aux(_ForwardIterator first, _ForwardIterator last,
		const T& value, _false_type);

	template<typename _ForwardIterator, typename size, typename T>
	void _uninitialized_fill(_ForwardIterator first, _ForwardIterator last,
		const T& value)
	{
		using isPODtype = typename _type_traits<iterator_traits<_ForwardIterator>::type_value>::is_POD_type;
	}


	template<typename _ForwardIterator, typename T>
	void _uninitialized_fill_aux(_ForwardIterator first, _ForwardIterator last,
		const T& value, _true_type)
	{
		fill(first, last, value);
	}

	template<typename _ForwardIterator, typename size, typename T>
	void _uninitialized_fill_aux(_ForwardIterator first, _ForwardIterator last,
		const T& value, _false_type)
	{
		for (auto iter = first; iter != last; ++iter)
			construct(&*iter, value);
	}

	/*****************fill_n******************/
	template<typename _ForwardIterator, typename size, typename T>
	_ForwardIterator _uninitialized_fill_n_aux(_ForwardIterator first, size n,
		const T& value, _true_type);

	template<typename _ForwardIterator, typename size, typename T>
	_ForwardIterator _uninitialized_fill_n_aux(_ForwardIterator first, size n,
		const T& value, _false_type);

	template<typename _ForwardIterator, typename size, typename T>
	_ForwardIterator _uninitialized_fill_n(_ForwardIterator first, size n,
		const T& value)
	{
		using isPODtype = typename _type_traits<iterator_traits<_ForwardIterator>::type_value>::is_POD_type;
		_uninitialized_fill_n_aux(first, n, value);
	}

	template<typename _ForwardIterator, typename size, typename T>
	_ForwardIterator _uninitialized_fill_n_aux(_ForwardIterator first, size n,
		const T& value, _true_type)
	{
		return fill_n(first, first + n, value);

	}

	template<typename _ForwardIterator, typename size, typename T>
	_ForwardIterator _uninitialized_fill_n_aux(_ForwardIterator first, size n,
		const T& value, _false_type)
	{
		auto iter = first;
		for (auto i = 0; i < n; ++i, ++iter)
			construct(&*iter, value);
		return iter;
	}

}