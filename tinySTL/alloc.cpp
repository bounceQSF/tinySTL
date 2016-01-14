#include "alloc.h"

namespace tinySTL {
	char* alloc::start_free = nullptr;
	char* alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = { nullptr };

	void* alloc(size_t bytes)
	{
		//if(bytes > E)
	}

}