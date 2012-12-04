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
	MidiNote(unsigned long,char[7]);
	virtual ~MidiNote();

	MidiNote* next;
	unsigned long delay;
	char signal[7];
private:
	void create(unsigned long,char[7]);
};

} /* namespace MidiEngine */
#endif /* MIDINOTE_H_ */
