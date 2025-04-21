/*
 * File:  store_event.c
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

extern char time[9];
char store[10];
extern unsigned char speed;
extern unsigned char ev[][3], ev_ind;
int n;
int log;

//logic for storing the events
void store_event() 
{
    //storing the values which is wanted to retrieved 
    store[0] = time[0];
    store[1] = time[1];
    store[2] = time[3];
    store[3] = time[4];
    store[4] = time[6];
    store[5] = time[7];
    store[6] = ev[ev_ind][0];
    store[7] = ev[ev_ind][1];
    store[8] = (speed / 10) + '0';
    store[9] = (speed % 10) + '0';
    
    //logic for shifting the events stored , if the log reached max of 10 values
    if (log == 10) 
    {
        for (log = 0; log < 9; log++) 
        {
            for (n = 0; n < 10; n++) 
            {
                write_ext_eeprom((log * 10) + n, (read_ext_eeprom(((log + 1)*10) + n)));
            }
        }
    }
    //storing the log , if it is less than 10
    for (n = 0; n < 10; n++) 
    {
        write_ext_eeprom(log * 10 + n, store[n]);
        write_ext_eeprom(250,log);
    }
    //storing the log value only max of 10 (0-9)
    if(log < 10)
    log++;
}