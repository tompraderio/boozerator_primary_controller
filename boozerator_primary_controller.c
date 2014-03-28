/*
 * boozerator_primary_controller.c
 *
 *  Created on: Jan 9th, 2014
 *      Author: Tom
 */
#include <msp430.h>
#include <string.h>
#include <stdlib.h>
#include "tshell.h"
#include "fifo.h"
#include "boozerator_primary_controller.h"
#include "ishan.h"
#include "ds18x20.h"


unsigned char tx_framebuf[FRAMEBUF_SIZE];

uint16_t temp0 = 0;
uint16_t temp1 = 0;
uint16_t temp2 = 0;
uint16_t temp3 = 0;
uint16_t temp4 = 0;
uint16_t temp5 = 0;

// map the thermometers
#define FREEZER_TEMP temp1
#define FRIDGE_TEMP temp0

int polling_counter = 0;
int fridge_comm_counter = 0;

void onewire_test() {

	uint16_t result1 = 0;
	uint8_t char1 = 0;
	uint8_t char2 = 0;

	result1 = GetData_TEMP0();

	char1 = result1;
	char2 = result1 >> 8;

	print_char((char)(result1));
	print_char((char)(result1 >> 8));
	print_line("Done");


}

// Fires every 1.5 ~ 2 seconds
void timer_event_handler() {
	int fridge_state = 0;
	int freezer_state = 1;

	// Poll temps every 6 seconds or so
	if (polling_counter >= 2) {
		polling_counter = 0;
		poll_and_send_temps();
	}
	else {
		polling_counter++;
	}

	// Control the freezer and fridge every 1.5 minutes or so
	if (fridge_comm_counter >= 31) {
		fridge_comm_counter = 0;
		freezer_state = control_freezer();
		fridge_state = control_fridge();
		send_fridge_state_frame(fridge_state,freezer_state);
	}
	else {
		fridge_comm_counter++;
	}
}

// grab all the temps and send them up
void poll_and_send_temps() {
	P1OUT |= BIT0; // turn LED1 on to indicate polling
	temp0 = GetData_TEMP0();
	temp1 = GetData_TEMP1();
	temp2 = GetData_TEMP2();
	temp3 = GetData_TEMP3();
	temp4 = GetData_TEMP4();
	temp5 = GetData_TEMP5();
	send_temp_data_frame(temp0, temp1, temp2, temp3, temp4, temp5);
	P1OUT &= ~BIT0; // polling finished
}

// Control algorithm for the freezer side temperature
int control_freezer() {
	int16_t freezer_signed_temp = 0;
	float freezer_temp_f = 0.0;

	// convert the current freezer temp to signed F
	freezer_signed_temp = FREEZER_TEMP;
	freezer_temp_f = (freezer_signed_temp*0.0625)*1.8 + 32.0;

	// control the freezer to between 38F and 43F
	if (freezer_temp_f < 38.0) {
		// freezer off
		set_fridge_both_off();
		P4OUT &= ~BIT7; // green led off
		return 0;
	}
	else if (freezer_temp_f > 43.0) {
		// freezer on
		set_fridge_high_freezer_low();
		P4OUT |= BIT7; // green led on
		return 1;
	}

}

int control_fridge() {
	int16_t fridge_signed_temp = 0;
	float fridge_temp_f = 0.0;

	// convert the current fridge temp to signed F
	fridge_signed_temp = FREEZER_TEMP;
	fridge_temp_f = (fridge_signed_temp*0.0625)*1.8 + 32.0;

	// control the fridge temp
	if (fridge_temp_f < 48.0) {
		// fridge fan off
		fan_off();
		return 0;
	}
	else if (fridge_temp_f > 53.0) {
		// fridge fan on
		fan_on();
		return 1;
	}
}

// ===================================================================================================================
// Upstream protocol
// ===================================================================================================================

