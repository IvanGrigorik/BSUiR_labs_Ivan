#include <msp430.h>
#include <stdbool.h>
// Var 11
static volatile bool button1INTR = false;
static volatile bool button2INTR = false;
static volatile unsigned timer_counter = 0;

void setup(void) {
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = 62;    // equal to 0x00111110

    // PIN1 - PIN5 is LEDs, PIN7 - button
    P1DIR &= ~BIT7;
    P1SEL = 0;		// All pins are I/O
    P1REN = BIT7;	// Pullup for button
    P1OUT = BIT7;
    P1IES |= BIT7;
    P1IFG = 0;
    P1IE |= BIT7;

    // Set up second button
    P2DIR &= ~BIT2;
    P2SEL = 0;
    P2REN = BIT2;
    P2OUT = BIT2;
    P2IES |= BIT2;
    P2IFG = 0;
    P2IE |= BIT2;

    // Set up timer
    TA1CCTL0 = CCIE;
    TA1CCR0 = 2047;
    TA1CTL = TASSEL_2 + MC_1 + TACLR;

    // LED 1 is on by default
    P1OUT |= BIT1;

    __bis_SR_register(GIE);
}


#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    P1IFG &= ~BIT7;
    button1INTR = true;
}

#pragma vector=PORT2_VECTOR;
__interrupt void PORT2_ISR(void) {
    P2IFG &= ~BIT2;
    button2INTR = true;
}


#pragma vector=TIMER1_A0_VECTOR;
__interrupt void TIMER1_A0_ISR(void) {

    static int is_button1_pressed = 0;
    static int is_button2_pressed = 0;
    static int b1_debounce_total = 0;
	static int b2_debounce_total = 0;
    static int b1_debounce_pressed = 0;
	static int b2_debounce_pressed = 0;

    static bool isOn = false;

    static bool is_in_lpm = false;
    static bool need_to_switch = false;

    timer_counter++;
    if (button1INTR) {
		P1IE &= ~BIT7;
		b1_debounce_total++;
		b1_debounce_pressed += (P1IN & BIT7) == 0;

		if (b1_debounce_total == 10) {
			is_button1_pressed = b1_debounce_pressed > 7;
			b1_debounce_pressed = 0;
			b1_debounce_total = 0;

	    	need_to_switch = true;
			button1INTR = false;

			if(is_button1_pressed) P1IES &= ~BIT7;
			else P1IES |= BIT7;

			P1IE |= BIT7;	// Turn interruption off
		}
	}

    if (button2INTR) {
		P2IE &= ~BIT2;
		b2_debounce_total++;
		b2_debounce_pressed += (P2IN & BIT2) == 0;

		if (b2_debounce_total == 10) {
			is_button2_pressed = b2_debounce_pressed > 7;
			b2_debounce_pressed = 0;
			b2_debounce_total = 0;

			button2INTR = false;

			if(is_button2_pressed) P2IES &= ~BIT2;
			else P2IES |= BIT2;

			P2IE |= BIT2;	// Turn interruption off
		}
	}




    if(is_button1_pressed && need_to_switch && !is_in_lpm) {
    	is_in_lpm = true;
    	P1OUT &= ~BIT1;
    	P1OUT |= BIT3;
    	need_to_switch = false;
    	is_button1_pressed = false;
//    	__bis_SR_register(LPM0_bits | GIE);

    } else if (is_button1_pressed && need_to_switch && is_in_lpm){
    	is_in_lpm = false;
    	P1OUT &= ~BIT3;
    	P1OUT |= BIT1;
    	need_to_switch = false;
//    	__bis_SR_register_on_exit(LPM0_BITS);

    }

//    if (is_button2_pressed){
//    	is_in_lpm = false;
//    	P1OUT &= ~BIT3;
//    	P1OUT |= BIT1;
//    	__bis_SR_register_on_exit(LPM0_bits);
//    }



}


int main(void) {
    setup();

    __enable_interrupt();
    __low_power_mode_0();
    __no_operation();
}




