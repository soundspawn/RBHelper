#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <alsa/asoundlib.h>
#include <alsa/asoundef.h>
#include <alsa/global.h>
#include <alsa/rawmidi.h>
#include <signal.h>

static void usage(void) {
	fprintf(stderr, "usage: RBHelper [options] <device>\n");
	fprintf(stderr, " options:\n");
	fprintf(stderr, "    -v: verbose mode\n");
	fprintf(stderr, "\n example:\n");
	fprintf(stderr, "    RBHelper /dev/midi1\n");
}

int stop = 0;
unsigned char verbose = 0;
void sighandler(int dum) {
	stop = 1;
}

int closedevice(int fd){
	if (verbose) {
		fprintf(stderr, "Closing\n");
	}
	close(fd);
	return 1;
}

int main(int argc, char** argv) {

	int closedevice(int);

	int i;
	unsigned char ch;
	char *device = NULL;
	int fd = -1;
	if (argc == 1) {
		usage();
		exit(0);
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
			if (i + 1 < argc) {
				device = argv[++i];
				fd = open(device, O_RDWR);
				continue;
			}
		}
	}
	if (fd < 0) {
		fprintf(stderr, "open %s for input and output failed\n", device);
		closedevice(fd);
		return -1;
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
					fprintf(stderr, "Error writing to device");
					closedevice(fd);
					return -1;
				}
			}
		}else{
			fprintf(stderr, "Could not read from device");
			closedevice(fd);
			return -1;
		}
	}
	closedevice(fd);
	return 0;
}
