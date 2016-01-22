#pragma once
//make_heap
//up
//different
#include "function.h"
#include "iterator.h"

//first version without custom cmp
namespace tinySTL {
	template<typename _RandonmIterator>
	void push_heap(_RandonmIterator first, _RandonmIterator last)
	{
		//the element occupied last - 1
		__push_heap_aux(first, last, value_type(first));
	}

	template<typename _RandonmIterator, typename distance, typename T>
	void __push_heap_aux(_RandonmIterator first, _RandonmIterator last, T*)
	{
		__push_heap(first, distance((last - first) - 1), distance(0), T(*first));

	}

	template<typename _RandonmIterator, typename distance, typename T>
	void __push_heap(_RandonmIterator first, distance holeIndex, distance topIndex, T value)
	{
		auto parentIndex = (holeIndex - 1) / 2;
		auto value = *(last - 1);
		while (parentIndex > topIndex && *(first + parentIndex) < value) {
			*(first + holeIndex) = *(first + parentIndex);
			holeIndex = parentIndex;
			parentIndex = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}


	template<typename _RandonmIterator>
	void pop_heap(_RandonmIterator first, _RandonmIterator last)
	{
		__pop_heap_aux(first, last, value_type(first));
	}

	template<typename _RandonmIterator, typename distance, typename T>
	void __pop_heap_aux(_RandonmIterator first, _RandonmIterator last, T*)
	{
		*(last - 1) = *first;
		adjust_heap(first, distance(0), distance(last - first), T(*(last - 1)));
	}

	template<typename _RandonmIterator, typename distance, typename T>
	void adjust_heap(_RandonmIterator first, distance holeIndex,
		distance len, T value)
	{
		auto topIndex = holeIndex;
		auto firstChildIndex = 2 * holeIndex + 1;
		while (firstChildIndex < len) {
			if (*(first + firstChildIndex) < *(first + firstChildIndex + 1))
				++firstChildIndex;
			*(first + holeIndex) = *(first + firstChildIndex);
			holeIndex = firstChildIndex;
			firstChildIndex = 2 * holeIndex + 1;
		}
		push_heap(first, holeIndex, topIndex, value);
	}

	template<typename _RandonmIterator>
	void sort_heap(_RandonmIterator first, _RandonmIterator last)
	{
		while (last - first > 1) {
			pop_heap(first, last--);
		}
	}

	template<typename _RandonmIterator, typename BinaryPredicate>
	void make_heap(_RandonmIterator first, _RandonmIterator last, BinaryPredicate pred)
	{
		//__make_heap(first, last, value_type(first), distance_type(first));
	}

	template<typename _RandonmIterator>
	void make_heap(_RandonmIterator first, _RandonmIterator last)
	{
		__make_heap(first, last, value_type(first), distance_type(first));
	}

	template<typename _RandonmIterator, typename T, typename distance>
	void make_heap(_RandonmIterator first, _RandonmIterator last, T*, distance*)
	{
		auto len = last - first;
		if (len < 2) return;
		auto parent = len / 2;
		while (true) {
			adjust_heap(first, parent, len, T(*first));
			if (parent--) return;
		}
	}
}

//	template<typename random_access_iterator, typename Compare>
//	static void up(random_access_iterator first, random_access_iterator last,
//		random_access_iterator head, Compare cmp)
//	{
//		if (first != last - 1) {
//			auto index = last - 1 - head;
//			auto parentIndex = (index - 1) / 2;
//			for (auto cur = last - 1; parentIndex >= 0, cur != head; parentIndex = (index - 1) / 2) {
//				auto parent = head + parentIndex;
//				if (cmp(*parent, *child))
//					swap(*parent, *child);
//				cur = parent;
//				index = cur - head;
//			}
//		}
//	}
//	//down
//	template<typename random_access_iterator, typename Compare>
//	static void down(random_access_iterator first, random_access_iterator last,
//		random_access_iterator head, Compare cmp)
//	{
//		if (first != last - 1) {
//			auto index = first - head;
//			auto lChildIndex = 2 * index + 1;
//			for (auto cur = first; lChildIndex < last - head && cur < last; lChildIndex = 2 * index + 1) {
//				auto child = head + lChildIndex;
//				if (cmp(*parent, *child))
//					swap(*parent, *child);
//				cur = parent;
//				index = cur - head;
//			}
//		}
//	}
//
//	template<typename random_access_iterator>
//	void make_heap(RandomAccessIterator first, RandomAccessIterator last)
//	{
//		make_heap(first, last,
//			tinySTL::less<typename tinySTL::iterator_traits<RandomAccessIterator>::value_type>());
//	}
//
//	template<typename random_access_iterator, class Compare>
//	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp)
//	{
//		auto range = last - first;
//		for (auto cur = first + range / 2 - 1; cur >= first; --cur) {
//			tinySTL::down(cur, last, first, cmp);
//		}
//
//	}
//	//push_heap
//
//	template<typename random_access_iterator>
//	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp)
//	{
//		tiny::push_heap(first, last,
//			tinySTL::less<typename tinySTL::iterator_traits<RandomAccessIterator>::value_type>());
//	}
//
//	template<typename random_access_iterator, class Compare>
//	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp)
//	{
//		tinySTL::up(first, last, first, cmp);
//	}
//	//pop_heap
//	template<typename random_access_iterator>
//	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp)
//	{
//		tiny::pop_heap(first, last,
//			tinySTL::less<typename tinySTL::iterator_traits<RandomAccessIterator>::value_type>());
//	}
//
//	template<typename random_access_iterator, class Compare>
//	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp)
//	{
//		tinySTL::swap(*first, *(last - 1));
//		if (last - first >= 2)
//			TinySTL::down(first, last - 1, first, comp);
//	}
//}
