/*
 * NoteEvaluator.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: xo
 */

#include "NoteEvaluator.h"

namespace MidiEngine {

NoteEvaluator::NoteEvaluator() {
    unsigned char j;
    unsigned char i;

    for (j = 0; j < SIGNAL_HISTORY_SIZE; j++) {
        for (i = 0; i < 7; i++) {
            this->old_signals[j][i] = 0;
        }
        this->old_notetimes[j] = 0;
    }
    for (i = 0; i < 7; i++) {
        this->signals[i] = 0;
    }
    this->song_filename = new char[50];
    this->song_filename[0] = 0x00;
    this->input = INPUT_SOURCE_FD; //default fd device;
}

NoteEvaluator::~NoteEvaluator() {
    delete (this->song_filename);
    delete (this->output);
    delete (this->track);
}

int NoteEvaluator::set_fd(int fd) {
    if (fd > -1) {
        this->fd = fd;
        return 1;
    }
    //Invalid fd, return it
    return fd;
}

int NoteEvaluator::set_verbose(char verbose) {
    this->verbose = verbose;
    return 1;
}

int NoteEvaluator::set_input(unsigned char source) {
    char* temp_filename = new char[1];
    temp_filename[0] = 0x00;
    return (this->set_input(source, temp_filename));
}

int NoteEvaluator::set_input(unsigned char source, char* song_filename) {

    SongReader* song;

    this->input = source;
    if (source == INPUT_SOURCE_DEBUG) {
        if (strlen(this->song_filename) == 0) {
            strcpy(this->song_filename, "./debugsong.song");
        } else {
            return 1;
        }
    } else {
        strcpy(this->song_filename, song_filename);
    }
    if (source == INPUT_SOURCE_PLAYER || source == INPUT_SOURCE_DEBUG) {
        //Create a song
        delete (this->track);
        this->track = new MidiSong();
        song = new SongReader();
        if (this->verbose) {
            fprintf(stderr, "Opening %s for playback\n", this->song_filename);
        }
        song->read_file(this->song_filename, this->track);
    }
    return 1;
}

int NoteEvaluator::set_output(char* output,unsigned int newfile_timer) {
    if(this->output != NULL){
        delete(this->output);
    }
    this->output = new SongWriter();
    this->output->set_file(output);
    this->output->set_newfile_timer(newfile_timer);
    return 1;
}

int NoteEvaluator::new_note() {
    struct timespec time;
    unsigned char i;

    if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
        return -1;
    }
    this->iLastNoteTime = this->iCurrentNoteTime;
    this->iCurrentNoteTime = time.tv_sec * 1000000000 + time.tv_nsec;
    if (this->verbose) {
        sprintf(this->verb_timestamp, "% 9ld %09ld - ", time.tv_sec, time.tv_nsec);
    }
    for (i = 0; i < 7; i++) {
        this->signals[i] = 0;
    }
    return 1;
}

int NoteEvaluator::save_to_history() {
    unsigned char j;
    unsigned char i;

    for (j = SIGNAL_HISTORY_SIZE - 2; j < 254; j--) {
        for (i = 0; i < 7; i++) {
            this->old_signals[j + 1][i] = this->old_signals[j][i];
        }
        this->old_notetimes[j+1] = this->old_notetimes[j];
    }
    for (i = 0; i < 7; i++) {
        this->old_signals[0][i] = this->signals[i];
    }
    this->old_notetimes[0] = this->iCurrentNoteTime;
    return 1;
}

int NoteEvaluator::get_input(unsigned char& ch) {
    if (this->input == INPUT_SOURCE_FD) {
        if (read(this->fd, &ch, 1) > 0) {
            return 1;
        } else {
            return -1;
        }
    } else {
        ch = this->track->get_stream();
        return 1;
    }
    return 0;
}

