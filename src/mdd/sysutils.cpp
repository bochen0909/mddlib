/*
 * sysutils.cpp
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#include <unistd.h>
#include "strutils.h"
#include "sysutils.h"
namespace mdd {

std::string __get_env_string(std::string name, std::string default_value) {
	const char *val = std::getenv(name.c_str());
	if (val == nullptr) { // invalid to assign nullptr to std::string
		return default_value;
	} else {
		return val;
	}
}

template<> void set_env(std::string name, std::string value) {
	setenv(name.c_str(), value.c_str(), 1);
}

std::string get_env(std::string name) {
	return get_env<std::string>(name, "");
}

size_t get_env_as_size_bytes(std::string name) {
	std::string s = get_env(name);
	to_upper(trim(s));
	if (s.empty()) {
		return 0;
	}
	char c = s.at(s.length() - 1);
	if (is_digit(c)) {
		return cast<size_t>(s);
	} else {
		s = s.substr(0, s.length() - 1);
		size_t n = cast<size_t>(s);
		if (c == 'M') {
			return n * 1024 * 1024;
		} else if (c == 'K') {
			return n * 1024;
		} else if (c == 'G') {
			return n * 1024 * 1024 * 1024;
		} else {
			return 0;
		}
	}

}

bool has_env(std::string name) {
	const char *val = std::getenv(name.c_str());
	return val != nullptr;
}

std::string get_hostname() {
	int name_len;
	char hostname[1024];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	return hostname;
}


} //end mdd
