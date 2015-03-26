RBHelper
========

Rock Band USB note detection helper

##Purpose
I have a v-drum set and a rockband midi adapter, but between the two there are many sensitivity issues.  For example the hi hat contains a *lot* of double hits at higher sensitivities and ignores all but the sternest of hits if you turn the sensitivity down much at all.  This directly translates to "missing" or "extra" hits which count against you and disrupt the drum audio track.

After working with the sensitivity of both devices I eventually realized I would need to build a middle device that can read the midi notes and decide what to pass and what to hold.

Effectively this program takes in all midi information and uses recent information to filter out the extra noise - allowing the drums and midi adapter to be near or at max sensitivity without the extra hits.

##Requirements
* A midi output from your drums
* A midi input on your computer
* A midi output from your computer
* A midi controller for your game device
* Linux(?)  Never tested with Windows or Mac.

###Compiling
* Need a /dev/midi device.  I used "sudo apt-get install midisport-firmware"
* Need a few support files, asoundlib.h - "sudo apt-get install libasound2-dev"
* And need timer.h - "sudo apt-get install libc6-dev"
* Compile with g++ -lrt main.cpp

###Sample set up
* [Simmons SD7PK](http://www.amazon.com/Simmons-SD7PK-Electronic-Drum-Set/dp/B003XXM8R4)
* [M-Audio Midisport Uno](http://www.amazon.com/M-Audio-Midisport-Uno-MIDI-Interface/dp/B00007JRBM)
* [Xbox 360 Midi PRO-Adapter](http://www.amazon.com/Xbox-360-Rock-Band-Midi-PRO-Adapter/dp/B00409SOD2)

##Usage
`RBHelper [options] <device>`

    -v: verbose mode
    -d: debug mode (plays a debugging track)
    -f <file>: play a specified file
    -o <file>: record input to the specified file
    -b <seconds>: break the recording into a separate file after <seconds> delay

Most of these options were developed for testing, and may only be partially working at the time of this documentation.

My personal launch script contains
`RBHelper -v /dev/midi1`

##How to adjust
The individual settings are hard coded but can be overridden (check commit history to see me fumbling around doing just that).  To find the note code and intensity run with -v and watch the output.
