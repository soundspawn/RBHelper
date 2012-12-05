/*
 * SongWriter.cpp
 *
 *  Created on: Dec 5, 2012
 *      Author: xo
 */

#include "SongWriter.h"

namespace MidiEngine {

SongWriter::SongWriter() {
    this->filename = new char[50];
}

SongWriter::~SongWriter() {
    delete(this->filename);
    if(this->file.is_open()){
        this->file.close();
    }
}

int SongWriter::set_file(char* filename){
    strcpy(this->filename,filename);
    if(this->file.is_open()){
        this->file.close();
    }
    this->file.open(filename);
    return 1;
}

int SongWriter::write_note(unsigned long long delay,unsigned char* signal){

    char* del = new char[10];
    char* note = new char[3];
    char* vel = new char[3];

    sprintf(del,"%llu ",delay);
    sprintf(note,"%d ",signal[1]);
    sprintf(vel,"%d ",signal[2]);

    if(this->file.is_open()){
        this->file.write(del,10);
        this->file.write(note,3);
        this->file.write(vel,3);
        return 1;
    }
    return 0;
}

} /* namespace MidiEngine */
