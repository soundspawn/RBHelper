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
	char* c_buffer = new char[255];
	unsigned long long delay;
	char* del = new char[10];
	char* note = new char[5];
	char* vel = new char[5];
	unsigned char notecode;
	unsigned char velocity;

	songfile.open(filename);
	if(!songfile.is_open()){
		c_buffer = getcwd(c_buffer,255);
		fprintf(stderr,"File Note Found!\npath: %s\nfile: %s\n\n",c_buffer,filename);
		return 1;
	}
	while(!songfile.eof() && songfile.good()){
		songfile.getline(c_buffer,255);
		sscanf(c_buffer,"%s %s %s",del,note,vel);
		delay = atoi(del);
		notecode = atoi(note);
		velocity = atoi(vel);
		track->add_note(delay,notecode,velocity);
	}
	songfile.close();
	return 1;
}

} /* namespace MidiEngine */