// Sends a temperature data frame out
void send_temp_data_frame(uint16_t temp0, uint16_t temp1, uint16_t temp2, uint16_t temp3, uint16_t temp4, uint16_t temp5) {
	unsigned int i=0;
	unsigned int size = 17; // 4 header + 12 data + 1 checksum
	unsigned char checksum = 0;

	tx_framebuf[0] = 0x55;	// Start Delimiter
	tx_framebuf[1] = 0x55;	// Start Delimiter
	tx_framebuf[2] = size;	// Size of frame
	tx_framebuf[3] = 0x01;	// Temp data type
	tx_framebuf[4] = (char)(temp0 >> 8); // temp1 upper byte
	tx_framebuf[5] = (char)(temp0); 	 // temp1 lower byte
	tx_framebuf[6] = (char)(temp1 >> 8); // temp2 upper byte
	tx_framebuf[7] = (char)(temp1);		 // temp2 lower byte
	tx_framebuf[8] = (char)(temp2 >> 8); // temp1 upper byte
	tx_framebuf[9] = (char)(temp2); 	 // temp1 lower byte
	tx_framebuf[10] = (char)(temp3 >> 8); // temp2 upper byte
	tx_framebuf[11] = (char)(temp3);		 // temp2 lower byte
	tx_framebuf[12] = (char)(temp4 >> 8); // temp1 upper byte
	tx_framebuf[13] = (char)(temp4); 	 // temp1 lower byte
	tx_framebuf[14] = (char)(temp5 >> 8); // temp2 upper byte
	tx_framebuf[15] = (char)(temp5);		 // temp2 lower byte

	// calculate the checksum
	checksum = 0;
	for(i=0; i<size-1; i++) {
		checksum += tx_framebuf[i];	// Calculate checksum
	}
	tx_framebuf[size-1] = 0xFF - checksum; // Append frame with checksum
	tx_framebuf[size] = '\0';	// Terminate

	// Now send the assembled frame out to the uart
	print_string_no_term(tx_framebuf, size);
}

// Sends a fridge state data frame out
void send_fridge_state_frame(int fridge_state, int freezer_state) {
	unsigned int i=0;
	unsigned int size = 7; // 4 header + 2 data + 1 checksum
	unsigned char checksum = 0;

	tx_framebuf[0] = 0x55;	// Start Delimiter
	tx_framebuf[1] = 0x55;	// Start Delimiter
	tx_framebuf[2] = size;	// Size of frame
	tx_framebuf[3] = 0x02;	// Fridge state type
	tx_framebuf[4] = (char)(fridge_state); // temp1 upper byte
	tx_framebuf[5] = (char)(freezer_state); 	 // temp1 lower byte

	// calculate the checksum
	checksum = 0;
	for(i=0; i<size-1; i++) {
		checksum += tx_framebuf[i];	// Calculate checksum
	}
	tx_framebuf[size-1] = 0xFF - checksum; // Append frame with checksum
	tx_framebuf[size] = '\0';	// Terminate

	// Now send the assembled frame out to the uart
	print_string_no_term(tx_framebuf, size);
}


// ===================================================================================================================
// Fan commands
// Controls the fan to regulate the fridge side temperature
// ===================================================================================================================
void fan_off() {
	P6OUT &= ~BIT6;
}

void fan_on() {
	P6OUT |= BIT6;
}



// ===================================================================================================================
// Fridge commands
// Controls the compressor to regulate the freezer side temperature
// ===================================================================================================================
void set_fridge_both_off() {
	send_packet_header(); // send the standard header
	fridge_char(0x64); // fridge = 100 (off)
	send_segment_terminator();
	fridge_char(0x64); // freezer = 100 (off)
	send_segment_terminator();
	fridge_char(0x00); // spacer segment
	send_segment_terminator();
	fridge_char(0x8A); // first "checksum" = 138
	send_segment_terminator();
	fridge_char(0x5C); // second "checksum" = 92
	send_segment_terminator();
	send_packet_footer(); // send footer

}

