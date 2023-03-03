#include <avr/io.h>
#include <util/delay.h>
#ifndef LCD_H_
#define LCD_H_
void LCD_init()
{
	DDRA |= 0xF0;
	PORTA &= (~(0xF0));
	DDRB |= (1<<2);
	PORTB &= (~(1<<2)); // RW = 0
	DDRB |= (1<<0); // LCD_E
	DDRB |= (1<<1);
	LCD_SendCommand(0x02); // 4 Bits
	LCD_SendCommand(0x01); // clear screen
	LCD_SendCommand(0x28); // 2 lines
	LCD_SendCommand(0x0C); // cursor OFF
}
void LCD_SendCommand(unsigned char command)
{
	PORTB &= (~(1<<1)); // instruction RS = 0
	PORTA = (command & 0xF0);
	_delay_ms(1);
	PORTB |= (1<<0);
	_delay_ms(1);
	PORTB &= (~(1<<0));
	_delay_ms(1);
	PORTA = ((command<<4) & 0xF0);
	_delay_ms(1);
	PORTB |= (1<<0);
	_delay_ms(1);
	PORTB &= (~(1<<0));
	_delay_ms(1);
}
void LCD_SendData(unsigned char data)
{
	PORTB |= (1<<1); // data RS = 1
	PORTA = (data & 0xF0);
	_delay_ms(1);
	PORTB |= (1<<0);
	_delay_ms(1);
	PORTB &= (~(1<<0));
	_delay_ms(1);
	PORTA = ((data<<4) & 0xF0);
	_delay_ms(1);
	PORTB |= (1<<0);
	_delay_ms(1);
	PORTB &= (~(1<<0));
	_delay_ms(1);
}
void LCD_SendString(const unsigned char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		LCD_SendData(str[i]); // print on line 1
		i++;
		if(i==16)
		{
			LCD_SendCommand(0xC0); // line 2
		}
		if(i==32)
		{
			LCD_ClearScreen();
			LCD_SendString("String Too Long");
			break;
		}
	}
}
void LCD_GoToRowCol(int row, int col)
{
	switch (row)
	{
	case 1: LCD_SendCommand(0x80 + (col-1));break;
	case 2: LCD_SendCommand(0xC0 + (col-1));break;
	}
}
void LCD_SendStringToRowCol(const unsigned char *str, int row, int col)
{
	LCD_GoToRowCol(row,col);
	LCD_SendString(str);
}
void LCD_ClearScreen()
{
	LCD_SendCommand(0x01);
}



#endif /* LCD_H_ */
