#pragma once
#include "deque.h"

namespace tinySTL{
	template<typename T, typename Sequence = deque<T>>
	class stack
	{
		friend bool operator==(const stack&, const stack&);
		friend bool operator<(const stack&, const stack&);
	public:
		typedef typename Sequence::value_type              value_type;
		typedef typename Sequence::size_type               size_type;
		typedef typename Sequence::reference               reference;
		typedef typename Sequence::const_reference         const_reference;

	protected:
		Sequence seq;

	public:
		stack():seq(){}
		template<typename _InputIterator>
		stack(_InputIterator first, _InputIterator last):seq(first, last){}
		
		bool empty() { return seq.empty(); }
		size_type size() { return seq.size(); }
		reference top()
		{
			return c.back();
		}

		const_reference top()const 
		{
			return c.back();
		}

		void pop()
		{
			c.pop_back();
		}
		void push(const value_type& x)
		{
			c.push_back(x);
		}
	};

	template<typename T, typename Sequence = deque<T>>
	bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
	{
		return x.c == y.c;
	}

	template<typename T, typename Sequence = deque<T>>
	bool operator<(const stack<T, Sequence> &x, const stack<T, Sequence> &y)
	{
		return x.c < y.c;
	}
}

 