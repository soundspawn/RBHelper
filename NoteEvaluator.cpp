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
	unsigned char iBitCount = 0;
	unsigned char iColumnWidth;
	unsigned long long iLastNoteTime = 0;
	unsigned long long iCurrentNoteTime;
	struct timespec time;

	while (1) {
		if (read(this->fd, &ch, 1) > 0) {
			if (ch != 0xf8 && ch != 0xfe) {
				if(iBitCount == 0){
					//iCurrentNoteTime
					clock_gettime(CLOCK_REALTIME,&time);
					iLastNoteTime = iCurrentNoteTime;
					iCurrentNoteTime = time.tv_sec * 1000000000+ time.tv_nsec;
				}
				if (verbose) {
					if(iBitCount == 0){
						iColumnWidth = 6;
						fprintf(stderr, "%lu %09lu - ",time.tv_sec,time.tv_nsec);
					}
					if(iBitCount == 1){
						if(ch == 0x04){
							iColumnWidth = 3;
						}
					}
					fprintf(stderr, "%02x ", ch);
					iBitCount = (iBitCount+1) % iColumnWidth;
					if(iBitCount == 0){
						fprintf(stderr, "\n");
					}
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
