#pragma once
#include <cstdlib>
#include <new>

#include "algorithm.h"
//#include "construct.h"

namespace tinySTL {
	//simple_alloc
	template<typename T, typename Alloc>
	class simple_alloc {
		static T* allocate(size_t n) { return n ? static_cast<T*>(Alloc::allocate(n * sizeof(T))) : nullptr; }
		static T* allocate() { return static_cast<T*>(Alloc::allocate(sizeof(T))); }
		static void deallocate(T* ptr, size_t n) { if (n) Alloc::deallocate(ptr, n * sizeof(T)); }
		static void deallocate(T* ptr) { Alloc::deallocate(ptr, sizeof(T)); }
	};

	//malloc_alloc_template
	template<int inst>
	class malloc_alloc_template {
	private:
		static void* oom_malloc(size_t n) 
		{
			while (true) {
				if (!malloc_alloc_oom_handler) { assert(0); }
				malloc_alloc_oom_handler();
				auto ret = allocate(n);
				if (ret) return ret;
			}
		}

		static void* oom_realloc(void* p, size_t n)
		{
			while (true) {
				if (!malloc_alloc_oom_handler) { assert(0); }
				malloc_alloc_oom_handler();
				auto ret = reallocate(p, 0, n);//?
				if (ret) return ret;
			}
		}

		static void (*malloc_alloc_oom_handler)();

	public:
		static void* allocate(size_t n)
		{
			auto ret = malloc(n);
			if (!ret)
				ret = oom_malloc(n);
			return ret;

		}

		static void deallocate(void* ptr, size_t)
		{
			free(ptr);
		}

		static void* reallocate(void* ptr, size_t, size_t new_sz)
		{
			auto ret = realloc(ptr, new_sz);//?
			if (!ret)
				ret = oom_realloc(ptr, new_sz);
			return ret;
		}

	};
	template<int inst>
	void(*malloc_alloc_template<inst>::malloc_alloc_oom_handler)() = nullptr;

	typedef malloc_alloc_template<0> malloc_alloc;
	

	//default_alloc_template
	template<bool threads, int inst>
	class default_alloc_template
	{
	private:
		enum EAlign { ALIGN = 8 }; //小型区块
		enum EMaxBytes { MAXBYTES = 128 }; //区块个数上限
		enum ENFreeLists { NFREELISTS = MAXBYTES / ALIGN }; //free-list个数
		enum ENObjs { ENOBJS = 20 }; //每次增加节点数

	private:
		union obj {
			obj *next;
			char client[1];
		};

		static obj* volatile free_list[NFREELISTS];

	private:
		static char *start_free;
		static char *end_free;
		static size_t heap_size;

	private:
		static size_t ROUND_UP(size_t bytes)
		{
			return (bytes + ALIGN) & ~(ALIGN - 1);
		}

		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (bytes + ALIGN - 1) / ALIGN - 1;
		}
		
		static void* refill(size_t n)
		{
			size_t nobjs = 20;
			obj* volatile *my_free_list, *cur_obj, *next_obj;
			auto chunk = chunk_alloc(n, nobjs);
			if (nobjs == 1)
				return chunk;
			my_free_list = free_list + FREELIST_INDEX(n);
			*my_free_list = next_obj = static_cast<obj*>(chunk + n);
			for (auto i = 1;; ++i) {
				cur_obj = next_obj;
				next_obj = static_cast<obj*>(static_cast<char*>(cur_obj) + n);
				if (i == nobjs - 1) {
					cur_obj->next = nullptr;
					break;
				}
				else cur_obj->next = next_obj;
			}
			return static_cast<obj*>(chunk);
		}

