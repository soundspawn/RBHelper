/*
 * SongReader.h
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#ifndef SONGREADER_H_
#define SONGREADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "MidiSong.h"

namespace MidiEngine {

class SongReader {
public:
	SongReader();
	virtual ~SongReader();

	int read_file(char*,MidiSong*);
};

} /* namespace MidiEngine */
#endif /* SONGREADER_H_ */
