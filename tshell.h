/*
 * tshell.h
 * v1.0.2
 *
 *  Created on: Sep 26, 2012
 *      Author: Tom
 */

#ifndef TSHELL_H_
#define TSHELL_H_

#endif

#include "fifo.h"

#define UART_TX_REG UCA0TXBUF
#define SCREEN_HEIGHT 30
#define BUF_SIZE 64
#define MAX_ARG_COUNT 8
#define MAX_ARG_LENGTH 20
#define SHELL_RX_FIFO_SIZE 4
#define tx_delay 20000

//extern FIFO rx_fifo;
//extern uint8_t* fifo_buf;

extern FIFO shell_rx_fifo;
extern uint8_t shell_rx_fifo_buf[SHELL_RX_FIFO_SIZE];

extern unsigned char inputbuf[BUF_SIZE];
extern unsigned int bufindex;
extern unsigned int arrow_char;

void init_tShell ();
void process_char (unsigned char inchar);
void print_prompt ();
void print_char(char inchar);
void print_string(char* instring);
void print_line(char* instring);
void scan_buf();
void function_handler (int argc, char *argv[]);
