/*
 * File:  change_password.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "ext_eeprom.h"

unsigned int pa[5], repa[5], same = 0, i, re_flag = 0;
extern char main_f;
extern unsigned char ev_ind;
unsigned int count1,go_back_f,blink_delay;
unsigned char blink_state;

void change_pass(char key) 
{
    //once the password is entered or 3 attempts is failed
    //go back to menu by reseting all flags
    if(go_back_f == 1)
    {
        if (i++ == 800)
        {
            //store the change password event
            ev_ind = 12;
            store_event();
            main_f = MENU;
            count1 = 0;
            re_flag = 0;
            go_back_f = 0;
        }
    }
    //to enter the new password for first time
    else if (re_flag == 0 )  
    {
        if (count1 < 4) 
        {
            go_back_f = 0;
            clcd_print("ENTER NEW PASSWD", LINE1(0));
            if (key == MK_SW11) 
            {
                pa[count1] = '0';
                clcd_putch('*', LINE2(count1++));
            }
            if (key == MK_SW12) 
            {
                pa[count1] = '1';
                clcd_putch('*', LINE2(count1++));
            }
        }
        //once password is entered set the re_flag 
        else if (count1 == 4) 
        {
            re_flag = 1;
            CLEAR_DISP_SCREEN;
            clcd_print("RE-ENTER PASSWD ", LINE1(0));
            count1 = 0;
        }
    }
    //to re-enter the new password for confirmation
    else if (re_flag == 1 ) 
    {
        if (count1 < 4) 
        {
            if (key == MK_SW11) 
            {
                repa[count1] = '0';
                clcd_putch('*', LINE2(count1++));
            }
            if (key == MK_SW12) 
            {
                repa[count1] = '1';
                clcd_putch('*', LINE2(count1++));
            }
        }
        if (count1 == 4 && same == 0) 
        {
            //to compare both the passwords entered and reentered password
            for (i = 0; i < 4; i++) 
            {
                if (repa[i] != pa[i]) 
                {
                    same = 1; //set the same if password mismatched
                    break;
                }
            }
            //if password not matched print the message and set/reset respective flags
            if (same == 1) 
            {
                CLEAR_DISP_SCREEN;
                clcd_print("PSWD NOT MATCHED", LINE2(0));
                re_flag = 0;
                same = 0;
                go_back_f = 1;
            } 
            //if password is matched print successful
            else 
            {
                clcd_print("PASSWORD UPDATE ", LINE1(0));
                clcd_print("SUCCESSFUL      ", LINE2(0));
                write_ext_eeprom(200, repa[0]);  //store the password in external EEPROM
                write_ext_eeprom(201, repa[1]);
                write_ext_eeprom(202, repa[2]);
                write_ext_eeprom(203, repa[3]);
                go_back_f = 1;
                count1 = 0;
            }
        }
    }
    //logic to blink the '_' 
    if(count1 < 4 && go_back_f == 0)
        if (blink_delay++ > 200)  //adjust this value for blinking speed
        {
            blink_delay = 0;
            blink_state = !blink_state; // toggle the blink state
           
            if (blink_state)
            {
                clcd_print("_               ", LINE2(count1)); // Display the underscore
            }
            else
            {
                clcd_print("                ", LINE2(count1)); // Clear the underscore
            }
        }
}