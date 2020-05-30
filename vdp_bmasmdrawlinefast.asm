# line draw function for bitmap mode
# Requires color table at >2000 and Pattern table at >0000
# draws about 32 lines per second color, faster in mono
# variable "$dcol" contains the color values (foreground/background)
# variable "$dmode" contains 0 to set, 1 to unset, and 2 to XOR

# This uses a fixed workspace of >8320 to reduce register stress on the c compiler
# Note this may conflict with other tasks. It does not need the data between calls.
#    mov   y2,@>8324   # y2 -> r2
#    mov   x2,@>8326   # x2 -> r3
#    mov   y1,@>8328   # y1 -> r4
#    mov   x1,@>832a   # x1 -> r5
#    mov MODE,@>833a   # mode -> R13
#    movb COLR,@>833c  # color -> r14 (MSB)
#    bl @bm_asmdrawlinefast

	def bm_asmdrawlinefast

vdpwa equ >8c02
vdprd equ >8800
vdpwd equ >8c00

# register usage---
#       r0:  scratch, in loop the slope calc
#       r1:  scratch
#       r2:  y2, then height
#       r3:  x2, then width
#       r4:  y1, x code for post-shift branch
#       r5:  x1, the current bit mask
#       r6:  current address (in pattern table @ >0000)
#       r7:  x code for plotting (soc, szc, or xor)
#       r8:  VDPWA
#	   r11:  x code for bitmask shift (sla or srl)
#      r12:  countdown for draw loop
# 	   r13:  draw mode ($dmode)
#      r14:  draw color ($dcol)

# also: @$dmode: 0=draw, 1=erase, 2=xor
#       @$dcol:  bitmap color to write (regardless of dmode)

# color - >0000    pattern - >2000


# these codes are for x, using r5 into r1
xorcode	equ	>2845
soccode	equ >e045
szccode	equ >4045

# these codes are for x, using r5. 
slacode	equ >0a15
srccode	equ >0b15

# these codes are for x, to jump varying words ahead (when wrap occurs)
jeqcode	equ >1304		# used for negative step
jltcode	equ >1101		# used for positive step

# these are bitmasks for r5 (only used to initialize)
bits	data >8080,>4040,>2020,>1010,>0808,>0404,>0202,>0101

# compare word for testing when the row exceeds a character
rowmask	data >0007

# out of registers, so, this table helps us with x character boundaries
xcharstep data >0008,>fff8

# draw a bitmap mode line
bm_asmdrawlinefast
	LWPI >8320
    
# save VDPWA to register
	LI r8,vdpwa
    
# work out the raster operation from $dmode (the original method was cute! using that.)
# we do it once, at the beginning, to speed the inner draw loop. x adds 8 cycles but that
# is faster than a single jump.
	mov r13,r0
	dec r0
	jeq dundr		# undraw
	jlt drw			# draw
	li r7,xorcode	# it'll be xor
	jmp sortlin
dundr
	li r7,szccode	# 'un'draw
	jmp sortlin
drw
	li r7,soccode	# draw

# now figure out if we're drawing a steep or shallow line
sortlin
	mov r5,r0		# x1
	s r3,r0			# x2
	abs r0			# width
	mov r4,r1		# y1
	s r2,r1			# y2
	abs r1			# height
	c r0,r1			# which is larger?
	jl steep		# steep line
	
# shallow (up to 45 degrees)

# check whether x is incrementing or decrementing, remember result in r11 (no bl!)
	clr r11			# just a flag for incrementing
	c r5,r3
	jle noflip
	seto r11		# flag decrementing
noflip
	
# sort it so y increments (this lets us do address assumptions later)
# note we never use x2,y2 (r3,r2) again, so... we don't actually need to preserve them here.
# less a swap than a replacement then.
	c 	r4,r2		# y1,y2
	jle yokay		# jump if already okay
	mov r3,r5		# start at other x
	mov r2,r4		# start at other y
	inv r11			# other direction to whatever it was
yokay

# save off the width as our countdown, then save width and height for counting (overwrites x2,y2)
	mov r0,r12		# width for countdown
	inc r12			# include both ends (and don't have a zero count!)
	mov r0,r3		# width for slope calc
	mov r1,r2		# height for slope calc
	
