# 60hz music and sound effects player with sfx priority
# 2014 by Tursi aka Mike Brent
# Released to public domain, may be used freely

# uses 252 bytes of RAM plus 32 bytes for a temporary workspace (284 total)
# 968 bytes of code

# following notes are for /each track/ - so are doubled in worst case.
# cycle counting an average song gives a range of about 1000-10000 cycles per frame, with an
# average of 2000 cycles. That's 333uS - 3333uS, average of 665uS. One scanline (out of 262)
# is about 63.666uS, so the decompression takes from 5-52 scanlines, average of 10 scanlines.
# That means about 2%-20% of the CPU, with an average of 4%, at 60Hz playback.

# externally visible data (on return):
# this data applies to music only - SFX data is not retained for external viewing.
# R7 (songwp+14) contains >0000 if nothing is playing. MSB is channels for music, LSB is channels for sfx.
# R9-R10 (songwp+18-20) contains one byte for each voice's current volume
# R12-R15 (songwp+24-30) contain the current frequency word (last channel is just a noise nibble)
# Note: for this player, you must not modify R9-R10, or R12-R15, as they are used to restore
# music when a sound effect ends!

# R0 = return data, scratch       R8 = scratch
# R1 = scratch                    R9 = user volume bytes 0-1
# R2 = scratch                    R10= user volume bytes 2-3
# R3 = stream base for fctn       R11= subroutine return
# R4 = voice counter (0-3)        R12= voice 0 frequency
# R5 = stream base pointer        R13= voice 1 frequency
# R6 = time counter pointer       R14= voice 2 frequency
# R7 = data ptr/play flags		  R15= noise type (byte)

# this one ported to work with the gcc assembler,  and export data
	def stinitsfx
	def ststopsfx
	def stplaysfx
	def sfxinitsfx
	def sfxstopsfx
	def allstopsfx

# these are just intended to be read from the map file so they can be used for timing
	def timinginsfx
	def timingoutsfx

# helpful for finding the variable for volume attenuation
	def atten

# must point to a workspace that the player can corrupt at will, 
# however,  the memory is not needed between calls
# C runtime uses >8300, and >8320 is used to store 0s for my own hack
songwp equ >8322

	dseg

		even
# pointers,  in order streampos,  streamref,  streamcnt, streambase, repeated 12 times (for decompression)
strm	bss 96
# time countdown for each of 4 channels (bytes)
tmcnt	bss 4
# count of override for timestreams (bytes)
tmocnt	bss 4
# type of override for timestreams (bytes)
tmovr	bss 4
# pointer to the song data (needed for offset fixups)
songad	bss 2
# pointer to the frequency table (used for speedup)
freqad	bss 2

# pointers,  in order streampos,  streamref,  streamcnt, streambase, repeated 12 times (for decompression)
sfxstrm		bss 96
# time countdown for each of 4 channels (bytes)
sfxtmcnt	bss 4
# count of override for timestreams (bytes)
sfxtmocnt	bss 4
# type of override for timestreams (bytes)
sfxtmovr	bss 4
# pointer to the song data (needed for offset fixups)
sfxsongad	bss 2
# pointer to the frequency table (used for speedup)
sfxfreqad	bss 2

# global attenuation (in MSB, 0 (no attenuation), f (max attenuation))
# defaults to 0 on every stinit!
atten bss 2
# return addresses
retad	bss 2
retad2	bss 2
# Bitmask storage for R7 in (LSB) and out (MSB)
# copied in and out via R7
playmask bss 2
# sfx flag - contains current priority (0 means not playing)
sfxflag bss 2


	pseg 
# get a compressed byte from a stream - stream data base in r3
# byte is return in r0 msb
# uses r1, r2
	even
getbyte
	mov @2(r3), r1				# test streamref
	jeq getb1					# not set
	ci r1,0xffff				# test for repeat count
	jeq getb1noinc					# not backref
	movb *r1+, r0				# get back-referenced byte
	mov r1, @2(r3)				# write new value back
	dec @4(r3)					# decrement counter
	jne getb2					# not 0 yet
	clr @2(r3)					# out of bytes,  clear back reference ptr
getb2
	b *r11						# and return
 
getb1
# get byte with increment - need to pre-test for 0, so the jnc
	dec @4(r3)					# count down
	jnc getb3						# we went negative, so it must have been 0

getb1inc
	mov *r3, r1					# get stream pointer
	movb *r1+, r0				# get byte from current run
	mov r1, *r3					# write new value back
	b *r11						# and return

# get byte with no increment - need to pre-test for 0, so the jnc
getb1noinc
	dec @4(r3)				# count down
	jnc getb3					# it went negative, so must have been 0
	jeq getb1inc			# increment always on last byte
	
	mov *r3, r1				# get stream pointer
	movb *r1, r0			# get byte from current run
	b *r11						# and return

