#define BOOST_TEST_MODULE example
#include <boost/test/unit_test.hpp>
#include "mdd/strutils.h"
#include "mdd/sysutils.h"

using namespace mdd;
using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_getenv,* utf::tolerance(0.00001)) {
	string s;
	{
		s = get_env("HOME");
		BOOST_TEST(!s.empty(), "");
		s = get_env("HOME");
		BOOST_TEST(!s.empty());
	}
	{
		BOOST_TEST(!has_env("NEVEL_EXIST_ENV_HERE"));
	}
	{
		std::string name = "TMP_MEM_SET_MEM";
		set_env(name, "100M");
		BOOST_TEST("100M"==get_env(name));

	}
	{
		std::string name = "TMP_MEM_SET_MEM";
		set_env(name, 10000);
		int32_t i;
		i = get_env<int32_t>(name, -1);
		BOOST_TEST(10000==i);

	}
	{
		size_t i;
		std::string name = "TMP_MEM_SET_MEM";
		set_env(name, 10000);
		i = get_env_as_size_bytes(name);
		BOOST_TEST(10000==i);

		set_env(name, "10000");
		i = get_env_as_size_bytes(name);
		BOOST_TEST(10000==i);

		set_env(name, " 10M ");
		i = get_env_as_size_bytes(name);
		BOOST_TEST(10*1024*1024==i);

		set_env(name, " 10g ");
		i = get_env_as_size_bytes(name);
		BOOST_TEST((size_t) 10*1024*1024*1024==i);

		set_env(name, " 10k ");
		i = get_env_as_size_bytes(name);
		BOOST_TEST(10*1024==i);

	}

	{
		s=get_hostname();
		BOOST_TEST(!s.empty());
	}

}

