#pragma once
#include "algorithm.h"
#include "alloc.h"
#include "construct.h"
#include "functional.h"
#include "utility.h"

namespace tinySTL {
	typedef bool Rb_tree_Color_type;
	const Rb_tree_Color_type Rb_tree_red = false;
	const Rb_tree_Color_type Rb_tree_red = true;

	struct Rb_tree_Node_base {
		typedef Rb_tree_Color_type color_type;
		typedef Rb_tree_Node_base* base_ptr;
		color_type _color;
		base_ptr _left;
		base_ptr _right;
		base_ptr _parent;

		static base_ptr min_ptr(base_ptr p) {
			while (p->_left != nullptr) p = p->_left;
			return p;
		}

		static base_ptr max_ptr(base_ptr p) {
			while (p->_right != nullptr) p = p->_right;
			return p;
		}
	};

	template<typename T>
	struct Rb_tree_Node :public Rb_tree_Node_base {
		typedef Rb_tree_Node<T>*             link_type;
		T _value_field;
	};

	struct Rb_tree_base_iterator
	{
		typedef Rb_tree_Node_base::base_ptr  base_ptr;
		typedef bidirectional_iterator_tag   iterator_categpry;
		typedef ptrdiff_t                    difference_Type;
		base_ptr _node;

		void increment() {//to be finished
			if (_node->_right) {
				_node = _node->_right->_right;
				while (_node->_left) {
					_node = _node->_left;
				}
			}
			else {
				auto p = _node->_parent;
				while (_node == p->_right) {
					_node = p;
					p = p->_parent;
				}
				if (_node->_right != p)
					_node = p;
			}
		}

		void decrement()
		{

		}
	};

	template<typename T, typename Ref, typename Ptr>
	struct Rb_tree_iterator :public Rb_tree_base_iterator
	{
		typedef T                                            value_type;
		typedef Ptr                                          pointer;
		typedef Ref                                          reference;
		typedef Rb_tree_iterator<T, T&, T*>                  iterator;
		typedef Rb_tree_iterator<T, const T&, const T*>      const_iterator;
		typedef Rb_tree_iterator<T, Ref, Ptr>                self;
		typedef Rb_tree_Node<T>*                             link_type;
		
		Rb_tree_iterator() = default;
		Rb_tree_iterator(link_type x) { _node = x; }
		Rb_tree_iterator(const iterator& it) { _node = it._node; }

		reference operator*()const { return static_cast<link_type>(_node)->value_field; };
		pointer operator->()const { return &(operator*()); };

		self operator++() { increment(); return *this; }
		self operator++(int) {
			auto tmp = *this;
			increment(); 
			return tmp; 
		}
		self operator--() { decrement(); return *this; }
		self operator--(int) {
			auto tmp = *this;
			decrement();
			return tmp;
		}
	};

	template<typename Key, typename T, typename KeyOfT, typename Compare,
			 typename Alloc = alloc>
	class rb_tree {
	protected:
		typedef void*                                       void_pointer;
		typedef Rb_tree_Node_base*                          base_ptr;
		typedef Rb_tree_Node<T>                             rb_tree_node;
		typedef simple_alloc<rb_tree_node, alloc>           rb_tree_allocator;
		typedef Rb_tree_Color_type                          color_type;

	public:
		typedef Key                                         Keytype;
		typedef T                                           value_type;
		typedef value_type*                                 pointer;
		typedef const value_type*                           const_pointer;
		typedef value_type&                                 reference;
		typedef const value_type&                           const_reference;
		typedef rb_tree_node*                               link_type;
		typedef size_t                                      size_type;
	    typedef ptrdiff_t                                   difference_Type;

	protected:
		link_type get_node() { return rb_tree_allocator::allocate(); }
		link_type put_node() { return rb_tree_allocator::deallocate(); }
		link_type create_node(const value_type& x) 
		{
			auto tmp = get_node();
			try {
				construct(&tmp->value_field, x);
			}
			catch (...) {
				put_node(tmp);
				throw;
			}
		}

		link_type clone_node(link_type x) 
		{
			auto tmp = create_node(x->_value_field);
			tmp->_left = nullptr;
			tmp->_right = nullptr;
			tmp->_parent = nullptr; //?
			tmp->_color = x->_color;
		}

		link_type destroy_node(link_type x) 
		{
			destroy(&x->_value_field);
			put_node(x);
		}

	protected:
		size_type _size;
		link_type _header;
		Compare _key_cmp;

		//all for _header!
		link_type& root()const { return (link_type&)_header->_parent; }
		link_type& leftmost()const { return (link_type&) _header->_left; }
		link_type& rightmost()const { return (link_type&)_header->_right; }

		static link_type& left(link_type x) { return (link_type&)(x->_left); }
		static link_type& right(link_type x) { return (link_type&)(x->_right); }
		static link_type& parent(link_type x) { return (link_type&)(x->_parent); }
		static reference& value(link_type x) { return x->_value_field; }
		static Key& key(link_type x) { return KeyOfT()(value(x)); }
		static color_type& color(link_type x) { return (color_type&)x->_color; }

		static link_type& left(base_ptr x) { return (link_type&)(x->_left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->_right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->_parent); }
		static reference& value(base_ptr x) { return ((link_type))x->_value_field; }
		static const Key& key(base_ptr x) { return KeyOfT()(value(x)); }
		static color_type& color(base_ptr x) { return (color_type&)x->_color; }

