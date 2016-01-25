#pragma once
template <class _Key> struct hash { };

inline size_t stl_hash_string(const char* s)
{
	unsigned long h = 0;
	for (; *s; ++s)
		h = 5 * h + *s;

	return size_t(h);
}

struct hash<char*>
{
	size_t operator()(const char* s) const { return stl_hash_string(s); }
};

struct hash<const char*>
{
	size_t operator()(const char* s) const { return stl_hash_string(s); }
};

struct hash<char> {
	size_t operator()(char x) const { return x; }
};

struct hash<unsigned char> {
	size_t operator()(unsigned char x) const { return x; }
};

struct hash<signed char> {
	size_t operator()(unsigned char x) const { return x; }
};

struct hash<short> {
	size_t operator()(short x) const { return x; }
};

struct hash<unsigned short> {
	size_t operator()(unsigned short x) const { return x; }
};

struct hash<int> {
	size_t operator()(int x) const { return x; }
};

struct hash<unsigned int> {
	size_t operator()(unsigned int x) const { return x; }
};

struct hash<long> {
	size_t operator()(long x) const { return x; }
};

struct hash<unsigned long> {
	size_t operator()(unsigned long x) const { return x; }
};
