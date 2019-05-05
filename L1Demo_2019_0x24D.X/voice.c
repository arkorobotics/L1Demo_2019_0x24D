#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "audio.h"
#include "voice.h"

// Speech Generation
// Credits to Cornell University
// https://people.ece.cornell.edu/land/courses/ece4760/Speech/index.html

#define voice_zero      0
#define voice_one       1700
#define voice_two       2900
#define voice_three     4000
#define voice_four      5500
#define voice_five      6640
#define voice_six       8000
#define voice_seven     9500
#define voice_eight     11000
#define voice_nine      12100

#define voice_size      13580

#define numberstation_size 18
//init the output indexes
uint16_t outI = 0; 
uint16_t tableI = 0; 
uint16_t tableEnd = 0;
//init the ouptut value
uint8_t lastout = 128; 

//reconstruction differentials 
int16_t PCMvalue[4] = {-78, -16, 16, 78};

uint8_t cycle ;  			//decode phase counter
uint8_t out;				//output values
uint8_t p1, p2, p3, p4;		//hold 4 differentials
uint8_t packed;				//byte containing 4 2-bit values	

uint8_t numberstation_idx = 0;
uint8_t numberstation_msg[numberstation_size] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8 ,9, 7 ,9, 3, 2, 3};

void voice_init(void)
{
    audio_mode = VOICE_MODE;

    PR1 = 0x7D7;	 // PR1 Sample Freq (8 kHz)
    _T1IP = 5;	     // set interrupt priority
    _TON  = 1;	     // turn on the timer
    _T1IF = 0;	     // reset interrupt flag
    _T1IE = 1;	     // turn on the timer1 interrupt
}

void voice_isr(void)
{
	//compute next sample
	cycle = outI%4;
	if (cycle==0)                  //do we need to unpack more data?
	{
        if (tableI<tableEnd)       //end of stored wave?
        {
        	//unpack a table entry
        	packed = DPCMAllDigits[tableI];
            p1 = (packed & 192)>>6;
           	p2 = (packed & 48)>>4;
            p3 = (packed & 12)>>2;
            p4 = (packed & 3); 
            tableI++ ; 
        }   //end unpack table entry
        else
        {
            outI = 0; 

            if(numberstation_idx < numberstation_size)
            {
                switch(numberstation_msg[numberstation_idx])
                {
                    case 0:
                        tableI = voice_zero;
                        tableEnd = voice_one;
                        break;
                    case 1:
                        tableI = voice_one;
                        tableEnd = voice_two;
                        break;
                    case 2:
                        tableI = voice_two;
                        tableEnd = voice_three;
                        break;
                    case 3:
                        tableI = voice_three;
                        tableEnd = voice_four;
                        break;
                    case 4:
                        tableI = voice_four;
                        tableEnd = voice_five;
                        break;
                    case 5:
                        tableI = voice_five;
                        tableEnd = voice_six;
                        break;
                    case 6:
                        tableI = voice_six;
                        tableEnd = voice_seven;
                        break;
                    case 7:
                        tableI = voice_seven;
                        tableEnd = voice_eight;
                        break;
                    case 8:
                        tableI = voice_eight;
                        tableEnd = voice_nine;
                        break;
                    case 9:
                        tableI = voice_nine;
                        tableEnd = voice_size;
                        break;
                }

                numberstation_idx++;
            }
            
            lastout = 128;
        }
        
 	    //compute the output and send to PWM
        out = lastout + PCMvalue[p1] - (lastout>>4) ;  	
 	}
 	else if (cycle==1)
 		out = lastout + PCMvalue[p2] - (lastout>>4) ;
  	else if (cycle==2)
  		out = lastout + PCMvalue[p3] - (lastout>>4) ; 
  	else if (cycle==3) 
  		out = lastout + PCMvalue[p4] - (lastout>>4) ;
	
	//update outputs
	PORTB = 255*out;
    outI++;

    // Time Keeping
    time_subsec++;

    if(time_subsec >= 0x1F40)
    {
        time_sec++;
        time_subsec = 0;
    }

    // Clear Timer Flag
    _T1IF = 0;
}
