/*
 * NoteEvaluator.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#include "NoteEvaluator.h"

namespace MidiEngine {

NoteEvaluator::NoteEvaluator() {
	unsigned char j;
	for(j=0;j<SIGNAL_HISTORY_SIZE;j++){
		strcpy(this->old_signals[j],"\0\0\0\0\0\0");
	}
	strcpy(this->signals,"\0\0\0\0\0\0");
	this->input = INPUT_SOURCE_FD;//default fd device;
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

int NoteEvaluator::set_input(unsigned char source){
	this->input = source;
	return 1;
}

int NoteEvaluator::new_note(){
	struct timespec time;

	if(clock_gettime(CLOCK_MONOTONIC,&time) == -1){
		return -1;
	}
	this->iLastNoteTime = this->iCurrentNoteTime;
	this->iCurrentNoteTime = time.tv_sec * 1000000000+ time.tv_nsec;
	if(this->verbose){
		sprintf(this->verb_timestamp, "% 9lu %09lu - ",time.tv_sec,time.tv_nsec);
	}
	strncpy(this->signals,"\0\0\0\0\0\0",6);
	return 1;
}

int NoteEvaluator::save_to_history(){
	unsigned char j;

	for(j = SIGNAL_HISTORY_SIZE-2; j < 254; j--) {
		strcpy(this->old_signals[j+1],this->old_signals[j]);
	}
	strcpy(this->old_signals[0],this->signals);
	return 1;
}

int NoteEvaluator::get_input(unsigned char& ch){
	static unsigned char len = 0;

	if(this->input == INPUT_SOURCE_FD){
		if (read(this->fd, &ch, 1) > 0) {
			return 1;
		} else {
			return -1;
		}
	}
	if(this->input == INPUT_SOURCE_DEBUG){
		len++;
		len = len % 6;
		switch(len){
		case 1:
			ch = 0x99;
			break;
		case 2:
			ch = 0x26;
			break;
		case 3:
			ch = 0x2E;
			break;
		case 4:
			ch = 0x99;
			break;
		case 5:
			ch = 0x26;
			break;
		case 0:
			ch = 0x00;
			usleep(200000);
			break;
		}
		return 1;
	}
	return 0;
}

int NoteEvaluator::process_input_as_loop(char*& message) {

	unsigned char ch;
	unsigned char iBitCount = 0;
	unsigned char iColumnWidth;
	char verb_buffer[60];
	char msg_buffer[20];

	while (1) {
		if(this->get_input(ch) == 1){
			if (ch != 0xf8 && ch != 0xfe) {
				//Reset
				if(iBitCount == 0){
					if(this->new_note() == -1){
						sprintf(message, "Error getting time\n");
						return -1;
					}
					iColumnWidth = 6;
				}
				if(iBitCount == 1){
					//Hi Hat depressed (single 99 04 7F message)
					if(ch == 0x04){
						iColumnWidth = 3;
					}
				}
				if (this->verbose) {
					if(iBitCount == 0){
						strcpy(verb_buffer,this->verb_timestamp);
					}
					sprintf(msg_buffer, "%02x ", ch);
					strcat(verb_buffer,msg_buffer);
				}
				this->signals[iBitCount] = ch;
				iBitCount = (iBitCount+1) % iColumnWidth;
				if(iBitCount == 0){
					if(this->verbose){
						while(iColumnWidth<6){
							strcat(verb_buffer,"   ");
							iColumnWidth++;
						}
						if(this->iCurrentNoteTime - this->iLastNoteTime < 5000000000){
							sprintf(msg_buffer," - % 9lu µs",(this->iCurrentNoteTime-this->iLastNoteTime)/1000);
							strcat(verb_buffer,msg_buffer);
						}
						fprintf(stderr, "%s\n",verb_buffer);
					}
					//Pass the signal for processing
					if (write(fd, this->signals, 5) == -1) {
						sprintf(message, "Error writing to device\n");
						return -1;
					}
					this->save_to_history();
				}
			}
		}
	}
	return 1;
}

} /* namespace MidiEngine */
