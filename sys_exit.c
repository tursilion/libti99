// exit reboots the system
void exit() {
  __asm__("BLWP @>0000");
}
