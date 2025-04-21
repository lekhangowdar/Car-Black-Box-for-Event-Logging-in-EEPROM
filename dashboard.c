/*
 * File:  dashboard.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"

extern unsigned char speed, sp1, sp2, time[9];
extern unsigned char ev[][3], ev_ind;
extern char main_f, key, wait_f;
extern char menu_ind, arrow_st;

void dashboard()
{
    //dashboard display logic
    clcd_print("  TIME    EV  SP", LINE1(0));
    clcd_print(time, LINE2(0));        //for time
    clcd_print(ev[ev_ind], LINE2(10)); //for event
    clcd_putch(sp1, LINE2(14));        //for speed tens place digit
    clcd_putch(sp2, LINE2(15));        //for speed unit place digit
    
    //check if key 11 is pressed
    if(key == MK_SW11)
    {
        CLEAR_DISP_SCREEN;
        menu_ind = 0;
        arrow_st = 0;
        main_f = PASSWORD; //enter into the password logic
    }
}


