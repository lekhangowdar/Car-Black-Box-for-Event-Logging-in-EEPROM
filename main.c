/*
 * File:  main.c
 * Author: Lekhan Gowda R [ 240008_023 ]
 * Description : Car Black Box
 * Created on 1 October, 2024, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "uart.h"
#include "timer0.h"
#include "isr.h"
#include "matrix_keypad.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "ds1307.h"

unsigned char speed,sp1,sp2,ev_ind;
unsigned short adc_reg_val;
unsigned char ev[][3]={"ON","GR","GN","G1","G2","G3","G4","G5","_C","DL","CL","ST","CP"};
char main_f, key, menu_f, menu_key;
unsigned char clock_reg[3],time[9];
extern unsigned char pass[];
extern int log;

inti_config()
{
    GIE = 1;
    init_matrix_keypad();
    init_clcd();
    init_adc();
    init_i2c();
	init_ds1307();
    init_timer0();
    init_uart();
}

//function to read the time from RTC
void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR); //store hour value, similarly for min ands sec
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);
    time[0] = '0' + ((clock_reg[0] >> 4) & 0x03); //extract 2 bits [4-5]
    time[1] = '0' + (clock_reg[0] & 0x0F);  //extract 4 bits [0-3]
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F); 
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void main(void) 
{
    inti_config();
    
    //read ADC value which act as the speed for displaying
    unsigned char sp_val;
    log = read_ext_eeprom(250);
    get_time();
    store_event();
    while (1)
    {
        adc_reg_val = read_adc(CHANNEL4);
        speed = adc_reg_val/10.33;
        sp1 = 48 + speed/10;
        sp2 = 48 + speed%10;
        get_time();
        
        //read the switches 1, 2 & 3 for gear changes
        key = read_switches(STATE_CHANGE); //read it as state change
        if (key == MK_SW2)
        {
            if(ev_ind<7)
            ev_ind++ ;    
            if(ev_ind == 8)
            ev_ind = 2;
            store_event();  //call store event function for each gear event
        }
        else if(key == MK_SW3)
        {
            if(ev_ind>1 && ev_ind!=8)
            ev_ind-- ;
            if(ev_ind == 8)
            ev_ind = 2;
            store_event();
        }
        else if(key == MK_SW1)
        {
            ev_ind = 8;
            store_event();
        }   
        /*
         * update main_f to perform each main function logic 
         * update menu_f to perform each menu function logic
         */
        if(main_f == DASHBOARD)
        {
            dashboard(); 
        }
        else if(main_f == PASSWORD)
        {
            password(key);
        }
        else if(main_f == MENU)
        {
            menu_key = read_switches(LEVEL_CHANGE); //read the switches for menu 
            menu(menu_key);
        }
        else if(main_f == MENU_ENTER)
        {
            //read the switches for each menu options and call respective functions
            menu_key = read_switches(LEVEL_CHANGE);  
            if(menu_f == VIEWLOG)
            {
                view_log(menu_key);
            }
            else if(menu_f == DOWNLOADLOG)
            {
                download_log();
            }
            else if(menu_f == CLEARLOG)
            {
                clear_log(key);
            }
            else if(menu_f == SETTIME)
            {
                set_time(menu_key);
            }
            else if(menu_f == CHANGEPASS)
            {
                change_pass(key);
            }
        }
    }
    return;
}
