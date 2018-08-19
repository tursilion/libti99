// RS232 code for the TI-99/4A by Tursi
// You can copy this file and use it at will ;)
// This is based on the standard TI RS232 controller and may require tweaks for other cards
// Port for PIO is defined too. Note this bypasses the DSR - just use the file code for DSR access
// Note there's no flow control in this lib (RTS/CTS/etc)

// TODO: do we actually need to map the card in to talk to the UARTs? Flipside never did...
// But I had a Corcomp card, so I'm not sure if it was different...

#ifndef RS232_H
#define RS232_H

// CRU addresses
// standard card
#define RS232_CARD		(0x1300)	// base CRU for the card
// alternate card
#define RS232_ALT_CARD	(0x1500)

// TMS9902 CRU offsets
#define RS232_9902A		(0x0040)	// base CRU of the first UART
#define RS232_9902B		(0x0080)	// base CRU of the second UART

// CRU bits (card base)
#define RS232_BIT_SELECT	0		// standard enable/disable bit
#define RS232_BIT_PIOREAD	1		// set bit to 1 to read PIO, 0 to write it
#define RS232_BIT_PIOHS		2		// read for HANDSHAKEIN, write for HANDSHAKEOUT
#define RS232_BIT_PIOSPARE	3		// read for SPAREIN, write for SPAREOUT
#define RS232_BIT_MIRROR	4		// reads back whatever you wrote to it
#define RS232_BIT_CTS1		5		// CTS 1 pin
#define RS232_BIT_CTS2		6		// CTS 2 pin
#define RS232_BIT_LED		7		// activates the card LED when '1'

// 9902 CRU bits (9902 base)
#define RS232_UART_RX		0		// 8 bits are mapped here for the character receive, use LDCR
#define RS232_UART_RXERR	9		// set on receive error
#define RS232_UART_PARTITY	10		// set on parity error
#define RS232_UART_RXOV		11		// set on receive overflow
#define RS232_UART_FRAMEERR	12		// set on framing error
#define RS232_UART_1STBIT	13		// set when first bit arrived
#define RS232_UART_RXTEST	14		// set when receiving byte (for test purposes)
#define RS232_UART_RIN		15		// status of RIN pin (data in?)
#define RS232_UART_RXINT	16		// rx interrupt (clear with bit 18)
#define RS232_UART_TXINT	17		// tx interrupt (clear by loading emission register)
#define RS232_UART_TIMERINT	19		// timer interrupt (clear by writing bit 20)
#define RS232_UART_CTSINT	20		// RTS (or DSR?) /or/ CTS interrupt occurred (reset bit 21)
#define RS232_UART_RXFULL	21		// receive register full (clear with bit 18 (?))
#define RS232_UART_TXEMPTY	22		// emission register empty
#define RS232_UART_SHEMPTY	23		// shift register emtpy (no data currently being sent)
#define RS232_UART_TIMERR	24		// timer error - elapsed twice. Reset with bit 20
#define RS232_UART_TIMEDONE	25		// timer elapsed - reset with bit 20
#define RS232_UART_RTS		26		// status of RTS pin (inverted)
#define RS232_UART_DSR		27		// status of DSR pin (inverted)
#define RS232_UART_CTS		28		// status of CRS pin (inverted)
#define RS232_UART_CTSNEW	29		// DSR or CTS changed (or RTS? Reset bit 21)
#define RS232_UART_LOADING	30		// register being loaded from 0-10
#define RS232_UART_INTACT	31		// an interrupt has occurred (reset bit 31: output 11-14=1, 17-20=0, input 23-23=1, 13,21,25=0)

// alternate names for write mode - to write a register set the bit first, then LDCR into bits 0-??
#define RS232_UART_WRITE	0		// 0 is also used to write a selected register (11 bits here) Registers auto-increment!
#define RS232_UART_EMITRATE	11		// write 1 to load emission rate register with bits 0-10 (emit and recv may be used together)
#define RS232_UART_RECVRATE	12		// write 1 to load reception rate register with bits 0-10
#define RS232_UART_INTERVAL	13		// write 1 to load interval register with bits 0-7 (countdown timer)
#define RS232_UART_CONTROL	14		// write 1 to load control register with bits 0-7
#define RS232_UART_LOOPTEST	15		// write 1 to set loopback test mode (RTS->CTS, XOUT->RIN, timer*32)
#define RS232_UART_SETRTS	16		// set RTS pin (1 only is input bits 22 and 23 are 0)
#define RS232_UART_ABORT	17		// abort transmission (XOUT=0 if bits 22 and 23 are 0)
#define RS232_UART_ENRXINT	18		// enable rx interrupts
#define RS232_UART_ENTXINT	19		// enable tx interrupts
#define RS232_UART_ENTIMINT	20		// enable timer interrupts
#define RS232_UART_ENCTSINT	21		// enable interrupt when CTS or DSR change? (TODO: but read pin 20 is RTS/CTS?)
#define RS232_UART_RESETINT	31		// reset interrupt, if output 11-14=1, 17-20=0, input 23-23=1, 13,21,25=0

