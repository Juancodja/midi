//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jan  6 19:00:08 PST 1999
// Last Modified: Wed Jan  6 19:00:18 PST 1999
// Filename:      insimple.c
// Syntax:        C
// $Smake:        gcc -O3 -Wall -o %b %f && strip %b 
//
// Description:   echos MIDI input to MIDI output with a transposition
//		  of the MIDI notes.  This program was designed using
//		  an 88-key synthesizer (a Yamaha Disklavier), so it
//		  won't work the same with smaller-sized keyboards.
//		  The MIDI data is echoed back to the same device from
//		  which it originates, but you could modify the code
//		  to have the input sent to an alternate device
//		  output.  You could also modify this program to 
//		  produce chords when a single note is played on the
//		  keyboard.
//
//		  There are 13 control keys (or so) at the bottom of
//		  the keyboard.  When you press the lowest note on the
//		  keyboard (A0), the transposition value will remain
//		  the same, but the transposition direction will
//		  switch -- from either up to down or down to up.
//		  The other low keys on the keyboard are used for
//		  specifying the transposition interval.  B-flat0
//		  would set the transposition to a minor second (one
//		  semi-tone), B0 would be a major second, C1 is a
//		  minor third, and so on.
//


#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  MIDI_DEVICE  "/dev/sequencer" 


int main(int argc, char** argv) {
   unsigned char packet[4];
   int transposition = 7;    // Tranposition of MIDI notes
   int keyflag = 0;          // true when next byte is a MIDI key number
   int count = 0;            // for checking on running status
   int seqfd = -1;           // sequencer file descriptor

   if (argc == 2) {
      transposition = atoi(argv[1]); 
   }

   // first open the sequencer device for reading.
   seqfd = open(MIDI_DEVICE, O_RDWR);
   if (seqfd == -1) {
      printf("Error: cannot open %s\n", MIDI_DEVICE);
      exit(1);
   } 

   // now just wait around for MIDI bytes to arrive, when they
   // do, then send notes back out with a transposition 
   while (1) {
      read(seqfd, &packet, sizeof(packet));
 
      // process the device packet if it contains one MIDI Byte
      if (packet[0] == SEQ_MIDIPUTC) {
         printf("received MIDI byte: %d\n", packet[1]);
         if ((packet[1] & 0xf0) == 0x80 || (packet[1] & 0xf0) == 0x90) {
            keyflag = 1;
            count = 0;
            write(seqfd, &packet, sizeof(packet));
         } else if (packet[1] > 0x7f) {
            keyflag = 0;
            write(seqfd, &packet, sizeof(packet));
         } else if (keyflag) {
            count++;
            if (count % 2 == 1) {
               if (packet[1] == 21) {
                  transposition = -transposition;
               } else if (packet[1] <= 34) {
                  if (transposition < 0) {
                     transposition = - (packet[1] - 21);
                  } else {
                     transposition = packet[1] - 21;
                  }
               }
               packet[1] += transposition;
               packet[1] = packet[1] % 0x80;
            } 
            write(seqfd, &packet, sizeof(packet));
         }
      } 
   }
      
   return 0;
}


