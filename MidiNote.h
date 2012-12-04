/*
 * MidiNote.h
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#ifndef MIDINOTE_H_
#define MIDINOTE_H_

#include <stdlib.h>
#include <cstring>

namespace MidiEngine {

class MidiNote {
public:
	MidiNote();
	MidiNote(unsigned long,unsigned char*);
	virtual ~MidiNote();

	MidiNote* next;
	unsigned long delay;
	unsigned char signal[7];
private:
	void create(unsigned long,unsigned char[7]);
};

} /* namespace MidiEngine */
#endif /* MIDINOTE_H_ */
