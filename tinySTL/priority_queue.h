#pragma once

#include "vector.h"
#include "functional.h"
#include "heap.h"

namespace tinySTL {
	template<typename T, typename Sequence = tinySTL::vector<T>, typename Compare = tinySTL::less<typename Sequence::value_type>>
	class priority_queue
	{
	public:
		typedef typename Sequence::value_type              value_type;
		typedef typename Sequence::size_type               size_type;
		typedef typename Sequence::reference               reference;
		typedef typename Sequence::const_reference         const_reference;

	protected:
		Sequence seq;
		Compare cmp;

	public:
		priority_queue():seq(){}
		explicit priority_queue(const Compare& c): seq(), cmp(x){}
		template<typename _InputIterator>
		priority_queue(_InputIterator first, _InputIterator last, const Compare &c)
			:seq(first, last),cmp(c)
		{ tinySTL::make_heap(seq.first, seq.last, cmp);}
		template<typename _InputIterator>
		priority_queue(_InputIterator first, _InputIterator last)
			:seq(first, last) 
		{ tinySTL::make_heap(seq.first, seq.last);}

		bool empty() { return seq.empty(); }
		size_type size() { return seq.size(); }
		const_reference top() { return seq.front(); }
		void push(const value_type& x) 
		{
			seq.push_back(x);
			push_heap(seq.begin(), seq.end(), comp);
		}
		
		void pop(const value_type& x)
		{
			pop_heap(seq.begin(), seq.end(), comp);
			seq.pop_back();
		}
	};
}
