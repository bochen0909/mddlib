/*
 * Message.h
 *
 *  Created on: Jul 17, 2020
 *      Author: bo
 */

#ifndef SUBPROJECTS__SPARC_MPI_SRC_SPARC_MESSAGE_H_
#define SUBPROJECTS__SPARC_MPI_SRC_SPARC_MESSAGE_H_

#include <string>
#include <vector>

namespace mdd{

class Message {
public:
	Message();
	virtual ~Message();

	Message& operator<<(uint32_t i);
	Message& operator<<(int8_t i);
	Message& operator<<(float i);
	Message& operator>>(uint32_t& i);
	Message& operator>>(int8_t &integer);
	Message& operator>>(float &floating_point);

	Message& operator<<(size_t i);
	Message& operator>>(size_t& i);


protected:
	inline void add_raw(uint8_t* v, size_t size);
	inline void assign(char* out,size_t length);

protected:
	size_t curr_ptr;
	std::vector<uint8_t> data;
};

}
#endif /* SUBPROJECTS__SPARC_MPI_SRC_SPARC_MESSAGE_H_ */
