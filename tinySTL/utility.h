#pragma once
#include <utility>
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
		T2 first;

	public:
		pair() {}
		template<typename U, typename V>
		pair(const pair<U, V>& p);
		pair(pair<U, V>&& p);
		pair(const pair& p) = default;

		pair(pair&& p) = default;
		template<class U, class V> 
		pair(U&& a, V&& b);

		pair(const first_type& a, const second_type& b);
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


		template<typename T1, typename T2>
		template<typename U, typename V>
		pair(const pair<U, V>& p):
			first(p.first), second(p.second){}
		
		template<typename T1, typename T2>
		pair(const first_type& a, const second_type& b):
			first(a), second(b) {}

		//i think it's proper to use forward but
		template<class U, class V>
		pair(U&& a, V&& b):
			first(std::forward<U>(a)), first(std::forward<V>(b)){}

		template<typename T1, typename T2>
		pair& operator= (const pair& p)
		{
			if (this != &p) {
				first = p.first;
				second = p.second;
			}
			return *this;
		}

		template<typename T1, typename T2>
		pair& operator= (const pair& p)
		{
			if (this != &p) {
				first = p.first;
				second = p.second;
			}
			return *this;
		}

		void swap(pair& p) noexcept(
			noexcept(swap(first, p.first))&&
			noexcept(swap(second, p.second)))
		{
			swap(first, p.first);
			swap(second, p.second);
		}

		template <typename T1, typename T2>
		friend bool operator== (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return lhs.first == rhs.first & lhs.second == rhs.second;
		}

		template <typename T1, typename T2>
		friend bool operator!= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return !(lhs == rhs);
		}

		template <typename T1, typename T2>
		friend bool operator<  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return lhs.first < rhs.first || 
				(!(rhs.first < lhs.first) && lhs.second < rhs.second);
		}

		template <typename T1, typename T2>
		friend bool operator<= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return !(rhs < lhs);
		}

		template <typename T1, typename T2>
		friend bool operator>  (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return rhs < lhs;
		}

		template <typename T1, typename T2>
		friend bool operator>= (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
		{
			return !(lhs < rhs);
		}

		template <typename T1, typename T2>
		friend void swap(pair<T1, T2>& x, pair<T1, T2>& y)
		{
			x.swap(y);
		}

		template <typename U, typename V>
		pair<U, V> make_pair(const U& u, const V& v)
		{
			return pair<U, V>(u, v);
		}
	};
}