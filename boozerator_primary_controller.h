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
#define FRIDGE_TX_DELAY 12000
#define PACKET_SEGMENT_DELAY 28000 // should be about 3.75ms
#define FRAMEBUF_SIZE 256

// map the thermometers
#define FREEZER_TEMP temp1
#define FRIDGE_TEMP temp0

// Set points
#define FRIDGE_SET_POINT 53.0
#define FRIDGE_TOLERANCE 5.0
#define FREEZER_SET_POINT 40.0
#define FREEZER_TOLERANCE 5.0

// Control fucntions
void timer_event_handler();
void poll_and_send_temps();
int control_freezer();
int control_fridge();

// upstream protocol
void send_temp_data_frame(uint16_t temp0, uint16_t temp1, uint16_t temp2, uint16_t temp3, uint16_t temp4, uint16_t temp5);
void send_fridge_state_frame(int fridge_state, int freezer_state);

// fan commands
void fan_off();
void fan_on();

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
