/*
 * MidiSong.h
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#ifndef MIDISONG_H_
#define MIDISONG_H_

#include "MidiNote.h"
#include <unistd.h>

namespace MidiEngine {

class MidiSong {
public:
	MidiSong();
	virtual ~MidiSong();
	int add_note(unsigned long,char[7]);
	char* play_note();
	int cut_note();
private:
	MidiNote* track_start;
	MidiNote* track_end;
};

} /* namespace MidiEngine */
#endif /* MIDISONG_H_ */
