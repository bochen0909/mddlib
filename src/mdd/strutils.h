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
#include <boost/lexical_cast.hpp>

namespace mdd {

template<typename T> std::string as_string(T v) {
	return boost::lexical_cast < std::string > (v);
}

template<typename T> T cast(const std::string &v) {
	return boost::lexical_cast < T > (v);
}

std::string replace(const std::string &s, const char *search,
		const char *substr);

std::string replace(const char *s, const char *search, const char *substr);

bool is_space(char c);

bool is_digit(char c);

bool starts_with(const std::string &s, const std::string start);

bool ends_with(const char *s, const char *start);

bool ends_with(const std::string &s, const std::string start);

bool ends_with(const char *s, const char *start);

int find_nth(const std::string &s, std::string sfind, int n = 0);

int find_nth(const char *s, const char *sfind, int n = 0);

std::string& to_upper(std::string &s);

std::string to_upper(const char *s);

std::string& to_lower(std::string &s);

std::string to_lower(const char *s);

// trim from start (in place)
std::string& ltrim(std::string &s);

// trim from end (in place)
std::string& rtrim(std::string &s);

// trim from both ends (in place)
std::string& trim(std::string &s);

// trim from start (copying)
std::string ltrim_copy(std::string s);

// trim from end (copying)
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
std::string trim_copy(std::string s);

void split(std::vector<std::string> &results, const std::string &source,
		const char *delimiter, bool keepEmpty = false);

std::vector<std::string> split(const char *source, const char *delimiter,
		bool keepEmpty = false);

std::vector<std::string> split(const std::string &source, const char *delimiter,
		bool keepEmpty = false);

void split(std::vector<std::string> &results, const std::string &source);

std::vector<std::string> split(const char *source);

std::vector<std::string> split(const std::string &source);


uint32_t _internal_fnv_hash(const std::string &str);

template<typename T> uint32_t fnv_hash(T v){
	std::string s = as_string<T>(v);
	return _internal_fnv_hash(s);
}

}

#endif /* SUBPROJECTS__MDDLIB_SRC_MDD_STRUTILS_H_ */
