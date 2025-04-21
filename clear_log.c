/*
 * File:  clear_log.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ext_eeprom.h"

extern int log;
int wait1;
extern char main_f;
extern unsigned char ev_ind;

void clear_log(char key)
{
    //logic for clear
    if(wait1 == 0) 
    {
        //update log as 0 for clearing
        log = 0;  
        write_ext_eeprom(250,log);
        //store the clear log event
        ev_ind = 10;
        store_event();
        clcd_print("   LOG CLEARED  ",LINE1(0));
        clcd_print("  SUCCESSFULLY  ",LINE2(0));
    }
    //after some delay go to menu by reseting all flags 
    if(wait1++ == 450)
    {
        wait1 = 0;
        main_f = MENU;
        CLEAR_DISP_SCREEN;
    }
}
