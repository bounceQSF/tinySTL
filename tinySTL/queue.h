#pragma once
#include "deque.h"

namespace tinySTL {
	template<typename T, typename Sequence = deque<T>>
	class queue{
		friend bool operator==(const queue&, const queue&);
		friend bool operator<(const queue&, const queue&);


	public:
		typedef typename Sequence::value_type              value_type;
		typedef typename Sequence::size_type               size_type;
		typedef typename Sequence::reference               reference;
		typedef typename Sequence::const_reference         const_reference;
	
	protected:
		Sequence seq;

	public:
		queue():seq(){}
		explicit queue(const Sequence&x) :seq(x) {}
		bool empty() { return seq.empty(); }
		bool size() { return seq.size(); }
		reference front() { return seq.front(); }
		const_reference front()const { return seq.front(); }
		reference back() { return seq.back(); }
		const_reference back()const { return seq.back(); }
		void push(const value_type& x) { seq.push_back(x); }
		void pop(const value_type& x) { seq.pop_back(); }
	};

	template<typename T, typename Sequence = deque<T>>
	bool operator==(const queue<T, Sequence>&x, const queue<T, Sequence>&y)
	{
		return x == y;
	}

	template<typename T, typename Sequence = deque<T>>
	bool operator<(const queue<T, Sequence>&x, const queue<T, Sequence>&y)
	{
		return x < y;
	}

}