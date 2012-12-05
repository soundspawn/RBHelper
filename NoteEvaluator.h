/*
 * NoteEvaluator.h
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#ifndef NOTEEVALUATOR_H_
#define NOTEEVALUATOR_H_

#include <alsa/asoundlib.h>
#include <alsa/asoundef.h>
#include <alsa/global.h>
#include <alsa/rawmidi.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include "MidiSong.h"
#include "SongReader.h"
#include "SongWriter.h"

namespace MidiEngine {

#define SIGNAL_HISTORY_SIZE 5

#define INPUT_SOURCE_FD 0
#define INPUT_SOURCE_DEBUG 1
#define INPUT_SOURCE_PLAYER 2

class NoteEvaluator {
public:
    NoteEvaluator();
    virtual ~NoteEvaluator();

    int set_fd(int);
    int set_verbose(char);
    int process_input_as_loop(char*&);
    int set_input(unsigned char);
    int set_input(unsigned char, char*);
    int set_output(char*);
private:
    int fd;
    char verbose;
    char verb_timestamp[40];
    unsigned long long iLastNoteTime;
    unsigned long long iCurrentNoteTime;
    unsigned char input;
    unsigned char signals[7];
    unsigned char old_signals[SIGNAL_HISTORY_SIZE][7];
    unsigned long long old_notetimes[SIGNAL_HISTORY_SIZE];
    char* song_filename;
    MidiSong* track;
    SongWriter* output;

    int new_note();
    int print_output(char*);
    int add_signal(char);
    int signal_to_verb(char*);
    int save_to_history();
    int get_input(unsigned char&);
    int save_note();
};

} /* namespace MidiEngine */
#endif /* NOTEEVALUATOR_H_ */
