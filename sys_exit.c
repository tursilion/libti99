// exit reboots the system
void exit(int x) {
  __asm__("BLWP @>0000");
}
