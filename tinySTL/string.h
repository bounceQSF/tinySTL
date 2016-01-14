#pragma once

#include "allocator.h"
#include "algorithm.h"
#include "iterator.h"
#include "reverseIterator.h"
#include "utility.h"
#include "typetraits.h"
#include "uninitializeFunctions.h"
#include "construct.h"

#include <cstring>

namespace tinySTL {
	class string {
	public:
		typedef char                                  value_type;
		typedef char*                                 iterator;
		typedef const char*                           const_iterator;
		//typedef reverse_iterator_t<char*>             iterator;
		//typedef reverse_iterator_t<const char*>       const_iterator;
		typedef iterator                           pointer;
		typedef char&                                 reference;
		typedef const char&                           const_reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;
		static const size_t npos = -1;

	private:
		char* _first;
		char* _last;
		char* _free;
		using dataAllocator = allocator<char>;

	public:
		string() :_first(nullptr), _last(nullptr), _free(nullptr) {}
		string(const string& str);
		string(string&& str);
		string(const string& str, size_t pos, size_t len = npos);
		string(const char* s);
		string(const char* s, size_t n);
		string(size_t n, char c);
		template <class InputIterator>
		string(InputIterator first, InputIterator last);

		string& operator= (const string& str);
		string& operator= (string&& str);
		string& operator= (const char* s);
		string& operator= (char c);

		~string();

		iterator begin() { return _first; }
		const_iterator begin() const { return _first; }
		iterator end() { return _last; }
		const_iterator end() const { return _last; }
		//reverse_iterator rbegin() { return reverse_iterator(_last); }
		//const_reverse_iterator rbegin() const { return const_reverse_iterator(_last); }
		//reverse_iterator rend() { return reverse_iterator(_first); }
		//const_reverse_iterator rend() const { return const_reverse_iterator(_first); }
		const_iterator cbegin() const { return _first; }
		const_iterator cend() const { return _last; }
		//const_reverse_iterator crbegin() const { return const_reverse_iterator(_last); }
		//const_reverse_iterator crend() const { return const_reverse_iterator(_first); }
		size_t size() const { return _last - _first; }
		size_t length() const { return size(); }
		size_t capacity() const { return _free - _first; }
		void clear();

		bool empty() const { return begin() == end(); }
		void resize(size_t n);
		void resize(size_t n, char c);
		void reserve(size_t n = 0);
		void shrink_to_fit() {}

		char& operator[] (size_t pos) { return *(_first + pos); }
		const char& operator[] (size_t pos) const { return *(_first + pos); }
		char& back() { return *(_last - 1); }
		const char& back() const { return *(_last - 1); }
		char& front() { return *(_first); }
		const char& front() const { return *(_first); }

		void push_back(char c) { insert(end(), c); }
		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const string& str, size_t subpos, size_t sublen = npos);
		string& insert(size_t pos, const char* s);
		string& insert(size_t pos, const char* s, size_t n);
		string& insert(size_t pos, size_t n, char c);
		iterator insert(iterator p, size_t n, char c);
		iterator insert(iterator p, char c);
		template <class InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
		string& append(const string& str);
		string& append(const string& str, size_t subpos, size_t sublen = npos);
		string& append(const char* s);
		string& append(const char* s, size_t n);
		string& append(size_t n, char c);
		template <class InputIterator>
		string& append(InputIterator first, InputIterator last);
		string& operator+= (const string& str);
		string& operator+= (const char* s);
		string& operator+= (char c);

		void pop_back() { erase(end() - 1, end()); }
		string& erase(size_t pos = 0, size_t len = npos);
		iterator erase(iterator p);
		iterator erase(iterator first, iterator last);

