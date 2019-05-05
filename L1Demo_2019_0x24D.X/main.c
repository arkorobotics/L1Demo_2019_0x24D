/*
 Event>>  			Layerone Demo Party 2019
 Title>>  			NO TITLE YET
 Group>>			0x24D
 By>>     			Arko & Davo
 Base Libraries>>	Datagram & Hotdogs & Jamis
 ---------------------------------------------------------
 Warning>>			Don't take any of this code seriously
 Danger>>			We're super cereal
 ---------------------------------------------------------
 [ INSERT ASCII ART HERE ]
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "alu.h"
#include "gpu.h"
#include "audio.h"
#include "color.h"
#include "music.h"
#include "scene.h"
#include "sprites.h"
#include "voice.h"
 
#define  FCY    16000000UL                                // Instruction cycle frequency, Hz
#include <libpic30.h>
 
#pragma config FWDTEN = OFF , GWRP = OFF , GCP = OFF , JTAGEN = OFF
#pragma config POSCMOD = HS , FCKSM = CSDCMD , FNOSC = PRIPLL , PLL96MHZ = ON , PLLDIV = DIV2
#pragma config ALTPMP = ALTPMPEN , SOSCSEL = EC

int main(void) 
{
    // Initialize IO
	ANSB = 0x0000;                                        
	ANSC = 0x0000;
	ANSD = 0x0000;
	ANSF = 0x0000;
	ANSG = 0x0000;
	TRISB = 0x0000;

    // Initialize GPU
    gpu_init();
    gpu_set_res(RES_160x480, DOUBLEBUFFERED, BPP_4);

	gpu_config_clut();                                    // Configure Color LookUp Table (CLUT)
	gpu_config_chr();                                     // Configure Font Table
	gpu_clear_all_fb();                                   // Clear the screen
    
    gpu_clut_set(0, 0);                                   // Load CLUT
    gpu_clut_set(1, rgb_2_565(180, 180, 180));

    // Initialize Scene Manager
    scene_init();

    // Draw!
	while (1) 
	{
        scene_render_frame();

        //gpu_draw_border(0);                               // Draw black border (cleans up VGA frame)
        gpu_flip_fb();                                    // Flip buffers at the next Vsync
		frames++;                                         // Increment frame count
    }

	return 0;
}
