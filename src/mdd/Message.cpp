/*
 * Message.cpp
 *
 *  Created on: Jul 17, 2020
 *      Author: bo
 */

#include<iostream>
#include<assert.h>
#include "Message.h"

namespace mdd {
Message::Message() :
		curr_ptr(0) {
}

Message::~Message() {

}

Message& Message::operator<<(uint32_t i) {
	add_raw(reinterpret_cast<uint8_t*>(&i), sizeof(uint32_t));
	return *this;
}

Message& Message::operator>>(uint32_t &i) {
	assert(curr_ptr + sizeof(uint32_t) <= data.size());
	uint8_t *p = data.data();
	p += curr_ptr;
	uint32_t *network_order = static_cast<uint32_t*>((void*) p);
	i = (*network_order);
	curr_ptr += sizeof(uint32_t);
	return *this;
}

Message& Message::operator<<(size_t i) {
	add_raw(reinterpret_cast<uint8_t*>(&i), sizeof(size_t));
	return *this;
}

Message& Message::operator>>(size_t &i) {
	assert(curr_ptr + sizeof(size_t) <= data.size());
	uint8_t *p = data.data();
	p += curr_ptr;
	size_t *network_order = static_cast<size_t*>((void*) p);
	i = (*network_order);
	curr_ptr += sizeof(size_t);
	return *this;
}

Message& Message::operator<<(int8_t integer) {
	add_raw(reinterpret_cast<uint8_t*>(&integer), sizeof(int8_t));
	return *this;
}
Message& Message::operator>>(int8_t &integer) {
	assert(curr_ptr + sizeof(int8_t) <= data.size());
	uint8_t *p = data.data();
	p += curr_ptr;
	int8_t const *network_order = static_cast<int8_t*>((void*) p);
	integer = (*network_order);
	curr_ptr += sizeof(int8_t);
	return *this;

}

Message& Message::operator<<(float i) {
	add_raw(reinterpret_cast<uint8_t*>(&i), sizeof(float));
	return *this;
}

Message& Message::operator>>(float &floating_point) {
	assert(curr_ptr + sizeof(float) <= data.size());
	uint8_t *p = data.data();
	p += curr_ptr;
	float const *network_order = reinterpret_cast<float const*>(p);
	floating_point = (*network_order);
	curr_ptr += sizeof(float);
	return *this;
}

void Message::add_raw(uint8_t *v, size_t size) {
	for (size_t i = 0; i < size; i++) {
		data.push_back(*v);
		v++;
	}
}

inline void Message::assign(char *out, size_t length) {
	size_t i = 0;
	uint8_t *p2 = (uint8_t*) out;
	while (i < length) {
		data.push_back(*p2);
		p2++;
		i++;
	}
}

} //end mdd
