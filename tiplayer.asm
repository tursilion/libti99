# decompress a compressed vgm file
# there are 12 streams that we decompress overall
# for each channel there is a time stream,  a volume stream,  and a tone stream

# uses 84 bytes of RAM plus 32 bytes for a temporary workspace (114 total)
# 470 bytes of code
# uses about 35% (high estimate) of vblank to play, depending on tempo and density of the song

# externally visible data:
# R7 (songwp+14) contains >FFFF if the song is still playing, and >0000 if it's done
# R9-R10 (songwp+18-20) contains one byte for each voice's current volume
# R12-R15 (songwp+24-30) contain the current frequency word (last channel is just a noise nibble)

# this one ported to work with the gcc assembler,  and export data
	def stinit
	def ststop
	def stplay

# must point to a workspace that the player can corrupt at will, 
# however,  the memory is not needed between calls
# C runtime uses >8300 for RT, and >8320 is used to store 0s for it in my own hack
songwp equ >8322

# screen timeout register - we reset this every frame we run
# we stop resetting it when the song ends
scrnto equ >83D6

	dseg

		even
# pointers,  in order streampos,  streamref,  streamcnt,  repeated 12 times (for decompression)
strm	bss 72
# time countdown for each of 4 channels (only need bytes,  but using words for simplicity)
tmcnt	bss 8
# pointer to the song data (needed for offset fixups)
songad	bss 2
# return address
retad	bss 2

	pseg 
# get a compressed byte from a stream - stream base in r3
# byte is return in r0 msb
# uses r1, r2
	even
getbyte
	mov @2(r3), r1				# test streamref
	jeq getb1					# not set
	ci r1,0xffff				# test for repeat count
	jeq getb1					# not backref
	movb *r1+, r0				# get back-referenced byte
	mov r1, @2(r3)				# write new value back
	dec @4(r3)					# decrement counter
	jne getb2					# not 0 yet
	clr @2(r3)					# out of bytes,  clear back reference ptr
getb2
	b *r11						# and return
 
getb1
	mov @4(r3), r1				# test streamcnt
	jeq getb3					# out of bytes,  need to figure out the next set

	dec @4(r3)					# count down
	jeq getb1inc				# increment always if last byte
	mov @2(r3), r1				# test streamref is 0
	jeq getb1inc				# increment if not a ref (must be 0xffff here)

# get byte with no increment
	mov *r3, r1					# get stream pointer
	movb *r1, r0				# get byte from current run
	b *r11						# and return

getb1inc
# get byte with increment
	mov *r3, r1					# get stream pointer
	movb *r1+, r0				# get byte from current run
	mov r1, *r3					# write new value back
	b *r11						# and return

getb3
# count is zero, need to get a new command
	mov *r3, r1					# get pointer to stream
	clr r2						# prepare r2
	movb *r1+, r2				# get new count byte
	jgt getb4					# if high bit is clear (no 0x80)

	coc @dat40,r2				# check for 40 (indicates 2 byte reference)
	jeq getb3double

#get single byte back reference - some duplicated code here to save registers
	andi r2, 0x3f00				# mask it
	swpb r2						# store in the lsbyte
	dec r2						# we are going to consume one byte below,  and we know it's at least 4
	mov r2, @4(r3)				# write it back
	clr r2						# prepare msb
	movb *r1+, r2				# get backref pointer (can't use mov,  might be misaligned)
	swpb r2						# put in correct order
	jmp getb3fin

getb3double
	andi r2, 0x3f00				# mask it
	swpb r2						# store in the lsbyte
	dec r2						# we are going to consume one byte below,  and we know it's at least 4
	mov r2, @4(r3)				# write it back
	movb *r1+, r2				# get backref pointer (can't use mov,  might be misaligned)
	swpb r2
	movb *r1+, r2
	swpb r2						# get back into correct order

getb3fin
	a @songad, r2				# make into a pointer
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
	clr @2(r3)					# make sure the streamref is zeroed
	b *r11						# and return

getb5
# 0x40 is set - set up for a repeated character
	andi r2, 0x3f00				# mask it
	swpb r2						# put in the correct byte
	dec r2						# count down the one we are going to take
	mov r2,@4(r3)				# save the result
	movb *r1, r0				# get the appropriate byte - note no increment!
	mov r1,*r3					# save it (necessary because we incremented above)
	seto @2(r3)					# set the reference to 0xffff
	b *r11						# and return

# start a new tune,  with the pointer to the module in r1
stinit
	mov r1,@songwp		# save the parameter in our workspace's R0
	lwpi songwp
	li r1, 12
	li r2, strm
	mov r0, @songad		# save it for later
	mov *r0, r0			# point to the table of pointers
	a @songad, r0		# make a memory pointer
