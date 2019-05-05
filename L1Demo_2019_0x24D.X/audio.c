#include "xc.h"
#include "audio.h"
#include "music.h"
#include "voice.h"

volatile uint8_t audio_mode = AUDIO_MODE;          // Select Audio or Voice Mode
volatile uint8_t audio_track = 0;
volatile uint32_t time_sec = 0;		                   // Demo Master Clock
volatile uint32_t time_subsec = 0;

volatile uint8_t last_audio_track = 0;
volatile unsigned short ch1_val = 0;               // Audio Channel 1
volatile unsigned short ch2_val = 0;               // Audio Channel 2
volatile unsigned short ch3_val = 0;               // Audio Channel 3

void audio_init(void) 
{
    audio_mode = AUDIO_MODE;

    PR1 = 0x100;	 // PR1 Sample Freq (62.5 kHz)
    _T1IP = 5;	     // set interrupt priority
    _TON  = 1;	     // turn on the timer
    _T1IF = 0;	     // reset interrupt flag
    _T1IE = 1;	     // turn on the timer1 interrupt
}

void audio_isr(void)
{
    // Song index
	static unsigned int idx = 0;

	static unsigned short duration = 0;
    static unsigned int ch1_ncount = 0;
    static unsigned int ch2_ncount = 0;
    static unsigned int ch3_ncount = 0;
    
    // Check if we've changed tracks
    if(last_audio_track != audio_track)
    {
        idx = 0;
        duration = 0;
        ch1_ncount = 0;
        ch2_ncount = 0;
        ch3_ncount = 0;
        last_audio_track = audio_track;
    }
    
    if(audio_track == 0)
    {
        // Lincolnshire Poacher Track, Single Channel.
        
        // Do Channel 1 Stuff
        ch1_ncount += song_lp_ch1f[idx] - 1;
        if (ch1_ncount > 0x803f)
        {
            ch1_ncount = 0;
        }

        // Get value for each channel from the sine table
        ch1_val = sinetable[ch1_ncount>>6];

        // Duration
        if(duration < 0x7A1)
        {
            duration++;
        }
        else
        {
            idx++;

            // Loop it!
            if(idx == sizeof(song_lp_ch1f) / sizeof(song_lp_ch1f[0]) ) 
            {
                idx = 0;
            }

            duration = 0;
        }

        // Mix and set audio
        PORTB = (ch1_val<<6);
    }
    else if(audio_track == 2)
    {
        // Unreal Superhero 3
        
        // Do Channel 1 Stuff
        ch1_ncount += song_us3_ch1f[idx] - 1;
        if (ch1_ncount > 0x803f)
        {
            ch1_ncount = 0;
        }

        // Do Channel 2 Stuff
        ch2_ncount += song_us3_ch4f[idx] - 1;
        if (ch2_ncount > 0x803f)
        {
            ch2_ncount = 0;
        }

        // Do Channel 3 Stuff
        ch3_ncount += song_us3_ch6f[idx] - 1;
        if (ch3_ncount > 0x803f)
        {
            ch3_ncount = 0;
        }

        // Get value for each channel from the sine table
        ch1_val = sinetable[ch1_ncount>>6];
        ch2_val = sinetable[ch2_ncount>>6];
        ch3_val = sinetable[ch3_ncount>>6];

        // Duration
        if(duration < 0x7A1)
        {
            duration++;
        }
        else
        {
            idx++;

            // Loop it!
            if(idx == sizeof(song_us3_ch4f) / sizeof(song_us3_ch4f[0]) ) 
            {
                idx = 0;
            }

            duration = 0;
        }

        // Mix and set audio
        PORTB = ((ch1_val<<4) + (ch2_val<<3) + (ch3_val<<2)) << 2;
    }
    else
    {
        PORTB = 0x0;
    }
    
    // Time Keeping
    time_subsec++;

    if(time_subsec >= 0xf424)
    {
        time_sec++;
        time_subsec = 0;
    }

    // Clear Timer Flag
	_T1IF = 0;
}


//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    if(audio_mode == AUDIO_MODE)
    {
        audio_isr();
    }
    else if(audio_mode == VOICE_MODE)
    {
        voice_isr();
    }
}
