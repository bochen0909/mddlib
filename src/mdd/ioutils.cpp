/*
 * ioutils.cpp
 *
 *  Created on: Jul 21, 2020
 *      Author: bo
 */

#include <string>
#include <fstream>
#include "gzstream.h"
#include "lz4_stream.h"
#include "snappystream.hpp"
#include "strutils.h"
#include "ioutils.h"

namespace mdd {

template<typename T> void close_resource(T *&p) {
	if (p) {
		if (dynamic_cast<std::ofstream*>(p)) {
			((std::ofstream*) p)->close();
		} else if (dynamic_cast<ogzstream*>(p)) {
			((ogzstream*) p)->close();
		} else if (dynamic_cast<lz4_stream::ostream*>(p)) {
			((lz4_stream::ostream*) p)->close();
		} else if (dynamic_cast<std::ifstream*>(p)) {
			((std::ifstream*) p)->close();
		} else if (dynamic_cast<igzstream*>(p)) {
			((igzstream*) p)->close();
		} else if (dynamic_cast<snappy::oSnappyStream*>(p)) {
			//((snappy::oSnappyStream*) p)->close();
		} else if (dynamic_cast<snappy::iSnappyStream*>(p)) {
			//((snappy::iSnappyStream*) p)->close();

		}
		delete p;
		p = NULL;
	}
}

outputstream::outputstream(const std::string &filepath, STREAM_FORMAT format,
		STREAM_COMPRESSION compression) :
		_format(format), _compression(compression), filepath(filepath) {

	init();
}

void outputstream::init() {
	if (_format == STREAM_FORMAT::TEXT_FORMAT) {
		if (_compression == NONE) {
			owrite = new std::ofstream(filepath);
		} else if (_compression == GZIP) {
			owrite = new ogzstream(filepath.c_str());
		} else if (_compression == LZ4) {
			ounderlying = new std::ofstream(filepath);
			owrite = new lz4_stream::ostream(*ounderlying);
		} else if (_compression == SNAPPY) {
			ounderlying = new std::ofstream(filepath);
			owrite = new snappy::oSnappyStream(*ounderlying);
		}
	} else { //binary

		if (_compression == NONE) {
			ounderlying = new std::ofstream(filepath);
			oarchive = new binary_oarchive(*ounderlying);
		} else if (_compression == GZIP) {
			ounderlying = new ogzstream(filepath.c_str());
			oarchive = new binary_oarchive(*ounderlying);
		} else if (_compression == LZ4) {
			ounderlying = new std::ofstream(filepath);
			ounderlying2 = new lz4_stream::ostream(*ounderlying);
			oarchive = new binary_oarchive(*ounderlying2);
		} else if (_compression == SNAPPY) {
			ounderlying = new std::ofstream(filepath);
			ounderlying2 = new snappy::oSnappyStream(*ounderlying);
			oarchive = new binary_oarchive(*ounderlying2);
		}

	}

}

outputstream::outputstream(const std::string &filepath) :
		filepath(filepath) {

	if (ends_with(filepath, ".gz")) {
		_compression = STREAM_COMPRESSION::GZIP;
	} else if (ends_with(filepath, ".lz4")) {
		_compression = STREAM_COMPRESSION::LZ4;
	} else if (ends_with(filepath, ".snappy")) {
		_compression = STREAM_COMPRESSION::SNAPPY;
	} else {
		_compression = STREAM_COMPRESSION::NONE;
	}

	if (ends_with(filepath, ".bin.gz") || ends_with(filepath, ".bin.lz4")
			|| ends_with(filepath, ".bin")|| ends_with(filepath, ".bin.snappy")) {
		_format = STREAM_FORMAT::BINARY_FORMAT;
	} else {
		_format = STREAM_FORMAT::TEXT_FORMAT;
	}
	init();
}

outputstream::~outputstream() {
	close();
}

void outputstream::close() {
	close_resource(owrite);
	close_resource(oarchive);
	close_resource(ounderlying2);
	close_resource(ounderlying);

}

inputstream::inputstream(const std::string &filepath, STREAM_FORMAT format,
		STREAM_COMPRESSION compression) :
		_format(format), _compression(compression), filepath(filepath) {

	init();

}

inputstream::inputstream(const std::string &filepath) :
		filepath(filepath) {

	if (ends_with(filepath, ".gz")) {
		_compression = STREAM_COMPRESSION::GZIP;
	} else if (ends_with(filepath, ".lz4")) {
		_compression = STREAM_COMPRESSION::LZ4;
	} else if (ends_with(filepath, ".snappy")) {
		_compression = STREAM_COMPRESSION::SNAPPY;
	} else {
		_compression = STREAM_COMPRESSION::NONE;
	}

	if (ends_with(filepath, ".bin.gz") || ends_with(filepath, ".bin.lz4")
			|| ends_with(filepath, ".bin")|| ends_with(filepath, ".bin.snappy")) {
		_format = STREAM_FORMAT::BINARY_FORMAT;
	} else {
		_format = STREAM_FORMAT::TEXT_FORMAT;
	}

	init();
}

void inputstream::init() {
	if (_format == STREAM_FORMAT::TEXT_FORMAT) {
		if (_compression == NONE) {
			oread = new std::ifstream(filepath);
		} else if (_compression == GZIP) {
			oread = new igzstream(filepath.c_str());
		} else if (_compression == LZ4) {
			ounderlying = new std::ifstream(filepath);
			oread = new lz4_stream::istream(*ounderlying);
		} else if (_compression == SNAPPY) {
			ounderlying = new std::ifstream(filepath);
			oread = new snappy::iSnappyStream(*ounderlying);
		}

	} else {
		if (_compression == NONE) {
			ounderlying = new std::ifstream(filepath);
			iarchive = new binary_iarchive(*ounderlying);
		} else if (_compression == GZIP) {
			ounderlying = new igzstream(filepath.c_str());
			iarchive = new binary_iarchive(*ounderlying);
		} else if (_compression == LZ4) {
			ounderlying = new std::ifstream(filepath);
			ounderlying2 = new lz4_stream::istream(*ounderlying);
			iarchive = new binary_iarchive(*ounderlying2);
		} else if (_compression == SNAPPY) {
			ounderlying = new std::ifstream(filepath);
			ounderlying2 = new snappy::iSnappyStream(*ounderlying);
			iarchive = new binary_iarchive(*ounderlying2);
		}
	}
}

inputstream::~inputstream() {
	close();
}

void inputstream::close() {
	close_resource(oread);
	close_resource(iarchive);
	close_resource(ounderlying2);
	close_resource(ounderlying);
}

std::istream& inputstream::istream() {
	if (_format == BINARY_FORMAT) {
		throw std::runtime_error("binary format is not supported");
	} else {
		return *oread;
	}
}

} //end namespace mdd
