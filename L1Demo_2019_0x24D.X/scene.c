#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include <libpic30.h>
#include "xc.h"
#include "alu.h"
#include "gpu.h"
#include "audio.h"
#include "color.h"
#include "music.h"
#include "scene.h"
#include "sprites.h"
#include "voice.h"

struct Scene scene[TOTAL_NUM_SCENES];

void (*scene_func)(void);

void scene_init(void)
{
    // Configure settings for all scenes

    // Loading screen
    scene[0].scene_id = 0;
    scene[0].start_time = 0;
    scene[0].stop_time = 15;
    scene[0].music_track_id = 0;
    scene[0].res = RES_160x480;
    scene[0].fb_num = SINGLEBUFFERED;
    scene[0].color_depth = BPP_4;

    // Numberstation
    scene[1].scene_id = 1;
    scene[1].start_time = 15;
    scene[1].stop_time = 30;
    scene[1].music_track_id = 1;
    scene[1].res = RES_80x480;
    scene[1].fb_num = SINGLEBUFFERED;
    scene[1].color_depth = BPP_4;

    // Lorenz Attractor
    scene[2].scene_id = 2;
    scene[2].start_time = 30;
    scene[2].stop_time = 40;
    scene[2].music_track_id = 2;
    scene[2].res = RES_160x480;
    scene[2].fb_num = SINGLEBUFFERED;
    scene[2].color_depth = BPP_4;

    // Plasma
    scene[3].scene_id = 3;
    scene[3].start_time = 40;
    scene[3].stop_time = 60;
    scene[3].music_track_id = 2;
    scene[3].res = RES_80x480;
    scene[3].fb_num = DOUBLEBUFFERED;
    scene[3].color_depth = BPP_4;

    // Parachute
    scene[4].scene_id = 4;
    scene[4].start_time = 60;
    scene[4].stop_time = 80;
    scene[4].music_track_id = 2;
    scene[4].res = RES_160x480;
    scene[4].fb_num = DOUBLEBUFFERED;
    scene[4].color_depth = BPP_4;

    // Hackers
    scene[5].scene_id = 5;
    scene[5].start_time = 80;
    scene[5].stop_time = 95;
    scene[5].music_track_id = 2;
    scene[5].res = RES_160x480;
    scene[5].fb_num = SINGLEBUFFERED;
    scene[5].color_depth = BPP_4;

    // Shibe
    scene[6].scene_id = 6;
    scene[6].start_time = 95;
    scene[6].stop_time = 125;
    scene[6].music_track_id = 2;
    scene[6].res = RES_160x480;
    scene[6].fb_num = SINGLEBUFFERED;
    scene[6].color_depth = BPP_4;

    // Credits
    scene[7].scene_id = 7;
    scene[7].start_time = 125;
    scene[7].stop_time = 5500;
    scene[7].music_track_id = 2;
    scene[7].res = RES_160x480;
    scene[7].fb_num = SINGLEBUFFERED;
    scene[7].color_depth = BPP_4;

    // Set the current scene function
    scene_func = &scene_loadscreen;

    // Set the start time
    time_sec = scene[START_SCENE].start_time;

    // Configure the GPU for the start scene
    gpu_set_res(scene[START_SCENE].res, scene[START_SCENE].fb_num, scene[START_SCENE].color_depth);
}

void scene_render_frame(void)
{
    static uint16_t scene_index = START_SCENE;

    // Change scenes when we reach the stop time for the current scene
    if(time_sec >= scene[scene_index].stop_time)
    {
        // Increment scene index
        if(scene_index < TOTAL_NUM_SCENES)
        {
            scene_index++;
        }
        
        // Change Scenes
        switch(scene_index)
        {
            case 0:
                scene_func = &scene_loadscreen;
                break;
            case 1:
                scene_func = &scene_numberstation;
                break;
            case 2:
                scene_func = &scene_lorenz;
                break;
            case 3:
                scene_func = &scene_plasma;
                break;
            case 4:
                scene_func = &scene_parachute;
                break;
            case 5:
                scene_func = &scene_hackers;
                break;
            case 6:
                scene_func = &scene_shibe;
                break;
            case 7:
                scene_func = &scene_credits;
                break;
            default:
                scene_func = &scene_loadscreen;
                break;
        }
        
        // Configure new graphics settings
        if((scene_index != 0) &&
           ( (scene[scene_index].res != scene[scene_index-1].res) ||
             (scene[scene_index].fb_num != scene[scene_index-1].fb_num) ||
             (scene[scene_index].color_depth != scene[scene_index-1].color_depth)
           ))
        {
            gpu_set_res(scene[scene_index].res, scene[scene_index].fb_num, scene[scene_index].color_depth);
        }

        // Update music track here
        audio_track = scene[scene_index].music_track_id;
    }

    scene_func();
}

