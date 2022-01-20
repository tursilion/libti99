// exit reboots the system
// TODO: should clear the interrupt hook
void exit() {
  __asm__("BLWP @>0000");
}