int NoteEvaluator::process_input_as_loop(char*& message) {

    unsigned char ch;
    unsigned char iBitCount = 0;
    unsigned char iColumnWidth;
    char verb_buffer[60];
    char msg_buffer[20];
    unsigned char sending;
    static unsigned long long delay = 0;
    static unsigned long long threshold = 0;
    static unsigned int j;
    unsigned char midibuffer[7];
    static unsigned long long temp = 0;

    while (1) {
        if (this->get_input(ch) == 1) {
            if (ch != 0xf8 && ch != 0xfe) {
                //Reset
                if (iBitCount == 0) {
                    if (this->new_note() == -1) {
                        sprintf(message, "Error getting time\n");
                        return -1;
                    }
                    iColumnWidth = 6;
                }
                if (iBitCount == 1) {
                    //Hi Hat depressed (single 99 04 7F message)
                    if (ch == 0x04) {
                        iColumnWidth = 3;
                    }
                }
                if (this->verbose) {
                    if (iBitCount == 0) {
                        strcpy(verb_buffer, this->verb_timestamp);
                    }
                    sprintf(msg_buffer, "%02x ", ch);
                    strcat(verb_buffer, msg_buffer);
                }
                this->signals[iBitCount] = ch;
                iBitCount = (iBitCount + 1) % iColumnWidth;
                if (iBitCount == 0) {
                    //Pass the signal for processing
                    sending = 1;
                    //if (this->fd > 0) {
                        delay = (this->iCurrentNoteTime - this->iLastNoteTime) / 1000;
                        //Hi Hat Splash
                        if(this->signals[1] == 0x2E && this->old_signals[0][1] == 0x04 && delay < 2500){
                            sending = 0;
                        }
                        //Hi Hat Sensitivity
                        if(this->signals[1] == 0x2E && this->signals[2] < 0x30){
                            sending = 0;
                        }
                        //Hi Hat Pedal press
                        if(this->signals[0] == 0xB9 && this->signals[1] == 0x04){
                            sending = 0;
                        }
                        //Ride double hits
                        if(this->signals[1] == 0x33 && this->signals[2] < 55){
                            sending = 0;
                        }
                        //Cymbal/hihat Doubles
                        if(sending && (this->signals[1] == 0x2E || this->signals[1] == 0x2F || this->signals[1] == 0x31 || this->signals[1] == 0x33)){
                            threshold = 60000;//Time threshold
                            for(j=0;j<SIGNAL_HISTORY_SIZE;j++){
                                //If the delay is already greater than our threshold, it's not a double hit
                                if(delay >= threshold){
                                    break;
                                }
                                //If the old signal was a hit hat hit
                                if(this->old_signals[j][1] == this->signals[1]){
                                    sending = 0;
                                    break;
                                }
                                //If it was not within a hi hat hit, increase the delay by it's delay (making future checks harder because there were longer ago)
                                if(j==(SIGNAL_HISTORY_SIZE-1)){
                                    break;
                                }
                                temp = (this->iCurrentNoteTime-this->old_notetimes[j+1]) / 1000;
                                delay = temp;
                            }
                        }
                        //Any double hits
                        threshold = 9000;//Time threshold
                        for(j=0;j<SIGNAL_HISTORY_SIZE;j++){
                            //If the delay is already greater than our threshold, it's not a double hit
                            if(delay >= threshold){
                                break;
                            }
                            //If the old signal was a hit hat hit
                            if(this->old_signals[j][1] == this->signals[1]){
                                sending = 0;
                                break;
                            }
                            //If it was not within a hi hat hit, increase the delay by it's delay (making future checks harder because there were longer ago)
                            if(j==(SIGNAL_HISTORY_SIZE-1)){
                                break;
                            }
                            temp = (this->iCurrentNoteTime-this->old_notetimes[j+1])/1000;
                            delay = delay+temp;
                        }

                        //If the note is valid, pass it through
                        if(sending){
                            for(j=0;j<6;j++){
                                midibuffer[j] = this->signals[j];
                            }
                            //Substitution
                            if(this->signals[1] == 0x2F){
                                midibuffer[1] = 0x33;
                                midibuffer[4] = 0x33;
                            }
                            if (write(fd, midibuffer, 5) == -1) {
                                sprintf(message, "Error writing to device\n");
                                return -1;
                            }
                        }
                    //}
                    if(sending || (this->signals[1] == 0x04)){
                        //If recording, save the entry
                        if(this->output != NULL && this->signals[1] != 0x04){
                            this->save_note();
                        }
                        this->save_to_history();
                    }
                    if (this->verbose){// && sending) {
                        while (iColumnWidth < 6) {
                            strcat(verb_buffer, "   ");
                            iColumnWidth++;
                        }
                        if (this->iCurrentNoteTime - this->iLastNoteTime < 5000000000) {
                            sprintf(msg_buffer, " - % 9lld µs",(this->iCurrentNoteTime - this->iLastNoteTime) / 1000);
                            strcat(verb_buffer, msg_buffer);
                        }
                        if(sending == 0){
                            fprintf(stderr,"    ");
                        }
                        fprintf(stderr, "%s\n", verb_buffer);
                    }
                }
            }
        }
    }
    return 1;
}

int NoteEvaluator::save_note(){

    unsigned long long delay;
    delay = (this->iCurrentNoteTime - this->iLastNoteTime) / 1000;
    if(this->iLastNoteTime == 0){
        delay = 0;
    }

    if(this->output != NULL){
        this->output->write_note(delay,this->signals);
        return 1;
    }
    return 0;
}

} /* namespace MidiEngine */
