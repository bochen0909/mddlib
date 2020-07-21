#define BOOST_TEST_MODULE example
#include <stdlib.h>
#include <boost/test/unit_test.hpp>
#include "mdd/strutils.h"
#include "mdd/fsutils.h"

using namespace mdd;
using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_check_path_dir,* utf::tolerance(0.00001)) {
	{
		string dir = "/tmp/mdd_test_check_path_dir";
		string dirlink = "/tmp/mdd_test_check_path_dir_link";

		//clean first
		remove_path(dir);
		remove_path(dirlink);
		BOOST_TEST(!path_exists(dir));
		BOOST_TEST(!path_exists(dirlink));

		//mkdir
		system(("mkdir " + dir).c_str());
		BOOST_TEST(path_exists(dir));

		system(("ln -s " + dir + " " + dirlink).c_str());
		BOOST_TEST(path_exists(dirlink));

		BOOST_TEST(is_directory(dir));
		BOOST_TEST(is_directory(dirlink));
		BOOST_TEST(!is_file(dir));
		BOOST_TEST(!is_file(dirlink));

		//remove dir first
		remove_path(dir);
		BOOST_TEST(!path_exists(dir));
		BOOST_TEST(!path_exists(dirlink));

		//remove link
		remove_path(dirlink);
		BOOST_TEST(!path_exists(dirlink));
	}

}

BOOST_AUTO_TEST_CASE(test_check_path_file,* utf::tolerance(0.00001)) {
	{
		string file = "/tmp/mdd_test_check_path_file";
		string filelink = "/tmp/mdd_test_check_path_file_link";

		//clean first
		remove_path(file);
		remove_path(filelink);
		BOOST_TEST(!path_exists(file));
		BOOST_TEST(!path_exists(filelink));

		//mkdir
		system(("touch " + file).c_str());
		BOOST_TEST(path_exists(file));

		system(("ln -s " + file + " " + filelink).c_str());
		BOOST_TEST(path_exists(filelink));

		BOOST_TEST(!is_directory(file));
		BOOST_TEST(!is_directory(filelink));
		BOOST_TEST(is_file(file));
		BOOST_TEST(is_file(filelink));

		//remove file first
		remove_path(file);
		BOOST_TEST(!path_exists(file));
		BOOST_TEST(!path_exists(filelink));

		//remove link
		remove_path(filelink);
		BOOST_TEST(!path_exists(filelink));
	}

}

BOOST_AUTO_TEST_CASE(test_path_names,* utf::tolerance(0.00001)) {
	{

		cout << "absolute path for '.' is " << absolute_path(".") << endl;
		cout << "absolute path for '/tmp/../tmp/../tmp' is "
				<< absolute_path("/tmp/../tmp/../tmp") << endl;
		//cout << "absolute path for 'notexists' is " << absolute_path("notexists") << endl;

	}
	{
		BOOST_TEST("a.txt" == get_basename("a.txt"));
		BOOST_TEST("a.txt" == get_basename("/tmp/a.txt"));

		BOOST_TEST(get_working_dir() == get_dirname("a.txt"));
		BOOST_TEST("/tmp" == get_dirname("/tmp/a.txt"));

		BOOST_TEST("" == get_file_extension("afile"));
		BOOST_TEST("" == get_file_extension("/tmp/afile"));
		BOOST_TEST("zip" == get_file_extension("a.zip"));
		BOOST_TEST("zip" == get_file_extension("./a.zip"));
		BOOST_TEST("zip" == get_file_extension("./b/zip/a.zip"));

		BOOST_TEST("/tmp/afile" == strip_file_extension("/tmp/afile"));
		BOOST_TEST("/tmp/afile" == strip_file_extension("/tmp/afile.zip"));
		BOOST_TEST(
				"/tmp/afile.tar" == strip_file_extension("/tmp/afile.tar.gz"));
	}

	{
		string s = "/tmp/not_eixist_test/b/c/d";
		create_directory(s);
		BOOST_TEST(is_directory(s));
		remove_path(s);
	}

}

BOOST_AUTO_TEST_CASE(test_glob,* utf::tolerance(0.00001)) {
	{
		auto lst = glob("/tmp");
		for (auto &s : lst) {
			cout << "in /tmp: " << s << endl;
		}
	}

	{
		auto lst = glob("/etc", "*.conf");
		for (auto &s : lst) {
			cout << "in /etc: " << s << endl;
		}
	}
}

