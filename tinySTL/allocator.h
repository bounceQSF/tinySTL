#pragma once
#include "construct.h"
#include <new>
#include <iostream>

namespace tinySTL
{
	template<typename T>
	T* allocate(ptrdiff_t size, T*) {
		set_new_handler(0);
		auto tmp = static_cast<T *>(::operator new((size_t)(size * sizeof(T))));
		if (!tmp) {
			std::cerr << "out of memory" << endl;
			exit(1);
		}
		return tmp;
	}

	template<typename T>
	void deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	template<typename T>
	class allocator {
	public:
		typedef T                                  value_type;
		typedef T*                                 pointer;
		typedef const T*                           const_pointer;
		typedef T&                                 reference;
		typedef const T&                           const_reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;

		static pointer allocate(size_type n)
		{
			return tinySTL::allocate(static_cast<difference_type>(n), nullptr);
		}

		static void deallocate(pointer p) { tinySTL::deallocate(p); }
		static void destroy(pointer fisrt, pointer last)
		{
			for (; first != last; ++first) {
				first->~T();
			}
		}

		static void construct(pointer fisrt, const value_type& value)
		{
			new(ptr)T(value);
		}

		pointer address(reference x) { return (pointer)&x; }
		const_pointer address(const_reference x) { return (const_pointer)&x; }
	};


}