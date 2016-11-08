/*
 Simulating a timer counting seconds. This code could be done using timer
 interrupting too.

 Copyright (c) <2016> <Phelipe Wener>
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */
#include <msp430g2553.h>

#define LED0 BIT0

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // Clocking counting until 1x10^6, but timer can only count up to 2^16=65536
    BCSCTL1 = CALBC1_1MHZ;
    // Choosing frequency of digital controlled oscillator
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= LED0;
	P1OUT &= ~LED0;

	/**
	 * We can solve the above problem setting IDx to 3 that will divide clock to 8:
	 * 1x10^6 / 8 = 125x10^3
	 * That number are the double of 62500. What means that we can count every 0.5 seconds.
	 * To have 1 second we can enable the up/down mode, setting MC to 3, counting two times
	 */
	TACCR0 = 62499; // Effectively it counts 62500
	TACTL = TASSEL_2; // SMCLK choosing DCO has counter
	TACTL += ID_3; // Input divider
	TACTL += MC_3; // Up mode, counts up to TACCR0

	while(1) {
		while((TACTL & TAIFG) == 0);
		P1OUT ^= LED0;
		TACTL &= ~TAIFG; // clear interrupt
	}
}
