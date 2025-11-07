#pragma once
#include <optional>

namespace state {
	template <typename T>
	bool matches(const std::optional<T>& opt, const T& val) {
		return !opt.has_value() || opt.value() == val;
	}
}