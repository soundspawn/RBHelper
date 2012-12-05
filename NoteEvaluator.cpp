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
	unsigned char i;

	for(j=0;j<SIGNAL_HISTORY_SIZE;j++){
		for(i=0;i<7;i++){
			this->old_signals[j][i] = 0;
		}
	}
	for(i=0;i<7;i++){
		this->signals[i] = 0;
	}
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
	if(source == INPUT_SOURCE_DEBUG){
		//Create a song
		this->track = new MidiSong();
		this->track->add_note(0,0x25);
		this->track->add_note(800000,0x28);
		this->track->add_note(800000,0x25);
		this->track->add_note(400000,0x25);
		this->track->add_note(400000,0x28);
	}
	return 1;
}

int NoteEvaluator::new_note(){
	struct timespec time;
	unsigned char i;

	if(clock_gettime(CLOCK_MONOTONIC,&time) == -1){
		return -1;
	}
	this->iLastNoteTime = this->iCurrentNoteTime;
	this->iCurrentNoteTime = time.tv_sec * 1000000000+ time.tv_nsec;
	if(this->verbose){
		sprintf(this->verb_timestamp, "% 9ld %09ld - ",time.tv_sec,time.tv_nsec);
	}
	for(i=0;i<7;i++){
		this->signals[i] = 0;
	}
	return 1;
}

int NoteEvaluator::save_to_history(){
	unsigned char j;
	unsigned char i;

	for(j = SIGNAL_HISTORY_SIZE-2; j < 254; j--) {
		for(i=0;i<7;i++){
			this->old_signals[j+1][i] = this->old_signals[j][i];
		}
	}
	for(i=0;i<7;i++){
		this->old_signals[0][i] = this->signals[i];
	}
	return 1;
}

int NoteEvaluator::get_input(unsigned char& ch){
	if(this->input == INPUT_SOURCE_FD){
		if (read(this->fd, &ch, 1) > 0) {
			return 1;
		} else {
			return -1;
		}
	}
	if(this->input == INPUT_SOURCE_DEBUG){
		ch = this->track->get_stream();
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
							sprintf(msg_buffer," - % 9lld µs",(this->iCurrentNoteTime-this->iLastNoteTime)/1000);
							strcat(verb_buffer,msg_buffer);
						}
						fprintf(stderr, "%s\n",verb_buffer);
					}
					//Pass the signal for processing
					if(fd > 0){
						if (write(fd, this->signals, 5) == -1) {
							sprintf(message, "Error writing to device\n");
							return -1;
						}
					}
					this->save_to_history();
				}
			}
		}
	}
	return 1;
}

} /* namespace MidiEngine */