void scene_loadscreen(void)
{
    static uint8_t init = 0;
    static uint8_t ls_counter = 0;
    static uint8_t w1 = 0;
    static uint8_t w2 = 0;
    static uint8_t w3 = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();
        audio_init();
        gpu_clut_set(0, 0x0);
        gpu_clut_set(1, 0xffff);
        gpu_clut_set(2, 0x37e6);
        gpu_clut_set(3, 0x1542);
        gpu_chr_bg_color(0);
        gpu_chr_fg_color(3);
    }
    else
    {
        ls_counter ++;
        if (ls_counter%25 == 0)
        {
            if (w3 < 38)
            {
                w3 ++;
            }
        }
        if (ls_counter%20 == 0)
        {
            if (w2 < 38)
            {
                w2 ++;
            }
        }
        if (ls_counter%15 == 0)
        {
            if (w1 < 38)
            {
                w1 ++;
            }
        }
    }
    rcc_color(1);
    rcc_rec(5, 455, w1, 15);
    rcc_color(2);
    rcc_rec(5+w1, 455, w2, 15);
    rcc_color(3);
    rcc_rec(5+w1+w2, 455, w3, 15);
    gpu_chr_print("0x24D", 125, 460, 0);
    
    rcc_color(0);
}

void scene_numberstation(void)
{
    static uint8_t init = 0;

    if(init == 0)
    {
        init = 1;
        sprites_load_all();
        sprites_load_clut(0);
        gpu_clear_all_fb();
        sprites_draw(30, 150, 0, 1, 0);
        voice_init();
    }
}

void scene_lorenz(void)
{
    static uint8_t init = 0;

    // Draw a '3D' Lorenz Attractor then make it go rainbow!
    static uint16_t scene_count = 0;
    static uint16_t drawcount = 0;

    static double h = 0.008;
    static double a = 10;
    static double b = 28;
    static double c = 2.66;

    static double x = 0;
    static double y = 10;
    static double z = 10;

    static uint16_t red = 255;
    static uint16_t green = 0;
    static uint16_t blue = 0;

    static double clut_red = 0;
    static double clut_green = 0;
    static double clut_blue = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();
        audio_init();
    }

    if(red > 0 && blue == 0)
    {
        green++;
        red--;
    }
    else if(green > 0)
    {
        green--;
        blue++;
    }
    else if(blue > 0)
    {
        blue--;
        red++;
    }

    clut_red = ((double)red/255);
    clut_green = ((double)green/255);
    clut_blue = ((double)blue/255);

    int i;

    for(i=1; i<15; i++)
    {
        gpu_clut_set(i, rgb_2_565( (uint8_t)(16*i)*(clut_red), (uint8_t)(16*i)*(clut_green), (uint8_t)(16*i)*(clut_blue) ));
    }

    while(drawcount < 100)
    {
        // LORENZ ATTRACTOR   ¯\_(ツ)_/¯
        x+=h*a*(y-x);               
        y+=h*(x*(b-z)-y);          
        z+=h*(x*y-c*z);

        rcc_color( ((uint16_t)(z/4)%15) );

        rcc_pixel((uint16_t)(3*(x+28)),(uint16_t)(5*(y+50)));

        drawcount++;
    }

    drawcount = 0;

    scene_count++;
}

void scene_plasma(void)
{
    static uint8_t init = 0;

    static uint8_t adamschiff = 0;

    if(init == 0)
    {
        init = 1;
        //audio_init();
        gpu_clear_all_fb();

        int i;
        for(i=1; i<15; i++)
        {
            gpu_clut_set(i, rgb_2_565( 0, (uint8_t)(16*i), 0 ));
        }
    }

    int y;
    int x;

    for(y = 1; y < gfx.vres-1; y=y+gfx.hscale)
    {
        for(x = 1; x < gfx.hres-1; x++)
        {
            //the plasma buffer is a sum of sines
            int color = (int)
            (
            (sinetable[(uint8_t)x+adamschiff])
            + (sinetable[(uint8_t)y+adamschiff])
            );

            rcc_color( color % 15 );
            rcc_pixel(x,y);
        }
    }
    adamschiff++;
}

