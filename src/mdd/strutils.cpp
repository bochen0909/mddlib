/*
 * strutils.cpp
 *
 *  Created on: Jul 20, 2020
 *      Author: bo
 */

#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "strutils.h"

using namespace std;
namespace mdd {

uint32_t _internal_fnv_hash(const std::string &str) {
	uint32_t h = 2166136261;
	for (size_t i = 0; i < str.size(); i++) {
		h = h ^ uint32_t(int8_t(str[i]));
		h = h * 16777619;
	}
	return h;
}

std::string replace(const std::string &s, const char *search,
		const char *substr) {
	return boost::replace_all_copy<string>(s, search, substr);
}
std::string replace(const char *s, const char *search, const char *substr) {
	return boost::replace_all_copy<string>(s, search, substr);
}

bool is_space(char c) {
	return std::isspace(c);
}

bool is_digit(char c) {
	return std::isdigit(c);
}

bool starts_with(const string &s, const string start) {
	return boost::starts_with(s, start);
}

bool starts_with(const char *s, const char *start) {
	return boost::starts_with(s, start);
}

bool ends_with(const string &s, const string start) {
	return boost::ends_with(s, start);
}

bool ends_with(const char *s, const char *start) {
	return boost::ends_with(s, start);
}

int find_nth(const std::string &s, std::string sfind, int n) {
	auto r = boost::find_nth(s, sfind, n);
	if (r.end() == s.end()) {
		return -1;
	} else {
		return std::distance(s.begin(), r.begin());
	}
}

int find_nth(const char *s, const char *sfind, int n) {
	return find_nth(string(s), sfind, n);
}

string& to_upper(string &s) {
	boost::to_upper<std::string>(s);
	return s;
}

string to_upper(const char *s) {
	return boost::to_upper_copy<std::string>(s);
}

string& to_lower(string &s) {
	boost::to_lower<std::string>(s);
	return s;
}

string to_lower(const char *s) {
	return boost::to_lower_copy<std::string>(s);
}

// trim from start (in place)
inline std::string& ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
	return s;
}

// trim from end (in place)
inline std::string& rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
	return s;
}

// trim from both ends (in place)
std::string& trim(std::string &s) {
	ltrim(s);
	rtrim(s);
	return s;
}

// trim from start (copying)
std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s) {
	trim(s);
	return s;
}

std::vector<std::string> split(const std::string &source, const char *delimiter,
		bool keepEmpty) {
	std::vector<std::string> results;
	split(results, source, delimiter, keepEmpty);
	return results;
}

std::vector<std::string> split(const char *source, const char *delimiter,
		bool keepEmpty) {
	std::string s(source);
	return split(s, delimiter, keepEmpty);
}

void split(std::vector<std::string> &results, const std::string &source,
		const char *delimiter, bool keepEmpty) {

	size_t prev = 0;
	size_t next = 0;

	while ((next = source.find_first_of(delimiter, prev)) != std::string::npos) {
		if (keepEmpty || (next - prev != 0)) {
			results.push_back(source.substr(prev, next - prev));
		}
		prev = next + 1;
	}

	if (prev < source.size()) {
		results.push_back(source.substr(prev));
	}

}

void split(std::vector<std::string> &results, const std::string &source) {
	istringstream ss(source);
	while (ss) {
		string word;
		ss >> word;
		if (!word.empty())
			results.push_back(word);
	};
}

std::vector<std::string> split(const char *source) {
	std::vector<std::string> v;
	split(v, source);
	return v;
}

std::vector<std::string> split(const std::string &source) {
	std::vector<std::string> v;
	split(v, source);
	return v;
}

} //end namespace

