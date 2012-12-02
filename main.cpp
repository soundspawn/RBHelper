#include <stdio.h>
#include <alsa/asoundlib.h>
#include "NoteEvaluator.h"

using namespace std;

unsigned char verbose = 0;
MidiEngine::NoteEvaluator* eng;

static void usage(void) {
	fprintf(stderr, "usage: RBHelper [options] <device>\n");
	fprintf(stderr, " options:\n");
	fprintf(stderr, "    -v: verbose mode\n");
	fprintf(stderr, "\n example:\n");
	fprintf(stderr, "    RBHelper /dev/midi1\n");
}

int confirm_device_is_streaming(int fd,unsigned char seconds){
	struct timeval timeout;
	fd_set set;
	//Set timeout structure
	timeout.tv_sec = seconds;
	timeout.tv_usec = 0;
	//Set add our fd to the set
	FD_ZERO(&set);
	FD_SET(fd, &set);
	//Expecting positive number if communication is happening
	return(select(fd + 1,&set,NULL,NULL,&timeout));
}

int closedevice(int fd){
	if (verbose) {
		fprintf(stderr, "Closing\n");
	}
	//Close our handle
	close(fd);
	//Return success
	return 1;
}

int main(int argc, char** argv) {

	int closedevice(int);
	int confirm_device_is_streaming(int,unsigned char);

	int i;
	unsigned char ch;
	char *device = NULL;
	int fd = -1;
	int status;
	char* message;

	//No args
	if (argc == 1) {
		usage();
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'v':
				verbose = 1;
				break;
			}
		}else{
			//Assume <device>
			if (i + 1 == argc) {
				device = argv[i];
				fd = open(device, O_RDWR);
				continue;
			}
		}
	}
	if (fd < 0) {
		fprintf(stderr, "open %s for input and output failed, aborting\n", device);
		exit(-1);
	}
	if(confirm_device_is_streaming(fd,3) < 1){
		ch = 10;
		fprintf(stderr, "Device not streaming data, waiting additional %d seconds...\n",ch);
		if(confirm_device_is_streaming(fd,ch) < 1){
			fprintf(stderr, "Device is not streaming data, aborting\n");
			closedevice(fd);
			exit(-1);
		}
	}
	if (verbose) {
		fprintf(stderr, "Using Device: %s\n", device);
	}
	//Create our engine and hand it control
	fprintf(stderr, "Press ctrl-c to stop\n");
	eng = new MidiEngine::NoteEvaluator();
	eng->set_fd(fd);
	eng->set_verbose(verbose);
	status = eng->process_input_as_loop(message);
	closedevice(fd);
	if(status == -1){
		//Error, display it
		fprintf(stderr, "Error: %s\n",message);
	}else{
		fprintf(stdout, "%s\n",message);
		if(status == 1){
			exit(0);
		}
	}
	exit(status);
}
