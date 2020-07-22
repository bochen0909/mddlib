/*
 * binaryarchive.h
 *
 *  Created on: Jul 22, 2020
 *      Author: bo
 */

#ifndef MDDLIB_SRC_MDD_BINARYARCHIVE_H_
#define MDDLIB_SRC_MDD_BINARYARCHIVE_H_

#include <string>
#include <vector>
#include <fstream>

namespace mdd {

class binary_iarchive {
public:
	binary_iarchive(std::istream &is): is(is){};
	virtual ~binary_iarchive(){};

	template<typename T> binary_iarchive& operator>>(T &v) {
		size_t size = sizeof(T);
		char buf[size];
		is.read(buf, size);
		T *p = static_cast<T*>((void*) buf);
		v = (*p);
		return *this;
	}

protected:

protected:
	std::istream &is;
};

class binary_oarchive {
public:
	binary_oarchive(std::ostream &os):os(os){};
	virtual ~binary_oarchive(){};

	template<typename T> binary_oarchive& operator<<(const T& v){
			add_raw(reinterpret_cast<const char*>(&v), sizeof(T));
			return *this;

	}
protected:
	inline void add_raw(const char *v, size_t size){
		os.write(v, size);
	}

protected:
	std::ostream &os;
};

template<> inline binary_iarchive& binary_iarchive::operator>>(
		std::string &v) {
	while (true) {
		char buf;
		is.read(&buf, 1);
		if (buf) {
			v.push_back(buf);
		} else {
			break;
		}
	}
	return *this;
}

template<> inline binary_oarchive& binary_oarchive::operator<<(const std::string &s) {
	char buf[s.length() + 1];
	char *p1 = buf;
	const char *p2 = s.data();
	for (size_t i = 0; i < s.length(); i++) {
		*p1 = *p2;
		p1++;
		p2++;
	}
	buf[s.length()] = '\0';
	add_raw(reinterpret_cast<const char*>(buf), sizeof(s.length() + 1));
	return *this;
}


} //end mdd
#endif /* MDDLIB_SRC_MDD_BINARYARCHIVE_H_ */
