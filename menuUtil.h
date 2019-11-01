#ifndef MENUUTIL_H
#define MENUUTIL_H

/**
   contains implementations of all the functions
   related to printing the menu, reading user input, etc.
   */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Print all menu options to the user
 */
void print_menu();

/**
 * Clears screen
 */

/**
 * calls to appropirate function based on user imnput
 * and prints out message indicating outcome
 */
void choice();

/**
 *prints out appropriate error message
 */
void error();

/**
 *deals with user inputs
 */

void input_r();

void input_w();

void input_br();

void input_c();

void input_cn();

void input_bl();

void input_sh();

void input_e();

#endif
