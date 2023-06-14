// This file makes a sine wave and plays it through the sound card.

// To compile this file use:
//    gcc -o sound sound.c -lm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#define  SOUND_DEVICE  "/dev/snd/pcmC0D0c"

int main(int argc, char** argv) {
   int fd;
   int arg;
   int status;
   int frequency = 440;
   int volume = 0x7fff;
   int seconds = 2;
   int rate = 44100;
   int i;
   unsigned char buffer[32];
   
    // open sound device
    fd = open(SOUND_DEVICE, O_WRONLY);

    if (fd == -1) {
        printf("Error: cannot open %s\n", SOUND_DEVICE);
        exit(1);
    }
    
    // set sample parameters
    arg = rate;
    status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
    if (status == -1) {
        perror("SOUND_PCM_WRITE_RATE ioctl failed");
        exit(1);
    }
    arg = 1;   // mono
    status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (status == -1) {
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
        exit(1);
    }
    arg = 8;   // 8-bit samples
    status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
    if (status == -1) {
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
        exit(1);
    }

    // write out a sine wave
    for (i = 0; i < rate * seconds; i++) {
        buffer[0] = volume * sin(2 * M_PI * frequency * i / rate);
        write(fd, buffer, 1);
    }

    // close sound device
    close(fd);
    return 0;
}