nostream
	movb r2,r0					# return (r2 is expected to be zero!)
	b *R11

getb3
# count is zero, need to get a new command
	mov *r3, r1					# get pointer to stream
	clr r2						# prepare r2
	movb *r1+, r2				# get new count byte
	jeq nostream
	jgt getb4					# if high bit is clear (no 0x80)

	coc @dat40,r2				# check for 40 (indicates 2 byte reference)
	jeq getb3double

# get single byte back reference
	andi r2, 0x3f00				# mask it
	swpb r2						# store in the lsbyte
	dec r2						# we are going to consume one byte below,  and we know it's at least 4
	mov r2, @4(r3)				# write it to count
	clr r2						# prepare msb
	movb *r1+, r2				# get backref pointer
	swpb r2						# put in correct order
	a @6(r3),r2					# add stream base, r2 now has the correct address
	jmp getb3fin

getb3double
	andi r2, 0x3f00				# mask it
	swpb r2								# store in the lsbyte
	dec r2								# we are going to consume one byte below,  and we know it's at least 4
	mov r2, @4(r3)				# write it back
	movb *r1+, r2					# the absolute address saves 2 swpb's for 2 bytes code and 8 cycles
	movb *r1+, @songwp+5	# get backref pointer (can't use mov,  might be misaligned, r2 LSB)

	a @songad, r2				# make into a pointer

getb3fin
	movb *r2+, r0				# get back-referenced byte
	mov r2, @2(r3)				# and store back reference pointer
	mov r1, *r3					# and save stream pointer
	b *r11						# and return

getb4
# 0x80 is not set, check for just 0x40
	coc @dat40, r2
	jeq getb5					# it's set, so go process that

# neither bit is set, so it's just a plain run
	swpb r2						# fix byte order (assumes no corruption,  lsbyte is already zero)
	movb *r1+, r0				# get byte from current run
	dec r2						# count down - no need to test here
	mov r2, @4(r3)				# save count
	mov r1, *r3					# save pointer
	clr @2(r3)				# make sure the streamref is zeroed (needed for the 0xffff case)
	b *r11						# and return

getb5
# 0x40 is set - set up for a repeated character
	andi r2, 0x3f00				# mask it
	swpb r2						# put in the correct byte
	dec r2						# count down the one we are going to take
	mov r2,@4(r3)				# save the result
	movb *r1, r0				# get the appropriate byte - note no increment!
	mov r1,*r3				# save it (necessary because we incremented way above)
	seto @2(r3)					# set the reference to 0xffff
	b *r11						# and return

# start a new sound effect,  with the pointer to the module in r1, and index of tune in r2
sfxinitsfx
	c r3,@sfxflag		# check whether a higher priority sfx is already playing
	jhe playsfx
	b *r11				# already higher

playsfx
	lwpi songwp

	mov @sfxflag,r0
	jeq sfxinit2
	bl @restorechans	# we were already playing, so we must restore the channels
sfxinit2
	mov @>8306,@sfxflag	# save off the priority (r3)
	mov @>8302,r0		# save the address (r1) in our workspace's R0
	mov @>8304,r3		# save the index (r2) in our workspace's R3

	mov r0, @sfxsongad	# save it for later
	inct r0						# point to the frequency table
	mov *r0,r1				# get the pointer
	dect r0						# back to the base
	a r0,r1						# make a real pointer
	mov r1,@sfxfreqad		# and remember it
	
	li r1, 12
	li r2, sfxstrm
	mov *r0, r0			# point to the table of pointers
	li r4,24			# 24 bytes per table
	mpy r3,r4			# get the offset to the requested stream table (into r4,r5)
	a r5,r0				# add it in
	a @sfxsongad, r0	# make a memory pointer
	mov r7,@retad		# save music version of r7
	li r7,sfxstrm-strm	# base offset
	jmp sti1

# start a new tune,  with the pointer to the module in r1, and index of tune in r2 (usually 0)
stinitsfx
	mov r1,@songwp		# save the address in our workspace's R0
	mov r2,@songwp+6	# save the index in our workspace's R3
	lwpi songwp
	mov r0, @songad		# save it for later
	inct r0						# point to the frequency table
	mov *r0,r1				# get the pointer
	dect r0						# back to the base
	a r0,r1						# make a real pointer
	mov r1,@freqad		# and remember it

	li r1, 12
	li r2, strm
	mov *r0, r0			# point to the table of pointers
	li r4,24			# 24 bytes per table
	mpy r3,r4			# get the offset to the requested stream table (into r4,r5)
	a r5,r0				# add it in
	a @songad, r0		# make a memory pointer
	clr r7				# offset
