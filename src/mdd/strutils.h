/*
 * strutils.h
 *
 *  Created on: Jul 20, 2020
 *      Author: bo
 */

#ifndef SUBPROJECTS__MDDLIB_SRC_MDD_STRUTILS_H_
#define SUBPROJECTS__MDDLIB_SRC_MDD_STRUTILS_H_

#include<string>
#include<vector>

namespace mdd {

// trim from start (in place)
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

// trim from start (copying)
std::string ltrim_copy(std::string s);

// trim from end (copying)
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
std::string trim_copy(std::string s);

void shuffle(std::vector<int> &v);

void shuffle(std::vector<std::string> &v);

void split(std::vector<std::string> &results, const std::string &source,
		const char *delimiter = " ", bool keepEmpty = false);

std::vector<std::string> split(const std::string &source, const char *delimiter,
		bool keepEmpty);

}

#endif /* SUBPROJECTS__MDDLIB_SRC_MDD_STRUTILS_H_ */
