#include <stdio.h>
#include <alsa/asoundlib.h>
#include "NoteEvaluator.h"

using namespace std;

unsigned char verbose = 0;
unsigned char debug = 0;
MidiEngine::NoteEvaluator* eng;

static void usage(void) {
    fprintf(stderr, "usage: RBHelper [options] <device>\n");
    fprintf(stderr, " options:\n");
    fprintf(stderr, "    -v: verbose mode\n");
    fprintf(stderr, "    -d: debug mode (plays a debugging track)\n");
    fprintf(stderr, "    -f <file>: play a specified file\n");
    fprintf(stderr, "    -o <file>: record input to the specified file\n");
    fprintf(stderr, "    -b <seconds>: break the recording into a separate file after <seconds> delay\n");
    fprintf(stderr, "\n example:\n");
    fprintf(stderr, "    RBHelper /dev/midi1\n");
}

int confirm_device_is_streaming(int fd, unsigned char seconds) {
    struct timeval timeout;
    fd_set set;
    //Set timeout structure
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    //Set add our fd to the set
    FD_ZERO(&set);
    FD_SET(fd, &set);
    //Expecting positive number if communication is happening
    return (select(fd + 1, &set, NULL, NULL, &timeout));
}

int closedevice(int fd) {
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
    int confirm_device_is_streaming(int, unsigned char);

    int i;
    unsigned char ch;
    char *device = NULL;
    int fd = -1;
    int status;
    char* message = NULL;
    char* filename = NULL;
    char* output = NULL;
    unsigned int newfile_timer = 0;

    //No args
    if (argc == 1) {
        usage();
        exit(1);
    }

    eng = new MidiEngine::NoteEvaluator();

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'v':
                verbose = 1;
                break;
            case 'd':
                debug = 1;
                break;
            case 'f':
                filename = argv[++i];
                break;
            case 'o':
                output = argv[++i];
                break;
            case 'b':
                sscanf(argv[++i],"%d",&newfile_timer);
                break;
            }
        } else {
            //Assume <device>
            if (i + 1 == argc) {
                device = argv[i];
                continue;
            }
        }
    }
    eng->set_verbose(verbose);
    if (filename != NULL) {
        eng->set_input(INPUT_SOURCE_PLAYER, filename);
    }
    if (output != NULL) {
        eng->set_output(output,newfile_timer);

    }
    fd = open(device, O_RDWR);
    if (debug) {
        eng->set_input(INPUT_SOURCE_DEBUG);
        fprintf(stderr, "***DEBUG MODE***\n");
    } else {
        if (fd < 0) {
            fprintf(stderr, "open %s for input and output failed, aborting\n",
                    device);
            exit(-1);
        }
        if (confirm_device_is_streaming(fd, 3) < 1) {
            ch = 10;
            fprintf(stderr,
                    "Device not streaming data, waiting additional %d seconds...\n",
                    ch);
            if (confirm_device_is_streaming(fd, ch) < 1) {
                fprintf(stderr, "Device is not streaming data, aborting\n");
                closedevice(fd);
                exit(-1);
            }
        }
        if (verbose) {
            fprintf(stderr, "Using Device: %s\n", device);
        }
    }
    fprintf(stderr, "Press ctrl-c to stop\n");
    eng->set_fd(fd);
    status = eng->process_input_as_loop(message);
    if (!debug) {
        closedevice(fd);
    }
    if (status == -1) {
        //Error, display it
        fprintf(stderr, "Error: %s\n", message);
    } else {
        fprintf(stdout, "%s\n", message);
        if (status == 1) {
            exit(0);
        }
    }
    delete (eng);
    exit(status);
}
