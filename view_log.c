/*
 * File:  view_log.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "timer0.h"
#include"ext_eeprom.h"
#include"i2c.h"
#include"ds1307.h"

char view_array[10][11], set=1;
int i1, j1;
extern int log;
int view_ind;
extern char main_f;
extern char menu_ind, arrow_st;
int time5, time6;

//logic for view log
void view_log(char key) 
{
    clcd_print("# TIME     EV SP", LINE1(0));
    //reading the value from the EEPROM and storing in an register
    if(set)
    {
        set=0;
        for (i1 = 0; i1 < log; i1++) 
        {
            for (j1 = 0; j1 < 10; j1++) 
            {
                view_array[i1][j1] = read_ext_eeprom(i1 * 10 + j1);
            }
        }
    }
    //to display the all the stored event in CLCD
    clcd_putch(view_ind + '0', LINE2(0));
    clcd_putch(' ', LINE2(1));
    clcd_putch(view_array[view_ind][0], LINE2(2));
    clcd_putch(view_array[view_ind][1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(view_array[view_ind][2], LINE2(5));
    clcd_putch(view_array[view_ind][3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(view_array[view_ind][4], LINE2(8));
    clcd_putch(view_array[view_ind][5], LINE2(9));
    clcd_putch(' ', LINE2(10));
    clcd_putch(view_array[view_ind][6], LINE2(11));
    clcd_putch(view_array[view_ind][7], LINE2(12));
    clcd_putch(' ', LINE2(13));
    clcd_putch(view_array[view_ind][8], LINE2(14));
    clcd_putch(view_array[view_ind][9], LINE2(15));
    key = read_switches(LEVEL_CHANGE);

    //check if switch 12 is pressed
    if (key == MK_SW12) 
    {
        time5++;
        //if long pressed go to menu by reseting all the flags
        if (time5 > 200) 
        {
            time5 = 0;
            menu_ind = 0;
            arrow_st = 0;
            main_f = MENU;
            set=1;
            view_ind = 0;
            CLEAR_DISP_SCREEN;
        }
    } 
    //if short pressed scroll up stored events
    else if (time5 < 200 && time5 != 0) 
    {
        time5 = 0;
        if (view_ind < log-1) 
        {
            view_ind++;
        }
    }
    //check if switch 12 is pressed
    if (key == MK_SW11) 
    {
        time6 = 2;
    } 
    //if short pressed scroll up stored events
    else if (time6 < 200 && time6 != 0) 
    {
        time6 = 0;
        if (view_ind > 0) 
        {
            view_ind--;
        }
    }
}