# calculate the address offset of x1,y1 (r5,r4) (modified e/a routine)
	mov r4,r6
	sla r6,5
	soc r4,r6
	andi r6,>ff07
	a r5,r6
	andi r5,7		# r5 now has bit offet
	s r5,r6			# r6 now has byte offset (which is also a pattern table offset at >0000)
	sla r5,1		# bits # 2
	mov @bits(r5),r5	# now it's a bitmask
	
# clear r0 for slope accumulator
	clr r0

# and, we can now overwrite r4 and r11 for bitshift setup
# r11 is 0 for increment, and >ffff for decrement
	mov r11,r11		# test for 0
	jeq xinc
	li r11,slacode	# x instruction to shift left
	li r4,jeqcode	# x instruction to jump on equal (wrapped)
	clr r1
	movb r1,r5		# need lsb of r5 to be zeroed (we don't know where it starts though)
	swpb r5			# don't need first swap, both bytes were equal
	jmp hloop
xinc
	li r11,srccode	# x instruction to circular right shift
	li r4,jltcode	# x instruction to jump on arithmetic less than (wrapped)
	
# now:
#	r0 - slope accumulator
#	r1 - scratch
#	r2 - height count
#	r3 - width count
#	r4 - 'x' instruction for post-shift compare (jump if not byte wrap)
#	r5 - bitmask of pixel (both bytes!)
#   r6 - pattern table address of pixel
#	r7 - 'x' instruction opcode for plot
#	r11- 'x' instruction opcode for pixel shift
#	r12- number of pixels left to plot

hloop
# basic algorithm:
# - plot pixel (have to do it all inline anyway, out of registers!?)
# - countdown, exit if zero
# - add to slope accumlator
# - if greater than width, then increment address y position (this takes a little effort), and subtract from accum
# - shift bitmask
# - if comparison fails, then update address x position (also takes some effort)
# - loop
# (vdp access done 'safely' so should work on accelerated consoles and geneve)

# write the pixel itself
	swpb r6				# set address for read (>0000)
	movb r6,*R8
	swpb r6
	movb r6,*R8
	socb @bits+2,r6		# interleaving to give vdp time to fetch the data (set adr for write - >4000)
	movb @vdprd,r1		# get the byte
	x r7				# update the byte using r5 and the selected rop
	swpb r6				# set address for write
	movb r6,*R8
	swpb r6
	movb r6,*R8
	szcb @bits+2,r6		# more interleaving (restore adr and give time - >0000)
	movb r1,@vdpwd		# write the word back
	
# update the color map
	mov r13,r13			# draw mode is 0, all others skip the color update
	jne nocolor
	ai r6,>6000			# Add >2000 for color table plus >4000 for write mode bit
	swpb r6				# set address for write
	movb r6,*R8
	swpb r6
	movb r6,*R8
	ai r6,->6000		# undo the change and delay for vdp
	movb r14,@vdpwd		# set the color byte
nocolor
	
# countdown and check for exit
	dec r12
	jne notdone
	LWPI >8300
	b *r11
notdone

# count up slope accumulator
	a r2,r0				# add height
	c r0,r3				# compare against width - when it overflows, we drop down a row
	jl noslope
	s r3,r0				# overflow, so, subtract width from accumulator
	inc r6				# next row, but...
	czc @rowmask,r6		# we have to check for character row overflow
	jne noslope			# no overflow yet
	ai r6,248			# top of the next character
noslope

# handle bitmask shift
	x r11				# shifts r5 appropriately (circular, whichever way)
	x r4				# jumps if we /did/ wrap (different distances!)
	jmp hloop			# no wrap, so we skip the following code
	
# positive increment jumps here (skip of '1' word)
	ai r6,8				# 8 bytes to next step
	jmp hloop			# and that's it! (bit pattern rotates)
	
# negative increment jumps here (skip of 4 words) (a little slower sadly)
	ai r6,-8			# -8 bytes to next step
	li r5,>0100			# back to >0100 bit pattern
	jmp hloop
	
# steep line version of above... 
steep

# most of this setup is the same, but, we didn't have a spare register to inline the differences
# so the code is duplicated here.

# check whether x is incrementing or decrementing, remember result in r11 (no bl!)
	clr r11			# just a flag for incrementing
	c r5,r3
	jle nofl2
	seto r11		# flag decrementing
nofl2
	
# sort it so y increments (this lets us do address assumptions later)
# note we never use x2,y2 (r3,r2) again, so... we don't actually need to preserve them here.
# less a swap than a replacement then.
	c 	r4,r2		# y1,y2
	jle yoka2		# jump if already okay
	mov r3,r5		# start at other x
	mov r2,r4		# start at other y
	inv r11			# other direction to whatever it was
