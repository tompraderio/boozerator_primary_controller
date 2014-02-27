//	---------------------------------------------------------------
//	tShell
//
//	A tiny, poorly implemented shell for microprocessors
//	but hey, it does its thang
//
//	Created on: Sep 26, 2012
//	Author: Tom Praderio
//
//	Revision History:
//	1.0.0 - 9/28/12 - IT LIVES!
//	1.0.1 - 9/29/12 - Backspace support added
//	1.0.2 - 9/30/12 - Added RX FIFO buffer
//	1.0.3 - 10/10/12 - Removed all DMA calls
//  1.0.4 - 1/30/14 - Added print_line() and cleaned up a little
//
//	---------------------------------------------------------------
//
//	SET-UP INSTRUCTIONS:
//	1) Add tshell.c and tshell.h to your project
//	2) Include tshell.h in your main source file
//	3) Call init_tShell() in your main function
//	4) Call process_char(<your UART rx register>) in your UART rx interrupt handler
//	5) Add your UART's tx register to the UART_TX_REG definition in the header
//	6) ???
//	7) PROFIT
//
//	ADDING YOUR OWN FUNCTIONS:
//	1) Include your header file below
//	2) Add a rung on the if-else ladder for your function's name in function_handler()
//	   under the custom functions section


#include <msp430.h>
#include <string.h>
#include <stdlib.h>
#include "tshell.h"
#include "fifo.h"
#include "boozerator_primary_controller.h"
#include "ishan.h"
#include "ds18x20.h"

FIFO shell_rx_fifo;
uint8_t shell_rx_fifo_buf[SHELL_RX_FIFO_SIZE];

unsigned char inputbuf[BUF_SIZE]; 		// Global input character buffer
unsigned char validbuf[BUF_SIZE];		// Buffer for command parsing
char *argv[MAX_ARG_COUNT];				// Parsed Arguments
unsigned int bufindex = 0;		// Input buffer index
unsigned int arrow_char = 0; 	// Arrow key identifier

// Clears the screen, displays the fancy header
// Call this in main
void init_tShell () {
	int x;

	// Clear the screen
	for(x=0;x<SCREEN_HEIGHT;x++) {
		print_char(0x0D); //CR
		print_char(0x0A); //LF
	}

	// Init shell rx fifo
	fifo_init(&shell_rx_fifo, SHELL_RX_FIFO_SIZE, shell_rx_fifo_buf);
	if(fifo_status(&shell_rx_fifo)!=FIFO_GOOD){
		print_string("Shell RX fifo not initialized. Check for memory problems");
		print_char(0x0D); //CR
		print_char(0x0A); //LF
	}

	//Print header
	print_string("Welcome to tShell!");
	print_char(0x0D); //CR
	print_char(0x0A); //LF
	print_string("Version 1.0.4 January 30th, 2014");
	print_char(0x0D); //CR
	print_char(0x0A); //LF
	print_string("Written by Tom Praderio");
	print_char(0x0D); //CR
	print_char(0x0A); //LF
	print_char(0x0D); //CR
	print_char(0x0A); //LF

	print_prompt();
}

// Takes characters in from the UART register and adds them to a buffer
// Call this in the UART interrupt handler
void process_char (unsigned char inchar) {
	// Check to make sure the last character wasn't the first of an arrow key
	if(!arrow_char) { // No arrow key identifier
		// first check for special characters
		switch(inchar) {
			case 0x0D: // enter (CR) is received
				if(inputbuf[0] != 0x00 && inputbuf[0] != ' ' && inputbuf[0] != '\0') { 	// Check if the buffer is valid
					scan_buf();
				}
				bufindex = 0;
				inputbuf[0] = 0x00; //Mark buffer as null
				print_prompt();
				break;
			case 0x08: // Backspace
				if(bufindex > 0) {
					bufindex--;
					inputbuf[bufindex]=NULL;
					print_char(0x08);
					print_char(' ');
					print_char(0x08);
				}
				break;
			case 0x1B: // Arrow keys
				arrow_char = 1; //set the flag
				__delay_cycles(1000);
				break;
			default: // if no special characters,
				inputbuf[bufindex] = inchar; //add the new char to the buffer
				bufindex++;
				print_char(inchar);
		}
	}
	else { // If the last char was an arrow identifier, check the next char
		arrow_char = 0; //reset the flag
		switch(inchar) {
			case 0x41:
				print_string("up");
				break;
			case 0x42:
				print_string("down");
				break;
			case 0x43:
				print_string("left");
				break;
			case 0x44:
				print_string("right");
				break;
			default:
				break;
		}
	}

}