sti1
	mov *r0+, *r2		# get stream offset 
	a @songad(r7), *r2	# make it a pointer - when all four voices point to zero,  the tune is over
	mov *r2,@6(r2)		# copy into stream base pointer
	inct r2
	clr *r2+			# clear reference
	clr *r2+			# clear count
	inct r2				# skip stream base pointer
	dec r1
	jne sti1

	clr *r2+			# clear four byte time counters
	clr *r2+

	clr *r2+			# clear four byte timer override counters
	clr *r2+

	# put sanish values in the user feedback registers (not for sfx)
	mov r7,r7			# music will be zeroed
	jeq clrdata
	mov @retad,r7		# get song version of r7 back
	jmp sfxsane

clrdata
	seto r7				# playing flag
	mov @volmk,r9		# volume bytes - default to mute! (>90B0)
	mov @volmk+2,r10	# so we never need to check them  (>D0F0)
	li r2,>0F0F			# attenuation of >0F on each
	soc r2,r9
	soc r2,r10
	clr r12				# tone words
	clr r13
	clr r14
	clr r15

sfxsane
	clr @atten			# maximum volume
	lwpi >8300			# c workspace
	b *r11					# back to caller

# call to stop the tune or initialize to silence
# uses r0, r1
ststopsfx
	lwpi songwp

	li r0, strm
sts2
	li r1, 52			# 12*4 + 4
sts1
	clr *r0+			# get stream offset 
	dec r1
	jne sts1

	mov @volmk,r9		# volume bytes - default to mute! (>90B0)
	mov @volmk+2,r10	# so we never need to check them  (>D0F0)
	li r1,>0F0F			# attenuation of >0F on each
	soc r1,r9
	soc r1,r10

	clr r7				# clear playing flag
	lwpi >8300			# c workspace
	b *r11				# back to caller

# call to stop the sfx or initialize to silence
# uses r0, r1
sfxstopsfx
	lwpi songwp
	mov @sfxflag,r0
	jeq norest
	bl @restorechans
	clr @sfxflag
norest
	li r0, sfxstrm
	jmp sts2

# call to stop both SFX and music
allstopsfx
	mov r11,@retad
	bl @sfxstopsfx
	bl @ststopsfx
	mov @retad,r11
	b *r11

dat80	data >8000
dat40	data >4000
# dat01 is just a label now, we can use the first byte of bits
dat01
bits	data >0102, >0408
bits16	data >0001, >0002, >0004, >0008
tonemk	data >80a0, >c0e0
volmk	data >90b0, >d0f0
specdt	data >4142, >4300

# tiny helper for sfx restore
loadtone
	swpb r0
	movb r0,@>8400
	swpb r0
	movb r0,@>8400
	b *r11

restorechans
	mov r11,r2
	li r1,loadtone		# sfx ended - restore all the channels
	mov r12,r0
	bl *r1
	mov r13,r0
	bl *r1
	mov r14,r0
	bl *r1
	mov r15,r0
	swpb r0
	movb r0,@>8400
	movb r9,@>8400
	swpb r9
	movb r9,@>8400
	swpb r9
	movb r10,@>8400
	swpb r10
	movb r10,@>8400
	swpb r10
	b *r2

# call every vblank to update the music
# intended to be called from vblank hook - returns with
# the workspace changed to songwp
stplaysfx
timinginsfx
## temp hack - measuring time ##
#	li r0, >0487
#	movb r0, @>8c02
#	swpb r0
#	movb r0, @>8c02
#################################

	mov r11, @retad2	# master return address
	lwpi songwp			# get 'our' workspace

# process sound effects
	clr @playmask		# clear the channel masking data (MSB = in, LSB = out)

	li r7,sfxstrm-strm	# offset for sound effects
	bl @playone			# do it

	mov @playmask,r0
	swpb r0
	mov r0,@playmask	# prepare playmask for music
	jne runpart2		# jump if SFX still playing
	mov @sfxflag,r0
	jeq runpart2		# don't reset if it wasn't playing

	bl @restorechans	# sfx ended - restore all the channels

	clr @sfxflag

runpart2
# process music
	clr r7				# offset for music
	bl @playone

	mov @playmask,r7	# copy output flags to R7 (MSB music, LSB sfx)

	mov @retad2,r11		# get return adress back

## temp hack - measuring time ##
#	li r0, >0287
#	movb r0, @>8c02
#	swpb r0
#	movb r0, @>8c02
#################################

timingoutsfx	
	b *r11				# back to caller

bstpl2
	b @stpl2

playone
	mov r11,@retad		# save return address

	li r4, 3					# counter for 4 voices
	li r5, strm+24		# pointing to last stream object
	li r6, tmcnt+3		# pointing to last time counter

	a r7,r5				# add offset
	a r7,r6				# add offset

