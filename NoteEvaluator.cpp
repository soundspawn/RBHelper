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
	unsigned long long iCurrentNoteTime = 0;
	char verb_buffer[60];
	char msg_buffer[60];
	struct timespec time;

	while (1) {
		if (read(this->fd, &ch, 1) > 0) {
			if (ch != 0xf8 && ch != 0xfe) {
				if(iBitCount == 0){
					//iCurrentNoteTime
					if(clock_gettime(CLOCK_MONOTONIC,&time) == -1){
						sprintf(message, "Error getting time\n");
						return -1;
					}
					iLastNoteTime = iCurrentNoteTime;
					iCurrentNoteTime = time.tv_sec * 1000000000+ time.tv_nsec;
				}
				if (this->verbose) {
					if(iBitCount == 0){
						iColumnWidth = 6;
						sprintf(msg_buffer, "%lu %09lu - ",time.tv_sec,time.tv_nsec);
						strcpy(verb_buffer,msg_buffer);
					}
					if(iBitCount == 1){
						if(ch == 0x04){
							iColumnWidth = 3;
						}
					}
					sprintf(msg_buffer, "%02x ", ch);
					strcat(verb_buffer,msg_buffer);
					iBitCount = (iBitCount+1) % iColumnWidth;
					if(iBitCount == 0){
						if(iCurrentNoteTime - iLastNoteTime < 5000000000){
							sprintf(msg_buffer, "  - % 9lu µs",(iCurrentNoteTime-iLastNoteTime)/1000);
							strcat(verb_buffer,msg_buffer);
						}
						fprintf(stderr, "%s\n",verb_buffer);
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
