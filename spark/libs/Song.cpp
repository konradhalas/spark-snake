#include "application.h"
#include "Song.h"

int notes[] = {
    0,
    3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,
    1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,
    956,903,852,804,759,716,676,638,602,568,536,506,
    478,451,426,402,379,358,338,319,301,284,268,253,
    239,226,213,201,190,179,169,159,151,142,134,127
};
    
Song::Song(char *song, int tonePin): p(song), songDone(false), default_dur(4), default_oct(6), lowest_oct(3), highest_oct(7), bpm(120), tonePin(tonePin) {
    // duration
    int num;
    if(*p == 'd') {
        p++;
        p++;              
        num = 0;
        while (isdigit(*p)) {
            num = (num * 10) + (*p++ - '0');
        }
        if (num > 0) {
            default_dur = num;
        }
        p++;                   
    }
    // octave
    if(*p == 'o') {
        p++; p++;
        num = *p++ - '0';
        if(num >= lowest_oct && num <=highest_oct) {
            default_oct = num;
        }
        p++;     
    }
    // BPM
    if(*p == 'b')
    {
        p++; 
        p++;
        num = 0;
        while(isdigit(*p)) {
            num = (num * 10) + (*p++ - '0');
        }
        bpm = num;
        p++;                  
    }
    wholenote = (60 * 1000L / bpm) * 4;
    songStart = p;
}

bool Song::playNote() {
    int num;       
    long duration;
    byte scale;
    if (*p) {
        // first, get note duration, if available
        num = 0;
        while(isdigit(*p)) {
            num = (num * 10) + (*p++ - '0');
        }
        
        if (num) {
            duration = wholenote / num;
        }
        else {
            duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
        }
        
        // now get the note
        byte note = 0;
        
        switch(*p)
        {
            case 'c':
                note = 1;
                break;
            case 'd':
                note = 3;
                break;
            case 'e':
                note = 5;
                break;
            case 'f':
                note = 6;
                break;
            case 'g':
                note = 8;
                break;
            case 'a':
                note = 10;
                break;
            case 'b':
                note = 12;
                break;
            case 'p':
            default:
                note = 0;
        }
        p++;
        
        // now, get optional '#' sharp
        if (*p == '#') {
            note++;
            p++;
        }
        
        // now, get optional '.' dotted note
        if (*p == '.') {
            duration += duration/2;
            p++;
        }
        
        // now, get scale
        if (isdigit(*p)) {
            scale = *p - '0';
            p++;
        } else {
            scale = default_oct;
        }
        
        scale += OCTAVE_OFFSET;
        
        if (*p == ',') {
            p++;       // skip comma for next note (or we may be at the end)
        }
        
        // now play the note
        if (note) {
            tone(tonePin, notes[(scale - lowest_oct) * 12 + note], duration);
        } else {
            delay(duration);
        }
        return 1; // note played successfully.
    } else {
        p = songStart;
        return 0; // all done
    }
}

void Song::tone(int pin, int16_t note, int16_t duration) {
  for (int16_t x = 0; x < (duration * 1000 / note); x++) {
    PIN_MAP[pin].gpio_peripheral->BSRR = PIN_MAP[pin].gpio_pin; // HIGH
    delayMicroseconds(note);
    PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin;  // LOW
    delayMicroseconds(note);
  }
}
