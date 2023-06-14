#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  MIDI_DEVICE  "/dev/midi1"


int main(int argc, char** argv){
    unsigned char packet[4];

    int seqfd = -1;         // sequencer file descriptor

    // first open the sequencer device for reading.
    seqfd = open(MIDI_DEVICE, O_RDWR);
    if (seqfd == -1) {
        printf("Error: cannot open %s\n", MIDI_DEVICE);
        exit(1);
    }

    // now just wait around for MIDI bytes to arrive, when they
    // do, then send notes back

    while(1){
        read(seqfd, &packet, sizeof(packet));
        printf("received MIDI byte: %d\t%d\t%d\t%d\n",
            packet[1],
            packet[2],
            packet[3],
            packet[4]
        );
    }
    return 0;
}
     