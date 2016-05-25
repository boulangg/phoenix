/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#include <fs/KernelFS.hpp>

File* KernelFS::getUserApp(std::string name) {
	for (uint64_t i = 0; i < user_apps_symbol_table.nb_user_apps; ++i) {
		struct apps_desc app = user_apps_symbol_table.apps[i];
		if (name.compare(*(app.apps_name)) == 0) {
			return new File(app.apps_start, app.apps_end-app.apps_start);
		}
	}
	return nullptr;
}
