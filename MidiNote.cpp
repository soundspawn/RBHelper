/*
 * MidiNote.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#include "MidiNote.h"

namespace MidiEngine {

MidiNote::MidiNote() {
    unsigned char default_signal[7];
    unsigned char i;

    for (i = 0; i < 7; i++) {
        default_signal[i] = 0x00;
    }

    this->create(0, default_signal);
}

MidiNote::MidiNote(unsigned long delay, unsigned char* signal) {
    this->create(delay, signal);
}

MidiNote::~MidiNote() {
}

void MidiNote::create(unsigned long delay, unsigned char signal[7]) {
    unsigned char i;

    this->delay = delay;
    for (i = 0; i < 7; i++) {
        this->signal[i] = signal[i];
    }
    this->next = NULL;
}

} /* namespace MidiEngine */