void scene_parachute(void)
{
    //Physics Constants
    static float d2Theta1 = 0;
    static float d2Theta2 = 0;
    static float dTheta1  = 0;
    static float dTheta2  = 0;
    static float Theta1   = 0*(M_PI)/2;
    static float Theta2   = 1*(M_PI)/2;
    static float mu     = 1;
    static float m1     = 10;
    static float m2     = 10;
    static float l1     = 40;
    static float l2     = 40;

    static float X0     = 80;
    static float Y0     = 40;

    static float g      = 9.8;
    static float dt     = 0.05;

    static float X1 = 0;
    static float Y1 = 0;

    static float X2 = 0;
    static float Y2 = 0;

    static uint8_t init = 0;

    if(init == 0)
    {
        init = 1;
        //audio_init();
        //gpu_clear_all_fb();
        gpu_clut_set(0, 0);
        gpu_clut_set(1, rgb_2_565( 255, 255, 255 ));
        gpu_clut_set(2, rgb_2_565( 255, 0, 0 ));
        gpu_clut_set(3, rgb_2_565( 0, 255, 0 ));

        int i;
        for(i=0; i<MAX_PARTICLES; i++)
        {
            p[i].size = 0;
            p[i].posx = 0;
            p[i].posy = 0;
            p[i].speedx = 0;
            p[i].speedx = 0;
            p[i].color = 0;
        }
    }
    else
    {
        float sin_t1 = sin(Theta1);
        float cos_t1 = cos(Theta1);
        float sin_t2 = sin(Theta2);
        float cos_t2 = cos(Theta2);
        float sin_t1_t2 = sin(Theta1-Theta2);
        float cos_t1_t2 = cos(Theta1-Theta2);

        mu        =  1+m1/m2;
        d2Theta1  =  (g*(sin_t2*cos_t1_t2-mu*sin_t1)-(l2*dTheta2*dTheta2+l1*dTheta1*dTheta1*cos_t1_t2)*sin_t1_t2)/(l1*(mu-cos_t1_t2*cos_t1_t2));
        d2Theta2  =  (mu*g*(sin_t1*cos_t1_t2-sin_t2)+(mu*l1*dTheta1*dTheta1+l2*dTheta2*dTheta2*cos_t1_t2)*sin_t1_t2)/(l2*(mu-cos_t1_t2*cos_t1_t2));
        dTheta1   += d2Theta1*dt;                                                           
        dTheta2   += d2Theta2*dt;
        Theta1    += dTheta1*dt;
        Theta2    += dTheta2*dt;

        X1 = (X0+l1*sin_t1);
        Y1 = (Y0+(l1*cos_t1));
 
        X2 = (X1+l2*sin_t2);
        Y2 = (Y1+(l2*cos_t2));     

        gpu_clear_fb();

        rcc_line(X0,Y0*((float)gfx.hscale),X1,Y1*((float)gfx.hscale),1);
        rcc_line(X1,Y1*((float)gfx.hscale),X2,Y2*((float)gfx.hscale),1);
    }


    char buf[255];
    char greets[] = "          GREETS TO CHARLIEX ~ COINE ~ DATAGRAM ~ FSPHIL ~ HOTDOGS ~ JAMIS ~ JBUM ~ JKING ~ MMCA ~ MR1337357 ~ MERLIN ~ MORFIR                                                         ";
    
    static uint8_t i = 1;
    static uint8_t s = 0;
    static int8_t y_trans = 0;
    static uint16_t framers = 0;

    for(i = 1; i < 64; i++)
    {
        buf[0] = greets[i+s];
        buf[1] = '\0';

        gpu_chr_fg_color(1);
        gpu_chr_print(buf,i*6,400,1);
    }

    y_trans++; 

    if(framers%4 == 0)
    {
        s++;
    }

    if(s > 150)
    {
        s = 0;
        i = 1;
        y_trans=0;
    }

    framers++;
}

void scene_hackers(void)
{
    static uint8_t init = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();

        //COMMENT THIS OUT
        //sprites_load_all();
        
        sprites_load_clut(2);

        sprites_draw(20, 100, 2, 1, 0);
    }
}

void scene_shibe(void)
{
    static uint8_t init = 0;
    static int16_t scroll = 0;

    if(init == 0)
    {
        init = 1;
        gpu_clear_all_fb();

        //COMMENT THIS OUT
        //sprites_load_all();
        
        sprites_load_clut(3);

        sprites_draw(scroll, 10, 3, 4, 0);
    }

    if(scroll > -130)
    {
        scroll=scroll-6;
        //gpu_clear_fb();
        sprites_draw(scroll, 10, 3, 4, 0);
    }
}

void scene_credits(void)
{
    static uint8_t init = 0;

    if(init == 0)
    {
        init = 1;

        gpu_clear_all_fb();

        //COMMENT THIS OUT
        //sprites_load_all();
        
        sprites_load_clut(1);

        rcc_color(1);
        gpu_chr_print("0x24D CREW", 20, 60, 0);
        gpu_chr_print("ARKO", 20, 100, 0);
        gpu_chr_print("DAVO", 20, 120, 0);
        gpu_chr_print("PINGUINO", 20, 140, 0);
        gpu_chr_print("NECROFILIAC", 20, 160, 0);
        gpu_chr_print("THE KNIGHT", 20, 180, 0);
        sprites_draw(80, 200, 1, 1, 0);
    }
}



