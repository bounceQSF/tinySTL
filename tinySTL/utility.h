#pragma once
#include <utility>
#include "algorithm.h"

namespace tinySTL {
	template<typename T>
	void swap(T& l, T& r)
	{
		T temp = l;
		l = r;
		r = temp;
	}

	template<typename T1, typename T2>
	struct pair {
	public:
		typedef T1 first_type;
		typedef T2 second_type;

	public:
		T1 first;
		T2 second;

	public:
		pair() {}
		template<typename U, typename V>
		pair(const pair<U, V>& p);
		//template<typename U, typename V>
		//pair(pair<U, V>&& p);
		//pair(first_type&& a, second_type&& b);

		pair(const first_type&, const second_type&);
		pair& operator= (const pair& p);
		void swap(pair& p);

	public:
		template <typename T1, typename T2>
		friend bool operator== (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend bool operator!= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend bool operator<  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend bool operator<= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend bool operator>  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend bool operator>= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template <typename T1, typename T2>
		friend void swap(pair<T1, T2>& x, pair<T1, T2>& y);
	};

	template<typename T1, typename T2>
	template<typename U, typename V>
	pair<T1, T2>::pair(const pair<U, V>& p) :
		first(p.first), second(p.second) {}

	template<typename T1, typename T2>
	pair<T1, T2>::pair(const first_type& a, const second_type& b):
		first(a), second(b) {}
		

	//i think it's proper to use forward but

	//pair(first_type&& a, second_type&& b):
	//	first(std::forward<first_type>(a)), first(std::forward<second_type>(b)){}

	template<typename T1, typename T2>
	pair<T1, T2>& pair<T1, T2>::operator= (const pair& p)
	{
		if (this != &p) {
			first = p.first;
			second = p.second;
		}
		return *this;
	}

	template<typename T1, typename T2>
	void pair<T1, T2>::swap(pair<T1, T2>& p)
	{
		tinySTL::swap(first, p.first);
		tinySTL::swap(second, p.second);
	}

	template <typename T1, typename T2>
	bool operator== (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first & lhs.second == rhs.second;
	}

	template <typename T1, typename T2>
	bool operator!= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T1, typename T2>
	bool operator<  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first < rhs.first ||
			(!(rhs.first < lhs.first) && lhs.second < rhs.second);
	}

	template <typename T1, typename T2>
	bool operator<= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(rhs < lhs);
	}

	template <typename T1, typename T2>
	bool operator>  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rhs < lhs;
	}

	template <typename T1, typename T2>
	bool operator>= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs < rhs);
	}

	template <typename T1, typename T2>
	void swap(pair<T1, T2>& x, pair<T1, T2>& y)
	{
		x.swap(y);
	}

	template <typename U, typename V>
	pair<U, V> make_pair(const U& u, const V& v)
	{
		return pair<U, V>(u, v);
	}
}