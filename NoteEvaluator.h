/*
 * NoteEvaluator.h
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#ifndef NOTEEVALUATOR_H_
#define NOTEEVALUATOR_H_

namespace MidiEngine {

#define SIGNAL_HISTORY_SIZE 5

#include <stdio.h>
#include <alsa/asoundlib.h>
#include <alsa/asoundef.h>
#include <alsa/global.h>
#include <alsa/rawmidi.h>
#include <signal.h>
#include <time.h>
#include <math.h>

class NoteEvaluator {
public:
	NoteEvaluator();
	virtual ~NoteEvaluator();

	int set_fd(int);
	int set_verbose(char);
	int process_input_as_loop(char*);
private:
	int fd;
	char verbose;
	char verb_timestamp[40];
	unsigned long long iLastNoteTime;
	unsigned long long iCurrentNoteTime;
	char signals[7];
	char old_signals[SIGNAL_HISTORY_SIZE][7];

	int new_note();
	int print_output(char*);
	int add_signal(char);
	int signal_to_verb(char*);
	int save_to_history();
};

} /* namespace MidiEngine */
#endif /* NOTEEVALUATOR_H_ */
