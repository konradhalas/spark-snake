#ifndef SONG_H_
#define SONG_H

#include "application.h"

#define OCTAVE_OFFSET 0

class Song {
    public:
        Song(char *song, int tonePin);
        bool playNote();
        static void tone(int pin, int16_t note, int16_t duration);
    private:
        char *p;
        char *songStart;
        bool songDone;
        byte default_dur;
        byte default_oct;
        byte lowest_oct;
        byte highest_oct;
        int bpm;
        long wholenote;
        int tonePin;
};

#endif