		static char* chunk_alloc(size_t size, size_t nobjs) 
		{
			char *ret;
			auto total_bytes = size * nobjs;
			auto bytes_left = end_free - start_free;
			if (total_bytes <= bytes_left) {
				ret = start_free;
				start_free += total_bytes;
				return ret;
			}
			else if (bytes_left >= size) {
				nobjs = bytes_left / size;
				total_bytes = nobjs *size;
				ret = start_free;
				start_free += total_bytes;
				return ret;
			}
			else {
				auto bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);// I cannot fully understand why
				if (bytes_left > ALIGN) {
					auto my_free_list = free_list + FREELIST_INDEX(bytes_left);
					static_cast<obj*>(start_free)->next = *my_free_list;
					*my_free_list = static_cast<obj*>(start_free);
				}
				start_free = static_cast<char*>(malloc(bytes_to_get));
				if (!start_free) {
					obj *volatile* my_free_list, *p;
					for (auto i = size; i <= MAXBYTES; i += ALIGN) {
						my_free_list = free_list + FREELIST_INDEX(bytes_left);
						p = *my_free_list;
						if (!p) {
							*my_free_list = p->next;
							start_free = static_cast<char*>(p);
							end_free = start_free + i;
							return (chunk_alloc(size, nobjs));
						}
					}
					end_free = nullptr;
					start_free = static_cast<char*>(malloc_alloc::allocate(bytes_to_get));
					//why not apply for less memory? size instead
				}
				heap_size = bytes_to_get;
				end_free = start_free + bytes_to_get;
				return (chunk_alloc(size, nobjs));
			}
		}

	public:
		static void* allocate(size_t bytes)
		{
			obj* volatile *my_free_list;
			if (bytes > MAXBYTES)
				return malloc_alloc::allocate(bytes);
			
			my_free_list = free_list + FREELIST_INDEX(bytes);
			auto ret = *my_free_list;
			if (!ret) {
				auto r = refill(ROUND_UP(bytes));
				return r;
			}
			*my_free_list = ret->next;
			return (ret);
		}

		static void deallocate(void *ptr, size_t bytes)
		{
			if (bytes > MAXBYTES) {
				malloc_alloc::deallocate(ptr, bytes);
				return;
			}
			obj* volatile *my_free_list;
			my_free_list = free_list + FREELIST_INDEX(bytes);
			auto ret = static_cast<obj*>(ptr);
			ret->next = *my_free_list;
			*my_free_list = ret;
		}

		static void* reallocate(void* ptr, size_t old_sz, size_t new_sz)
		{
			if (old_sz > MAXBYTES && new_sz > MAXBYTES)
				return malloc_alloc::reallocate(ptr, old_sz, new_sz);
			if (ROUND_UP(old_sz) == ROUND_UP(old_sz)) return ptr;
			auto size = min(old_sz, new_sz);
			auto ret = allocate(new_sz);
			memcpy(ret, ptr, size);
			deallocate(ptr, old_sz);
			return (ret);
		}
	};

	template<bool threads, int inst>
	char *default_alloc_template<threads, inst>::start_free = nullptr;

	template<bool threads, int inst>
	char *default_alloc_template<threads, inst>::end_free = nullptr;

	template<bool threads, int inst>
	size_t default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	typename default_alloc_template<threads, inst>::obj* volatile free_list[default_alloc_template<threads, inst>::NFREELISTS] = { nullptr };

	typedef default_alloc_template<true, 0> alloc;
	typedef default_alloc_template<false, 0> single_client_alloc;

	//allocator
	template<typename T>
	inline T* allocate(ptrdiff_t size, T*) {
		//set_new_handler(nullptr);
		T* tmp = static_cast<T*>(::operator new(static_cast<size_t>(sizeof(T))));
		if (!tmp) {
			assert(0);
		}
		return tmp;
	}

	template<typename T>
	inline void deallocate(T* buffer) {
		::operator delete(buffer);
	}

	template<typename T>
	class allocator {
		typedef alloc                              _alloc;
	public:
		typedef T                                  value_type;
		typedef T*                                 pointer;
		typedef const T*                           const_pointer;
		typedef T&                                 reference;
		typedef const T&                           const_reference;
		typedef size_t                             size_type;
		typedef ptrdiff_t                          difference_type;

		template<typename T1>
		struct rebind
		{
			typedef allocator<T1> other;
		};

		allocator()noexcept{}
		allocator(const allocator& a)noexcept{}
		template<typename T1>
		allocator(const allocator<T1>& a)noexcept{}
		~allocator()noexcept {}

		static pointer allocate(size_type n = 1)
		{
			return n ? static_cast<pointer>(Alloc::)
			//if (!n) return nullptr;
			//return static_cast<T*>(alloc::allocate(sizeof(T) * n));
			return  tinySTL::allocate(static_cast<difference_type>(n), static_cast<pointer>(nullptr));
		}

		static void deallocate(pointer p) { underlying_alloc.deallocate; }
		static void destroy(pointer first, pointer last)
		{
			for (; first != last; ++first) {
				first->~T();
			}
		}

		static void destroy(pointer ptr)
		{
			ptr->~T();			
		}

		static void construct(pointer ptr, const value_type& value)
		{
			new(ptr)T(value);
		}

		static void construct(pointer ptr)
		{
			new(ptr)T();
		}

		static pointer address(reference x) { return static_cast<pointer>(&x); }
		static const_pointer address(const_reference x) { return static_cast<const_pointer>(&x); }
	};

	template <class T, class Allocator>
	struct alloc_traits
	{
		static const bool _S_instanceless = false;
		typedef typename Allocator::__STL_TEMPLATE rebind<T>::other
			allocator_type;
	};

	template <class T, class Allocator>
	const bool alloc_traits<T, Allocator>::_S_instanceless;

	// The version for the default allocator.

	template <class T, class T1>
	struct alloc_traits<T, allocator<T1> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, alloc> _Alloc_type;
		typedef allocator<T> allocator_type;
	};

	// Versions for the predefined SGI-style allocators.

	template <class T, int inst>
	struct alloc_traits<T, malloc_alloc_template<inst> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, malloc_alloc_template<inst> > _Alloc_type;
		typedef Allocator<T, malloc_alloc_template<inst> > allocator_type;
	};

	template <class T, bool __threads, int inst>
	struct alloc_traits<T, default_alloc_template<__threads, inst> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, default_alloc_template<__threads, inst> >
			_Alloc_type;
		typedef Allocator<T, default_alloc_template<__threads, inst> >
			allocator_type;
	};

	template <class T, class _Alloc>
	struct alloc_traits<T, debug_alloc<_Alloc> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, debug_alloc<_Alloc> > _Alloc_type;
		typedef Allocator<T, debug_alloc<_Alloc> > allocator_type;
	};

	// Versions for the Allocator adaptor used with the predefined
	// SGI-style allocators.

	template <class T, class T1, int inst>
	struct alloc_traits<T,
		Allocator<T1, malloc_alloc_template<inst> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, malloc_alloc_template<inst> > _Alloc_type;
		typedef Allocator<T, malloc_alloc_template<inst> > allocator_type;
	};

	template <class T, class T1, bool __thr, int inst>
	struct alloc_traits<T,
		Allocator<T1,
		default_alloc_template<__thr, inst> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, default_alloc_template<__thr, inst> >
			_Alloc_type;
		typedef Allocator<T, default_alloc_template<__thr, inst> >
			allocator_type;
	};

	template <class T, class T1, class _Alloc>
	struct alloc_traits<T, Allocator<T1, debug_alloc<_Alloc> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<T, debug_alloc<_Alloc> > _Alloc_type;
		typedef Allocator<T, debug_alloc<_Alloc> > allocator_type;
	};

}