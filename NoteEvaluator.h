/*
 * NoteEvaluator.h
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#ifndef NOTEEVALUATOR_H_
#define NOTEEVALUATOR_H_

namespace MidiEngine {

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
	void sighandler(int);
private:
	int fd;
	char verbose;
};

} /* namespace MidiEngine */
#endif /* NOTEEVALUATOR_H_ */
