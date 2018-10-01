# Paths to TMS9900 compilation tools 
# ( Set in environment to override paths )
TMS9900_DIR?=/home/tursilion/gcc9900/bin
ELF2EA5_DIR?=/home/tursilion/gcc9900/bin
EA5_SPLIT_DIR?=/home/tursilion/gcc9900/bin
CLASSIC99_DSK1?=/mnt/d/classic99/DSK1/

# Full paths to the executables used
GAS=$(TMS9900_DIR)/tms9900-as
LD=$(TMS9900_DIR)/tms9900-ld
CC=$(TMS9900_DIR)/tms9900-gcc
AR=$(TMS9900_DIR)/tms9900-ar
ELF2EA5=$(ELF2EA5_DIR)/elf2ea5
EA5_SPLIT=$(EA5_SPLIT_DIR)/ea5split

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
  conio_bgcolor.o   \
  conio_cclear.o    \
  conio_cclearxy.o  \
  conio_cgetc.o     \
  conio_chline.o    \
  conio_chlinexy.o  \
  conio_clrscr.o    \
  conio_cprintf.o   \
  conio_cputc.o     \
  conio_cputcxy.o   \
  conio_cputs.o     \
  conio_cputsxy.o   \
  conio_cvline.o    \
  conio_cvlinexy.o  \
  conio_getvram.o   \
  conio_kbhit.o     \
  conio_screensize.o\
  dsr_files.o		\
  dsr_dsrlnk.o		\
  dsr_dsrlnkraw.o	\
  grom_readdata.o	\
  grom_writedata.o	\
  kscan.o			\
  kscanfast.o		\
  joystfast.o		\
  math_sqrt.o   \
  player.o			\
  puff.o        \
  puff16k.o     \
  rs232_geterrs.o   \
  rs232_poll.o  \
  rs232_readbyte.o \
  rs232_readPIO.o \
  rs232_setbps.o \
  rs232_setcontrol.o \
  rs232_writebyte.o \
  rs232_writePIO.o \
  rs232raw_activateCard.o \
  rs232raw_checkstatus.o \
  rs232raw_deactivateCard.o \
  rs232raw_geterrs.o \
  rs232raw_getuart.o \
  rs232raw_poll.o \
  rs232raw_readbyte.o \
  rs232raw_setPIOinput.o \
  rs232raw_setPIOoutput.o \
  rs232raw_writebyte.o \
  stcount.o			\
  str_atoi.o      \
  str_gets.o      \
  str_int2str.o   \
  str_memcpy.o    \
  str_strcpy.o    \
  str_strlen.o		\
  str_uint2hex.o  \
  str_uint2str.o  \
  sys_exit.o      \
  sys_halt.o			\
  tiplayer.o		\
  tiplayer30hz.o	\
  tiplayersfx.o		\
  tiplayersfx30hz.o	\
  vdp_bigbuf.o          \
  vdp_bmclearpixel.o \
  vdp_bmclearscreen.o \
  vdp_bmcolor.o \
  vdp_bmconsolefont.o \
  vdp_bmdrawline.o \
  vdp_bmputs.o \
  vdp_bmsetpixel.o \
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
  vdp_fastscrnscroll.o	\
  vdp_fastscrnscroll_default.o	\
  vdp_setbitmap.o	\
  vdp_setgraphics.o	\
  vdp_setmode.o		\
  vdp_setmulticolor.o	\
  vdp_settext.o		\
  vdp_settext64.o		\
  vdp_settext80.o		\
  vdp_settext80color.o		\
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
	$(EA5_SPLIT) testlib.ea5.bin
	cp TESTLI* $(CLASSIC99_DSK1)

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
