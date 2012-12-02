#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <alsa/asoundlib.h>
#include <alsa/asoundef.h>
#include <alsa/global.h>
#include <alsa/rawmidi.h>
#include <signal.h>

int stop = 0;
unsigned char verbose = 0;

static void usage(void) {
	fprintf(stderr, "usage: RBHelper [options] <device>\n");
	fprintf(stderr, " options:\n");
	fprintf(stderr, "    -v: verbose mode\n");
	fprintf(stderr, "\n example:\n");
	fprintf(stderr, "    RBHelper /dev/midi1\n");
}

void sighandler(int dum) {
	stop = 1;
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

	fprintf(stderr, "Press ctrl-c to stop\n");
	while (!stop) {
		if(read(fd, &ch, 1) > 0){
			if (ch != 0xf8 && ch != 0xfe) {
				if (verbose) {
					fprintf(stderr, "read %02x\n", ch);
				}
				if(write(fd, &ch, 1) == -1){
					fprintf(stderr, "Error writing to device\n");
					closedevice(fd);
					exit(-1);
				}
			}
		}else{
			fprintf(stderr, "Could not read from device\n");
			closedevice(fd);
			exit(-1);
		}
	}
	closedevice(fd);
	return(0);
}
