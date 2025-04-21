#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);  //to transmit 1 byte
int puts(const char *s);         //transmit a string
unsigned char getch(void);       //recieve 1 byte
unsigned char getche(void);   //recieve, transmit 1 byte

#endif
