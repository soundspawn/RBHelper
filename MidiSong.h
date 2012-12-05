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
    unsigned char* note_to_signal(unsigned char, unsigned char);
    int add_note(unsigned long, unsigned char, unsigned char);
    unsigned char* play_note();
    int cut_note();
    unsigned char get_stream();
private:
    MidiNote* track_start;
    MidiNote* track_end;
};

} /* namespace MidiEngine */
#endif /* MIDISONG_H_ */
