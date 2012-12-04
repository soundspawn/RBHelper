/*
 * MidiNote.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#include "MidiNote.h"

namespace MidiEngine {

MidiNote::MidiNote() {
	char default_signal[7];
	strcpy(default_signal,"\0\0\0\0\0\0");

	this->create(0,default_signal);
}

MidiNote::MidiNote(unsigned long delay,char signal[7]){
	this->create(delay,signal);
}

MidiNote::~MidiNote() {
}

void MidiNote::create(unsigned long delay,char signal[7]){
	this->delay = delay;
	strcpy(this->signal,signal);
	this->next = NULL;
}

} /* namespace MidiEngine */
