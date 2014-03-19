

#include <msp430.h>
#include <stdlib.h>
#include <string.h>
#include "tshell.h"
#include "fifo.h"
#include "boozerator_primary_controller.h"
#include "ishan.h"
#include "ds18x20.h"



int main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  //setup DCO to 8MHZ
  UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
  do
  {
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
										  // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
  UCSCTL2 |= 249;                           // Set DCO Multiplier for 8MHz
										  // (N + 1) * FLLRef = Fdco
										  // (249 + 1) * 32768 = 8MHz
  __bic_SR_register(SCG0);                  // Enable the FLL control loop
  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  __delay_cycles(250000);

  // Xbee UART initialization
  P3SEL = BIT3+BIT4;                        // P3.3,4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // CLK = ACLK
  UCA0BR0 = 0x80;                           // Set for 9600 baud
  UCA0BR1 = 0x03;                           //
  UCA0MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  // Fridge UART initialization
  P4SEL = BIT4+BIT5;                        // P4.4,5 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // CLK = ACLK
  UCA1BR0 = 0x80;                           // Set for 9600 baud
  UCA1BR1 = 0x03;                           //
  UCA1MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  // LED initialization
  P1DIR |= BIT0;                            // Set P1.0 (led1) to output direction
  P4DIR |= BIT7;                            // Set P4.7 (led2) to output direction

  // Button init
  //P2REN |= BIT1;							// enable pull-up on P2.1 (S2)

  // Timer A1 init
  TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
  TA1CCR0 = 0xFFFF;
  TA1CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR

  init_tShell();

  __bis_SR_register(GIE);       			// Interrupts enabled

  // Main loop
  while (1) {

	  if(fifo_isEmpty(&shell_rx_fifo)==0) {
		  process_char(fifo_get(&shell_rx_fifo));
	  }


  }

}

// Xbee UART interrupt vector
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
	  fifo_put(&shell_rx_fifo, UCA0RXBUF); 	// Put the character in rx_fifo
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}

// Fridge UART interrupt vector
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
	  //fifo_put(&shell_rx_fifo, UCA1RXBUF); 	// Put the character in rx_fifo
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}

// Timer0 A0 interrupt service routine
// Triggers every ~2 seconds for polling temp sensors
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
	timer_event_handler();

}
