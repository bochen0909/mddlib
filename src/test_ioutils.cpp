#define BOOST_TEST_MODULE example
#include <stdlib.h>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "mdd/strutils.h"
#include "mdd/fsutils.h"
#include "mdd/ioutils.h"

using namespace mdd;
using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_text_non_compress,* utf::tolerance(0.00001)) {
	{
		string filepath = "/tmp/test_text_non_compress.txt";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::NONE == os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();
	}

	{
		string filepath = "/tmp/test_text_non_compress.txt";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::NONE == os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();

		inputstream is(filepath);
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

	{
		string filepath = "/tmp/test_text_non_compress.txt";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::NONE == os.compression());
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
BOOST_AUTO_TEST_CASE(test_text_gzip_compress,* utf::tolerance(0.00001)) {
	{
		string filepath = "/tmp/test_text_gzip_compress.gz";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::GZIP== os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();
	}

	{
		string filepath = "/tmp/test_text_gzip_compress.gz";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::GZIP == os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();

		inputstream is(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == is.format());
		BOOST_TEST(STREAM_COMPRESSION::GZIP == is.compression());
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

	{
		string filepath = "/tmp/test_text_gzip_compress.gz";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::GZIP == os.compression());
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


BOOST_AUTO_TEST_CASE(test_text_lz4_compress,* utf::tolerance(0.00001)) {
	{
		string filepath = "/tmp/test_text_lz4_compress.lz4";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::LZ4== os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();
	}

	{
		string filepath = "/tmp/test_text_lz4_compress.lz4";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::LZ4 == os.compression());
		os << 1 << " " << (size_t) 233 << " " << (uint32_t) -1 << " " << "a_str"
				<< " " << 'c' << "\n";
		os.close();

		inputstream is(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == is.format());
		BOOST_TEST(STREAM_COMPRESSION::LZ4 == is.compression());
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

	{
		string filepath = "/tmp/test_text_lz4_compress.lz4";
		outputstream os(filepath);
		BOOST_TEST(STREAM_FORMAT::TEXT_FORMAT == os.format());
		BOOST_TEST(STREAM_COMPRESSION::LZ4 == os.compression());
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
