/*****************************************************************************
* | File      	:   OLED_2in23_test.c
* | Author      :   
* | Function    :   
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-03-16
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
******************************************************************************/
#include "EPD_Test.h"
#include "OLED_2in23.h"
#include "ImageData.h"
#include "GUI_Paint.h"

#include "hardware/rtc.h"
#include "time.h"

PAINT_TIME sPaint_time;

int OLED_2in23_test(void)
{
    DEV_Delay_ms(20);
    printf("OELD_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        while(1){
            printf("END\r\n");
        }
    }
    
    /* Init */
    OLED_2in23_Init();
    UBYTE *BlackImage;
    UBYTE *BlackImage1;
    UWORD Imagesize = ((OLED_2in23_WIDTH%8==0)? (OLED_2in23_WIDTH/8): (OLED_2in23_WIDTH/8+1)) * OLED_2in23_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        while(1){
            printf("Failed to apply for black memory...\r\n");
        }
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, OLED_2in23_WIDTH, OLED_2in23_HEIGHT, 0, BLACK);	
    
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    DEV_Delay_ms(20);
    Paint_Clear(BLACK);

char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

datetime_t t = {
    .year = 2022,
    .month  = 4,
    .day    = 28,
    .dotw   = 4,
    .hour   = 16,
    .min    = 13,
    .sec    = 00
};

rtc_init();
rtc_set_datetime(&t);


/*
sPaint_time.Sec =55;
sPaint_time.Min = 59;
sPaint_time.Hour = 23;

int64_t time_clock(){
    sPaint_time.Sec +=1;
    if(sPaint_time.Sec>59)
    {
        sPaint_time.Sec = 0;
        sPaint_time.Min +=1;
    }
    if(sPaint_time.Min>59)
    {
        sPaint_time.Min = 0;
        sPaint_time.Hour +=1;
    }
    if(sPaint_time.Hour >23)
    {
        sPaint_time.Hour = 0;
    }
    return 0;
}
*/
    while(1){     
        //Paint_BmpWindows(0, 2, &D2_12832[0], 128, 32); 
        //Paint_BmpWindows(24, 2, &Bluetooth88[0], 8, 8);
        //Paint_BmpWindows(40, 2, &Msg816[0], 16, 8);
        //Paint_BmpWindows(64, 2, &GPRS88[0], 8, 8);
        //Paint_BmpWindows(90, 2, &Alarm88[0], 8, 8);
        //Paint_BmpWindows(112, 2, &Bat816[0], 16, 8);
        //time_clock();
        rtc_get_datetime(&t);
        datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
        Paint_DrawString_EN(2,2,datetime_str,&Font12,WHITE,BLACK);
        //Paint_DrawTime(5,5,&sPaint_time,&Font24,WHITE,BLACK);
        OLED_2in23_draw_bitmap(0,0,BlackImage,128,32);
        Paint_Clear(BLACK);
        DEV_Delay_ms(992);     
    }
    DEV_Module_Exit();
    return 0;
}
