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

MidiNote::~MidiNote() {
	free(this);
}

void MidiNote::create(unsigned long delay,char* signal){

}

} /* namespace MidiEngine */
