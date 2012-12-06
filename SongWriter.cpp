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
    this->file_counter = 1;
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
    if(this->file_counter > 1){
        sprintf(filename,"%s_%d",filename,this->file_counter);
    }
    this->file.open(filename);
    return 1;
}

int SongWriter::get_new_file(){
    this->file_counter++;
    this->set_file(this->filename);
    return 1;
}

int SongWriter::set_newfile_timer(unsigned int timer){
    this->newfile_timer = timer;
    return 1;
}

int SongWriter::write_note(unsigned long long delay,unsigned char* signal){

    char* del = new char[10];
    char* note = new char[3];
    char* vel = new char[3];

    //Round the delay
    delay /= 1000;
    delay *= 1000;

    sprintf(del,"%llu ",delay);
    sprintf(note,"%d ",signal[1]);
    sprintf(vel,"%d ",signal[2]);

    if(delay > this->newfile_timer*1000000){
        this->get_new_file();
    }
    if(this->file.is_open() && this->file.good()){
        this->file << del << note << vel << std::endl;
        return 1;
    }
    return 0;
}

} /* namespace MidiEngine */