// Scans the input buffer and parses the arguments
void scan_buf() {
	int x;
	int argc = 1;
	int size = bufindex;
	char* pch;

	// Copy the valid portion of the input buffer to validbuf
	for(x=0; x<size; x++) {
		validbuf[x] = inputbuf[x];
	}
	validbuf[size] = '\0';				// and terminate

	// Loop through the buffer and count the arguments
	for(x=0; x<size; x++) {
		if(validbuf[x]==' '){
			argc++;
		}
	}

	// parse the input buffer and populate argv
	int count = 0;
	pch = strtok(validbuf," ");
	while (pch != NULL)
	  {
		argv[count] = pch;
	    count++;
	    pch = strtok (NULL, " ");
	  }

	// Pass the arguments off to the function handler
	function_handler(argc, argv);

}

// Determines the intended function from argv[0] and calls it
// Put your functions in here
void function_handler (int argc, char *argv[]) {

	// Built-in functions
	if(strcmp(argv[0], "hello")==0){
		print_line("Why, top o' the morning to you, chap!");
	}
	else if (strcmp(argv[0], "print_char")==0) {
		print_char(0x0D); //CR
		print_char(0x0A); //LF
		print_char(argv[1][0]);
	}
	else if (strcmp(argv[0], "cls")==0) {
		int x;
		for(x=0;x<SCREEN_HEIGHT;x++) {
				print_char(0x0D); //CR
				print_char(0x0A); //LF
			}
	}

	// Custom functions

	else if (strcmp(argv[0], "blink1")==0){
		blink1();
	}
	else if (strcmp(argv[0], "blink2")==0){
		blink2();
	}
	else if (strcmp(argv[0], "checkb")==0){
		check_button();
	}
	else if (strcmp(argv[0], "fridge")==0){
		fridge_test(argc, argv);
	}
	else if (strcmp(argv[0], "all_off")==0){
		set_fridge_both_off();
	}
	else if (strcmp(argv[0], "all_low")==0){
		set_fridge_both_low();
	}
	else if (strcmp(argv[0], "all_high")==0){
		set_fridge_both_high();
	}
	else if (strcmp(argv[0], "high_low")==0){
		set_fridge_high_freezer_low();
	}
	else if (strcmp(argv[0], "low_high")==0){
		set_fridge_low_freezer_high();
	}
	else if (strcmp(argv[0], "w")==0){
		onewire_test();
	}



	// End custom functions
	else {
		print_char(0x0D); //CR
		print_char(0x0A); //LF
		print_string("tshell: No such function. WHAT DO?");
	}



}

void print_prompt () {
	print_char(0x0D); //CR
	print_char(0x0A); //LF
	char shell[] = "tShell>";
	print_string(shell);
}

void print_char(char inchar) {
	__delay_cycles(tx_delay); // delay for the buffer
	UART_TX_REG = inchar;
}

void print_string(char* instring) {
	int x;
	for(x=0; x<strlen(instring); x++) {
		print_char(instring[x]);
	}

}

void print_line(char* instring) {
	print_char(0x0D); //CR
	print_char(0x0A); //LF
	print_string(instring);
	print_char(0x0D); //CR
	print_char(0x0A); //LF

}