		static link_type minimum(link_type x) { return (link_type)Rb_tree_Node_base::min_ptr(x); }
		static link_type maximum(link_type x) { return (link_type)Rb_tree_Node_base::max_ptr(x); }

	public:
		typedef Rb_tree_iterator<value_type, reference, pointer> iterator;

	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		link_type copy(link_type x, link_typep);
		void erase(link_type x);
		void init()
		{
			_header = get_node();
			color(_header) = Rb_tree_red;
			root() = nullptr;
			leftmost() = _header;
			rightmost() = _header;
		}
	public:
		rb_tree(const Compare& cmp= Compare())
			:_size(0), _key_cmp(cmp) {init();}
		~rb_tree() 
		{
			clear();
			put_node(_header);
		}
		//rb_tree<Key, T, KeyOfT, Compare, Alloc>& operator=(const rb_tree<Key, T, KeyOfT, Compare, Alloc>& x);
	public:
		Compare key_comp() const { return _key_cmp; }
		iterator begin() { return leftmost(); }
		iterator end() { return rightmost(); }
		bool empty() { return !_size; }
		bool size() { return _size; }

	public:
		void insert_equal(const value_type& v)
		{
			link_type y = _header;
			link_type x = root();
			while (x != nullptr) {
				y = x;
				x = key_comp(KeyOfT()(v), key(x)) ? left(x) : right(x);
			}
			return __insert(x, y, v);
		}

		void insert_unique(const value_type& v) 
		{
			link_type y = header;
			link_type x = root();
			bool comp = true;
			while (x != nullptr) {
				y = x;
				comp = key_comp(KeyOfT()(v), key(x)) ? left(x) : right(x);
				x = _key_cmp;
			}
			auto j = iterator(y);
			if (comp) {
				if (j == begin())
					return pair<iterator, bool>(__insert(x, y, v), true);
				else
					--j;
			}
			if (key_comp(key(j.node), KeyOfT()(v)))
				return pair<iterator, bool>(__insert(x, y, v), true);
			return pair<iterator, bool>(j, false);
		}

		iterator __insert(base_ptr x, base_ptr y, const value_type& v)
		{
			link_type z;
			if (y == _header || x || key_comp(KeyOfT()(v), key(y))) {
				z = create_node(v);
				left(y) = z;
				if (y == _header) {
					root() = z;
					rightmost = z;
				}
				else if (y == leftmost())
					leftmost() = z;
			}
			else {
				z = create_node(v);
				right(y) = z;
				if (y == rightmost())
					rightmost() = z;
			}
			parent(z) = y;
			left(z) = nullptr;
			right(z) = nullptr;
			rb_tree_rebalance(z, header->parent);
			++_size;
			return iterator(z);
		}

		void rb_tree_rebalance(Rb_tree_Node_base *x, Rb_tree_Node_base*& root)
		{
			x->_color = Rb_tree_red;
			while (x != root && x->_parent->_color == Rb_tree_red) {//父节点为红
				if (x == x->_parent->_left) {//父节点为祖父节点的左子节点
					auto u = x->_parent->_right;
					if (u && u->_color == Rb_tree_red) {//伯父节点为红色
						x->_parent->_color = Rb_tree_black;
						u->_color = Rb_tree_black;
						x->_parent->_parent = Rb_tree_red;
						x = x->_parent->_parent;
					}
					else {//伯父节点为黑色或不存在
						if (x == x->_parent->_right) {
							x = x->_parent;
							x = rb_tree_rotate_left(x, root);
						}
						x->_parent->_parent->_color = Rb_tree_red;
						x->_parent->_color = Rb_tree_red;
						rb_tree_rotate_right(x->_parent->_parent, root);
					}
				}
				else {//父节点为祖父节点的右子节点
					auto u = x->_parent->_right;
					if (u && u->_color == Rb_tree_red) {//伯父节点为红色
						x->_parent->_color = Rb_tree_black;
						u->_color = Rb_tree_black;
						x->_parent->_parent = Rb_tree_red;
						x = x->_parent->_parent;
					}
					else {//伯父节点为黑色或不存在
						if (x == x->_parent->_left) {
							x = x->_parent;
							x = rb_tree_rotate_right(x, root);
						}
						x->_parent->_parent->_color = Rb_tree_red;
						x->_parent->_color = Rb_tree_red;
						rb_tree_rotate_left(x->_parent->_parent, root);
					}
				}
			}
			root->_color = Rb_tree_black;
		}

		void rb_tree_rotate_left(Rb_tree_Node_base *x, Rb_tree_Node_base*& root)
		{
			auto tmp = x->_right;
			x->_right = tmp->_left;
			if (tmp->_left)
				tmp->_left->_parent = x;
			x->_parent = tmp->_parent;
			if (x == root)
				root = y;
			else if (x == x->_parent->left)
				x->_parent->left = tmp;
			else
				x->_parent->_right = tmp;
			tmp->_left = x;
			x->_parent = tmp;
		}

		void rb_tree_rotate_right(Rb_tree_Node_base *x, Rb_tree_Node_base*& root)
		{
			auto tmp = x->_left;
			x->_left = tmp->_right;
			if (tmp->_right)
				tmp->_right->_parent = x;
			tmp->_parent = x->_parent;
			if (x == root)
				root = tmp;
			else if (x == x->_parent->_right)
				x->_parent->_right = tmp;
			else
				x->_parent->_left = tmp;
			tmp->_right = x;
			tmp->_parent = x->_parent;
	};

	

	
}
