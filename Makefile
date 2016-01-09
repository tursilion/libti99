# Paths to TMS9900 compilation tools
GAS=/cygdrive/c/cygwin/home/tursi/bin/tms9900-as
LD=/cygdrive/c/cygwin/home/tursi/bin/tms9900-ld
CC=/cygdrive/c/cygwin/home/tursi/bin/tms9900-gcc
AR=/cygdrive/c/cygwin/home/tursi/bin/tms9900-ar
CP=/usr/bin/cp
ELF2EA5=/cygdrive/c/cygwin/home/tursi/elf2ea5
EA5PLIT=/cygdrive/c/cygwin/home/tursi/ea5split/ea5split

LDFLAGS_EA5=\
  --section-start .text=a000 --section-start .data=2080 -M

OBJECT_LIST_EA5=\
  crt0_ea5.o\

# output file
NAME=libti99.a

C_FLAGS=\
  -O2 -std=c99 -s --save-temp -I./ -fno-builtin

# List of compiled objects used in executable
OBJECT_LIST=\
  dsr_files.o		\
  dsr_dsrlnk.o		\
  dsr_dsrlnkraw.o	\
  grom_readdata.o	\
  grom_writedata.o	\
  kscan.o			\
  kscanfast.o		\
  joystfast.o		\
  player.o			\
  puff.o        \
  puff16k.o     \
  tiplayer.o		\
  tiplayer30hz.o	\
  tiplayersfx.o		\
  tiplayersfx30hz.o	\
  stcount.o			\
  str_atoi.o      \
  str_gets.o      \
  str_int2str.o   \
  str_memcpy.o    \
  str_strcpy.o    \
  str_strlen.o		\
  str_uint2str.o  \
  sys_exit.o      \
  sys_halt.o			\
  vdp_char.o		\
  vdp_charset.o		\
  vdp_charsetlc.o	\
  vdp_delsprite.o	\
  vdp_gchar.o		\
  vdp_gplvdp.o		\
  vdp_hchar.o		\
  vdp_hexprint.o	\
  vdp_fasthexprint.o\
  vdp_fasterhexprint.o\
  vdp_byte2hex.o	\
  vdp_ints.o		\
  vdp_memcpy.o		\
  vdp_memread.o		\
  vdp_memset.o		\
  vdp_printf.o    \
  vdp_putchar.o   \
  vdp_putstring.o	\
  vdp_readchar.o	\
  vdp_screenchar.o	\
  vdp_scrnscroll.o	\
  vdp_setbitmap.o	\
  vdp_setgraphics.o	\
  vdp_setmode.o		\
  vdp_setmulticolor.o	\
  vdp_settext.o		\
  vdp_sprite.o		\
  vdp_textdefs.o	\
  vdp_vchar.o		\
  vdp_waitvint.o	\
  vdp_writeinc.o	\
  vdp_writescreeninc.o	\
  vdp_writestring.o

# Recipe to compile the library
all: library test

library: $(OBJECT_LIST)
	$(AR) rcs $(NAME) $(OBJECT_LIST)

test: library testlib.o $(OBJECT_LIST_EA5) 
	$(LD) $(OBJECT_LIST_EA5) testlib.o $(LDFLAGS_EA5) -L. -lti99 -o testlib.ea5.elf > ea5.map
	$(ELF2EA5) testlib.ea5.elf testlib.ea5.bin
	$(EA5PLIT) testlib.ea5.bin
	$(CP) TESTLI* /cygdrive/c/classic99/dsk1/

# Recipe to clean all compiled objects
.phony clean:
	-rm *.o
	-rm *.a
	-rm *.s
	-rm *.i
	-rm *.elf
	-rm *.map
	-rm *.bin
	-rm TESTLI*

# Recipe to compile all assembly files
%.o: %.asm
	$(GAS) $< -o $@

# Recipe to compile all C files
%.o: %.c
	$(CC) -c $< $(C_FLAGS) -o $@
