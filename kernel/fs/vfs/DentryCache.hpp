#pragma once

#include <string>
#include <vector>

#include "Dentry.hpp"

class DentryCache {
public:
	static Dentry* findDentry(Dentry* parent, std::vector<std::string> pathname, size_t index);

};
