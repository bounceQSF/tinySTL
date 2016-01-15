#ifndef _ITERATOR_H_
#define _ITERATOR_H_

namespace tinySTL {
	
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};	

	template<typename T, typename Difference>
	struct inputIterator {
		typedef input_iterator_tag  iterator_category;
		typedef T                   value_type;
		typedef Difference          difference_type;
		typedef T*                  pointer;
		typedef T&                  reference;
	};

	struct output_iterator {
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};

	template<typename T, typename Difference>
	struct forwardIterator {
		typedef forward_iterator_tag iterator_category;
		typedef T                    value_type;
		typedef Difference           difference_type;
		typedef T*                   pointer;
		typedef T&                   reference;
	};

	template<typename T, typename Difference>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag iterator_category;
		typedef T                          value_type;
		typedef Difference                 difference_type;
		typedef T*                         pointer;
		typedef T&                         reference;
	};

	template<typename T, typename Difference>
	struct random_access_iterator {
		typedef random_access_iterator_tag iterator_category;
		typedef T                          value_type;
		typedef Difference                 difference_type;
		typedef T*                         pointer;
		typedef T&                         reference;
	};

	template<typename Category, typename T, typename Difference = ptrdiff_t,
	typename Pointer = T*, typename Reference = T&>
	struct iterator {
		typedef Category                   iterator_category;
		typedef T                          value_type;
		typedef Difference                 difference_type;
		typedef Pointer                    pointer;
		typedef Reference                  reference;
	};

	template<typename iterator>
	struct iterator_traits {
		typedef iterator::iterator_category          iterator_category;
		typedef iterator::value_type                 value_type;
		typedef iterator::difference_type            difference_type;
		typedef iterator::pointer                    pointer;
		typedef iterator::reference                  reference;
	};

	template<typename T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag           iterator_category;
		typedef T                                    value_type;
		typedef ptrdiff_t                            difference_type;
		typedef T*                                   pointer;
		typedef T&                                   reference;
	};

	template<typename T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag           iterator_category;
		typedef T                                    value_type;
		typedef ptrdiff_t                            difference_type;
		typedef const T*                             pointer;
		typedef const T&                             reference;
	};

	template<typename iterator>
	inline typename iterator_traits<iterator>::iteraotr_category
		iterator_category(const iterator&) 
	{
		typedef typename iterator_traits<iterator>::iterator_category category;
		return category();
	}

	template<class iterator>
	inline typename iterator_traits<iterator>::difference_type*
		distance_type(const iterator&)
	{
		return static_cast<typename iterator_traits<iterator>::difference_type*>(0);
	}

	template<class iterator>
	inline typename iterator_traits<iterator>::value_type*
		value_type(const iterator&)
	{
		return static_cast<typename iterator_traits<iterator>::value_type*>(0);
	}
}
#endif