
/*
 * File:  menu.c
 * Author: Lekhan Gowda R
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"

unsigned char menu_op[5][17]={"  View Log     ","  Download Log ","  Clear Log    ","  Set Time     ","  Change Pswd  "};
unsigned char menu_ind,arrow_st;
extern unsigned char ev_ind, in_flag, tc_flag;
extern char main_f, menu_f;
unsigned int time2,time1; /*varaible to check for short/long press*/

    //logic for menu
void menu(char menu_key)
{
    //display the menu options and star
    clcd_print(menu_op[menu_ind],LINE1(1));
    clcd_print(menu_op[menu_ind+1],LINE2(1));
    //display the star at the specific option for selection 
    if(arrow_st == 0)
    {
        clcd_putch('*',LINE1(0));
        clcd_putch(' ',LINE2(0));
    }
    else
    {
        clcd_putch(' ',LINE1(0));
        clcd_putch('*',LINE2(0));
    }
    //check if key 11 is pressed
    if(menu_key == MK_SW11)
    {
        time1++;    /*increment the time if the key is pressed*/
        if(time1 > 200)  /*check for long press*/
        {
            //go to menu_enter by reseting all flags 
            time1=0;
            main_f = MENU_ENTER;
            CLEAR_DISP_SCREEN;
            tc_flag = 0;
            menu_f = menu_ind + arrow_st; //store the option index in menu_f
        }
    }
    //scroll up logic
    else if(time1 < 200 && time1 != 0) /*check for short press*/
    {
        time1 = 0;  /*reset the time*/
        if(arrow_st == 1)
            arrow_st = 0;
        else if(menu_ind > 0)
            menu_ind--;  
    }
    //check if key 12 is pressed
    if(menu_key == MK_SW12)
    {
        time2++;    /*increment the time if the key is pressed*/
        if(time2 > 200)/*check for long press*/
        {
            //go to dashboard by reseting all flags
            time2=0;
            main_f = DASHBOARD;
            CLEAR_DISP_SCREEN;
        }
    }
    //scroll down logic
    else if(time2 < 200 && time2 != 0) /*check for short press*/
    {
        time2 = 0;  /*reset the time*/
        if(arrow_st == 0)    //">"   = 00111110   //10101011  //01111110 = "->"
            arrow_st = 1;
        else if (menu_ind < 3)
            menu_ind++;  
    }
}
