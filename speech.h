#ifndef _SPEECH_H
#define _SPEECH_H 1

#define SPCHRD    *((volatile unsigned char*)0x9000)
#define SPCHWT		*((volatile unsigned char*)0x9400)

#define SPCH_CMD_RESET 0x70
#define SPCH_CMD_EXT 0x60
#define SPCH_CMD_VOCAB 0x50
#define SPCH_CMD_ADDR 0x40
#define SPCH_CMD_READ 0x10

#define SPCH_STATUS_TALK 0x80
#define SPCH_STATUS_LOW 0x40

// Define a default location in scratch pad for the safe read routine and
// status byte mailbox. Requires 14 bytes.
#ifndef SAFE_READ_PAD
#define SAFE_READ_PAD 0x8320
#endif


struct __attribute__((__packed__)) LpcPlaybackCtx {
    char* addr;
    int remaining;
};


/*
 * Issue the reset command to the speech synthesizer.
 * (this is automatically included in a detect_speech call)
 */
void speech_reset();

/*
 * Return 1 if speech synthesizer is attached to the 4A.
 * It sends the reset cmd, then a read of PHROM address 0x0000.
 */
int detect_speech();

/*
 * Say the vocabulary phrase in the speech ROM (PHROM) at the specified address.
 * See EA manual for addresses of each built in phrase.
 */
void say_vocab(int phrase_addr);

/*
 * With an LPC code loaded into CPU RAM at addr, say_data sends the say_external command and transmits the entire LCP into the synthesizer's FIFO.
 */
void say_data(const char* addr, int len);

/*
 * Given a LpcPlaybackCtx with a pointer to the LPC data, and the remaining bytes to send, sends the command code and upto the first 16 bytes of the
 * LPC data. ctx.remaining and ctx.addr are modified to be used by subsequent speech_continue calls.
 */
void speech_start(struct LpcPlaybackCtx* ctx);

/*
 * Continue feeding speech data to the synthesizer, upto 8 bytes per call. ctx is modified.
 */
void speech_continue(struct LpcPlaybackCtx* ctx);

/*
 * Wait for speech to finish producing the current phrase.
 */
void speech_wait();


/*
 * The following defines addresses for words and phrases in the resident
 * speech synthesizer vocabulary, for use with the say_vocab function.
 */
