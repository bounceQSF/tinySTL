#pragma once
#include "algorithm.h"
#include "alloc.h"
#include "construct.h"
//#include "function.h"
#include "utility.h"


namespace tinySTL {
	typedef bool Rb_tree_Color_type;
	const Rb_tree_Color_type Rb_tree_red = false;
	const Rb_tree_Color_type Rb_tree_black = true;

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
				_node = _node->_right;
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
			if (_node->_color == Rb_tree_red && _node->_parent->_parent == _node) //_node = header
				_node = _node->_right;
			else if (_node->_left) {
				_node = _node->_left;
				while (_node->_right)
					_node = _node->_right;
			}
			else {
				auto tmp = _node->_parent;
				while (tmp->_left == _node) {
					_node = tmp;
					tmp = tmp->_parent;
				}
				_node = tmp;
			}
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
		typedef void*                                                   void_pointer;
		typedef Rb_tree_Node_base*                                  base_ptr;
		typedef Rb_tree_Node<T>                                      rb_tree_node;
		typedef simple_alloc<rb_tree_node, alloc>             rb_tree_allocator;
		typedef Rb_tree_Color_type                                     color_type;

	public:
		typedef Key                                                              Keytype;
		typedef T                                                          value_type;
		typedef value_type*                                               pointer;
		typedef const value_type*                                       const_pointer;
		typedef value_type&                                                reference;
		typedef const value_type&                                      const_reference;
		typedef rb_tree_node*                                             link_type;
		typedef size_t                                                           size_type;
	    typedef ptrdiff_t                                                       difference_Type;

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
			return tmp;
		}

		link_type clone_node(link_type x) 
		{
			auto tmp = create_node(x->_value_field);
			tmp->_left = nullptr;
			tmp->_right = nullptr;
			tmp->_parent = nullptr; 
			tmp->_color = x->_color;
			return tmp;
		}

		void destroy_node(link_type x) 
		{
			destroy(&x->_value_field);
			put_node(x);
		}

	protected:
		size_type _size;
		link_type _header;
		Compare _key_cmp;

		//all for _header!
		link_type& root()const { return static_cast<link_type&>(_header->_parent); }
		link_type& leftmost()const { return static_cast<link_type&>(_header->_left); }
		link_type& rightmost()const { return static_cast<link_type&>(_header->_right); }

		static link_type& left(link_type x) { return static_cast<link_type&>(x->_left); }
		static link_type& right(link_type x) { return static_cast<link_type&>(x->_right); }
		static link_type& parent(link_type x) { return static_cast<link_type&>(x->_parent); }
		static reference& value(link_type x) { return (static_cast<link_type&>(x))->_value_field; }
		static Key& key(link_type x) { return KeyOfT()(value(x)); }
		static color_type& color(link_type x) { return x->_color; }

		static link_type& left(base_ptr x) { return static_cast<link_type&>(x->_left); }
		static link_type& right(base_ptr x) { return static_cast<link_type&>(x->_right); }
		static link_type& parent(base_ptr x) { return static_cast<link_type&>(x->_parent); }
		static reference& value(base_ptr x) { return (static_cast<link_type&>(x))->_value_field; }
		static const Key& key(base_ptr x) { return KeyOfT()(value(x)); }
		static color_type& color(base_ptr x) { return static_cast<color_type&>(x->_color); }

		static link_type minimum(link_type x) { return static_cast<link_type>(Rb_tree_Node_base::min_ptr(x)); }
		static link_type maximum(link_type x) { return static_cast<link_type>(Rb_tree_Node_base::max_ptr(x)); }

	public:
		typedef Rb_tree_iterator<value_type, reference, pointer> iterator;

	public:
		//let us put alloc aside first
		rb_tree() : _size(0), _key_cmp(Compare())
		{ init(); }
		rb_tree(const Compare& cmp) : _size(0), _key_cmp(cmp)
		{ init();}
		rb_tree(const rb_tree<Key, T, KeyOfT, Compare, Alloc>& tree) :
			_size(tree._size), _key_cmp(tree._key_cmp)
		{
			if (!tree.root())
				init();
			else {
				color(_header) = Rb_tree_red;
				root() = copy(tree.root(), tree._header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
			}
		}

		rb_tree<Key, T, KeyOfT, Compare, Alloc> operator=(const rb_tree<Key, T, KeyOfT, Compare, Alloc>& tree)
		{
			if (this != &tree) {
				clear();
				_size = tree._size;
				_key_cmp = tree._key_cmp;
				if (!tree.root) {
					leftmost() = _header;
					rightmost() = _header;
					root() = nullptr;
				}
				else {
					root() = copy(tree.root(), tree._header);
					leftmost() = minimum(root());
					rightmost() = maximum(root());
				}
			}
			return *this;
		}

		void swap(const rb_tree<Key, T, KeyOfT, Compare, Alloc>& tree)
		{
			tinySTL::swap(_header, tree._header);
			tinySTL::swap(_size, tree._size);
			tinySTL::swap(_key_cmp, tree._key_cmp);
		}

		~rb_tree()
		{
			clear();
			put_node(_header);
		}

	private:
		link_type copy(link_type x, link_type p)
		{
			auto ret = clone_node(x);
			ret->parent = p;
			if (x->_right)
				ret->_right = copy(right(x), ret);
			p = ret;
			x = left(x);
			while (x)
			{
				auto y = clone_node(x);
				left(p) = y;
				y._parent = p;				
				if (x->_right)
					y->_right = copy(x->_right, x);
				p = y;
				x = left(x);
			}
			return ret;
		}

		size_type erase(const value_type& x)
		{
			auto p = equal_range(x);
			auto n = distance(p.first, p.second);
			erase(p.first, p.second);
			return n;
		}

		void erase(link_type x)
		{
			while (x)
			{
				erase(right(x));
				auto y = left(x);
				destroy_node(x);
				x = y;
			}
		}

		void erase(iterator first, iterator last)
		{
			if (first == begin() && last == end())
				clear();
			while(first != last)
			{
				erase(*first);
			}
		}

		void clear()//note that header is special
		{
			if(_size)
			{
				erase(root());
				leftmost() = _header;
				rightmost() = _header;
				root() = nullptr;
				_size = 0;
			}
			
		}

		void init()
		{
			_header = get_node();
			color(_header) = Rb_tree_red;
			root() = nullptr;
			leftmost() = _header;
			rightmost() = _header;
		}
		
	public:
		Compare key_comp() const { return _key_cmp; }
		iterator begin() { return leftmost(); }
		iterator end() { return _header; }
		bool empty()const { return !_size(); }
		bool size()const { return _size(); }

	public:
		void insert_equal(iterator first, iterator last)
		{
			while (first != last)
				insert_equal(*first);
		}

		void insert_unique(iterator first, iterator last)
		{
			while (first != last)
				insert_unique(*first);
		}

		iterator insert_equal(const value_type& v)
		{
			auto y = _header;
			auto x = root();
			while (x != nullptr) {
				y = x;
				x = _key_cmp(KeyOfT()(v), key(x)) ? left(x) : right(x);
			}
			return __insert(x, y, v);
		}

		pair<iterator, bool> insert_unique(const value_type& v)
		{
			auto y = _header;
			auto x = root();
			auto comp = true;
			while (x != nullptr) {
				y = x;
				comp = _key_cmp(KeyOfT()(v), key(x));
				x = comp ? left(x) : right(x);
			}
			auto j = iterator(y);
			if (comp) {
				if (j == begin())
					return pair<iterator, bool>(__insert(x, y, v), true);
				else
					--j;
			}
			if(_key_cmp(key(j._node), KeyOfT()(v)))
				return pair<iterator, bool>(__insert(x, y, v), true);
			return pair<iterator, bool>(j, false);
		}

		iterator insert_unique(iterator pos, const value_type& v)
		{
			if (pos._node == _header->_left) {
				if (size() &&//
					key_comp(KeyOfT()(v), key(pos._node)))
					return __insert(pos._node, pos._node, v);
					return insert_unique(v).first;
			}
			else if (pos._node == _header)
			{
	
				//if()
			}
			else
			{
				
			}
		}

		iterator __insert(base_ptr _x, base_ptr _y, const value_type& v)
		{
			auto x = static_cast<link_type>(_x);
			auto y = static_cast<link_type>(_y);
			auto z = create_node(v);
			if (y == _header || x || key_comp(KeyOfT()(v), key(y))) {
				left(y) = z;
				if (y == _header) {
					root() = z;
					rightmost = z;
				}
				else if (y == leftmost())
					leftmost() = z;
			}
			else {
				right(y) = z;
				if (y == rightmost())
					rightmost() = z;
			}
			parent(z) = y;
			left(z) = nullptr;
			right(z) = nullptr;
			rb_tree_rebalance(z, _header->parent);
			++_size;
			return iterator(z);
			
		}

		static void rb_tree_rebalance(base_ptr x, base_ptr& root)
		{
			x->_color = Rb_tree_red;
			while (x != root && x->_parent->_color == Rb_tree_red) {//父节点为红
				if (x == x->_parent->_left) {//父节点为祖父节点的左子节点
					auto u = x->_parent->_right;
					if (u && u->_color == Rb_tree_red) {//伯父节点为红色
						x->_parent->_color = Rb_tree_black;
						u->_color = Rb_tree_black;
						x->_parent->_parent->_color = Rb_tree_red;
						x = x->_parent->_parent;
					}
					else {//伯父节点为黑色或不存在
						if (x == x->_parent->_right) {
							x = x->_parent;
							rb_tree_rotate_left(x, root);
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
						x->_parent->_parent->_color = Rb_tree_red;
						x = x->_parent->_parent;
					}
					else {//伯父节点为黑色或不存在
						if (x == x->_parent->_left) {
							x = x->_parent;
							rb_tree_rotate_right(x, root);
						}
						x->_parent->_parent->_color = Rb_tree_red;
						x->_parent->_color = Rb_tree_red;
						rb_tree_rotate_left(x->_parent->_parent, root);
					}
				}
			}
			root->_color = Rb_tree_black;
		}

		static void rb_tree_rotate_left(base_ptr x, base_ptr& root)
		{
			auto tmp = x->_right;
			x->_right = tmp->_left;
			if (tmp->_left)
				tmp->_left->_parent = x;
			x->_parent = tmp->_parent;
			if (x == root)
				root = tmp;
			else if (x == x->_parent->_left)
				x->_parent->_left = tmp;
			else
				x->_parent->_right = tmp;
			tmp->_left = x;
			x->_parent = tmp;
		}

		static void rb_tree_rotate_right(base_ptr x, base_ptr& root)
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
		}

		iterator find(const Key& k)
		{
			auto y = _header; //last node not less than k
			auto x = root();
			while (x) {
				if (!key_comp(key(x), k))
					x = right(x);
				else
					y = x, x = left(x);
			}
			auto j = iterator(y);
			return (j == end() || key_comp(k, key(j._node))) ? end() : j;
		}

		size_type count(const value_type& x)
		{
			
		}

		iterator lower_bound(const value_type& x)
		{
			
		}

		iterator upper_bound(const value_type& x)
		{
			
		}

		iterator equal_range(const value_type& x)
		{

		}
	};
}
