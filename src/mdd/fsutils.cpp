/*
 * fsutils.cpp
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#include <fnmatch.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::filesystem;
#include "strutils.h"
#include "fsutils.h"

namespace mdd {

std::string get_working_dir() {
	return current_path().c_str();
}

std::string get_basename(const std::string &path) {
	return basename(path.c_str());
}

std::string get_dirname(const std::string &path) {
	std::string s(path);
	trim(s);
	std::string name = get_basename(s);
	if (name == s) {
		return absolute_path(get_working_dir());
	} else {
		return absolute_path(s.substr(0, path.length() - name.length()));
	}

}

std::string get_file_extension(const std::string &path) {
	std::string name = get_basename(path);
	auto itor = boost::find_last(name, ".");
	return name.substr(std::distance(name.begin(), itor.end()));
}

bool create_directory(const std::string &path) {
	return create_directories(path);

}
std::string strip_file_extension(const std::string &path) {
	std::string name = get_basename(path);
	std::string dirname = get_dirname(path);

	auto itor = boost::find_last(name, ".");
	if (itor.end() == name.end()) {
		return path;
	}
	std::string striped_name = name.substr(0,
			std::distance(name.begin(), itor.end()) - 1);
	return (dirname + "/" + striped_name);
}

std::string absolute_path(const std::string &path) {
	return canonical(absolute(path)).c_str();
}
bool is_file(const std::string &path) {
	return is_regular_file(path);
}

bool is_directory(const std::string &path) {
	return boost::filesystem::is_directory(path);
}
bool path_exists(const std::string &path) {
	return exists(path);
}
size_t remove_path(const std::string &path) {
	return remove_all(path);
}

void glob(const std::string &p, const char *pattern,
		std::vector<std::string> &ret) {
	try {
		auto namematch = [pattern](const std::string &path) {
			std::string name = get_basename(path);

			if (pattern) {
				if (fnmatch(pattern, name.c_str(), 0) == 0) {
					return true;
				} else {
					return false;
				}
			} else {
				return true;
			}

		};
		if (exists(p)) {
			if (is_file(p) && namematch(p)) {
				ret.push_back(p);
			} else if (is_directory(p)) {
				for (directory_entry &x : directory_iterator(p)) {
					glob(x.path().c_str(), pattern, ret);
				}
			}
		}
	} catch (const filesystem_error &ex) {
		std::cerr << ex.what() << '\n';
	}

}

std::vector<std::string> glob(const std::string &p, const char *pattern) {
	std::vector<std::string> ret;
	glob(p, pattern, ret);
	return ret;

}

} //end mdd
