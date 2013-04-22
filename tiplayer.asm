# decompress a compressed vgm file
# there are 12 streams that we decompress overall
# for each channel there is a time stream,  a volume stream,  and a tone stream

# uses 84 bytes of RAM plus 32 bytes for a temporary workspace (114 total)
# 322 bytes of code
# uses about 30% (high estimate) of vblank to play, depending on tempo and density of the song

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

# pointers,  in order streampos,  streamref,  streamcnt,  repeated 12 times
		even
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
	mov *r3, r1					# get stream pointer
	movb *r1+, r0				# get byte from current run
	mov r1, *r3					# write new value back
	dec @4(r3)					# count down - no need to test here
	b *r11						# and return

getb3
	mov *r3, r1					# get pointer to stream
	clr r2						# prepare r2
	movb *r1+, r2				# get new count byte
	jgt getb4					# if high bit is clear
	andi r2, >7f00				# mask it
	swpb r2						# store in the lsbyte
	dec r2						# we are going to consume one byte below,  and we know it's at least 4
	mov r2, @4(r3)				# write it back
	movb *r1+, r2				# get backref pointer (can't use mov,  might be misaligned)
	swpb r2
	movb *r1+, r2
	swpb r2						# get back into correct order
	a @songad, r2				# make into a pointer
	movb *r2+, r0				# get back-referenced byte
	mov r2, @2(r3)				# and store back reference pointer
	mov r1, *r3					# and save stream pointer
	b *r11						# and return

getb4
	swpb r2						# fix byte order (assumes no corruption,  lsbyte is already zero)
	movb *r1+, r0				# get byte from current run
	dec r2						# count down - no need to test here
	mov r2, @4(r3)				# save count
	mov r1, *r3					# save pointer
	b *r11						# and return

# start a new tune,  with the pointer to the module in r1
stinit
	mov r1,@songwp		# save the parameter in our workspace's R0
	lwpi songwp
	li r1, 12
	li r2, strm
	mov r0, @songad		# save it for later
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
	mov @48(r5), r0		# test time stream pointer (stream 8,  6 bytes per stream,  6#8)
	jeq stpl2			# skip if empty

	seto r7				# found valid data,  flag for caller

	mov *r6, r0			# check countdown timer
	jeq stplx1			# process time
	dec *r6				# decrement timer
	jmp stpl2			# was not zero,  next loop

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
	bl @getbyte			# get it

	socb @tonemk(r4), r0	# or in the sound command nibble
	movb r0, @>8400		# move to the sound chip
	swpb r0				# swap data so we can save it off

	ci r4, 3			# check for noise channel
	jeq stpl4a

	bl @getbyte			# two bytes for tone channels
	movb r0, @>8400		# move to the sound chip

stpl4a
	sla r4,1			# make an index
#	swpb r0				# make it easier on the user (not bothering, it's mangled anyway, they can sort it if they care)
	mov r0,@songwp+24(r4)	# save it (r12->r15)
	srl r4,1			# change it back

stpl4
	coc @dat40, r8		# check for volume
	jne stpl5

	mov r5, r3
	ai r3, 24			# 4 streams up,  4#6
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
	ai r5, 6				# next stream struct
	inct r6				# next timer
	inc r4				# next loop
	ci r4, 4				# are we done?
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
