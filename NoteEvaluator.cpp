/*
 * NoteEvaluator.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#include "NoteEvaluator.h"

namespace MidiEngine {

NoteEvaluator::NoteEvaluator() {
}

NoteEvaluator::~NoteEvaluator() {
}

int NoteEvaluator::set_fd(int fd) {
	if (fd > -1) {
		this->fd = fd;
		return 1;
	}
	//Invalid fd, return it
	return fd;
}

int NoteEvaluator::set_verbose(char verbose){
	this->verbose = verbose;
	return 1;
}

int NoteEvaluator::process_input_as_loop(char* message) {

	unsigned char ch;

	while (1) {
		if (read(this->fd, &ch, 1) > 0) {
			if (ch != 0xf8 && ch != 0xfe) {
				if (verbose) {
					fprintf(stderr, "read %02x\n", ch);
				}
				if (write(fd, &ch, 1) == -1) {
					sprintf(message, "Error writing to device\n");
					return -1;
				}
			}
		} else {
			sprintf(message, "Could not read from device\n");
			return -1;
		}
	}
	return 1;
}

} /* namespace MidiEngine */