sti1
	mov *r0+, *r2		# get stream offset 
	a @songad, *r2+		# make it a pointer - when all four voices point to zero,  the tune is over
	clr *r2+			# clear reference
	clr *r2+			# clear count
	dec r1
	jne sti1

	clr *r2+			# clear four time counters
	clr *r2+
	clr *r2+
	clr *r2+		

	# put sanish values in the user feedback registers
	seto r7				# playing flag
	seto r9				# volume bytes
	seto r10
	clr r12				# tone words
	clr r13
	clr r14
	clr r15

	lwpi >8300			# c workspace
	b *r11				# back to caller

# call to stop the tune or initialize to silence
# uses r0, r1
ststop
	lwpi songwp

	li r1, 40			# 12#3 + 4
	li r0, strm
sts1
	clr *r0+			# get stream offset 
	dec r1
	jne sts1

	lwpi >8300			# c workspace
	b *r11				# back to caller


dat80	data >8000
dat40	data >4000
tonemk	data >80a0, >c0e0
volmk	data >90b0, >d0f0

# call every vblank to update the music
# intended to be called from vblank hook - returns with
# the workspace changed to songwp
stplay

## temp hack - measuring time ##
#	li r0, >0487
#	movb r0, @>8c02
#	swpb r0
#	movb r0, @>8c02
#################################

	mov r11, @retad		# save return address
	lwpi songwp			# get 'our' workspace

	seto @scrnto		# reset the screen timeout

	clr r7				# flag for caller - if 0,  the song is over (songwp+14)
	
	clr r4				# counter for 4 voices
	li r5, strm			# pointing to first stream object
	li r6, tmcnt		# pointing to first time counter

stpl1
	mov @48(r5), r0		# test time stream pointer (stream 8,  6 bytes per stream,  6*8)
	jeq stpl2			# skip if empty

	seto r7				# found valid data,  flag for caller

	dec *r6				# decrement timer
	joc stpl2			# was not zero,  next loop (this will catch 0 -> -1, as desired)
#	mov *r6, r0			# check for zero (should be the same effect as the two lines above?)
#	jeq stplx1			# yes, process
#	dec *r6				# no, count down
#	jmp stpl2			# and next channel

stplx1
	mov r5, r3
	ai r3, 48			# pointer to time stream (stream 8)
	bl @getbyte			# get a compressed byte
	movb r0, r0			# test the result
	jne stpl3			# was not zero

	clr *r3				# zero the timestream pointer
	jmp stpl2			# next loop

stpl3
	movb r0, r8			# save the command
	coc @dat80, r8		# check for tone
	jne stpl4			# not set,  skip

	mov r5, r3			# base stream is tones
	clr r0				# prepare for tone index (not needed if noise)
	bl @getbyte			# get it

	ci r4, 3			# check for noise channel
	jne sttone

#noise channel
#	socb @tonemk(r4), r0	# or in the sound command nibble
	socb @tonemk+3, r0	# or in the sound command nibble (we know we are on channel 3, save some code+time)
	movb r0, @>8400		# move to the sound chip
	swpb r0				# swap data so we can save it off
	jmp stpl4a

sttone
	swpb r0				# get into correct byte
	sla r0,1			# make index
	mov @songad,r1		# we need this address twice
	a r1,r0				# make pointer
	a *r1,r0			# and add the offset to the pointer table
	ai r0,24			# skip the table to the frequency table
	mov *r0, r0			# get the frequency data
	socb @tonemk(r4), r0	# or in the sound command nibble
	movb r0, @>8400		# move to the sound chip
	swpb r0				# swap data so we can save it off
	movb r0, @>8400		# move the second byte

stpl4a
	sla r4,1			# make an index
	mov r0,@songwp+24(r4)	# save it (r12->r15)
	srl r4,1			# change it back

stpl4
	coc @dat40, r8		# check for volume
	jne stpl5

	mov r5, r3
	ai r3, 24			# 4 streams up,  4*6
	bl @getbyte			# get it
	socb @volmk(r4), r0	# or in the sound command nibble
	movb r0, @>8400		# move to the sound chip
	movb r0, @songwp+18(r4)	# save it off (r9->r10)

stpl5
	andi r8, >3f00		# mask off the count
	swpb r8				# make int
	dec r8				# decement for this tick
	mov r8, *r6			# save it off

stpl2
	ai r5, 6			# next stream struct
	inct r6				# next timer
	inc r4				# next loop
	ci r4, 4			# are we done?
	jne stpl1			# not yet

## temp hack - measuring time ##
#	li r0, >0287
#	movb r0, @>8c02
#	swpb r0
#	movb r0, @>8c02
#################################

	mov @retad, r11		# get return address back
	b *r11				# now done 1 tick

	end