// address for PIO port (when CRU mapped in)
#define PIO_PORT 		*((unsigned char*)(0x5000))

// bitrates - 500,000 / bps - assumes 3MHz console and CLK4M is unset - 300-2400 are well tested
#define RS232_BPS_300	1667
#define RS232_BPS_1200	417
#define RS232_BPS_2400	208
#define RS232_BPS_4800	104
#define RS232_BPS_9600	52
#define RS232_BPS_19200	26
#define RS232_BPS_38400	13

// control bits for RS232 line format
#define RS232_CTRL_STOP_1		0x80	// 1 stop bit
#define RS232_CTRL_STOP_15		0x00	// 1.5 stop bits
#define RS232_CTRL_STOP_2		0x40	// 2 stop bits

#define RS232_CTRL_NO_PARITY	0x00	// no parity
#define RS232_CTRL_EVEN_PARITY	0x20	// even parity
#define RS232_CTRL_ODD_PARITY	0x30	// odd parity

#define RS232_CTRL_CLK4M		0x08	// if set, divide clock by 4 instead of 3 (base rate then is 375,000 instead of 500,000)

#define RS232_CTRL_5_BIT		0x00	// 5 bits
#define RS232_CTRL_6_BIT		0x01	// 6 bits
#define RS232_CTRL_7_BIT		0x02	// 7 bits
#define RS232_CTRL_8_BIT		0x03	// 8 bits

// lots of notes and this implementation based on http://www.unige.ch/medecine/nouspikel/ti99/rs232c.htm
// The PIO is a combination of memory-mapped and some CRU for control
// the UARTs are entirely CRU
// Normally, set your card base, configure the card, then set the UART/9902 base and configure the UART itself
// funtions below do the calculations/mapping each call, except for the 'raw' functions, which will be faster if
// you can afford to remember the card's state in your application

// basic card functions - you don't need these unless you use the 'raw' mode function calls

// turn on the specified card and activate its LED
// never activate more than one card at a time and don't do any other DSRs (like disk)
// while the RS232 card is active.
void rs232raw_activateCard(int card);
void rs232raw_deactivateCard(int card);


// PIO functions - no raw access function, just use PIO_PORT directly
// card is either RS232_CARD or RS232_ALT_CARD

// write a byte to the specified PIO (sets it to output)
void rs232_writePIO(int card, int byte);
// read a byte from the specified PIO (sets it to input)
int rs232_readPIO(int card);

// activates the card and sets PIO to output - you must use deactivateCard when finished!
void rs232raw_setPIOoutput(int card);
// activates the card and sets PIO to input - you must use deactivateCard when finished!
void rs232raw_setPIOinput(int card);


// RS232 functions
// card is either RS232_CARD or RS232_ALT_CARD
// uart is RS232_9902A or RS232_9902B
// bps is one of the bitrate defines above (note high speed success varies by card ;) )

// sets the specified line format, use the RS232_CTRL_xxx bits
void rs232_setcontrol(int card, int uart, int control);

// set the bitrate on the card and uart (sets both directions the same)
void rs232_setbps(int card, int uart, int bps);

// writes a byte to the specified serial port - returns 0 if the byte could not be written
// (because the write register is full)
int rs232_writebyte(int card, int uart, int ch);

// test if a byte is available at the specied serial port (returns 0 if not, other value if so)
int rs232_poll(int card, int uart);

// reads a byte from the specified serial port, blocks - so always check rs232_poll first!
int rs232_readbyte(int card, int uart);

// return the error bits from the specified port (we don't check the timer here)
// 0x01 = reception error (set for any of the other three)
// 0x02 = parity error
// 0x04 = overflow error (receive data lost)
// 0x08 = frame error (line corruption or incorrect line format)
int rs232_geterrs(int card, int uart);


// raw versions of the above functions - you must have the correct card active

// activates the card, and calculates and returns the correct uart rawCRU address for
// the rest of the raw functions. You MUST call deactivateCard when done!
int rs232raw_getuart(int card, int uart);

// check the TX and RX status
// 0x01 = receive buffer contains a byte
// 0x02 = transmit buffer empty
// 0x04 = transmission line clear
// 0x08 = timer error (not used in this code)
// 0x10 = time elapsed (not used in this code)
// 0x20 = RTS (inverted)
// 0x40 = DSR (inverted)
// 0x80 = CTS (inverted)
int rs232raw_checkstatus(int rawCRU);

// writes a byte to the specified serial port - does not check the transmission register!
void rs232raw_writebyte(int rawCRU, int ch);

// test if a byte is available at the specied serial port (returns 0 if not, other value for true)
int rs232raw_poll(int rawCRU);

// reads a byte from the specified serial port - whatever is there. Does not check or
// block, so if you need correct data do a poll first!
int rs232raw_readbyte(int rawCRU);

// return the error bits from the specified port (we don't check the timer here)
// 0x01 = reception error (set for any of the other three)
// 0x02 = parity error
// 0x04 = overflow error (receive data lost)
// 0x08 = frame error (line corruption or incorrect line format)
int rs232raw_geterrs(int rawCRU);


#endif /* RS232_H */
