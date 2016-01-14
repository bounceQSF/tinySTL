#pragma once
#include "typetraits.h"

namespace tinySTL {
	template<typename T1, typename T2>
	void construct(T1* ptr, const T2& value){
		new(ptr) T1(value);
	}

	template<typename T>
	void destroy(T* ptr) {
		ptr->~T();
	}

	template<typename _ForwardIterator>
	void _destroy(_ForwardIterator first, _ForwardIterator last, _true_type){}

	template<typename _ForwardIterator>
	void _destroy(_ForwardIterator first, _ForwardIterator last, _false_type)
	{
		for (; first != last; ++first)
			destroy(&*first);
	}

	template<typename _ForwardIterator, typename T>
	void _destroy(_ForwardIterator first, _ForwardIterator last, T*)
	{
		using _trivial_destructor = typename _type_traits<T>::has_trivial_destructor;
		_destroy(first, last, _trivial_destructor());
	}
}