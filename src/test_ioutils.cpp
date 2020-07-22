#define BOOST_TEST_MODULE example

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <boost/test/unit_test.hpp>
#include <boost/bind/bind.hpp>
#include "mdd/strutils.h"
#include "mdd/fsutils.h"
#include "mdd/ioutils.h"

template<class T>
std::string type_name() {
	typedef typename std::remove_reference<T>::type TR;
	std::unique_ptr<char, void (*)(void*)> own(
#ifndef _MSC_VER
			abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
                nullptr,
#endif
			std::free);
	std::string r = own != nullptr ? own.get() : typeid(TR).name();
	if (std::is_const<TR>::value)
		r += " const";
	if (std::is_volatile<TR>::value)
		r += " volatile";
	if (std::is_lvalue_reference<T>::value)
		r += "&";
	else if (std::is_rvalue_reference<T>::value)
		r += "&&";
	return r;
}

using namespace mdd;
using namespace std;
namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

template<typename F> void general_test_function_text(F fun) {
	{ //text format
		fun("", TEXT_FORMAT, NONE);
		fun(".txt", TEXT_FORMAT, NONE);
		fun(".gz", TEXT_FORMAT, GZIP);
		fun(".lz4", TEXT_FORMAT, LZ4);
		fun(".snappy", TEXT_FORMAT, SNAPPY);

	}

}

template<typename F> void general_test_function_binary(F fun) {

	{ //binary format
		fun(".bin", BINARY_FORMAT, NONE);
		fun("text.bin", BINARY_FORMAT, NONE);
		fun(".bin.gz", BINARY_FORMAT, GZIP);
		fun(".bin.lz4", BINARY_FORMAT, LZ4);
		fun(".bin.snappy", BINARY_FORMAT, SNAPPY);

	}

}

template<typename F> void general_test_function(F fun) {
	general_test_function_text<F>(fun);
	general_test_function_binary<F>(fun);
}

void test_initialization(std::string extension, STREAM_FORMAT expected_format,
		STREAM_COMPRESSION expected_compression) {
	std::string file_path = "/tmp/test_outputstream_initialization" + extension;

	cerr << "Testing " << __func__ << setw(20) << extension << setw(10)
			<< expected_format << "\t" << expected_compression << endl;
	{
		outputstream os(file_path);
		BOOST_TEST(expected_format == os.format());
		BOOST_TEST(expected_compression == os.compression());
		os.close();
	}
	{
		inputstream os(file_path);
		BOOST_TEST(expected_format == os.format());
		BOOST_TEST(expected_compression == os.compression());
		os.close();
	}
}

void test_simple(std::string extension, STREAM_FORMAT expected_format,
		STREAM_COMPRESSION expected_compression) {
	std::string file_path = "/tmp/test_simple" + extension;

	cerr << "Testing " << __func__ << setw(20) << extension << setw(10)
			<< expected_format << "\t" << expected_compression << endl;
	{
		outputstream os(file_path);
		BOOST_TEST(expected_format == os.format());
		BOOST_TEST(expected_compression == os.compression());
		if (expected_format == TEXT_FORMAT) {
			os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " "
					<< "a_str" << " " << 'c' << "\n";
		} else {
			os << 1 << (size_t) 233 << (uint32_t) -1 << "a_str" << 'c';
		}
		os.close();
	}
	{
		inputstream is(file_path);
		BOOST_TEST(expected_format == is.format());
		BOOST_TEST(expected_compression == is.compression());

		uint32_t i1;
		size_t i2;
		uint32_t i3;
		std::string str1;
		char c;
		is >> i1 >> i2 >> i3 >> str1 >> c;

		BOOST_TEST(i1==1);
		BOOST_TEST(i2==233);
		BOOST_TEST(i3==(uint32_t) -1);
		BOOST_TEST(str1=="a_str");
		BOOST_TEST('c'==c);
		is.close();

	}
}

