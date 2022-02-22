// check if QUIT is pressed, and reboot if so
// TODO: clear interrupt vector before reboot
void checkquit() {
__asm__("li r12,>0024\n\tldcr @>0012,3\n\tsrc r12,7\n\tli r12,6\n\tstcr r0,8\n\tandi r0,>1100\n\tjne 4\n\tblwp @>0000" : : : "r12","r0");
}
