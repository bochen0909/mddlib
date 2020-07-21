/*
 * sysutils.h
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#ifndef MDDLIB_SRC_MDD_SYSUTILS_H_
#define MDDLIB_SRC_MDD_SYSUTILS_H_

#include <string>
#include "strutils.h"
namespace mdd {

std::string get_hostname();

extern std::string __get_env_string(std::string name,
		std::string default_value);

template<typename T> T get_env(std::string name, T default_value) {
	std::string s = __get_env_string(name, "");
	if (s.empty()) {
		return default_value;
	}
	return cast<T>(s);
}

template<typename T> void set_env(std::string name, T value) {
	std::string s = as_string<T>(value);
	set_env<std::string>(name, s);
}

std::string get_env(std::string name);

bool has_env(std::string name);

size_t get_env_as_size_bytes(std::string name);

} //end mdd

#endif /* MDDLIB_SRC_MDD_SYSUTILS_H_ */
