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

unsigned char* MidiSong::note_to_signal(unsigned char note){
	static unsigned char signal[7];
	signal[0] = 0x99;
	signal[1] = note;
	signal[2] = 0x30;
	signal[3] = 0x99;
	signal[4] = note;
	signal[5] = 0x00;

	return(signal);
}

int MidiSong::add_note(unsigned long delay,unsigned char note){
	static unsigned char signal[7];
	unsigned char* tsig;
	unsigned char i;

	tsig = this->note_to_signal(note);
	for(i=0;i<7;i++){
		signal[i] = tsig[i];
	}
	if(this->track_end == NULL){
		this->track_end = new MidiNote(delay,signal);
	}else{
		this->track_end->next = new MidiNote(delay,signal);
		this->track_end = this->track_end->next;
	}
	if(this->track_start == NULL){
		this->track_start = this->track_end;
	}
	return 1;
}

int MidiSong::cut_note(){
	MidiNote* delme;

	delme = this->track_start;
	if(this->track_end == this->track_start){
		this->track_end = NULL;
	}
	this->track_start = this->track_start->next;
	if(delme != NULL){
		free(delme);
	}
	if(this->track_start == NULL){
		this->add_note(800000,0x25);
	}
	return 1;
}

unsigned char* MidiSong::play_note(){
	static unsigned char signal[7];
	unsigned char i;

	usleep(this->track_start->delay);
	for(i=0;i<7;i++){
		signal[i] = this->track_start->signal[i];
	}
	this->cut_note();
	return(signal);
}

unsigned char MidiSong::get_stream(){
	static unsigned char signal[7] = "\0\0\0\0\0\0";
	unsigned char* tsig;
	static unsigned char len = 6;
	unsigned char i;

	if(len == 6){
		tsig = this->play_note();
		for(i=0;i<7;i++){
			signal[i] = tsig[i];
		}
		len = 0;
	}
	len++;
	return signal[(len-1)];
}

} /* namespace MidiEngine */
