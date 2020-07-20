#define BOOST_TEST_MODULE example
#include <boost/test/unit_test.hpp>
#include "mdd/strutils.h"

using namespace mdd;
using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_trim) {
	std::string s = string(" ");
	trim(s);
	BOOST_TEST(""== s);
	s = string("\r");
	trim(s);
	BOOST_TEST(""== s);
	s = string("\n");
	trim(s);
	BOOST_TEST(""== s);
	s = string("\t");
	trim(s);
	BOOST_TEST(""== s);
	s = string("\r\n");
	trim(s);
	BOOST_TEST(""== s);
	s = string(" \t\n");
	trim(s);
	BOOST_TEST(""== s);
	s = string("\t \t      AAA \t  \t \r \n");
	BOOST_TEST("AAA"== trim(s));

	BOOST_TEST("AAA"== rtrim(ltrim(s)));

}

BOOST_AUTO_TEST_CASE(test_split) {
	std::vector<std::string> v;
	char *sep = " ";
	{
		v = split("", sep);
		BOOST_TEST(0== v.size());
		v = split(" ", sep);
		BOOST_TEST(0== v.size());
		v = split("  a b   c       d  e  ", sep);
		BOOST_TEST(5== v.size());
		string s = "  a b   c       d  e  ";
		v = split(s, sep);
		BOOST_TEST(5== v.size());
		vector<string> r;
		split(r, s, sep);
		BOOST_TEST(5== v.size());

	}
	sep = "\t";
	{
		v = split("", sep);
		BOOST_TEST(0== v.size());
		v = split("\t", sep);
		BOOST_TEST(0== v.size());
		v = split("   a\t b \t  c  \t     d \t e  ", sep);
		BOOST_TEST(5== v.size());
	}

	sep = ",";
	{
		v = split("", sep);
		BOOST_TEST(0== v.size());
		v = split(",", sep);
		BOOST_TEST(0== v.size());
		v = split("123,12 313,asdsaf,,", sep);
		BOOST_TEST(3== v.size());
	}

	//test split without sep
	{
		BOOST_TEST(0== split("").size());
		BOOST_TEST(0== split(" ").size());
		BOOST_TEST(0== split("\t").size());
		BOOST_TEST(0== split("\r\n").size());
		BOOST_TEST(0== split("  \r	\n \t \r   \t  \n").size());

		v = split("   a\t b \t  c  \t     d \t e  \r\n f \r \n");
		BOOST_TEST(6== v.size());
		BOOST_TEST("a"== v[0]);
		BOOST_TEST("b"== v[1]);
		BOOST_TEST("c"== v[2]);
		BOOST_TEST("d"== v[3]);
		BOOST_TEST("e"== v[4]);
		BOOST_TEST("f"== v[5]);
	}

}
BOOST_AUTO_TEST_CASE(test_strcases) {
	{
		BOOST_TEST("ABC" == to_upper("abc"));
		string a("abc");
		BOOST_TEST("ABC" == to_upper(a));

		BOOST_TEST("" == to_upper(""));
		BOOST_TEST(" A B C \t" == to_upper(" A b c \t"));
	}

	{
		BOOST_TEST("abc" == to_lower("ABC"));
		string a("AbC");
		BOOST_TEST("abc" == to_lower(a));

		BOOST_TEST("" == to_lower(""));
		BOOST_TEST(" a b c \t" == to_lower(" A b c \t"));
	}

}

BOOST_AUTO_TEST_CASE(test_start_or_end_with) {
	{
		BOOST_TEST(starts_with("ABCDEFG", "AB"));
		string a("abc");
		BOOST_TEST(starts_with(a, "a"));
		BOOST_TEST(starts_with(to_lower("ABCDEFG"), "ab"));

	}
	{
		BOOST_TEST(ends_with("ABCDEFG", "FG"));
		string a("abc");
		BOOST_TEST(ends_with(a, "c"));
		BOOST_TEST(ends_with(to_lower("ABCDEFG"), "fg"));

	}
	{
		BOOST_TEST(1==find_nth("a name and a title", " ", 0));
		BOOST_TEST(6==find_nth("a name and a title", " ", 1));
		BOOST_TEST(10==find_nth("a name and a title", " ", 2));
		BOOST_TEST(12==find_nth("a name and a title", " ", 3));
		BOOST_TEST(-1==find_nth("a name and a title", " ", 4));
		BOOST_TEST(-1==find_nth("ABCD", " "));
	}

}

BOOST_AUTO_TEST_CASE(test_check) {
	BOOST_TEST(is_digit('0'));
	BOOST_TEST(is_digit('1'));
	BOOST_TEST(is_space(' '));
	BOOST_TEST(is_space('\t'));
	BOOST_TEST(is_space('\r'));
	BOOST_TEST(is_space('\n'));
}

BOOST_AUTO_TEST_CASE(test_cast_to_string) {
	{
		BOOST_TEST("-32"==as_string(-32));
		BOOST_TEST("32"==as_string(32));
		BOOST_TEST("32"==as_string((short)32));
		BOOST_TEST("32"==as_string((unsigned short)32));
		BOOST_TEST("32"==as_string((long)32));
		BOOST_TEST("32"==as_string((unsigned long)32));
		BOOST_TEST("32"==as_string((long)32));
		BOOST_TEST("32"==as_string((int32_t)32));
		BOOST_TEST("32"==as_string((int16_t)32));
		BOOST_TEST("32"==as_string((int64_t)32));
		BOOST_TEST("32"==as_string((uint32_t)32));
		BOOST_TEST("32"==as_string((uint16_t)32));
		BOOST_TEST("32"==as_string((uint64_t)32));
		BOOST_TEST("32"==as_string((size_t)32));
	}
	{
		BOOST_TEST("1.23"==as_string(1.23f).substr(0,4));
		BOOST_TEST("1.23"==as_string(1.23).substr(0,4));
		BOOST_TEST("0.23"==as_string(0.23).substr(0,4));
		BOOST_TEST("-1.23"==as_string(-1.23).substr(0,5));
	}
}

BOOST_AUTO_TEST_CASE(test_cast_from_string,* utf::tolerance(0.00001)) {
	{
		BOOST_TEST(-32==cast<int> ("-32"));
		BOOST_TEST(-32==cast<int32_t> ("-32"));
		BOOST_TEST(4294967295==cast<uint32_t> ("-1"));
		BOOST_TEST(-1==cast<int16_t> ("-1"));
		BOOST_TEST(65535==cast<uint16_t> ("-1"));
		BOOST_TEST(-1==cast<int64_t> ("-1"));
		BOOST_TEST((uint64_t) -1 ==cast<uint64_t> ("-1"));
		BOOST_TEST(1234==cast<size_t> ("1234"));
	}
	{
		BOOST_TEST(1.23==cast<float>("1.23"));
		BOOST_TEST(-1.23==cast<double>("-1.23"));
	}
}

BOOST_AUTO_TEST_CASE(test_replace) {
	BOOST_TEST(
			"the cthet sits on the mthet" == replace("a cat sits on a mat","a","the"));
	BOOST_TEST(" ct sits on  mt" == replace("a cat sits on a mat","a",""));
}