		string& replace(size_t pos, size_t len, const string& str);
		string& replace(iterator i1, iterator i2, const string& str);
		string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen = npos);
		string& replace(size_t pos, size_t len, const char* s);
		string& replace(iterator i1, iterator i2, const char* s);
		string& replace(size_t pos, size_t len, const char* s, size_t n);
		string& replace(iterator i1, iterator i2, const char* s, size_t n);
		string& replace(size_t pos, size_t len, size_t n, char c);
		string& replace(iterator i1, iterator i2, size_t n, char c);
		template <class _InputIterator>
		string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);

		void swap(string& str);
		size_t copy(char* s, size_t len, size_t pos = 0) const;


		size_t find(const string& str, size_t pos = 0) const;
		size_t find(const char* s, size_t pos = 0) const;
		size_t find(const char* s, size_t pos, size_t n) const;
		size_t find(char c, size_t pos = 0) const;
		size_t rfind(const string& str, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos, size_t n) const;
		size_t rfind(char c, size_t pos = npos) const;
		size_t find_first_of(const string& str, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_of(char c, size_t pos = 0) const;
		size_t find_last_of(const string& str, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_of(char c, size_t pos = npos) const;
		size_t find_first_not_of(const string& str, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_not_of(char c, size_t pos = 0) const;
		size_t find_last_not_of(const string& str, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_not_of(char c, size_t pos = npos) const;

		string substr(size_t pos = 0, size_t len = npos) const {
			len = changeVarWhenEuqalNPOS(len, size(), pos);
			return string(begin() + pos, begin() + pos + len);
		}

		int compare(const string& str) const;
		int compare(size_t pos, size_t len, const string& str) const;
		int compare(size_t pos, size_t len, const string& str,
			size_t subpos, size_t sublen = npos) const;
		int compare(const char* s) const;
		int compare(size_t pos, size_t len, const char* s) const;
		int compare(size_t pos, size_t len, const char* s, size_t n) const;
	private:
		void moveData(string& str);
		//插入时空间不足的情况
		template<typename _InputIterator>
		iterator insert_aux_copy(iterator p, _InputIterator first, _InputIterator last);
		//插入时空间不足的情况
		iterator insert_aux_filln(iterator p, size_t n, value_type c);
		size_type getNewCapacity(size_type len)const;
		void allocateAndFillN(size_t n, char c);
		template<typename _InputIterator>
		void allocateAndCopy(_InputIterator first, _InputIterator last);
		void string_aux(size_t n, char c, _true_type);
		template<typename _InputIterator>
		void string_aux(_InputIterator first, _InputIterator last, _false_type);
		void destroyAndDeallocate();
		size_t rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, int cond)const;
		size_t find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const;
		int compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const;
		bool isContained(char ch, const_iterator first, const_iterator last)const;
		size_t changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const;
	public:
		friend std::ostream& operator <<(std::ostream& os, const string&str);
		friend std::istream& operator >> (std::istream& is, string& str);
		friend string operator+ (const string& lhs, const string& rhs);
		friend string operator+ (const string& lhs, const char* rhs);
		friend string operator+ (const char* lhs, const string& rhs);
		friend string operator+ (const string& lhs, char rhs);
		friend string operator+ (char lhs, const string& rhs);
		friend bool operator== (const string& lhs, const string& rhs);
		friend bool operator== (const char*   lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char*   rhs);
		friend bool operator!= (const string& lhs, const string& rhs);
		friend bool operator!= (const char*   lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char*   rhs);
		friend bool operator<  (const string& lhs, const string& rhs);
		friend bool operator<  (const char*   lhs, const string& rhs);
		friend bool operator<  (const string& lhs, const char*   rhs);
		friend bool operator<= (const string& lhs, const string& rhs);
		friend bool operator<= (const char*   lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char*   rhs);
		friend bool operator>  (const string& lhs, const string& rhs);
		friend bool operator>  (const char*   lhs, const string& rhs);
		friend bool operator>  (const string& lhs, const char*   rhs);
		friend bool operator>= (const string& lhs, const string& rhs);
		friend bool operator>= (const char*   lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char*   rhs);
		friend void swap(string& x, string& y);
		friend std::istream& getline(std::istream& is, string& str, char delim);
		friend std::istream& getline(std::istream& is, string& str);
	};// end of string

	//init
	string::string() :_first(nullptr), _last(nullptr), _free(nullptr) {}
	string::string(const string& str)
	{
		allocateAndCopy(str._first, str._last);
	}

	string::string(string&& str)
	{
		moveData(str);
	}

	string::string(const string& str, size_t pos, size_t len = npos)
	{
		len = (len == npos ? size() - pos : len);
		auto iter = str._first + pos;
		allocateAndCopy(iter, iter + npos);
	}

	string::string(const char* s)
	{
		allocateAndCopy(s, s + strlen(s));
	}

	string::string(const char* s, size_t n)
	{
		allocateAndCopy(s, s + n);
	}

	string::string(size_t n, char c)
	{
		allocateAndFillN(n, c);
	}

	template <class InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		allocateAndCopy(first, last);
	}

	string& string::operator= (const string& str)
	{
		if (this != &str) {
			destroyAndDeallocate();
			allocateAndCopy(str._first, str._last);
		}
		return *this;
	}

	string& string::operator= (string&& str)
	{
		if (this != &str) {
			destroyAndDeallocate();
			moveData(str);
		}
		return *this;
	}

	string& string::operator= (const char* s)
	{
		destroyAndDeallocate();
		allocateAndCopy(s, s + strlen(s));
	}

	string& string::operator= (char c)
	{
		destroyAndDeallocate();
		allocateAndFillN(1, c);
	}

	string::~string()
	{
		destroyAndDeallocate();
	}

	//modify
	void string::clear()
	{
		destroy(_first);
		_last = _first;
	}
	void string::resize(size_t n);
	void string::resize(size_t n, char c);
	void string::reserve(size_t n = 0);

	//base
	inline void string::moveData(string& str)
	{
		str._first = _first;
		str._last = _last;
		str._free = _free;
		str._first = str._free = str._last = nullptr;
	}

	template<typename _InputIterator>
	inline string::iterator string::insert_aux_copy(iterator p, _InputIterator first, _InputIterator last)
	{
		auto diffIns = last - first;
		auto len = getNewCapacity(diffIns);
		auto newFirst = dataAllocator::allocate(len);
		auto newLast = copy(_first, p, newFirst);
		newLast = copy(first, last, newLast);
		auto ret = newLast;
		newLast = copy(p, _last, newLast);
		destroyAndDeallocate();
		_first = newFirst;
		_last = newLast;
		_free = _first + len;
		return ret;
	}

	inline string::iterator string::insert_aux_filln(iterator p, size_t n, value_type c)
	{		
		auto len = getNewCapacity(n);
		auto newFirst = dataAllocator::allocate(len);
		auto newLast = tinySTL::copy(_first, p, newFirst);
		newLast = fill_n(newLast, n, c);
		auto ret = newLast;
		newLast = tinySTL::copy(p, _last, newLast);
		destroyAndDeallocate();
		_first = newFirst;
		_last = newLast;
		_free = _first + len;
		return ret;	
	}

	string::size_type string::getNewCapacity(size_type len)const
	{
		auto n = capacity();
		auto ret = max(len, n);
		return n ? 2 * n : (len ? len : 1);
	}

	void string::allocateAndFillN(size_t n, char c)
	{
		_first = dataAllocator::allocate(n);
		_last = _uninitialized_fill_n(_first, n, c);
		_free = _last;
	}

	template<typename _InputIterator>
	void string::allocateAndCopy(_InputIterator first, _InputIterator last)
	{
		_first = dataAllocator::allocate(last - first);
		_last = _uninitialized_copy(first, last, _first);
		_free = _last;
	}

	void string::string_aux(size_t n, char c, _true_type)
	{
		allocateAndFillN(n, c);
	}

	template<typename _InputIterator>
	inline void string::string_aux(_InputIterator first, _InputIterator last, _false_type)
	{
		allocateAndCopy(first, last);
	}

	void string::destroyAndDeallocate()
	{
		dataAllocator::destroy(_first, _last);
		dataAllocator::deallocate(_first);
	}

	size_t string::rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, int cond)const
	{

	}

	size_t string::find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const
	{
		size_t i, j;
		
	}

	int string::compare_aux(size_t pos, size_t len, const_iterator cit, size_t subpos, size_t sublen)const
	{

	}

	bool string::isContained(char ch, const_iterator first, const_iterator last)const
	{

	}

	size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const
	{

	}



}