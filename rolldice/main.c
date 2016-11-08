/**
 This code gets a randomic number when P1.3 was clicked

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

#define BTN BIT3
/*
 * It is the limit of drawn number,
 * you can change to any other number supported by TAR
 */
#define LIMIT 8

unsigned int timer_count = 0;

int main(void) {
  WDTCTL = WDTPW + WDTHOLD; // disable WDT
  P1IE |= BTN; // P1.3 interrupt enabled
  P1IFG &= ~BTN; // P1.3 IFG cleared
  P1REN |= BTN; // Enable resistor on BTN

  CCTL0 = CCIE;
  TACTL = TASSEL_2 + MC_2;
  _BIS_SR(LPM4_bits+GIE);

  __enable_interrupt(); // enable all interrupts

  return 0;
}

/*
 * Syntax of interruptions used in code composer
 */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	timer_count = (TAR % LIMIT); // found a value between 0 to LIMIT
	/*
	 * According documentation, if this flags are not setted to 0,
	 * the interrupts stay peding.
	 */
 	P1IFG &= ~BTN; // P1.3 IFG cleared
}
