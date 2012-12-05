/*
 * SongReader.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: xo
 */

#include "SongReader.h"

namespace MidiEngine {

SongReader::SongReader() {
	// TODO Auto-generated constructor stub

}

SongReader::~SongReader() {
	// TODO Auto-generated destructor stub
}

int SongReader::read_file(char* filename,MidiSong* track){
	std::ifstream songfile;
	char* delay = new char[50];
	char* note = new char[5];
	unsigned int notecode;

	songfile.open(filename, std::ifstream::in);
	while(!songfile.eof()){
		songfile>>delay;
		songfile>>note;
		sscanf(note,"%x",&notecode);
		track->add_note(atoi(delay),notecode);
	}
	songfile.close();
	return 1;
}

} /* namespace MidiEngine */
