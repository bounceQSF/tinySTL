#pragma once
namespace tinySTL {
	template<typename Arg, typename Ret>
	struct unary_function{
		typedef Arg              argument_type;
		typedef Ret              result_type;
	};

	template<typename Arg1, typename Arg2, typename Ret>
	struct binary_function {
		typedef Arg1              first_argument_type;
		typedef Arg2              second_argument_type;
		typedef Ret               result_type;
	};

	template<typename T = void> //c++14?
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& lhs, const T& rhs)
		{
			return lhs < rhs;
		}
	};

	template<typename T = void> //c++14?
	struct  equal_to : public binary_function<T, T, bool>{
		bool operator()(const T& lhs, const T& rhs)
		{
			return lhs == rhs;
		}
	};

	template<typename T>
	struct identity : public unary_function<T, T> {
		T operator()(const T& x)const
		{
			return x;
		}
	};
}