#define VOCAB_0 0x13C3
#define VOCAB_1 0x1409
#define VOCAB_2 0x145C
#define VOCAB_3 0x149A
#define VOCAB_4 0x14E7
#define VOCAB_5 0x1531
#define VOCAB_6 0x15A8
#define VOCAB_7 0x15E8
#define VOCAB_8 0x1637
#define VOCAB_9 0x1664
#define VOCAB_A1_uh 0x1700
#define VOCAB_A_ay 0x16E4
#define VOCAB_ABOUT 0x1714
#define VOCAB_AFTER 0x1769
#define VOCAB_AGAIN 0x17A5
#define VOCAB_ALL 0x1807
#define VOCAB_AM 0x1830
#define VOCAB_AN 0x1876
#define VOCAB_AND 0x18AC
#define VOCAB_ANSWER 0x1913
#define VOCAB_ANY 0x1962
#define VOCAB_ARE 0x556E
#define VOCAB_AS 0x19A7
#define VOCAB_ASSUME 0x19E8
#define VOCAB_AT 0x1A25
#define VOCAB_B 0x1A42
#define VOCAB_BACK 0x1A64
#define VOCAB_BASE 0x1A8F
#define VOCAB_BE 0x1A42
#define VOCAB_BETWEEN 0x1ADE
#define VOCAB_BLACK 0x1B47
#define VOCAB_BLUE 0x1B8A
#define VOCAB_BOTH 0x1BB6
#define VOCAB_BOTTOM 0x1BEA
#define VOCAB_BUT 0x1C20
#define VOCAB_BUY 0x1C48
#define VOCAB_BY 0x1C48
#define VOCAB_BYE 0x1C48
#define VOCAB_C 0x1C86
#define VOCAB_CAN 0x1CD9
#define VOCAB_CASSETTE 0x1D10
#define VOCAB_CENTER 0x1D47
#define VOCAB_CHECK 0x1D82
#define VOCAB_CHOICE 0x1DA2
#define VOCAB_CLEAR 0x1DE6
#define VOCAB_COLOR 0x1E20
#define VOCAB_COME 0x1E54
#define VOCAB_COMES 0x1E87
#define VOCAB_COMMA 0x1EDE
#define VOCAB_COMMAND 0x1F1A
#define VOCAB_COMPLETE 0x1F71
#define VOCAB_COMPLETED 0x1FCD
#define VOCAB_COMPUTER 0x2034
#define VOCAB_CONNECTED 0x208B
#define VOCAB_CONSOLE 0x20F3
#define VOCAB_CORRECT 0x213C
#define VOCAB_COURSE 0x2182
#define VOCAB_CYAN 0x21C0
#define VOCAB_D 0x2203
#define VOCAB_DATA 0x223C
#define VOCAB_DECIDE 0x2294
#define VOCAB_DEVICE 0x22FD
#define VOCAB_DID 0x2366
#define VOCAB_DIFFERENT 0x23C4
#define VOCAB_DISKETTE 0x242D
#define VOCAB_DO 0x2480
#define VOCAB_DOES 0x24B3
#define VOCAB_DOING 0x24EA
#define VOCAB_DONE 0x253E
#define VOCAB_DOUBLE 0x2599
#define VOCAB_DOWN 0x25D3
#define VOCAB_DRAW 0x2612
#define VOCAB_DRAWING 0x2668
#define VOCAB_E 0x26CB
#define VOCAB_EACH 0x26F0
#define VOCAB_EIGHT 0x1637
#define VOCAB_EIGHTY 0x2723
#define VOCAB_ELEVEN 0x2579
#define VOCAB_ELSE 0x27B6
#define VOCAB_END 0x27F5
#define VOCAB_ENDS 0x2866
#define VOCAB_ENTER 0x28AD
#define VOCAB_ERROR 0x28EF
#define VOCAB_EXACTLY 0x2937
#define VOCAB_EYE 0x3793
#define VOCAB_F 0x299F
#define VOCAB_FIFTEEN 0x29C2
#define VOCAB_FIFTY 0x2A1D
#define VOCAB_FIGURE 0x2A60
#define VOCAB_FIND 0x2AD7
#define VOCAB_FINE 0x2B1E
#define VOCAB_FINISH 0x2B5B
#define VOCAB_FINISHED 0x2B94
#define VOCAB_FIRST 0x2BD7
#define VOCAB_FIT 0x2C14
#define VOCAB_FIVE 0x1531
#define VOCAB_FOR 0x14E7
#define VOCAB_FORTY 0x2C3E
#define VOCAB_FOUR 0x14E7
#define VOCAB_FOURTEEN 0x2C7F
#define VOCAB_FOURTH 0x2D19
#define VOCAB_FROM 0x2D74
#define VOCAB_FRONT 0x2DBC
#define VOCAB_G 0x2DEB
#define VOCAB_GAMES 0x2E28
#define VOCAB_GET 0x2E8C
#define VOCAB_GETTING 0x2EBA
#define VOCAB_GIVE 0x2F38
#define VOCAB_GIVES 0x2F8D
#define VOCAB_GO 0x2FFC
#define VOCAB_GOES 0x3031
#define VOCAB_GOING 0x3079
#define VOCAB_GOOD 0x30D6
#define VOCAB_GOODBYE 0x3148
#define VOCAB_GOOD_WORK 0x30FA
#define VOCAB_GOT 0x31A0
#define VOCAB_GRAY 0x31D1
#define VOCAB_GREEN 0x321D
#define VOCAB_GUESS 0x327E
#define VOCAB_H 0x32C0
#define VOCAB_HAD 0x32EF
#define VOCAB_HAND 0x3339
#define VOCAB_HANDHELD_UNIT 0x337F
#define VOCAB_HAS 0x3405
#define VOCAB_HAVE 0x344A
#define VOCAB_HEAD 0x348C
#define VOCAB_HEAR 0x34E5
#define VOCAB_HELLO 0x351A
#define VOCAB_HELP 0x3571
#define VOCAB_HERE 0x34E5
#define VOCAB_HIGHER 0x35AE
#define VOCAB_HIT 0x360A
#define VOCAB_HOME 0x363E
#define VOCAB_HOW 0x3689
#define VOCAB_HUNDRED 0x36EF
#define VOCAB_HURRY 0x3757
#define VOCAB_I 0x3793
#define VOCAB_IF 0x3850
#define VOCAB_IN 0x3872
#define VOCAB_INCH 0x38B5
#define VOCAB_INCHES 0x38FA
#define VOCAB_INSTRUCTION 0x394B
#define VOCAB_INSTRUCTIONS 0x39BD
#define VOCAB_IS 0x3A32
#define VOCAB_IT 0x3A7A
#define VOCAB_I_WIN 0x37CF
#define VOCAB_J 0x3AAE
#define VOCAB_JOYSTICK 0x3AED
#define VOCAB_JUST 0x3B4C
#define VOCAB_K 0x3B8A
#define VOCAB_KEY 0x3BB9
#define VOCAB_KEYBOARD 0x3BE9
#define VOCAB_KNOW 0x3C4F
#define VOCAB_L 0x3C8F
#define VOCAB_LARGE 0x3CD0
#define VOCAB_LARGER 0x3D19
#define VOCAB_LARGEST 0x3D67
#define VOCAB_LAST 0x3DDE
#define VOCAB_LEARN 0x3E1E
#define VOCAB_LEFT 0x3E78
#define VOCAB_LESS 0x3EB2
#define VOCAB_LET 0x3F08
#define VOCAB_LIKE 0x3F2F
#define VOCAB_LIKES 0x3F6A
#define VOCAB_LINE 0x3FD5
#define VOCAB_LOAD 0x404B
#define VOCAB_LONG 0x40D3
#define VOCAB_LOOK 0x413D
#define VOCAB_LOOKS 0x4191
#define VOCAB_LOWER 0x41E7
#define VOCAB_M 0x4233
#define VOCAB_MADE 0x4267
#define VOCAB_MAGENTA 0x42AE
#define VOCAB_MAKE 0x432E
#define VOCAB_ME 0x437D
#define VOCAB_MEAN 0x43BD
#define VOCAB_MEMORY 0x4405
#define VOCAB_MESSAGE 0x446C
#define VOCAB_MESSAGES 0x44D7
#define VOCAB_MIDDLE 0x4551
#define VOCAB_MIGHT 0x4593
#define VOCAB_MODULE 0x45DF
#define VOCAB_MORE 0x4642
#define VOCAB_MOST 0x4693
#define VOCAB_MOVE 0x46DF
#define VOCAB_MUST 0x473D
#define VOCAB_N 0x4789
#define VOCAB_NAME 0x47C0
#define VOCAB_NEAR 0x4833
#define VOCAB_NEED 0x4880
#define VOCAB_NEGATIVE 0x48DC
#define VOCAB_NEGATIVE 0x48DC
#define VOCAB_NEXT 0x4959
#define VOCAB_NICE_TRY 0x49A5
#define VOCAB_NINE 0x1664
#define VOCAB_NINETY 0x4A4E
#define VOCAB_NO 0x3C4F
#define VOCAB_NOT 0x4AAB
#define VOCAB_NOW 0x4ADA
#define VOCAB_NUMBER 0x4B20
#define VOCAB_O 0x4B7D
#define VOCAB_OF 0x4BBA
#define VOCAB_OFF 0x4C13
#define VOCAB_OH 0x4B7D
#define VOCAB_ON 0x4C41
#define VOCAB_ONE 0x1409
#define VOCAB_ONLY 0x4C8B
#define VOCAB_OR 0x4CDC
#define VOCAB_ORDER 0x4D34
#define VOCAB_OTHER 0x4D8A
#define VOCAB_OUT 0x4DD4
#define VOCAB_OVER 0x4E0A
#define VOCAB_P 0x4E66
#define VOCAB_PART 0x4E9F
#define VOCAB_PARTNER 0x4EE0
#define VOCAB_PARTS 0x4F31
#define VOCAB_PERIOD 0x4F81
#define VOCAB_PLAY 0x4FE5
#define VOCAB_PLAYS 0x502D
#define VOCAB_PLEASE 0x5093
#define VOCAB_POINT 0x50EC
#define VOCAB_POINT 0x50EC
#define VOCAB_POSITION 0x5148
#define VOCAB_POSITIVE 0x51B3
#define VOCAB_POSITIVE 0x51B3
#define VOCAB_PRESS 0x5231
#define VOCAB_PRINT 0x526D
#define VOCAB_PRINTER 0x52AA
#define VOCAB_PROBLEM 0x52F9
#define VOCAB_PROBLEMS 0x5360
#define VOCAB_PROGRAM 0x53EE
#define VOCAB_PUT 0x5477
#define VOCAB_PUTTING 0x54AA
#define VOCAB_Q 0x5520
#define VOCAB_R 0x556E
#define VOCAB_RANDOMLY 0x55A0
#define VOCAB_READ1_red 0x57C1
#define VOCAB_READ_read 0x5652
#define VOCAB_READY_TO_START 0x56B3
#define VOCAB_RECORDER 0x5745
#define VOCAB_RED 0x57C1
#define VOCAB_REFER 0x5801
#define VOCAB_REMEMBER 0x5861
#define VOCAB_RETURN 0x58CF
#define VOCAB_REWIND 0x593A
#define VOCAB_RIGHT 0x7C38
#define VOCAB_ROUND 0x59C2
#define VOCAB_S 0x5A5A
#define VOCAB_SAID 0x5AA1
#define VOCAB_SAVE 0x5AEF
#define VOCAB_SAY 0x5B65
#define VOCAB_SAYS 0x3BA2
#define VOCAB_SCREEN 0x5BFB
#define VOCAB_SECOND 0x5C5B
#define VOCAB_SEE 0x1C86
#define VOCAB_SEES 0x5CBF
#define VOCAB_SET 0x5D1B
#define VOCAB_SEVEN 0x15E8
#define VOCAB_SEVENTY 0x5D50
#define VOCAB_SHAPE 0x5DA5
#define VOCAB_SHAPES 0x5DDE
#define VOCAB_SHIFT 0x5E27
#define VOCAB_SHORT 0x5E5C
#define VOCAB_SHORTER 0x5EA5
#define VOCAB_SHOULD 0x5F24
#define VOCAB_SIDE 0x5F6D
#define VOCAB_SIDES 0x5FC8
#define VOCAB_SIX 0x15A8
#define VOCAB_SIXTY 0x601A
#define VOCAB_SMALL 0x6070
#define VOCAB_SMALLER 0x60AE
#define VOCAB_SMALLEST 0x60F1
#define VOCAB_SO 0x6153
#define VOCAB_SOME 0x6197
#define VOCAB_SORRY 0x61C6
#define VOCAB_SPACE 0x6226
#define VOCAB_SPACES 0x625D
#define VOCAB_SPELL 0x62CC
#define VOCAB_SQUARE 0x6333
#define VOCAB_START 0x637C
#define VOCAB_STEP 0x63C5
#define VOCAB_STOP 0x63F7
#define VOCAB_SUM 0x6197
#define VOCAB_SUPPOSED 0x6423
#define VOCAB_SUPPOSED_TO 0x6489
#define VOCAB_SURE 0x64F4
#define VOCAB_T 0x6551
#define VOCAB_TAKE 0x658B
#define VOCAB_TEEN 0x65BF
#define VOCAB_TELL 0x6603
#define VOCAB_TEN 0x664E
#define VOCAB_TEXAS_INSTRUMENTS 0x6696
#define VOCAB_THAN 0x675B
#define VOCAB_THAT 0x67B6
#define VOCAB_THAT_IS_INCORRECT 0x6816
#define VOCAB_THAT_IS_RIGHT 0x68FE
#define VOCAB_THE1_the 0x69B6
#define VOCAB_THEIR 0x6A72
#define VOCAB_THEN 0x69E1
#define VOCAB_THERE 0x6A72
#define VOCAB_THESE 0x6ADE
#define VOCAB_THE_thee 0x6974
#define VOCAB_THEY 0x6B47
#define VOCAB_THING 0x6BA0
#define VOCAB_THINGS 0x6C0F
#define VOCAB_THINK 0x6C73
#define VOCAB_THIRD 0x6CBC
#define VOCAB_THIRTEEN 0x6D11
#define VOCAB_THIRTY 0x6DA2
#define VOCAB_THIS 0x6DDE
#define VOCAB_THREE 0x149A
#define VOCAB_THREW 0x6E26
#define VOCAB_THROUGH 0x6E26
#define VOCAB_TIME 0x6E69
#define VOCAB_TO 0x145C
#define VOCAB_TOGETHER 0x6EB0
#define VOCAB_TONE 0x6F1F
#define VOCAB_TOO 0x145C
#define VOCAB_TOP 0x6F8D
#define VOCAB_TRY 0x6FBB
#define VOCAB_TRY_AGAIN 0x700F
#define VOCAB_TURN 0x7092
#define VOCAB_TWELVE 0x70CE
#define VOCAB_TWENTY 0x7119
#define VOCAB_TWO 0x145C
#define VOCAB_TYPE 0x7170
#define VOCAB_U 0x71BE
#define VOCAB_UHOH 0x71F4
#define VOCAB_UNDER 0x7245
#define VOCAB_UNDERSTAND 0x729D
#define VOCAB_UNTIL 0x732F
#define VOCAB_UP 0x739F
#define VOCAB_UPPER 0x73C3
#define VOCAB_USE 0x7403
#define VOCAB_V 0x7449
#define VOCAB_VARY 0x7487
#define VOCAB_VERY 0x74DA
#define VOCAB_W 0x7520
#define VOCAB_WAIT 0x759D
#define VOCAB_WANT 0x75DF
#define VOCAB_WANTS 0x7621
#define VOCAB_WAY 0x76B0
#define VOCAB_WE 0x767D
#define VOCAB_WEIGH 0x76B0
#define VOCAB_WEIGHT 0x759D
#define VOCAB_WELL 0x7717
#define VOCAB_WERE 0x775D
#define VOCAB_WHAT 0x77BC
#define VOCAB_WHAT_WAS_THAT 0x77E9
#define VOCAB_WHEN 0x7875
#define VOCAB_WHERE 0x78AB
#define VOCAB_WHICH 0x78F4
#define VOCAB_WHITE 0x7924
#define VOCAB_WHO 0x7969
#define VOCAB_WHY 0x79B4
#define VOCAB_WILL 0x7A11
#define VOCAB_WITH 0x7A6B
#define VOCAB_WON 0x1409
#define VOCAB_WORD 0x7AAB
#define VOCAB_WORDS 0x7B0A
#define VOCAB_WORK 0x7B75
#define VOCAB_WORKING 0x7BBC
#define VOCAB_WRITE 0x7C38
#define VOCAB_X 0x7C8D
#define VOCAB_Y 0x7CB2
#define VOCAB_YELLOW 0x7CF8
#define VOCAB_YES 0x7D58
#define VOCAB_YET 0x7D99
#define VOCAB_YOU 0x71BE
#define VOCAB_YOUR 0x7E4D
#define VOCAB_YOU_WIN 0x7DDB
#define VOCAB_Z 0x7E99
#define VOCAB_ZERO 0x13C3

#endif