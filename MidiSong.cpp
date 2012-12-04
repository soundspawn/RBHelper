/*
 * MidiSong.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#include "MidiSong.h"

namespace MidiEngine {

MidiSong::MidiSong() {
	this->track_start = NULL;
	this->track_end = NULL;
}

MidiSong::~MidiSong() {
	MidiNote* delme;
	MidiNote* next;
	delme = this->track_start;
	while(delme != NULL){
		next = delme->next;
		free(delme);
		delme = next;
	}
}

int MidiSong::add_note(unsigned long delay,char signal[7]){
	this->track_end = new MidiNote(delay,signal);
	if(this->track_start == NULL){
		this->track_start = this->track_end;
	}
	return 1;
}

int MidiSong::cut_note(){
	MidiNote* delme;

	delme = this->track_start;
	this->track_start = this->track_start->next;
	free(delme);
	return 1;
}

char* MidiSong::play_note(){
	static char signal[7];

	usleep(this->track_start->delay);
	strcpy(signal,this->track_start->signal);
	this->cut_note();
	return((char*)signal);
}

} /* namespace MidiEngine */
