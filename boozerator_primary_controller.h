/*
 * boozerator_primary_controller.h
 *
 *  Created on: Jan 9th, 2014
 *      Author: Tom
 */

#ifndef BOOZERATOR_PRIMARY_CONTROLLER_H_
#define BOOZERATOR_PRIMARY_CONTROLLER_H_



#endif /* BOOZERATOR_PRIMARY_CONTROLLER_H_ */

#include "tshell.h"


#define FRIDGE_TX_REG UCA1TXBUF
#define FRIDGE_TX_DELAY 1000
#define PACKET_SEGMENT_DELAY 4000 // should be about 3.75ms
#define FRAMEBUF_SIZE 256

void onewire_test();
void poll_and_send_temps();

// upstream protocol
void send_temp_data_frame(uint16_t temp0, uint16_t temp1, uint16_t temp2, uint16_t temp3, uint16_t temp4, uint16_t temp5);

// fridge commands
void set_fridge_both_off();
void set_fridge_both_low();
void set_fridge_both_high();
void set_fridge_high_freezer_low();
void set_fridge_low_freezer_high();

// packet building blocks
void send_packet_header();
void send_packet_footer();
void send_segment_terminator();

// fridge interface helper functions
void fridge_char(char inchar);
void fridge_string(char* instring);

// sanity check functions
void blink1();
void blink2();
void check_button();
void fridge_test(int argc, char *argv[]);