void set_fridge_both_low() {
	send_packet_header(); // send the standard header
	fridge_char(0x01); // fridge = 1
	send_segment_terminator();
	fridge_char(0x01); // freezer = 1
	send_segment_terminator();
	fridge_char(0x00); // spacer segment
	send_segment_terminator();
	fridge_char(0x0E); // first "checksum" = 14
	send_segment_terminator();
	fridge_char(0x18); // second "checksum" = 24
	send_segment_terminator();
	send_packet_footer(); // send footer

}

void set_fridge_both_high() {
	send_packet_header(); // send the standard header
	fridge_char(0x09); // fridge = 9
	send_segment_terminator();
	fridge_char(0x09); // freezer = 9
	send_segment_terminator();
	fridge_char(0x00); // spacer segment
	send_segment_terminator();
	fridge_char(0x2E); // first "checksum" = 46
	send_segment_terminator();
	fridge_char(0x10); // second "checksum" = 16
	send_segment_terminator();
	send_packet_footer(); // send footer

}

void set_fridge_high_freezer_low() {
	send_packet_header(); // send the standard header
	fridge_char(0x09); // fridge = 9
	send_segment_terminator();
	fridge_char(0x01); // freezer = 1
	send_segment_terminator();
	fridge_char(0x00); // spacer segment
	send_segment_terminator();
	fridge_char(0xA7); // first "checksum" = 167
	send_segment_terminator();
	fridge_char(0xB9); // second "checksum" = 185
	send_segment_terminator();
	send_packet_footer(); // send footer

}

void set_fridge_low_freezer_high() {
	send_packet_header(); // send the standard header
	fridge_char(0x01); // fridge = 1
	send_segment_terminator();
	fridge_char(0x09); // freezer = 9
	send_segment_terminator();
	fridge_char(0x00); // spacer segment
	send_segment_terminator();
	fridge_char(0x87); // first "checksum" = 135
	send_segment_terminator();
	fridge_char(0xB1); // second "checksum" = 177
	send_segment_terminator();
	send_packet_footer(); // send footer

}


// ===================================================================================================================
// packet building blocks
// ===================================================================================================================
void send_packet_header() {
	// standard packet header structure is (decimal):
	// 2
	// 16 6
	// \n 6
	// 165 6
	fridge_char(0x02); // 2
	__delay_cycles(PACKET_SEGMENT_DELAY); // delay 3.75ms between segments, no 6 termination here
	fridge_char(0x10); // 16
	send_segment_terminator();
	fridge_char(0x0A); // \n
	send_segment_terminator();
	fridge_char(0xA5); // 165
	send_segment_terminator();
}

void send_packet_footer() {
	// standard packet footer structure (decimal):
	// 3 6 6
	fridge_char(0x03); // 3
	fridge_char(0x06); // 6
	send_segment_terminator();
}

void send_segment_terminator() {
	fridge_char(0x06);
	__delay_cycles(PACKET_SEGMENT_DELAY); // delay 3.75ms between segments
}



// ===================================================================================================================
// Helper functions
// ===================================================================================================================
void fridge_char(char inchar) {
	__delay_cycles(FRIDGE_TX_DELAY); // delay for the buffer
	FRIDGE_TX_REG = inchar;
}

void fridge_string(char* instring) {
	int x;
		for(x=0; x<strlen(instring); x++) {
			fridge_char(instring[x]);
		}
}


// ===================================================================================================================
// sanity checks
// ===================================================================================================================
void blink1() {
	P1OUT ^= BIT0;                          // Toggle P1.0 using exclusive-OR
	print_line("Blinked LED1");

}

void blink2() {
	P4OUT ^= BIT7;                          // Toggle P4.7 using exclusive-OR
	print_line("Blinked LED2");

}

void check_button() {
	if ((BIT1 & P2IN)) {// active low switch
		print_line("button is not pushed");
	}
	else {
		print_line("button is pushed");
	}
}

void fridge_test(int argc, char *argv[]) {
	fridge_string(argv[1]);
}
