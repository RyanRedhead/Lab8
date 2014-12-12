//-----------------------------------------------------------------
// Name:
// File:	main.c
// Date:	Fall 2014
// Purp:
//-----------------------------------------------------------------
#include "msp430g2553.h"
//#include "ADC10.h"

void initMSP430();

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {

	unsigned short LeftSample, CenterSample, RightSample;
	unsigned int count = 0;

	IFG1=0; 													// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;

	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	P2DIR |= BIT0;
	P2OUT |= BIT0;

	P2DIR |= BIT1;
	P2OUT &= ~BIT1;

	P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
	P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

	P2DIR |= BIT3;
	P2OUT &= ~BIT3;

	P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
	P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
	TA1CCR0 = 0x0100;						// set signal period


	while(1) {

//		LeftSample = readvalue(2);
//		CenterSample = readvalue(3);
//		RightSample = readvalue(4);

		ADC10CTL0 = 0;											//P1.3
		ADC10CTL1 = INCH_3 | ADC10DIV_3 ;
		ADC10AE0 = BIT3;
		ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;

		ADC10CTL0 |= ADC10SC;									// Start a conversion
		while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
		CenterSample = ADC10MEM;									// collect that 10-bit value

		if (CenterSample < 0x210){
	    	P2OUT |= BIT0;
	        P2OUT &= ~BIT1;
	        TA1CCTL1 = OUTMOD_7;
	        TA1CCR1 = 0x0080;


	        P2OUT &= ~BIT3;
	        TA1CCTL2 = OUTMOD_3;
	        TA1CCR2 = 0x0030;
		}

		else if (CenterSample > 0x210 && count == 0){

			P2OUT |= BIT0;
	        P2OUT |= BIT1;
	        TA1CCTL1 = OUTMOD_7;
	        TA1CCR1 = 0x0070;


	        P2OUT &= ~BIT3;
	        TA1CCTL2 = OUTMOD_3;
	        TA1CCR2 = 0x0030;

	        count++;

	        __delay_cycles(10000);
		}

/*		else if (CenterSample > 0x210){

		 if (LeftSample > 0x200 && RightSample < 0x200){
			P2OUT &= ~BIT0;

			__delay_cycles(10000);

			P2OUT |= BIT0;
			P2OUT &= ~BIT1;
		    TA1CCR1 = 0x0000;
		    TA1CCTL1 = OUTMOD_7;

			P2OUT &= ~BIT3;
			TA1CCTL2 = OUTMOD_7;
			TA1CCR2 = 0x0060;
			}
		}
		 else if (RightSample > 0x150 && LeftSample < 0x150){

			 P2OUT &= ~BIT0;

			__delay_cycles(10000);

			P2OUT |= BIT0;
		    P2OUT &= ~BIT1;
		    TA1CCR1 = 0x0060;
		    TA1CCTL1 = OUTMOD_7;

		    P2OUT &= ~BIT3;
		    TA1CCTL2 = OUTMOD_7;
		    TA1CCR2 = 0x0000;
			}
		}
*/
	} // end infinite loop

} // end main

