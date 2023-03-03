#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 1000000// Clock Speed 1MHz
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void PWM(int DutyCycle);
void USART_Init(unsigned int ubrr);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);

void main(void)
{
	LCD_init();
	_delay_ms(50); // delay for LCD in manual
	USART_Init(MYUBRR);
	_delay_ms(1);
	// ****** DC Motor ******
	DDRC |= 0xFF;
	DDRB |= (1<<3);
	DDRD &= (~(1<<4));
	DDRD &= (~(1<<5)); // <--
	DDRD &= (~(1<<0));
	PWM(255); // Start with max. speed
	LCD_SendString("AMIT Learning...");
	unsigned char x;
	while(1)
	{
		x = USART_Receive();
		_delay_ms(1);
		USART_Transmit(x); // for testing if sending the received data correctly or not
		switch(x)
		{
		case 'a':
			PWM(63);
			LCD_ClearScreen();
			LCD_SendString("Speed 25%");
			break;
		case 'b':
			PWM(127);
			LCD_ClearScreen();
			LCD_SendString("Speed 50%");
			break;
		case 'c':
			PWM(255);
			LCD_ClearScreen();
			LCD_SendString("Speed 100%");
			break;
		case '1': // Right forward PC3 // car goes to left
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b00001000;
			LCD_ClearScreen();
			LCD_SendString("Left Forward");
			break;
		case '2': // Right backward PC4
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b00010000;
			LCD_ClearScreen();
			LCD_SendString("Left Backward");
			break;
		case '3': // Left forward PC5 // car goes to right
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b00100000;
			LCD_ClearScreen();
			LCD_SendString("Right Forward");
			break;
		case '4': // Left backward PC6
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b01000000;
			LCD_ClearScreen();
			LCD_SendString("Right Backward");
			break;
		case '5': // Forward PC3+PC5
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b00101000;
			LCD_ClearScreen();
			LCD_SendString("Forward");
			break;
		case '6': // Backward PC4+PC6
			PORTC = 0x00;
			_delay_ms(1000);
			PORTC = 0b01010000;
			LCD_ClearScreen();
			LCD_SendString("Backward");
			break;
		case '7': // STOP
			PORTC = 0x00;
			LCD_ClearScreen();
			LCD_SendString("Stop");
			break;
		}
	}
}
void PWM(int DutyCycle)
{
	DDRB |= (1<<3);
	TCCR0 |= 0x6D;
	OCR0 = DutyCycle;
}
void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable double speed */
	//UCSRA |= (1<<U2X);
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}
unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSRA & (1<<RXC)));
	/* Get and return received data from buffer */
	return UDR;
}
void USART_Transmit(unsigned char data)
{
//	/* Wait for empty transmit buffer */
//	while (!( UCSRA & (1<<UDRE)));
//	/* Put data into buffer, sends the data */
//	UDR = data;
	UDR = data;
	while(!(UCSRA & (1<<TXC))){} // Wait until the transimission is complete TXC = 1
	UCSRA|=(1<<TXC); // Clear the TXC flag
}
