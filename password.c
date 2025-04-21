/*
 * File:  password.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "timer0.h"
#include "ext_eeprom.h"

extern unsigned char main_f,arrow_st;

unsigned char pass[]="1111";  //initialize an array with the correct password 
unsigned char input_pass[4];       //duplicate array to compare entered password
unsigned int blink_delay = 0;
unsigned int i;
unsigned char sec = 120;
//initialize and declare the required variables, only 3 attempts are allowed
unsigned char blink_state = 0,flag = 0,attempt_f = 0,attempt = 3,wait_f;

void password(char key)
{
    if (flag<4 && attempt > 0 && wait_f == 0)  //check all the 8 characters are entered and attempt is not 0
    {
        clcd_print("ENTER PASSWORD  ", LINE1(0));  //to print initial message on clcd    
        attempt_f = 0;   //updater attempt flag as 0
        if(key == MK_SW11)  //check switch 1 of matrix keypad is pressed
        {
            input_pass[flag] = '0';  //store 0 in dup array at flag position
            clcd_putch('*',LINE2(flag)); //print '*' to hide and to know character is entered 
            flag++;  //increment flag
        }
        if(key == MK_SW12) //check switch 2 of matrix keypad is pressed
        {
            input_pass[flag] = '1';  //store 0 in dup array at flag position
            clcd_putch('*',LINE2(flag));
            flag++; 
        }
        //logic to blink the '_'  
        if (blink_delay++ > 200)  //adjust this value for blinking speed
        {
            blink_delay = 0;
            blink_state = !blink_state; // toggle the blink state
           
            if (blink_state)
            {
                clcd_print("_               ", LINE2(flag)); // Display the underscore
            }
            else
            {
                clcd_print("                ", LINE2(flag)); // Clear the underscore
            }
        }
    }
    else if(flag == 4)
    {
        //check if entered password is equal to correct password
        pass[0] = read_ext_eeprom(200);
        pass[1] = read_ext_eeprom(201);
        pass[2] = read_ext_eeprom(202);
        pass[3] = read_ext_eeprom(203);
        if(attempt_f == 0)
        for(i = 0; i<4; i++)
        {
            if(pass[i] != input_pass[i])
            {
                attempt_f = 1;  //update attempt flag
                attempt--;      //decrement attempt    
                break;
            }
        }
        //if the correct password is entered print it
        if(attempt_f == 0) 
        {
            clcd_print("CORRECT PASSWORD", LINE2(0));
            attempt = 3;
            flag = 0;
            //attempt_f = 0;
            main_f = MENU;   
        }
        //if wrong password is entered
        else if (attempt<3)
        {
            if(attempt == 2)
            {
                clcd_print("WRONG PASSWORD  ", LINE1(0)); //print wrong password
                clcd_print("2 Attempts left ", LINE2(0));
            }
            else if(attempt == 1)
            {
                clcd_print("WRONG PASSWORD  ", LINE1(0)); //print wrong password
                clcd_print("1 Attempt left  ", LINE2(0));
            }
            //if max attempts is reached print the message and wait for the reset
            if(attempt == 0)
            {
                //on the timer and reset all the flags
                TMR0ON = 1;   
                clcd_print("YOU ARE BLOCKED ", LINE1(0));
                clcd_print("Wait... ", LINE2(0));
                wait_f = 1;  //update wait flag
                sec = 60;
                flag = 0;
            }
            else if(i++ == 500)
            {
                i=0;
                flag = 0;
                CLEAR_DISP_SCREEN;
            }
        }     
    }
    //display blocked message foe 60 sec 
    else if(wait_f == 1)
    {
        //if sec is 0, reset all the flags
        if(sec == 0)
        {
            TMR0ON = 0; //off the timer
            wait_f = 0;
            attempt = 3;
        }
        clcd_putch('0' + sec/10, LINE2(8));
        clcd_putch('0' + sec%10, LINE2(9));
        clcd_print(" sec",LINE2(10));
    }
}