stpl1
	mov @64(r5), r0		# test time stream pointer (stream 8,  8 bytes per stream,  8*8)
	jeq bstpl2			# skip if empty

	socb @bits(r4),@playmask	# set the active bit

	sb @dat01,*r6		# decrement timer
	joc bstpl2			# was not zero,  next loop (this will catch 0 -> -1, as desired)

stplx1
	mov r5, r3
	ai r3, 64			# pointer to time stream (stream 8)

	movb @4(r6),r0	# tmocnt
	jeq stplx2			# no override active

	sb @dat01,@4(r6) # tmocnt (count down)
	movb @8(r6),r8	# tmovr - get the override byte	
	jmp postld			# jump ahead to process

stplx2
	bl @getbyte			# get a compressed byte
	movb r0, r0			# test the result
	jne stpl3			# was not zero

	clr *r3				# zero the timestream pointer
	szcb @bits(r4),@playmask	# clear the active bit
	jmp stpl2			# next loop

stpl3
	clr r8
	movb r0, r8			# save the command

	ci r8,>7a00			# test for special range
	jl stlp3b
	jeq stshrt
	ci r8,>7f00
	jh stlp3b

	ci r8,>7d00
	jl stborc

	ai r8,->7c00
	movb r8,@4(r6)		# tmocnt
	
	movb @specdt,r8		# was 0x7d,0x7e,0x7f
	movb r8,@8(r6)		# tmovr
	jmp postld
	
stborc
	ai r8,->7a00
	movb r8,@4(r6)		# tmocnt
	
	movb @specdt+1,r8	# was 0x7b or 0x7c
	movb r8,@8(r6)		# tmovr
	jmp postld

stshrt
	movb @dat01,@4(r6)	# tmocnt
	movb @specdt+2,r8		# was a 0x7a
	movb r8,@8(r6)			# tmovr

postld
# r8 now has tmovr

stlp3b
	coc @dat80, r8		# check for tone
	jne stpl4			# not set,  skip

	mov r5, r3			# base stream is tones
	clr r0				# prepare for tone index (not needed if noise)
	bl @getbyte			# get it

	ci r4, 3			# check for noise channel
	jne sttone

#noise channel
	ori r0,>e000			# or in the sound command nibble (we know we are on channel 3, save some code+time)
	mov r4,r1			# need this to check
	sla r1,1			# make index
	mov @playmask,r2	# get mask
	coc @bits16(r1),r2	#check if we are allowed to play
	jeq nonoise
	movb r0, @>8400		# move to the sound chip
nonoise
	swpb r0				# swap data so we can save it off
	jmp stpl4a

sttone
	swpb r0				# get into correct byte
	sla r0,1			# make index
	a @freqad(r7),r0	# make pointer
	mov *r0, r0			# get the frequency data
	socb @tonemk(r4), r0	# or in the sound command nibble
	mov r4,r1			# need this to check
	sla r1,1			# make index
	mov @playmask,r2	# get mask
	coc @bits16(r1),r2	#check if we are allowed to play
	jeq notone
	movb r0, @>8400		# move to the sound chip
	swpb r0				# swap data so we can save it off
	movb r0, @>8400		# move the second byte

stpl4a
	mov r7,r7			# check if on music track
	jne stpl4			# don't save it
	sla r4,1			# make an index
	mov r0,@songwp+24(r4)	# save it (r12->r15)
	srl r4,1			# change it back

stpl4
	coc @dat40, r8		# check for volume
	jne stpl5

	mov r5, r3
	ai r3, 32			# 4 streams up,  4*8
	bl @getbyte		# get it
  ab @atten,r0  # add global volume attenuation
	ci r0,>1000   # can we get around this?       
  jl stvl       # it's okay
	li r0,>0f00   # clamp to silence                                    
stvl
	socb @volmk(r4), r0	# or in the sound command nibble
	mov r4,r1			# need this to check
	sla r1,1			# make index
	mov @playmask,r2	# get mask
	coc @bits16(r1),r2	#check if we are allowed to play
	jeq novol
	movb r0, @>8400		# move to the sound chip
novol
	mov r7,r7			# check if on music track
	jne stpl5			# don't save it
	movb r0, @songwp+18(r4)	# save it off (r9->r10)

stpl5
	andi r8, >3f00		# mask off the count
	sb @dat01,r8			# decement for this tick
	movb r8, *r6			# save it off

stpl2
	ai r5, -8					# next stream struct
	dec r6						# next timer
	dec r4						# next loop
	jnc gohome				# not done yet
	b @stpl1			# not yet

gohome

	mov @retad, r11		# get return address back

	b *r11				# now done 1 tick

# moved down here so that playing tones doesn't get more expensive
notone
	swpb r0				# so the user feedback looks the same
	jmp stpl4a			# back to mainline


	end
