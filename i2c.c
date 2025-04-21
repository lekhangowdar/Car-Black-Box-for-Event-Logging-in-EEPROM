
#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}

//to check the instruction is executed or not
void i2c_idle(void)
{
	while (!SSPIF);
    //0- instruction execution is not completed
    //1- instruction execution is completed
	SSPIF = 0;
}

void i2c_ack(void)
{
	if (ACKSTAT) //since we are not using acknowledgment, the function is empty 
	{
		/* Do debug print here if required */
	}
}

//to start the communication
void i2c_start(void)
{
	SEN = 1;     //in SSPCON2, start enable bit
	i2c_idle();  //to check communication is started or not
}

//to stop the communication
void i2c_stop(void)
{
	PEN = 1;     //in SSPCON2, stop enable bit
	i2c_idle();  //to check communication is stopped or not
}

//to initiate repeat start
void i2c_rep_start(void)
{
	RSEN = 1;    //in SSPCON2, repeat start enable bit
	i2c_idle();  //to check communication is repeat started or not
}

//to transmit 1 byte of data
void i2c_write(unsigned char data)
{
	SSPBUF = data;   //store 1 byte in buffer register
	i2c_idle();      //to check the data is transmitted or not
}

//to enable receive mode
void i2c_rx_mode(void)
{
	RCEN = 1;    //receive mode enable bit
	i2c_idle();  
}

//to send "not acknowledgment"
void i2c_no_ack(void)
{
	ACKDT = 1;  //ack data bit
	ACKEN = 1;  //ack enable bit
}

//to receive 1 byte of data
unsigned char i2c_read(void)
{
	i2c_rx_mode();  //to enable the receive mode
	i2c_no_ack();   //to sen not ack to RTC

	return SSPBUF;
}