#include <limits>

namespace std {

template<>
class numeric_limits<bool>
{
	static constexpr bool min() noexcept
	{
		return false;
	}

	static constexpr bool max() noexcept
	{
		return true;
	}
};

}
