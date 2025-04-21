/*
 * File:  set_time.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "ds1307.h"

unsigned char in_flag, tc_flag, timeout, hr, min, secs, field;
unsigned int wait, blink, time3, time4;
extern char main_f, menu_f;
extern unsigned char time[9];
extern unsigned char ev_ind;

    //logic for set time
void set_time(char menu_key)
{
    //check if time is changed
    if(tc_flag == 1)
    {
        //display the message
        clcd_print("Time changed",LINE1(0));
        clcd_print("successfully",LINE2(0));
        in_flag=0;
        //after some delay go to menu by reseting all flags 
        if(timeout == 0)
        {
            CLEAR_DISP_SCREEN;
            in_flag = 0;
            tc_flag = 0;
            main_f = MENU;
        }
    }
    //check if time is not changed
    else if(tc_flag == 2)
    {
        //display the message
        clcd_print("    Time not    ",LINE1(0));
        clcd_print("    changed     ",LINE2(0));
        in_flag=0;
        //after some delay go to menu by reseting all flags 
        if(timeout == 0)
        {
            CLEAR_DISP_SCREEN;
            main_f = MENU;
            tc_flag = 0;
        }
    }
    //logic to change time and update it
    else
    {
        clcd_print("HH:MM:SS",LINE1(3));
        //display the present time by storing it in variables 
        if(in_flag == 0)
        {
            hr=(time[0]-48)*10 + time[1]-48;
            min=(time[3]-48)*10 + time[4]-48;
            secs=(time[6]-48)*10 + time[7]-48;
            in_flag=1;
            //store the set time event
            ev_ind = 11;
            store_event();
        }
        //logic to display sec field is selected
        if(field == 0)
        {
            clcd_putch(hr%10+48,LINE2(4));
            clcd_putch(hr/10+48,LINE2(3));
            clcd_putch(':',LINE2(5));
            clcd_putch(min%10+48,LINE2(7));
            clcd_putch(min/10+48,LINE2(6));
            clcd_putch(':',LINE2(8));
            if(blink)
            {
                clcd_putch(secs%10+48,LINE2(10));
                clcd_putch(secs/10+48,LINE2(9));
            }
            else
            {
                clcd_print("  ",LINE2(9));
            }
        }
        //logic to display minute field is selected
        else if(field == 1)
        {
            clcd_putch(hr%10+48,LINE2(4));
            clcd_putch(hr/10+48,LINE2(3));
            clcd_putch(':',LINE2(5));
            clcd_putch(secs%10+48,LINE2(10));
            clcd_putch(secs/10+48,LINE2(9));

            clcd_putch(':',LINE2(8));
            if(blink)
            {
                clcd_putch(min%10+48,LINE2(7));
                clcd_putch(min/10+48,LINE2(6));
            }
            else
            {
                clcd_print("  ",LINE2(6));
            }
        }
        //logic to display hour field is selected
        else if(field == 2)
        {
            clcd_putch(secs%10+48,LINE2(10));
            clcd_putch(secs/10+48,LINE2(9));
            clcd_putch(':',LINE2(5));
            clcd_putch(min%10+48,LINE2(7));
            clcd_putch(min/10+48,LINE2(6));
            clcd_putch(':',LINE2(8));
            if(blink)
            {
                clcd_putch(hr%10+48,LINE2(4));
                clcd_putch(hr/10+48,LINE2(3));
            }
            else
            {
                clcd_print("  ",LINE2(3));
            }
        }
        //check if switch 11 is pressed
        if(menu_key == MK_SW11)
        {
            time3++;    /*increment the time if the key is pressed*/
            if(time3 > 200)  /*check for long press*/
            {
                time3 = 0;
                //store the all the time in respective register
                unsigned char hr_reg = ((hr/10)&0xF)<<4 | (hr%10)&0xF;
                unsigned char min_reg = ((min/10)&0xF)<<4 | (min%10)&0xF;
                unsigned char sec_reg = ((secs/10)&0xF)<<4 | (secs%10)&0xF;
                
                //write the time in RTC hour, minute and second register
                write_ds1307(HOUR_ADDR, hr_reg);
                write_ds1307(MIN_ADDR, min_reg);
                write_ds1307(SEC_ADDR, sec_reg);
                timeout = 3;
                tc_flag = 1;
                CLEAR_DISP_SCREEN;
            }
        }
        //change the field
        else if(time3 < 200 && time3 != 0) /*check for short press*/
        {
            field=(field+1)%3;
            time3 = 0;
        }
        //check if switch 11 is pressed
        if(menu_key == MK_SW12)
        {
            time4++;    /*increment the time if the key is pressed*/
            if(time4 > 200)/*check for long press*/
            {
                time4 = 0;
                timeout = 3;
                tc_flag=2;
            }
        }
        //increment the respective time values of selected field
        else if(time4 < 200 && time4 != 0) /*check for short press*/
        {
            if(field == 0)
                secs=(secs+1)%60;
            else if(field == 1)
                min=(min+1)%60;
            else if(field==2)
                hr=(hr+1)%24;
            time4 = 0;
        }
    }
    //logic to blink the field
    if(wait++ == 150)
    {
        wait=0;
        blink=~blink;
        timeout--;
    }
}