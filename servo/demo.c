/*****************************
 * PWM servo
 *****************************/
     
#include <avr/io.h>
#include <stdint.h>
#define F_CPU 1000000UL //16MHz
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_PWM_TOP 10000UL
#define SERVO_LEFT 500UL
#define SERVO_CENTER 750UL
#define SERVO_RIGHT 1000UL

void PWM_Init(void);
 
uint16_t PWM_Value = 19999 - 2000;
uint8_t pind; 
int main(void)
{
    //DDRA = 0xFF;    
    DDRB = (1<<DDB6)|(1<<DDB5);  
    DDRD = 0x00;
    PORTD = 0xFF;
    sei();  // interrupts
 
    PWM_Init();   
    while(1) {
	pind = PIND;
	if(pind & 0x01) {
	   OCR1A = SERVO_LEFT;
	}
	if(pind & 0x02) {
	   OCR1A = SERVO_RIGHT;
	}
	PWM_Value -= 5 * (~PIND & 0x01);
	PWM_Value += 5 * (~PIND & 0x02);
	//OCR1A = PWM_Value;
	_delay_ms(10);
    }
}
 
void PWM_Init(void)
{            
    TCNT1 = 0;
    TCCR1A = (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << CS10);
    ICR1 = SERVO_PWM_TOP;
    OCR1A = SERVO_CENTER;
    //TIMSK1 = 1<<TOIE1; // overflow interrupt
}
ISR(TIMER1_OVF_vect)
{
    OCR1A = PWM_Value;  
}
