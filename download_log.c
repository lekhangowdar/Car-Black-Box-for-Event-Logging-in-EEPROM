/*
 * File:  download_log.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "eeprom.h"
#include "matrix_keypad.h"
#include "timer0.h"
#include"ext_eeprom.h"
#include"i2c.h"
#include"ds1307.h"
#include "uart.h"

extern int log;
char d_log[10][14];
char Y;
int wait3;
extern unsigned char main_f, ev_ind;

void download_log() 
{
    //to perform the logic once
    if(wait3 == 0)
    {
        //store the download log event
        ev_ind = 9;
        store_event();
        clcd_print(" Log Downloaded ", LINE1(0));
        clcd_print("  Successfully  ", LINE2(0));
        puts("\n\n\r# Time     EV SP\n\r");
        
        //store the each event change continously till log times
        for (Y = 0; Y < log; Y++) 
        {
            //for storing log value
            d_log[Y][0] = Y + '0';
            d_log[Y][1] = ' ';
            //for storing time
            d_log[Y][2] = read_ext_eeprom((Y * 10) + 0);
            d_log[Y][3] = read_ext_eeprom((Y * 10) + 1);
            d_log[Y][4] = ':';
            d_log[Y][5] = read_ext_eeprom((Y * 10) + 2);
            d_log[Y][6] = read_ext_eeprom((Y * 10) + 3);
            d_log[Y][7] = ':';
            d_log[Y][8] = read_ext_eeprom((Y * 10) + 4);
            d_log[Y][9] = read_ext_eeprom((Y * 10) + 5);
            d_log[Y][10] = ' ';
            //for storing event value
            d_log[Y][11] = read_ext_eeprom((Y * 10) + 6);
            d_log[Y][12] = read_ext_eeprom((Y * 10) + 7);
            d_log[Y][13] = ' ';
            //for storing speed value
            d_log[Y][14] = read_ext_eeprom((Y * 10) + 8);
            d_log[Y][15] = read_ext_eeprom((Y * 10) + 9);
            puts(d_log[Y]);
            puts("\n\r");
        }
    }
    //after some delay go to menu by reseting all flags
    if(wait3++ == 450)
    {
        wait3 = 0;
        main_f = MENU;
        CLEAR_DISP_SCREEN;
    }
}