void test_get_lines(std::string extension, STREAM_FORMAT expected_format,
		STREAM_COMPRESSION expected_compression) {
	if (expected_format == BINARY_FORMAT) {
		return;
	}
	std::string filepath = "/tmp/test_get_lines" + extension;

	cerr << "Testing " << __func__ << setw(20) << extension << setw(10)
			<< expected_format << "\t" << expected_compression << endl;
	{
		outputstream os(filepath);
		BOOST_TEST(expected_format == os.format());
		BOOST_TEST(expected_compression == os.compression());
		os << "the first line\n";
		os << "the second line\n";
		os << "the thrid line\n";
		os.close();

		inputstream is(filepath);

		int i = 0;
		for (std::string line; std::getline(is.istream(), line); i++) {
			cerr << line << endl;

		}
		BOOST_TEST(i==3);
		is.close();

		inputstream is2(filepath);
		i = 0;
		is2.for_each_line([&i](const auto &line) {
			cerr << line << endl;
			i++;
		});
		BOOST_TEST(i==3);
		is2.close();

	}
}

template<typename T, typename F> void test_by_var_type(std::string extension,
		STREAM_FORMAT expected_format, STREAM_COMPRESSION expected_compression,
		F fun ) {
	std::string filepath = "/tmp/test_by_var_type" + extension;

	cerr << "Testing " << __func__ << setw(10) << type_name<T>() << setw(20)
			<< extension << setw(10) << expected_format << "\t"
			<< expected_compression << endl;
	{

		outputstream os(filepath);
		BOOST_TEST(expected_format== os.format());
		BOOST_TEST(expected_compression== os.compression());
		for (int i = 0; i < 10000; i++) {
			T x = fun(i);
			if (expected_format == TEXT_FORMAT) {
				os << x << "\n";
			} else {
				os << x;
			}
		}
		os.close();

		inputstream is(filepath);

		for (int i = 0; i < 10000; i++) {
			T k;
			is >> k;
			BOOST_TEST(((T ) fun(i)) == k );
		}
		is.close();

	}
}

BOOST_AUTO_TEST_CASE(test_1,* utf::tolerance(0.00001)) {

	general_test_function(test_initialization);
	general_test_function(test_simple);
	general_test_function(test_get_lines);

}

BOOST_AUTO_TEST_CASE(test_integer,* utf::tolerance(0.00001)) {

	{ //integer
		auto lambda = [](int i) {
			return i;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<int>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

	{ //unsigned int
		auto lambda = [](int i) {
			return (uint32_t) i;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<uint32_t>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

	{ //uint64
		auto lambda = [](int i) {
			return (uint64_t) i;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<uint64_t>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

	{ //int64
		auto lambda = [](int i) {
			return (int64_t) i;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<int64_t>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}
	{ //short
		auto lambda = [](int i) {
			return (short) i;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<short>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

}

BOOST_AUTO_TEST_CASE(test_float_text,* utf::tolerance(0.0001f)) {

	{ //float
		auto lambda = [](int i) {
			return (float) i / 17.0;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<float>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function_text(lambda2);
	}
}

BOOST_AUTO_TEST_CASE(test_float_binary,* utf::tolerance(0.00001f)) {

	{ //float
		auto lambda = [](int i) {
			return (float) i / 17.0;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<float>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function_binary(lambda2);
	}
}

BOOST_AUTO_TEST_CASE(test_double,* utf::tolerance(0.00001)) {

	{ //double
		auto lambda = [](int i) {
			return (double) i / 17.0;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<double>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

}

BOOST_AUTO_TEST_CASE(test_other_type,* utf::tolerance(0.00001)) {

	{ //bool
		auto lambda = [](int i) {
			return i % 2 == 0 ? true : false;
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<bool>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}
	{ //char text
		static char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890?><,./)(*&^%$#@!~+_=-";

			auto lambda = [](int i) {
				int j=(i % strlen(letters));
				return letters[j] ;
			};
			auto lambda2 = [&lambda](std::string extension,
					STREAM_FORMAT expected_format,
					STREAM_COMPRESSION expected_compression) {
				test_by_var_type<char>(extension, expected_format,
						expected_compression, lambda);
			};
			general_test_function_text(lambda2);
		}
	{ //char binary
		auto lambda = [](int i) {
			return (char) (i % 255);
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<char>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function_binary(lambda2);
	}

	{ //string
		auto lambda = [](int i) {
			return std::to_string(i);
		};
		auto lambda2 = [&lambda](std::string extension,
				STREAM_FORMAT expected_format,
				STREAM_COMPRESSION expected_compression) {
			test_by_var_type<std::string>(extension, expected_format,
					expected_compression, lambda);
		};
		general_test_function(lambda2);
	}

}

