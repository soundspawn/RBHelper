/*
 * SongWriter.h
 *
 *  Created on: Dec 5, 2012
 *      Author: xo
 */

#ifndef SONGWRITER_H_
#define SONGWRITER_H_

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>

namespace MidiEngine {

class SongWriter {
public:
    SongWriter();
    virtual ~SongWriter();
    int set_file(char*);
    int get_new_file();
    int set_newfile_timer(unsigned int);
    int write_note(unsigned long long,unsigned char*);
private:
    char* filename;
    std::ofstream file;
    unsigned int newfile_timer;
    unsigned int file_counter;
};

} /* namespace MidiEngine */
#endif /* SONGWRITER_H_ */
