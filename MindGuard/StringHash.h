#pragma once
#include <string>
#include <string_view>
struct StringHash {
	using is_transparent = void;
	inline std::size_t operator()(std::string_view sv) const {
		std::hash<std::string_view> hasher;
		return hasher(sv);
	}
};