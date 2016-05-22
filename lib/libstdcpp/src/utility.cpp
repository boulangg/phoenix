#include <utility>

namespace std {

size_t nearest_power_2(size_t num){
	num--;
	num |= num >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
	num |= num >> 2;   // and then or the results.
	num |= num >> 4;
	num |= num >> 8;
	num |= num >> 16;
	num |= num >> 32; // Work even with 32 bits size_t
	num++;
	return num;
}

}
