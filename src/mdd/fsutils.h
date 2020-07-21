/*
 * fsutils.h
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#ifndef MDDLIB_SRC_MDD_FSUTILS_H_
#define MDDLIB_SRC_MDD_FSUTILS_H_

#include <string>
#include <vector>

namespace mdd {

bool is_file(const std::string &path);
bool is_directory(const std::string &path);
bool path_exists(const std::string &path);
size_t remove_path(const std::string &path);
std::string absolute_path(const std::string &path);
std::string get_basename(const std::string &path);
std::string get_dirname(const std::string &path);
std::string get_working_dir();

std::string get_file_extension(const std::string &path);
std::string strip_file_extension(const std::string &path);
bool create_directory(const std::string &path);
std::vector<std::string> glob(const std::string &p, const char *pattern = 0);

}

#endif /* MDDLIB_SRC_MDD_FSUTILS_H_ */
