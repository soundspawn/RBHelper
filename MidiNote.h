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
	MidiNote(unsigned long,char*);
	virtual ~MidiNote();
private:
	void create(unsigned long,char*);

	unsigned long delay;
	char signal[7];
	MidiNote* next;
};

} /* namespace MidiEngine */
#endif /* MIDINOTE_H_ */
