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

} /* namespace MidiEngine */
