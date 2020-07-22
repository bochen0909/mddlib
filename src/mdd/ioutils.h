/*
 * ioutils.h
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#ifndef SUBPROJECTS__MDDLIB_SRC_MDD_IOUTILS_H_
#define SUBPROJECTS__MDDLIB_SRC_MDD_IOUTILS_H_

#include <string>
#include <fstream>
#include "binaryarchive.h"
namespace mdd {

enum STREAM_FORMAT {
	TEXT_FORMAT = 0, BINARY_FORMAT
};

enum STREAM_COMPRESSION {
	NONE = 0, GZIP, LZ4, SNAPPY
};

class outputstream {
public:

	outputstream(const std::string &filepath, STREAM_FORMAT format,
			STREAM_COMPRESSION compression);
	outputstream(const std::string &filepath);

	virtual ~outputstream();

	template<typename T> outputstream& operator<<(const T &v) {
		if (_format == TEXT_FORMAT) {
			std::ostream &os = *owrite;
			os << v;
		} else {
			binary_oarchive &os = *oarchive;
			os << v;
		}
		return *this;
	}

	inline STREAM_COMPRESSION compression() {
		return _compression;
	}

	inline STREAM_FORMAT format() {
		return _format;
	}

	void close();

protected:
	void init();
private:
	// ounderlying -> [ounderlying2] -> oread
	std::ostream *ounderlying = NULL;
	std::ostream *ounderlying2 = NULL;
	binary_oarchive *oarchive = NULL;
	std::ostream *owrite = NULL;
	STREAM_FORMAT _format;
	STREAM_COMPRESSION _compression;
	std::string filepath;

};

class inputstream {
public:

	inputstream(const std::string &filepath, STREAM_FORMAT format,
			STREAM_COMPRESSION compression);
	inputstream(const std::string &filepath);
	virtual ~inputstream();

	template<typename T> inputstream& operator>>(T &v) {
		if (_format == TEXT_FORMAT) {
			std::istream &os = *oread;
			os >> v;
		} else {
			auto &os = *iarchive;
			os >> v;
		}
		return *this;
	}

	inline STREAM_COMPRESSION compression() {
		return _compression;
	}

	inline STREAM_FORMAT format() {
		return _format;
	}

	void close();

	template<typename F> void for_each_line(F fun) {
		for (std::string line; std::getline(*oread, line);) {
			fun(line);
		}
	}

	std::istream& istream();
protected:
	void init();
private:
	// ounderlying -> [ounderlying2] -> oread
	std::istream *ounderlying = NULL;
	std::istream *ounderlying2 = NULL;
	binary_iarchive *iarchive = NULL;
	std::istream *oread = NULL;
	STREAM_FORMAT _format;
	STREAM_COMPRESSION _compression;
	std::string filepath;
};

} //end namespace mdd

#endif /* SUBPROJECTS__MDDLIB_SRC_MDD_IOUTILS_H_ */
