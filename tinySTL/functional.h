#pragma once
namespace tinySTL {
	template<typename T = void> //c++14?
	struct less {
		bool operator()(const T& lhs, const T& rhs)
		{
			return lhs < rhs;
		}
		typedef T first_argument_type;//why?
		typedef T second_argument_type;
		typedef bool result_type;
	};

	template<typename T = void> //c++14?
	struct  equal_to{
		bool operator()(const T& lhs, const T& rhs)
		{
			return lhs == rhs;
		}
		typedef T first_argument_type;//why?
		typedef T second_argument_type;
		typedef bool result_type;
	};
}