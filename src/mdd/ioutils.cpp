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
#include "strutils.h"
#include "ioutils.h"

namespace mdd {

outputstream::outputstream(const std::string &filepath, STREAM_FORMAT format,
		STREAM_COMPRESSION compression) :
		_format(format), _compression(compression), filepath(filepath) {

	init();
}

void outputstream::init() {
	if (_format == STREAM_FORMAT::TEXT_FORMAT) {
		if (_compression == NONE) {
			owrite = std::shared_ptr<std::ostream>(new std::ofstream(filepath));
		} else if (_compression == GZIP) {
			owrite = std::shared_ptr<std::ostream>(
					new ogzstream(filepath.c_str()));
		} else {
			ounderlying = std::shared_ptr<std::ofstream>(
					new std::ofstream(filepath));
			owrite = std::shared_ptr<std::ostream>(
					new lz4_stream::ostream(*ounderlying));
		}
	} else {
		throw 1;
	}

}

outputstream::outputstream(const std::string &filepath) :
		filepath(filepath) {

	if (ends_with(filepath, ".gz")) {
		_compression = STREAM_COMPRESSION::GZIP;
	} else if (ends_with(filepath, ".lz4")) {
		_compression = STREAM_COMPRESSION::LZ4;
	} else {
		_compression = STREAM_COMPRESSION::NONE;
	}

	if (ends_with(filepath, ".bin.gz") || ends_with(filepath, ".bin.lz4")
			|| ends_with(filepath, ".bin")) {
		_format = STREAM_FORMAT::BINARY_FORMAT;
	} else {
		_format = STREAM_FORMAT::TEXT_FORMAT;
	}
	init();
}

outputstream::~outputstream() {
}

void outputstream::close() {
	if (_compression == NONE) {
		((std::ofstream*) owrite.get())->close();
	} else if (_compression == GZIP) {
		((ogzstream*) owrite.get())->close();
	} else if (_compression == LZ4) {
		((lz4_stream::ostream*) owrite.get())->close();
		ounderlying->close();
	}
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
	} else {
		_compression = STREAM_COMPRESSION::NONE;
	}

	if (ends_with(filepath, ".bin.gz") || ends_with(filepath, ".bin.lz4")
			|| ends_with(filepath, ".bin")) {
		_format = STREAM_FORMAT::BINARY_FORMAT;
	} else {
		_format = STREAM_FORMAT::TEXT_FORMAT;
	}

	init();
}

void inputstream::init() {
	if (_format == STREAM_FORMAT::TEXT_FORMAT) {
		if (_compression == NONE) {
			oread = std::shared_ptr<std::istream>(new std::ifstream(filepath));
		} else if (_compression == GZIP) {
			oread = std::shared_ptr<std::istream>(
					new igzstream(filepath.c_str()));
		} else {
			ounderlying = std::shared_ptr<std::ifstream>(
					new std::ifstream(filepath));
			oread = std::shared_ptr<std::istream>(
					new lz4_stream::istream(*ounderlying));

		}

	} else {
		throw 1;
	}
}

inputstream::~inputstream() {
}

void inputstream::close() {
	if (_compression == NONE) {
		((std::ifstream*) oread.get())->close();
	} else if (_compression == NONE) {
		((igzstream*) oread.get())->close();
	} else if (_compression == LZ4) {
		//((lz4_stream::istream*) oread.get())->close();
		ounderlying->close();
	}
}

std::istream& inputstream::istream() {
	return *oread;
}

} //end namespace mdd
