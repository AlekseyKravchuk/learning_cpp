#define F_CPU 1000000UL  // set MK frequency to 1MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>  // size_t

void TurnOnDigit(int index_of_common_cathode_pin);
void GetDigitsFromNumber(int number);

// use port D for segments control (7 segments: a, b, c, d, e, f, g)
#define SEG7_DDRx  DDRD
#define SEG7_PORTx PORTD

// use port B for common cathodes control (4 common cathodes in case of 4-digit 7-segment indicator)
#define CC_DDRx DDRB
#define CC_PORTx PORTB

int digits_7seg_CC[] = {  // PORTX values to highlight digits in 7-segment indicator, Common Cathode (CC)
	0b00111111,  // highlight "0"  Common Cathode (CC)
	0b00000110,  // highlight "1"  Common Cathode (CC)
	0b01011011,  // highlight "2"  Common Cathode (CC)
	0b01001111,  // highlight "3"  Common Cathode (CC)
	0b01100110,  // highlight "4"  Common Cathode (CC)
	0b01101101,  // highlight "5"  Common Cathode (CC)
	0b01111101,  // highlight "6"  Common Cathode (CC)
	0b00000111,  // highlight "7"  Common Cathode (CC)
	0b01111111,  // highlight "8"  Common Cathode (CC)
	0b01101111,  // highlight "9"  Common Cathode (CC)
	0b10000000   // highlight "dp/colon" Common Cathode (CC)
};

// ================== Global variables ==================
int cc[4] = {0, 1, 2, 3};  // set common cathodes pins for selected port (PB0, PB1, PB2, PB3)
const size_t digits_len = 4;
int digits[] = {0, 0, 0, 0};  //size_t digits_len = sizeof(digits) / sizeof(int);
int seconds_cntr = 0;
int cc_idx = 0;  // index of currently active common cathode (GROUND will be applied to that pin): [0,1,2,3]
// =======================================================

//Interrupt Service Routine will be called each 2 milliseconds for each digit, so period T = 2ms*4digits = 8ms
ISR(TIMER0_OVF_vect) {
	TurnOnDigit(cc_idx);
	cc_idx = (cc_idx + 1) % 4;
}

// this interrupt will be triggered each 1 second
ISR(TIMER1_COMPA_vect) {
	//seconds_cntr = (seconds_cntr + 1) % 10000;
	//seconds_cntr = (seconds_cntr > 9999) ? 0 : seconds_cntr + 1;
	if (++seconds_cntr > 5) {
		seconds_cntr = 0;
		PORTC |= 1<<0;
	}
	
	GetDigitsFromNumber(seconds_cntr);
	//PORTC ^= 1<<0;  // toggle LED connected to PC0
}

void GetDigitsFromNumber(int number) {
	//if (!number) { return; }

	int i = digits_len - 1;  // initial value of i == 3
	while (number) {
		digits[i--] = number % 10;
		number /= 10;
	}
}

void TurnOnDigit(int i) {
	// we need to ensure that all common cathodes are DISABLED (i.e. supplied with HIGH potential)
	CC_PORTx |= (1 << cc[0]) | (1 << cc[1])   | (1 << cc[2])  | (1 << cc[3]);
	
	// select digit which should be turned ON (configure needed segments)
	SEG7_PORTx = digits_7seg_CC[digits[i]];
	
	// apply GROUND to desired common cathode pin (set '0' in corresponding bit in PORTx)
	CC_PORTx &= ~(1<<cc[i]);
}

void ConfigureSegmentsPins() {
	SEG7_DDRx = 0xFF;  // all pins of port D set to OUTPUT
	SEG7_PORTx = 0x00; // all pins of port D set to LOW potential: by default all segments are turned OFF
}

void ConfigureCommonCathodesPins() {
	// configure all 4 pins corresponding to common cathodes (arr_cc[i]) as OUTPUT
	//	CC_DDRX  |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
	CC_DDRx |= (1 << cc[0]) | (1 << cc[1])   | (1 << cc[2])  | (1 << cc[3]);

	// configure all 4 pins corresponding to common cathodes (cc[i]) as HIGH potential
	// as we have common cathode (CC) we need to configure pins with HIGH potential for digits to be turned OFF by default
	// i.e. to turn ON desired digit we should apply GROUND ("-") to corresponding common cathode pin
	//	CC_PORTX |= |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
	CC_PORTx |= (1 << cc[0]) | (1 << cc[1])   | (1 << cc[2])  | (1 << cc[3]);
}

void ConfigureTimerCounter() {
	//// ====================== Configure Timer/Counter0 =====================
	// Timer/Counter0 Control Register: enable running TCNT0 and set prescaler = clk/8
	// Toverflow = prescaler * max_TCNT_val / freq_MCU = 8 * 255 / 10**6 = 0.00204 seconds ~ 2 miliseconds
	TCCR0 |= 1<<CS01;    
	TCCR0 &= ~(1<<CS00); 
	
	TCNT0 = 0;
	TIMSK |= 1<<TOIE0;  // Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
	//// ================ END of Timer/Counter0 configuration ================
	

	// ====================== Configure Timer/Counter1 =====================
	// set Prescaler (clk/64) for Timer/Counter1; 1'000'000 / 64 = 15'625 Hz ()
	TCCR1B |= 1<<CS11 | 1<<CS10;
	TCCR1B &= ~(1<<CS12);
	
	TCNT1 = 0;
	OCR1A = 15625;       // Output Compare Register, when TCNT1 becomes equal to OCR1AL, interrupt will occur
	TIMSK |= 1<<OCIE1A;  // Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TCCR1B |= 1<<WGM12;  // reset TCNT1 immediately when match occured; mode #4: Clear Timer on Compare Match (CTC)
	
	// ================ END of Timer/Counter1 configuration ================
	
	sei();  // Status Register Bit 7 – I: Global Interrupt Enable
}

void ConfigureStandaloneLED() {
	DDRC |= 1<<0;      // set PC0 as OUTPUT
	PORTC &= ~(1<<0);  // set PC0 as LOW level
}

int main() {
	ConfigureSegmentsPins();
	ConfigureCommonCathodesPins();
	ConfigureTimerCounter();
	ConfigureStandaloneLED();

	while (true) {
		// all useful stuff is implemented in Interrupt Service Routines (ISR), or Interrupt Handlers
	}

	return 0;
}





