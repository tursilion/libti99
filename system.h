#ifndef SYSTEM_H
#define SYSTEM_H

// Halt function -- stops the program and waits for
// the user to press QUIT (Fctn-=). Use this if you
// do not want to simply exit main (which in my startup
// will reset the console, and in the default setup
// will just enter an infinite loop).
void halt() __attribute__ ((noreturn));

// Exit function -- reboots the console.
void exit() __attribute__ ((noreturn));

#endif /* SYSTEM_H */
