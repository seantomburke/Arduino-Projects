/*
 * rgb_strobe.c
 *
 * Distributed under Creative Commons 3.0 -- Attib & Share Alike
 *
 *  Created on: Feb 6, 2010
 *      Author: PaulBo
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
#ifndef F_CPU
    #define F_CPU 8000000UL
#endif
 
//Hardware definitions
#define MAGENTA_LED  PB5
#define YELLOW_LED   PB4
#define CYAN_LED     PB3

#define RED_LED      PB2
#define GREEN_LED    PB1
#define BLUE_LED     PB0
#define ALL_LEDS    ((1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED))
#define BALL_LEDS    ((1 << CYAN_LED) | (1 << YELLOW_LED) | (1 << MAGENTA_LED))
 
//Maximum value for led brightness
#define R_MAX 255
#define G_MAX 255
#define B_MAX 255

#define MAGENTA_INDEX   0
#define YELLOW_INDEX    1
#define CYAN_INDEX      2
 
#define RED_INDEX   0
#define GREEN_INDEX 1
#define BLUE_INDEX  2
 
//Cycle States
#define RedToYellow     0
#define YellowToGreen   1
#define GreenToCyan     2
#define CyanToBlue      3
#define BlueToMagenta   4
#define MagentaToWhite  5
#define WhiteToRed      6
 
//set red to max (we start in the RedToYellow state)
volatile unsigned char mRgbBuffer[] = {0,0,0};
volatile unsigned char mMycBuffer[] = {0,0,0};

unsigned char mRgbValues[] = {255,0,0};
unsigned char mMycValues[] = {255,0,0};
unsigned char mState;
 
void init_timers()
{
    TIMSK0 = (1 << TOIE0);         // enable overflow interrupt
    TCCR0B = (1 << CS00);          // start timer, no prescale
 
    //enable interrupts
    sei();
}
 
void purple(int purple){
  mRgbValues[RED_INDEX] = purple;
  mRgbValues[GREEN_INDEX] = purple;
  mRgbValues[BLUE_INDEX] = purple;
}

void rgbCycle(){
  
    switch (mState) {
    case RedToYellow:
        mRgbValues[GREEN_INDEX]++;
        if (mRgbValues[GREEN_INDEX] == G_MAX)
            mState++;
        break;
    case YellowToGreen:
        mRgbValues[RED_INDEX]--;
        if (mRgbValues[RED_INDEX] == 0)
            mState++;
        break;
    case GreenToCyan:
        mRgbValues[BLUE_INDEX]++;
        if (mRgbValues[BLUE_INDEX] == B_MAX)
            mState++;
        break;
    case CyanToBlue:
        mRgbValues[GREEN_INDEX]--;
        if (mRgbValues[GREEN_INDEX] == 0)
            mState++;
        break;
    case BlueToMagenta:
        mRgbValues[RED_INDEX]++;
        if (mRgbValues[RED_INDEX] == R_MAX)
            mState++;
        break;
    case MagentaToWhite:
        mRgbValues[GREEN_INDEX]++;
         if(mRgbValues[GREEN_INDEX] == G_MAX)
           mState++;
         break;
    case WhiteToRed:
        mRgbValues[BLUE_INDEX]--;
        mRgbValues[GREEN_INDEX]--;
        if (mRgbValues[BLUE_INDEX] == 0)
            mState++;
        break;
    }
 
    //state should never advance beyond 5.
    //It wraps back to 0 when we reach 6
    mState %= 7;
}


char touch_switch (void)
{
    digitalWrite(3,0);    // Discharge the pin
    _delay_ms(1); // Wait
    digitalWrite(3,1);    // Turn on the Constant current source
    _delay_us(5); // Wait for 5 u_seconds
    if (digitalRead(4) == 1)
        return (0);   // No touch detected
    else
        return (1);   // Touch detected
} 
 
int main(void){
    //Set LED pins to output
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, INPUT);
    
    //DDRB |= ALL_LEDS;
    //DDRB |= BALL_LEDS;
    int state = 1;
    
    init_timers();
    while(1)
    {
      
     //int pin3 = analogRead(3);
     //int pin4 = analogRead(4);
     
     //int x = map(pin3, 0, 10, 0, 255);
     //int y = map(pin4, 0, 10, 0, 255);
     if(touch_switch()) 
     {
    //purple(x);
      rgbCycle();
      _delay_ms(5);
     }
      
      
      /*
      for(int i=0; state == 1; i++){
          rgbCycle();
          _delay_ms(5);
      */
          //I like the orange state and it only lasts for a second
          //so lets extend it a little bit more
          /*if(mState == MagentaToWhite)
          {
              _delay_ms(10);
          }*/
      /*    
          if( i > 2000)
          {
            i = 0;
           state = 0; 
          }
      }
      for(int i=0; state == 0; i++)
      {
        purple();
        _delay_ms(5);
        
        if( i > 2000)
          {
           i = 0;
           state = 1; 
          }
      }*/
    }
    return 0;
}
 
/*
 * Timer/Counter overflow interrupt. This is called each time
 * the counter overflows (255 counts/cycles).
 */
ISR(TIM0_OVF_vect)
{
    //static variables maintain state from one call to the next
    static unsigned char sPortBmask = ALL_LEDS;
    static unsigned char sCounter = 255;
 
    //set port pins straight away (no waiting for processing)
    PORTB = sPortBmask;
 
    //this counter will overflow back to 0 after reaching 255.
    //So we end up adjusting the LED states for every 256 interrupts/overflows.
    if(++sCounter == 0)
    {
        mRgbBuffer[RED_INDEX] = mRgbValues[RED_INDEX];
        mRgbBuffer[GREEN_INDEX] = mRgbValues[GREEN_INDEX];
        mRgbBuffer[BLUE_INDEX] = mRgbValues[BLUE_INDEX];
 
        //set all pins to low (remember this is a common anode LED)
        sPortBmask &=~ ALL_LEDS;
    }
    //this loop is considered for every overflow interrupt.
    //this is the software PWM.
    if(mRgbBuffer[RED_INDEX]   == sCounter) sPortBmask |= (1 << RED_LED);
    if(mRgbBuffer[GREEN_INDEX] == sCounter) sPortBmask |= (1 << GREEN_LED);
    if(mRgbBuffer[BLUE_INDEX]  == sCounter) sPortBmask |= (1 << BLUE_LED);
}