yoka2

# save off the height as our countdown, then save width and height for counting (overwrites x2,y2)
	mov r1,r12		# height for countdown
	inc r12			# include both ends (and don't have a zero count!)
	mov r0,r3		# width for slope calc
	mov r1,r2		# height for slope calc
	
# calculate the address offset of x1,y1 (r5,r4) (modified e/a routine)
	mov r4,r6
	sla r6,5
	soc r4,r6
	andi r6,>ff07
	a r5,r6
	andi r5,7		# r5 now has bit offet
	s r5,r6			# r6 now has byte offset (and pattern table at >0000)
	sla r5,1		# bits # 2
	mov @bits(r5),r5	# now it's a bitmask
	
# clear r0 for slope accumulator
	clr r0

# and, we can now overwrite r4 and r11 for bitshift setup
# r11 is 0 for increment, and >ffff for decrement
	mov r11,r11		# test for 0
	jeq xinc2
	li r11,slacode	# x instruction to shift left
	li r4,jeqcode	# x instruction to jump on equal (wrapped)
	clr r1
	movb r1,r5		# need lsb of r5 to be zeroed (we don't know where it starts though)
	swpb r5			# don't need first swap, both bytes were equal
	jmp vloop
xinc2
	li r11,srccode	# x instruction to circular right shift
	li r4,jltcode	# x instruction to jump on arithmetic less than (wrapped)
	
# now:
#	r0 - slope accumulator
#	r1 - scratch
#	r2 - height count
#	r3 - width count
#	r4 - 'x' instruction for post-shift compare (jump if not byte wrap)
#	r5 - bitmask of pixel (both bytes!)
#   r6 - pattern table address of pixel
#	r7 - 'x' instruction opcode for plot
#	r11- 'x' instruction opcode for pixel shift
#	r12- number of pixels left to plot

vloop
# basic algorithm:
# - plot pixel (have to do it all inline anyway, out of registers!?)
# - countdown, exit if zero
# - increment y address (update address on wrap)
# - add to slope accumlator
# - if greater than height, then update address x position via bitmask and position
# - loop
# (vdp access done 'safely' so should work on accelerated consoles and geneve)

# write the pixel itself
	swpb r6				# set address for read
	movb r6,*R8
	swpb r6
	movb r6,*R8
	socb @bits+2,r6		# interleaving to give vdp time to fetch the data (set adr for write - >4000)
	movb @vdprd,r1		# get the byte
	x r7				# update the byte using r5 and the selected rop
	swpb r6				# set address for write
	movb r6,*R8
	swpb r6
	movb r6,*R8
	szcb @bits+2,r6		# more interleaving (restore adr and give time - >0000)
	movb r1,@vdpwd		# write the word back
	
# update the color map
	mov r13,r13			# draw mode is 0, all others skip the color update
	jeq nocolor2
	ai r6,>6000			# Add >2000 for color table plus >4000 for write mode bit
	swpb r6				# set address for write
	movb r6,*R8
	swpb r6
	movb r6,*R8
	ai r6,->6000		# undo the change and delay for vdp
	movb R14,@vdpwd		# set the color byte
nocolor2
	
# countdown and check for exit
	dec r12
	jne notdon2
	LWPI >8300
	b *r11
notdon2

	inc r6				# next row, but...
	czc @rowmask,r6		# we have to check for character row overflow
	jne noov2			# no overflow yet
	ai r6,248			# top of the next character
noov2

# count up slope accumulator
	a r3,r0				# add width
	c r0,r2				# compare against height - when it overflows, we update x
	jl vloop
	s r2,r0				# overflow, so, subtract height from accumulator

# handle bitmask shift
	x r11				# shifts r5 appropriately (circular, whichever way)
	x r4				# jumps if we /did/ wrap (different distances!)
	jmp vloop			# no wrap, so we skip the following code
	
# positive increment jumps here (skip of '1' word)
	ai r6,8				# 8 bytes to next step
	jmp vloop			# and that's it! (bit pattern rotates)
	
# negative increment jumps here (skip of 4 words) (a little slower sadly)
	ai r6,-8			# -8 bytes to next step
	li r5,>0100			# back to >0100 bit pattern
	jmp vloop


	end
